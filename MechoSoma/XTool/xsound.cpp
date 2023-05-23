#include "xsound.h"

#include <cassert>

#include "music_player.h"
#include "sound_manager.h"

std::unique_ptr<MusicPlayer> musicPlayer;
std::unique_ptr<SoundManager> soundManager;

int SoundInit(int maxHZ, int digMode, int channels) {
  if (digMode != DIG_F_STEREO_16) {
    return 0;
  }

  soundManager = std::make_unique<SoundManager>(maxHZ, channels);
  musicPlayer = std::make_unique<MusicPlayer>();

  return 1;
}

void SoundPlay(void *lpDSB, int channel, int priority, int cropos, int flags) {
  if (soundManager) {
    soundManager->playSound(lpDSB, channel, priority, cropos, flags);
  }
}

void SoundRelease(void *lpDSB) {
  if (soundManager) {
    soundManager->releaseSound(lpDSB);
  }
}

void SoundStop(int channel) {
  if (soundManager) {
    soundManager->stopSound(channel);
  }
}

void *GetSound(int channel) { return soundManager ? soundManager->getSound(channel) : nullptr; }

void SoundLoad(char *filename, void **lpDSB) {
  assert(soundManager != nullptr);
  *lpDSB = soundManager->loadSound(filename);
}

void SoundFinit() { soundManager = nullptr; }

void SoundVolume(int channel, int volume) {
  if (soundManager) {
    soundManager->setSoundVolume(channel, volume);
  }
}

void SetVolume(void *lpDSB, int volume) {
  if (soundManager) {
    soundManager->setVolume(lpDSB, volume);
  }
}

int GetVolume(void *lpDSB) {
  if (soundManager) {
    soundManager->getVolume(lpDSB);
  }
  return 0;
}

int GetSoundVolume(int channel) {
  if (soundManager) {
    return soundManager->getSoundVolume(channel);
  }
  return 0;
}

void GlobalVolume(int volume) {
  if (soundManager) {
    soundManager->setGlobalVolume(volume);
  }
}

void SoundPan(int channel, int panning) {
  if (soundManager) {
    soundManager->setSoundPan(channel, panning);
  }
}

int SoundStatus(int channel) { return soundManager ? soundManager->getSoundStatus(channel) : 0; }

int SoundStatus(void *lpDSB) { return soundManager ? soundManager->getSoundStatus(lpDSB) : 0; }

int ChannelStatus(int channel) { return soundManager ? soundManager->getChannelStatus(channel) : -1; }

int GetSoundFrequency(void *lpDSB) { return 0; }

void SetSoundFrequency(void *lpDSB, int frq) {}

void SoundStreamOpen(char *filename, void **strptr) {}

void SoundStreamClose(void *stream) {}

void SoundStreamRelease(void *stream) {}

void xsInitCD() {}

void xsMixerOpen() {}

bool PlayMusic(const char *filename, bool looping) {
  return musicPlayer ? musicPlayer->play(filename, looping) : false;
}

void StopMusic() {
  if (musicPlayer) {
    musicPlayer->stop();
  }
}

void PauseMusic() {
  if (musicPlayer) {
    musicPlayer->pause();
  }
}

void ResumeMusic() {
  if (musicPlayer) {
    musicPlayer->resume();
  }
}

int GetMusicStatus() {
  if (musicPlayer) {
    switch (musicPlayer->getStatus()) {
      case MusicPlayer::Status::play:
        return XCD_PLAYING;

      case MusicPlayer::Status::pause:
        return XCD_PAUSED;

      case MusicPlayer::Status::stop:
        return XCD_STOPPED;
    }
  }
  return XCD_STOPPED;
}

void SetMusicVolume(int volume) {
  if (musicPlayer) {
    musicPlayer->setVolume(volume);
  }
}

int GetMusicLengthInSamples() { return musicPlayer ? musicPlayer->getLengthInSamples() : 0; }
