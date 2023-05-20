#include "sound_manager.h"

#include <algorithm>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#define MA_NO_RESOURCE_MANAGER
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "audio_file.h"
#include "filesystem.h"
#include "xerrhand.h"
#include "xsound.h"

struct BufferDataSource final {
  ma_data_source_base base;
  std::vector<float> buffer;
  ma_uint32 channels = 0;
  ma_uint32 sampleRate = 0;
  ma_uint64 cursor = 0;

  // Read data here. Output in the same format returned by my_data_source_get_data_format().
  static ma_result read(ma_data_source *dataSource, void *frames, ma_uint64 frameCount, ma_uint64 *framesRead) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    assert(source->buffer.size() >= source->cursor);

    const auto count = std::min(frameCount, source->buffer.size() - source->cursor);
    auto begin = source->buffer.data() + source->cursor;
    std::copy(begin, begin + count, static_cast<float *>(frames));

    source->cursor += count;
    *framesRead = count;

    return MA_SUCCESS;
  }

  // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking is not supported.
  static ma_result seek(ma_data_source *dataSource, ma_uint64 frameIndex) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    assert(source->buffer.size() > frameIndex);

    source->cursor = frameIndex;
    return MA_SUCCESS;
  }

  // Return the format of the data here.
  static ma_result getDataFormat(ma_data_source *dataSource, ma_format *format, ma_uint32 *channels,
                                 ma_uint32 *sampleRate, ma_channel *, size_t) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    *format = ma_format_f32;
    *channels = source->channels;
    *sampleRate = source->sampleRate;
    return MA_SUCCESS;
  }

  // Retrieve the current position of the cursor here. Return MA_NOT_IMPLEMENTED and set cursor to 0
  // if there is no notion of a cursor.
  static ma_result getCursor(ma_data_source *dataSource, ma_uint64 *cursor) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    *cursor = source->cursor;
    return MA_SUCCESS;
  }

  // Retrieve the length in PCM frames here. Return MA_NOT_IMPLEMENTED and set length to 0 if there is no notion
  // of a length or if the length is unknown.
  static ma_result getLength(ma_data_source *dataSource, ma_uint64 *length) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    *length = source->buffer.size();
    return MA_SUCCESS;
  }

  static constexpr ma_data_source_vtable table = {read, seek, getDataFormat, getCursor, getLength};

  explicit BufferDataSource(const std::string &path) {
    auto baseConfig = ma_data_source_config_init();
    baseConfig.vtable = &table;

    const auto result = ma_data_source_init(&baseConfig, &base);
    if (result != MA_SUCCESS) {
      XAssert("ma_data_source_init");
    }

    AudioFile file(path);
    while (file.read(buffer)) {
    }

    channels = file.getChannels();
    if (channels != 1) {
      XAssert("moni audio expected");
    }
    sampleRate = file.getSampleRate();
    if (sampleRate != 22050) {
      XAssert("22050 sample rate expected");
    }
  }

  ~BufferDataSource() { ma_data_source_uninit(&base); }
};

struct Sound {
  BufferDataSource dataSource;
  ma_sound sound;

  explicit Sound(const std::string &path) : dataSource(path) {}

  void setVolume(int volume) {
    // value is measured in hundredths of a decibel (dB), in the range of -10000 to 10000
    auto gain = static_cast<float>(volume) + 10000.0f;
    gain *= 0.0001f;
    ma_sound_set_volume(&sound, gain);
  }

  int getVolume() {
    auto gain = ma_sound_get_volume(&sound);
    gain *= 10000.0f;
    return static_cast<int>(gain - 10000.0f);
  }

  ma_sound *getObject() { return &sound; }
};

struct Channel {
  Sound *sound = nullptr;
  int priority = 0;
  int cropos = 0;
  int flags = 0;
};

struct SoundManager::Internal {
  ma_engine_config config;
  ma_engine engine;
  std::vector<std::unique_ptr<Sound>> sounds;
  std::vector<Channel> channels;

  Internal(int sampleRate, int channels) : channels(channels) {
    config = ma_engine_config_init();
    config.sampleRate = static_cast<ma_int32>(sampleRate);
    config.channels = 1;

    const auto result = ma_engine_init(&config, &engine);
    if (result != MA_SUCCESS) {
      XAssert("ma_engine_init");
    }
  }

  ~Internal() {
    ma_engine_stop(&engine);
    ma_engine_uninit(&engine);
  }

