///////////////////////////////////////////////////////////////////////////////
// Основа для управляемых точками объектов
///////////////////////////////////////////////////////////////////////////////
#ifndef __POINT_CONTROLLED_OBJECT__
#define __POINT_CONTROLLED_OBJECT__

#include "ControlledObject.h"

class PointControlledObject : virtual public ControlledObject 
{
protected:
	typedef list<Vect3f> ControlPointList;

	ControlPointList control_points;

	unsigned int back_moving_enabled : 1;
	unsigned int stopping : 1;  // Не могу пройти 
	unsigned int set_traction_by_distance : 1;  // Дальше семя - быстрее ехать
	unsigned int enable_control : 1; // Разрешает управление
	unsigned int enable_joystick_control : 1; 
	unsigned int controled_only_by_point: 1; // Управляется только точками - для hotseat
	unsigned int do_brake : 1; // Включает тормоз
	unsigned int AI_controlled : 1; // Управляется АИ
	unsigned int check_points_only_controlled_ : 1; // Управляется только чек-пойтами (без промежуточных контрол-пойнтов)
	unsigned int disable_traction_modulation : 1; // Как расстоянием от семени до семени, так и внешним АИ
	unsigned int ignore_point_control : 1; // Для лягушки, которая прыгает по кувшинкам

	// Control points ///////
	float traction_modulation; // 0..2, по умолчанию 1  
	float curr_traction;
	float dist_to_control_point;
	AverageConditionTimer control_point_approach_timer;
	float catch_width, catch_length;
	float allowed_width;
	float sliding_factor;

	// States //////////
	float traction_avr;
	float velocity_y_avr;
	string control_description;

	// Controls /////////
	AverageConditionTimer backward_maneuver_obstacle_timer;
	AverageConditionTimer backward_maneuver_velocity_timer;
	DurationTimer backward_maneuver;

	// Curvature
	float required_curvature;
	float actual_curvature;
	float psi_prev;


	// Protected methods
	void setPositionInit();
	void write_state(XBuffer& buf);

	void point_control();
	
	virtual int forwardObstacle() { return 0; }
	virtual int backwardObstacle() { return 0; }
	virtual void resetObstacle() {}

public:
		PointControlledObject();

	int num_control_points() const { return control_points.size(); }

	int add_control_point(const Vect3f& point);
	int del_control_point(); 
	void del_control_point(const Vect3f& point); 
	void move_control_point(const Vect3f& prev_point, const Vect3f& curr_point);
	void take_control_point(); // recalculates control parameters

	void control();
	void target_achieved(); 
	void show() const;

	void disable_control() { enable_control = 0; rudder = 0; traction = 0; }
	void stop_movement();
	virtual void set_brake(int on) { is_dynamic_ = !on; is_static_ = on; traction = !on; }
	void set_brake_for_turn_based(int on);
	void set_joystick_control(int on) { enable_joystick_control = on; }
	void reset_stopping();

	// Bonus Interface
	virtual void AddBonus(int id){}
	virtual int CheckBonusSlot(int id){ return 0; }
	
	// Control Point Interface
	virtual int AddSeed(const Vect3f& point, int tp = 0){ return 0; }
	virtual void removePassedSeed(){}
	virtual void RemoveSeeds(){}

	int check_points_only_controlled() { return check_points_only_controlled_; }
};

#endif  // __POINT_CONTROLLED_OBJECT__
