#include "StdAfx.h"
#include "TrackSpline.h"
#include "CycledMath.h"
#include "xreal_utl.h"

TrackSpline::TrackSpline(float avr_dist, int step_)
{
	average_distance = avr_dist;
	step = step_;
}

TrackSpline& TrackSpline::set(const TrackIterator& ti)
{
	p[0] = p[1] = ti;
	p[0] -= step;
	p[2] = p[1];
	p[2] += step;
	p[3] = p[2];
	p[3] += step;

	shift(0);

	return *this;
}

void TrackSpline::shift(int dir)
{
	t = 0;
	switch(dir){
		case -1:
			p[3] = p[2];
			p[2] = p[1];
			p[1] = p[0];
			p[0] -= step;
			t = 1.f;
			break;
		case 1:
			p[0] = p[1];
			p[1] = p[2];
			p[2] = p[3];
			p[3] += step;
			break;
		}

	P[0] = p[0];
	P[1] = p[1];
	P[2] = p[2];
	P[3] = p[3];

	UNCYCLE(P[0], P[1]);
	Vect3f d21 = getDist(P[2], P[1]);
	Vect3f d32 = getDist(P[3], P[2]);
	P[2] = P[1] + d21;
	P[3] = P[2] + d32;

	weight = P[2].distance(P[1]);
	if(weight > 1e-5f)
		weight = average_distance/weight;
	else
		weight = 1.f;
}

TrackSpline& TrackSpline::operator += (float dt)
{
	if((t += dt*weight) > 1.f)
		shift(1);
	return *this;
}

TrackSpline& TrackSpline::operator -= (float dt)
{
	if((t -= dt*weight) < 0)
		shift(-1);
	return *this;
}

Vect3f TrackSpline::operator()() const 
{
	return CYCLE((3.0f/2.0f*P[1]-3.0f/2.0f*P[2]-P[0]/2.0f+P[3]/2.0f)*t*t*t +
		(2.0f*P[2]+P[0]-P[3]/2.0f-5.0f/2.0f*P[1])*t*t + (P[2]/2.0f-P[0]/2.0f)*t + P[1]);
}

Vect3f TrackSpline::deriv() const 
{
	return 3.0f*(3.0f/2.0f*P[1]-3.0f/2.0f*P[2]-P[0]/2.0f+P[3]/2.0f)*t*t +
		2.0f*(2.0f*P[2]+P[0]-P[3]/2.0f-5.0f/2.0f*P[1])*t + (P[2]/2.0f-P[0]/2.0f);
}

void TrackSpline::show() const
{
	for(int i = 0; i < 4; i++){
		Vect3f v(P[i]);
		show_map_circle(CYCLE(v), 3, GREEN);
		}
	Vect3f p = (*this)();
	show_map_circle(p, 3, RED);
	show_map_line(p, p + deriv().Normalize(200), RED);
}
