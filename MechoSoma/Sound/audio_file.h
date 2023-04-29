#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class AudioFile final {
 public:
  explicit AudioFile(const std::filesystem::path &path);
  ~AudioFile();

  bool read(std::vector<float> &buffer);

  int getSampleRate() const;
  int getChannels() const;

 private:
  struct Internal;
  std::unique_ptr<Internal> _internal;
};
