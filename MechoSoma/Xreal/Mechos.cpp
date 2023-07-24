#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "Meteorite.h"
#include "SimpleClip.h"
#include "Params.h"
#include "sound.h"
#include "Mesh3ds.h"
#include "IVisGeneric.h"
#include "M3d_effects.h"
#include "Line2D.h"
#include "mechosoma.h"
#include "Xreal_utl.h"
#include "fxlabInterface.h"
#include "BaseTrail.h"
#include "TERRA.H"
#include "controls.h"
#include "CustomFieldSources.h"
#include "XJoystick.h"
#include "PolyhedronLibrary.h"
#include "RungeKutta4.h"
#include "MechosPrm.h"
#include "ArcansPrm.h"
#include "PersonageDifferetiationPrm.h"
#include "Statistics.h"
#include "DebugPrm.h"
#include "AnimalArcansPrm.h"

extern unsigned int XRndVal;
extern int allow_transparency_of_bridges_by_camera;

void TrailColorsInit();

void fxlabMechosFireProcess(Mechos* p);
void fxlabMechosArmor(Mechos* p);
void fxlabMechosSkin(Mechos* p);
void fxlabMechosIsolation(Mechos* p);
void fxlabMechosFireDestruction(Body* p);
void fxlabMechosGlowDestruction(Mechos* p);
void fxlabDustSource(const Vect3f& p, float speed, int terrain_type, Mechos* owner);
void fxlabDamageWarinig(Vect3f& v,class Mechos* owner,int damage_style);
void fxlabNitroSource(class Mechos* owner);
void fxlabJumpSource(class Mechos* owner);
void fxlabCharacterTeleportation(Vect3f v0,Vect3f v1);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Static members
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sColor4f TrailColors[TrailTypeMax];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Constructor & initialization
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mechos::Mechos(const char* KindsStr)
: fxlabMechosInfo(*new fxlabMechosProcessInformation)
{
	type_ = MECHOS;
	enabled_fields = ~0;
	features_ = NO_FEATURES;
//	simple_integrator = 0;
//	damage_ability_ = 1.f;
//	hit_damage_ability_ = 1.f;

	int Kinds[NUM_PARTS];
	XBuffer buf(KindsStr, strlen(KindsStr));
	int i;
	for(i = 0; i < NUM_ACTIVE_PARTS; i++){
		buf >= Kinds[i];
		if(!Kinds[i])
			Kinds[i] = 1;
		}
#ifndef _FINAL_VERSION_
	extern int mechos_default_config_counter;
	for(i = 0; i < NUM_ACTIVE_PARTS; i++)
		if(mechos_default_config_enable && mechos_default_config[i].array[i])
			Kinds[i] = mechos_default_config[mechos_default_config_counter].array[i];
	mechos_default_config_counter = (mechos_default_config_counter + 1) % 5;
	if(enable_random_assembling){
		static int skip;
		for(int w = 0; w < mchCurrentWorld; w++)
			for(int t = 0; t < mchCurrentTrack; t++)	
				skip = random(10);
		for(int i = 0; i < NUM_ACTIVE_PARTS; i++){
		repeate:
			Kinds[i] = random(random_assembling_kinds[i].size) + 1;
			for(int j = 0; j < random_assembling_kinds[i].n_excluded; j++)
				if(Kinds[i] == random_assembling_kinds[i].excluded[j])
					goto repeate;
			fout <= Kinds[i] < " ";
			}
		fout < "\n";
		}
#endif
	Kinds[PART_INDEX(M3D_LF_WHEEL)] = Kinds[PART_INDEX(M3D_RF_WHEEL)];
	Kinds[PART_INDEX(M3D_LB_WHEEL)] = Kinds[PART_INDEX(M3D_RB_WHEEL)];

	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(Kinds[0], M3D_ENGINE));
	geometry -> attachDummyChildren(Kinds);
	registerGeometry();	

	memset(attracted_parts,0,sizeof(attracted_parts));
	TrailColorsInit();
	
	speed_increment_factor = 1;
	bound_type = CenteredBottomAlignmentedBound; // personage
	set_control_config(0);
}

void Mechos::setPositionInit()
{
	swap(wheels_spheres, spheres);
	setTrans(max_height_threshould, Z_AXIS);
	place_on_the_ground();

	PointControlledObject::setPositionInit();
	setTrans(R().z - mechos_wheel_penetration, Z_AXIS);
	swap(wheels_spheres, spheres);
	post_quant();
	if(!control_points.empty())
		target = control_points.front();
}

void Mechos::registerGeometry()
{
	personage_index = M3D_KIND(part_by_index(Engine) -> Type) - 1;
	(PersonageDifferetiationData&)(*this) =	personage_diff_data[personage_index];

	setGeometry(geometry);
	set_archimedean_force(mechos_buoyancy);
	I.x *= moment_of_inertia_XY_scaling_factor;
	I.y *= moment_of_inertia_XY_scaling_factor;
	I.z *= moment_of_inertia_Z_scaling_factor;
	I_inv = Vect3f::ID/I;

	setAlpha(1.f);
	
	content = 0;
	int i;
	for(i = 0; i < NUM_PARTS; i++)
		if(part_by_index(i))
			content |= M3D_TYPE(part_by_index(i) -> Type);

	energy = energy_max();
	state_ = NORMAL_MECHOS;
	enabled_fields = ~0;
	completed_ = 1;
	visible_ = 1;
	is_dynamic_ = 1;
	features_ = NO_FEATURES;
	forward_wheels_phase = backward_wheels_phase = 0;
	obstacle_x_avr = obstacle_y_avr = obstacle_avr = obstacle_rudder_t_avr =  0;
	manual_essence_turn = 0;
	engineSound = 0;
	startEngineSound();
	restitution_plus_one = mechos_k_restitution_plus_one;
	
	forward_wheels_quant = forward_wheels_mode() == WHEEL ? &Mechos::forward_rotated_quant : &Mechos::forward_foot_quant;
	backward_wheels_quant = backward_wheels_mode() == WHEEL ? &Mechos::backward_rotated_quant : &Mechos::backward_foot_quant;
	sliding_factor = 1;
	wheels_colliding = 0;
	trail_type = OrdinaryTrail;
	
	part_offsets[0] = MatXf::ID;
	for(i = 1; i < NUM_PARTS; i++)
		part_offsets[i] = part_by_index(i) -> LocalMatrix;
	swing_phase = 0;
	character_angles.set(0.0);
	character_angles_velocity.set(0.0);
	velocity_y_prev = 0;
	sensor_source = 0;
	effects_queue.clear();
	if(current_effect != StoneHitEffect)
		runEffect(NoEffect);
	acceleration_spot_direction.set(0.0);
	formic_transport_finish_angle = 0;
	rotation_angle = 0;
	alpha = 1;
	jabasoma_jumping = 0;
	assembling_direction = Vect3f::J;
	beep_pressed = 0;

	left_rudder_angle = right_rudder_angle = 0;
	forward_wheels_phase = backward_wheels_phase = 0;
	assembling_point_dist = 0;
	dV_applied.set(0.0);
	reset_stopping();
	jabasoma_jumping_log = 0;

//	PointControlledObject::target_achieved();
}

Mechos::~Mechos()
{
	delete &fxlabMechosInfo;
}

void Mechos::release_body_handles()
{
	for(int i = 0; i < NUM_PARTS; i++)
		attracted_parts[i] = 0;
	ControlledObject::release_body_handles();
}

void Mechos::set_activity(int flag) 
{
	active_ = flag;
	startEngineSound();
	setPermanentAlpha(1.f);
	if(geometry)
		if(active() && allow_transparency_of_bridges_by_camera && allow_transparency_of_bridges)
			geometry -> SetAttribute(MESH_FIRST_DRAW);
		else
			geometry -> ClearAttribute(MESH_FIRST_DRAW);
}

