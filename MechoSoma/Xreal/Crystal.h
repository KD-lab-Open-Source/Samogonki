////////////////////////////////////////////////////////////////////
//	��������
////////////////////////////////////////////////////////////////////
#ifndef __CRYSTAL_H__
#define __CRYSTAL_H__

#include "ArcaneStatistics.h"
#include "OwnerProtection.h"
#include "GrowingBody.h"

class Crystal : public GrowingBody, public OwnerProtection, ArcaneStatisticsService {
	friend class Mechos;
	int collisions;
	int FrameTime, MaxTime;
public:
	Crystal();
	virtual void quant() {
		//@caiiiycuk: ???
	}
	virtual void calc_forces_and_drags() {
		//@caiiiycuk: ???
	}
	int is_colliding_with(const Body& body);
	void body_hit_reaction(const Body& body, const Contact& c);
	void save(XBuffer& buf);
	void load(XBuffer& buf);
	void setPositionInit();
	void post_quant();
	void body_overlap_reaction(Body& body);
	void kill(void);
};

#endif __CRYSTAL_H__
