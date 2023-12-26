#include "xsound.h"

#include <cassert>

#include "sound_manager.h"
#include "xtool.h"

void xtRegisterSysFinitFnc(void (*fPtr)(void), int id);
void xtUnRegisterSysFinitFnc(int id);
void xtDeactivateSysFinitFnc(int id);

void XSoundQuantFnc();

std::unique_ptr<game::sound::SoundManager> soundManager;

int SoundInit(int maxHZ, int digMode, int channels) {
  if (digMode != DIG_F_STEREO_16) {
    return 0;
  }

  soundManager = std::make_unique<game::sound::SoundManager>(44100, channels);

	xtUnRegisterSysFinitFnc(XSOUND_SYSOBJ_ID);
	xtRegisterSysFinitFnc(SoundFinit, XSOUND_SYSOBJ_ID);
	xtRegisterSysQuant(XSoundQuantFnc, XSOUND_SYSOBJ_ID);

  return 1;
}

void SoundPlay(void *lpDSB, int channel, int priority, int cropos, int flags) {
  if (soundManager) {
    soundManager->samplePlayer().playSound(lpDSB, channel, priority, cropos, flags);
  }
}

void SoundRelease(void *lpDSB) {
  if (soundManager) {
    soundManager->samplePlayer().releaseSound(lpDSB);
  }
}

void SoundStop(int channel) {
  if (soundManager) {
    soundManager->samplePlayer().stopSound(channel);
  }
}

void *GetSound(int channel) { return soundManager ? soundManager->samplePlayer().getSound(channel) : nullptr; }

void SoundLoad(char *filename, void **lpDSB) {
  assert(soundManager != nullptr);
  *lpDSB = soundManager->samplePlayer().loadSound(filename);
}

void SoundFinit() {
  if (soundManager != nullptr) {
    soundManager = nullptr;

    xtUnRegisterSysQuant(XSOUND_SYSOBJ_ID);
    xtDeactivateSysFinitFnc(XSOUND_SYSOBJ_ID);
  }
}

void SoundVolume(int channel, int volume) {
  if (soundManager) {
    soundManager->samplePlayer().setSoundVolume(channel, volume);
  }
}

void SetVolume(void *lpDSB, int volume) {
  if (soundManager) {
    soundManager->samplePlayer().setVolume(lpDSB, volume);
  }
}

int GetVolume(void *lpDSB) {
  if (soundManager) {
    soundManager->samplePlayer().getVolume(lpDSB);
  }
  return 0;
}

int GetSoundVolume(int channel) {
  if (soundManager) {
    return soundManager->samplePlayer().getSoundVolume(channel);
  }
  return 0;
}

void GlobalVolume(int volume) {
  if (soundManager) {
    soundManager->samplePlayer().setGlobalVolume(volume);
  }
}

void SoundPan(int channel, int panning) {
  if (soundManager) {
    soundManager->samplePlayer().setSoundPan(channel, panning);
  }
}

int SoundStatus(int channel) { return soundManager ? soundManager->samplePlayer().getSoundStatus(channel) : 0; }

int SoundStatus(void *lpDSB) { return soundManager ? soundManager->samplePlayer().getSoundStatus(lpDSB) : 0; }

int ChannelStatus(int channel) { return soundManager ? soundManager->samplePlayer().getChannelStatus(channel) : -1; }

int GetSoundFrequency(void *lpDSB) { return soundManager ? soundManager->samplePlayer().getSoundFrequency(lpDSB) : 0; }

void SetSoundFrequency(void *lpDSB, int frequency) {
  if (soundManager) {
    soundManager->samplePlayer().setSoundFrequency(lpDSB, frequency);
  }
}

void SoundStreamOpen(char *filename, void **strptr) {}

void SoundStreamClose(void *stream) {}

void SoundStreamRelease(void *stream) {}

void xsInitCD() {}

void xsMixerOpen() {}

bool PlayMusic(const char *filename, bool looping) {
  return soundManager ? soundManager->musicPlayer().play(filename, looping) : false;
}

void StopMusic() {
  if (soundManager) {
    soundManager->musicPlayer().stop();
  }
}

void PauseMusic() {
  if (soundManager) {
    soundManager->musicPlayer().pause();
  }
}

void ResumeMusic() {
  if (soundManager) {
    soundManager->musicPlayer().resume();
  }
}

int GetMusicStatus() {
  if (soundManager) {
    switch (soundManager->musicPlayer().getStatus()) {
      case game::sound::MusicPlayer::Status::play:
        return XCD_PLAYING;

      case game::sound::MusicPlayer::Status::pause:
        return XCD_PAUSED;

      case game::sound::MusicPlayer::Status::stop:
        return XCD_STOPPED;
    }
  }
  return XCD_STOPPED;
}

void SetMusicVolume(int volume) {
  if (soundManager) {
    soundManager->musicPlayer().setVolume(volume);
  }
}

#ifdef GPX
int GetMusicLengthInSamples(const char* filename) { return soundManager ? soundManager->musicPlayer().getLengthInSamples(filename) : 0; }
#else
int GetMusicLengthInSamples() { return soundManager ? soundManager->musicPlayer().getLengthInSamples() : 0; }
#endif

void XSoundQuantFnc(void) {
  if (soundManager) {
    soundManager->quant();
  }
}
