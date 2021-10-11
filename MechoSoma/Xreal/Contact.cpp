#include "StdAfx.h"
#include "Body.h"
#include "SimpleClip.h"
#include "params.h"
#include "CollisionHandler.hpp"

const int contacts_vector_size_max = 300;
int contacts_vector_size = 0;
CContact contacts_vector[contacts_vector_size_max];

////////////////////////////////////////////////////////////////////////////////
//		ContactProperty
////////////////////////////////////////////////////////////////////////////////
ContactProperty::ContactProperty()
{ 
	memset(this, 0, sizeof(*this)); 
	restitution_plus_one = 1.f; 
}

void ContactProperty::combination(const ContactProperty& prop)
{
	restitution_plus_one = sqrt(restitution_plus_one*prop.restitution_plus_one);
	impact_friction = sqrt(impact_friction*prop.impact_friction);
}

ContactProperty DynamicBody::contactProperty(const Contact& c) const
{
	ContactProperty prop;
	prop.restitution_plus_one = restitution_plus_one;
	prop.impact_friction = friction;
	return prop;
}

////////////////////////////////////////////////////////////////////////////////
//		Contact
////////////////////////////////////////////////////////////////////////////////
Contact::Contact()
{
	state = General;
	u_n = u_t = 0;
	a_n = a_t = 0;
	j_n = j_t = 0;
	f_n = f_t = 0;

	is_frictional = 0;
	is_dynamic_frictional = 0;

	tangent = Vect3f::ZERO;
	normal_index = tangent_index = -1;
}

void Contact::set(float dist_, const Vect3f& cp1_, const Vect3f& cp2_, const Vect3f& normal_, 
	Sphere* sphere_, const Triangle* triangle_,
	DynamicBody* body1_, DynamicBody* body2_)
{  
	dist = dist_; 
	cp1 = cp1_; 
	cp2 = cp2_; 
	normal = normal_; 
	sphere = sphere_;
	triangle = triangle_;
	body1 = body1_;
	body2 = body2_;
	(ContactProperty&)(*this) = body1 -> contactProperty(*this);
	body1 -> Xlg.xformVect(cp1, cp1g);
	if(body2){
		combination(body2 -> contactProperty(*this));
		body2 -> Xlg.xformVect(cp2, cp2g);
		}
	else
		cp2g = cp2;
	
	if(UseCollisionHandler && dist < 0){
		CContact& cp = contacts_vector[contacts_vector_size++];
		cp.penetration=-dist;
		cp.normal[0]=normal[0];
		cp.normal[1]=normal[1];
		cp.normal[2]=normal[2];
		Vect3f point1 = body1 -> R() + cp1g;
		cp.point[0]=point1[0];
		cp.point[1]=point1[1];
		cp.point[2]=point1[2];
		assert(cp.penetration > 0);
		cp.body[0] = body1;
		cp.body[1] = body2;
		cp.restitution = restitution_plus_one - 1;
		cp.friction = impact_friction;
		}

	calc_normal_velocity();
	if(-u_n > hit_reaction_velocity_threshould){
		if(body2){
			static_cast<Body*>(body1) -> body_hit_reaction(static_cast<Body&>(*body2), *this);
			static_cast<Body*>(body2) -> body_hit_reaction(static_cast<Body&>(*body1), *this);
			}
		else
			static_cast<Body*>(body1) -> ground_hit_reaction(*this);
		}
}

float Contact::calc_normal_velocity()
{
	u_n = dot(body1 -> global_velocity(cp1g), normal);
	if(body2)
		u_n -= dot(body2 -> global_velocity(cp2g), normal);
	return u_n;
}		

float Contact::calc_normal_acceleration()
{
	a_n = dot(body1 -> global_acceleration(cp1g), normal);
	if(body2)
		a_n -= dot(body2 -> global_acceleration(cp2g), normal);
	return a_n;
}

