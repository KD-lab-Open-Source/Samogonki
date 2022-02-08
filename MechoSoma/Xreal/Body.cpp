//////////////////////////////////////////////////////////////////
//	Òåëî - áàçîâûé êëàññ 
//	äèíàìè÷åñêèõ îáúåêòîâ
//////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "SimpleClip.h"
#include "Params.h"
#include "Mesh3ds.h"
#include "sound.h"
#include "Xreal_utl.h"
#include "PolyhedronLibrary.h"
#include "TERRA.H"
#include "DebugPrm.h"
#include "mechosoma.h"
#include "Statistics.h"
#include "CollisionHandler.hpp"

const int MIN_BODY_RADIUS = 10;

int Body::IDs;
int Body::instancies;
Vect3d DynamicBody::gravity;

void Body::internal_init()
{
	type_ = BODY;
	instancies++;
	ID = ++IDs;
	active_ = 0;
	is_dynamic_ = 1;
	is_static_ = 0;
	geometry = 0;
	radius_ = 1;
	bbox_ = Vect3f::ID;
	submersion_ = 0;
	involve_into_collision_test_counter = 0;
	in_contact = 0;
	body_colliding = 0;
	ground_colliding = 0;
	evolve_degeneracy = 0;
	dV_constraint = dW_constraint = F_constraint = K_constraint = Vect3f::ZERO; 
	completed_ = 1;
	visible_ = 1;
	disable_collision_test = 0;
	damage_ability_ = 0;
	hit_damage_ability_ = 0;
	energy = 1e10f;
	affecting_fields_ = 0;
	enabled_fields = 0;
	speed_avr = 0;
	
	mesh_offset.set(0.f);

	disable_collision_mask = 0;
	enable_collision_mask = ~0;

	density = Density;
	restitution_plus_one = general_k_restitution_plus_one;
	friction = 0;
	max_velocity_threshould = max_velocity_threshould_general;
	k_archimedean = k_submersion = 0;

	radius_ = 1;
	bbox_.set(1);
	mass_inv = mass = 1;		
	I_inv = I.set(1);
	gravity.set(0,0,-Gravity);
	
	prevXform = MatXf::ID;

	hidden_ = 0;
	dragV.set(free_linear_damping);
	dragW.set(free_angular_damping);

	clearForces();
	clearVelocities();
}	

Body::Body(cMesh* geometry_, BoundType bound_type_, float spheres_radius_parameter_)
{
	bound_type = bound_type_;	
	spheres_radius_parameter = spheres_radius_parameter_;
	internal_init();
	setGeometry(geometry_);
}

Body::Body(int m3d_type, BoundType bound_type_, float spheres_radius_parameter_)
{
	bound_type = bound_type_;	
	spheres_radius_parameter = spheres_radius_parameter_;
	internal_init();
	if(m3d_type)
		setGeometry((cMesh*)xrealIVG -> CreateObject(M3D_KIND(m3d_type), M3D_TYPE(m3d_type)));
}

Body::~Body()
{
	instancies--;
	
	releaseGeometry();
}

void Body::releaseGeometry() 
{
	if(xrealIVG && geometry){
		xrealIVG -> ReleaseObject(geometry);
		geometry = 0;
		}
}

void Body::setGeometry(cMesh* geometry_)
{
	geometry = geometry_;
	assert(geometry);

	radius_ = geometry -> rmaxTotal();
	bbox_.set(geometry -> xmaxTotal() - geometry -> xminTotal(), 
			geometry -> ymaxTotal() - geometry -> yminTotal(), 	
			geometry -> zmaxTotal() - geometry -> zminTotal());
	if(bbox_.norm() < MIN_BODY_RADIUS)
		bbox_.normalize(MIN_BODY_RADIUS);

	createBound();

	radius_ = polyhedrons.rad();
	assert(radius() < 1000);

	setPose(geometry -> LocalMatrix);
	setTrans(R() - Alg()*mesh_offset);
	R_prev = R();

	prevXform = MatXf::ID;

	clearForces();
	clearVelocities();

	hidden_ = 0;
	dragV.set(free_linear_damping);
	dragW.set(free_angular_damping);
}

