#pragma once

#include <memory>

namespace game::sound
{

struct SamplePlayer
{
  virtual void *loadSound(const char *filename) = 0;
  virtual void playSound(void *soundAddress, int channelIndex, int priority, int cropos, int flags) = 0;
  virtual void releaseSound(void *soundAddress) = 0;
  virtual void setVolume(void *soundAddress, int volume) = 0;
  virtual int getVolume(void *soundAddress) = 0;
  virtual int getSoundStatus(void *soundAddress) = 0;

  virtual void *getSound(int channelIndex) = 0;
  virtual void setSoundVolume(int channelIndex, int volume) = 0;
  virtual int getSoundVolume(int channelIndex) = 0;
  virtual void setGlobalVolume(int volume) = 0;
  virtual void setSoundPan(int channelIndex, int panning) = 0;
  virtual void stopSound(int channelIndex) = 0;
  virtual int getSoundStatus(int channelIndex) = 0;
  virtual int getChannelStatus(int channelIndex) = 0;
};

struct MusicPlayer {
  virtual bool play(const char *filename, bool looping) = 0;
  virtual void stop() = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;

  enum class Status { stop, play, pause };
  virtual Status getStatus() = 0;

  virtual void setVolume(int volume) = 0;
  virtual int getLengthInSamples() = 0;
};

class SoundManager final {
 public:
  SoundManager(int sampleRate, int channels);
  ~SoundManager();

  SamplePlayer &samplePlayer();
  MusicPlayer &musicPlayer();

 private:
  struct Internal;
  std::unique_ptr<Internal> _internal;
};

}
