#include "StdAfx.h"
#include "Body.h"
#include "CustomFieldSources.h"
#include "Params.h"
#include "xreal_utl.h"
#include "ArcansPrm.h"
#include "AnimalArcansPrm.h"
#include "Mechos.h"

//////////////////////////////////////////////////////////////////
//	Общий Точечный Источник
//////////////////////////////////////////////////////////////////
GeneralFieldSource::GeneralFieldSource(const Vect3f& r_, float radius_, float attraction_force_, float lift_force_, 
	const Vect3f& vortical_force_, const Vect3f& torque_, int owner_ID_)
: FieldSource(r_, radius_, owner_ID_)
{
	attraction_force = attraction_force_;
	lift_force = lift_force_;
	vortical_force = vortical_force_;
	torque = torque_;
}
	
//////////////////////////////////////////////////////////////////
//	Вихрь
//////////////////////////////////////////////////////////////////
VortexFieldSource::VortexFieldSource(const Vect3f& r_, float radius_, int owner_ID_, float factor, int clock_wise)
: GeneralFieldSource(r_, radius_, k_vortex_attraction, k_vortex_lift, 
	Vect3f(0,0, factor*k_vortex_force), 
	Vect3f(0,0, factor*k_vortex_torque), 
	owner_ID_)
{
	type_ = Vortex;
	if(clock_wise){
		vortical_force.negate();
		torque.negate();
		}
}

void VortexFieldSource::affect(Body& b) const
{
	if(!is_affecting(b) || b.relativeHeight() > 60)
		return;				

	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0 || rad < radius/8)
		return;
	
	set_affection(b);

	r.normalize();
	Vect3f F = (-attraction_force*(rad/rad0))*r;
	F.z += lift_force;
	F += r % vortical_force;

	if(b.Vlocal().norm() < max_velocity_threshould_general)
		b.applyGlobalForce(F*Gravity);

	if(b.Wlocal().norm() < max_angular_velocity_threshould)
		b.applyGlobalTorque(torque);

	putArcaneStatistics(b.ID);
}

//////////////////////////////////////////////////////////////////
//	Магнитное поле
//////////////////////////////////////////////////////////////////
MagneticFieldSource::MagneticFieldSource(const Vect3f& r_, float radius_, int owner_ID_)
: GeneralFieldSource(r_, radius_, -magnetic_field_repulsion, 0, 
	Vect3f::ZERO, Vect3f::ZERO, owner_ID_)
{
	type_ = Magnetic;
}

void MagneticFieldSource::affect(Body& b) const
{
	if(!is_affecting(b) || !b.colliding())
		return;

	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0 || rad < 1)
		return;

	set_affection(b);

	if(dot(r, b.Vglobal()) > 0 || b.Vglobal().norm() < max_velocity_threshould_general){
		r *= -attraction_force*Gravity*(rad0 - rad)/(rad0*rad);
		b.applyGlobalForce(r);
		}

	putArcaneStatistics(b.ID);
}

//////////////////////////////////////////////////////////////////
//	Притяжение
//////////////////////////////////////////////////////////////////
AttractionFieldSource::AttractionFieldSource(const Vect3f& r_, float radius_, int owner_ID_)
: GeneralFieldSource(r_, radius_, k_attraction_arcan, 0, 
	Vect3f::ZERO, Vect3f::ZERO, owner_ID_)
{
	type_ = Attraction;
}

void AttractionFieldSource::affect(Body& b) const
{
	if(!is_affecting(b))
		return;

	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0 || rad < radius/8)
		return;

	set_affection(b);

	if(dot(r, b.Vglobal()) > 0 || b.Vglobal().norm() < max_velocity_threshould_general){
		r *= -attraction_force*Gravity/rad0;
		b.applyGlobalForce(r);
		}

	putArcaneStatistics(b.ID);
}

/////////////////////////////////////////////////////////////////////////////////////////
//	Притяжение, сфера
/////////////////////////////////////////////////////////////////////////////////////////
BubbleFieldSource::BubbleFieldSource(const Vect3f& r_, float radius_, int owner_ID_)
: GeneralFieldSource(r_, radius_, 6, 0, 
	Vect3f::ZERO, Vect3f::ZERO, owner_ID_)
{
	type_ = BubbleField;
}
void BubbleFieldSource::affect(Body& b) const
{
	if(!is_affecting(b))
		return;

	if(b.affecting_fields() & type()) // Only one field per mechos
		return;
	if(victim && victim != &b) //  Only one mechos per field
		return;

	Vect3f position_(position);
	position_.z -= bubble_field_z_offset;
	Vect3f r = getDist(b.R(), position_);
	float rad0 = (radius + b.radius())*bubble_field_source_radius_factor;
	float rad = r.norm();
	if(rad > rad0)
		return;

	set_affection(b);
	victim = &b;

	if(dot(r, b.Vglobal()) > 0 || b.Vglobal().norm() < max_velocity_threshould_general){
		r *= -bubble_field_force*Gravity;
		b.applyGlobalForce(r);
		}

	putArcaneStatistics(b.ID);
}

///////////////////////////////////////////////////////////////////////
//	CircleMapSource
///////////////////////////////////////////////////////////////////////
void CircleMapSource::affect(Body& b) const
{
	if(!is_affecting(b))
		return;
	
	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0 || b.R().z < position.z || b.R().z > z_up)
		return;
	if(b.type() == Body::MECHOS ? dynamic_cast<Mechos&>(b).is_wheels_colliding() : b.colliding()){
		set_affection(b);
		putArcaneStatistics(b.ID);
		}
}

///////////////////////////////////////////////////////////////////////
//	RedTrackSource
///////////////////////////////////////////////////////////////////////
RedTrackSource::RedTrackSource(const Vect3f& pose, float rad, int time_to_live, int owner_ID, const ArcaneStatisticsService& stat)
: CircleMapSource(pose.x, pose.y, rad, RED_TRACK, owner_ID),
ArcaneStatisticsService(stat),
BaseObjectTTL(time_to_live)
{
	set_owner(owner_ID, 10000);
}

///////////////////////////////////////////////////////////////////////
//	Utils
///////////////////////////////////////////////////////////////////////
void FieldSource::show() const
{
	show_vector(position, radius, XCOL(CYAN, 100));
}
