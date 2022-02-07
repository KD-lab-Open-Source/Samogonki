#include "StdAfx.h"
#include "Mechos.h"
#include "SimpleClip.h"
#include "FieldSource.h"
#include "Mesh3ds.h"
#include "BodyDispatcher.h"
#include "sound.h"
#include "Params.h"
#include "MechosPrm.h"
#include "ArcansPrm.h"


///////////////////////////////////////////////////////
//	Часть мехоса
///////////////////////////////////////////////////////
Part::Part(cMesh* geometry_, int formic_)
: Body(geometry_),
targetMechos(*new ShareHandle<Mechos>)
{
	type_ = PART;
	disable_collision_mask = 0;//PART;
	enabled_fields = FieldSource::Attraction | FieldSource::Vortex | FieldSource::Magnetic;
	parentMesh = 0;
	targetMechos = 0;
	do_merge_to_mechos = 0;
	distance_to_mechos = DBL_INF;
	character = M3D_TYPE(geometry -> Type) == M3D_ENGINE;
	if(!character && !formic_)
		friction = impact_friction_factor;
	set_archimedean_force(part_buoyancy);
	make_non_colliding();
	geometry -> SetAttribute(MESH_LIGHTING_MUL | MESH_NOT_METALL);
	if(formic_){
		formic = xrealM3D -> CreateObject(myM3DType(13));
		formic -> SetAttribute(MESH_LIGHTING_MAX);
		}
	else
		formic = 0;
}

Part::~Part()
{
	if(formic)
		xrealM3D -> Delete(formic);
	delete &targetMechos;
}

void Part::release_body_handles()
{
	targetMechos = 0;
}

void Part::preLink(Mechos* mechos, cMesh* mesh)
{
	assert(mesh);
	targetMechos = mechos;
	parentMesh = mesh;
	distance_to_mechos = getDist(mechos -> R(), R()).norm();
	linkOffset = geometry -> CalcDummyOffset(mesh);
}

void Part::preLink(Mechos* mechos, const Vect3f& target)
{
	targetMechos = mechos;
	parentMesh = 0;
	distance_to_mechos = getDist(target, R()).norm();
	linkOffset = target;
}

void Part::post_quant()
{
	Body::post_quant();

	if(targetMechos && character)
		targetMechos -> setPose(pose());		

	if(formic){
		MatXf m = pose();
		m.trans() -= Yglobal()*((formic -> ymaxReal() + length()/2)*0.8);
		formic -> SetPosition(m);
		}
}

//////////////////////////////////////////////////////////////////
//	Общие свойства
//////////////////////////////////////////////////////////////////
void Part::quant()
{
	if(!character)
		geometry -> SetColor(0.2f,0.2f,0.2f);

	if(targetMechos){
		if(formic){
			if(!character){
				if((parentMesh = targetMechos -> part_by_type(M3D_PARENT(M3D_TYPE(geometry -> Type)))) == 0)
					return;
				}
			else
				if(targetMechos -> active())
					startSound(EFF_FORMICARY_ADD);
			}
	
		if(do_merge_to_mechos)
			merge_to_mechos();
		}
}

void Part::merge_to_mechos()
{
	if(parentMesh){
		MatXf Xpg = parentMesh -> GlobalMatrix;
		Vect3f r;
		linkOffset = geometry -> CalcDummyOffset(parentMesh);
		Xpg.xformPoint(linkOffset, r);
		geometry -> SetPosition(Xpg.rot(), r);
		parentMesh -> AttachChild(geometry);
		xrealM3D -> Detach(geometry);
		if(targetMechos -> active() && formic)
			stopSound(EFF_FORMICARY_ADD);
		}
	else{
		if(formic)
			targetMechos -> setRot(-targetMechos -> formic_transport_finish_angle, Z_AXIS);
		geometry -> SetPosition(targetMechos -> pose());
		targetMechos -> geometry = geometry;
		}
	geometry -> SetColor(1.0f, 1.0f, 1.0f);
	geometry -> ClearAttribute(MESH_LIGHTING_MUL | MESH_NOT_METALL);
	targetMechos -> addPart(geometry);
	geometry = 0;
	kill();
}

