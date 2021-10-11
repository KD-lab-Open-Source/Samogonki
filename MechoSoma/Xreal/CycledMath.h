#ifndef	__CYCLED_MATH__
#define __CYCLED_MATH__

#pragma warning( disable : 4244 )  

const float H_SIZE_FLT = 2048.f;
const float V_SIZE_FLT = 2048.f;

const double H_SIZE_DBL = 2048.;
const double V_SIZE_DBL = 2048.;

inline double getDistX(double v0,double v1)
{
	double d = v0 - v1;
	double ad = fabs(d);
	double dd = H_SIZE_DBL - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + H_SIZE_DBL : d - H_SIZE_DBL;
}

inline double getDistY(double v0,double v1)
{
	double d = v0 - v1;
	double ad = fabs(d);
	double dd = V_SIZE_DBL - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + V_SIZE_DBL : d - V_SIZE_DBL;
}

inline float getDistX(float v0,float v1)
{
	float d = v0 - v1;
	float ad = (float)fabs(d);
	float dd = H_SIZE_FLT - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + H_SIZE_FLT : d - H_SIZE_FLT;
}

inline float getDistY(float v0,float v1)
{
	float d = v0 - v1;
	float ad = (float)fabs(d);
	float dd = V_SIZE_FLT - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + V_SIZE_FLT : d - V_SIZE_FLT;
}

inline float& cycle(float& f, float size)
{
	f = fmod(fmod(f, size) + size, size);
	return f;
}

inline float getDist(float v0, float v1, float size)
{
	float d = fmod(v0 - v1, size);
	float ad = (float)fabs(d);
	float dd = size - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + size : d - size;
}

inline float& uncycle(float& f1, float f0, float size)
{
	f1 = f0 + getDist(f1, f0, size);
	return f1;
}


//////////////////////////////////////////////////////////////////
//	Cycled Vect3d
//////////////////////////////////////////////////////////////////
inline Vect3d& CYCLE(Vect3d &R)
{
	R.x = fmod(R.x + H_SIZE_DBL, H_SIZE_DBL);
	R.y = fmod(R.y + V_SIZE_DBL, V_SIZE_DBL);
	return R;
}
inline Vect3d getDist(const Vect3d &P1,const Vect3d &P0)
{
	return Vect3d( getDistX(P1.x,P0.x), getDistY(P1.y,P0.y), P1.z - P0.z );
}
inline Vect3d getDelta(const Vect3d &P1,const Vect3d &P0)
{
	return Vect3d( fmod(P1.x - P0.x + H_SIZE_DBL, H_SIZE_DBL), fmod(P1.y - P0.y + V_SIZE_DBL, V_SIZE_DBL), P1.z - P0.z );
}
inline Vect3d& UNCYCLE(Vect3d &P1,const Vect3d &P0)
{
	P1.x = P0.x + getDistX(P1.x, P0.x);
	P1.y = P0.y + getDistY(P1.y, P0.y);
	return P1;
}
inline Vect3d getUNCYCLED(const Vect3d &P1,const Vect3d &P0)
{
	return Vect3d (P0.x + getDistX(P1.x, P0.x),
			     P0.y + getDistY(P1.y, P0.y),
			     P1.z);
}

//////////////////////////////////////////////////////////////////
//	Cycled Vect3f
//////////////////////////////////////////////////////////////////
inline Vect3f& CYCLE(Vect3f &R)
{
	R.x = fmod(R.x + H_SIZE_FLT, H_SIZE_FLT);
	R.y = fmod(R.y + V_SIZE_FLT, V_SIZE_FLT);
	return R;
}
inline Vect3f getDist(const Vect3f &P1,const Vect3f &P0)
{
	return Vect3f( getDistX(P1.x,P0.x), getDistY(P1.y,P0.y), P1.z - P0.z );
}
inline Vect3f getDelta(const Vect3f &P1,const Vect3f &P0)
{
	return Vect3f( fmod(P1.x - P0.x + H_SIZE_FLT, H_SIZE_FLT), fmod(P1.y - P0.y + V_SIZE_FLT, V_SIZE_FLT), P1.z - P0.z );
}
inline Vect3f& UNCYCLE(Vect3f &P1,const Vect3f &P0)
{
	P1.x = P0.x + getDistX(P1.x, P0.x);
	P1.y = P0.y + getDistY(P1.y, P0.y);
	return P1;
}
inline Vect3f getUNCYCLED(const Vect3f &P1,const Vect3f &P0)
{
	return Vect3f (P0.x + getDistX(P1.x, P0.x),
			     P0.y + getDistY(P1.y, P0.y),
			     P1.z);
}


#endif // __CYCLED_MATH__
