#ifndef _TRACK_SPLINE_
#define _TRACK_SPLINE_

#include "TrackIterator.h"

class TrackSpline 
{
	int step;
	TrackIterator p[4];	
	Vect3f P[4];
	float t, weight;
	float average_distance;

	void shift(int dir);

public:

	TrackSpline(float avr_dist = 30.f, int step = 4);
	TrackSpline& set(const TrackIterator& ti);
	TrackSpline& operator += (float dt);
	TrackSpline& operator -= (float dt);

	Vect3f operator()() const;
	Vect3f deriv() const;
	void show() const;
};

#endif // _TRACK_SPLINE_
