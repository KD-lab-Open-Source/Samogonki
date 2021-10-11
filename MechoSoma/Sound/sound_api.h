
int sndVisible(int x,int y);
int sndVolumeXY(int x,int y);
int sndPanXY(int x,int y);
int sndPanScrXY(int x,int y);
int sndFreqXY(int x,int y,int freq_val,int freq_delta);

void sndCoordsRnd(int& x,int& y);

int sndGetFreeChannel(int x,int y,int& pr);
int sndGetFreeChannel2(int x,int y,int& pr);

char* sndGetTargetName(char* p);

int sndGetCurrentCD_Track(void);
void sndNextCD_Track(void);

int sndGetCameraVolume(void);
void sndDumpStats(void);

int sndCheckTrack(struct scrDataBlock* p);

extern unsigned sndRndVal;
inline unsigned sndRnd(unsigned m)
{
	sndRndVal ^= sndRndVal >> 3;
	sndRndVal ^= sndRndVal << 28;
	sndRndVal &= 0x7FFFFFFF;

	if(!m)
		return 0;

	return sndRndVal % m;
}

void sndMusicPlay(int track);
void sndMusicStop(void);
void sndMusicPause(void);
void sndMusicResume(void);
int sndMusicStatus(void);
int sndMusicCurTrack(void);
int sndMusicNumTracks(void);
void sndMusicSetVolume(int val);

extern int sndCameraVolumeZ0;
extern int sndCameraVolumeZ1;
extern int sndCameraVolumeV0;
extern int sndCameraVolumeV1;

extern int sndMaxPanning;
extern int sndVisibleArea;
extern int sndMinVolume;
