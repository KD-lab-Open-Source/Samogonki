#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "SimpleClip.h"
#include "FixedPathBody.h"
#include "Mesh3ds.h"
#include "params.h"
#include "sound.h"
#include "CameraPrm.h"
#include "QuatMath.h"


class SoundTable : hash_map<string, int>
{
public:
	SoundTable()
	{
		(*this)["watch_work.3ds->gear09"] = EFF_BLADE; 
	}

	int look(char* fname, char* name)
	{
		string s = strlwr(fname);
		s += "->";
		s += strlwr(name);
		int pos = s.rfind("\\");
		if(pos != string::npos)
			s.erase(0, pos + 1);
		iterator it = find(s);
		return it == end() ? 0 : it -> second;
	}
};

SoundTable sound_table;

////////////////////////////////////////////////////////////////////////////////
//		FixedPathBody
////////////////////////////////////////////////////////////////////////////////	
FixedPathBody::FixedPathBody(cMesh* mesh, int damaging, BodyDispatcher* body_dispatcher)
{
	type_ = GEAR;
	is_dynamic_ = 0;
	if(mesh -> Parent)
		setGeometry(mesh);
	else
		geometry = mesh;
	setPose(mesh -> GlobalMatrix);
	soundID = sound_table.look(mesh -> GetGeneralParent() -> GetFileName(), mesh -> GetName());
	prev_move = 0;
	
	mass_inv = 0;
	I_inv.set(0.f);
	
	if(damaging)
		damage_ability_ = 1000.f;

	body_dispatcher -> attachBody(this);
	if(mesh -> Parent && camera_enable_gears_clipping)
		tri_map -> addBrush(*mesh);

	for(cList* mi = mesh -> Child; mi; mi = mi -> next)
		new FixedPathBody(mi -> Mesh, damaging, body_dispatcher);
}	

FixedPathBody::~FixedPathBody()
{
	geometry = 0;
}

void FixedPathBody::createBound()
{
	polyhedrons.clear();
	Polyhedron* p = new Polyhedron;
	p -> setMesh(*geometry, 0);
	polyhedrons.add(p);

	spheres.clear();
	spheres.push_back(Sphere(Vect3f::ZERO, polyhedrons.rad()));
}

void FixedPathBody::non_dynamic_evolve(float dt)
{
	Se3f Tlg0 = pose();
	setPose(geometry -> GlobalMatrix);
	QuatF q(Alg());
	setGlobalVelocity((R() - Tlg0.trans())/dt);
	setGlobalAngularVelocity(calc_Wglobal(q, (q - Tlg0.rot())/dt));
}

float FixedPathBody::hit_damage_ability(const class Body& b, float velocity) const 
{ 
	return Vlocal().norm2() + Wlocal().norm2()*100 > 1 ? Body::hit_damage_ability(b, velocity) : 0; 
}

void FixedPathBody::quant()
{
	if(!soundID)
		return;
	if(Vlocal().norm2() + Wlocal().norm2()*100 > 4){
		if(!prev_move)
			startSound(soundID);
		prev_move = 1;
		}
	else
		prev_move = 0;
}

////////////////////////////////////////////////////////////////////////////////
//		FixedPathRoot
////////////////////////////////////////////////////////////////////////////////	
FixedPathRoot::FixedPathRoot(cMesh* mesh, int damaging, BodyDispatcher* body_dispatcher)
: FixedPathBody(mesh, damaging, body_dispatcher)
{
	speed = phase = 0;
	mesh -> GetAnimation(speed, phase);
	speed = speed ? 1000./(speed*real_time_to_evolve_time) : 0;
	mesh -> SetAnimation(0, phase);
}

void FixedPathRoot::non_dynamic_evolve(float dt)
{
	geometry -> SetAnimation(0, phase += speed*dt);
}

////////////////////////////////////////////////////////////////////////////////
//		loadGears
////////////////////////////////////////////////////////////////////////////////	
void WorldDispatcher::loadGears(cM3D& M3D)
{
	cMesh* mesh = M3D.First();
	while(mesh){
		char* substr;
		if(mesh -> description && (substr = strstr(mesh -> description, "G")) != 0){
			new FixedPathRoot(mesh, substr[1] == 'd',  this);
			}
		mesh = M3D.Next();
		}
}			
