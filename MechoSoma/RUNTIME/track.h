
#ifndef __TRACK_H__
#define __TRACK_H__

#define _FCONTAINER_LOAD_

#include "Grid2D.h"

const int TRACK_CELL_BYTES	= 7;
const int TRACK_CELL_SIZE	= 1 << TRACK_CELL_BYTES;
const int TRACK_CELL_SIZE2	= 1 << (TRACK_CELL_BYTES - 1);

struct mchCheckpoint;
struct mchTrackBranch;

// mchTrackElement::flags
#define TRACK_EL_BUSY		0x01
#define TRACK_EL_FIRE		0x02
#define TRACK_EL_ELECTRICITY	0x04
#define TRACK_EL_SLIME		0x08
#define TRACK_EL_TELEPORT	0x10
#define TRACK_EL_REMOVE		0x20

class mchTrackElement : public Grid2DElement
{
public:
	int ID;

	int Cx;
	int Cy;
	int Cz;

	int index;

	mutable int flags;

	void* list;
	mchTrackElement* prev;
	mchTrackElement* next;

	mchTrackBranch* owner;

	// Grid
	void show() const;
	iRectangle getBound(int dx = 0,int dy = 0) const 
	{ 
		return iRectangle(Cx - TRACK_CELL_SIZE2 + dx,Cy - TRACK_CELL_SIZE2 + dy,Cx + TRACK_CELL_SIZE2 + dx,Cy + TRACK_CELL_SIZE2 + dy); 
	}

	void init(void);
	void redraw(void);

	int get_dist(void);

	int is_removed(void) const { return (flags & TRACK_EL_REMOVE); }

	operator Vect3f () const { return Vect3f(Cx, Cy, Cz); } // хотя лучше наследовать от Vect3f

	mchTrackElement(void){ list = NULL; flags = 0; }
};

struct mchTrackBranch
{
	int ID;
	int index;

	int flags;

	int start_index;

	xtList<mchTrackElement>* pointLst;

	void* list;
	mchTrackBranch* prev;
	mchTrackBranch* next;

	mchCheckpoint* owner;

	void Load(XStream& fh);

#ifndef _FCONTAINER_LOAD_
	void LoadBTR(XStream& fh,int version);
#else
	void LoadBTR(XBuffer& fh,int version);
#endif

	void redraw(void);

	void merge(mchTrackBranch* br);
	void init(void);
	void init_dist(int dist0);
	void init_ids(int& id0);
	void clear_flags(int mode);

	void clean(void);

	int get_dist(mchTrackElement* el);

	mchTrackBranch(void);
	~mchTrackBranch(void);
};

class mchTrackMap : public Grid2D<mchTrackElement,TRACK_CELL_BYTES>
{
public:
	void SetFlag(int x,int y,int sx,int sy,int fl);
	void InsertBranch(mchTrackBranch* p);

	mchTrackElement* GetTrackEl(int x,int y,int sx,int sy);
	mchTrackElement* GetTrackEl(Vect3f r,int sx,int sy,int id = -1);
};

extern xtList<mchTrackBranch>* mchTrackData;
extern mchTrackMap* mch_trackMap;

void mchLoadTrack(void);
void mchFreeTrack(void);

mchTrackElement* mchGetTrackEl(int id);

#endif // __TRACK_H__
