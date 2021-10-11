#include "Graph3d_Software.h"
#include "Software16.h"

void cGraph3dSoftware::R16_ZTW_CMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe32_*dye31-dxe31_*dye32;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) 
		yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) 
		ye3=yScrMin;
	if(ye3>=yScrMax) 
		yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			int xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			int r_=rb_, g_=gb_, b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			for(;l<l_end; z_+=dz_,l++)
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					scr[l]=rPal16[r_>>PREC_DRAW]+gPal16[g_>>PREC_DRAW]+bPal16[b_>>PREC_DRAW];;
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;		
	}
}
void cGraph3dSoftware::L16_ZTW_CMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe31_*dye32-dxe32_*dye31;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	zb_=ze_=z1;		zc_=z2;		dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) 
		yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) 
		ye3=yScrMin;
	if(ye3>=yScrMax) 
		yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long r_=rb_, g_=gb_, b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			for(;l>=l_end; z_+=dz_,l--)						//---
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					scr[l]=rPal16[r_>>PREC_DRAW]+gPal16[g_>>PREC_DRAW]+bPal16[b_>>PREC_DRAW];;
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;
	}
}

void cGraph3dSoftware::R16_ZTW_CMDAS(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1+(ar3-ar1), dg31_=mg3-mg1+(ag3-ag1), db31_=mb3-mb1+(ab3-ab1), 
			dr21_=mr2-mr1+(ar2-ar1), dg21_=mg2-mg1+(ag2-ag1), db21_=mb2-mb1+(ab2-ab1), 
			dr32_=mr3-mr2+(ar3-ar2), dg32_=mg3-mg2+(ag3-ag2), db32_=mb3-mb2+(ab3-ab2),
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe32_*dye31-dxe31_*dye32;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long rb_=mr1+ar1, gb_=mg1+ag1, bb_=mb1+ab1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) 
		yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) 
		ye3=yScrMin;
	if(ye3>=yScrMax) 
		yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			int xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			int r_=rb_, g_=gb_, b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			for(;l<l_end; z_+=dz_,l++)
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned int aa=rPal16[r_>>PREC_DRAW]+gPal16[g_>>PREC_DRAW]+bPal16[b_>>PREC_DRAW];;
					assert(aa<=65535);
					scr[l]=aa;
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;		
	}
}
void cGraph3dSoftware::L16_ZTW_CMDAS(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1+(ar3-ar1), dg31_=mg3-mg1+(ag3-ag1), db31_=mb3-mb1+(ab3-ab1), 
			dr21_=mr2-mr1+(ar2-ar1), dg21_=mg2-mg1+(ag2-ag1), db21_=mb2-mb1+(ab2-ab1), 
			dr32_=mr3-mr2+(ar3-ar2), dg32_=mg3-mg2+(ag3-ag2), db32_=mb3-mb2+(ab3-ab2),
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe31_*dye32-dxe32_*dye31;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW;	xec_=xe2<<PREC_DRAW;	dxee_=dxe21_; 
	zb_=ze_=z1;		zc_=z2;		dze_=dz21_;
	long rb_=mr1+ar1, gb_=mg1+ag1, bb_=mb1+ab1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) 
		yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) 
		ye3=yScrMin;
	if(ye3>=yScrMax) 
		yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long r_=rb_, g_=gb_, b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			for(;l>=l_end; z_+=dz_,l--)						//---
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					scr[l]=rPal16[r_>>PREC_DRAW]+gPal16[g_>>PREC_DRAW]+bPal16[b_>>PREC_DRAW];;
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;
	}
}

void cGraph3dSoftware::R16_ZT_CMD_AMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	l=dxe32_*dye31-dxe31_*dye32;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1, ab_=ma1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			int xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			int r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				a_+=(xScrMin-xeb)*da_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;		
	}
}
void cGraph3dSoftware::L16_ZT_CMD_AMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	l=dxe31_*dye32-dxe32_*dye31;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1, ab_=ma1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				a_+=(xeb-xScrMax+1)*da_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
	}
}
void cGraph3dSoftware::R16_ZT_CAD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe32_*dye31-dxe31_*dye32;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			int xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			int r_=rb_,g_=gb_,b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+(r_>>PREC_DRAW);
							int g=gScr+(g_>>PREC_DRAW);
							int b=bScr+(b_>>PREC_DRAW);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+(r_>>PREC_DRAW);
							int g=gScr+(g_>>PREC_DRAW);
							int b=bScr+(b_>>PREC_DRAW);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;		
	}
}
void cGraph3dSoftware::L16_ZT_CAD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1,  
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, 
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe31_*dye32-dxe32_*dye31;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long r_=rb_,g_=gb_,b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+(r_>>PREC_DRAW);
							int g=gScr+(g_>>PREC_DRAW);
							int b=bScr+(b_>>PREC_DRAW);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+(r_>>PREC_DRAW);
							int g=gScr+(g_>>PREC_DRAW);
							int b=bScr+(b_>>PREC_DRAW);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;
	}
}

