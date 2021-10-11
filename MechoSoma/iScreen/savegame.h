#ifndef __SAVEGAME_H__
#define __SAVEGAME_H__

enum sgRecordType
{
	SG_VOID_RECORD = 1,
	SG_SEEDS_RECORD,
	SG_END_TURN_RECORD,
	SG_CONTROL_RECORD
};

class sgRecord 
{
	int time;
	int type;
	int gameID;

	int data_size;
	unsigned char* data;
public:
	void* list;
	sgRecord* prev;
	sgRecord* next;

	void SetTime(int tm) { time = tm; }
	void SetType(int tp) { type = tp; }
	void SetGameID(int game) { gameID = game; }

	int Time(void) const { return time; }
	int Type(void) const { return type; }
	int GameID(void) const { return gameID; }
	int DataSize(void) const { return data_size; }
	unsigned char* Data(void) const { return data; }

	void Alloc(int sz){ if(data) delete data; data = new unsigned char[data_size = sz]; }

	sgRecord(void);
	sgRecord(int tm,int tp,int game,int sz);

	~sgRecord(void);
};

class sgDispatcher
{
	int save_flag_;

	int gameID;
	xtList<sgRecord> recLst;
public:
	sgRecord* get_first(int time);
	sgRecord* get_next(void);

	int num_records(void){ return recLst.size(); }

	void load(char* fname,int clear_flag = 0);
	void save(char* fname,int time = -1);

	void add_record(int time,int type,int data_sz,void* data);

	void clear(void);
	void clean_list(void);

	int time_max(void);
	int turn_max(void);

	void set_save_flag(int fl = 1) { save_flag_ = fl; }
	int save_flag(void) const { return save_flag_; }

	sgDispatcher(void);
	~sgDispatcher(void);
};

extern sgDispatcher* sgD;

void sgGetOnlineData(void);
int sgOnlineControlCheck(void);
void sgPrepareSeedsData(void);
void sgPrepareControlData(void);
void sgPrepareRefreshData(void);
void sgProcessRefreshData(void);
void sgGetWorldID(char* fname);
int sgCheckPacket(char* fname);

#endif /* __SAVEGAME_H__ */