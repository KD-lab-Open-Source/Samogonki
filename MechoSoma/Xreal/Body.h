//////////////////////////////////////////////////////////////////
//	Тело - базовый класс 
//	динамических объектов
//////////////////////////////////////////////////////////////////
#ifndef __BODY_H__
#define __BODY_H__

#include "BaseObject.h"
#include "Grid2D.h"
#include "SimpleClip.h"

extern class cInterfaceVisGeneric* xrealIVG;
extern class cInterfaceGraph3d* xrealIGR;
extern class cM3D* xrealM3D;
extern int interface_world_now;

typedef double real;

class DynamicBody
{
	friend struct Contact;
	friend class Body;
	friend class Mechos;
	friend class BodyDispatcher;
	friend class ContactingBodies;
	friend class BodyAffectOp;
	friend class LCP_ContactSolver;

protected:
	MatXf Xlg;
	QuatF quat;  
	Vect3f V, W;
	Vect3f R_prev;

	Vect3f F_total, K_total; 
	Mat3f Ig_inv;
	Vect3f dV_constraint, dW_constraint; 
	Vect3f F_constraint, K_constraint; 

	float mass, mass_inv;
	Vect3f I, I_inv; // Diagonal of mass matrix
	Vect3f dragV, dragW;  // damping factors in local frame
	float restitution_plus_one;
	float friction;
	float max_velocity_threshould;

	void setRot() { quat.set(Alg()); Ig_inv = Alg()*Mat3f(I_inv, Vect3f::ZERO)*xpose(Alg()); }

public:

	// Full orientation //////////
	void setPose(const MatXf& pose) { Xlg = pose; setRot(); }
	const MatXf& pose() const { return Xlg; }

	// Translation //////	
	void setTrans(const Vect3f& trans) { Xlg.trans() = trans; }
	void setTrans(float coord, eAxis axis) { Xlg.trans()[axis] = coord; }
	const Vect3f& R() const { return Xlg.trans(); }
	const Vect3f& trans() const { return Xlg.trans(); }

	// Rotation //////	
	void setRot(const Mat3f& rot) { Xlg.rot() = rot; setRot(); }
	void setRot(float angle, eAxis axis) { Xlg.rot().set(angle, axis); setRot(); }
	const Mat3f& Alg() const { return Xlg.rot(); }
	const Mat3f& rot() const { return Xlg.rot(); }

	// Angles ///////////
	virtual float psi() const { return atan2f(Alg()[0][1], Alg()[1][1]); }
	float theta() const { return Alg().zrow().theta(); }

	// Local coords system in world's frame
	Vect3f Xglobal() const { return Alg().xcol(); }
	Vect3f Yglobal() const { return Alg().ycol(); }
	Vect3f Zglobal() const { return Alg().zcol(); }

	// World's coords system in local frame
	const Vect3f& Xlocal() const { return Alg().xrow(); }
	const Vect3f& Ylocal() const { return Alg().yrow(); }
	const Vect3f& Zlocal() const { return Alg().zrow(); }

	// Clear
	void clearForces() { F_total.set(0.f); K_total.set(0.f); }
	void clearVelocities() { V.set(0.f); W.set(0.f); }

	// Forces are multiplied by mass, torque - by I
	void applyLocalForce(Vect3f F){ Alg().xform(F); F_total += F*mass; }
	void applyLocalForce(float f, eAxis axis) { F_total += f*mass*Alg().col(axis); }

	void applyGlobalForce(const Vect3f& F){ F_total += F*mass; }
	void applyGlobalForce(float f, eAxis axis) { F_total[axis] += f*mass; }

	void applyLocalTorque(Vect3f K){ K*= I; Alg().xform(K); K_total += K; }
	void applyLocalTorque(float k, eAxis axis) { K_total += k*I[axis]*Alg().col(axis); }

	void applyGlobalTorque(Vect3f K){ Xlg.invXformVect(K); K*= I; Xlg.xformVect(K); K_total += K; }
	void applyGlobalTorque(float k, eAxis axis) { Vect3f K = I*Alg()[axis]*k; Xlg.xformVect(K); K_total += K; }

