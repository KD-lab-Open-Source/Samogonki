#include "xsound.h"

int SoundInit(int maxHZ, int digMode, int channels)
{
	return 0;
}

void SoundPlay(void *lpDSB, int channel, int priority, int cropos, int flags)
{
}

void SoundRelease(void *lpDSB)
{
}

void SoundStop(int channel)
{
}

void* GetSound(int channel)
{
    return nullptr;
}

void SoundLoad(char *filename, void **lpDSB)
{
}

void SoundFinit(void)
{
}

void SoundVolume(int channel, int volume)
{
}

void SetVolume(void *lpDSB, int volume)
{
}

int GetVolume(void *lpDSB)
{
	return 0;
}

int GetSoundVolume(int channel)
{
	return 0;
}

void GlobalVolume(int volume)
{
}

void SoundPan(int channel, int panning)
{
}

int SoundStatus(int channel)
{
	return 0;
}

int SoundStatus(void* lpDSB)
{
	return 0;
}

int ChannelStatus(int channel)
{
	return 0;
}

int GetSoundFrequency(void *lpDSB)
{
	return 0;
}

void SetSoundFrequency(void *lpDSB,int frq)
{
}

void SoundStreamOpen(char *filename, void **strptr)
{
}

void SoundStreamClose(void *stream)
{
}

void SoundStreamRelease(void *stream)
{
}

void xsInitCD(void)
{
}

void xsMixerOpen(void)
{
}
