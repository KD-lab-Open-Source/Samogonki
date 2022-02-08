///////////////////////////////////////////////////////////
//		BonusMan
///////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "BonusMan.h"
#include "SimpleClip.h"
#include "Params.h"
#include "IVisGeneric.h"
#include "AnimalArcansPrm.h"

void fxlabTakeBonus(int type, const Vect3f& pos);
void fxlabBonusFire(const Vect3f& v);

BonusMan::BonusMan(int bonusID_)
{ 
	type_ = BONUS_MAN; 
	bonusID = bonusID_; 
	k_traction = 15;
	k_rudder = 0.8f;
	traction = 1;
}

void BonusMan::setPositionInit()
{
	track.set(R().xi(), R().yi());
	setTrans((Vect3f)track + Vect3f(0,0,300));
	PointControlledObject::setPositionInit();
	setChannel("paratrooper");
	setAnimation(1000, 0);
	state = Paratrooper;
	set_archimedean_force(bonus_man_buoyancy);
	control_points.clear();
	reset_stopping();
	clearVelocities();
	clearForces();
	ground_colliding = 0;
	track_step = random(2) ? 2 : -2;
}

void BonusMan::control()
{
	while(num_control_points() < 5){
		track += track_step;
		Vect3f p = track(frnd(0.8), 0);
		add_control_point(p);
		}
	PointControlledObject::control();
	traction = 1;
}

void BonusMan::quant()
{
	if(state != Burning && submersion() > 0.1 && Mdisp -> waterDamage()){
		fxlabBonusFire(R() - Vect3f(0, 0, radius()*0.8));
		state = Burning;
		burning_timer.start(bonus_man_burning_time);
		}
	if(state != Burning && stopping){
		state = Burning;
		burning_timer.start(bonus_man_burning_time);
		}

	if(relativeHeight() > bonus_man_shadow_height_threshould)
		geometry -> SetAttribute(MESH_NOT_SHADE);
	else
		geometry -> ClearAttribute(MESH_NOT_SHADE);

	switch(state){
		case Running:
			if(submersion() > bonus_man_submersion_threshould){
				setChannel("swim");
				state = Swimming;
				}
			break;
		case Swimming:
			if(submersion_timer(submersion() < bonus_man_submersion_threshould, bonus_man_switch_time_threshould)){
				setChannel("main");
				state = Running;
				}
			break;
		case Paratrooper:
			if(colliding() || submersion() > bonus_man_submersion_threshould){
				setChannel("main");
				setAnimation(1000, 0);
				state = Running;
				}
			break;
		case Burning:
			setAlpha(burning_timer()/(float)bonus_man_burning_time);
			if(!burning_timer){
				setAlpha(1);
				setPosition(Vect3f(random(2048), random(2048), frnd(M_PI)));
				}
			break;
		case Beeped:
			traction = 1.5;
			if(after_beep_timer())
				setAlpha(after_beep_timer()/(float)bonus_man_beeped_time);
			else{
				setAlpha(1);
				setChannel("main");
				setPosition(Vect3f(random(2048), random(2048), frnd(M_PI)));
				}
			break;
		}
}

void BonusMan::calc_forces_and_drags()
{
	switch(state){
		case Running:
		case Swimming:
		case Beeped:
			if(state != Beeped)
				control();
			else
				ControlledObject::control();
			Body::calc_forces_and_drags();

			applyLocalTurnTorque(Zlocal(), Vect3f::K);
			applyLocalTorque(k_rudder*rudder, Z_AXIS);
			if(colliding() || submersion() > bonus_man_submersion_threshould){
				if(colliding())
					applyGlobalForce(1.1*Gravity, Z_AXIS);
				applyLocalForce(k_traction*traction, Y_AXIS);
				}
			if(state == Swimming){
				dragV.set(contact_linear_damping);
				dragW.set(contact_angular_damping);
				}
			break;

		case Paratrooper:
			dragV.set(1);
			dragW.set(1);
			applyLocalTurnTorque(Zlocal(), Vect3f::K);
			break;

		case Burning:
			dragV.set(10);
			dragW.set(10);
			break;
		}
}

void BonusMan::body_overlap_reaction(Body& body)
{
	switch(body.type()){
		case MECHOS:
			if(((state == Running || state == Swimming) && body.averageSpeed() > bonus_man_mechos_hit_velocity || state == Paratrooper) && dynamic_cast<Mechos&>(body).CheckBonusSlot(bonusID)){
				dynamic_cast<Mechos&>(body).AddBonus(bonusID);
				fxlabTakeBonus(bonusID, R());
				// Выкидываем снова
				setPosition(Vect3f(random(2048), random(2048), frnd(M_PI)));
				return;
				}				
		}

	if(!turn_latency){ // Reverse
		control_points.clear();
		track_step = -track_step;
		track += track_step*3;
		control();
		target_achieved();
		control();
		setRot(Mat3f(M_PI, Z_AXIS)*Alg());
		turn_latency.start(3000);
		}
}

void BonusMan::write_state(XBuffer& buf)
{
	PointControlledObject::write_state(buf);
	buf < "track_step: " <= track_step < "\n";
	buf < "turn_latency: " <= turn_latency() < "\n";
}

/////////////////////////////////////////////////////////////
//	Beep
/////////////////////////////////////////////////////////////
class BeepedBonus : public Body
{
	int bonusID;
public: 
	BeepedBonus(int bonusID_, const Vect3f& R_, const Vect3f& V_);
	void body_overlap_reaction(Body& body);
};

BeepedBonus::BeepedBonus(int bonusID_, const Vect3f& R_, const Vect3f& V_)
: Body(myM3DType(32 + bonusID_ - 1))
{	
	type_ = STATIC_BONUS; 
	bonusID = bonusID_;
	setTrans(R_);
	setGlobalVelocity(V_);
	Vect3f w;
	w.x = frnd(beeped_bonus_W);
	w.y = frnd(beeped_bonus_W);
	w.z = frnd(beeped_bonus_W);
	w.normalize(beeped_bonus_W);
	setGlobalAngularVelocity(w);
}	

void BeepedBonus::body_overlap_reaction(Body& body)
{
	if(completed() && body.type() == MECHOS && dynamic_cast<Mechos&>(body).CheckBonusSlot(bonusID)){
		dynamic_cast<Mechos&>(body).AddBonus(bonusID);
		fxlabTakeBonus(bonusID, R());
		kill();
		}
}

void BonusMan::beep(const Vect3f& pos)
{
	if((state == Running || state == Swimming) && getDist(pos, (Vect3f)R()).norm() < bonus_man_beeped_distance){
		Mdisp -> attachBody(new BeepedBonus(bonusID, R(), Vglobal() + Yglobal()*beeped_bonus_Vxy + Zglobal()*beeped_bonus_Vz));
		state = Beeped;
		setChannel("beeped");
		setInverseTarget(pos);
		after_beep_timer.start(bonus_man_beeped_time);
		}
}