	void applyLocalForce(Vect3f F, Vect3f r){ Alg().xform(F); Alg().xform(r); F *= mass; F_total += F; K_total += r % F; }
	void applyGlobalForce(float f, eAxis axis, Vect3f r_local) { f *= mass; F_total[axis] += f; Alg().xform(r_local); Vect3f F(0); F[axis] = f; K_total += r_local % F; }

	void applyLocalTurnTorque(const Vect3f& target, const Vect3f& current, float factor = 1.f);
	void applyGlobalTurnTorque(const Vect3f& target, const Vect3f& current, float factor = 1.f);


	// Linear velocity
	const Vect3f& Vglobal() const { return V; }
	Vect3f Vlocal() const { Vect3f Vl; Xlg.invXformVect(V, Vl); return Vl; }
	void setLocalVelocity(Vect3f Vi) { Xlg.xformVect(Vi); V = Vi; }
	void setLocalVelocity(float v, eAxis axis) { Vect3f Vi; Xlg.invXformVect(V, Vi); Vi[axis] = v; Xlg.xformVect(Vi); V = Vi; }
	void setGlobalVelocity(const Vect3f& Vg) { V = Vg; }
	void setGlobalVelocity(float v, eAxis axis) { V[axis] = v; }

	// Angular velocity
	const Vect3f& Wglobal() const { return W; }
	Vect3f Wlocal() const { Vect3f Wl; Xlg.invXformVect(W, Wl); return Wl; }
	void setLocalAngularVelocity(Vect3f Wi) { Xlg.xformVect(Wi); W = Wi; }
	void setLocalAngularVelocity(float w, eAxis axis) { Vect3f Wi; Xlg.invXformVect(W, Wi); Wi[axis] = w; Xlg.xformVect(Wi); W = Wi; }
	void setGlobalAngularVelocity(const Vect3f& Wg) { W = Wg; }
	void setGlobalAngularVelocity(float w, eAxis axis) { W[axis] = w; }
	
	// Integration
	void integrateEuler(float dt);
	void boundVelocities();

	// Contact resolving
	Vect3f global_velocity(const Vect3f& rg) const { return V + W % rg; }
	Vect3f global_acceleration(const Vect3f& rg) const { return F_total*mass_inv + (Ig_inv*K_total) % rg + W % (W % rg); }
	Mat3f& local_mass_matrix(const Vect3f& ri, const Vect3f& rj, Mat3f& K);
	Mat3f& global_mass_matrix(const Vect3f& ri, const Vect3f& rj, Mat3f& K);
	virtual struct ContactProperty contactProperty(const Contact& c) const;

	int is_static() const { return is_static_; }
	int is_dynamic() const { return is_dynamic_; }
	int colliding() const { return ground_colliding | body_colliding; }

	// Fast Car Interface
	real GetMass()const;
	void GetInverseTOIWorld(real t[3][3])const;
	void GetPosition(real position[3]) const; 
	void SetPosition(const real position[3]); 
	void GetQuaternion(real Q[4]) const;
	void SetQuaternion(const real Q[4]);
	void GetVelocity(real velocity[3])const; 
	const real* GetVelocity()const; 
	void SetVelocity(const real velocity[3]); 
	void GetAngularVelocity(real angular_velocity[3])const; 
	const real* GetAngularVelocity()const; 
	void SetAngularVelocity(const real angular_velocity[3]); 

	static Vect3d gravity;

protected:	
	unsigned int is_dynamic_ : 1;
	unsigned int is_static_ : 1;
     	unsigned int ground_colliding : 1;
     	unsigned int body_colliding : 1;
	unsigned int in_contact : 1;
};

class FieldSource;

class Body :  public DynamicBody, public BaseObject, public Grid2DElement
{
	// Friends
	friend ShareHandle<Body>;
	friend class BodyDispatcher;
	friend class FieldSource;
	friend class ContactingBodiesList;
	friend class BodyClipOp;
	friend class MechosWheelsBodyClipOp;
	friend struct BodyPair;
	friend class BodyFeatureHT;
	friend class ContactingBodies;
	friend class FieldGrid;
	friend class Ball;
	friend struct Contact;
	     
public:
	enum Type {
		BODY,