  Sound *loadSound(const char *filename) {
    const auto path = file::normalize_path(filename);
    sounds.push_back(std::make_unique<Sound>(path));
    auto &sound = sounds.back();

    const auto result = ma_sound_init_from_data_source(&engine, &sound->dataSource, 0, nullptr, &sound->sound);
    if (result != MA_SUCCESS) {
      XAssert("ma_sound_init_from_data_source");
    }

    return sound.get();
  }
};

SoundManager::SoundManager(int sampleRate, int channels)
    : _internal(std::make_unique<Internal>(sampleRate, channels)) {}

SoundManager::~SoundManager() {}

void *SoundManager::loadSound(const char *filename) { return _internal->loadSound(filename); }

void SoundManager::playSound(void *soundAddress, int channelIndex, int priority, int cropos, int flags) {
  assert(soundAddress != nullptr);
  assert(channelIndex < _internal->channels.size());

  if (flags & DS_STREAM || flags & DS_QUEUE) {
    // don't support stream and queue
    return;
  }

  auto sound = static_cast<Sound *>(soundAddress);
  auto &channel = _internal->channels[channelIndex];

  ma_uint64 currentPlayCursor = 0;
  ma_sound_get_cursor_in_pcm_frames(sound->getObject(), &currentPlayCursor);

  if (!channel.sound || !priority || channel.priority > priority ||
      (channel.sound == sound && (!channel.cropos || (channel.cropos < currentPlayCursor)))) {
    if (channel.sound) {
      stopSound(channelIndex);
    }

    if (cropos) {
      ma_uint64 length = 0;
      ma_sound_get_length_in_pcm_frames(sound->getObject(), &length);
      channel.cropos = static_cast<int>(length) * cropos / 255;
    } else {
      channel.cropos = 0;
    }

    channel.priority = priority;
    channel.sound = sound;
    channel.flags = flags;

    ma_sound_set_looping(sound->getObject(), (flags & DS_LOOPING) == 1);
    ma_sound_start(sound->getObject());
  }
}

void SoundManager::releaseSound(void *soundAddress) {
  assert(soundAddress != nullptr);

  auto sound = static_cast<Sound *>(soundAddress);
  auto found = std::find_if(_internal->sounds.begin(), _internal->sounds.end(),
                            [sound](auto &other) { return other.get() == sound; });
  if (found != _internal->sounds.end()) {
    _internal->sounds.erase(found);
  } else {
    XAssert("unknown sound address");
  }
}

void SoundManager::setVolume(void *soundAddress, int volume) {
  assert(soundAddress != nullptr);

  auto sound = static_cast<Sound *>(soundAddress);
  sound->setVolume(volume);
}

int SoundManager::getVolume(void *soundAddress) {
  assert(soundAddress != nullptr);

  auto sound = static_cast<Sound *>(soundAddress);
  return sound->getVolume();
}

int SoundManager::getSoundStatus(void *soundAddress) {
  assert(soundAddress != nullptr);

  auto sound = static_cast<Sound *>(soundAddress);
  return ma_sound_is_playing(sound->getObject()) ? 1 : 0;
}

void *SoundManager::getSound(int channelIndex) {
  assert(channelIndex < _internal->channels.size());

  return _internal->channels[channelIndex].sound;
}

void SoundManager::setSoundVolume(int channelIndex, int volume) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  if (channel.sound) {
    channel.sound->setVolume(volume);
  }
}

int SoundManager::getSoundVolume(int channelIndex) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  return channel.sound ? channel.sound->getVolume() : 0;
}

void SoundManager::setGlobalVolume(int volume) {
  for (int channelIndex = 0; channelIndex < _internal->channels.size(); channelIndex++) {
    setSoundVolume(channelIndex, volume);
  }
}

void SoundManager::setSoundPan(int channelIndex, int panning) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  if (channel.sound) {
    // value is measured in hundredths of a decibel (dB), in the range of -10000 to 10000
    ma_sound_set_pan(channel.sound->getObject(), static_cast<float>(panning) * 0.0001f);
  }
}

void SoundManager::stopSound(int channelIndex) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  if (channel.sound) {
    ma_sound_stop(channel.sound->getObject());
    ma_sound_seek_to_pcm_frame(channel.sound->getObject(), 0);

    channel.sound = nullptr;
    channel.priority = 0;
    channel.flags = 0;
    channel.cropos = 0;
  }
}

int SoundManager::getSoundStatus(int channelIndex) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  if (channel.sound) {
    return getSoundStatus(channel.sound);
  }
  return 0;
}

int SoundManager::getChannelStatus(int channelIndex) {
  assert(channelIndex < _internal->channels.size());

  auto &channel = _internal->channels[channelIndex];
  return channel.sound ? channel.priority : -1;
}
