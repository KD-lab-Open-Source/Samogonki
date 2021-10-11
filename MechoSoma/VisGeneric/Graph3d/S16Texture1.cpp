#include "Graph3d_Software.h"
#include "Software16.h"

void cGraph3dSoftware::R16_ZTW_CMT1(void *pscr,void *pzBuffer)
{
	SET_COORD_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; 
	zb_=ze_=z1; zc_=z2;	dze_=dz21_;
	long ub_=u11, vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,l_end=yeb+xee,z_=zb_;
			long u_=ub_, v_=vb_<<_xt1;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l<l_end; l++,z_+=dz_)
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=((Color4444>>8)&15)<<(8-4);
					int g=((Color4444>>4)&15)<<(8-4);
					int b=((Color4444>>0)&15)<<(8-4);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				u_+=du_,v_+=dv_;
			}
			ub_+=du31_,vb_+=dv31_;
		}
		du_=du1_,dv_=dv1_;
	}
}
void cGraph3dSoftware::L16_ZTW_CMT1(void *pscr,void *pzBuffer)
{
	SET_COORD_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long ub_=u11, vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
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
			int l=yeb+xeb,l_end=yeb+xee,z_=zb_;
			long u_=ub_, v_=vb_<<_xt1;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l>=l_end; l--,z_+=dz_)						//---
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=((Color4444>>8)&15)<<(8-4);
					int g=((Color4444>>4)&15)<<(8-4);
					int b=((Color4444>>0)&15)<<(8-4);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				u_+=du_,v_+=dv_;
			}
			ub_+=du31_,vb_+=dv31_;
		}
		du_=du1_,dv_=dv1_;
	}
}

void cGraph3dSoftware::R16_ZT_CMT1_AMT1(void *pscr,void *pzBuffer)
{
	SET_COORD_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_; 
	zb_=ze_=z1; zc_=z2;	dze_=dz21_;
	long ub_=u11, vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb,l_end=yeb+xee,z_=zb_;
			long u_=ub_, v_=vb_<<_xt1;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=((Color4444>>12)&15)<<(8-4);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((Color4444>>8)&15)<<(8-4);
								int gSrc=((Color4444>>4)&15)<<(8-4);
								int bSrc=((Color4444>>0)&15)<<(8-4);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						u_+=du_,v_+=dv_; 
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=((Color4444>>12)&15)<<(8-4);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((Color4444>>8)&15)<<(8-4);
								int gSrc=((Color4444>>4)&15)<<(8-4);
								int bSrc=((Color4444>>0)&15)<<(8-4);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						u_+=du_,v_+=dv_; 
					}
					break;
				default:
					assert(0);
			}
			ub_+=du31_,vb_+=dv31_;
		}
		du_=du1_,dv_=dv1_;
	}
}
void cGraph3dSoftware::L16_ZT_CMT1_AMT1(void *pscr,void *pzBuffer)
{
	SET_COORD_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long ub_=u11, vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
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
			int l=yeb+xeb,l_end=yeb+xee,z_=zb_;
			long u_=ub_, v_=vb_<<_xt1;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=((Color4444>>12)&15)<<(8-4);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((Color4444>>8)&15)<<(8-4);
								int gSrc=((Color4444>>4)&15)<<(8-4);
								int bSrc=((Color4444>>0)&15)<<(8-4);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						u_+=du_,v_+=dv_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=((Color4444>>12)&15)<<(8-4);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((Color4444>>8)&15)<<(8-4);
								int gSrc=((Color4444>>4)&15)<<(8-4);
								int bSrc=((Color4444>>0)&15)<<(8-4);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						u_+=du_,v_+=dv_;
					}
					break;
				default:
					assert(0);
			}
			ub_+=du31_,vb_+=dv31_;
		}
		du_=du1_,dv_=dv1_;
	}
}

