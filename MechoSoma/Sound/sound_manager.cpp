#include "sound_manager.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#ifdef __APPLE__
#define MA_NO_RUNTIME_LINKING
#endif

#define MA_NO_RESOURCE_MANAGER
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "audio_file.h"
#include "filesystem.h"
#include "xerrhand.h"
#include "xsound.h"

namespace game::sound
{

struct BufferDataSource final {
  ma_data_source_base base;
  std::vector<float> buffer;
  ma_uint64 total_frames;
  ma_uint32 channels = 0;
  ma_uint32 sampleRate = 0;
  ma_uint64 cursor = 0;

  // Read data here. Output in the same format returned by my_data_source_get_data_format().
  static ma_result read(ma_data_source *dataSource, void *frames, ma_uint64 frameCount, ma_uint64 *framesRead) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    assert(source->total_frames >= source->cursor);

    const auto count = std::min(frameCount, source->total_frames - source->cursor);
    auto begin = source->buffer.data() + source->cursor * 2;
    std::copy(begin, begin + count * 2, static_cast<float *>(frames));

    source->cursor += count;
    *framesRead = count;

    return MA_SUCCESS;
  }

  // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking is not supported.
  static ma_result seek(ma_data_source *dataSource, ma_uint64 frameIndex) {
    auto source = static_cast<BufferDataSource *>(dataSource);
    assert(source->total_frames > frameIndex);

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
    *length = source->total_frames;
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
    while (file.read(buffer)) {}

    channels = file.getChannels();
    if (channels != 2) {
      XAssert("stereo audio expected");
    }
    sampleRate = file.getSampleRate();
    if (sampleRate != 44100) {
      XAssert("44100 sample rate expected");
    }

    total_frames = buffer.size() / 2;
  }

  ~BufferDataSource() { ma_data_source_uninit(&base); }
};

struct StreamDataSource final {
  ma_data_source_base base;
  AudioFile file;
  std::vector<float> buffer;
  ma_uint32 channels = 0;
  ma_uint32 sampleRate = 0;

  // Read data here. Output in the same format returned by my_data_source_get_data_format().
  static ma_result read(ma_data_source *dataSource, void *frames, ma_uint64 frameCount, ma_uint64 *framesRead) {
    auto source = static_cast<StreamDataSource *>(dataSource);
    const auto sampleCount = frameCount * source->channels;

    while (source->buffer.size() < sampleCount) {
      if (!source->file.read(source->buffer)) {
        break;
      }
    }

    const auto count = std::min(sampleCount, static_cast<ma_uint64>(source->buffer.size()));
    auto begin = source->buffer.data();
    std::copy(begin, begin + count, static_cast<float *>(frames));

    source->buffer.erase(source->buffer.begin(), source->buffer.begin() + count);
    *framesRead = count / source->channels;

    return MA_SUCCESS;
  }

  // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking is not supported.
  static ma_result seek(ma_data_source *, ma_uint64) { return MA_NOT_IMPLEMENTED; }

  // Return the format of the data here.
  static ma_result getDataFormat(ma_data_source *dataSource, ma_format *format, ma_uint32 *channels,
                                 ma_uint32 *sampleRate, ma_channel *, size_t) {
    auto source = static_cast<StreamDataSource *>(dataSource);
    *format = ma_format_f32;
    *channels = source->channels;
    *sampleRate = source->sampleRate;
    return MA_SUCCESS;
  }

  // Retrieve the current position of the cursor here. Return MA_NOT_IMPLEMENTED and set cursor to 0
  // if there is no notion of a cursor.
  static ma_result getCursor(ma_data_source *, ma_uint64 *cursor) {
    *cursor = 0;
    return MA_NOT_IMPLEMENTED;
  }

  // Retrieve the length in PCM frames here. Return MA_NOT_IMPLEMENTED and set length to 0 if there is no notion
  // of a length or if the length is unknown.
  static ma_result getLength(ma_data_source *, ma_uint64 *length) {
    *length = 0;
    return MA_NOT_IMPLEMENTED;
  }

  static constexpr ma_data_source_vtable table = {read, seek, getDataFormat, getCursor, getLength};

