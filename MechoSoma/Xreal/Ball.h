#ifndef __BALL_H__
#define __BALL_H__

class Ball : public Body
{
	ShareHandle<Body> mechos;
	DurationTimer time_to_live;
	DurationTimer wave_timer;
	float psi;
	int control_config;
	int kill_mechos;
public:
	Ball(Body* mechos_);
	void release_body_handles();
	void quant();
	void kill();
	void post_quant();
	void ground_hit_reaction(const Contact& c);
	void calc_forces_and_drags();
};


#endif  // __BALL_H__