void cGraph3dSoftware::R16_ZT_CMDAS_AMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1+(ar3-ar1), dg31_=mg3-mg1+(ag3-ag1), db31_=mb3-mb1+(ab3-ab1), da31_=ma3-ma1, 
			dr21_=mr2-mr1+(ar2-ar1), dg21_=mg2-mg1+(ag2-ag1), db21_=mb2-mb1+(ab2-ab1), da21_=ma2-ma1, 
			dr32_=mr3-mr2+(ar3-ar2), dg32_=mg3-mg2+(ag3-ag2), db32_=mb3-mb2+(ab3-ab2), da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	l=dxe32_*dye31-dxe31_*dye32;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1, ab_=ma1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			int xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			int r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				a_+=(xScrMin-xeb)*da_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; z_+=dz_,l++)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;		
	}
}
void cGraph3dSoftware::L16_ZT_CMDAS_AMD(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,	dz31_=z3-z1, dxe21_=xe2-xe1, dz21_=z2-z1, dxe32_=xe3-xe2, dz32_=z3-z2,
			xeb_,xec_,xee_, dxee_,l, zb_,ze_,zc_,dze_;
	long	dr31_=mr3-mr1+(ar3-ar1), dg31_=mg3-mg1+(ag3-ag1), db31_=mb3-mb1+(ab3-ab1), da31_=ma3-ma1, 
			dr21_=mr2-mr1+(ar2-ar1), dg21_=mg2-mg1+(ag2-ag1), db21_=mb2-mb1+(ab2-ab1), da21_=ma2-ma1, 
			dr32_=mr3-mr2+(ar3-ar2), dg32_=mg3-mg2+(ag3-ag2), db32_=mb3-mb2+(ab3-ab2), da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	l=dxe31_*dye32-dxe32_*dye31;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1, gb_=mg1, bb_=mb1, ab_=ma1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		zb_+=(yScrMin-ye1)*dz31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		xee_+=(yScrMin-ye1)*dxee_;
		ze_+=(yScrMin-ye1)*dze_;
		yeb=yScrMin*xScr;
		ye1=yScrMin;
	}	else yeb=ye1*xScr;
	if(ye2<yScrMin) {
		xec_+=(yScrMin-ye2)*dxe32_;
		zc_+=(yScrMin-ye2)*dz32_;
		yec=yScrMin*xScr;
	}	else yec=ye2*xScr;
	if(ye2>=yScrMax) yec=(yScrMax-1)*xScr;
	if(ye3<yScrMin) ye3=yScrMin;
	if(ye3>=yScrMax) yee=(yScrMax-1)*xScr;
	else yee=ye3*xScr;
	for(;yeb<yee; dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,zb_+=dz31_,ze_+=dze_,yeb+=xScr)
		{
			long xeb=(xeb_>>PREC_DRAW)-1,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				a_+=(xeb-xScrMax+1)*da_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<11))>>8;
							int gScr=(ColorScr&(63<<5))>>3;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; z_+=dz_,l--)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							int Alfa=a_>>PREC_DRAW;
							int ColorScr=scr[l];
							int rScr=(ColorScr&(31<<10))>>7;
							int gScr=(ColorScr&(31<<5))>>2;
							int bScr=(ColorScr&(31<<0))<<3;
							int r=rScr+((Alfa*((r_>>PREC_DRAW)-rScr))>>8);
							int g=gScr+((Alfa*((g_>>PREC_DRAW)-gScr))>>8);
							int b=bScr+((Alfa*((b_>>PREC_DRAW)-bScr))>>8);
							scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
							if(RenderState_zwrite)
								zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
	}
}