void Part::calc_forces_and_drags()
{
//	Body::calc_forces_and_drags();
	dragV.set(part_dragV);
	dragW.set(part_dragW);

	if(submersion() > 0.1 && !colliding()){
		float phase = archimedean_force_wave_time_factor*global_time() - archimedean_force_wave_distance_factor*sqrt(sqr(R().x) + sqr(R().y));
		applyGlobalForce(part_swing_lift_force*sin(phase), Z_AXIS);
		applyGlobalTorque(part_swing_torque*sin(phase + archimedean_force_wave_phase), X_AXIS);
		}
	
	if(targetMechos){
		Vect3f dr;
		if(!formic && !character){
			MatXf Xpg;
			if(parentMesh){
				Xpg = parentMesh -> GlobalMatrix;
				CYCLE(Xpg.trans());
				}
			Vect3f r;
			Xpg.xformPoint(linkOffset, r);
			dr = getDist(r, R());
			}
		else
			dr = getDist(linkOffset, R());

		float dist = sqrt(sqr(dr.x) + sqr(dr.y));
		if(ground_colliding)
			colliding_timer.start(part_colliding_duration);
		dr.z = 0;
		dr.Normalize(formic ? formicary_force : part_link_force);
		if(colliding_timer())
			dr.z = part_lift_force_factor;
		
		if(distance_to_mechos > part_merge_distance){
			if(distance_to_mechos > dist)
				distance_to_mechos = dist;
			else{
				dr.z = part_forced_moving_lift_force_factor;
				setTrans(R().z + part_forced_moving_distance, Z_AXIS);
				}
			}
		else
			do_merge_to_mechos = 1;
		if(stopping_detection_timer(distance_to_mechos - 0.5 < dist, part_stopping_detection_delay)){
			setTrans(max_height_threshould, Z_AXIS);
			place_on_the_ground();
			}
		distance_to_mechos = dist;

		applyGlobalForce(dr*Gravity);
		if(formic || character){
			dragW.set(part_dragW_formic);
			applyLocalTurnTorque(Zlocal(), Vect3f::K);
			applyLocalTurnTorque(Vlocal(), Vect3f::J);
			}
		}
}

int Part::is_colliding_with(const Body& body)
{
	return (parts_collision_flags & 2) && body.type() == MECHOS && targetMechos != &body 
		|| (parts_collision_flags & 4) && body.type() == PART;
}


/////////////////////////////////////////////////////////////////////////////
//			Utils
/////////////////////////////////////////////////////////////////////////////
Part* BodyDispatcher::searchPart(const Mechos& mechos, int Type, int Kind)
{
	float d, dmin = 1e+10;
	Part* nearest_part = 0;
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi)
		if((*bi) -> type() == Body::PART){
			Part& p = static_cast<Part&>(**bi);
			if(M3D_TYPE(p.geometry -> Type) == (unsigned)Type && !p.targetMechos){
				if(Kind && (unsigned)Kind != M3D_KIND(p.geometry -> Type))	
					continue;
				if((d = getDist(mechos.R(), p.R()).norm()) < dmin){
					dmin = d;
					nearest_part = &p;
					}
				}
			}
	return nearest_part;
}

void cMesh::attachDummyChildren(const int Kinds[])
{
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->attachDummyChildren(Kinds);
	
	int parentType = M3D_TYPE(Type);
	DummyList::iterator i;
	FOR_EACH(Dummies, i){
		int childType = i -> Type;
		if(childType < parentType ||
			FindMesh(childType)) // Already exist
				continue;

		int childTT = M3D_TOTAL_TYPE(Kinds[PART_INDEX(childType)], childType);
		if(!MeshLibrary -> Find(childTT))
			childTT = M3D_TOTAL_TYPE(1, childType);
		cMesh* mesh;
		if((mesh = xrealM3D -> CreateObject(childTT)) == 0)
			ErrH.Abort("Part not found", XERR_USER, childTT);
		
		xrealM3D -> Detach(mesh);
		AttachChild(mesh);
		mesh -> SetPosition(Mat3f::ID, mesh -> CalcDummyOffset(this));

		mesh -> attachDummyChildren(Kinds);
		}
}
