////////////////////////////////////////////////////////////////////
//	Метеорит
////////////////////////////////////////////////////////////////////
#ifndef __METEORITE_H__
#define __METEORITE_H__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"

class Meteorite : public Body, public OwnerProtection 
{
public:
	Meteorite(int m3d_model, const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID = 0);
	void calc_forces_and_drags(); 
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body) { return 0; }
	void scaleRadius(float factor);
};

class StatisticalMeteorite : public Meteorite, public ArcaneStatisticsService 
{
public:
	StatisticalMeteorite(int m3d_model, const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID = 0)
		: Meteorite( m3d_model, R_, V_, damage__, owner_ID) {}
	
	void body_overlap_reaction(Body& body);
	float damage_ability(const Body& b) const { return b.ID != owner() ? Body::damage_ability(b) : 0; }
};

class BonusMeteorite : public StatisticalMeteorite 
{
public:
	BonusMeteorite(int m3d_model, const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID = 0);
	void body_overlap_reaction(Body& body);
	void calc_forces_and_drags(){ Body::calc_forces_and_drags(); }
};


class RealBonusMeteorite : public BonusMeteorite 
{
	int bonus_ID;

public:
	RealBonusMeteorite(int m3d_model, const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID, int bonus_ID_)
		: BonusMeteorite( m3d_model, R_, V_, damage__, owner_ID) { bonus_ID = bonus_ID_; type_ = REAL_BONUS_METEORITE; }

	void body_overlap_reaction(Body& body);
};


#endif // __METEORITE_H__
