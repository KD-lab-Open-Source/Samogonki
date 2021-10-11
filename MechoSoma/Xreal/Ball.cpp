#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "Ball.h"
#include "mesh3ds.h"
#include "sound.h"
#include "AnimalArcansPrm.h"
#include "params.h"
#include "controls.h"
#include "MechoSoma.h"

void fxlabClownProcess(Vect3f& v);
void fxlabBubbleSource(Vect3f& pos,DurationTimer& timer);

Ball::Ball(Body* mechos_)
: mechos(mechos_),
Body(myM3DType(15))
{
	type_ = BALL;
	friction = impact_friction_factor;
	restitution_plus_one = 2.;
	setPose(mechos -> pose());
	setGlobalVelocity(mechos -> Vglobal() + Vect3f(0, 0, ball_initial_V));
	setLocalAngularVelocity(fabsRnd(ball_initial_W), X_AXIS);
	make_non_colliding();	
//	mechos -> kill();
	kill_mechos = 1;
	time_to_live.start(ball_time_to_live);
	startSound(EFF_CLOWN_ADD); // Mechos -> Ball
	set_archimedean_force(ball_buoyancy);
	dragV.set(ball_drag_vw);
	dragW.set(ball_drag_vw);
	control_config = dynamic_cast<Mechos*>(&*mechos) -> get_control_config();
	psi = mechos -> psi();
}
	
void Ball::release_body_handles()
{
	mechos = 0;
}

void Ball::kill()
{
	if(!alive())
		return;
	fxlabClownProcess(Vect3f(R()));
	startSound(EFF_CLOWN_BALL); // Ball -> Mechos
	mechos -> setTrans(R());
	mechos -> make_non_colliding();
	mechos -> setGlobalVelocity(Vglobal());
	Mdisp -> attachBody(mechos);	
	dynamic_cast<Mechos*>(mechos()) -> reset_stopping();
	Body::kill();
}

void Ball::quant()
{
	if(!wave_timer && submersion())
		fxlabBubbleSource(Vect3f(R()),wave_timer);
	if(!time_to_live)
		kill();
	if(mechos && kill_mechos){
		mechos -> kill();
		kill_mechos = 0;
		}
}

void Ball::post_quant()
{
	Body::post_quant();
	mechos -> setTrans(R());
	mechos -> geometry -> SetPosition(R());
}

void Ball::ground_hit_reaction(const Contact& c)
{
	startSound(EFF_CLOWN_BALL_ADD);
}

void Ball::calc_forces_and_drags()
{
	Vect3f v = Vglobal();
	v.z = 0;
	if(v.norm() > ball_max_Vxy){
		v.normalize(ball_max_Vxy);
		v.z = Vglobal().z;
		setGlobalVelocity(v);
		}
	if(!mechos -> active() || mchTurnBasedGame)
		return;
	Vect3f force(sin(psi), cos(psi), 0);
	force *= ball_traction_factor*Gravity;
	if(mchKeyPressed(MCH_KEY_MOVE_FORWARD, control_config))
		applyGlobalForce(force);
	if(mchKeyPressed(MCH_KEY_MOVE_BACKWARD, control_config))
		applyGlobalForce(-force);

	if(mchKeyPressed(MCH_KEY_TURN_RIGHT, control_config))
		psi -= ball_steering_factor;
	if(mchKeyPressed(MCH_KEY_TURN_LEFT, control_config))
		psi += ball_steering_factor;
	mechos -> setRot(-psi, Z_AXIS);
}
