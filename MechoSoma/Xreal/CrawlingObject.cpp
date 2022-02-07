#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "CrawlingObject.h"
#include "SimpleClip.h"

CrawlingObject::CrawlingObject() 
{ 
	is_dynamic_ =  0; 
	current_tri = 0; 
	search_closest_triangle_counter = search_closest_triangle_delay = 0;
}

void CrawlingObject::non_dynamic_evolve(float dt)
{
	control();

	// Поворот (в мировой системе)
	setRot(Mat3f(rudder*k_rudder*dt, Z_AXIS)*Alg());

	// Тяга
	setLocalVelocity(Vect3f(0, traction*k_traction, 0));
	setTrans(R() + Vglobal()*dt);

	// Выравнивание по рельефу
	if(!current_tri || !search_closest_triangle_counter--){
		current_tri = tri_map -> closest_triangle(R(), radius());
		if(!current_tri){
			setTrans(R().z + H(), Z_AXIS);
			current_tri = tri_map -> closest_triangle(R(), radius());
			assert(current_tri);
			}
		search_closest_triangle_counter = search_closest_triangle_delay;
		}
	const Triangle& tri = *current_tri;
	Vect3f normal(tri.normal());
	setTrans(R() + -tri.dist(R())*normal);
	//if(R().z > 300 && Yglobal().z > 0) // баг с деревьями
	//	Alg().premult(Mat3f(M_PI, Z_AXIS));
	Vect3f rot = turn_cross(Zglobal(), normal);
	setRot(Mat3f(rot, 0.02*rot.norm())*Alg());
}	

void CrawlingObject::show() const
{
	Body::show();
	if(current_tri)
		current_tri -> show();
}

