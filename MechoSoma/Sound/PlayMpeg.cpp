#include <windows.h>
#include "mp+\mppdec.h"
#include <dsound.h>
#include "PlayMpeg.h"

//#include <stdio.h>//temp
#include <math.h>

static LPDIRECTSOUND g_pDS=NULL;
const DWORD sizeDSBuffer=512*1024;
static bool b_cycled;
static long volume=255;
static LPDIRECTSOUNDBUFFER pDSBuffer=NULL;
static DWORD dwWriteOffset;
static char fname[256];
static bool clear_end_buffer=false;
static MpegState mpeg_state=MPEG_STOP;

const maximal_len=1152*2;

static HANDLE hWaitEvent=INVALID_HANDLE_VALUE;
static HANDLE hThread=INVALID_HANDLE_VALUE;
static int b_thread_must_stop=0;


class EWait
{
public:
	EWait()
	{
		if(hWaitEvent==INVALID_HANDLE_VALUE)return;
		WaitForSingleObject(hWaitEvent,INFINITE);
	}
	~EWait()
	{
		if(hWaitEvent==INVALID_HANDLE_VALUE)return;
		SetEvent(hWaitEvent);
	}
};

#define DB_MIN		-10000
#define DB_MAX		0
#define DB_SIZE 	10000

static int FromDirectVolume(long vol)
{
	double v=exp((exp(((vol-DB_MIN)/(double)DB_SIZE)*log(10))-1.0)*log(2.0)*8/9.0)-1;
	
	return (int)(v+0.5);
}

static void ProcessStream();
static DWORD WINAPI ThreadProc(LPVOID lpParameter);

static long ToDirectVolume(int vol)
{

	int v = DB_MIN + (int)(0.5+
		log10(
			  9.0*log(double(vol + 1))/(log(2.0)*8) + 1.0
			 )*DB_SIZE
		);
	return v;
}

static void InternalMpegSetVolume(int _volume)
{
	volume=_volume;
	if(pDSBuffer)
	{
		HRESULT hr;
		long ddvol=ToDirectVolume(_volume);
		hr=pDSBuffer->SetVolume(ddvol);
	}
}

static bool InitSoundBuffer()
{
	HRESULT hr;
	/*
		Здесь создавать DirectSoundBuffer
	*/
	WAVEFORMATEX  wfx;

	wfx.wFormatTag=WAVE_FORMAT_PCM;
	wfx.nChannels=2;
	wfx.nSamplesPerSec=44100;
    wfx.nAvgBytesPerSec=44100*4;
    wfx.nBlockAlign=4; 
    wfx.wBitsPerSample=16;
    wfx.cbSize=0;

    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_CTRLPAN       | DSBCAPS_CTRLVOLUME |
                         DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2;
    dsbd.dwBufferBytes = sizeDSBuffer;
    dsbd.lpwfxFormat   = &wfx;

    // Create the static DirectSound buffer using the focus set by the UI
    if( FAILED( hr = g_pDS->CreateSoundBuffer( &dsbd, &pDSBuffer, NULL ) ) )
	{
		pDSBuffer=NULL;
        return false;
	}

	InternalMpegSetVolume(volume);

	return true;
}

static void InternalMpegStop()
{
	mpeg_state=MPEG_STOP;
	if(g_pDS==NULL)return;
	if(pDSBuffer)
		pDSBuffer->Stop();
}

static bool InternalMpegOpenToPlay(const char* _fname,bool cycled)
{
	if(g_pDS==NULL)return false;
	InternalMpegStop();

	if(_fname==NULL)return false;
	b_cycled=cycled;

	if(fname!=_fname)
		strcpy(fname,_fname);

	bool is_initialize=MpegOpen(fname);
	if(!is_initialize)
		return false;

	dwWriteOffset=0;
	int n=sizeDSBuffer/2/(2*maximal_len);
	for(int i=0;i<n;i++)
	{
		short* pData;
		int len;
		if(!MpegGetNextSample(pData,len))
			break;

		BYTE *AudioPtr1,*AudioPtr2;
		DWORD AudioBytes1,AudioBytes2;

		if(FAILED(pDSBuffer->Lock(
			  dwWriteOffset,
			  len*2,
			  (LPVOID*)&AudioPtr1,
			  &AudioBytes1,
			  (LPVOID*)&AudioPtr2,
			  &AudioBytes2,
			  0
			)))
			return false;

		if(AudioBytes1+AudioBytes2==(DWORD)len*2)
		{
			memcpy(AudioPtr1,pData,AudioBytes1);
			memcpy(AudioPtr2,pData+AudioBytes1,AudioBytes2);
		}


		pDSBuffer->Unlock(
			(LPVOID)AudioPtr1,
			AudioBytes1,
			(LPVOID)AudioPtr2,
			AudioBytes2);

		dwWriteOffset=(dwWriteOffset+len*2)%sizeDSBuffer;
	}

	if(pDSBuffer && i>0)
	{
		pDSBuffer->SetCurrentPosition(0);
		pDSBuffer->Play(0,0,DSBPLAY_LOOPING);
	}

	if(is_initialize)
		mpeg_state=MPEG_PLAY;
	return is_initialize;
}

