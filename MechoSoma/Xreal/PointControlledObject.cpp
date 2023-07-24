#include "StdAfx.h"
#include "Body.h"
#include "PointControlledObject.h"
#include "Params.h"
#include "MechosPrm.h"
#include "Xreal_utl.h"
#include "BodyDispatcher.h"
#include "DebugPrm.h"

////////////////////////////////////////////////////////////////////////////////
//		Control Points 
////////////////////////////////////////////////////////////////////////////////
PointControlledObject::PointControlledObject()
{
	back_moving_enabled = 1;
	stopping = 0;
	set_traction_by_distance = 1;
	curr_traction = 1;
	traction_modulation = 1;
	dist_to_control_point = 0;
	catch_width = catch_width_general;
	catch_length = catch_length_general;
	allowed_width = allowed_width_general;
	control_description = "Initial";
	sliding_factor = 1;
	enable_control = 1;
	AI_controlled = 1;
	check_points_only_controlled_ = 0;
	disable_traction_modulation = 0;
	ignore_point_control = 0;
	do_brake = 0;
	controled_only_by_point = 0;
	enable_joystick_control = 0;
}

void PointControlledObject::setPositionInit()
{
	psi_prev = psi();	
	velocity_y_avr = traction_avr = 0;
	actual_curvature = required_curvature = 0;
	backward_maneuver.stop();
	ControlledObject::setPositionInit();
}	

int PointControlledObject::add_control_point(const Vect3f& point)
{				
	if(ignore_point_control)
		return 0;

	control_points.push_back(point);
	if(num_control_points() == 1)
		take_control_point();

	return num_control_points();
}	
int PointControlledObject::del_control_point()
{
	if(num_control_points())
		control_points.pop_back();
	return num_control_points();
}
void PointControlledObject::del_control_point(const Vect3f& point)
{
	ControlPointList::iterator pi;
	if(num_control_points())						
		FOR_EACH(control_points, pi)
			if(pi -> x == point.x && pi -> y == point.y){
				control_points.erase(pi);
				take_control_point();
				break;
				}
}
void PointControlledObject::move_control_point(const Vect3f& prev_point, const Vect3f& curr_point)
{
	if(target.x == prev_point.x && target.y == prev_point.y)
		target = curr_point;
	
	ControlPointList::iterator pi;
	if(num_control_points())
		FOR_EACH(control_points, pi)
			if(pi -> x == prev_point.x && pi -> y == prev_point.y){
				*pi = curr_point;
				break;
				}
}

////////////////////////////////////////////////////////////////////////////////
//	Control by Points
////////////////////////////////////////////////////////////////////////////////
float Curvature(float x, float y)
{
	return -2*x/(sqr(x) + sqr(y) + curvature_denominator_anti_singular_addition);
}

class CurvatureInterval
{
	float left;
	float right;
public:

	CurvatureInterval()
	{
		left = -DBL_INF;
		right = DBL_INF;
	}

	CurvatureInterval(float x, float y, float dx)
	{
		left = Curvature(x + dx, y);
		right = Curvature(x - dx, y);
		if(left > right)
			std::swap(left, right);
	}

	int intersect(const CurvatureInterval& ci)
	{
		float l = max(left, ci.left);
		float r = ::min(right, ci.right);
		if(l > r)
			return 0;
		left = l;
		right = r;
		return 1;
	}

	float avr()
	{
		if(SIGN(left) != SIGN(right))
			return 0;
		return (left + right)/2;
	}

	float min()
	{
		if(SIGN(left) != SIGN(right))
			return 0;
		return minAbs(left, right);
	}

};
		
void PointControlledObject::target_achieved()
{
	ControlledObject::target_achieved();	
	if(num_control_points()){
		control_points.pop_front();
		removePassedSeed();
		take_control_point();
		}
}