void Body::setMass(float m)
{
	mass = m;
	xassert(mass > 0 && polyhedrons.vol() > 0);
	density = mass/polyhedrons.vol();		
	mass_inv = 1/mass;
	const Vect3f& m2 = polyhedrons.mov2();
	I = Vect3f(m2.y + m2.z, m2.z + m2.x, m2.x + m2.y)*density;
	I_inv = Vect3f::ID/I;
}

void Body::createBound()
{
	polyhedrons.clear();
	polyhedrons.add((*polyhedron_lib)[geometry -> Type]);

	setMass(density*polyhedrons.vol());

	spheres.clear();

	//spheres.push_back(Sphere(polyhedrons.center(), polyhedrons.rad()));

	Vect3f center(Vect3f::ZERO);
	float radius = polyhedrons.rad();
	switch(bound_type){
		case CenteredCircumscribedBound:
			radius = polyhedrons.rad();
			break;
		case CenteredInscribedBound:
			radius = minAbs(geometry -> xmaxTotal(), geometry -> ymaxTotal(), geometry -> zmaxTotal());
			break;
		case CenteredScaledBy2Bound:
			radius = polyhedrons.rad()/2;
			break;
		case CenteredBottomAlignmentedBound:
			radius = fabs(geometry -> zminTotal());
			break;
		case VolumetricEquivalentBound:
			radius = pow((float)(polyhedrons.vol()*3/(4*M_PI)), 1/3.f);
			break;
		case FloatsBound: {
			xassert(spheres_radius_parameter);
			vector<Vect3f> floats_;
			geometry -> Dummies.Query("float", back_insert_iterator<vector<Vect3f> >(floats_));
			vector<Vect3f>::iterator fi;
			FOR_EACH(floats_, fi)
				spheres.push_back(Sphere(*fi, spheres_radius_parameter));	       
			return;
			}
		default:
			assert(0);
		}
	spheres.push_back(Sphere(center, radius));
}

void Body::setScale(const Vect3f& scale)
{
	start_autostop_timer(setScale, STAT_BODYDISP);

	if(geometry)
		geometry -> SetScale(scale);

	MatXf X(Mat3f(scale, Vect3f::ZERO), Vect3f::ZERO);

	MatXf M(prevXform);
	M.Invert();
	M.premult(X);
	prevXform = X;

	polyhedrons.xform(M);

	Vect3f scl(M.rot()[0][0], M.rot()[1][1], M.rot()[2][2]);
	float k = (scl.x + scl.y + scl.z)/3;
	SphereList::iterator si;
	FOR_EACH(spheres, si){
		M.xformPoint(si -> center);
		si -> radius *= k;
		}

	bbox_ *= scl;
	radius_ *= k;

	if(is_dynamic())
		linear_projection();
		//make_non_colliding();
}	

void Body::make_non_colliding()
{
	if(!test_collision(0))
		return;

	const int D = 16;
	float dz = D/2;

	// начальное выравнивание
  	float h = tri_map -> H(R().xi(), R().yi(), R().z + mesh_offset.z + 20);
	if(R().z + mesh_offset.z < h)
		setTrans(h - mesh_offset.z, Z_AXIS);

	// грубый проход, Go Up
	do{
		if((Xlg.trans().z += D) > max_height_threshould)
			break;
		} while(test_collision(0));

	// бинарный поиск
	do{
		Xlg.trans().z -= dz;
		if(test_collision(0))
			Xlg.trans().z += dz;
		dz /= 2;
		} while(dz > 0.5);

	assert(!test_collision(0));

	ground_colliding = 0;
}	

void Body::place_on_the_ground()
{
	const int D = 16;
	float dz = D/2;

	// начальное выравнивание
  	float h = tri_map -> H(R().xi(), R().yi(), R().z + mesh_offset.z + 20);
	if(R().z + mesh_offset.z < h)
		Xlg.trans().z = h - mesh_offset.z;

	// грубый проход
	if(test_collision(0)){ // Go Up
		do{
			if((Xlg.trans().z += D) > max_height_threshould)
				break;
			} while(test_collision(0));
		}
	else{	// Go Down
		do{
			if((Xlg.trans().z -= D) < -1000){
				Xlg.trans().z =  h + height()/2;
				//Xlg.trans().z = max_height_threshould;
				break;
				}
			} while(!test_collision(0));
		Xlg.trans().z += D;
		}

	// бинарный поиск
	do{
		Xlg.trans().z -= dz;
		if(test_collision(0))
			Xlg.trans().z += dz;
		dz /= 2;
		} while(dz > 0.5);

	place_on_the_water();

//	assert(!test_collision(0));

	ground_colliding = 0;
}	

