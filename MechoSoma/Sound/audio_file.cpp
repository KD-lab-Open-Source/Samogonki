#include "audio_file.h"

#include <algorithm>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include "xerrhand.h"

struct AudioFile::Internal {
  AVFormatContext *context = nullptr;
  AVCodecContext *codecContext = nullptr;
  AVPacket packet;
  AVFrame *frame = nullptr;
  unsigned int streamIndex = 0;

  explicit Internal(const std::filesystem::path &path) : context(avformat_alloc_context()) {
    auto codec = getCodec(path);
    if (codec == nullptr) {
      XAssert("audio codec not found");
    }

    codecContext = avcodec_alloc_context3(codec);
    if (codecContext == nullptr) {
      XAssert("avcodec_alloc_context3");
    }

    {
      const auto result = avcodec_parameters_to_context(codecContext, context->streams[streamIndex]->codecpar);
      if (result < 0) {
        XAssert("avcodec_parameters_to_context");
      }
    }

    const auto result = avcodec_open2(codecContext, codec, nullptr);
    if (result < 0) {
      XAssert("avcodec_open2");
    }

    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    frame = av_frame_alloc();
    if (frame == nullptr) {
      XAssert("av_frame_alloc");
    }
  }

  ~Internal() {
    if (frame != nullptr) {
      av_frame_free(&frame);
    }

    if (codecContext != nullptr) {
      avcodec_free_context(&codecContext);
    }

    if (context != nullptr) {
      avformat_close_input(&context);
    }
  }

  AVCodec *getCodec(const std::filesystem::path &path) {
    auto input_format = Internal::getFormat(path.extension());
    const auto result = avformat_open_input(&context, path.string().c_str(), input_format, nullptr);
    if (result < 0) {
      XAssert("avformat_open_input");
    }

    streamIndex = getAudioStreamIndex();
    const auto codec_id = context->streams[streamIndex]->codecpar->codec_id;
    if (codec_id != AV_CODEC_ID_PCM_S16LE && codec_id != AV_CODEC_ID_MUSEPACK7) {
      XAssert("unsupported codec");
    }
    return avcodec_find_decoder(codec_id);
  }

  static AVInputFormat *getFormat(const std::filesystem::path &extension) {
    if (extension == ".wav") {
      return av_find_input_format("wav");
    }

    if (extension == ".mp+") {
      return av_find_input_format("mpc");
    }

    XAssert("unsupported file extension");
    return nullptr;
  }

  unsigned int getAudioStreamIndex() {
    const auto result = avformat_find_stream_info(context, nullptr);
    if (result < 0) {
      XAssert("avformat_find_stream_info");
    }

    for (unsigned int i = 0; i < context->nb_streams; i++) {
      if (context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
        return i;
      }
    }

    XAssert("audio stream not found");
    return 0;
  }

  void decodePacket(std::vector<float> &buffer) {
    const auto result = avcodec_send_packet(codecContext, &packet);
    if (result < 0) {
      XAssert("avcodec_send_packet");
    }

    while (true) {
      const auto received = avcodec_receive_frame(codecContext, frame);
      if (received == AVERROR(EAGAIN) || received == AVERROR_EOF) {
        return;
      } else if (received < 0) {
        XAssert("avcodec_receive_frame");
      }

      if (frame->format == AV_SAMPLE_FMT_S16) {
        const auto p = reinterpret_cast<const int16_t *>(frame->extended_data[0]);
        for (size_t i = 0; i < frame->nb_samples * 2; i++) {
          const auto value = static_cast<float>(p[i]) / std::numeric_limits<int16_t>::max();
          buffer.push_back(value);
        }
        return;
      }

      if (frame->format == AV_SAMPLE_FMT_S16P) {
        for (size_t i = 0; i < frame->nb_samples; i++) {
          for (int ch = 0; ch < 2; ch++) {
            const auto p = reinterpret_cast<const int16_t *>(frame->data[ch]);
            const auto value = static_cast<float>(p[i]) / std::numeric_limits<int16_t>::max();
            buffer.push_back(value);
          }
        }
        return;
      }

      XAssert("unsupported sample format");
    }
  }

  void seekToStart() {
    const auto result = av_seek_frame(context, streamIndex, 0, AVSEEK_FLAG_FRAME);
    if (result < 0) {
      XAssert("av_seek_frame");
    }
  }

  bool read(std::vector<float> &buffer) {
    if (av_read_frame(context, &packet) >= 0) {
      decodePacket(buffer);
      return true;
    }
    return false;
  }

  int getSampleRate() const { return context->streams[streamIndex]->codecpar->sample_rate; }

  int getChannels() const { return context->streams[streamIndex]->codecpar->channels; }

  int getLengthInSamples() const { return static_cast<int>(context->duration) / AV_TIME_BASE * getSampleRate(); }
};

AudioFile::AudioFile(const std::filesystem::path &path) : _internal(std::make_unique<Internal>(path)) {}

AudioFile::~AudioFile() {}

void AudioFile::seekToStart() { _internal->seekToStart(); }

bool AudioFile::read(std::vector<float> &buffer) { return _internal->read(buffer); }

int AudioFile::getSampleRate() const { return _internal->getSampleRate(); }

int AudioFile::getChannels() const { return _internal->getChannels(); }

int AudioFile::getLengthInSamples() const { return _internal->getLengthInSamples(); }
