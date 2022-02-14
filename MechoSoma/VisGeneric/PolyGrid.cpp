#include "float.h"
#include "PolyGrid.h"
// TODO: @caiiiycuk investigate this
#ifdef SOFTWARE_RENDER
#include "SoftWare16.h"
#endif

cPolyGrid::cPolyGrid()
{
	Point=0;
	LocalMatrix.NewMatrix();
	xstep=ystep=xsize=ysize=0;
	uofs=vofs=du=dv=0; usize=vsize=1.f; 
	pgL=pgR=pgU=pgD=pgLU=pgLD=pgRU=pgRD=0;
	attribute=0;
}
cPolyGrid::~cPolyGrid()
{
	if(Point) delete Point; Point=0;
}
inline void SetPoint2(sPointPolyGrid &pl,sPointPolyGrid &pr)
{
	pl.du=pr.du=(pl.du+pr.du)>>1; 
	pl.dv=pr.dv=(pl.dv+pr.dv)>>1; 
	pl.r=pr.r=(pl.r+pr.r)>>1; 
	pl.g=pr.g=(pl.g+pr.g)>>1; 
	pl.b=pr.b=(pl.b+pr.b)>>1; 
	pl.a=pr.a=(pl.a+pr.a)>>1; 
}
inline void SetPoint4(sPointPolyGrid &pl,sPointPolyGrid &pr,sPointPolyGrid &pu,sPointPolyGrid &pd)
{
	pl.du=pr.du=pu.du=pd.du=(pl.du+pr.du+pu.du+pd.du)>>2; 
	pl.dv=pr.dv=pu.dv=pd.dv=(pl.dv+pr.dv+pu.dv+pd.dv)>>2; 
	pl.r=pr.r=pu.r=pd.r=(pl.r+pr.r+pu.r+pd.r)>>2; 
	pl.g=pr.g=pu.g=pd.g=(pl.g+pr.g+pu.g+pd.g)>>2; 
	pl.b=pr.b=pu.b=pd.b=(pl.b+pr.b+pu.b+pd.b)>>2; 
	pl.a=pr.a=pu.a=pd.a=(pl.a+pr.a+pu.a+pd.a)>>2; 
}
void cPolyGrid::PreDraw(cLight *LightObject)
{ // производится зацикливание
	if(attribute&POLYGRID_NOT_GURO) return;
	sPointPolyGrid pNull;
	int xend=xsize-1,yend=ysize-1;
	memset(&pNull,0,sizeof(sPointPolyGrid));
	sPointPolyGrid *pU_LU,*pL_LU,*pLU_LU,*p_LU=&Point[0+0*xsize];
	sPointPolyGrid *pU_RU,*pR_RU,*pRU_RU,*p_RU=&Point[xend+0*xsize];
	sPointPolyGrid *pD_LD,*pL_LD,*pLD_LD,*p_LD=&Point[0+yend*xsize];
	sPointPolyGrid *pD_RD,*pR_RD,*pRD_RD,*p_RD=&Point[xend+yend*xsize];
	if(pgL)	// left border
	{
		pL_LU=&pgL->Point[xend]; pL_LD=&pgL->Point[xend+yend*ysize];
		for(int j=1;j<yend;j++)
			if(Point[(0+0)+(j+0)*xsize].attribute!=POLYGRID_NOT_USE)
				SetPoint2(Point[(0+0)+(j+0)*xsize],pgL->Point[(xend+0)+(j+0)*xsize]);
	} else pL_LU=pL_LD=&pNull;
	if(pgR)	// right border
	{
		pR_RU=&pgR->Point[0];	pR_RD=&pgR->Point[yend*ysize];
		for(int j=1;j<yend;j++)
			if(Point[(xend+0)+(j+0)*xsize].attribute!=POLYGRID_NOT_USE)
				SetPoint2(Point[(xend+0)+(j+0)*xsize],pgR->Point[(0+0)+(j+0)*xsize]);
	} else pR_RU=pR_RD=&pNull;
	if(pgU)	// up border
	{
		pU_LU=&pgU->Point[yend*ysize]; pU_RU=&pgU->Point[xend+yend*ysize]; 
		for(int i=1;i<xend;i++)
			if(Point[(i+0)+(0+0)*xsize].attribute!=POLYGRID_NOT_USE)
				SetPoint2(Point[(i+0)+(0+0)*xsize],pgU->Point[(i+0)+(yend+0)*xsize]);
	} else pU_LU=pU_RU=&pNull;
	if(pgD)	// down border
	{
		pD_RD=&pgD->Point[xend]; pD_LD=&pgD->Point[0]; 
		for(int i=1;i<xend;i++)
			if(Point[(i+0)+(yend+0)*xsize].attribute!=POLYGRID_NOT_USE)
				SetPoint2(Point[(i+0)+(yend+0)*xsize],pgD->Point[(i+0)+(0+0)*xsize]);
	} else pD_RD=pD_LD=&pNull;
	if(pgLU) pLU_LU=&pgLU->Point[xend+yend*xsize]; else pLU_LU=&pNull;
	if(pgLD) pLD_LD=&pgLD->Point[xend+0*xsize]; else pLD_LD=&pNull;
	if(pgRU) pRU_RU=&pgRU->Point[yend*xsize]; else pRU_RU=&pNull;
	if(pgRD) pRD_RD=&pgRD->Point[0*xsize]; else pRD_RD=&pNull;
	if(p_LU->attribute!=POLYGRID_NOT_USE) SetPoint4(*pU_LU,*pL_LU,*pLU_LU,*p_LU);
	if(p_RU->attribute!=POLYGRID_NOT_USE) SetPoint4(*pU_RU,*pR_RU,*pRU_RU,*p_RU);
	if(p_LD->attribute!=POLYGRID_NOT_USE) SetPoint4(*pD_LD,*pL_LD,*pLD_LD,*p_LD);
	if(p_RD->attribute!=POLYGRID_NOT_USE) SetPoint4(*pD_RD,*pR_RD,*pRD_RD,*p_RD);
}
void cPolyGrid::New(int xsize,int ysize,int xstep,int ystep)
{
	if(Point) delete Point;
	Point=new sPointPolyGrid[(cPolyGrid::xsize=xsize)*(cPolyGrid::ysize=ysize)];
	cPolyGrid::xstep=xstep;	cPolyGrid::ystep=ystep;
	memset(Point,0,sizeof(sPointPolyGrid)*xsize*ysize);
}
#ifdef _DEBUG
int cPolyGrid::AssertValid()
{
	if(Point==0) return 0;
	return 1;
}
#endif