  explicit StreamDataSource(const std::string &path) : file(path) {
    auto baseConfig = ma_data_source_config_init();
    baseConfig.vtable = &table;

    const auto result = ma_data_source_init(&baseConfig, &base);
    if (result != MA_SUCCESS) {
      XAssert("ma_data_source_init");
    }

    channels = file.getChannels();
    if (channels != 2) {
      XAssert("stereo audio expected");
    }
    sampleRate = file.getSampleRate();
    if (sampleRate != 44100) {
      XAssert("44100 sample rate expected");
    }
  }

  ~StreamDataSource() { ma_data_source_uninit(&base); }
};

struct Sound {
  BufferDataSource dataSource;
  ma_sound sound;

  explicit Sound(const std::string &path) : dataSource(path) {}

  ~Sound() {
    ma_sound_uninit(&sound);
  }

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

  void setFrequency(int frequency) {
    const auto pitch = 1.0f * frequency / getFrequency();
    ma_sound_set_pitch(&sound, pitch);
  }

  int getFrequency() {
    const auto pitch = ma_sound_get_pitch(&sound);
    return static_cast<int>(44100.0f * pitch);
  }

  ma_sound *getObject() { return &sound; }
};

struct Channel {
  Sound *sound = nullptr;
  int priority = 0;
  int cropos = 0;
  int flags = 0;
};

struct MusicFile {
  StreamDataSource dataSource;
  ma_sound sound;

  explicit MusicFile(const std::string &path) : dataSource(path) {}

  ~MusicFile() {
      ma_sound_uninit(&sound);
  }

  ma_sound *get_sound() { return &sound; }
};

class InternalSamplePlayer final : public SamplePlayer {
public:
  InternalSamplePlayer(ma_engine *engine, int channels) : _engine(engine), _channels(channels) {}

  void *loadSound(const char *filename) override {
    const auto path = file::normalize_path(filename);
    _sounds.push_back(std::make_unique<Sound>(path));
    auto &sound = _sounds.back();

    const auto result = ma_sound_init_from_data_source(_engine, &sound->dataSource, 0, nullptr, &sound->sound);
    if (result != MA_SUCCESS) {
      XAssert("ma_sound_init_from_data_source");
    }

    return sound.get();
  }

  void playSound(void *soundAddress, int channelIndex, int priority, int cropos, int flags) override {
    assert(soundAddress != nullptr);
    assert(channelIndex < _channels.size());

    if (flags & DS_STREAM || flags & DS_QUEUE) {
      // don't support stream and queue
      return;
    }

    auto sound = static_cast<Sound *>(soundAddress);
    auto &channel = _channels[channelIndex];

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

  void releaseSound(void *soundAddress) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    auto found = std::find_if(_sounds.begin(), _sounds.end(), [sound](auto &other) { return other.get() == sound; });
    if (found != _sounds.end()) {
      _sounds.erase(found);
    } else {
      XAssert("unknown sound address");
    }
  }

  void setVolume(void *soundAddress, int volume) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    sound->setVolume(volume);
  }

  int getVolume(void *soundAddress) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    return sound->getVolume();
  }

  void setSoundFrequency(void *soundAddress, int frequency) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    sound->setFrequency(frequency);
  }

  int getSoundFrequency(void *soundAddress) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    return sound->getFrequency();
  }

  int getSoundStatus(void *soundAddress) override {
    assert(soundAddress != nullptr);

    auto sound = static_cast<Sound *>(soundAddress);
    return ma_sound_is_playing(sound->getObject()) ? 1 : 0;
  }

  void *getSound(int channelIndex) override {
    assert(channelIndex < _channels.size());

    return _channels[channelIndex].sound;
  }

  void setSoundVolume(int channelIndex, int volume) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    if (channel.sound) {
      channel.sound->setVolume(volume);
    }
  }

  int getSoundVolume(int channelIndex) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    return channel.sound ? channel.sound->getVolume() : 0;
  }

  void setGlobalVolume(int volume) override {
    for (int channelIndex = 0; channelIndex < _channels.size(); channelIndex++) {
      setSoundVolume(channelIndex, volume);
    }
  }

  void setSoundPan(int channelIndex, int panning) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    if (channel.sound) {
      // value is measured in hundredths of a decibel (dB), in the range of -10000 to 10000
      ma_sound_set_pan(channel.sound->getObject(), static_cast<float>(panning) * 0.0001f);
    }
  }

  void stopSound(int channelIndex) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    if (channel.sound) {
      ma_sound_stop(channel.sound->getObject());
      ma_sound_seek_to_pcm_frame(channel.sound->getObject(), 0);

      channel.sound = nullptr;
      channel.priority = 0;
      channel.flags = 0;
      channel.cropos = 0;
    }
  }

  int getSoundStatus(int channelIndex) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    if (channel.sound) {
      return getSoundStatus(channel.sound);
    }
    return 0;
  }

  int getChannelStatus(int channelIndex) override {
    assert(channelIndex < _channels.size());

    auto &channel = _channels[channelIndex];
    return channel.sound ? channel.priority : -1;
  }

  void quant() {
    for (auto &channel : _channels) {
      if (channel.sound != nullptr && !getSoundStatus(channel.sound)) {
        channel.sound = nullptr;
        channel.priority = 0;
        channel.flags = 0;
        channel.cropos = 0;
      }
    }
  }

