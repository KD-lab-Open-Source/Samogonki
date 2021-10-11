#ifndef __POLYGRID_H__
#define __POLYGRID_H__

#include "camera.h"
#include "material.h"

#define POLYGRID_NOT_USE								(0<<0)
#define POLYGRID_USE									(1<<0)

#define POLYGRID_NOT_GURO								(1<<1)

class cLight;

struct sPointPolyGrid
{
	unsigned char r,g,b,a;
	unsigned char du,dv;											// 0.f < u+(du/255) < 1.f
	char attribute;
	int Time,Delta;
};

class cPolyGrid : public cMaterial
{
public:
	int					xsize,ysize;								// size 2d-array Points
	int					xstep,ystep;								// step size of 
	float				du,dv;										// ofs texture for move
	float				uofs,vofs;									// ofs texture 
	float				usize,vsize;								// 0 < texture size < 1
	unsigned int		attribute;
	sPointPolyGrid		*Point;
	cMatrix				LocalMatrix;
	cPolyGrid			*pgLU,	*pgU,	*pgRU;
	cPolyGrid			*pgL,/*this*/	*pgR;
	cPolyGrid			*pgLD,	*pgD,	*pgRD;

	cPolyGrid();
	~cPolyGrid();

	void PreDraw(cLight *LightObject);						
	void New(int xsize,int ysize,int xstep,int ystep);
	inline void SetPosition(float x,float y,float z)				{ LocalMatrix.trans().set(x,y,z); }
	inline float& x()												{ return LocalMatrix.trans().x; }
	inline float& y()												{ return LocalMatrix.trans().y; }
	inline float& z()												{ return LocalMatrix.trans().z; }
	inline sPointPolyGrid& operator () (int i,int j)				{ assert(Point&&(i>=0)&&(j>=0)&&(i<xsize)&&(j<ysize)); return Point[i+j*xsize]; }
	inline void BuildDrawMatrix(cCamera *Camera,MatXf &DrawMatrix,Vect3f &TilePos)
	{
		DrawMatrix=LocalMatrix;
		WorldToCameraCutting(DrawMatrix,Camera);
		TilePos=DrawMatrix.trans()-Camera->GetPos();
		DrawMatrix.trans().x=Camera->GetPos().x; DrawMatrix.trans().y=Camera->GetPos().y;
		DrawMatrix=Camera->GetMatrix()*DrawMatrix;
	}
private:
#ifdef _DEBUG
	int AssertValid();
#else
	inline int AssertValid()										{ return 1; }
#endif
};

#endif //__POLYGRID_H__