#include "DrawPolygonShade.h"
#include "Maths.h"
#include "IncTerra.h"
#include "object3d.h"
#include "PolyMgr.h"
#include <fstream>

#define KOOF_WATER_ATTRIBUTE		0.3f

#define SET_COORD_SHADE(p1,p2,p3)														\
	{																					\
		xe1=round(p1.xe); ye1=round(p1.ye); w1=round(p1.z)<<PREC_DRAW;				\
		xe2=round(p2.xe); ye2=round(p2.ye); w2=round(p2.z)<<PREC_DRAW;				\
		xe3=round(p3.xe); ye3=round(p3.ye); w3=round(p3.z)<<PREC_DRAW;				\
	}
#define SET_COORD_SHADE_MASK(p1,p2,p3)													\
	{																					\
		SET_COORD_SHADE(p1,p2,p3);														\
		u1=round(p1.u1()*(1<<(_xt+PREC_DRAW))); v1=round(p1.v1()*(1<<(_yt+PREC_DRAW)));	\
		u2=round(p2.u1()*(1<<(_xt+PREC_DRAW))); v2=round(p2.v1()*(1<<(_yt+PREC_DRAW)));	\
		u3=round(p3.u1()*(1<<(_xt+PREC_DRAW))); v3=round(p3.v1()*(1<<(_yt+PREC_DRAW)));	\
	}

void sTracePolygon::PointShade(int xe,int ye,int w,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax)
{
	if(ye<yScrMin||ye>=yScrMax||xe<xScrMin||xe>=xScrMax) return;
	int l=xe+ye*(xScrMax-xScrMin);
	if((short)w>scr[l])
		scr[l]=w;
}
void sTracePolygon::PolygonRightShade(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax) // for x>0 and y>0, right
{
	int xScr=xScrMax-xScrMin;
	long	xeb_,xec_,xee_,dxee_;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1, dw31_=w3-w1, dxe21_=xe2-xe1, dw21_=w2-w1, dxe32_=xe3-xe2, dw32_=w3-w2,
			wb_,we_,wc_,dwe_;
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dw31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dw21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dw32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	wb_=we_=w1;	wc_=w2;	dwe_=dw21_;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		wb_+=(yScrMin-ye1)*dw31_;
		xee_+=(yScrMin-ye1)*dxee_;
		we_+=(yScrMin-ye1)*dwe_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		wc_+=(yScrMin-ye2)*dw32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,	l_end=yeb+xee;
			long w_=wb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dw_=(we_-wb_)/dxe;
			if(xeb<xScrMin)
			{
				w_+=(xScrMin-xeb)*dw_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			while(l<l_end)
			{
				if((short)(w_>>PREC_DRAW)>scr[l])
					scr[l]=(short)(w_>>PREC_DRAW);
				w_+=dw_;	l++;
			}

			xeb_+=dxe31_;	xee_+=dxee_;	
			wb_+=dw31_;		we_+=dwe_;	
			yeb+=xScr;		
		}
		dxee_=dxe32_;	xee_=xec_;
		dwe_=dw32_;		we_=wc_;
		yec=yee;	
	}
}
void sTracePolygon::PolygonLeftShade(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax) // for x>0 and y>0, left
{
	int xScr=xScrMax-xScrMin;
	long	xeb_,xec_,xee_,dxee_;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1, dw31_=w3-w1, dxe21_=xe2-xe1, dw21_=w2-w1, dxe32_=xe3-xe2, dw32_=w3-w2,	wb_,we_,wc_,dwe_;
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dw31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dw21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dw32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	wb_=we_=w1;	wc_=w2;	dwe_=dw21_;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		wb_+=(yScrMin-ye1)*dw31_;
		xee_+=(yScrMin-ye1)*dxee_;
		we_+=(yScrMin-ye1)*dwe_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		wc_+=(yScrMin-ye2)*dw32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,	l_end=yeb+xee, w_=wb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dw_=(wb_-we_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				w_+=(xeb-xScrMax+1)*dw_;			//---
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			while(l>=l_end)						//---
			{
				if((short)(w_>>PREC_DRAW)>scr[l])
					scr[l]=(short)(w_>>PREC_DRAW);
				w_+=dw_;	l--;
			}

			xeb_+=dxe31_;	xee_+=dxee_;
			wb_+=dw31_;		we_+=dwe_;
			yeb+=xScr;		
		}
		dxee_=dxe32_;	xee_=xec_;
		dwe_=dw32_;		we_=wc_;
		yec=yee;	
	}
}