private:
  ma_engine *_engine = nullptr;
  std::vector<std::unique_ptr<Sound>> _sounds;
  std::vector<Channel> _channels;
};

class InternalMusicPlayer final : public MusicPlayer {
public:
  explicit InternalMusicPlayer(ma_engine *engine) : _engine(engine) {}

  bool play(const char *filename, bool looping) override {
    stop();

    const auto path = file::normalize_path(filename);
    _file = std::make_unique<MusicFile>(path);

    const auto result = ma_sound_init_from_data_source(_engine, &_file->dataSource, 0, nullptr, &_file->sound);
    if (result != MA_SUCCESS) {
      XAssert("ma_sound_init_from_data_source");
    }

    ma_sound_set_looping(_file->get_sound(), looping);
    ma_sound_start(_file->get_sound());
    _status = Status::play;
    return true;
  }

  void stop() override {
    if (_file) {
      ma_sound_stop(_file->get_sound());
    }
    _status = Status::stop;
  }

  void pause() override {
    if (_file) {
      ma_sound_stop(_file->get_sound());
    }
    _status = Status::pause;
  }

  void resume() override {
    if (_file) {
      ma_sound_start(_file->get_sound());
    }
    _status = Status::play;
  }

  Status getStatus() override { return _status; }

  void setVolume(int volume) override {
    if (_file) {
      // 0..255
      ma_sound_set_volume(_file->get_sound(), static_cast<float>(volume) / 255.0f);
    }
  }

  int getLengthInSamples() override {
    if (_file) {
      return _file->dataSource.file.getLengthInSamples();
    }
    return 0;
  }

private:
  ma_engine *_engine = nullptr;
  Status _status = Status::stop;
  std::unique_ptr<MusicFile> _file;
};

struct SoundManager::Internal {
  ma_engine_config config;
  ma_engine engine;
  std::unique_ptr<InternalSamplePlayer> samplerPlayer;
  std::unique_ptr<InternalMusicPlayer> musicPlayer;

  Internal(int sampleRate, int channels) {
    config = ma_engine_config_init();
    config.sampleRate = static_cast<ma_int32>(sampleRate);
    config.channels = 2;

    const auto result = ma_engine_init(&config, &engine);
    if (result != MA_SUCCESS) {
      XAssert("ma_engine_init");
    }

    samplerPlayer = std::make_unique<InternalSamplePlayer>(&engine, channels);
    musicPlayer = std::make_unique<InternalMusicPlayer>(&engine);
  }

  ~Internal() {
    ma_engine_stop(&engine);
    ma_engine_uninit(&engine);
  }
};

}

using namespace game::sound;

SoundManager::SoundManager(int sampleRate, int channels)
    : _internal(std::make_unique<Internal>(sampleRate, channels)) {}

SoundManager::~SoundManager() {}

game::sound::SamplePlayer &SoundManager::samplePlayer() {
  return *_internal->samplerPlayer;
}

game::sound::MusicPlayer &SoundManager::musicPlayer() {
  return *_internal->musicPlayer;
}

void SoundManager::quant() {
  _internal->samplerPlayer->quant();
}