static void TimeCallbackTrue()
{
	DWORD dwPlayCursor,dwWriteCursor;
	short* pData;
	int len;

//	char temp_buf[256];
/*
	{//temp
		if(FAILED(pDSBuffer->GetCurrentPosition(
				&dwPlayCursor,  
				&dwWriteCursor  
			)))
			return;

		if(dwPlayCursor<=dwWriteOffset)
			dwPlayCursor+=sizeDSBuffer;

		sprintf(temp_buf,"e=%2.3f\n",(dwPlayCursor-dwWriteOffset)/(double)(4*44100));
	}
*/
Retry:
	if(FAILED(pDSBuffer->GetCurrentPosition(
			&dwPlayCursor,  
			&dwWriteCursor  
		)))
		return;

	if(dwPlayCursor<=dwWriteOffset)
		dwPlayCursor+=sizeDSBuffer;

	if(dwWriteOffset+maximal_len*2>=dwPlayCursor)
	{
//		OutputDebugString(temp_buf);
		return;
	}

	if(MpegGetNextSample(pData,len))
	{
		BYTE *AudioPtr1,*AudioPtr2;
		DWORD AudioBytes1,AudioBytes2;

		if(FAILED(pDSBuffer->Lock(
			  dwWriteOffset,
			  len*2,
			  (LPVOID*)&AudioPtr1,
			  &AudioBytes1,
			  (LPVOID*)&AudioPtr2,
			  &AudioBytes2,
			  0
			)))
		{
//			OutputDebugString("Failed\n");
			return;
		}

		if(AudioBytes1+AudioBytes2==(DWORD)len*2)
		{
			if(AudioPtr1)memcpy(AudioPtr1,pData,AudioBytes1);
			if(AudioPtr2)memcpy(AudioPtr2,AudioBytes1+(BYTE*)pData,AudioBytes2);
		}

		pDSBuffer->Unlock((LPVOID)AudioPtr1,AudioBytes1,(LPVOID)AudioPtr2,AudioBytes2);

		dwWriteOffset=(dwWriteOffset+len*2)%sizeDSBuffer;

		clear_end_buffer=true;

		goto Retry;
	}

	if(clear_end_buffer && !b_cycled)
	{//Очистить конец буфера
		clear_end_buffer=false;

		BYTE *AudioPtr1,*AudioPtr2;
		DWORD AudioBytes1,AudioBytes2;

		if(FAILED(pDSBuffer->Lock(
			  dwWriteOffset,
			  maximal_len*2,
			  (LPVOID*)&AudioPtr1,
			  &AudioBytes1,
			  (LPVOID*)&AudioPtr2,
			  &AudioBytes2,
			  0
			)))
		{
//			OutputDebugString("Failed\n");
			return;
		}

		if(AudioBytes1+AudioBytes2==(DWORD)maximal_len*2)
		{
			if(AudioPtr1)memset(AudioPtr1,0,AudioBytes1);
			if(AudioPtr2)memset(AudioPtr2,0,AudioBytes2);
		}

		pDSBuffer->Unlock((LPVOID)AudioPtr1,AudioBytes1,(LPVOID)AudioPtr2,AudioBytes2);

//		dwWriteOffset=(dwWriteOffset+maximal_len*2)%sizeDSBuffer;
	}

	if(FAILED(pDSBuffer->GetCurrentPosition(&dwPlayCursor,&dwWriteCursor)))
		return;
//	sprintf(temp_buf,"write=%i,\tplay=%i\n",dwWriteOffset,dwPlayCursor);
//	OutputDebugString(temp_buf);


//	OutputDebugString("Mpeg STOP\n");
	MpegClose();
	if(b_cycled)
	{
		if(MpegOpen(fname))
		{
//			OutputDebugString("Mpeg PLAY\n");
			mpeg_state=MPEG_PLAY;
		}else
			InternalMpegStop();
	}else
	{
		if(dwPlayCursor>dwWriteOffset || dwPlayCursor+maximal_len*2<dwWriteOffset)
			return;
		InternalMpegStop();
	}
}

static DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL);

	while(b_thread_must_stop==0)
	{
		{
			EWait w;
			TimeCallbackTrue();
			ProcessStream();
		}

		Sleep(10);
	}

	b_thread_must_stop=2;
	return 0;
}

bool MpegOpenToPlay(const char* _fname,bool cycled)
{
	MpegStop();
	bool is_ok=InternalMpegOpenToPlay(_fname,cycled);
	if(is_ok)
	{
		b_thread_must_stop=0;
		DWORD ThreadId;
		hThread=CreateThread(
			NULL,
			0,
			ThreadProc,
			NULL,
			0,
			&ThreadId);
 
		hWaitEvent=CreateEvent(NULL,FALSE,TRUE,NULL);
	}

	return is_ok ;
}