void Body::linear_projection()
{
	test_collision(1);
	ContactList::iterator ci;

	Vect3f dR(Vect3f::ZERO);
	for(int i = 0; i < 100; i++){
		float d_min = FLT_INF;
		Contact* contact = 0;
		FOR_EACH(contacts, ci){
			float d = ci -> dist + dot(ci -> normal, dR);
			if(d_min > d){
				d_min = d;
				contact = &*ci;
				}
			}
		if(d_min > linear_projection_tolerance)
			break;
		dR += contact -> normal*(-d_min);
		}
	Xlg.trans() += dR;
}

void Body::setPosition(const Vect3f& pose, float psi)
{
	setPose(MatXf(Mat3f(-psi, Z_AXIS), pose));
	CYCLE(Xlg.trans());
	Xlg.trans().z =  tri_map -> H(R().xi(), R().yi(), R().z + 20) + height()/2;
	setPositionInit();
}

void Body::setPosition(const Vect3f& pose, const Vect3f& target)
{
	Vect3f dr = getDist(target, Vect3f(pose.x, pose.y, target.z));
	setPosition(pose, atan2(dr.x, dr.y));
}

void Body::setPositionInit()
{
	R_prev = R();
	clearForces();
	clearVelocities();
	speed_avr = 0;
	if(geometry)
		geometry -> SetPosition(Alg(), R() + Alg()*mesh_offset);
}


//////////////////////////////////////////////////////////////////
//	Общие свойства
//////////////////////////////////////////////////////////////////
void Body::calc_forces_and_drags()
{
	if(ground_colliding){
		dragV.set(contact_linear_damping);
		dragW.set(contact_angular_damping);
		}
	else
		if(submersion() > 0.1){
			dragV.set(0.5);
			dragW.set(0.5);
			}
		else{
			dragV.set(free_linear_damping);
			dragW.set(free_angular_damping);
			}
}

void Body::post_quant()
{
	average(speed_avr, (float)Vlocal().norm(), 0.06f);
	CYCLE(Xlg.trans());
	if(R().z > max_height_threshould)
		Xlg.trans().z = max_height_threshould;
	if(R().z < -30){
		#ifndef _FINAL_VERSION_
		//	fout < "Below terrain !!! ["  < (char*)typeid(*this).name() < "]: " <= R();
		#endif
		Xlg.trans().z = max_height_threshould;
		place_on_the_ground();
		}

	if(geometry)
		geometry -> SetPosition(Alg(), Rgeometry());
}

void Body::write_state(XBuffer& buf)
{
	buf < typeid(*this).name() < ", ";
	if(geometry){
		buf < geometry -> GetFileName();
		if(geometry -> Parent)
			buf < ", "  < geometry -> GetName();
		if(geometry -> Child)
			buf < " ...";
		}
	buf < ", ID: " <= ID < "\n";
	buf < "Coords: " <= R();
	--buf;
	buf < " ; H: " <= H() < "\n";
	buf < "m: " <= mass < "; I: " <= I;
	buf < "Radius: " <= radius() < "; Box: " <= bbox_/2;
	buf < "N contacts: " <= contacts.size() < "\n";
	buf < "speed_avr: " <= averageSpeed() < "\n";
	buf < "colliding: " <= colliding() < "\n";
	buf < "submersion: " <= submersion() < "\n";
	buf < "life_time: " <= life_time() < "\n";
	buf < "restitution_plus_one: " <= restitution_plus_one < "\n";
	buf.SetDigits(15);
	buf < "Drift: " <= R().norm2() + dot(quat, quat)*1000000 < "\n";
	buf.SetDigits(4);
	buf < "V: " <= Vlocal();
	buf < "W: " <= Wlocal();
	buf < "dragV: " <= dragV;
	buf < "dragW: " <= dragW;
	buf < "F_total: " <= F_total;
	buf < "K_total: " <= K_total;
	buf < "dV_constraint: " <= dV_constraint;
	buf < "dW_constraint: " <= dW_constraint;
	buf < "F_constraint: " <= F_constraint;
	buf < "K_constraint: " <= K_constraint;
	buf < "-------------------------------------------\n";
}	

