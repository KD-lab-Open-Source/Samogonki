#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "Debris.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "Mesh3ds.h"

Debris::Debris(cMesh* mesh, const Body& parent)
: Body(mesh, VolumetricEquivalentBound)
{
	type_ = DEBRIS;
	damage_ability_ = debris_damage_ability;
	friction = impact_friction_factor;


	Vect3f v;
	v.x = frnd(debris_Vxy); 
	v.y = frnd(debris_Vxy); 
	v.z = fabsRnd(debris_Vz); 
	setGlobalVelocity(v + parent.Vglobal());

	Vect3f w;
	w.x = frnd(debris_W);
	w.y = frnd(debris_W);
	w.z = frnd(debris_W);
	w.normalize(debris_W);
	setGlobalAngularVelocity(w + parent.Wglobal());

	place_on_the_ground();

	timer.start();
}	

void Debris::quant()
{
	float k = (debris_time_to_dwindle - timer())/debris_time_to_dwindle;
	setScale(k);
	if(k < 0.4)
		kill();
}

void explodeBody(cMesh* mesh, const Body& parent)
{
	for(cList* i = mesh -> Child; i;){
		cMesh* child = i -> Mesh;
		i = i -> next;
		child -> DetachChild();
		xrealM3D ->Attach(child);
		explodeBody(child, parent);
		}
	Debris* d = new Debris(mesh, parent);
	Mdisp -> attachBody(d);
}

void Body::explode(int mesh_id)
{
	cMesh* mesh = xrealM3D -> CreateObject(mesh_id);
	mesh -> SetPosition(pose());
	explodeBody(mesh, *this);
}

