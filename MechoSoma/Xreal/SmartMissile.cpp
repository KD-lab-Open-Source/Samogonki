#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "FlyingObject.h"
#include "SmartMissile.h"
#include "BodyDispatcher.h"
#include "M3d_effects.h"
#include "sound.h"

////////////////////////////////////////////////////////
//	SmartMissile
////////////////////////////////////////////////////////

void fxlabFireBallExplosion(class Body* p);
void fxlabLightningBoltExplosion(class Body* p);

SmartMissile::SmartMissile()
: Body(M3D_METEOR0)
{
	type_ = MISSILE;
	damage_ability_ = 100;
	k_traction = 70;
	setLocalVelocity(k_traction, Y_AXIS);
	setHeight(30);
	time_to_live.start(3000);

	traction = 1;
	k_rudder = 0.4f;
	radius_ = 15;
}	

void SmartMissile::setPosition(const Vect3f& pose_, float psi)
{
	setRot(-psi, Z_AXIS);
	Vect3f pose = pose_;
	CYCLE(pose);
	setTrans(pose);
	setPositionInit();
	setTrans(pose);
}

static SmartMissile* missile;
float forward_loocking_metric(const Body& body)
{	       
	if(missile -> owner() == body.ID)
		return DBL_INF;
	Vect3f dir = getDist(body.R(), missile -> R());
	float dist = dir.norm();
	if(dist < DBL_EPS)
		return 0;
	dir /= dist;
	dist *= (5 - 3*dot(dir, missile -> Yglobal()))/2;
	return dist;
}

void SmartMissile::set_owner(int ID, int duration) 
{ 
	OwnerProtection::set_owner(ID, duration); 
	missile = this;
	setTarget( Mdisp -> searchBody(&forward_loocking_metric, MECHOS) );
}

void SmartMissile::quant()
{
	if(!time_to_live || test_collision(0)){ //  ������������ � ������������
		kill();
		}
}

void SmartMissile::non_dynamic_evolve(float dt)
{
	clearForces();

	control();
	setRot(Mat3f(rudder*M_PI, Z_AXIS)*Alg());
	setLocalVelocity(traction*k_traction, Y_AXIS);

	float z = currentHeight() + relative_height;
	float dz = z - R().z;
	applyGlobalForce(dz*0.8, Z_AXIS);

	 integrateEuler(dt);
}


void SmartMissile::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		putArcaneStatistics(body.ID);
		kill();
		}
}

////////////////////////////////////////////////////////
//	SmartFireBall
////////////////////////////////////////////////////////
SmartFireBall::SmartFireBall()
: Body(M3D_FIRE_BALL)
{
	damage_ability_ = 0;
	k_traction = 70;
	setLocalVelocity(k_traction, Y_AXIS);
	setHeight(20);
	time_to_live.start(10000);
}

void SmartFireBall::kill()
{
	if(alive()){
		fxlabFireBallExplosion(this);
		startSound(EFF_FIREBALLS_ADD);	
		}
	Body::kill();
};

////////////////////////////////////////////////////////
//	SmartLightningBolt
////////////////////////////////////////////////////////
SmartLightningBolt::SmartLightningBolt()
: Body(M3D_MORNING_STAR)
{
	damage_ability_ = 0;
	k_traction = 70;
	setLocalVelocity(k_traction, Y_AXIS);
	setHeight(20);
	time_to_live.start(10000);
}

void SmartLightningBolt::kill()
{
	if(alive()){
		fxlabLightningBoltExplosion(this);
		startSound(EFF_LIGHT_BOLT_ADD);
		}
	Body::kill();
}

////////////////////////////////////////////////////////
//	SmartVoodooFire
////////////////////////////////////////////////////////
SmartVoodooFire::SmartVoodooFire()
: Body(M3D_METEOR0)
{
	damage_ability_ = 0;
	k_traction = 70;
	setLocalVelocity(k_traction, Y_AXIS);
	traction = 0.2f;
	setHeight(30);
	k_rudder = 0.4f;
	time_to_live.start(120000);
}

void SmartVoodooFire::set_owner(int ID, int duration) 
{ 
	OwnerProtection::set_owner(ID, 0x7fffffff); 
	setTarget( parent = Mdisp -> searchBody(ID) );
}

void SmartVoodooFire::quant()
{
	control();
	if(!time_to_live)
		kill();
	traction = 0.25 + parent -> averageSpeed()*0.75/50;
}