void Body::unhide() 
{ 
	if(geometry)
		xrealM3D -> Attach(geometry); 
	hidden_ = 0;
} 

void Body::hide() 
{ 
	if(geometry)
		xrealM3D -> Detach(geometry); 
	hidden_ = 1;
} 

float Body::getFrame() const
{
	cFrame* frame = geometry -> FindFrame();
	return frame ? frame -> GetPhase() : 0;
}

void Body::setFrame(float frame)
{
	geometry -> SetAnimation(0, frame);
}	

cFrame& Body::findFrame()
{
	cFrame* frame = geometry ? geometry -> FindFrame() : 0;
	if(!frame){
		cerr << "Frame not found: "<< typeid(*this).name() << endl;
		ErrH.Abort("Frame not found");
		}
	return *frame;
}

void Body::setAnimation(float AnimationPeriod, float StartPhase, float FinishPhase, unsigned int mask)
{
	xrealIVG -> SetObjectAnimation(geometry, AnimationPeriod, StartPhase, FinishPhase, mask);
}

void Body::setChannel(char* channel)
{
	xrealIVG -> SetObjectChannel(geometry, channel);
}

void Body::setChannelRandom(char* channel)
{
	int n = xrealIVG -> GetObjectNumberChannel(geometry, channel);
	xrealIVG -> SetObjectChannel(geometry, channel, 0, random(n));
}

void Body::setAlpha(float alpha)
{
	sColor4f c1(geometry -> GetDiffuse().r, geometry -> GetDiffuse().g, geometry -> GetDiffuse().b, alpha);
	xrealIVG -> SetObjectColor(geometry, &c1);
}

void Body::setColor(float r, float g, float b)
{
	if(geometry -> GetAttribute(MESH_REFLECTION))
	{
		sColor4f c1(r, g, b, geometry -> GetDiffuse().a);
		xrealIVG -> SetObjectColor(geometry, &c1);
	}
	else
	{
		sColor4f c1(r, g, b, 1);
		xrealIVG -> SetObjectColor(geometry, 0, &c1);
	}
}

float Body::H() const 
{
	return tri_map -> H(R().xi(), R().yi(), R().z + radius()); 
}

float Body::maxHeight() const 
{
	return max((float)GetWH(XCYCL(R().xi()), YCYCL(R().yi())), H());

/*
	if(spheres.empty())
		return H();
	SphereList::iterator si;
	float h, hmax = 0;
	FOR_EACH(spheres, si)
		if((h = H(si -> center)) > hmax)
			hmax = h;
	return hmax;
*/
}

iRectangle Body::getBound() const
{ 
	return iRectangle(round(R().x - radius()), round(R().y - radius()), round(R().x + radius()), round(R().y + radius())); 
}

Vect3f Body::turn_cross(const Vect3f& target, const Vect3f& current) const
{
	Vect3f cross = target % current;
	float len = cross.norm();
	if(len > DBL_EPS)
		cross /= len;
	else{
		Vect3f tmp;
		decomposition(target, fabs(target.x) > fabs(target.y) ? Vect3f::J : Vect3f::I, tmp, cross);
		cross.normalize();
		}
	return cross*Acos(dot(target, current)/(target.norm()*current.norm() + FLT_EPS));
}

void Body::startSound(int id)
{
	mchStartSoundEFF(id, R().x, R().y);
}

void Body::startSound(int id, float volume, float frequency)
{
	if(volume > 1)
		volume = 1;
	if(volume > minimal_volume_level)
		mchStartSoundEFF(id, R().x, R().y, volume, frequency);
}

void Body::updateSound(int id)
{
	mchSetSoundCoords(id, R().x, R().y);
}

void Body::stopSound(int id)
{
	mchStopSoundEFF(id);
}

float Body::hit_damage_ability(const class Body& b, float velocity) const 
{ 
	return hit_damage_ability_*velocity*velocity_to_damage_factor; 
} 