void MpegStop()
{
	b_thread_must_stop=1;
	EWait w;
	InternalMpegStop();

	if(hThread!=INVALID_HANDLE_VALUE)
	while(b_thread_must_stop==1)
		Sleep(10);

	if(hWaitEvent!=INVALID_HANDLE_VALUE)
		CloseHandle(hWaitEvent);
	hWaitEvent=INVALID_HANDLE_VALUE;
	hThread=INVALID_HANDLE_VALUE;
}

bool MpegInitLibrary(void* pDS)
{
	g_pDS=(LPDIRECTSOUND)pDS;
	return InitSoundBuffer();
}

void MpegDeinitLibrary()
{
	MpegStop();

	if(pDSBuffer)pDSBuffer->Release();
	pDSBuffer=NULL;
	g_pDS=NULL;

}

void MpegSetVolume(int _volume)
{
	EWait w;
	InternalMpegSetVolume(_volume);
}

int MpegGetVolume()
{
	return volume; 
}

void MpegPause()
{
	EWait w;
	if(pDSBuffer==NULL)return;
	pDSBuffer->Stop();
	mpeg_state=MPEG_PAUSE;
}

void MpegResume()
{
	EWait w;
	if(pDSBuffer==NULL)return;
	if(mpeg_state==MPEG_STOP)return;

	if(SUCCEEDED(pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))
		mpeg_state=MPEG_PLAY;
}

MpegState MpegIsPlay()
{
	EWait w;
	if(pDSBuffer==NULL)return MPEG_STOP;
	DWORD status;
	if(FAILED(pDSBuffer->GetStatus(&status)))
		return MPEG_STOP;
	return mpeg_state;//(status&DSBSTATUS_PLAYING)?true:false;
}


///////////////////////Stream/////////////////////////
struct STREAM_COMMAND
{
	enum COMMAND
	{
		VOLUME_TO,
		VOLUME_SET,
		OPEN_TO_PLAY,
	} command;

	bool first;

	DWORD curtime;
	DWORD deltatime;
	int volume,old_volume;

	char fname[260];
	bool cycled;
};

const max_command=3;
static STREAM_COMMAND command[max_command];
static cur_command=0,num_command=0;

bool MpegSteamIsEmpty()
{
	return num_command==0;
}

bool MpegStreamVolumeTo(int vol,DWORD time)
{
	if(num_command>=max_command)
		return false;
	int cc=(cur_command+num_command)%max_command;
	STREAM_COMMAND& sc=command[cc];
	num_command++;
	sc.command=STREAM_COMMAND::VOLUME_TO;
	sc.first=true;

	sc.curtime=timeGetTime();
	sc.deltatime=time;
	sc.volume=vol;
	sc.old_volume=volume;
	
	return true;
}

bool MpegStreamSetVolume(int vol)
{
	if(num_command>=max_command)
		return false;
	int cc=(cur_command+num_command)%max_command;
	STREAM_COMMAND& sc=command[cc];
	num_command++;
	sc.command=STREAM_COMMAND::VOLUME_SET;
	sc.first=true;

	sc.volume=vol;

	return true;
}

bool MpegStreamOpenToPlay(const char* fname,bool cycled)
{
	if(num_command>=max_command)
		return false;
	int cc=(cur_command+num_command)%max_command;
	STREAM_COMMAND& sc=command[cc];
	num_command++;

	sc.command=STREAM_COMMAND::OPEN_TO_PLAY;
	sc.first=true;
	
	for(int i=0;i<sizeof(sc.fname)-2;i++ && fname[i])
		 sc.fname[i]=fname[i];
	sc.fname[i]=0;
	sc.cycled=cycled;

	return true;
}

static void ProcessStream()
{
	if(num_command==0)return;
	STREAM_COMMAND& sc=command[cur_command];

	if(sc.command==STREAM_COMMAND::VOLUME_TO)
	{
		sc.first=false;
		if(sc.first)
			sc.old_volume=volume;

		DWORD time=timeGetTime();
		DWORD max_time=sc.curtime+sc.deltatime;
		int vol=sc.volume;

		if(time<max_time)
		{
			float t=(time-sc.curtime)/(float)sc.deltatime;

			vol=(int)(sc.old_volume*(1-t)+sc.volume*t+0.5f);
			if(vol<0)vol=0;
			if(vol>255)vol=255;
		}

		if(pDSBuffer)
		{
			HRESULT hr;
			long ddvol=ToDirectVolume(vol);
			hr=pDSBuffer->SetVolume(ddvol);
		}

		if(time<max_time)
			return;
	}else
	if(sc.command==STREAM_COMMAND::VOLUME_SET)
	{
		InternalMpegSetVolume(sc.volume);
	}else
	if(sc.command==STREAM_COMMAND::OPEN_TO_PLAY)
	{
		InternalMpegOpenToPlay(sc.fname,sc.cycled);
	}

	cur_command=(cur_command+1)%max_command;
	num_command--;
	if(num_command<0)num_command=0;
}