void Contact::prepare_to_impulse_calculation()
{
	is_frictional = 0;

	Vect3f vel = body1 -> global_velocity(cp1g);
	if(body2)
		vel -= body2 -> global_velocity(cp2g);
	
	u_n = dot(vel, normal);

	if(u_n > -DBL_EPS || impact_friction < FrictionZeroThreshould)
		return;

	tangent = vel - normal*u_n;
	u_t = tangent.norm();
	if(u_t > VelocityZeroThreshould){
		tangent /= u_t;
		is_frictional = 1;
		}
}

void Contact::prepare_to_force_calculation(int enable_dynamic_friction)
{
	is_dynamic_frictional = 0;
	is_frictional = 0;

	Vect3f acc = body1 -> global_acceleration(cp1g);
	if(body2)
		acc -= body2 -> global_acceleration(cp2g);

	a_n = dot(acc, normal);

	if(a_n > 0 || static_friction < FrictionZeroThreshould)
		return;

	Vect3f vel = body1 -> global_velocity(cp1g);
	if(body2)
		vel -= body2 -> global_velocity(cp2g);
	
	u_n = dot(vel, normal);
	tangent = vel - normal*u_n;
	u_t = tangent.norm();
	if(u_t > MinDynamicFrictionVelocity){ 
		if(!enable_dynamic_friction || dynamic_friction < FrictionZeroThreshould)
			return;
		is_dynamic_frictional = 1;
		tangent = tangent/u_t;
		return;
		}

	tangent = acc - normal*a_n;
	a_t = tangent.norm();
	if(a_t > AccelerationZeroThreshould){ 
		if(static_friction < FrictionZeroThreshould)
			return;
		is_frictional = 1;
		tangent = tangent/a_t;
		}
}

//////////////////////////////////////////////////////////////////////////////////////
//	Contact's Utils
//////////////////////////////////////////////////////////////////////////////////////
int Contact::log_resolving(ostream& out)
{
	float u_n_0 = u_n;
	float u_t_0 = u_t;
	float a_n_0 = a_n;
	float a_t_0 = a_t;

	Vect3f vel = body1 -> global_velocity(cp1g);
	if(body2)
		vel -= body2 -> global_velocity(cp2g);

	u_n = dot(vel, normal);
	u_t = dot(vel, tangent);

	Vect3f acc = body1 -> global_acceleration(cp1g);
	if(body2)
		acc -= body2 -> global_acceleration(cp2g);
	
	a_n = dot(acc, normal);
	a_t = dot(acc, tangent);

	out.precision(6);
	out.width(15);
	out << u_n_0 << " ";
	out.width(15);
	out << u_n << " ";
	out.width(15);
	out << j_n << " | ";

	out.width(15);
	out << u_t_0 << " ";
	out.width(15);
	out << u_t << " ";
	out.width(15);
	out << j_t << " | ";  

	out.width(15);
	out << a_n_0 << " ";
	out.width(15);
	out << a_n << " ";
	out.width(15);
	out << f_n << " | ";  
	out.width(15);
	out << dist << "\n";  

	return 0;
}

void Contact::show() const
{
	const MatXf& X1g = body1 -> pose();
	Vect3f center;
	X1g.xformPoint(cp1, center);
	int color;
	switch(state){
		case Contact::BodyOfMechos:
			color = XCOL(CYAN, 255);
			break;
		default:
			color = XCOL(MAGENTA, 255);
		}
	show_vector(center, color);

	if(f_n < j_n){
		show_vector(center, normal*f_n, XCOL(CYAN, 200));
		show_vector(center, normal*j_n, XCOL(RED, 200));
		}
	else{
		show_vector(center, normal*j_n, XCOL(RED, 200));
		show_vector(center, normal*f_n, XCOL(CYAN, 200));
		}

	if(f_t < j_t){
		show_vector(center, tangent*f_t, XCOL(CYAN, 200));
		show_vector(center, tangent*j_t, XCOL(RED, 200));
		}
	else{
		show_vector(center, tangent*j_t, XCOL(RED, 200));
		show_vector(center, tangent*f_t, XCOL(CYAN, 200));
		}
}


	
