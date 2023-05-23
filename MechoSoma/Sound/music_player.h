#pragma once

#include <memory>

class MusicPlayer final {
 public:
  MusicPlayer();
  ~MusicPlayer();

  bool play(const char *filename, bool looping);
  void stop();
  void pause();
  void resume();

  enum class Status { stop, play, pause };
  Status getStatus();

  void setVolume(int volume);
  int getLengthInSamples();

 private:
  struct Internal;
  std::unique_ptr<Internal> _internal;
  Status _status = Status::stop;
};
