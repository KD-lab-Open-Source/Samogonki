#ifndef __STATISTICAL_ANALIZATOR_H__
#define __STATISTICAL_ANALIZATOR_H__

#include "math.h"
#include "_xtool.h"

#define NOMINMAX
#undef min
#undef max

class StatisticalAnalizator
{
	int cnt;
	double x_sum, x2_sum, x_min, x_max;

public:
	StatisticalAnalizator(){ init(); }
	void init() { cnt = 0; x_sum = x2_sum = 0; x_max = -1e+15; x_min = 1e+15; }
	void add(double x) { cnt++; x_sum += x; if(x_min > x) x_min = x; if(x_max < x) x_max = x; x2_sum += x*x; }

	int count() const { return cnt; }
	double avr() const { return x_sum/cnt; }
	double sigma() const { return sqrt((x2_sum - x_sum*x_sum/cnt)/((double)cnt*(cnt - 1))); }
	double max() const { return x_max; }
	double min() const { return x_min; }
			    
	friend XStream& operator <= (XStream& s, const StatisticalAnalizator& x)
	{
		s < "Count: " <= x.count() < "\t";
		if(x.count()){
			s < "Avr: " <= x.avr() < "\t";
			s < "Min: " <= x.min() < "\t";
			s < "Max: " <= x.max() < "\t";
			s < "Sigma: " <= x.sigma()*100./fabs(x.avr()) < " % ";
			}
		return s;
	}
};

class StaticStatisticalAnalizator : public StatisticalAnalizator
{
	char* fname;
public:
	StaticStatisticalAnalizator(char* fname_) { fname = fname_; }
	~StaticStatisticalAnalizator() 
	{  
		XStream f(fname, XS_OUT); 
		f <= *this < "\n";
	}
};

#endif __STATISTICAL_ANALIZATOR_H__