void sTracePolygon::PolygonRightShadeMask(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax) // for x>0 and y>0, right
{
	int xScr=xScrMax-xScrMin;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	du31_=u3-u1, dv31_=v3-v1, dw31_=w3-w1,
			dxe21_=xe2-xe1, du21_=u2-u1, dv21_=v2-v1, dw21_=w2-w1,
			dxe32_=xe3-xe2, du32_=u3-u2, dv32_=v3-v2, dw32_=w3-w2,
			du_,dv_,du1_,dv1_,l, wb_,we_,wc_,dwe_;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt;	
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt;
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, du31_/=dye31, dv31_/=dye31; dw31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, du21_/=dye21, dv21_/=dye21; dw21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, du32_/=dye32, dv32_/=dye32; dw32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	wb_=we_=w1;		wc_=w2;		dwe_=dw21_;
	long ub_=u1, vb_=v1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		wb_+=(yScrMin-ye1)*dw31_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		xee_+=(yScrMin-ye1)*dxee_;
		we_+=(yScrMin-ye1)*dwe_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		wc_+=(yScrMin-ye2)*dw32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,	l_end=yeb+xee;
			long u_=ub_, v_=vb_<<_xt, w_=wb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dw_=(we_-wb_)/dxe;
			if(xeb<xScrMin)
			{
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				w_+=(xScrMin-xeb)*dw_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xtm+((1<<PREC_DRAW)-1);

			while(l<l_end)
			{
				if((short)(w_>>PREC_DRAW)>scr[l])
				{
					if(t2[((v_&ytm)+(u_&xtm))>>PREC_DRAW]!=0)
						scr[l]=(short)(w_>>PREC_DRAW);
				}
				u_+=du_;	v_+=dv_;	w_+=dw_;	l++;
			}

			xeb_+=dxe31_;	xee_+=dxee_;	
			wb_+=dw31_;		we_+=dwe_;	
			ub_+=du31_;		vb_+=dv31_;		
			yeb+=xScr;		
		}
		dxee_=dxe32_;	xee_=xec_;
		dwe_=dw32_;		we_=wc_;
		du_=du1_;		dv_=dv1_;		
		yec=yee;	
	}
}
void sTracePolygon::PolygonLeftShadeMask(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax) // for x>0 and y>0, left
{
	int xScr=xScrMax-xScrMin;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1, du31_=u3-u1, dv31_=v3-v1, dw31_=w3-w1,
			dxe21_=xe2-xe1, du21_=u2-u1, dv21_=v2-v1, dw21_=w2-w1,
 			dxe32_=xe3-xe2, du32_=u3-u2, dv32_=v3-v2, dw32_=w3-w2,
			du_,dv_,du1_,dv1_,l, wb_,we_,wc_,dwe_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt;	
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt;	
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, du31_/=dye31, dv31_/=dye31; dw31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, du21_/=dye21, dv21_/=dye21; dw21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, du32_/=dye32, dv32_/=dye32; dw32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	wb_=we_=w1;	wc_=w2;	dwe_=dw21_;
	long ub_=u1, vb_=v1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		wb_+=(yScrMin-ye1)*dw31_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		xee_+=(yScrMin-ye1)*dxee_;
		we_+=(yScrMin-ye1)*dwe_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		wc_+=(yScrMin-ye2)*dw32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,	l_end=yeb+xee;
			long u_=ub_, v_=vb_<<_xt, w_=wb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dw_=(wb_-we_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				w_+=(xeb-xScrMax+1)*dw_;			//---
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xtm+((1<<PREC_DRAW)-1);

			while(l>=l_end)						//---
			{
				if((short)(w_>>PREC_DRAW)>scr[l])
				{
					if(t2[((v_&ytm)+(u_&xtm))>>PREC_DRAW]!=0)
						scr[l]=(short)(w_>>PREC_DRAW);
				}
				u_+=du_; v_+=dv_; w_+=dw_; l--;
			}

			xeb_+=dxe31_;	xee_+=dxee_;
			wb_+=dw31_;		we_+=dwe_;
			ub_+=du31_;		vb_+=dv31_;
			yeb+=xScr;		
		}
		dxee_=dxe32_;	xee_=xec_;
		dwe_=dw32_;		we_=wc_;
		du_=du1_;	dv_=dv1_;
		yec=yee;	
	}
}

