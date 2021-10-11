#pragma once
//
bool MpegInitLibrary(void* LPDIRECTSOUND_pDS);

//Вызывать до деинициализации DirectSound
void MpegDeinitLibrary();

bool MpegOpenToPlay(const char* fname,bool cycled);
void MpegStop();
void MpegPause();
void MpegResume();

enum MpegState
{
	MPEG_STOP=0,
	MPEG_PLAY=1,
	MPEG_PAUSE=2,
};

MpegState MpegIsPlay();


void MpegSetVolume(int volume);//0..255
int MpegGetVolume();


//Потоковые команды, каждая последующая выполняется после окончания предыдущей.
//возвращают true если команда добавлена в очередь.
//в очереди могут быть не более 3x команд

bool MpegSteamIsEmpty();//Нет потоковых команд

//Уменьшить громкость c текущего значения до volume  за time милисекунд
bool MpegStreamVolumeTo(int volume,DWORD time);
bool MpegStreamSetVolume(int volume);
bool MpegStreamOpenToPlay(const char* fname,bool cycled);