		PART,
		MECHOS,
		BONUS_MAN,
		STATIC_BONUS,
		
		METEORITE,
		DEBRIS,
		STONE,

		BONUS_METEORITE,
		REAL_BONUS_METEORITE,
		TITANIUM_BALL,

		WORM,
		BUTTERFLY,
		SHIP,
		GEAR,
		PIG,
		PENGUIN,
		DOG,
		BREVNO,
		SHARK,
		FISH,

		CRYSTAL,
		TOMATO,
		BUBBLE,
		CLOWN,
		EVIL_EYE,
		SUICIDE,
		LITTLE_SPIDER,
		CATERPILLAR,
		BALL,
		MISSILE,

		WHALE,
		SMART_STONE,
		DRAGON,
		FROG,
		SPIDER,

		BOSS, 
		SPIDER_RACER
		};

	enum BoundType {
		CenteredCircumscribedBound,
		CenteredInscribedBound,
		CenteredScaledBy2Bound,
		CenteredBottomAlignmentedBound,
		VolumetricEquivalentBound, 
		FloatsBound
		};

	Body(class cMesh* geometry, BoundType bound_type = CenteredCircumscribedBound, float spheres_radius_parameter = 0);
	explicit Body(int m3d_type = 0, BoundType bound_type = CenteredCircumscribedBound, float spheres_radius_parameter = 0);
	virtual ~Body();

	Type type() const { return type_; }
	virtual void kill() { alive_ = 0; completed_ = 0; release_body_handles(); }

	int active() const { return active_; }
	virtual void set_activity(int flag) { active_ = flag; }
	
	// Coordinate of cMesh //////	
	Vect3f Rgeometry() const { return R() + Alg()*mesh_offset; }

	// Geometric size
	float width() const { return bbox_.x; }
	float length() const { return bbox_.y; }
	float height()	const { return bbox_.z ; }
	float radius() const { return radius_; }

	// Масштабирование: (1,1,1) - единичное
	void setScale(const Vect3f& scale);
	void setScale(float scale){ setScale(Vect3f(scale, scale, scale)); }

	// Установка координат
	virtual void setPosition(const Vect3f& pose, float psi = 0);
	void setPosition(const Vect3f& pose, const Vect3f& target);

	// Global velocity
	float averageSpeed() const { return speed_avr; } // усредненная норма скорости

	// Верхняя высота по полигональной карте
	float H() const;
	// Max height
	float maxHeight() const; 
	// Relative height
	float relativeHeight() const { return R().z - maxHeight(); }

	// Energy - damage 	
	float Energy() { return energy; }	
	virtual void damage(float decr,int damage_style = 0){ if((energy -= decr) <= 0){ energy = 0; kill(); } }
	virtual float damage_ability(const class Body& b) const { return damage_ability_; } // вызывается  при любом контакте
	virtual float hit_damage_ability(const class Body& b, float velocity) const; // вызывается при ударе дин. тел, модулируется импульсом
	
	// Some physical props ////
	int is_floating() const { return k_archimedean > 1e-12f; }
	float submersion() const { return submersion_; }
	int completed() const { return completed_; }
	int visible() const { return visible_; } // доступен для searchBody

	// Life time
	time_type life_time() const { return life_time_(); }

	// cMesh manipulation
	float getFrame() const;
	void setFrame(float frame);
	class cFrame& findFrame();
	void setAnimation(float AnimationPeriod = 2000.f, float StartPhase = 0.f, float FinishPhase = -1.f, unsigned int mask = 0xffffffff);
	void setChannel(char* channel);
	void setChannelRandom(char* channel);
	void setAlpha(float alpha);
	void setColor(float r, float g, float b);

	// Действующие на тело поля
	int affecting_fields() const { return affecting_fields_; }
	virtual void field_affection(const FieldSource* sensor){}

	// Debug
	void show() const;

	/////////////////////////////////////////////////////
	//	Public data
	/////////////////////////////////////////////////////
	int ID;
	cMesh* geometry;

protected:
	/////////////////////////////////////////////////////
	//	Private data
	/////////////////////////////////////////////////////
	Type type_;

