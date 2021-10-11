#include "StdAfx.h"
#include "Body.h"
#include "CustomFieldSources.h"
#include "Params.h"

// Однородное поле
void UniformFieldSource::affect(Body& b) const
{
	if(!is_affecting(b))
		return;

	if(b.affecting_fields() & type()) // Only one field!
		return;

	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0)
		return;

	set_affection(b);
	
	float f = dot(force - b.Vglobal(), force)/force.norm2();
	b.applyGlobalForce(force*f);

	Vect3f Ft = Mat3f(M_PI/2, Z_AXIS)*force;
	if(dot(Ft, r) > 0)
		Ft.negate();
	b.applyGlobalForce(Ft/4);
}

// Течение (условие на погружение в воду)
void StreamFieldSource::affect(Body& b) const
{
	if(b.submersion() > 0.01)
		UniformFieldSource::affect(b);
}

/////////////////////////////////////////////////////		
//		Sensor
/////////////////////////////////////////////////////		
void SensorFieldSource::affect(Body& b) const
{
	if(!is_affecting(b)) // Пропускает только Mechos
		return;

	// Проверка на круг (цилиндр в 3D)
	Vect3f r = getDist(b.R(), position);
	r.z = 0;	
	float rad0 = radius + b.radius();
	float rad = r.norm();
	if(rad > rad0)
		return;

	set_affection(b);

	b.field_affection(this);
}
