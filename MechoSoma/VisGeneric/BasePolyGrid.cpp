#include "BasePolyGrid.h"
#include "BaseDefine.h"
//#include "BaseReflection.h"

void cBasePolyGrid::New(int xSize,int ySize,float xStep,float yStep)
{
	delete[] pColor;
	delete[] pUVWarp;
	delete[] pXYZWarp;

        if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP)) {
          delete[] reinterpret_cast<sBaseWarpTime *>(pStructWarp);
        }
        if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP)) {
          delete[] reinterpret_cast<sBaseWarpWave *>(pStructWarp);
        }

	xsize=xSize; ysize=ySize;
	xstep=xStep; ystep=yStep;
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))
		NEW_MEM(pColor,sBaseColor4c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_UVWARP))
		NEW_MEM(pUVWarp,sBasePoint2c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP))
		NEW_MEM(pXYZWarp,sBasePoint3c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP))
		NEW_MEM(pStructWarp,sBaseWarpTime,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP))
		NEW_MEM(pStructWarp,sBaseWarpWave,xsize*ysize);
}
void cBasePolyGrid::SetPosition(float x,float y,float z)
{
	Matrix.trans().set(x,y,z);
}
inline void SetPoint2(sBaseColor4c &pl,sBaseColor4c &pr)
{
	pl.r=pr.r=(pl.r+pr.r)>>1; 
	pl.g=pr.g=(pl.g+pr.g)>>1; 
	pl.b=pr.b=(pl.b+pr.b)>>1; 
	pl.a=pr.a=(pl.a+pr.a)>>1; 
}
inline void SetPoint4(sBaseColor4c &pl,sBaseColor4c &pr,sBaseColor4c &pu,sBaseColor4c &pd)
{
	pl.r=pr.r=pu.r=pd.r=(pl.r+pr.r+pu.r+pd.r)>>2; 
	pl.g=pr.g=pu.g=pd.g=(pl.g+pr.g+pu.g+pd.g)>>2; 
	pl.b=pr.b=pu.b=pd.b=(pl.b+pr.b+pu.b+pd.b)>>2; 
	pl.a=pr.a=pu.a=pd.a=(pl.a+pr.a+pu.a+pd.a)>>2; 
}
/*
int cBasePolyGrid::Draw(cCamera *Camera)
{
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_MULTIMATERIAL))
		return 1;
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_SPECIALCYCL))
		return DrawSpecialCycl(Camera,MaterialArray(0));
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_CYCL))
		return DrawCycl(Camera,0,0,xsize,ysize,MaterialArray[0]);
	return 1;
}
int cBasePolyGrid::DrawTilePolyGrid(cCamera *Camera,int i1,int j1,int i2,int j2,cMaterial &Material)
{
	cMatrix gMatrix=Matrix;
	gMatrix.trans().x+=xstep*i1; gMatrix.trans().y+=ystep*j1;
	WorldToCameraCutting(gMatrix.trans(),Camera);
	Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
	int xofs=round(gMatrix.trans().x-CameraPosScale.x), 
		yofs=round(gMatrix.trans().y-CameraPosScale.y);
//	int RadiusWorldShare=Camera->RadiusShare, SqrRadiusWorldShare=RadiusWorldShare*RadiusWorldShare, RadiusWorldShare2=RadiusWorldShare*2,SqrRadiusWorldShare2=RadiusWorldShare2*RadiusWorldShare2;
	float RadiusWorldShare=GlobalWorldRadius,DivRadiusWorldShare=GRAD_TO_DGRAD(1)/(GRAD_RAD*RadiusWorldShare);
	gMatrix.trans().x=CameraPosScale.x; gMatrix.trans().y=CameraPosScale.y;
	gMatrix=Camera->GetMatrix()*gMatrix;
	LoadMatrix(&gMatrix,1,1,1);

	int isize=i2-i1, jsize=j2-j1, ij=0;
	assert(isize<=xsize);
	assert(jsize<=ysize);
*	if(Material.AttributeMaterial&cMaterial_COLOR)
	{
		P3D->InitFix(RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE|RENDER_CLIPPING3D,isize*jsize);
		float xpos=xofs,ypos=yofs;
		
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) 
			for(int j=j1;j<j2;j++,xpos=xofs,ypos+=ystep)
				for(int i=i1;i<i2;i++,xpos+=xstep)
				{
					sBaseColor4c &p=pColor[(i%xsize)+(j%ysize)*xsize];
					float xv,yv,zv,xe,ye,zw=0;

					float xPos=xpos,yPos=ypos;
					ShareMapping2(Camera,xPos,yPos,zw,RadiusWorldShare,DivRadiusWorldShare);
					ConvertorPointWE(xPos,yPos,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij++,xe,ye,p.r,p.g,p.b,p.a,xv,yv,zv);
				}
		else 
			for(int j=j1;j<j2;j++,xpos=xofs,ypos+=ystep)
				for(int i=i1;i<i2;i++,xpos+=xstep)
				{
					sBaseColor4c &p=pColor[(i%xsize)+(j%ysize)*xsize];
					float xv,yv,zv,xe,ye;
					ConvertorPointOrthoWE(xpos,ypos,0,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij++,xe,ye,p.r,p.g,p.b,p.a,xv,yv,zv);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i,i+isize,i+1);
					P3D->AddPolygonFixTestPointFix(i+1,i+isize,i+1+isize);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i+isize,i,i+1);
					P3D->AddPolygonFixTestPointFix(i+isize,i+1,i+1+isize);
				}
	}
	else if(Material.AttributeMaterial&cMaterial_TEXTURE_PAL)
	{
		P3D->InitFix(RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_CLIPPING3D,isize*jsize);
		float xpos=xofs,ypos=yofs,duPoint0=du+uofs+u*i1,
			duPoint=duPoint0, dvPoint=dv+vofs+v*j1;
		
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) 
			for(int j=j1;j<j2;j++,xpos=xofs,ypos+=ystep,duPoint=duPoint0,dvPoint+=v)
				for(int i=i1;i<i2;i++,xpos+=xstep,duPoint+=u)
				{
					sBaseColor4c &p=pColor[(i%xsize)+(j%ysize)*xsize];
					float xv,yv,zv,xe,ye,zw=0,xPos=xpos,yPos=ypos;
					ShareMapping2(Camera,xPos,yPos,zw,RadiusWorldShare,DivRadiusWorldShare);
					ConvertorPointWE(xPos,yPos,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij++,xe,ye,p.r,p.g,p.b,p.a,duPoint,dvPoint,xv,yv,zv);
				}
		else 
			for(int j=j1;j<j2;j++,xpos=xofs,ypos+=ystep,duPoint=duPoint0,dvPoint+=v)
				for(int i=i1;i<i2;i++,xpos+=xstep,duPoint+=u)
				{
					float xv,yv,zv,xe,ye;
					sBaseColor4c &p=pColor[(i%xsize)+(j%ysize)*xsize];
					ConvertorPointOrthoWE(xpos,ypos,0,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij++,xe,ye,p.r,p.g,p.b,p.a,duPoint,dvPoint,xv,yv,zv);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i,i+isize,i+1);
					P3D->AddPolygonFixTestPointFix(i+1,i+isize,i+1+isize);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i+isize,i,i+1);
					P3D->AddPolygonFixTestPointFix(i+isize,i+1,i+1+isize);
				}
	}
//	P3D->Draw(Camera);
*	return 1;
}
int cBasePolyGrid::DrawSpecialCycl(cCamera *Camera,cMaterial &Material)
{
	assert(P3D);
//	P3D->SetTexture((cMaterial*)&Material);

	int flDrawAll=1;
	Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
	int xView=Cycl(CameraPosScale.x+GlobalWorldSize.x*GlobalWorldScale.x/2,GlobalWorldSize.x*GlobalWorldScale.x),
		yView=Cycl(CameraPosScale.y+GlobalWorldSize.y*GlobalWorldScale.y/2,GlobalWorldSize.y*GlobalWorldScale.y),
		iView=xView/xstep+1, jView=yView/ystep+1;

//	D3DSetRenderState(D3DRENDERSTATE_ZWRITEENABLE,FALSE);
	flDrawAll&=DrawTilePolyGrid(Camera,0,0,iView,jView,Material);
	flDrawAll&=DrawTilePolyGrid(Camera,iView,0,xsize+1,jView,Material);
	flDrawAll&=DrawTilePolyGrid(Camera,0,jView,iView,ysize+1,Material);
	flDrawAll&=DrawTilePolyGrid(Camera,iView,jView,xsize+1,ysize+1,Material);
//	D3DSetRenderState(D3DRENDERSTATE_ZWRITEENABLE,TRUE);
	return flDrawAll;
}
int cBasePolyGrid::DrawCycl(cCamera *Camera,int i1,int j1,int i2,int j2,cMaterial &Material)
{
	assert(P3D);
/*	P3D->SetTexture((cMaterial*)&Material);
	cMatrix gMatrix=Matrix;
	gMatrix.trans().x+=i1*xstep; gMatrix.trans().y+=j1*ystep;
	float xofs,yofs,isize=i2-i1,jsize=j2-j1;
	WorldToCameraCutting(gMatrix.trans(),Camera);
	Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
	xofs=gMatrix.trans().x-CameraPosScale.x, yofs=gMatrix.trans().y-CameraPosScale.y;
	gMatrix.trans().x=CameraPosScale.x; gMatrix.trans().y=CameraPosScale.y;
	gMatrix=Camera->GetMatrix()*gMatrix;
	LoadMatrix(&gMatrix,1,1,1);
	int RadiusWorldShare=GlobalWorldRadius, SqrRadiusWorldShare=RadiusWorldShare*RadiusWorldShare;
	if(Material.AttributeMaterial&cMaterial_COLOR)
	{
		P3D->InitFix(RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE|RENDER_CLIPPING3D,isize*jsize);
		float xpos=xofs,ypos=yofs;
		
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) 
 			for(int j=j1*xstep,ij=0;j<j2*xstep;j+=xstep,xpos=xofs,ypos+=ystep)
				for(int l=j+i1;l<(j+i2);l++,xpos+=xstep,ij++)
				{
					int r,g,b,a;
					float xv,yv,zv,xe,ye,w,zw;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))	{ r=pColor[l].r; g=pColor[l].g; b=pColor[l].b; a=pColor[l].a; }	else r=g=b=a=255;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP)) zw=pXYZWarp[l].z; else zw=0;
					if(Camera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_SHARE))
						zw-=SHARE_FLOAT(round(xpos*xpos+ypos*ypos));
					w=ConvertorPointWE(xpos,ypos,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij,xe,ye,r,g,b,a,xv,yv,zv);
				}
		else 
 			for(int j=j1*xstep,ij=0;j<j2*xstep;j+=xstep,xpos=xofs,ypos+=ystep)
				for(int l=j+i1;l<(j+i2);l++,xpos+=xstep,ij++)
				{
					int r,g,b,a;
					float xv,yv,zv,xe,ye,w,zw;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))	{ r=pColor[l].r; g=pColor[l].g; b=pColor[l].b; a=pColor[l].a; }	else r=g=b=a=255;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP)) zw=pXYZWarp[l].z; else zw=0;
					w=ConvertorPointOrthoWE(xpos,ypos,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij,xe,ye,r,g,b,a,xv,yv,zv);
				}
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i,i+isize,i+1);
					P3D->AddPolygonFixTestPointFix(i+1,i+isize,i+1+isize);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i+isize,i,i+1);
					P3D->AddPolygonFixTestPointFix(i+isize,i+1,i+1+isize);
				}
	}
	else if(Material.AttributeMaterial&cMaterial_TEXTURE_PAL)
	{
		int BaseRender=RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_CLIPPING3D;
		if(Material.Opacity) BaseRender|=RENDER_ALPHA_MOD_TEXTURE1;
		P3D->InitFix(BaseRender,isize*jsize);
		float ustep=u/xsize, vstep=v/ysize;
		float xpos=xofs,ypos=yofs,duPoint0=du+uofs+ustep*i1,dvPoint=dv+vofs+vstep*j1;
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_MULTIMATERIAL))
		{ // убирается зацикливание, константы подобраны имперически для текстуры 128
			duPoint0=fmod(duPoint0+1,1);	dvPoint=fmod(dvPoint+1,1);
			duPoint0+=0.0035f; dvPoint+=0.0035f;
			ustep=(u-0.0035f)*(1.f-0.0035f)/xsize, vstep=(v-0.0035f)*(1.f-0.0035f)/ysize;
		}
		float uLimit=u/128,vLimit=v/128,duPoint=duPoint0;
		
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) 
			for(int j=j1*xsize,ij=0;j<j2*xsize;j+=xsize,xpos=xofs,ypos+=ystep,duPoint=duPoint0,dvPoint+=vstep)
				for(int l=j+i1;l<(j+i2);l++,xpos+=xstep,duPoint+=ustep,ij++)
				{
					int r,g,b,a;
					float xw=xpos,yw=ypos,zw;
					float xv,yv,zv,xe,ye,w,du,dv;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))	{ r=pColor[l].r; g=pColor[l].g; b=pColor[l].b; a=pColor[l].a; }	else r=g=b=a=255;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP)) { xw+=pXYZWarp[l].x; yw+=pXYZWarp[l].y; zw=pXYZWarp[l].z; } else zw=0;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_UVWARP)) { du=pUVWarp[l].x*uLimit; dv=pUVWarp[l].y*vLimit; } else du=dv=0;
					if(Camera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_SHARE))
					{
   						int l=xw*xw+yw*yw;
						if(l<SqrRadiusWorldShare)
							zw-=SHARE_FLOAT(l);
						else
						{
							float k=(float)RadiusWorldShare/sqrt(l);
							xw*=k; xw*=k;
	   						l=xw*xw+yw*yw;
							zw-=RadiusWorldShare+sqrt(SqrRadiusWorldShare-l);
						}
					}
					w=ConvertorPointWE(xw,yw,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij,xe,ye,duPoint+du,dvPoint+dv,r,g,b,a,xv,yv,zv);
				}
		else 
			for(int j=j1*xsize,ij=0;j<j2*xsize;j+=xsize,xpos=xofs,ypos+=ystep,duPoint=duPoint0,dvPoint+=vstep)
				for(int l=j+i1;l<(j+i2);l++,xpos+=xstep,duPoint+=ustep,ij++)
				{
					int r,g,b,a;
					float xw=xpos,yw=ypos,zw;
					float xv,yv,zv,xe,ye,w,du,dv;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))	{ r=pColor[l].r; g=pColor[l].g; b=pColor[l].b; a=pColor[l].a; }	else r=g=b=a=255;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP)) { xw+=pXYZWarp[l].x; yw+=pXYZWarp[l].y; zw=pXYZWarp[l].z; } else zw=0;
					if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_UVWARP)) { du=pUVWarp[l].x*uLimit; dv=pUVWarp[l].y*vLimit; } else du=dv=0;
					w=ConvertorPointOrthoWE(xw,yw,zw,xv,yv,zv,xe,ye);
//					P3D->SetPointFix(ij,xe,ye,duPoint+du,dvPoint+dv,r,g,b,a,xv,yv,zv);
				}
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i,i+isize,i+1);
					P3D->AddPolygonFixTestPointFix(i+1,i+isize,i+1+isize);
				}
		if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW))
			for(int j=0;j<(jsize-1)*isize;j+=isize)
				for(int i=j;i<(j+isize-1);i++)
				{
					P3D->AddPolygonFixTestPointFix(i+isize,i,i+1);
					P3D->AddPolygonFixTestPointFix(i+isize,i+1,i+1+isize);
				}
	}
//	P3D->Draw(Camera);
*	return 1;
}

/*
void* BaseObject_BaseWaveProcess_Create(float x,float y,float z,float TimeLife,float dPhase)
{
	if(!BaseObjectMgr) return 0;
	cSurfaceReflectionMultiMaterial *SurfaceReflection=0;
	for(cBaseObjectList *start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			SurfaceReflection=(cSurfaceReflectionMultiMaterial*)start->Base;
	if(!SurfaceReflection) return 0;
	float xpos=x-SurfaceReflection->x(), ypos=y-SurfaceReflection->y();
	int iposNew=round(xpos/SurfaceReflection->xstep),jposNew=round(ypos/SurfaceReflection->ystep);
	for(start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEWAVEPROCESS))
		{
			cBaseWaveProcess *WaveProcess=(cBaseWaveProcess*)start->Base;
			float xpos=WaveProcess->Pos.x-SurfaceReflection->x(), ypos=WaveProcess->Pos.y-SurfaceReflection->y();
			int ipos=round(xpos/SurfaceReflection->xstep),jpos=round(ypos/SurfaceReflection->ystep);
			if((iposNew==ipos)&&(jpos==jposNew))
				return 0;
		}
	cBaseWaveProcess *WaveProcess=(cBaseWaveProcess*)BaseObjectMgr->New(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	WaveProcess->Pos.set(x,y,z);
	WaveProcess->Time=TimeLife;
	WaveProcess->dTime=0.05f;
	WaveProcess->Amplitude=0.8f;
	WaveProcess->dAmplitude=WaveProcess->Amplitude*WaveProcess->dTime/WaveProcess->Time;
	WaveProcess->Phase=0.f;
	WaveProcess->dPhase=dPhase;
//	WaveProcess->dPhase=0.5f;
//	WaveProcess->Time=WaveProcess->dTime*3.14/WaveProcess->dPhase;
	return WaveProcess;
}
*/