void Mechos::createBound()
{
	int forward_kind = forward_wheels_mode() == WHEEL ? 0 : change_forward_wheels_kind(1);
	int backward_kind = backward_wheels_mode() == WHEEL ? 0 : change_backward_wheels_kind(3);

	int w;
	for(w = RFWheel; w <= LBWheel; w++)
		part_by_index(w) -> LocalMatrix.trans().z -= mechos_wheel_z_offset;

	MatXf X = geometry -> LocalMatrix;
	geometry -> SetPosition( MatXf( Mat3f::ID, Vect3f::ZERO ) );
	
	float xmax = (part_by_index(LBWheel) -> GlobalMatrix.trans().x - part_by_index(RBWheel) -> GlobalMatrix.trans().x)/2*mechos_xsize_scale;
	float xmin = -xmax;
	float ymin = part_by_index(RBWheel) -> GlobalMatrix.trans().y - part_by_index(RBWheel) -> zmaxReal(); 
	float ymax = part_by_index(RFWheel) -> GlobalMatrix.trans().y + part_by_index(RFWheel) -> zmaxReal();
	float zmin = geometry -> zminTotal();
	float zmax = geometry -> zmaxTotal()*mechos_zsize_scale;

	// Поправка для персонажей, ассиметричных по X
	float delta_x = -(part_by_index(RBWheel) -> GlobalMatrix.trans().x + part_by_index(LBWheel) -> GlobalMatrix.trans().x)/2;

	// Сдвигаем вперед ЦМ
	float delta_y = -(ymax + ymin)/2;
	ymin += delta_y;
	ymax += delta_y;

	// Опускаем ЦМ
	float z_axles = 0;
	for(w = RFWheel; w <= LBWheel; w++)
		z_axles += part_by_index(w) -> GlobalMatrix.trans().z;
	z_axles /= 4;
	float delta_z = -z_axles - mechos_CM_z_offset;
	zmin += delta_z;
	zmax += delta_z;

	mesh_offset.set(delta_x, delta_y, delta_z);
	
	geometry -> SetPosition( MatXf( Mat3f::ID, mesh_offset ) );

	polyhedrons.clear();
	cMesh *mesh = MeshLibrary -> Find(myM3DType(100));
	Vect3f max0(mesh -> xmaxTotal(), mesh -> ymaxTotal(), mesh -> zmaxTotal());
	Vect3f min0(mesh -> xminTotal(), mesh -> yminTotal(), mesh -> zminTotal());
	Vect3f min1(xmin, ymin, zmin);
	Vect3f max1(xmax, ymax, zmax);
	Vect3f scale = (max1 - min1)/(max0 - min0);
	Vect3f displace = (max0*min1 - min0*max1)/(max0 - min0);
	Polyhedron* poly = new Polyhedron(*(*polyhedron_lib)[mesh -> Type], scale, displace );
	polyhedrons.add(poly);

	setMass((*polyhedron_lib)[part_by_index(Engine) -> Type] -> vol()*mechos_density*(features() & HUMBLE ? humble_mass_scale : 1));

	// Setting wheels and spheres
	spheres.clear();
	spheres.reserve(6);
	wheels_spheres.clear();
	wheels_spheres.reserve(4);

	cMesh* fm = part_by_type(M3D_RF_WHEEL);
	Vect3f fv = fm -> GlobalMatrix.trans();
	fv.z += mechos_wheel_radius - fm -> zmaxReal();
	wheels[WheelRF].position = fv;
	fv.x = -fv.x;
	wheels[WheelLF].position = fv;
//	fv = fm -> GlobalMatrix.trans();

	cMesh* bm = part_by_type(M3D_RB_WHEEL);
	Vect3f bv = bm -> GlobalMatrix.trans();
	bv.z += mechos_wheel_radius - bm -> zmaxReal();
	wheels[WheelRB].position = bv;
	bv.x = -bv.x;
	wheels[WheelLB].position = bv;
//	bv = bm -> GlobalMatrix.trans();
	fv.x = bv.x;

	spheres.push_back(Sphere(Vect3f(0, 0, (fv.z + bv.z)/2 + mechos_engine_sphere_dz), mechos_engine_sphere_radius));

	fv.x += mechos_border_sphere_dx;
	fv.y += mechos_border_sphere_dy;
	fv.z += mechos_border_sphere_dz;
	spheres.push_back(Sphere(fv, mechos_border_sphere_radius, BorderSphere));
	fv.x = -fv.x;
	spheres.push_back(Sphere(fv, mechos_border_sphere_radius, BorderSphere));
	fv.x = 0;
	fv.y += mechos_front_sphere_dy;
	fv.z += mechos_front_sphere_dz - mechos_border_sphere_dz;
	spheres.push_back(Sphere(fv, mechos_front_sphere_radius, BorderSphere));

	bv.x += mechos_border_sphere_dx;
	bv.z += mechos_border_sphere_dz;
	spheres.push_back(Sphere(bv, mechos_border_sphere_radius, BorderSphere));
	bv.x = -bv.x;
	spheres.push_back(Sphere(bv, mechos_border_sphere_radius, BorderSphere));

	for(int i = 0; i < 4; i++){
		Wheel& w = wheels[i];
		w.init(mechos_wheel_mass, mechos_wheel_radius, mechos_wheel_penetration, mechos_suspension_damping_scale, this);
		wheels_spheres.push_back(Sphere(w.position, w.radius, RFWheel + i));
		}
		
	if(forward_kind)
		change_forward_wheels_kind(forward_kind);
	if(backward_kind)
		change_backward_wheels_kind(backward_kind);

	geometry -> SetPosition(X);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Destroy-Assembling Mechanism
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::disorganize()
{
	if(energy < 0)
		energy = 0;

	if(features() & NON_DESTRUCTING || !geometry)
		return;

	if(features() & HUMBLE)
		reset_feature(HUMBLE);

	if(active() && state() == NORMAL_MECHOS)
		startSound(EFF_DISORGANIZE);

	stopSounds();

	fxlabMechosGlowDestruction(this);

	for(int i = NUM_PARTS - 1; i >= 1; i--){
		if(attracted_parts[i]){
			attracted_parts[i] -> targetMechos = 0;
			attracted_parts[i] = 0; 
			}
		if(part_by_index(i)){
			Part* pp = detachPart(1 << i, 0);
			if(active()) fxlabMechosFireDestruction(pp);
			}
		}

	setTrans(R().z + mesh_offset.z, Z_AXIS);
	mesh_offset.set(0.0);
	Body::createBound();
	set_archimedean_force(personage_buoyancy);

	state_ = STAYING_PERSONAGE;
	enabled_fields = 0;
	completed_ = 0;
	visible_ = 0;
	quants.clear();
	forces.clear();
	features_ = NO_FEATURES;
	clearVelocities();
	setRot(-psi(), Z_AXIS);
	speed_avr = 0;
	velocity_y_avr = 0;
	after_disorganize_latency.start(disorganize_duration);	
	make_non_colliding();

#ifndef _FINAL_VERSION_
	{ int content_ = 0;
	for(int i = 0; i < NUM_PARTS; i++)
		if(part_by_index(i))
			content_ |= M3D_TYPE(part_by_index(i) -> Type);
	xassert(content_ == content); }
#endif
}

Part* Mechos::detachPart(int part_id, int formic)
{
	cMesh* mesh = geometry -> FindMesh(part_id);
	if(mesh){
		xassert(content & part_id);
		content &= ~part_id;
		attracted_parts[PART_INDEX(part_id)] = 0;
		
		mesh -> DetachChild();
		mesh -> SetAnimation(0, 0);

		if(M3D_KIND(mesh -> Type) & (MORPH_KIND | ACTIVATED_KIND)){
			int old_type = mesh -> Type;
			xrealM3D -> RebuildObject(mesh, old_type,  old_type & ~M3D_TOTAL_TYPE((MORPH_KIND | ACTIVATED_KIND), 0) );
			mesh -> ReCalcTotalBound();
			}
		xrealM3D -> Attach(mesh);

		Part* part = new Part(mesh, formic);
  		Mdisp -> attachBody(part);
		if(!formic){
			xassert(M3D_TYPE(mesh -> Type) != M3D_ENGINE);
			Vect3f Vp = getDist(mesh -> GlobalMatrix.trans(), R());
			Vp.Normalize();
			Vp.z += 1;
			Vp.normalize(part_V); 
			Vp += Vglobal();
			part -> setGlobalVelocity(Vp);

			Vect3f Wp;
			Wp.x = frnd(part_W);
			Wp.y = frnd(part_W);
			Wp.z = frnd(part_W);
			Wp.normalize(part_W);
			Wp += Wglobal();
			part -> setGlobalAngularVelocity(Wp);
			}
		return part;
		}
	return 0;
}

Part* Mechos::searchPart(int m3dType)
{
	int m3dKind = 0;
	Part* rw;
	cMesh* rwm;
	if(m3dType == M3D_LF_WHEEL){
		if((rwm = part_by_type(M3D_RF_WHEEL)) != 0)
			m3dKind = M3D_KIND(rwm -> Type);
		else
			if((rw = attracted_parts[PART_INDEX(M3D_RF_WHEEL)]) != 0)
				m3dKind = M3D_KIND(rw -> geometry -> Type);
			else 
				return 0;
		}
	if(m3dType == M3D_LB_WHEEL){
		if((rwm = part_by_type(M3D_RB_WHEEL)) != 0)
			m3dKind = M3D_KIND(rwm -> Type);
		else
			if((rw = attracted_parts[PART_INDEX(M3D_RB_WHEEL)]) != 0)
				m3dKind = M3D_KIND(rw -> geometry -> Type);
			else 
				return 0;
		}

	Part* part = Mdisp -> searchPart(*this, m3dType, m3dKind);
	if(!part)
		return 0;
	cMesh *mesh = 0;								  
	if(geometry){
		mesh = geometry -> FindMesh(M3D_PARENT(M3D_TYPE(part -> geometry -> Type)));
		part -> preLink(this, mesh);
		}
	else
		part -> preLink(this, target);
	attracted_parts[PART_INDEX(m3dType)] = part;
	return part;
}

void Mechos::addPart(cMesh* geometry)
{
	if(active())
		startSound(EFF_JOIN_PART);
	int Type = M3D_TYPE(geometry -> Type);
	content |= Type;
	attracted_parts[PART_INDEX(Type)] = 0;

#ifndef _FINAL_VERSION_
	int content_ = 0;
	for(int i = 0; i < NUM_PARTS; i++)
		if(part_by_index(i))
			content_ |= M3D_TYPE(part_by_index(i) -> Type);
	xassert(content_ == content);
	content = content_;
#endif

	if(content == M3D_TOTAL_MECHOS){
		registerGeometry();
		make_non_colliding();
		}
	else
		if(parts_collision_flags & 1){
			cMesh* p = geometry;
			while(p -> Parent) 
				p = p -> Parent;
			MatXf X = p -> GlobalMatrix;
			p -> SetPosition(MatXf::ID);
			Polyhedron* poly = new Polyhedron(*(*polyhedron_lib)[geometry -> Type], Vect3f::ID, geometry -> GlobalMatrix.trans() );
			polyhedrons.add(poly);
			p -> SetPosition(X);

			spheres.push_back(Sphere(poly -> center(), poly -> rad()));
			}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Quant-functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::quant()
{
	start_autostop_timer(Mechos_quant, STAT_MECHOS);

#ifndef _FINAL_VERSION_
	int content_ = 0;
	for(int i = 0; i < NUM_PARTS; i++)
		if(part_by_index(i))
			content_ |= M3D_TYPE(part_by_index(i) -> Type);
	xassert(content_ == content);
#endif

	effectQuant();
	switch(state()){
		case NORMAL_MECHOS: 
			quant_of_normal_mechos();
			break;
		
		case STAYING_PERSONAGE:
			if(num_control_points() && (colliding() || submersion() > 0.1f) && !after_disorganize_latency)
				if(getDist(target, R()).norm() < choose_teleportation_distance_xy_threshould && 
				   fabs(target.z - R().z) < choose_teleportation_distance_z_threshould && 
				   !(affecting_fields() & FieldSource::BubbleField) && 
				   (submersion() < 0.1f || Mdisp -> waterDamage() < 100)){
					state_ = MOVING_PERSONAGE;
					assembling_point_dist = getDist(target, R()).norm();
					assembling_point_approach_timer.stop();
					}
				else
					start_teleportation();
			break;
	
		case MOVING_PERSONAGE: {
			if(!num_control_points())
				state_ = STAYING_PERSONAGE;

			float dist = getDist(target, R()).norm();
			if(assembling_point_approach_timer(assembling_point_dist - dist < assembling_point_dist_eps, choose_teleportation_time_threshould))
				start_teleportation();
			if(assembling_point_dist > dist)
				assembling_point_dist = dist;
			} break;

		case ASSEMBLING_MECHOS:
			quant_of_assembling_mechos();
			break;
		}
}

void Mechos::quant_of_normal_mechos()
{
	start_autostop_timer(quant_of_normal_mechos, STAT_MECHOS);

	if(Energy() <= 0){
		disorganize();
		return;
		}

	QuantList::iterator qi;
	FOR_EACH(quants, qi)
		if(!((this ->* (*qi))()))
			qi = --quants.erase(qi);

	if(active()){
		if(submersion() > 0.1)
			startSound(EFF_MECHOS_WATER);
		else
			stopSound(EFF_MECHOS_WATER);

		if(features() & RED_TRACK_FEATURE)
			startSound(EFF_SCARLET_TRACE);
		}

	float sf;
	if(is_dynamic() && engine_sound_timer(engineSound && submersion() < 0.3 && (sf = speedSoundFactor()) > mechos_sound_factor_low_threshould, mechos_engine_sound_on_delay, mechos_engine_sound_off_delay))
		startSound(engineSound, 1, sf);
	else
		stopSound(engineSound);
	
	if(!slime_timer)
		trail_type = OrdinaryTrail;
	if(affecting_fields() & (FieldSource::RAGE_OF_SLIME | FieldSource::GREEN_SLIME | FieldSource::YELLOW_SLIME | FieldSource::BLUE_SLIME)){
		slime_timer.start(slime_duration);
		slime_obstacle();
		}
	if(affecting_fields() & FieldSource::GREEN_SLIME)
		trail_type = GreenSlimeTrail;
	if(affecting_fields() & FieldSource::YELLOW_SLIME)
		trail_type = YellowSlimeTrail;
	if(affecting_fields() & FieldSource::BLUE_SLIME)
		trail_type = BlueSlimeTrail;
	if(affecting_fields() & FieldSource::RAGE_OF_SLIME){
		essence_slime_timer.start(60000);
		slime_timer.start(slime_duration);
		trail_type = RedSlimeTrail;
		}
	if(affecting_fields() & FieldSource::RED_TRACK)
		damage(red_track_damage);

	if(submersion() > 0.1 && (Mdisp -> waterDamage() > 100 || !(features() & JABASOMA)))
			damage(Mdisp -> waterDamage());
	
	if(sensor_source && !sensor_timer){
		sensorOut(sensor_source);
		sensor_source = 0;
		}

	#ifndef _FINAL_VERSION_			
	changePart();
	#endif
} 

void Mechos::quant_of_assembling_mechos()
{
	if(!(features() & NON_CONSTRUCTING) && !after_disorganize_latency){
		if(!geometry){
			if(!attracted_parts[0])
				searchPart(M3D_ENGINE);
			return;
			}
		else{
			for(int i = 1; i < NUM_PARTS; i++)
				if(!part_by_index(i)){
					if(!attracted_parts[i] && part_by_type(M3D_PARENT(1 << i)))
						searchPart(1<< i);
					 }
			}
		}
 }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		calc_forces_and_drags - functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::calc_forces_and_drags()
{
	dragV.set(mechos_dragV_free);
	dragW.set(mechos_dragW_free);

	switch(state()){
		case NORMAL_MECHOS:	
			calc_forces_and_drags_of_normal_mechos();
			break;
		case MOVING_PERSONAGE:	
			calc_forces_and_drags_of_moving_personage();
			break;
		case WAITING_FOR_TELEPORTATION:
			if(!teleportation_timer){
				state_ = STARTING_TELEPORTATION;
				teleportation_timer.start(starting_teleportation_time);
				teleportation_psi = psi();
				if(active())
					startSound(EFF_ROTATION);
				}
			break;
		case STARTING_TELEPORTATION:
			setLocalAngularVelocity( teleportation_spin*(1 - teleportation_timer()/starting_teleportation_time) + teleportation_spin_initial, Z_AXIS);
			setPermanentAlpha(teleportation_timer()/starting_teleportation_time);
			geometry -> SetScale(teleportation_size - (teleportation_size - 1)*teleportation_timer()/starting_teleportation_time);
			if(!teleportation_timer){
				state_ = FINISHING_TELEPORTATION;
				setTrans(target);
				place_on_the_ground();
				teleportation_timer.start(finishing_teleportation_time);
				after_disorganize_latency.start(finishing_teleportation_time + 2000);
				}
			break;
		case FINISHING_TELEPORTATION:
			setLocalAngularVelocity(teleportation_spin*teleportation_timer()/starting_teleportation_time + teleportation_spin_initial, Z_AXIS);
			setPermanentAlpha(1 - teleportation_timer()/starting_teleportation_time);
			geometry -> SetScale(1 + (teleportation_size - 1)*teleportation_timer()/finishing_teleportation_time);
			if(!teleportation_timer && (fabs(getDist_f(teleportation_psi, psi(), 2*M_PI)) < teleportation_psi_error || !after_disorganize_latency)){
				state_ = ASSEMBLING_MECHOS;
				features_ &= ~ROTATING;
				setRot(-teleportation_psi, Z_AXIS);
				setLocalAngularVelocity(0, Z_AXIS);
				ControlPointList::iterator i = ++control_points.begin();
				assembling_direction = i != control_points.end() ? getDist(*i, control_points.front()) : Yglobal();
				if(!mchTurnBasedGame)
					RemoveSeeds();
				}
			break;
		case ASSEMBLING_MECHOS:	
			calc_forces_and_drags_of_assembling_personage();
			break;
		}

	if(state() != NORMAL_MECHOS){
		switch(state()){
			case STAYING_PERSONAGE:
			case WAITING_FOR_TELEPORTATION:
			case STARTING_TELEPORTATION:
			case FINISHING_TELEPORTATION:
			case ASSEMBLING_MECHOS:	
				if(ground_colliding && !in_contact || submersion() > 0.1f)
					dragV += standing_personage_dragV;
				dragW += standing_personage_dragW;
				applyLocalTurnTorque(Zlocal(), Vect3f::K);
				break;
			}

		switch(state()){
			case MOVING_PERSONAGE:
			case STAYING_PERSONAGE:
			case WAITING_FOR_TELEPORTATION:
			case STARTING_TELEPORTATION:
			case FINISHING_TELEPORTATION:
			case ASSEMBLING_MECHOS:	
				if(submersion() > 0.1 && !colliding()){
					swing_phase += evolve_time_step*(1. + speed_avr/20);
					applyGlobalForce(mechos_swing_lift_force*sin(swing_phase), Z_AXIS);
					applyGlobalTorque(mechos_swing_torque*sin(swing_phase + mechos_swing_phase), X_AXIS);
					}
				break;
			}
		}

	if(affecting_fields() & FieldSource::BubbleField){
		dragV += Vect3f(bubble_field_damping);
		dragW += Vect3f(bubble_field_damping);
		}

#ifndef _FINAL_VERSION_
	if(xreal_log){
		write_state(bout);
		bout < "XRndVal: " <= XRndVal < "\n";
		bout < "randomVAL: " <= xm_random_generator() < "\n";
		for(int i = Front; i <= RBWheel; i++)
			bout < "Mana[" <= i < "]" <= (part_by_index(i) ? part_by_index(i) -> arcaneData.Power : 0) < "\n";
		bout < "\n";
		}
#endif
}

void Mechos::calc_forces_and_drags_of_normal_mechos()
{
	dV_applied += dV_constraint;
	dV_applied.z = 0;
	Alg().invXform(dV_applied);
	float Vy = Vlocal().y;
	Vy -= dV_applied.y*mechos_collision_correction_restitution;
	setLocalVelocity(Vy, Y_AXIS);
	dV_applied.set(0.0);

	control();
	
	// Установка на колеса
	if(theta() > M_PI/3)
		applyLocalTurnTorque(Zlocal(), Vect3f::K);

	sliding_factor = 1;
	
	applyWheelsForce();

	//  Additional Forces
	QuantList::iterator qi;
	FOR_EACH(forces, qi)
		if(!((this ->* (*qi))()))
			qi = --forces.erase(qi);

	// Water 
	if(submersion() > 0.1){
		dragV += mechos_dragV_water;
		dragW += mechos_dragW_water;
		if(brake)
			dragV.y += mechos_water_brake_damping;
		if(!(features() & JABASOMA)){
			applyLocalForce( traction*speed_increment_factor*k_traction_water, Y_AXIS);
			applyLocalTorque( rudder*k_rudder_water*SIGN(Vlocal().y + traction*5), Z_AXIS);
			}
		// Покачивание
		if(!ground_colliding){
			swing_phase += evolve_time_step*(1. + speed_avr/20);
			applyGlobalForce(mechos_swing_lift_force*sin(swing_phase), Z_AXIS);
			applyGlobalTorque(mechos_swing_torque*sin(swing_phase + mechos_swing_phase), X_AXIS);
			}
		}

	if(affecting_fields() & FieldSource::WILD_THICKET)
		dragV.y += wild_thicket_drag;
	if(affecting_fields() & FieldSource::Stream){
		dragV.set(0.5);
		dragW.set(1);
		}
	if(affecting_fields() & FieldSource::Magnetic)
		dragV.y += 1;

	if(features() & HUMBLE){
		dragV += Vect3f(humble_drag);
		if(!humble_timer)
			reset_feature(HUMBLE);
		}

	// Borders
	ContactList::iterator ci;
	FOR_EACH(contacts,ci){
		Contact& c = *ci;
		if(fabs(c.normal.z) < mechos_obstacle_slope_z){
			average(obstacle_avr, 1.f, obstacle_avr_tau);
			average(obstacle_x_avr, c.cp1.x, obstacle_avr_tau);
			average(obstacle_y_avr, c.cp1.y, obstacle_avr_tau);

			//static XStream fout("lst", XS_OUT);

			int double_contact = 0;
			Body* body2 = static_cast<Body*>(c.body2);
			Vect3f vel = global_velocity(c.cp1g);
			if(body2){
				vel -= body2 -> global_velocity(c.cp2g);
				if(body2 -> is_dynamic())
					double_contact = 1;
				}

			// Horizontal analytics
			Vect3f normal;
			if(!double_contact){
				normal = -c.cp1;
				normal.z = 0;
				normal.x *= horizontal_ellipsoid_factor;
				normal.normalize();
				Alg().xform(normal);
				}
			else
				normal = c.normal;
			float u_n = dot(vel, normal) - horizontal_velocity_relaxation_value;
			if(u_n < 0){
				Mat3f Ig_inv1(Ig_inv), Ig_inv2;
				Ig_inv[0].set(0.0);
				Ig_inv[1].set(0.0);
				Ig_inv[2] *= horizontal_Ig_inv_z_factor;
				Mat3f m1;
				float Aii = dot(normal, global_mass_matrix(c.cp1g, c.cp1g, m1)*normal);
				if(body2 && body2 -> is_dynamic()){
					if(body2 -> type() == MECHOS){
						Ig_inv2 = body2 -> Ig_inv;
						body2 -> Ig_inv[0].set(0.0);
						body2 -> Ig_inv[1].set(0.0);
						body2 -> Ig_inv[2] *= horizontal_Ig_inv_z_factor;
						}
					{
						Mat3f m1;
						Aii += dot(normal, body2 -> global_mass_matrix(c.cp2g, c.cp2g, m1)*normal);
					}
					}
				float j_n = -u_n*horizontal_k_restitution_plus_one/(Aii + 0.001);
				Vect3f J = normal*j_n;
				V.scaleAdd(J, mass_inv);
				W += Ig_inv*(c.cp1g % J);
				if(body2 && body2 -> is_dynamic()){
					body2 -> V.scaleAdd(J, -body2 -> mass_inv);
					body2 -> W -= body2 -> Ig_inv*(c.cp2g % J);
					if(body2 -> type() == MECHOS)
						body2 -> Ig_inv = Ig_inv2;
					}
				Ig_inv = Ig_inv1;
				}
				
			// Vertical analytics
			Vect3f z_axis = Vect3f::K;
			float u_z = dot(vel, z_axis) + vertical_velocity_relaxation_value;
			if(enable_vertical_analytics && u_z > 0){
				Mat3f m1;
				float Aii = dot(z_axis, global_mass_matrix(c.cp1g, c.cp1g, m1)*z_axis);
				if(body2)
				{
					Mat3f m2;
					Aii += dot(z_axis, body2 -> global_mass_matrix(c.cp2g, c.cp2g, m2)*z_axis);
				}
				float j = -u_z/(Aii + 0.001);
				Vect3f J = z_axis*j;
				V.scaleAdd(J, mass_inv);
				W += Ig_inv*(c.cp1g % J);
				if(body2){
					body2 -> V.scaleAdd(J, -body2 -> mass_inv);
					body2 -> W -= body2 -> Ig_inv*(c.cp2g % J);
					}
				}
			}
		}
}

void Mechos::calc_forces_and_drags_of_moving_personage()
{
	traction = 1;
	ControlledObject::control();

	dragV = personage_dragV;
	dragW = personage_dragW;

	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	applyLocalTorque(personage_rudder*rudder*(Vlocal().y + 5), Z_AXIS);
	if(colliding())
		applyGlobalForce(1.1*Gravity, Z_AXIS);
	if(colliding() || submersion() > 0.01f)
		applyLocalForce(personage_traction*traction, Y_AXIS);
}

void Mechos::calc_forces_and_drags_of_assembling_personage()
{
	// Ручной поворот сущности
	if(active() && (mchKeyPressed(MCH_KEY_TURN_LEFT, control_config) || mchKeyPressed(MCH_KEY_TURN_RIGHT, control_config))){
		manual_essence_turn = 1;
		setGlobalAngularVelocity((mchKeyPressed(MCH_KEY_TURN_RIGHT, control_config) ? 1 : -1)*standing_personage_manual_rudder, Z_AXIS);
		}

	// Поворот на семя
	if(!manual_essence_turn){
		Vect3f r;
		Alg().invXform(assembling_direction, r);
		setGlobalAngularVelocity(atan2(-r.x, r.y)*standing_personage_automatic_rudder, Z_AXIS);
		}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Non-dynamic features
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Mechos::isFlyingByArcane() const
{
	return (features() & (BUTTERFLY | DIRIGIBLE | JABASOMA | DRAGON_POWER | HELICOPTER | FLYING_BY_JUMP_PAD)) || cannon_ball_timer() | jump_timer();
}

void Mechos::set_feature(Feature feature)
{
	if(content != M3D_TOTAL_MECHOS || features() & feature)
		return;
	switch(feature){
		case BUTTERFLY:
			butterfly_start();
			break;

		case JABASOMA:
			jabasoma_start();
			break;

		case DRAGON_POWER:
			dragon_power_start();
			break;

		case MAGNETIC_CUSHION:
			forces.push_back((QuantFunction)&Mechos::magnetic_cushion_force);
			stopEngineSound();
			enabled_fields &= ~FieldSource::Stream;
			break;

		case NULIFICATOR:
			visible_ = 0;
			break;

		case STATIC_NULIFICATOR:
			//RemoveSeeds();
			is_dynamic_ = 0;
			visible_ = 0;
			submersion_ = 0;
			break;

		case IRON_HIP:
			iron_hip_timer.start();
			part_by_type(M3D_FRONT) -> SetFirstAnimation(0, 0);
			forces.push_back((QuantFunction)&Mechos::iron_hip_quant);
			break;

		case DIRIGIBLE:
			forward_wheels_quant = &Mechos::forward_non_rotated_quant;
			set_forward_wheels_animation(1000, -1, 1);
			forces.push_back((QuantFunction)&Mechos::dirigible_quant);
			stopEngineSound();
			break;
		
		case HELICOPTER:
			if(!(features() & HELICOPTER))
				helicopter_start();
			break;

		case RED_TRACK_FEATURE:
			red_track_arcane_statistics = ArcaneStatisticsService();
			break;

		case HUMBLE: 
			if(!(features() & HUMBLE)){
				for(int i = Front; i < NUM_PARTS; i++){
					cMesh* mesh = part_by_index(i);
					mesh -> SetPosition(Mat3f::ID, mesh -> CalcDummyOffset(mesh -> Parent));
					}
				setScale(humble_scale);
				features_ |= HUMBLE;
				registerGeometry();
				make_non_colliding();
				humble_timer.start(humble_duration);
				startSound(EFF_PETARD_MINIMIZE);
				}
			break;
		}
	features_ |= feature;
}				

void Mechos::reset_feature(Feature feature)
{
	if(content != M3D_TOTAL_MECHOS)
		return;
	switch(feature){
		case BUTTERFLY:
			butterfly_stop();
			break;

		case JABASOMA:
			jabasoma_stop();
			break;

		case DRAGON_POWER:
			dragon_power_stop();
			break;

		case MAGNETIC_CUSHION:
			forces.remove((QuantFunction)&Mechos::magnetic_cushion_force);
			startEngineSound();
			enabled_fields |= FieldSource::Stream;
			break;

		case NULIFICATOR:
			visible_ = 1;
			break;

		case STATIC_NULIFICATOR:
			features_ &= ~feature;
			if(state() == NORMAL_MECHOS){
				is_dynamic_ = 1;
				visible_ = 1;
				}
			return;

		case IRON_HIP:
			part_by_type(M3D_FRONT) -> SetFirstAnimation(0, 0);
			forces.remove((QuantFunction)&Mechos::iron_hip_quant);
			break;

		case DIRIGIBLE:
			forward_wheels_quant = &Mechos::forward_rotated_quant;
			set_forward_wheels_animation(-1000, -1, 0);
			forces.remove((QuantFunction)&Mechos::dirigible_quant);
			stopSound(EFF_DIRIGIBLE_ADD);
			startEngineSound();
			break;

		case HELICOPTER:
			if(features() & HELICOPTER)
				helicopter_stop();
			break;

		case RED_TRACK_FEATURE:
			stopSound(EFF_SCARLET_TRACE);
			break;

		case HUMBLE: {
			for(int i = Front; i < NUM_PARTS; i++){
				cMesh* mesh = part_by_index(i);
				mesh -> SetPosition(Mat3f::ID, mesh -> CalcDummyOffset(mesh -> Parent));
				}
			setScale(1);
			features_ &= ~HUMBLE;
			registerGeometry();
			make_non_colliding();
			startSound(EFF_PETARD_MAXIMIZE);
			break; }
		}
	features_ &= ~feature;
}			

void Mechos::damage(float dE, int damage_style)
{
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "damage: " <= dE < ", " <= damage_style < "\n";
#endif

	if(state() != Mechos::NORMAL_MECHOS)
		return;
	if(features() & ELASTIC_HIP)
		return;
	if(features() & IRON_HIP)
		dE /= 2;

	if(features() & PROTECT_FROM_DAMAGE)
		return;

	switch(damage_style){
		case GENERIC_DAMAGE:
			break;
		case FIRE_DAMAGE:
			if(features() & FIRE_PROTECTION)
				return;
			fxlabMechosFireProcess(this);
			break;
		case LIGHTNING_DAMAGE:
			if(features() & LIGHTNING_PROTECTION)
				return;
			break;
	}

	if(dE > 0){
		if((energy -= dE) < 0)
			energy = 0;		
		if(features() & NON_DESTRUCTING && energy < 1)
			energy = 1;
		Vect3f v1(R());
		fxlabDamageWarinig(v1,this,damage_style);
		}
}

void Mechos::charge(float dE)
{
	if(energy < energy_max())
		if(dE > 0 && (energy += dE) > energy_max())
			energy = energy_max();
}

void Mechos::body_overlap_reaction(Body& body)
{
	if(features() & (NULIFICATOR | STATIC_NULIFICATOR))
		return;

	if(state() == NORMAL_MECHOS){
		damage(body.damage_ability(*this));
		switch(body.type()){
			case WORM:
				slime_timer.start(slime_duration);
				trail_type = GreenSlimeTrail;
				slime_obstacle();
				break;
			case CATERPILLAR:
				if(dynamic_cast<OwnerProtection&>(body).owner() != ID){
					slime_timer.start(slime_duration);
					trail_type = YellowSlimeTrail;
					slime_obstacle();
					}
				break;
			case LITTLE_SPIDER:
				if(dynamic_cast<OwnerProtection&>(body).owner() != ID)
					animal_obstacle();
				break;
				
			case BONUS_METEORITE:
				if(dynamic_cast<OwnerProtection&>(body).owner() != ID)
					animal_obstacle();
				break;

			case PIG:
			case SHARK:
			case FISH:
			case PENGUIN:
			case SPIDER_RACER:
				animal_obstacle();
				break;
			}
		}

	if(state() == ASSEMBLING_MECHOS && !(parts_collision_flags & 1) && body.type() == MECHOS){
		Mechos& m = dynamic_cast<Mechos&>(body);
		if(m.state() == NORMAL_MECHOS){
			//if(m.num_control_points())
			//	disorganize();
			//else{
				Vect3f r = getDist(m.R(), R());
				float d = r.norm();
				if(d > DBL_EPS)
					m.applyGlobalForce(r*.5/d);
			//	}
			}
		}
}

int Mechos::is_colliding_with(const Body& body) 
{ 
	//if(features() & (NULIFICATOR | STATIC_NULIFICATOR) || state() != NORMAL_MECHOS)
	if(features() & (NULIFICATOR | STATIC_NULIFICATOR))
			return 0;
	return 1; 
}

float Mechos::damage_ability(const Body& b) const 
{ 
	if(state() == NORMAL_MECHOS)
		return features() & IRON_HIP ? 2*damage_ability_ : damage_ability_;
	else 
		return 0;
}

void Mechos::body_hit_reaction(const Body& body, const Contact& c)
{
	if(features() & FROZEN && -c.u_n > frozen_mechos_hit_velocity_threshould)
		damage(1000);
	damage(body.hit_damage_ability(*this, -c.u_n));
	switch(body.type()){
		case MECHOS:
			if(active())
				startSound(EFF_MECHOS_HIT, -c.u_n*mechos_hit_velocity2sound_factor);
			if(-c.u_n > collision_effect_velocity_threshould)
				startEffect(CollisionEffect);
			break;

		case STONE:
		case SMART_STONE:
		case TITANIUM_BALL:
			if(-c.u_n > stone_hit_effect_velocity_threshould)
				startEffect(StoneHitEffect);
		case SHIP:
		case PIG:
		case GEAR:
		case BREVNO:
		case TOMATO:
			if(active() && -c.u_n > hit_sound_velocity_threshould)
				startSound(EFF_HIT);
			break;
			}

//	if(-c.u_n > mechos_upper_hit_velocity_threshould && getDist((c.body1 == this ? c.body2 : c.body1) -> R(), R()).theta() < mechos_upper_hit_theta_threshould)
//		startEffect(StoneHitEffect);
}		    

void Mechos::ground_hit_reaction(const Contact& c)																	       
{
	if(features() & FROZEN && -c.u_n > frozen_mechos_hit_velocity_threshould)
		damage(1000);
	if(active())
		if(c.normal.z > 0.5)
			startSound(EFF_TERRAIN_WHEELS_HIT, -c.u_n*terrain_wheels_hit_velocity2sound_factor);
		else
			startSound(EFF_TERRAIN_HIT, -c.u_n*terrain_hit_velocity2sound_factor);
	if(c.normal.z > 0.5 && -c.u_n > 50)
		startEffect(LandingEffect);
}	

void Mechos::post_quant()
{
	Body::post_quant();

	if(!geometry)
		return;

	if(active() && allow_transparency_of_bridges_by_camera && allow_transparency_of_bridges)
		geometry -> SetAttribute(MESH_FIRST_DRAW);
	else
		geometry -> ClearAttribute(MESH_FIRST_DRAW);


	if(state() == NORMAL_MECHOS){
		RungeKutta4<2, float>(character_angles, character_angles_velocity, 
			Vect3f(acceleration_spot_timer() ? character_angle_x_spot_acceleration : character_angle_x_factor*(Vlocal().y - velocity_y_prev), !(features() & ROTATING) ? character_angle_y_factor*Wlocal().z : 0, 0), 
			Vect3f(character_angle_x_mass_inv, character_angle_y_mass_inv, 0) , (float)evolve_time_step);
		velocity_y_prev = Vlocal().y;
		if(character_angles.x < character_angle_x_min)
			character_angles.x = character_angle_x_min;
		if(character_angles.x > character_angle_x_max)
			character_angles.x = character_angle_x_max;
		if(fabs(character_angles.y) > character_angle_y_max)
			character_angles.y = SIGN(character_angles.y)*character_angle_y_max;

		float z_off = geometry -> zminReal();
		MatXf Rot = MatXf(Mat3f::ID, Vect3f(0 , 0, z_off))*MatXf(Mat3f(-character_angles.y, Y_AXIS)*Mat3f(character_angles.x, X_AXIS), Vect3f::ZERO)*MatXf(Mat3f::ID, Vect3f(0 , 0, -z_off));

		geometry -> SetPosition(pose()*MatXf(Mat3f::ID, mesh_offset)*Rot);

		(this ->* forward_wheels_quant)();
		(this ->* backward_wheels_quant)();

		int w;
		for(w = 0; w < 4; w++)
			part_offsets[RFWheel + w].trans().z += wheels[w].suspension_travel();

		Rot.invert();

		for(int i = Front; i < NUM_PARTS; i++)
			if(part_by_index(i) -> Parent == geometry)
				part_by_index(i) -> SetPosition(Rot*part_offsets[i]);
			else
				part_by_index(i) -> SetPosition(part_offsets[i]);

		for(w = 0; w < 4; w++)
			part_offsets[RFWheel + w].trans().z -= wheels[w].suspension_travel();
		}
}	

void Mechos::hide()
{
	Body::hide();
	stopSounds();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Система обратной связи для PointControl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::target_achieved()
{
	switch(state()){
		case NORMAL_MECHOS:
			if(Energy() > 0)
				PointControlledObject::target_achieved();
			else
				disorganize();
			if(Mdisp -> waterDamage() == Forest5_water_damage)
				jabasoma_jumping_log = 0;
			break;
		case MOVING_PERSONAGE: {
			state_ = ASSEMBLING_MECHOS;
			ControlPointList::iterator i = ++control_points.begin();
			assembling_direction = i != control_points.end() ? getDist(*i, control_points.front()) : Yglobal();
			if(!mchTurnBasedGame)
				RemoveSeeds();
			break; 
			}
		}
}
int Mechos::forwardObstacle()
{
	float K = obstacle_y_avr/(fabs(obstacle_x_avr) + DBL_EPS);
	return obstacle_avr > obstacle_avr_threshould && K > 1.;
}
int Mechos::backwardObstacle()
{
	float K = -obstacle_y_avr/(fabs(obstacle_x_avr) + DBL_EPS);
	return obstacle_avr > obstacle_avr_threshould && K > 1.;
}
void Mechos::resetObstacle()
{
	obstacle_avr = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Control
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::control()
{
	average(obstacle_avr, 0, obstacle_avr_tau);
	PointControlledObject::control();
	float t = obstacle_y_avr > 0 ? obstacle_avr*atan(obstacle_y_avr/(fabs(obstacle_x_avr) + DBL_EPS))*mechos_obstacle_rudder_t_factor : 0;
	if(pitch() > G2R(mechos_obstacle_rudder_pitch_threshould))
		t += 1;
	if(!mechos_obstacle_rudder_enabled || fabs(velocity_y_avr) > mechos_obstacle_rudder_velocity_threshould)
		t = 0;
	average(obstacle_rudder_t_avr, t, mechos_obstacle_rudder_t_avr_tau);
	if(active())
		statistics_add(obstacle_rudder_t_avr, STAT_NUMERIC, obstacle_rudder_t_avr);
	t = obstacle_rudder_t_avr;
	if(t > 1)
		t = 1;
	float rudder_angle = max_rudder_angle_original*(1.f - t) + max_rudder_angle_obstacle*t;
	rudder_angle /= 1.f + (control_points.empty() ? mechos_rudder_speed_decrease_factor : mechos_rudder_speed_decrease_factor_bot)*fabsf(Vlocal().y);
	float base_x = fabs(wheels[WheelRF].position.x);
	float base_y = wheels[WheelRF].position.y - wheels[WheelRB].position.y;
	float tan_a = tan(rudder*rudder_angle)*base_y/mechos_general_base_y;
	right_rudder_angle = atan2(base_y*tan_a, base_y - base_x*tan_a );
	left_rudder_angle = atan2(base_y*tan_a, base_y + base_x*tan_a);
	wheels[WheelRF].set_steering_angle(right_rudder_angle);
	wheels[WheelLF].set_steering_angle(left_rudder_angle);
	
	if(acceleration_chance_timer() && traction){
		acceleration_spot_start(Yglobal());
		fxlabNitroSource(this);
		}
}

void Mechos::keyboard_control()
{
	float traction_incr = 1.; // скорость набора мощности
	float traction_decr = 1; // скорость сброса (противоположная стрелка - мгновенно + тормоз)

	XJoystick* joystick = enable_joystick_control ? XJoystickGetJoystick() : 0;
	  
	// Traction
	float do_traction = joystick ? -joystick -> lY*2./(float)RANGE_MAX : 0;
	if(fabs(do_traction) > 1)
		do_traction = SIGN(do_traction);
	if(mchKeyPressed(MCH_KEY_MOVE_BACKWARD, control_config))
		do_traction = -1;
	if(mchKeyPressed(MCH_KEY_MOVE_FORWARD, control_config))
		do_traction = 1;

	if(do_traction > 0){
		if(Vlocal().y > -5){
			if((traction += traction_incr) > do_traction)
				traction = do_traction;
			}
		else{
		      traction = 0;
		      brake = 1;
		      }
		}
	else
		if(do_traction < 0){
			if(Vlocal().y < 5){
				if((traction -= traction_incr) < do_traction)
					traction = do_traction;
				}
			else{
			      traction = 0;
			      brake = 1;
			      }
			}
		else
			if(fabs(traction) > traction_decr)
				traction -= traction_decr*SIGN(traction);
			else
				traction = 0;


	// Rudder
	float do_rudder = joystick ? joystick -> lX*2./(float)RANGE_MAX : 0;
	if(fabs(do_rudder) > 1)
		do_rudder = SIGN(do_rudder);
	if(mchKeyPressed(MCH_KEY_TURN_RIGHT, control_config))
		do_rudder += 1;
	if(mchKeyPressed(MCH_KEY_TURN_LEFT, control_config))
		do_rudder += -1;

	if(do_rudder > 0){
		if(rudder < 0)
			rudder = 0;
		if((rudder += rudder_incr*evolve_time_step) > do_rudder)
			rudder = do_rudder;
		}
	else
		if(do_rudder < 0){
			if(rudder > 0)
				rudder = 0;
			if((rudder -= rudder_incr*evolve_time_step) < do_rudder)
				rudder = do_rudder;
			}
		else
			rudder = 0;

	if(mchKeyPressed(MCH_KEY_HORN, control_config)){
		if(!beep_pressed){
			beep();
			beep_pressed = 1;
			}
		}
	else
		beep_pressed = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Муравейник
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::formicTransport(const Vect3f& target, const Vect3f& next_point_for_direction)
{
	xassert(state() == NORMAL_MECHOS);

	if(features() & HUMBLE)
		reset_feature(HUMBLE);

//	RemoveSeeds();
//	AddSeed(target);

	Vect3f dr = getDist(next_point_for_direction, target);
	formic_transport_finish_angle = atan2(dr.x, dr.y);

	stopSounds();
	startSound(EFF_FORMICARY);

	for(int i = NUM_PARTS - 1; i >= 0; i--){
		Part* p = detachPart(1 << i, 1);
		p -> preLink(this, target);
		}

	setTrans(R().z + mesh_offset.z, Z_AXIS);
	mesh_offset.set(0.0);
	state_ = FORMIC_TRANSPORTED_MECHOS;
	completed_ = 0;
	visible_ = 0;
	is_dynamic_ = 0;
	submersion_ = 0;
	xrealM3D -> Detach(geometry);
	geometry = 0;
	spheres.clear();
	polyhedrons.clear();
	quants.clear();
	forces.clear();
	features_ = NO_FEATURES;
	clearVelocities();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Engine Sound
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::startEngineSound()
{
	changeEngineSound(active() ? EFF_ENGINE : EFF_ENGINE_ALIEN);
}
void Mechos::stopEngineSound()
{
	changeEngineSound(0);
}
void Mechos::changeEngineSound(int newSound)
{
	if(engineSound)
		stopSound(engineSound);
	engineSound = newSound;
}

float Mechos::speedSoundFactor()
{
	float t = 0;
	for(int i = 0; i < 4; i++)
		t += fabs(wheels[i].omega)*mechos_wheels_sound_frequency_factor;
	t -= 1.;
	statistics_add(sf, STAT_NUMERIC, t);
	if(t < -1.)
		t = -1.;
	if(t > 1.)
		t = 1.;
	return t;
}

void Mechos::stopSounds()
{
	if(engineSound)
		stopSound(engineSound);
	if(active()){
		stopSound(EFF_MECHOS_BRAKE);
		stopSound(EFF_MECHOS_SLIME);
		stopSound(EFF_MECHOS_WATER);
		stopSound(EFF_DIRIGIBLE_ADD);
		//stopSound(EFF_BULLET_FLIGHT_ADD);
		stopSound(EFF_HELI_ADD);
		stopSound(EFF_SCARLET_TRACE);
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Sensor	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::field_affection(const FieldSource* source)
{
	if(source -> type() != FieldSource::Sensor)
		return;
	if(sensor_source != (const SensorFieldSource*)source && !sensor_timer)
		sensorIn(sensor_source = (const SensorFieldSource*)source);
	sensor_timer.start(500);
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Speed Bonus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::speed_increment()
{
	if((speed_increment_factor += (mechos_speed_increment_by_bonus_factor - 1)/mechos_max_bonus_to_take) > mechos_speed_increment_by_bonus_factor)
		speed_increment_factor = mechos_speed_increment_by_bonus_factor;
}
int Mechos::speed_bonus_counter() const
{
	return round(mechos_max_bonus_to_take*(speed_increment_factor - 1)/(mechos_speed_increment_by_bonus_factor - 1));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Trail Colors
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrailColorsInit()
{
	TrailColors[OrdinaryTrail] = sColor4f(!Mdisp -> isSnow() ? ordinaryTrail : snowTrail);
	TrailColors[RedSlimeTrail] = sColor4f(redSlimeTrail);
	TrailColors[GreenSlimeTrail] = sColor4f(greenSlimeTrail);
	TrailColors[YellowSlimeTrail] = sColor4f(yellowSlimeTrail);
	TrailColors[BlueSlimeTrail] = sColor4f(blueSlimeTrail);
	TrailColors[RedTrackTrail] = sColor4f(redTrackTrail);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Parts Manipulation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::changePart()
{
	static DurationTimer timer;
	if(!geometry || !active() || timer() || !xPressed('Q'))
		return;

	if(xPressed(VK_UP) | xPressed(VK_DOWN) | xPressed(VK_LEFT) | xPressed(VK_RIGHT))
		timer.start(200);
	else 
		return;

	static int part;
	if(xPressed(VK_UP))
		part++;
	if(xPressed(VK_DOWN))
		part--;
	part %= NUM_ACTIVE_PARTS;

	int direct = 0;
	if(xPressed(VK_LEFT))
		direct = -1;
	if(xPressed(VK_RIGHT))
		direct = 1;


	int Kinds[NUM_PARTS];
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
		Kinds[i] = M3D_KIND(part_by_index(i) -> Type);

	unsigned kind = (M3D_KIND(part_by_index(part) -> Type) + direct + 20) % 20;
	while(!MeshLibrary -> Find(M3D_TOTAL_TYPE(kind, 1 << part)))
		kind = (kind + direct + 20) % 20;
	Kinds[part] = kind;

	Kinds[PART_INDEX(M3D_LF_WHEEL)] = Kinds[PART_INDEX(M3D_RF_WHEEL)];
	Kinds[PART_INDEX(M3D_LB_WHEEL)] = Kinds[PART_INDEX(M3D_RB_WHEEL)];

	xrealM3D -> Delete(geometry); 
	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(Kinds[0], M3D_ENGINE));
	geometry -> attachDummyChildren(Kinds);
	geometry -> SetPosition(Alg(), R() + Alg()*mesh_offset);
	registerGeometry();
	make_non_colliding();
}

void Mechos::changePart(int part_index, int part_number)
{
	int Kinds[NUM_PARTS];
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
		Kinds[i] = M3D_KIND(part_by_index(i) -> Type);

	Kinds[part_index] = part_number;
	Kinds[PART_INDEX(M3D_LF_WHEEL)] = Kinds[PART_INDEX(M3D_RF_WHEEL)];
	Kinds[PART_INDEX(M3D_LB_WHEEL)] = Kinds[PART_INDEX(M3D_RB_WHEEL)];

	xrealM3D -> Delete(geometry); 
	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(Kinds[0], M3D_ENGINE));
	geometry -> attachDummyChildren(Kinds);
	geometry -> SetPosition(Alg(), R() + Alg()*mesh_offset);
	int dyn_kind = is_dynamic();
	registerGeometry();
	is_dynamic_ = dyn_kind;
	make_non_colliding();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				Parts accessing
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cMesh* Mechos::part_by_type(int partType) const 
{ 
	return geometry ? geometry -> FindMesh(partType) : 0; 
}

cMesh* Mechos::part_by_index(int index) const 
{	
	return geometry ? geometry -> FindMesh(1 << index) : 0; 
}

Vect3f Mechos::part_coords(int partType) const
{
	if(!geometry)
		return R();
	cMesh* m = geometry -> FindMesh(partType);
	if(!m)
		return R();
	MatXf mx;
	mx = m -> GlobalMatrix;
	return mx.trans();
}

float Mechos::energy_max()
{
	return diff_data().energy_max;
}

void Mechos::setPermanentAlpha(float alpha_)
{
	if(geometry)
		setAlpha(alpha = alpha_);
}

void Mechos::setColor(int index, const sColor4f* diffuse, const sColor4f* specular)
{
	cMesh* mesh = part_by_index(index);
	if(mesh)
	{
		sColor4f c1(diffuse -> r, diffuse -> g, diffuse -> b, diffuse -> a*alpha);
		xrealIVG -> SetObjectColor(mesh, diffuse ? &c1 : 0, specular, 1 << index);
	}
}

void Mechos::getColor(int index, sColor4f* diffuse, sColor4f* specular)
{
	cMesh* mesh = part_by_index(index);
	if(mesh)
		xrealIVG -> GetObjectColor(mesh, diffuse, specular, 1 << index);
	if(diffuse)
		diffuse -> a = alpha;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			write & show
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::write_state(XBuffer& buf)
{
	PointControlledObject::write_state(buf);
	
	if(completed()){
		buf < "configuration: ";
		for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
			if(part_by_index(i))
				buf <= M3D_KIND(part_by_index(i) -> Type) < "  ";
		 buf < "\n";
		 }

	buf < "state: " <= state() < "\n";
	buf.SetRadix(2);
	buf < "features: " <= features() < "\n";
	buf < "affecting_fields: " <= affecting_fields() < "\n";
	buf.SetRadix(10);
//	buf < "yaw: " <= R2G(yaw()) < "\n";	
//	buf < "pitch: " <= R2G(pitch()) < "\n";	
//	buf < "roll: " <= R2G(roll()) < "\n";	
	//buf < "theta: " <= R2G(theta()) < "\n";	
	buf < "surface: " <= surface < "\n";
	buf < "jabasoma_jumping_log: " <= jabasoma_jumping_log < "\n";
	//buf < "obstacle_rudder_t_avr: " <= obstacle_rudder_t_avr < "\n";
//	buf < "name: " < diff_data().name < "\n";
	buf < "n_contacts: ";
	int i;
	for(i = 0; i < 4; i++)
		buf <= wheels[i].n_contacts < " ";
	buf < "\n";

	buf < "penetration: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].total_penetration < " ";
	buf < "\n";
	
	buf < "v_t: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].v_t < " ";
	buf < "\n";

	buf < "brake_delta_omega: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].brake_delta_omega < " ";
	buf < "\n";

	buf < "omega: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].omega < " ";
	buf < "\n";

	buf < "rxt: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].radius_cross_tangent_dot_axle < " ";
	buf < "\n";

	buf < "xxx: ";
	for(i = 0; i < 4; i++)
		buf <= wheels[i].TOI_inv*wheels[i].radius*wheels[i].j_t < " ";
	buf < "\n";
}	

void Mechos::show() const
{
	PointControlledObject::show();
	Vect3f r;
	pose().xformPoint(Vect3f(obstacle_x_avr, obstacle_y_avr, 0), r);
	show_vector(r, 3, XCOL(CYAN, 100));
	if(show_point_controlled_points){
		if(state() == ASSEMBLING_MECHOS)
			show_map_line(R(), R() + assembling_direction*10, YELLOW);
		}
	for(int i = 0; i < 4; i++){
		const Wheel& wi = wheels[i];
		pose().xformPoint(wi.position, r);
		Vect3f dr;
		//pose().xformVect(wi.axle, dr);
		//show_vector(r, dr, XCOL(wi.skidding ? RED : CYAN, 100));
		//show_vector(r, dr, XCOL(wi.skidding ? RED : CYAN, 200));
		show_vector(wi.contact_point + R(), wi.tangent*wi.j_t, XCOL(RED, 200));
		show_vector(wi.contact_point + R(), wi.normal*wi.f_n, XCOL(CYAN, 200));
		}

	show_vector(R(), 3, YELLOW);

	if(show_body_spheres){
		SphereList::const_iterator si;
		FOR_EACH(wheels_spheres,si){
			Vect3f cp1g;
			pose().xformPoint(si -> center, cp1g);
			show_vector(cp1g, si -> radius, XCOL(CYAN, 255));
			}
		}

	if(features() & IRON_HIP){
		std::vector<Vect3f> fires;
		part_by_type(M3D_FRONT) -> GetGlobalDummy("fireb", fires);
		std::vector<Vect3f>::iterator fi;
		FOR_EACH(fires, fi)
			show_vector(*fi, iron_hip_radius, XCOL(GREEN, 200));
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Acceleration Spot
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mechos::acceleration_spot_start(const Vect3f& direction)
{
	if(features() & ACCELERATED_BY_SPOT)
		return;
	features_ |= ACCELERATED_BY_SPOT;
	acceleration_spot_direction = direction;
	acceleration_spot_timer.start(mechos_acceleration_spot_time);
	forces.push_back((QuantFunction)&Mechos::acceleration_spot_quant);
	if(active()){
		startSound(EFF_BONUS_NITRO);
		startSound(EFF_ANIMATION_1_BACK + (M3D_KIND(part_by_index(Engine) -> Type) - 1)*100, 1, features() & HUMBLE ? 1 : 0);
		}
}

int Mechos::acceleration_spot_quant()
{
	if(active())
		updateSound(EFF_BONUS_NITRO);
	if(speed_avr < mechos_acceleration_spot_max_speed)
		applyGlobalForce(acceleration_spot_direction*mechos_acceleration_spot_traction);
	else
		dragV += Vect3f(mechos_speed_bound_drag);
	if(!acceleration_spot_timer){
		features_ &= ~ACCELERATED_BY_SPOT;
		return 0;
		}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Rotation arcanes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Mechos::startRotation(float angle, float speed)
{
	if(features() & ROTATING)
		return 0;
	features_ |= ROTATING;
	forces.push_back((QuantFunction)&Mechos::rotation_quant);
	prev_psi = psi();
	float sign = SIGN(frnd(1));
	rotation_angle = fabs(angle)*sign;
	rotation_speed = fabs(speed)*sign;
	return 1;
}

int Mechos::rotation_quant()
{
	setLocalAngularVelocity(rotation_speed, Z_AXIS);
	float dangle = getDist_f(psi(), prev_psi, 2*M_PI);
	if(fabs(dangle) > fabs(rotation_angle)){
		features_ &= ~ROTATING;
		setLocalAngularVelocity(0, Z_AXIS);
		return 0;
		}
	rotation_angle += dangle;
	prev_psi = psi();
	return 1;
}

void Mechos::kinetic_impulse(const Vect3f& epicenter)
{
	if(getDist(R(), epicenter).norm() < kinetic_impulse_distance)
		if(startRotation(4*M_PI, kinetic_impulse_rotation_speed))
			setGlobalVelocity(kinetic_impulse_vertical_velocity, Z_AXIS);
}

void Mechos::animal_obstacle()
{
	if(animal_obstacle_immunity_timer() || features() & JABASOMA)
		return;
	if(startRotation(2*M_PI*animal_obstacle_turns, animal_obstacle_rotation_speed)){
		if(active())
			startSound(EFF_ANIMATION_1_COLLISION + (M3D_KIND(part_by_index(Engine) -> Type) - 1)*100, 1, features() & HUMBLE ? 1 : 0);
		setGlobalVelocity(Vect3f(0, 0, animal_obstacle_vertical_velocity));
		animal_obstacle_immunity_timer.start(animal_obstacle_immunity_time);
		}
}

void Mechos::slime_obstacle()
{
	if(slime_obstacle_immunity_timer() || features() & JABASOMA)
		return;
	if(startRotation(2*M_PI*slime_obstacle_turns, slime_obstacle_rotation_speed)){
		Vect3f v = Vglobal();
		v.z = 0;
		v.Normalize(slime_obstacle_horizontal_velocity);
		setGlobalVelocity(v);
		slime_obstacle_immunity_timer.start(slime_obstacle_immunity_time);
		}
}

///////////////////////////////////////////////////////////////////////////////////////////
//		Teleportation
///////////////////////////////////////////////////////////////////////////////////////////
void Mechos::start_teleportation()
{
	state_ = WAITING_FOR_TELEPORTATION;
	features_ |= ROTATING;
	teleportation_timer.start(waiting_for_teleportation_time);
	fxlabCharacterTeleportation(R(), target);
}

///////////////////////////////////////////////////////////////////////////////////////////
//		Beep
///////////////////////////////////////////////////////////////////////////////////////////
void Mechos::beep()
{
	startSound(EFF_KLAXON);
	Mdisp -> beep(R());
}

///////////////////////////////////////////////////////////////////////////////////////////
//		Teleportation
///////////////////////////////////////////////////////////////////////////////////////////
void Mechos::start_teleportation(const Vect3f& pos)
{
	if(features() & TELEPORTATING)
		return;
	features_ |= TELEPORTATING | ROTATING;
	forces.push_back((QuantFunction)&Mechos::teleportation_quant);
	teleportation_timer.start(mechos_teleportation_time);
	teleportation_dpos = getDist(pos, (Vect3f)R())*(evolve_time_step_ms/mechos_teleportation_time);
	teleportation_pose.trans() = R();
	teleportation_pose.rot() = Mat3f(-psi(), Z_AXIS);
}

int Mechos::teleportation_quant()
{
	if(teleportation_timer())
		CYCLE(teleportation_pose.trans() += teleportation_dpos);
	teleportation_pose.rot() = Mat3f(-psi(), Z_AXIS);
	teleportation_pose.rot() *= Mat3f(mechos_teleportation_rotation, Z_AXIS);
	setPose(teleportation_pose);
	dragV += Vect3f(10);
	dragW += Vect3f(4);
	return 1;
}

void Mechos::cancel_teleportation()
{
	features_ &= ~(TELEPORTATING | ROTATING);
	forces.remove((QuantFunction)&Mechos::teleportation_quant);
}

void Mechos::set_brake(int on)
{
	PointControlledObject::set_brake(on);
	if(!on && !mchTurnBasedGame && active()){
		control();
		if(!traction)
			acceleration_chance_timer.start(acceleration_chance_time);
		}
	if(on){
		stopSounds();
		runEffect(NoEffect);
		clearVelocities();		
		rudder = 0;
		control();
		post_quant();
		}
}	

/////////////////////////////////////////////////////////////////////////////////
//			Wheels
/////////////////////////////////////////////////////////////////////////////////
Mechos::Wheel::Wheel()
{
	axle = Vect3f::I;
	skidding = 0;
	skidding_factor = 0;
	dynamic_frictional = 0;
}

void Mechos::Wheel::set_steering_angle(float angle)
{
	axle.set(cos(angle),  sin(angle), 0);
}

void Mechos::Wheel::init(float mass, float radius_, float wheel_penetration_, float damping_scale, Mechos* chassis_)
{
	chassis = chassis_;
	radius = radius_;
	wheel_penetration = wheel_penetration_;
	TOI_inv = 1.f/(mass*0.5f*sqr(radius));
	float m = chassis -> mass*0.25;
	stiffness = m*Gravity/wheel_penetration;
	damping = 2.f*sqrtf(stiffness*m)*damping_scale;
	phi = omega = 0;
	position.z -= wheel_penetration;
	suspension_z = wheel_penetration;
	brake_torque = 0;
	brake_damping = 0;
}

float Mechos::Wheel::suspension_travel()
{
	return suspension_z - wheel_penetration;
}

void Mechos::Wheel::convert(const MatXf& pose)
{
	pose.xformVect(axle, axle_global);
	pose.xformPoint(position, position_global);
	tangent.set(-axle.y, axle.x, 0);
	pose.xformVect(tangent);

	n_contacts = 0;
	total_penetration = 0;
	contact_point.set(0.0);
	normal.set(0.0);
	property = 0;

	n_contacts_non_central = 0;
	total_penetration_non_central = 0;
	contact_point_non_central.set(0.0);
	normal_non_central.set(0.0);
	property_non_central = 0;

	f_n = j_t = 0;
	v_n = v_t = 0;
	angular_acceleration = 0;
	contacted_body = 0;
	
	axle_cross_radius.set(0.0);
	radius_cross_tangent_dot_axle = 0;
	brake_delta_omega = 0;
	max_friction_impulse = 0;
}

void Mechos::Wheel::add_contact(int central, float penetration, const Vect3f& point_, const Vect3f& normal_, WheelContactProperty* property_, const Body* body)
{
	if(normal_.z > mechos_obstacle_slope_z){
		if(penetration > upper_direction_penetration_max)
			return;
		}
	else
		if(normal_.z < -mechos_obstacle_slope_z){
			if(penetration > lower_direction_penetration_max)
				return;
			}
		else{
			if(penetration > horizontal_direction_penetration_max)
				return;
			}

	if(central){
		n_contacts++;
		contact_point += point_*penetration;
		normal += normal_*penetration;
		total_penetration += penetration;
		property = property_;
		}
	else{
		n_contacts_non_central++;
		contact_point_non_central += point_*penetration;
		normal_non_central += normal_*penetration;
		total_penetration_non_central += penetration;
		property_non_central = property_;
		}
	if(body)
		contacted_body = body;
}

void Mechos::Wheel::post_collision()
{
	if(!n_contacts){
		if(!n_contacts_non_central)
			return;
		n_contacts = n_contacts_non_central;
		contact_point = contact_point_non_central;
		normal = normal_non_central;
		property = property_non_central;
		total_penetration = total_penetration_non_central;
		}
	
	f_n = normal.norm();
	if(f_n > FLT_EPS)
		normal /= f_n;
	f_n /= n_contacts;
	suspension_z = f_n*dot(normal, chassis -> Zglobal());
	f_n *= stiffness;

	Vect3f radial_vector;
	radial_vector.scale(normal, -radius);
	axle_cross_radius.cross(axle_global, radial_vector);

	contact_point /= total_penetration;
	contact_point -= chassis -> R();

	// v = V1 + W1 % (r2 + q2 % r3) + q1 % (W2 % r3)
	Vect3f vel = chassis -> global_velocity(contact_point);
	vel.scaleAdd(axle_cross_radius, omega);
	if(contacted_body)
		vel -= contacted_body -> global_velocity(contact_point + chassis -> R() - contacted_body -> R());

	v_n = dot(vel, normal);
	vel.scaleAdd(normal, -v_n);
	v_t = vel.norm();
	if(v_t > mechos_tangent_velocity_min)
		tangent.scale(vel, 1.f/v_t);
	else
		v_t = dot(vel, tangent);

	radius_cross_tangent_dot_axle = dot(radial_vector % tangent, axle_global);

	f_n -= damping*v_n;

	if(f_n < 0)
		f_n = 0;

	statistics_add(f_n, STAT_NUMERIC, f_n);
	
	if(f_n > mechos_penalty_force_max)
		f_n = mechos_penalty_force_max;

	max_friction_impulse = property ?
		((!dynamic_frictional ? property -> static_friction : property -> dynamic_friction)*chassis -> mechos_wheel_friction_factor*f_n*evolve_time_step) : 0;
	
	brake_delta_omega = brake_torque;
	if(property)
		brake_delta_omega += property -> brake_torque;
	brake_delta_omega *= TOI_inv*evolve_time_step;
}

void Mechos::Wheel::apply_torque(float torque)
{
	angular_acceleration -= TOI_inv*torque;
}

int Mechos::Wheel::check_if_blocked(float j)
{
	if(fabs(j) > max_friction_impulse)
		j  =  SIGN(max_friction_impulse, j);
	if(!angular_acceleration && fabs(omega + TOI_inv*radius_cross_tangent_dot_axle*j) < brake_delta_omega){
		radius_cross_tangent_dot_axle = 0;
		return 1;
		}
	return 0;
}

void Mechos::Wheel::set_solution(float j)
{
	statistics_add(j_t, STAT_NUMERIC, -j);
	statistics_add(skidding_factor, STAT_NUMERIC, skidding_factor);

	j_t = fabs(j) < max_friction_impulse ? j : SIGN(max_friction_impulse, j);
	dynamic_frictional = !(fabs(j) < max_friction_impulse);

	average( skidding_factor, v_t*suspension_z*mechos_skidding_factor, mechos_skidding_avr_tau);
	skidding = skidding_factor > 1.f;
	omega += TOI_inv*radius_cross_tangent_dot_axle*j_t;
}

void Mechos::Wheel::integrate(float dt)
{
	phi += omega*dt;
	if(radius_cross_tangent_dot_axle){
		omega += (angular_acceleration - (brake_damping + (property ? property -> brake_damping : 0))*omega)*dt;
		if(fabs(omega) > brake_delta_omega)
			omega += SIGN(brake_delta_omega, -omega);
		else 
			omega = 0;
		}
	else
		omega = 0;
}
	
void Mechos::Wheel::post_test()
{
	Vect3f vel = chassis -> global_velocity(contact_point);
	vel.scaleAdd(axle_cross_radius, omega);
	if(contacted_body)
		vel -= contacted_body -> global_velocity(contact_point + chassis -> R() - contacted_body -> R());
	v_t_ = dot(vel, tangent);
}

/////////////////////////////////////////////////////////////////////////////////
//		New Wheels force
/////////////////////////////////////////////////////////////////////////////////
class TestWheelsCollisionOp
{
	Mechos& m;
public:

	TestWheelsCollisionOp(Mechos& m_) : m(m_) {}
	
	void operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForTestCollision))
			return;
		
		for(int i = 0; i < 4; i++){
			Mechos::Wheel& w = m.wheels[i];
			Vect3f uncycled_r = w.position_global, normal, cp;
			uncycled_r.x += dx; uncycled_r.y += dy;
			float penetration;
			int central;
			if(central = triangle.clipPenalty(uncycled_r, w.radius, penetration, cp, normal)){
				cp.x -= dx; cp.y -= dy;
				w.add_contact(central == 1, penetration, cp, normal, &terrain_contact_property_array[BitSR(triangle.surface())]);
				m.surface |= triangle.surface();
				m.ground_colliding = 1;
				}
			}
	}			
};

void Mechos::add_contact(float penetration, const Vect3f& point, const Vect3f& normal, const Sphere& sphere, const Body* body)
{
	wheels[sphere.index - RFWheel].add_contact(1, penetration, point, normal, &gear_contact_property, body);
}

__forceinline void lin_solve(const float A[4][4], const float B[4], float X[4])
{
	// Analitical solve A*x = B,  
	// where A - symmetric, using upper triangle.
	float t15 = A[2][3];
	float t16 = A[1][3];
	float t18 = A[1][2];
	float t20 = A[0][1];
	float t21 = A[0][2];
	float t14 = A[2][2];
	float t32 = t18*t21-t20*t14;
	float t17 = A[1][1];
	float t39 = t20*t18;
	float t30 = t39-t17*t21;
	float t42 = t32*t16+t30*t15;
	float t22 = A[0][3];
	float t41 = -2.0*t22;
	float t40 = t16*t15;
	float t38 = t20*t16;
	float t37 = t18*t15;
	float t36 = t21*t40;
	float t10 = t20*t20;
	float t19 = A[0][0];
	float t35 = t10-t19*t17;
	float t12 = t21*t21;
	float t34 = -t12+t19*t14;
	float t11 = t18*t18;
	float t33 = t17*t14-t11;
	float t31 = -t21*t20+t19*t18;
	float t8 = t15*t15;
	float t9 = t16*t16;
	float t29 = t9*t14+t17*t8-2.0*t16*t37;
	float t28 = t19*t11+t10*t14-2.0*t21*t39-t34*t17;
	float t27 = B[2];
	float t26 = B[3];
	float t25 = B[0];
	float t24 = B[1];
	float t23 = A[3][3];
	float t13 = t22*t22;
	float t7 = t19*t40+t13*t18-t31*t23+(-t21*t16-t20*t15)*t22;
	float t6 = -t33*t22-t42;
	float t5 = t20*t8-t36+t32*t23+(-t37+t16*t14)*t22;
	float t4 = t31*t16+t35*t15-t30*t22;
	float t3 = t15*t38-t21*t9-t30*t23+(-t17*t15+t16*t18)*t22;
	float t2 = t34*t16-t31*t15+t32*t22;
	float t1 = 1./(-t12*t9+2.0*t20*t36-t10*t8+t33*t13+t29*t19+2.0*t42*t22+t28*t23);
	X[0] = ((-t33*t23+t29)*t25-t5*t24+t3*t27-t6*t26)*t1;
	X[3] = (-t6*t25+t2*t24-t4*t27+t28*t26)*t1;
	X[1] = (-t5*t25+(t19*t8+t21*t15*t41+t13*t14-t34*t23)*t24-t7*t27+t2*t26)*t1;
	X[2] = (t3*t25-t7*t24+(t19*t9+t38*t41+t13*t17+t35*t23)*t27-t4*t26)*t1;
}

void Mechos::applyWheelsForce()
{
	start_autostop_timer(applyWheelsForce, STAT_MECHOS);

	int i;
	for(i = 0; i < 4; i++)
		wheels[i].convert(pose());

	surface = 0;
	wheels_colliding = 0;
	TestWheelsCollisionOp op(*this);
	tri_map -> scan(getBound(), op);

	swap(wheels_spheres, spheres);
	Mdisp -> test_mechos_wheels_to_bodies_collisions(*this);
	swap(wheels_spheres, spheres);

	WheelContactProperty* forced_property = 0;
	if(slime_timer() && !(features() & JABASOMA))
		forced_property = &slime_contact_property;
	if(affecting_fields() & (FieldSource::BubbleField | FieldSource::Stream | FieldSource::Vortex))
		forced_property = &stream_contact_property;

	Vect3f Vl = Vlocal();
	float brake_torque = mechos_free_brake_torque;
	float brake_damping = mechos_free_brake_damping;
	if(!isFlyingByArcane() && traction){
		float f_t = traction*speed_increment_factor;
		float v_max = mechos_max_velocity_threshould*fabs(f_t);
		if(fabs(Vl.y) > v_max){
			f_t = 0;
			dragV += Vect3f(mechos_speed_bound_drag);
			}
		else
			if(fabs(Vl.y) < low_gear_velocity_threshould)
				f_t += SIGN(low_gear_traction, traction);
		f_t *= mass/(1 + mechos_traction_speed_decrease_factor*fabs(Vl.y));
		for(i = 0; i < 4; i++){
			Wheel& w = wheels[i];
			if(fabs(w.omega)*w.radius < v_max)
				if(i == WheelRF || i == WheelLF)
					w.apply_torque(f_t*mechos_forward_wheel_drive_torque);
				else{
					float sign = obstacle_rudder_t_avr < mechos_obstacle_rudder_t_backward_wheel_start_threshould || i == WheelRB && rudder <= 0 || i == WheelLB && rudder >= 0 ? 1 : -1;
					w.apply_torque(sign*f_t*mechos_backward_wheel_drive_torque);
					}
			}
		}
	else{
		brake_torque += mechos_auto_brake_torque;
		brake_damping += mechos_auto_brake_damping;
		}

	if(brake)
		brake_torque += brake*mechos_brake_torque;
	
	for(i = 0; i < 4; i++){
		Wheel& w = wheels[i];
		w.brake_torque = brake_torque;
		w.brake_damping = brake_damping;
		if(forced_property)
			w.property = forced_property;

		w.post_collision();

		Vect3f f;
		f.scale(w.normal, w.f_n);
		F_total += f;
		K_total.crossAdd(w.contact_point, f);
		wheels_colliding += w.n_contacts;
		}

	float A[4][4];
	float B[4], X[4];
	for(i = 0; i < 4; i++){
		Wheel& wi = wheels[i];
		Mat3f m1;
		A[i][i] = dot(wi.tangent, global_mass_matrix(wi.contact_point, wi.contact_point, m1)*wi.tangent
			+ (wi.TOI_inv*wi.radius_cross_tangent_dot_axle)*wi.axle_cross_radius
			)*mechos_diagonal_element_scaling_factor;
		for(int j = i + 1; j < 4; j++){
			Wheel& wj = wheels[j];
			Mat3f m2;
			A[i][j] = dot(wi.tangent, global_mass_matrix(wi.contact_point, wj.contact_point, m2)*wj.tangent);
			}
		B[i] = -wi.v_t;
		}

	lin_solve(A, B, X);
	
	int wheels_blocked = 0;
	for(i = 0; i < 4; i++){
		Wheel& wi = wheels[i];
		if(wi.check_if_blocked(X[i])){
			Mat3f m3;
			A[i][i] = dot(wi.tangent, global_mass_matrix(wi.contact_point, wi.contact_point, m3)*wi.tangent)*mechos_diagonal_element_scaling_factor;
			wheels_blocked++;
			}
		}

	if(wheels_blocked)
		lin_solve(A, B, X);

	Vect3f P(0), L(0);
	for(i = 0; i < 4; i++){
		Wheel& w = wheels[i];
		w.set_solution(X[i]);

		Vect3f J;
		J.scale(w.tangent, w.j_t);
		P += J;
		L.crossAdd(w.contact_point, J);
		w.integrate(evolve_time_step);
		}

	V += P*mass_inv;
	W += Ig_inv*L;

	//  Spots
	Spot* spot = tri_map -> testSpot(wheels_spheres, pose());
	if(!spot)
		spot = tri_map -> testSpot(spheres, pose());
	if(spot)
		if(spot -> type == Spot::Acceleration){
			acceleration_spot_start(*spot);
			fxlabNitroSource(this);
			}
		else{
			jump_pad_start(*spot);
			fxlabJumpSource(this);
			}

	if(wheels_colliding){
		dragV += mechos_dragV_wheel;
		dragW += mechos_dragW_wheel;
		}
	else{ // Flying but not floating
		if(submersion() < 0.1){
			dragV += mechos_dragV_flying;
			dragW += mechos_dragW_flying;
			}
		}

	////////////////////////////////////////////////////////
	//		Trail
	////////////////////////////////////////////////////////
	int wheels_skidding = 0;
	float max_skidding_factor = 0;
	for( i = 0;i < 4;i++){	       
		Wheel& w = wheels[i];
		int red_track = features() & RED_TRACK_FEATURE && submersion() < 0.01 && (i == WheelRB || i == WheelLB);
		int do_track = 0;
		if(max_skidding_factor < wheels[i].skidding_factor)
			max_skidding_factor = wheels[i].skidding_factor;
		float dust_speed = fabs(velocity_y_avr)*dust_speed_factor;
		if(w.skidding){
			wheels_skidding++;
			dust_speed = 1;
			}
		if(!isFlyingByArcane() && w.n_contacts && !w.contacted_body && (w.skidding || slime_timer() || red_track)){
			do_track = 1;
			trail_pos1[i] = w.contact_point + R();
			trail_pos1[i].z += mechos_trail_dz;
			if(surface & (Triangle::Soil | Triangle::Sand) && speed_avr > 3.)
				fxlabDustSource(trail_pos1[i], dust_speed, 1, this);
			}

		if(do_track){
			if(!trail_started[i])
				trail_pos0[i] = trail_pos1[i];
			else{
				Vect3f dr = getDist(trail_pos1[i], trail_pos0[i]);
				if(dr.norm() > mechos_trail_length){
					if(red_track){
						Mdisp -> attachBaseObject(new RedTrackSource(trail_pos0[i] + dr/2, 5, round(mechos_red_track_time*0.8), ID, red_track_arcane_statistics) );
						if(i == WheelRB){
							Vect3f dx = Alg().xcol()*((wheels[WheelLB].position.x - wheels[WheelRB].position.x)/2);
							dx.z = 0;
							auto v1 = (trail_pos0[WheelRB] + dx);
							xrealIVG -> CreateTangentTrail(&v1, 
								&TrailColors[RedTrackTrail], &dr, &TrailColors[RedTrackTrail], 	mechos_red_track_time, mechos_red_track_animation_period, 
								dx.norm() + mechos_red_track_thickness_add);
							}
						}
					else{
						sColor4f color = TrailColors[trail_type];
						if(surface & (Triangle::Sand | Triangle::Ice))
							color.a /= 2.f;
						xrealIVG -> CreateTangentTrail(&trail_pos0[i], &color, &dr, &color, mechos_trail_time);
						}
					trail_pos0[i] = trail_pos1[i];
					}
				}
			trail_started[i].start(mechos_trail_started_time);
			}
		}

	if(active())
		if(slime_timer())
			startSound(EFF_MECHOS_SLIME);
		else{
			stopSound(EFF_MECHOS_SLIME);
			int prev_log = wheels_skidding_timer();
			if(wheels_skidding_timer(max_skidding_factor > mechos_skidding_factor_sound_threshould && !(surface & Triangle::Ice), mechos_skidding_sound_on_delay, mechos_skidding_sound_pause) && !prev_log && !(features() & JABASOMA))
				startSound(EFF_MECHOS_BRAKE);
			}
}