void PointControlledObject::point_control()
{
	Mat3f A_l2g_plane(-psi(), Z_AXIS);

	Vect3f T = getDist(target, R());
	A_l2g_plane.invXform(T);
	  
	// Check if point is passed
	if(fabs(T.x) < catch_width && fabs(T.y) < catch_length){
		target_achieved();  // Подбор семени
		setRudder(0);
		return;
		}
	
	// Calc rudder & traction
	if(backward_maneuver() || (T.y < 0 && T.y > -back_moving_ymax && fabs(T.x/T.y) < back_moving_kmax)){
		// Backward moving
		if(Vlocal().y > 0)  // Еще не остановился 
			setRudder(0);
		else{
			Vect3f D(T.x, T.y, 0);
			D.normalize(1.f - FLT_EPS); // acos safe
			if(!backward_maneuver){
				float r = acos(-D.y)/M_PI;
				setRudder(D.x > 0 ? -r : r);
				control_description = "Backward";
				}
			else{
				float r = 2*acos(D.y)/M_PI;
				setRudder(D.x > 0 ? r : -r);
				// Проверка на выключение маневра
				if(backward_maneuver_obstacle_timer(backwardObstacle(), backward_maneuver_obstacle_timer_duration) 
				  || fabs(r) < backward_maneuver_threshold // Выровнялся на цель
				  || backward_maneuver_velocity_timer(velocity_y_avr < velocity_y_avr_zero && traction_avr < -traction_avr_zero, backward_maneuver_velocity_timer_duration)){ // Есть тяга, но нет хода
					backward_maneuver_obstacle_timer.stop();
					backward_maneuver.stop();
					velocity_y_avr = traction_avr = 0;
					resetObstacle();
					point_control();
					return;
					}
				}
			}
		setTraction(-curr_traction/2);
		}
	else{ //  Forward moving

		//  Проверка на препятствие 
		if(backward_maneuver_obstacle_timer(forwardObstacle(), backward_maneuver_obstacle_timer_duration) 
   		  || backward_maneuver_velocity_timer(velocity_y_avr < velocity_y_avr_zero && traction_avr > traction_avr_zero, backward_maneuver_velocity_timer_duration)){
			backward_maneuver_obstacle_timer.stop();
			backward_maneuver.start(backward_maneuver_duration);
			velocity_y_avr = traction_avr = 0;
			resetObstacle();
			control_description = "Backward maneuver against obstacle";
			point_control();
			return;
			}

		//  Проверка на очень крутые повороты
//		CurvatureInterval curv0(T.x, T.y, allowed_width);
//		if(fabs(curv0.min()) > max_curvature){
//			backward_maneuver.start(1500);
//			control_description = "Backward maneuver due to big curvature";
//			point_control();
//			return;
//			}

		if(T.y > -40 && (fabs(T.x)/(fabs(T.y) + FLT_EPS) < front_moving_kmax || fabs(T.x) < allowed_width)){ // Передняя зона - оптимизация
			CurvatureInterval curv;
			int n = 0;
			ControlPointList::iterator pi;
			FOR_EACH(control_points, pi){
				if(n++ > 20)
					break;
				Vect3f T = getDist(*pi, R());
				A_l2g_plane.invXform(T);
				if(!curv.intersect(CurvatureInterval(T.x, T.y, allowed_width) ) )
					break;
				}

			#ifndef _FINAL_VERSION_
			static XBuffer buf;
			buf.init();
			buf < "Fwd opt: " <= n;
			control_description = buf.address();
			#endif

			required_curvature = curv.avr();
			if(required_curvature){
				float cf = actual_curvature/required_curvature;
				if(required_curvature > 0){
					if(rudder < 0) 
						rudder = 0;
					if(cf < min_relative_curvature)
						rudder += rudder_step;
					if(cf > max_relative_curvature)
						rudder -= rudder_step;
					}
				else{
					if(rudder > 0) 
						rudder = 0;
					if(cf < min_relative_curvature)
						rudder -= rudder_step;
					if(cf > max_relative_curvature)
						rudder += rudder_step;
					}
				if(fabsf(rudder) > 1){
					rudder = SIGN(rudder);
					if(Vlocal().y > brake_lower_speed){
						control_description = "Fwd opt and brake";
						brake = (Vlocal().y - brake_lower_speed)/(brake_upper_speed - brake_lower_speed);
						}
					}
				
				if(Vlocal().y < 1)
					rudder = 0;
				}

//			setRudder(curv.avr()*curvature_factor);
			}
		else{ //  Задняя зона - максимальный руль
			setRudder(T.x <= 0 ? 1 : -1);
			control_description = "Fwd max";
			if(Vlocal().y > brake_lower_speed){
				control_description = "Fwd max and brake";
				brake = (Vlocal().y - brake_lower_speed)/(brake_upper_speed - brake_lower_speed);
				}
			}

		if(sliding_factor > 0.9)
			setTraction(curr_traction);
		else{
			setTraction(curr_traction/2);
			if(fabs(rudder *= 2) > 1)
				rudder = SIGN(rudder);
			}
		}

	float dist = getDist(target, R()).norm();
	stopping = control_point_approach_timer(dist_to_control_point < dist, control_point_approach_duration);
	if(dist_to_control_point > dist - control_point_approach_epsilon)
		dist_to_control_point = dist - control_point_approach_epsilon;
}

