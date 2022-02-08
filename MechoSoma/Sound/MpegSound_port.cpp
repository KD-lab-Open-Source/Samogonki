#include "PlayMpeg.h"

bool MpegInitLibrary(void* LPDIRECTSOUND_pDS)
{
	return true;
}

void MpegDeinitLibrary()
{
}

MpegSound::MpegSound()
{
}

MpegSound::~MpegSound()
{
}

bool MpegSound::OpenToPlay(const char* fname,bool cycled)
{
	return true;
}

void MpegSound::Stop()
{
}

void MpegSound::Pause()
{
}

void MpegSound::Resume()
{
}

MpegState MpegSound::IsPlay()
{
	return MPEG_STOP;
}

void MpegSound::SetVolume(int volume)
{

}

int MpegSound::GetVolume()
{
	return 0;
}

int MpegSound::GetLen()
{
	return -1;
}
