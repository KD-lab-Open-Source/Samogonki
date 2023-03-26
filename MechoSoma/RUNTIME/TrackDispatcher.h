
#ifndef __TRACKDISPATCHER_H__
#define __TRACKDISPATCHER_H__

enum mchTrackStatusValue {
	MCH_TRACK_NOT_AVAILABLE = 0,
	MCH_TRACK_DISABLED,
	MCH_TRACK_NEXT_LEVEL,
	MCH_TRACK_ACCESSIBLE,
	MCH_TRACK_DONE,
	MCH_TRACK_HIDDEN
};

const int MCH_TRACK_CD_SEQ_MAX = 5;

class mchTrackInfo
{
	int worldID;
	int trackID;

	int level;
	int status;
	int pass_status;
	int access;

	int is_enabled;

	int length;
	int partID;

	char* text;

	int cd_trackCur;
	int cd_trackLen;
	unsigned char cd_trackSeq[MCH_TRACK_CD_SEQ_MAX];

	char* name;

	char* openMovie;
	char* closeMovie;

	int bossID;
	int bossFigureID;
	int bossSoundID;
	char* boss;

	int AI_NormLevel;
	int AI_MaxLevel;
public:
	mchTrackInfo* prev;
	mchTrackInfo* next;

	void* list;

	int CD_Track(void) const { return cd_trackSeq[cd_trackCur]; }
	void CD_TrackNext(void){ if(++cd_trackCur >= cd_trackLen) cd_trackCur = 0; }
	void CD_TrackReset(void){ cd_trackCur = 0; }

	int World(void) const { return worldID; }
	int Track(void) const { return trackID; }
	int Part(void) const { return partID; }
	int Level(void) const { return level; }
	int Length(void) const { return length; }
	int Status(void) const { return status; }
	int PassStatus(void) const { return pass_status; }
	int Access(void) const { return access; }

	int isEnabled(void) const { return is_enabled; }

	const char* Name(void) const { return name; }
	const char* Text(void) const { return text; }

	const char* OpenMovie(void) const { return openMovie; }
	const char* CloseMovie(void) const { return closeMovie; }

	const char* Boss(void) const { return boss; }
	int BossID(void) const { return bossID; }
	int BossFigureID(void) const { return bossFigureID; }
	int BossSoundID(void) const { return bossSoundID; }

	void Reset(void){ status = pass_status = access = 0; if(!level) access = 1; }

	int AI_Level(void) const { return ((status) ? AI_MaxLevel : AI_NormLevel); }

	void SetLength(int l){ length = l; }
	void SetStatus(int st = 1){ status = st; }
	void SetPassStatus(int st = 1){ pass_status = st; }
	void SetAccess(int ac = 1){ access = ac; }
	void Open(void){ access = 1; }
	void Init(struct scrDataBlock* p);

	mchTrackInfo(void);
	~mchTrackInfo(void);
};

// TrackDispatcher::flags
const int MCH_TRACK_DEBUG_FLAG	= 0x01;

class mchTrackDispatcher
{
	int curLevel;
	int flags;

	char figureFlags[20];
	char figureFlags2[20];

	mchTrackInfo* curTrack;
	xtList<mchTrackInfo> trackLst;
	
public:
	int CurLevel(void) const { return curLevel; }
	void NextLevel(void){ curLevel ++; }

	int TrackPart(void) const { if(curTrack) return curTrack -> Part(); return 0; }
	int TrackPart(int world,int track);
	int TrackLength(void) const { if(curTrack) return curTrack -> Length(); return 0; }
	int TrackLevel(void) const { if(curTrack) return curTrack -> Level(); return 0; }
	int TrackStatus(void) const { if(curTrack) return curTrack -> Status(); return 0; }
	int TrackAI_Level(void) const { if(curTrack) return curTrack -> AI_Level(); return 0; }
	int TrackCD_Track(void) const { if(curTrack) return curTrack -> CD_Track(); return 0; }
	void TrackCD_TrackNext(void){ if(curTrack) curTrack -> CD_TrackNext(); }
	void TrackCD_TrackReset(void){ if(curTrack) curTrack -> CD_TrackReset(); }

	const char* TrackName(int world,int track);
	const char* TrackText(int world,int track);

	int FigureEnabled(int n) const { if(n >= 0 && n < 20) return figureFlags[n]; return 0; }
	int FigureEnabledHS(int n) const { if(n >= 0 && n < 20) return figureFlags2[n]; return 0; }

	void EnableFigure(int n){ if(n >= 0 && n < 20) figureFlags[n] = figureFlags2[n] = 1; SaveFigureFlags(); }
	void EnableFigures(int n){ int i; for(i = 0; i < n; i ++) figureFlags[n] = 1; }
	void DisableFigure(int n){ if(n >= 0 && n < 20) figureFlags[n] = 0; }

	void SaveFigureFlags(void);
	void LoadFigureFlags(void);

	void FiguresQuant(void);

	int TrackBossID(void) const { if(curTrack) return curTrack -> BossID(); return 0; }
	int TrackBossFigureID(void) const { if(curTrack) return curTrack -> BossFigureID(); return 0; }
	const char* TrackBoss(void) const { if(curTrack) return curTrack -> Boss(); return NULL; }

	mchTrackInfo* FirstTrack(void){	return trackLst.first(); }
	mchTrackInfo* NextTrack(int world,int track);
	mchTrackInfo* PrevTrack(int world,int track);

	mchTrackInfo* GetTrack(int world,int track);
	void SetCurTrack(int world,int track);

	int TrackPrice(int world = -1,int track = -1);
	int TrackPrice(mchTrackInfo* p);

	int TrackMode(int world,int track);
	int TrackMode(mchTrackInfo* p);

	int GetPrize(int nstar);

	void OpenTrack(int world,int track);
	void PassTrack(void){ 
		if(curTrack){ 
			curTrack -> SetStatus(); 
			if(curTrack -> Boss())
				EnableFigure(curTrack -> BossFigureID());
		}
	}

	int CheckLevel(int level);
	int AllBossesDefeated(void);
	int IsBossWorldOpen(void);
	int IsNewWorldOpen(int nstar);

	void Save(XStream& fh);
	void Load(XBuffer& xb,int ver = -1);

	void Reset(void);

	void SetFlag(int fl){ flags |= fl; }
	void DropFlag(int fl){ flags &= ~fl; }
	int Flags(void) const { return flags; }

	void GetRndTrack(int& wrld,int& track);

	mchTrackDispatcher(void);
	~mchTrackDispatcher(void);
};

extern mchTrackDispatcher* mch_trackD;

#endif /* __TRACKDISPATCHER_H__ */