void PointControlledObject::take_control_point()
{
	if(control_points.empty()){
		control_mode = NO_TARGET; 
		return;
		}
	control_mode = STATIC_TARGET; 
	target = control_points.front();
	dist_to_control_point = getDist(target, R()).norm();
	curr_traction = set_traction_by_distance ? dist_to_control_point*k_distance_to_traction*traction_modulation : traction_modulation;
	if(disable_traction_modulation)
		curr_traction = 1;
	if(curr_traction > Mdisp -> maxTraction())
		curr_traction = Mdisp -> maxTraction();
	reset_stopping();
}

void PointControlledObject::reset_stopping()
{
	dist_to_control_point = getDist(target, R()).norm();
	control_point_approach_timer.stop();
	velocity_y_avr = traction_avr = 0;
	backward_maneuver.stop();
	stopping = 0;
}

//__forceinline void average(Vect3f& x_avr, const Vect3f& x, float tau){ x_avr = x_avr*(1.f - tau) + tau*x; }
//Vect3f actual_curvature_(0);

void PointControlledObject::control()
{
	average( velocity_y_avr, Vlocal().y, velocity_y_avr_tau );
	average( traction_avr, traction, traction_avr_tau );
	brake = do_brake;
	
	float psi_curr = psi();
	float dl = getDist(R_prev, R()).norm();
	actual_curvature = dl > 1e-7 ? getDist_f(psi_prev, psi_curr, 2*M_PI)/dl : 0;
	psi_prev = psi_curr;

//	average( actual_curvature_, (Vglobal() % Wglobal() % Vglobal())/(pow(Vglobal().norm(), 3) + FLT_EPS), curvature_avr_tau);

#ifndef _FINAL_VERSION_
	if(exclusive_keyboard_control && active()){
		keyboard_control();
		return;
		}
#endif

	if(enable_control)
		if(num_control_points()){
			#ifndef _FINAL_VERSION_
				if(disable_point_control)
					return;
			#endif
			point_control();
			}
		else{
			if(!controled_only_by_point && active())
				keyboard_control();
			}
	else
		traction = rudder = 0;
}

void PointControlledObject::stop_movement()
{
	clearForces();
	clearVelocities();
	traction = rudder = 0;
}

void PointControlledObject::set_brake_for_turn_based(int on)
{ 
	enable_control = !on; 
	do_brake = on;
	traction = rudder = 0; 
	clearForces();
	clearVelocities();
}

void PointControlledObject::write_state(XBuffer& buf)
{
	ControlledObject::write_state(buf);
	buf < "num_control_points: " <= num_control_points() < "\n";
	buf < "control_description: " < control_description.c_str() < "\n";
	buf < "velocity_y_avr: " <= velocity_y_avr < "\n";
	buf < "traction_avr: " <= traction_avr < "\n";
	buf < "actual_curvature: " <= actual_curvature < "\n";
	buf < "relative_curvature: " <= actual_curvature/(required_curvature + FLT_EPS) < "\n";
	buf < "dist_to_control_point: " <= dist_to_control_point < "\n";
	buf < "stopping: " <= stopping < "\n";
	buf < "-------------------------------------------\n";
}
	
void PointControlledObject::show() const
{
	ControlledObject::show();

	if(show_point_controlled_points){
		ControlPointList::const_iterator i;
		FOR_EACH(control_points, i){
			show_vector(*i, 3, XCOL(CYAN, 200));
			show_map_circle(*i, 3, CYAN);
			}

		if(!control_points.empty())
			show_vector(control_points.front(), 3, CYAN);
		}
}
