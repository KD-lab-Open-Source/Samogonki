#ifndef __TRACKRECORDER_H__
#define __TRACKRECORDER_H__

class Vect3f;

// trkPoint flags
const int TRK_REMOVE	= 0x01;

class trkPoint 
{
	Vect3f R_;
	int flags;

public:
	void* list;
	trkPoint* prev;
	trkPoint* next;

	Vect3f R(void) const { return R_; }

	int get_flags(void) const { return flags; }
	void toggle_flag(int v) { flags ^= v; }

	void Draw(int sel = 0) const;

	trkPoint(void){ R_ = Vect3f::ZERO; list = NULL; flags = 0; }
	trkPoint(Vect3f r){ R_ = r; list = NULL; flags = 0; }
	trkPoint(Vect3f r,int fl){ R_ = r; list = NULL; flags = fl; }
	~trkPoint(void){ }
};

class trkBranch
{
	xtList<trkPoint> points;

	void save(XStream& fh);
	void load(XStream& fh,int version);
	
public:
	int ID;
	int sID;

	void* list;
	trkBranch* prev;
	trkBranch* next;

	void insert(Vect3f r);

	void Draw(void);

	trkPoint* first(void) { return points.first(); }
	trkPoint* last(void) { return points.last(); }

	trkBranch(void){ ID = sID = NULL; list = NULL; }
	~trkBranch(void);

	friend class trkRecorder;
};

class trkRecorder
{
	int mode;
	int enabled;

	trkBranch* curBranch;
	trkPoint* curPoint;
	xtList<trkBranch> branches;
public:
	void Toggle(int fl){ enabled = fl; }

	void clear(void);
	void alloc(int sz);

	void KeyTrap(int& k);

	void Redraw(void);
	void RecPrepare(int cp);
	void RecQuant(Vect3f r,int cp);

	void save(char* fname);
	void load(char* fname);

	trkRecorder(void);
	~trkRecorder(void);
};

extern trkRecorder* mch_trkRec;
extern int trkEnableFlag;

void trkRecQuant(void);

void trkInitRecorder(void);
void trkFinitRecorder(void);

#endif /* __TRACKRECORDER_H__ */
