#ifndef __BONUS_MAN_H__
#define __BONUS_MAN_H__

#include "TrackIterator.h"

#pragma warning( disable : 4250 )  

class BonusMan : public PointControlledObject
{
	enum State {
		Running,
		Swimming, 
		Paratrooper, 
		Burning, 
		Beeped
		};
	State state;
	TrackIterator track;
	int bonusID;
	DurationTimer turn_latency;
	DurationTimer burning_timer;
	DurationTimer after_beep_timer;
	AverageConditionTimer submersion_timer;
	int track_step;
public:
	BonusMan(int bonusID_);
	void quant();
	void control();
	void setPositionInit();
	void calc_forces_and_drags();
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body){ return 1; }
	void write_state(XBuffer& buf);
	void beep(const Vect3f& pos);
};


class BonusMan1 : public BonusMan
{
public:
	BonusMan1() : BonusMan(1), Body(myM3DType(18), CenteredBottomAlignmentedBound) {}
};

class BonusMan2 : public BonusMan
{
public:
	BonusMan2() : BonusMan(2), Body(myM3DType(19), CenteredBottomAlignmentedBound) {}
};

class BonusMan3 : public BonusMan
{
public:
	BonusMan3() : BonusMan(3), Body(myM3DType(20), CenteredBottomAlignmentedBound) {}
};

class BonusMan4 : public BonusMan
{
public:
	BonusMan4() : BonusMan(4), Body(myM3DType(21), CenteredBottomAlignmentedBound) {}
};

#pragma warning( default : 4250 )  

#endif // __BONUS_MAN_H__