void sTracePolygon::DrawPolygonShade(sVertexFix &p1,sVertexFix &p2,sVertexFix &p3,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax)
{
	assert(xScrMin<=xScrMax&&yScrMin<=yScrMax);
	int fl=TestVisiblePolygonShadeMask(p1,p2,p3);
	if(fl<-100) return;
	if(fl<0)
	{ 
		DrawPolygonShade(p1,p3,p2,scr,xScrMin,yScrMin,xScrMax,yScrMax);
		return; 
	}
	if(fl==0) return;
	if (ye1<=ye2)
	{
		if(ye2<=ye3)		{ SET_COORD_SHADE(p1,p2,p3); PolygonLeftShade(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
		else if(ye1<=ye3)	{ SET_COORD_SHADE(p1,p3,p2); PolygonRightShade(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
		else 				{ SET_COORD_SHADE(p3,p1,p2); PolygonLeftShade(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
	}
	else
	{
		if(ye1<=ye3)		{ SET_COORD_SHADE(p2,p1,p3); PolygonRightShade(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
		else if(ye2<=ye3)	{ SET_COORD_SHADE(p2,p3,p1); PolygonLeftShade(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
		else				{ SET_COORD_SHADE(p3,p2,p1); PolygonRightShade(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
	}
}
void sTracePolygon::DrawPolygonShadeMask(sVertexFix &p1,sVertexFix &p2,sVertexFix &p3,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax)
{
	assert(xScrMin<=xScrMax&&yScrMin<=yScrMax);
	int fl=TestVisiblePolygonShadeMask(p1,p2,p3);
	if(fl<0) { DrawPolygonShadeMask(p1,p3,p2,scr,xScrMin,yScrMin,xScrMax,yScrMax); return; }
	else if(fl==0) return;
	if (ye1<=ye2)
	{
		if(ye2<=ye3)		{ SET_COORD_SHADE_MASK(p1,p2,p3); PolygonLeftShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
		else if(ye1<=ye3)	{ SET_COORD_SHADE_MASK(p1,p3,p2); PolygonRightShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
		else 				{ SET_COORD_SHADE_MASK(p3,p1,p2); PolygonLeftShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
	}
	else
	{
		if(ye1<=ye3)		{ SET_COORD_SHADE_MASK(p2,p1,p3); PolygonRightShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
		else if(ye2<=ye3)	{ SET_COORD_SHADE_MASK(p2,p3,p1); PolygonLeftShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax); }	//+
		else				{ SET_COORD_SHADE_MASK(p3,p2,p1); PolygonRightShadeMask(scr,xScrMin,yScrMin,xScrMax,yScrMax);}	//-
	}
}

void sTracePolygon::PolygonRightMapAttribute()
{
#ifdef _USE_TILEMAP_
	long	xeb_,xec_,xee_,dxee_;
	long	dye31=ye3-ye1, 
			dye21=ye2-ye1, 
			dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	
			dxe21_=xe2-xe1, 
			dxe32_=xe3-xe2;
	if(dye31) dxe31_=(dxe31_<<PREC_DRAW)/dye31;
	if(dye21) dxe21_=(dxe21_<<PREC_DRAW)/dye21;
	if(dye32) dxe32_=(dxe32_<<PREC_DRAW)/dye32;
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	int yeb=ye1, yec=ye2, yee=ye3;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			unsigned int y_cycl=TERRA_YCYCL(yeb);
			for(int xe=xeb;xe<xee;xe++)
			{
				CollectAttribute(vMap_GetAt(TERRA_XCYCL(xe),y_cycl));
				FullAttribute++;
			}
			xeb_+=dxe31_;	xee_+=dxee_;	
			yeb++;		
		}
		dxee_=dxe32_;	xee_=xec_;
		yec=yee;	
	}
#endif //_USE_TILEMAP_
}
void sTracePolygon::PolygonLeftMapAttribute()
{
#ifdef _USE_TILEMAP_
	long	xeb_,xec_,xee_,dxee_;
	long	dye31=ye3-ye1, 
			dye21=ye2-ye1, 
			dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	
			dxe21_=xe2-xe1, 
			dxe32_=xe3-xe2;
	if(dye31) dxe31_=(dxe31_<<PREC_DRAW)/dye31;
	if(dye21) dxe21_=(dxe21_<<PREC_DRAW)/dye21;
	if(dye32) dxe32_=(dxe32_<<PREC_DRAW)/dye32;
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	int yeb=ye1, yec=ye2, yee=ye3;
	while(yeb<yee)
	{
		while(yeb<yec)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			unsigned int y_cycl=TERRA_YCYCL(yeb);
			for(int xe=xeb;xe>=xee;xe--)
			{
				CollectAttribute(vMap_GetAt(TERRA_XCYCL(xe),y_cycl));
				FullAttribute++;
			}
			xeb_+=dxe31_;	xee_+=dxee_;
			yeb++;		
		}
		dxee_=dxe32_;	xee_=xec_;
		yec=yee;	
	}
#endif //_USE_TILEMAP_
}
int sTracePolygon::GetAttribute(int xe1,int ye1,int xe2,int ye2,int xe3,int ye3)
{
	FullAttribute=AttributeBit4=AttributeBit5=AttributeBit6=AttributeBit7=0;
	AttributeTr0=AttributeTr1=AttributeTr2=AttributeTr3=AttributeTr4=AttributeTr5=AttributeTr6=AttributeTr7=0;
	AttributeTr8=AttributeTr9=AttributeTr10=AttributeTr11=AttributeTr12=AttributeTr13=AttributeTr14=AttributeTr15=0;
	if(((xe3-xe2)*(ye3-ye1)-(xe3-xe1)*(ye3-ye2))<0) return GetAttribute(xe1,ye1,xe3,ye3,xe2,ye2);
	if (ye1<=ye2)
	{
		if(ye2<=ye3)		{ SetCoordAttribute(xe1,ye1,xe2,ye2,xe3,ye3); PolygonLeftMapAttribute(); }	//+
		else if(ye1<=ye3)	{ SetCoordAttribute(xe1,ye1,xe3,ye3,xe2,ye2); PolygonRightMapAttribute();}	//-
		else 				{ SetCoordAttribute(xe3,ye3,xe1,ye1,xe2,ye2); PolygonLeftMapAttribute(); }	//+
	}
	else 
	{
		if(ye1<=ye3)		{ SetCoordAttribute(xe2,ye2,xe1,ye1,xe3,ye3); PolygonRightMapAttribute();}	//-
		else if(ye2<=ye3)	{ SetCoordAttribute(xe2,ye2,xe3,ye3,xe1,ye1); PolygonLeftMapAttribute(); }	//+
		else				{ SetCoordAttribute(xe3,ye3,xe2,ye2,xe1,ye1); PolygonRightMapAttribute();}	//-
	}
	if(FullAttribute==0)
	{
		CollectAttribute(vMap_GetAt(TERRA_XCYCL(xe1),TERRA_YCYCL(ye1)));
		CollectAttribute(vMap_GetAt(TERRA_XCYCL(xe2),TERRA_YCYCL(ye2)));
		CollectAttribute(vMap_GetAt(TERRA_XCYCL(xe3),TERRA_YCYCL(ye3)));
		CollectAttribute(vMap_GetAt(TERRA_XCYCL((xe1+xe2+xe3)/3),TERRA_YCYCL((ye1+ye2+ye3)/3)));
		FullAttribute=4;
	}
	FullAttribute*=KOOF_WATER_ATTRIBUTE;
	int TrMax=AttributeTr0,attr=0;
	if(TrMax<AttributeTr1) { TrMax=AttributeTr1; attr=1; }
	if(TrMax<AttributeTr2) { TrMax=AttributeTr2; attr=2; }
	if(TrMax<AttributeTr3) { TrMax=AttributeTr3; attr=3; }
	if(TrMax<AttributeTr4) { TrMax=AttributeTr4; attr=4; }
	if(TrMax<AttributeTr5) { TrMax=AttributeTr5; attr=5; }
	if(TrMax<AttributeTr6) { TrMax=AttributeTr6; attr=6; }
	if(TrMax<AttributeTr7) { TrMax=AttributeTr7; attr=7; }
	if(TrMax<AttributeTr8) { TrMax=AttributeTr8; attr=8; }
	if(TrMax<AttributeTr9) { TrMax=AttributeTr9; attr=9; }
	if(TrMax<AttributeTr10) { TrMax=AttributeTr10; attr=10; }
	if(TrMax<AttributeTr11) { TrMax=AttributeTr11; attr=11; }
	if(TrMax<AttributeTr12) { TrMax=AttributeTr12; attr=12; }
	if(TrMax<AttributeTr13) { TrMax=AttributeTr13; attr=13; }
	if(TrMax<AttributeTr14) { TrMax=AttributeTr14; attr=14; }
	if(TrMax<AttributeTr15) { TrMax=AttributeTr15; attr=15; }
	if(FullAttribute<AttributeBit4)	attr|=(1<<4);
	if(FullAttribute<AttributeBit5)	attr|=(1<<5);
	if(FullAttribute<AttributeBit6)	attr|=(1<<6);
	if(FullAttribute<AttributeBit7)	attr|=(1<<7);
	return attr;
}
void sTracePolygon::SaveAttribute(char *fname)
{
	fstream f(fname,ios::out);
	for(int j=0;j<2048;j+=2)
		for(int i=0;i<2048;i+=2)
		{
			unsigned char attr=vMap_GetAt(i,j);
			if(attr&(1<<4))	AttributeBit4++;
			if(attr&(1<<5))	AttributeBit5++;
			if(attr&(1<<6))	AttributeBit6++;
			if(attr&(1<<7))	AttributeBit7++;
			switch(attr&15)	
			{
				case 0:	attr=0;	break;
				case 1:	attr=1;	break;
				case 2:	attr=2;	break;
				case 3:	attr=3;	break;
				case 4:	attr=4;	break;
				case 5:	attr=5;	break;
				case 6:	attr=6;	break;
				case 7:	attr=7;	break;
				case 8:	attr=8;	break;
				case 9:	attr=9;	break;
				case 10: attr=10; break;
				case 11: attr=11; break;
				case 12: attr=12; break;
				case 13: attr=13; break;
				case 14: attr=14; break;
				case 15: attr=15; break;
			}
			attr<<=4;
			f.write((char*)&attr,1);
		}
	f.close();
}
void sTracePolygon::SaveVoxel(char *fname)
{
	fstream f(fname,ios::out);
	for(int j=0;j<2048;j+=2)
		for(int i=0;i<2048;i+=2)
		{
			unsigned char voxel=vMap_GetAlt(i,j);
			f.write((char *) &voxel,1);
		}
	f.close();
}
