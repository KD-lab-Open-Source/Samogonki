
#ifndef __SMART_STONE_H__
#define __SMART_STONE_H__

#include "race.h"
#include "SimpleTrack.h"
#pragma warning( disable : 4250 )  

class SmartStoneRacer :  public mchBossRacer
{
	enum State {
		Rolling,
		Flying
		};

	State state;
	SimpleTrack track;
	float psi_;
	cMesh* effect_geometry;

	void quant(); 
	void calc_forces_and_drags();
	void setPositionInit();
	void ground_hit_reaction(const Contact& c);
	void show() const;
public:
	SmartStoneRacer();
	~SmartStoneRacer();
	float psi() const { return psi_; }
};

#endif // __SMART_STONE_H__
