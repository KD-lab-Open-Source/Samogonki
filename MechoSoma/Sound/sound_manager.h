#pragma once

#include <memory>

class SoundManager final {
 public:
  SoundManager(int sampleRate, int channels);
  ~SoundManager();

  void *loadSound(const char *filename);
  void playSound(void *soundAddress, int channelIndex, int priority, int cropos, int flags);
  void releaseSound(void *soundAddress);
  void setVolume(void *soundAddress, int volume);
  int getVolume(void *soundAddress);
  int getSoundStatus(void *soundAddress);

  void *getSound(int channelIndex);
  void setSoundVolume(int channelIndex, int volume);
  int getSoundVolume(int channelIndex);
  void setGlobalVolume(int volume);
  void setSoundPan(int channelIndex, int panning);
  void stopSound(int channelIndex);
  int getSoundStatus(int channelIndex);
  int getChannelStatus(int channelIndex);

 private:
  struct Internal;
  std::unique_ptr<Internal> _internal;
};
