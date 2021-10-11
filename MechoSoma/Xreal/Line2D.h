#ifndef _LINE2D_H_
#define _LINE2D_H_

struct Line2D
{
	float a0, a1;

	Line2D(float x0, float y0, float x1, float y1)
	{
		a1 = (y1 - y0)/(x1 - x0);
		a0 = y0 - x0*a1;
	}
	float operator()(float x) { return a0 + a1*x; }
};

#endif // _LINE2D_H_
