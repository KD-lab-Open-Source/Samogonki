
#ifndef __CTL_POINT_H__
#define __CTL_POINT_H__

#include "arcane.h"

class cMesh;
struct mchRacer;
struct mchArcaneScreenElement;

struct mchControlPoint
{
	int ID;

	int PosX;
	int PosY;
	int PosZ;

//	int type;

	cMesh* mPtr;

	mchArcaneData arcaneData;
	class mchTrackElement* trackEl;

	void *list;
	mchControlPoint* prev;
	mchControlPoint* next;
	mchRacer* owner;

	void SetArcane(mchArcaneData& p,int tp,int p_id);
	void RemoveArcane(int mode = 0);

	void redraw(void);

	void createObj(void);
	void deleteObj(void);

	operator Vect3f () const { return Vect3f(PosX, PosY, PosZ); } // хотя лучше наследовать от Vect3f

	void save(XBuffer& xb);
	void load(XBuffer& xb);

	mchControlPoint(void);
	mchControlPoint(int x,int y,int z,int tp);
	~mchControlPoint(void);
};

#endif // __CTL_POINT_H__