void Body::show() const
{
	if(show_body_radius)
		show_vector(R(), radius(), XCOL(CYAN, 200));

	if(show_body_polyhedrons)
		polyhedrons.draw(pose(), XCOL(CYAN));

	if(show_body_spheres){
		SphereList::const_iterator si;
		FOR_EACH(spheres,si){
			Vect3f cp1g;
			pose().xformPoint(si -> center, cp1g);
			show_vector(cp1g, si -> radius, XCOL(CYAN, 180));
			}
		}

	if(show_body_contacts){
		ContactList::const_iterator ci;
		FOR_EACH(contacts,ci)
			ci -> show();
		}

	if(show_body_velocity){
		show_vector(R(), XCOL(CYAN, 200));
		show_vector(R(), Vglobal(), XCOL(CYAN, 200));
		show_vector(R(), Wglobal(), XCOL(YELLOW, 200));
		//Vect3f dr;
		//Alg().xform(F_total, dr);
		//show_vector(R(), dr, XCOL(BLUE, 200));
		//Alg().xform(K_total, dr);
		//show_vector(R(), dr, XCOL(GREEN, 200));
		}
}



void DynamicBody::applyLocalTurnTorque(const Vect3f& target, const Vect3f& current, float factor)
{
	Vect3f cross = current % target;
	float len = cross.norm();
	if(len < FLT_EPS)
		return;
	cross *= acos(dot(target, current)/(target.norm()*current.norm() + FLT_EPS))*factor/len;
	applyLocalTorque(cross);
}

void DynamicBody::applyGlobalTurnTorque(const Vect3f& target, const Vect3f& current, float factor)
{
	Vect3f cross = current % target;
	float len = cross.norm();
	if(len < FLT_EPS)
		return;
	cross *= acos(dot(target, current)/(target.norm()*current.norm() + FLT_EPS))*factor/len;
	applyGlobalTorque(cross);
}

///////////////////////////////////////////////////////////////////////////////
//			Fast Car Adaptation
///////////////////////////////////////////////////////////////////////////////
real DynamicBody::GetMass()const
{
 return mass;
}
void DynamicBody::GetInverseTOIWorld(real t[3][3])const
{
 //memcpy(t,m_TOI_world_inverse,9*sizeof(real));
 t[0][0]=Ig_inv[0][0];
 t[0][1]=Ig_inv[0][1];
 t[0][2]=Ig_inv[0][2];
 t[1][0]=Ig_inv[1][0];
 t[1][1]=Ig_inv[1][1];
 t[1][2]=Ig_inv[1][2];
 t[2][0]=Ig_inv[2][0];
 t[2][1]=Ig_inv[2][1];
 t[2][2]=Ig_inv[2][2];
}

void DynamicBody::GetPosition(real position[3])const
{//
 position[0]=R()[0];
 position[1]=R()[1];
 position[2]=R()[2];
}

void DynamicBody::SetPosition(const real position[3])
{//
 Xlg.trans()[0]=position[0];
 Xlg.trans()[1]=position[1];
 Xlg.trans()[2]=position[2];
}

void DynamicBody::GetQuaternion(real Q[4])const
{//
 Q[0] = quat.s();
 Q[1] = quat.x();
 Q[2] = quat.y();
 Q[3] = quat.z();
}

void DynamicBody::SetQuaternion(const real Q[4])
{//
  quat.set(Q[0], Q[1], Q[2], Q[3]);
}

void DynamicBody::GetVelocity(real velocity[3])const
{//
 velocity[0]=V[0];
 velocity[1]=V[1];
 velocity[2]=V[2];
}

const real* DynamicBody::GetVelocity()const
{
 static Vect3d Vd;
 Vd = V;
 return &Vd.x;
}

void DynamicBody::SetVelocity(const real velocity[3])
{
 V[0]=velocity[0];
 V[1]=velocity[1];
 V[2]=velocity[2];
}

void DynamicBody::GetAngularVelocity(real angular_velocity[3])const
{//
 angular_velocity[0]=W[0];
 angular_velocity[1]=W[1];
 angular_velocity[2]=W[2];
}

const real* DynamicBody::GetAngularVelocity()const
{
 static Vect3d Wd;
 Wd = W;
 return &Wd.x;
}

void DynamicBody::SetAngularVelocity(const real angular_velocity[3])
{
 W[0]=angular_velocity[0];
 W[1]=angular_velocity[1];
 W[2]=angular_velocity[2];
}


