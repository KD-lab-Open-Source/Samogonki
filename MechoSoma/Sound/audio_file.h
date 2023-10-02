#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class AudioFile final {
 public:
  explicit AudioFile(const std::filesystem::path &path);
  ~AudioFile();

  void seekToStart();
  bool read(std::vector<float> &buffer);

  int getSampleRate() const;
  int getChannels() const;
  int getLengthInSamples() const;

 private:
  struct Internal;
  std::unique_ptr<Internal> _internal;
};
