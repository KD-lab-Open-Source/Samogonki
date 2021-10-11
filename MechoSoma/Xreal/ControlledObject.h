#ifndef __CONTROLLED_OBJECT_H__
#define __CONTROLLED_OBJECT_H__

class ControlledObject : virtual public Body 
{
public:
	enum ControlMode {
			NO_TARGET,
			STATIC_TARGET,  // run to
			DYNAMIC_TARGET,
			INVERSE_STATIC_TARGET,	// run away
			INVERSE_DYNAMIC_TARGET, 
			TARGET_ACHIEVED
			};

	ControlledObject();

	void setRudder(float rudder_);
	void setTraction(float traction_);
	void setTarget(const Vect3f& r);
	void setTarget(const Body* b);
	void setInverseTarget(const Vect3f& r);
	void setInverseTarget(const Body* b);

	virtual void control();
	virtual void target_achieved();

	void write_state(XBuffer& buf);
	void keyboard_control();

	virtual int obstacle();

protected:
	ControlMode control_mode;
	float k_rudder;
	float k_traction;
	float traction;
	float rudder;
	Vect3f target;
	ShareHandle<const Body> dynamic_target;
	float brake;

	void setPositionInit();
	void release_body_handles();
	void show() const;
};

class AttackAndHomeObject : virtual public ControlledObject 
{
public:
	AttackAndHomeObject(float max_distance, int do_attack = 0, int waiting_time_ = 2000);	
	void setPositionInit();
	void control();
	void show() const;

private:	
	int do_attack;
	float max_distance;
	int waiting_time;
	Vect3f home;
	DurationTimer turn_latency;
};

#endif __CONTROLLED_OBJECT_H__