	unsigned int hidden_ : 1;
	unsigned int active_ : 1;
	unsigned int evolve_degeneracy : 1;
	unsigned int completed_ : 1;
     	unsigned int visible_ : 1;
     	unsigned int disable_collision_test : 1;

	unsigned enable_collision_mask;
	unsigned disable_collision_mask;

	// Physical properties //////
	float density;
	float radius_;
	float spheres_radius_parameter;
	Vect3f bbox_;
	BoundType bound_type;
	PolyTree polyhedrons; 
	SphereList spheres;
	MatXf prevXform;

	// Offset
	Vect3f mesh_offset;
	
	// Body to Body properties ////////
	float damage_ability_;
	float hit_damage_ability_;
	float energy;
	
	// Collision info ////////									     
	ContactList contacts;
	ContactList2 contacts2;


	struct NormalContact
	{
		Vect3f r, n;
		int index;
		int body_index;
	};
	vector<NormalContact> normal_contacts;

	struct TangentContact
	{
		Vect3f t;
		int index;
		int local_index;
	};
	vector<TangentContact> tangent_contacts;

	class Mass_Matrix_Array* mass_matrix_array;

	void calc_normal_mass_matrix(class Mat& M);
	void calc_tangent_mass_matrix(Mat& Mnt, Mat& Mtn, Mat& Mtt);
	void calc_dynamic_friction_mass_matrix(Mat& M);
	friend class LCP_ContactSolver;

	int test_collision(int put_contacts);
	int test_body_to_body_collision(const MatXf& X12, Body* body2, FeaturesList& features, int put_contacts);
	friend class PolyhedronClipOp;
	friend class TriangleClipOp;

	// Forces //////
	float speed_avr; // усредненная норма скорости
	
	// Water
	float k_archimedean;
	float k_submersion;
	float submersion_;
	
	// Fields
	unsigned int affecting_fields_;
	unsigned int enabled_fields;

	// Life time
	MeasurementTimer life_time_;

	/////////////////////////////////////////////////////
	//	Protected functions
	/////////////////////////////////////////////////////
	static unsigned myM3DType(int part_kind) { return ((unsigned)part_kind << 16) | 1024; }

	// Sound control
	void startSound(int id);    
	void startSound(int id, float volume, float frequency = 0);
	void updateSound(int id);    
	void stopSound(int id);

	// Поворот на вектор norm = [0..M_PI]
	Vect3f turn_cross(const Vect3f& target, const Vect3f& current) const;

	// Общие свойства
	virtual void calc_forces_and_drags();
	virtual void body_overlap_reaction(Body& body) {} 
	virtual int is_colliding_with(const Body& body) { return 1; }
	virtual void non_dynamic_evolve(float dt){}
	virtual void add_contact(float penetration, const Vect3f& point, const Vect3f& normal, const Sphere& sphere, const Body* body){}

	virtual void ground_hit_reaction(const Contact& c){} // to obtain constraint impulse information
	virtual void body_hit_reaction(const Body& body, const Contact& c){}

	virtual void keyboard_control() {}
	virtual void write_state(XBuffer& buf);

	virtual void release_body_handles(){} // it's called by dispatcher before averall clear to avoid leak due to cycled sharing

	void explode(int mesh_id);

	void internal_init();
	void setMass(float mass);
	void setGeometry(cMesh* geometry);
	void releaseGeometry();
	virtual void createBound();
	virtual void setPositionInit();

	virtual void post_quant();

	void make_non_colliding();
	void place_on_the_ground();
	void linear_projection();

	void set_default_forces();
	void set_archimedean_force(float factor);
	void place_on_the_water();
	void calc_archimedean_force();
	void degeneracy_remedy();
	virtual void integrate(float dt);

	virtual void hide();
	void unhide();
	int hidden() const { return hidden_; }

	// Grid Supporting
	int involve_into_collision_test_counter;
	iRectangle prev_bound;
	// Grid supporting function
	iRectangle getBound() const;

	// Static data
	static int IDs;
	static int instancies;
};

#endif // __BODY_H__