void cGraph3dSoftware::R16_ZTW_CMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_, zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long ub_=u11,vb_=v11, rb_=mr1,gb_=mg1,bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l<l_end; l++,z_+=dz_)
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
					int g=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
					int b=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				u_+=du_,v_+=dv_; r_+=dr_,g_+=dg_,b_+=db_;
			}
			ub_+=du31_,vb_+=dv31_;	rb_+=dr31_,gb_+=dg31_,bb_+=db31_;	
		}
		du_=du1_,dv_=dv1_; dr_=dr1_,dg_=dg1_,db_=db1_;
	}
}
void cGraph3dSoftware::L16_ZTW_CMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long ub_=u11,vb_=v11, rb_=mr1,gb_=mg1,bb_=mb1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
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
			int l=yeb+xeb,l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l>=l_end; l--,z_+=dz_)						//---
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
					int g=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
					int b=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
				u_+=du_,v_+=dv_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;
			ub_+=du31_,vb_+=dv31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;
		du_=du1_,dv_=dv1_;
	}
}

void cGraph3dSoftware::R16_ZTW_CMDT1AS(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	long	dar31_=ar3-ar1, dag31_=ag3-ag1, dab31_=ab3-ab1, 
			dar21_=ar2-ar1, dag21_=ag2-ag1, dab21_=ab2-ab1, 
			dar32_=ar3-ar2, dag32_=ag3-ag2, dab32_=ab3-ab2,
			dar_,dag_,dab_,dar1_,dag1_,dab1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	dar1_=((dar32_*dye31-dar31_*dye32)/l);
	dag1_=((dag32_*dye31-dag31_*dye32)/l);
	dab1_=((dab32_*dye31-dab31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	dar_=((dar21_*dye31-dar31_*dye21)/l);
	dag_=((dag21_*dye31-dag31_*dye21)/l);
	dab_=((dab21_*dye31-dab31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; dar31_/=dye31; dag31_/=dye31; dab31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; dar21_/=dye21; dag21_/=dye21; dab21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; dar32_/=dye32; dag32_/=dye32; dab32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_, zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long ub_=u11,vb_=v11, rb_=mr1,gb_=mg1,bb_=mb1, arb_=ar1,agb_=ag1,abb_=ab1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		arb_+=(yScrMin-ye1)*dar31_;
		agb_+=(yScrMin-ye1)*dag31_;
		abb_+=(yScrMin-ye1)*dab31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_;
			long ar_=arb_,ag_=agb_,ab_=abb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				ar_+=(xScrMin-xeb)*dar_;
				ag_+=(xScrMin-xeb)*dag_;
				ab_+=(xScrMin-xeb)*dab_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l<l_end; l++,z_+=dz_)
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
					int g=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
					int b=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				u_+=du_,v_+=dv_; r_+=dr_,g_+=dg_,b_+=db_; ar_+=dar_,ag_+=dag_,ab_+=dab_;
			}
			ub_+=du31_,vb_+=dv31_;	rb_+=dr31_,gb_+=dg31_,bb_+=db31_; arb_+=dr31_,agb_+=dag31_,abb_+=dab31_;
		}
		du_=du1_,dv_=dv1_; dr_=dr1_,dg_=dg1_,db_=db1_; dar_=dar1_,dag_=dag1_,dab_=dab1_;
	}
}
void cGraph3dSoftware::L16_ZTW_CMDT1AS(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2,
			dr_,dg_,db_,dr1_,dg1_,db1_;
	long	dar31_=ar3-ar1, dag31_=ag3-ag1, dab31_=ab3-ab1, 
			dar21_=ar2-ar1, dag21_=ag2-ag1, dab21_=ab2-ab1, 
			dar32_=ar3-ar2, dag32_=ag3-ag2, dab32_=ab3-ab2,
			dar_,dag_,dab_,dar1_,dag1_,dab1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	dar1_=((dar32_*dye31-dar31_*dye32)/l);
	dag1_=((dag32_*dye31-dag31_*dye32)/l);
	dab1_=((dab32_*dye31-dab31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	dar_=((dar21_*dye31-dar31_*dye21)/l);
	dag_=((dag21_*dye31-dag31_*dye21)/l);
	dab_=((dab21_*dye31-dab31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; dar31_/=dye31; dag31_/=dye31; dab31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; dar21_/=dye21; dag21_/=dye21; dab21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; dar32_/=dye32; dag32_/=dye32; dab32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW);	dxee_=dxe21_;
	zb_=ze_=z1;	zc_=z2;	dze_=dz21_;
	long ub_=u11,vb_=v11, rb_=mr1,gb_=mg1,bb_=mb1, arb_=ar1,agb_=ag1,abb_=ab1;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		arb_+=(yScrMin-ye1)*dar31_;
		agb_+=(yScrMin-ye1)*dag31_;
		abb_+=(yScrMin-ye1)*dab31_;
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
			int l=yeb+xeb,l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_;
			long ar_=arb_,ag_=agb_,ab_=abb_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				ar_+=(xeb-xScrMax+1)*dar_;
				ag_+=(xeb-xScrMax+1)*dag_;
				ab_+=(xeb-xScrMax+1)*dab_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			for(;l>=l_end; l--,z_+=dz_)						//---
			{
				if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
				{
					unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
					int r=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
					int g=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
					int b=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
					scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
					zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
				}
				r_+=dr_,g_+=dg_,b_+=db_;
				ar_+=dar_,ag_+=dag_,ab_+=dab_;
				u_+=du_,v_+=dv_;
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_;
			arb_+=dar31_,agb_+=dag31_,abb_+=dab31_;
			ub_+=du31_,vb_+=dv31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_;
		dar_=dar1_,dag_=dag1_,dab_=dab1_;
		du_=du1_,dv_=dv1_;
	}
}
void cGraph3dSoftware::R16_ZT_CMDT1_AMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_, zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1,gb_=mg1,bb_=mb1,ab_=ma1;
	long ub_=u11,vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				a_+=(xScrMin-xeb)*da_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
								int gSrc=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
								int bSrc=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						u_+=du_,v_+=dv_; 
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
								int gSrc=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
								int bSrc=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						u_+=du_,v_+=dv_; 
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;	
			ub_+=du31_,vb_+=dv31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
		du_=du1_,dv_=dv1_; 
	}
}
void cGraph3dSoftware::L16_ZT_CMDT1_AMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW); dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1,gb_=mg1,bb_=mb1,ab_=ma1;
	long ub_=u11,vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
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
			int l=yeb+xeb,l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				a_+=(xeb-xScrMax+1)*da_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
								int gSrc=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
								int bSrc=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						u_+=du_,v_+=dv_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=(((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8);
								int gSrc=(((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8);
								int bSrc=(((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						u_+=du_,v_+=dv_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;
			ub_+=du31_,vb_+=dv31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
		du_=du1_,dv_=dv1_;
	}
}

void cGraph3dSoftware::R16_ZT_CMDT1AS_AMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1, dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12,dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	long	dar31_=ar3-ar1, dag31_=ag3-ag1, dab31_=ab3-ab1, daa31_=aa3-aa1, 
			dar21_=ar2-ar1, dag21_=ag2-ag1, dab21_=ab2-ab1, daa21_=aa2-aa1, 
			dar32_=ar3-ar2, dag32_=ag3-ag2, dab32_=ab3-ab2, daa32_=aa3-aa2,
			dar_,dag_,dab_,daa_,dar1_,dag1_,dab1_,daa1_;
	if((dxe21_>(xScr<<1))||(dxe32_>(xScr<<1))||(dxe31_>(xScr<<1))) return;
	l=dxe32_*dye31-dxe31_*dye32;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;	
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	dar1_=((dar32_*dye31-dar31_*dye32)/l);
	dag1_=((dag32_*dye31-dag31_*dye32)/l);
	dab1_=((dab32_*dye31-dab31_*dye32)/l);
	daa1_=((daa32_*dye31-daa31_*dye32)/l);
	l=dxe21_*dye31-dxe31_*dye21;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	dar_=((dar21_*dye31-dar31_*dye21)/l);
	dag_=((dag21_*dye31-dag31_*dye21)/l);
	dab_=((dab21_*dye31-dab31_*dye21)/l);
	daa_=((daa21_*dye31-daa31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; dar31_/=dye31; dag31_/=dye31; dab31_/=dye31; daa31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; dar21_/=dye21; dag21_/=dye21; dab21_/=dye21; daa21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; dar32_/=dye32; dag32_/=dye32; dab32_/=dye32; daa32_/=dye32; }
	xeb_=xee_=xe1<<PREC_DRAW; xec_=xe2<<PREC_DRAW; dxee_=dxe21_, zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1,gb_=mg1,bb_=mb1,ab_=ma1;
	long arb_=ar1,agb_=ag1,abb_=ab1,aab_=aa1;
	long ub_=u11,vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		arb_+=(yScrMin-ye1)*dar31_;
		agb_+=(yScrMin-ye1)*dag31_;
		abb_+=(yScrMin-ye1)*dab31_;
		aab_+=(yScrMin-ye1)*daa31_;
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
	for(;yeb<yee;dxee_=dxe32_,xee_=xec_,dze_=dz32_,ze_=zc_,yec=yee)
	{
		for(;yeb<yec; xeb_+=dxe31_,xee_+=dxee_,yeb+=xScr,zb_+=dz31_,ze_+=dze_)
		{
			long xeb=xeb_>>PREC_DRAW,xee=xee_>>PREC_DRAW;
			int l=yeb+xeb, l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			long ar_=arb_,ag_=agb_,ab_=abb_,aa_=aab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(ze_-zb_)/dxe;
			if(xeb<xScrMin)
			{
				z_+=(xScrMin-xeb)*dz_;
				u_+=(xScrMin-xeb)*du_;
				v_+=(xScrMin-xeb)*dv_;
				r_+=(xScrMin-xeb)*dr_;
				g_+=(xScrMin-xeb)*dg_;
				b_+=(xScrMin-xeb)*db_;
				a_+=(xScrMin-xeb)*da_;
				ar_+=(xScrMin-xeb)*dar_;
				ag_+=(xScrMin-xeb)*dag_;
				ab_+=(xScrMin-xeb)*dab_;
				aa_+=(xScrMin-xeb)*daa_;
				l=yeb+xScrMin;
			}
			if(xee>xScrMax) l_end=yeb+xScrMax;
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
								int gSrc=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
								int bSrc=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						ar_+=dar_,ag_+=dag_,ab_+=dab_,aa_+=daa_;
						u_+=du_,v_+=dv_; 
					}
					break;
				case TEXFMT_RGB555:
					for(;l<l_end; l++,z_+=dz_)
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
								int gSrc=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
								int bSrc=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						ar_+=dar_,ag_+=dag_,ab_+=dab_,aa_+=daa_;
						u_+=du_,v_+=dv_; 
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;	
			arb_+=dar31_,agb_+=dag31_,abb_+=dab31_,aab_+=daa31_;	
			ub_+=du31_,vb_+=dv31_;	
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
		dar_=dar1_,dag_=dag1_,dab_=dab1_,daa_=daa1_;
		du_=du1_,dv_=dv1_; 
	}
}
void cGraph3dSoftware::L16_ZT_CMDT1AS_AMDT1(void *pscr,void *pzBuffer)
{
	SET_COORD_MDRGBA_UV1_SRGBA(v1,v2,v3);
	unsigned short *scr=(unsigned short*)pscr,*zBuffer=(unsigned short*)pzBuffer,*Tex4444=(unsigned short*)texture;
	long	dye31=ye3-ye1, dye21=ye2-ye1, dye32=ye3-ye2;
	long	dxe31_=xe3-xe1,dz31_=z3-z1, dxe21_=xe2-xe1,dz21_=z2-z1,	dxe32_=xe3-xe2,dz32_=z3-z2,
			xeb_,xec_,xee_,dxee_, l,  zb_,ze_,zc_,dze_;
	long	du31_=u13-u11,dv31_=v13-v11, du21_=u12-u11,dv21_=v12-v11, du32_=u13-u12, dv32_=v13-v12,
			du_,dv_,du1_,dv1_;
	long	dr31_=mr3-mr1, dg31_=mg3-mg1, db31_=mb3-mb1, da31_=ma3-ma1, 
			dr21_=mr2-mr1, dg21_=mg2-mg1, db21_=mb2-mb1, da21_=ma2-ma1, 
			dr32_=mr3-mr2, dg32_=mg3-mg2, db32_=mb3-mb2, da32_=ma3-ma2,
			dr_,dg_,db_,da_,dr1_,dg1_,db1_,da1_;
	long	dar31_=ar3-ar1, dag31_=ag3-ag1, dab31_=ab3-ab1, daa31_=aa3-aa1, 
			dar21_=ar2-ar1, dag21_=ag2-ag1, dab21_=ab2-ab1, daa21_=aa2-aa1, 
			dar32_=ar3-ar2, dag32_=ag3-ag2, dab32_=ab3-ab2, daa32_=aa3-aa2,
			dar_,dag_,dab_,daa_,dar1_,dag1_,dab1_,daa1_;
	l=dxe31_*dye32-dxe32_*dye31;
	du1_=((du32_*dye31-du31_*dye32)/l);
	dv1_=((dv32_*dye31-dv31_*dye32)/l)<<_xt1;
	dr1_=((dr32_*dye31-dr31_*dye32)/l);
	dg1_=((dg32_*dye31-dg31_*dye32)/l);
	db1_=((db32_*dye31-db31_*dye32)/l);
	da1_=((da32_*dye31-da31_*dye32)/l);
	dar1_=((dar32_*dye31-dar31_*dye32)/l);
	dag1_=((dag32_*dye31-dag31_*dye32)/l);
	dab1_=((dab32_*dye31-dab31_*dye32)/l);
	daa1_=((daa32_*dye31-daa31_*dye32)/l);
	l=dxe31_*dye21-dxe21_*dye31;
	du_=((du21_*dye31-du31_*dye21)/l);
	dv_=((dv21_*dye31-dv31_*dye21)/l)<<_xt1;	
	dr_=((dr21_*dye31-dr31_*dye21)/l);
	dg_=((dg21_*dye31-dg31_*dye21)/l);
	db_=((db21_*dye31-db31_*dye21)/l);
	da_=((da21_*dye31-da31_*dye21)/l);
	dar_=((dar21_*dye31-dar31_*dye21)/l);
	dag_=((dag21_*dye31-dag31_*dye21)/l);
	dab_=((dab21_*dye31-dab31_*dye21)/l);
	daa_=((daa21_*dye31-daa31_*dye21)/l);
	if(dye31) { dxe31_=(dxe31_<<PREC_DRAW)/dye31, dz31_/=dye31; du31_/=dye31, dv31_/=dye31; dr31_/=dye31; dg31_/=dye31; db31_/=dye31; da31_/=dye31; dar31_/=dye31; dag31_/=dye31; dab31_/=dye31; daa31_/=dye31; }
	if(dye21) { dxe21_=(dxe21_<<PREC_DRAW)/dye21, dz21_/=dye21; du21_/=dye21, dv21_/=dye21; dr21_/=dye21; dg21_/=dye21; db21_/=dye21; da21_/=dye21; dar21_/=dye21; dag21_/=dye21; dab21_/=dye21; daa21_/=dye21; }
	if(dye32) { dxe32_=(dxe32_<<PREC_DRAW)/dye32, dz32_/=dye32; du32_/=dye32, dv32_/=dye32; dr32_/=dye32; dg32_/=dye32; db32_/=dye32; da32_/=dye32; dar32_/=dye32; dag32_/=dye32; dab32_/=dye32; daa32_/=dye32; }
	xeb_=xee_=(xe1<<PREC_DRAW);	xec_=(xe2<<PREC_DRAW); dxee_=dxe21_; zb_=ze_=z1; zc_=z2; dze_=dz21_;
	long rb_=mr1,gb_=mg1,bb_=mb1,ab_=ma1;
	long arb_=ar1,agb_=ag1,abb_=ab1,aab_=aa1;
	long ub_=u11,vb_=v11;
	unsigned int yeb,yec,yee;
	if(ye1<yScrMin) { 
		xeb_+=(yScrMin-ye1)*dxe31_;
		xee_+=(yScrMin-ye1)*dxee_;
		zb_+=(yScrMin-ye1)*dz31_;
		ze_+=(yScrMin-ye1)*dze_;
		ub_+=(yScrMin-ye1)*du31_;
		vb_+=(yScrMin-ye1)*dv31_;
		rb_+=(yScrMin-ye1)*dr31_;
		gb_+=(yScrMin-ye1)*dg31_;
		bb_+=(yScrMin-ye1)*db31_;
		ab_+=(yScrMin-ye1)*da31_;
		arb_+=(yScrMin-ye1)*dar31_;
		agb_+=(yScrMin-ye1)*dag31_;
		abb_+=(yScrMin-ye1)*dab31_;
		aab_+=(yScrMin-ye1)*daa31_;
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
			int l=yeb+xeb,l_end=yeb+xee, z_=zb_;
			long u_=ub_,v_=vb_<<_xt1;
			long r_=rb_,g_=gb_,b_=bb_,a_=ab_;
			long ar_=arb_,ag_=agb_,ab_=abb_,aa_=aab_;
			int dxe=xee-xeb; if(dxe==0) dxe=1;
			int dz_=(zb_-ze_)/dxe;
			if(xeb>(xScrMax-1))						//---
			{
				z_+=(xeb-xScrMax+1)*dz_;			//---
				u_+=(xeb-xScrMax+1)*du_;			//---
				v_+=(xeb-xScrMax+1)*dv_;			//---
				r_+=(xeb-xScrMax+1)*dr_;
				g_+=(xeb-xScrMax+1)*dg_;
				b_+=(xeb-xScrMax+1)*db_;
				a_+=(xeb-xScrMax+1)*da_;
				ar_+=(xeb-xScrMax+1)*dar_;
				ag_+=(xeb-xScrMax+1)*dag_;
				ab_+=(xeb-xScrMax+1)*dab_;
				aa_+=(xeb-xScrMax+1)*daa_;
				l=yeb+(xScrMax-1);				//---
			}
			if(xee<xScrMin) l_end=yeb+xScrMin;	//---
			u_&=xmt1+((1<<PREC_DRAW)-1);

			switch(ScrTextureFormat)
			{
				case TEXFMT_RGB565:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
								int gSrc=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
								int bSrc=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<11))>>8;
								int gDest=(ColorDest&(63<<5))>>3;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						ar_+=dar_,ag_+=dag_,ab_+=dab_,aa_+=daa_;
						u_+=du_,v_+=dv_;
					}
					break;
				case TEXFMT_RGB555:
					for(;l>=l_end; l--,z_+=dz_)						//---
					{
						if((unsigned short)(z_>>PREC_DRAW)<=zBuffer[l])
						{
							unsigned short Color4444=Tex4444[((v_&ymt1)+(u_&xmt1))>>PREC_DRAW];
							int Alfa=(((Color4444>>12)&15)*a_)>>(PREC_DRAW-8+4+8);
							if(Alfa>=AlfaRef)
							{
								int rSrc=((((Color4444>>8)&15)*r_)>>(PREC_DRAW-8+4+8))+(ar_>>PREC_DRAW);
								int gSrc=((((Color4444>>4)&15)*g_)>>(PREC_DRAW-8+4+8))+(ag_>>PREC_DRAW);
								int bSrc=((((Color4444>>0)&15)*b_)>>(PREC_DRAW-8+4+8))+(ab_>>PREC_DRAW);
								int ColorDest=scr[l];
								int rDest=(ColorDest&(31<<10))>>7;
								int gDest=(ColorDest&(63<<5))>>2;
								int bDest=(ColorDest&(31<<0))<<3;
								int r=rDest+((Alfa*(rSrc-rDest))>>8);
								int g=gDest+((Alfa*(gSrc-gDest))>>8);
								int b=bDest+((Alfa*(bSrc-bDest))>>8);
								scr[l]=rPal16[r]+gPal16[g]+bPal16[b];
								if(RenderState_zwrite)
									zBuffer[l]=(unsigned short)(z_>>PREC_DRAW);
							}
						}
						r_+=dr_,g_+=dg_,b_+=db_,a_+=da_;
						ar_+=dar_,ag_+=dag_,ab_+=dab_,aa_+=daa_;
						u_+=du_,v_+=dv_;
					}
					break;
				default:
					assert(0);
			}
			rb_+=dr31_,gb_+=dg31_,bb_+=db31_,ab_+=da31_;
			arb_+=dar31_,agb_+=dag31_,abb_+=dab31_,aab_+=daa31_;
			ub_+=du31_,vb_+=dv31_;
		}
		dr_=dr1_,dg_=dg1_,db_=db1_,da_=da1_;
		dar_=dar1_,dag_=dag1_,dab_=dab1_,daa_=daa1_;
		du_=du1_,dv_=dv1_;
	}
}