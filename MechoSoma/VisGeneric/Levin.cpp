#include "PolyMgr.h"
#include "Camera.h"
#include "Levin.h"
#include "Unknown.h"
#include "BaseDefine.h"

#ifdef _MECHOSOMA_
#include "mch_common.h" // For far target
#endif

inline float random_f()							{ return (((float)rand())/RAND_MAX); }

void cLevin::Draw(cUnknownClass *UCameraList,int var)
{
	Vect3f pePos,pvPos,pvDirection,peDirection;
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cRenderDevice *RenderDevice=(cRenderDevice*)P3D->GetRenderDevice(0);
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		P3D->SetClippingPlane(Camera);
		cConvertor ConvertorObjectToScreen;
		Camera->BuildMatrix(ConvertorObjectToScreen.GetMatrix(),Pos);
		ConvertorObjectToScreen.BuildMatrix();
		ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
		ConvertorObjectToScreen.ConvertPoint(Vect3f(0,0,0),pvPos,pePos);
		ConvertorObjectToScreen.ConvertPoint(Direction,pvDirection,peDirection);
		pvDirection-=pvPos; 
		pvDirection.normalize(1); 
		peDirection-=pePos;
		Vect2f width(peDirection.y,-peDirection.x);
		width.normalize(pePos.z*0.5f);
		int RenderAttribute=RENDER_COLOR_MOD_DIFFUSE;
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
		P3D->InitFix(RenderAttribute);
		switch(var)
		{
			case 1:
				GenerationLevin1(pvPos,pvDirection,width,1); 
				break;
			case 4:
				GenerationLevin4(pvPos,pvDirection,width,size,1); 
				break;
			default: 
				ErrAbort("Error: cLevin::Draw()\r\nUnknown var");
		}
		P3D->Draw((cUnknownClass*)Camera,(cUnknownClass*)RenderDevice);
	}
}
void cLevin::GenerationLevin1(const Vect3f &pos,const Vect3f &dpos,const Vect2f &width,int level,int count)
{
	const int step=10;
	if(count>MAX_SIZE_LEVIN) return;
	if(pos.z>POLYGON_CUTTING_W) 
	{
		int	r=Color.GetR(),g=Color.GetG(),b=Color.GetB(),a=Color.GetA();
		assert(r<=255&&g<=255&&b<=255&&a<=255);
		int NumberPoint=P3D->PointAttribute.length()=(P3D->PointFix.length()+=2);
		float div_zv=1/pos.z;
		Vect3f pe(pos.x*div_zv+width.x,pos.y*div_zv+width.y,div_zv);
		P3D->SetPointFix(NumberPoint-2,pe, r,g,b,a, pos);
		pe.set(pos.x*div_zv-width.x,pos.y*div_zv-width.y,div_zv);
		P3D->SetPointFix(NumberPoint-1,pe, r,g,b,a, pos);
		if((P3D->PointAttribute[NumberPoint-2].clip&(CLIP_XMIN|CLIP_XMAX|CLIP_YMIN|CLIP_YMAX|CLIP_ZMIN|CLIP_ZMAX))==0) 
		if((P3D->PointAttribute[NumberPoint-1].clip&(CLIP_XMIN|CLIP_XMAX|CLIP_YMIN|CLIP_YMAX|CLIP_ZMIN|CLIP_ZMAX))==0) 
		if(NumberPoint>=4)
		{
			P3D->AddPolygonFixTestPointFix(NumberPoint-2,NumberPoint-3,NumberPoint-1);
			P3D->AddPolygonFixTestPointFix(NumberPoint-2,NumberPoint-4,NumberPoint-3);
		}	
	}
	Vect3f posNew(
		pos.x+step*dpos.x+(random_f()-0.5f)*pAberration.x,
		pos.y+step*dpos.y+(random_f()-0.5f)*pAberration.y,
		pos.z+step*dpos.z+(random_f()-0.5f)*pAberration.z);

	if(random_f()*level*step<pNascency)
	{
		Vect3f dposNew(
			dpos.x+(random_f()-0.5f)*pLevel.x*pAberration.x,
			dpos.y+(random_f()-0.5f)*pLevel.y*pAberration.y,
			dpos.z+(random_f()-0.5f)*pLevel.z*pAberration.z);
		GenerationLevin1(posNew,dposNew,width,level+1,count+=step);
	}
	if(random_f()>(pStop*level*step))
		GenerationLevin1(posNew,dpos,width,level,count+=step);
}
void cLevin::GenerationLevin4(const Vect3f &pos,const Vect3f &dpos,const Vect2f &width,int length,int level,int count)
{
	const int step=10;
	if(count>MAX_SIZE_LEVIN) return;
	if(pos.z>POLYGON_CUTTING_W) 
	{
		int	r=Color.GetR(),g=Color.GetG(),b=Color.GetB(),a=Color.GetA();
		assert(r<=255&&g<=255&&b<=255&&a<=255);
		int NumberPoint=P3D->PointAttribute.length()=(P3D->PointFix.length()+=2);
		float div_zv=1/pos.z;
		Vect3f pe(pos.x*div_zv+width.x,pos.y*div_zv+width.y,div_zv);
		P3D->SetPointFix(NumberPoint-2,pe, r,g,b,a, pos);
		pe.set(pos.x*div_zv-width.x,pos.y*div_zv-width.y,div_zv);
		P3D->SetPointFix(NumberPoint-1,pe, r,g,b,a, pos);
		if((P3D->PointAttribute[NumberPoint-2].clip&(CLIP_XMIN|CLIP_XMAX|CLIP_YMIN|CLIP_YMAX|CLIP_ZMIN|CLIP_ZMAX))==0) 
		if((P3D->PointAttribute[NumberPoint-1].clip&(CLIP_XMIN|CLIP_XMAX|CLIP_YMIN|CLIP_YMAX|CLIP_ZMIN|CLIP_ZMAX))==0) 
		if(NumberPoint>=4)
		{
			P3D->AddPolygonFixTestPointFix(NumberPoint-2,NumberPoint-3,NumberPoint-1);
			P3D->AddPolygonFixTestPointFix(NumberPoint-2,NumberPoint-4,NumberPoint-3);
		}
	}
	Vect3f posNew(
		pos.x+step*dpos.x+(random_f()-0.5f)*pAberration.x,
		pos.y+step*dpos.y+(random_f()-0.5f)*pAberration.y,
		pos.z+step*dpos.z+(random_f()-0.5f)*pAberration.z);

	if(random_f()*level*step<pNascency)
	{
		Vect3f dposNew(
			dpos.x+(random_f()-0.5f)*pLevel.x*pAberration.x,
			dpos.y+(random_f()-0.5f)*pLevel.y*pAberration.y,
			dpos.z+(random_f()-0.5f)*pLevel.z*pAberration.z);
		GenerationLevin4(posNew,dposNew,width,length,level+1,count+=step);
	}
	if((random_f()>(pStop*level*step))||((level==1)&&(count<length)))
		GenerationLevin4(posNew,dpos,width,length,level,count+=step);
}

cL3D *L3D=0;

void cL3D::Draw(unsigned short *scr,unsigned short *zBuffer)
{
/*
	while(BaseList->next)
	{
		BaseList->next->Base->Draw(4);
		delete BaseList->next->Base;
		Pop();
	}
*/
}
