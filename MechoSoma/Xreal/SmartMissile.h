#ifndef __SMART_MISSILE__
#define __SMART_MISSILE__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"

class SmartMissile : public FlyingObject, public OwnerProtection, public ArcaneStatisticsService 
{
public:
	SmartMissile();
	void quant(); 
	void non_dynamic_evolve(float dt);
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body) { return 0; }
	void setPosition(const Vect3f& pose, float psi);
	void set_owner(int ID, int duration);
	float damage_ability(const Body& b) const { return b.ID != owner() ? Body::damage_ability(b) : 0; }
	void target_achieved(){ kill(); }
protected:
	DurationTimer time_to_live;
};

class SmartFireBall : public SmartMissile
{
public:
	SmartFireBall();
	void kill();
};

class SmartLightningBolt : public SmartMissile
{
public:
	SmartLightningBolt();
	void kill();
};

class SmartVoodooFire : public SmartMissile
{
	void target_achieved(){}
	void quant();

	Body* parent;
public:
	SmartVoodooFire();
	void set_owner(int ID, int duration);
};

#endif // __SMART_MISSILE__
