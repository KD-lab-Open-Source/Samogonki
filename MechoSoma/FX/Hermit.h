////////////////////////////////////////////////////////////////////////////////
//	Hermit 2.0
//	Hermit's spline of arbitrary arithmetical types
//	(C) 1999 K-D Lab
//	Author: Alexandre Kotliar
////////////////////////////////////////////////////////////////////////////////
#include <list>

template <class Vector>
class HermitIterator {
	typedef typename list<Vector>::iterator iterator;
	float t;
	Vector P[4];
	list<Vector>* points;
	iterator segment;
	int is_out;
	int check(int assign);
public:
		HermitIterator(list<Vector> &points_);
		HermitIterator();

	HermitIterator& set(const iterator &i_, float t_);
	HermitIterator& set(list<Vector>* points_,const iterator &i_, float t_);
	HermitIterator& operator += (const float &dt);	
	HermitIterator& operator -= (const float &dt);	
	Vector operator()();   // function
	Vector deriv();		// derivation
	int isOut() { return is_out || t < 0 || t > 1; }
	};


////////////////////////////////////////////////////////////////////////////////
//		Implementation
////////////////////////////////////////////////////////////////////////////////
template <class Vector>
inline HermitIterator<Vector>::HermitIterator(list<Vector> &points_)
{
	t = 0;
	points = &points_;
	segment = points_.begin();
	check(1);
}

template <class Vector>
inline HermitIterator<Vector>::HermitIterator()
{
	t = 0;
	points = NULL;
}

template <class Vector>
inline HermitIterator<Vector>& HermitIterator<Vector>::set(const iterator &i_, float t_)
{
	t = t_;
	segment = i_;
	check(1);
	return *this;
}

template <class Vector>
inline HermitIterator<Vector>& HermitIterator<Vector>::set(list<Vector> *points_,const iterator &i_, float t_)
{
	points = points_;
	t = t_;
	segment = i_;
	check(1);
	return *this;
}

template <class Vector>
inline HermitIterator<Vector>& HermitIterator<Vector>::operator += (const float &dt)
{
	t += dt;
	check(0);
	return *this;
}

template <class Vector>
inline HermitIterator<Vector>& HermitIterator<Vector>::operator -= (const float &dt)
{
	*this += -dt;
	return *this;
}

template <class Vector>
int HermitIterator<Vector>::check(int assign)
{
	is_out = 1;
	if(segment == points -> end()){
		if(points -> size() < 2)
			return 0;
		segment = points -> begin();
		assign = 1;
		}
		
	while(t >= 1 && segment != -- --points -> end()){
		++segment;
		t -= 1;
		assign = 1;
		}
	while(t < 0 && segment != points -> begin()){
		--segment;
		t += 1;
		assign = 1;
		}
		
	if(assign){
		if(points -> size() < 2)
			return 0;

		iterator p_minus, p_plus;
		p_minus = p_plus = segment;

		if(p_minus != points -> begin())
			--p_minus;
		P[0] = *p_minus;
		P[1] = *segment;
		if(p_plus != --points -> end())
			++p_plus;
		P[2] = *p_plus;
		if(p_plus != --points -> end())
			++p_plus;
		P[3] = *p_plus;
		}		
	
	is_out = 0;
	return 1;
}

template <class Vector>
inline Vector HermitIterator<Vector>::operator()()
{	
	return (3.f/2.f*P[1]-3.f/2.f*P[2]-P[0]/2.f+P[3]/2.f)*t*t*t +
		(2.f*P[2]+P[0]-P[3]/2.f-5.f/2.f*P[1])*t*t + (P[2]/2.f-P[0]/2.f)*t + P[1];
}

template <class Vector>
inline Vector HermitIterator<Vector>::deriv()
{	
	return 3.f*(3.f/2.f*P[1]-3.f/2.f*P[2]-P[0]/2.f+P[3]/2.f)*t*t +
			2.f*(2.f*P[2]+P[0]-P[3]/2.f-5.f/2.f*P[1])*t + (P[2]/2.f-P[0]/2.f);
}
   

