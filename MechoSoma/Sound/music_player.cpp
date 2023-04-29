#include "music_player.h"

#include <vector>

#define MA_NO_RESOURCE_MANAGER
#include <miniaudio.h>

#include "audio_file.h"
#include "filesystem.h"
#include "xerrhand.h"

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

struct File {
  StreamDataSource dataSource;
  ma_sound sound;

  explicit File(const std::string &path) : dataSource(path) {}

  ma_sound *get_sound() { return &sound; }
};

struct MusicPlayer::Internal {
  ma_engine_config config;
  ma_engine engine;
  std::unique_ptr<File> file;

  Internal() {
    config = ma_engine_config_init();
    config.sampleRate = 44100;
    config.channels = 2;

    const auto result = ma_engine_init(&config, &engine);
    if (result != MA_SUCCESS) {
      throw std::runtime_error("ma_engine_init");
    }
  }

  ~Internal() {
    stop();

    ma_engine_stop(&engine);
    ma_engine_uninit(&engine);
  }

  void play(const std::string &path, bool looping) {
    stop();

    ma_engine_stop(&engine);
    file = std::make_unique<File>(path);

    const auto result = ma_sound_init_from_data_source(&engine, &file->dataSource, 0, nullptr, &file->sound);
    if (result != MA_SUCCESS) {
      XAssert("ma_sound_init_from_data_source");
    }

    ma_sound_set_looping(file->get_sound(), looping);
    ma_sound_start(file->get_sound());

    ma_engine_start(&engine);
  }

  void stop() {
    if (file) {
      ma_sound_stop(file->get_sound());
    }
  }

  void resume() {
    if (file) {
      ma_sound_start(file->get_sound());
    }
  }
};

MusicPlayer::MusicPlayer() : _internal(std::make_unique<Internal>()) {}

MusicPlayer::~MusicPlayer() {}

bool MusicPlayer::play(const char *filename, bool looping) {
  const auto path = file::normalize_path(filename);
  _internal->play(path, looping);
  _status = Status::play;
  return true;
}

void MusicPlayer::stop() {
  _internal->stop();
  _status = Status::stop;
}

void MusicPlayer::pause() {
  _internal->stop();
  _status = Status::pause;
}

void MusicPlayer::resume() {
  _internal->resume();
  _status = Status::play;
}

MusicPlayer::Status MusicPlayer::getStatus() { return _status; }

int MusicPlayer::getVolume() {
  if (_internal->file) {
    const auto volume = ma_sound_get_volume(_internal->file->get_sound());
    return static_cast<int>(volume * 255.0f);
  }
  return 0;
}

void MusicPlayer::setVolume(int volume) {
  if (_internal->file) {
    // 0..255
    ma_sound_set_volume(_internal->file->get_sound(), static_cast<float>(volume) / 255.0f);
  }
}

int MusicPlayer::getLengthInSamples() {
  if (_internal->file) {
    const auto length = ma_sound_get_time_in_pcm_frames(_internal->file->get_sound());
    return static_cast<int>(length);
  }
  return 0;
}
