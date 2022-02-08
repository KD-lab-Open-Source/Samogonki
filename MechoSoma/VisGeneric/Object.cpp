#include "Object.h"
#include "Math3d.h"
#include "PolyMgr.h"

#ifdef _MECHOSOMA_
#include "mechosoma.h" // For far target
#endif

/////////////////////////////////////////////////////////////////////////////////

cLight::cLight()
{ 
	{
		Vect3f v1(0,0,0);
		Vect3f v2(0,0,0);
		SetPosition(v1,v2);
	}
	SetAmbient(sColor4f(0,0,0,1)); 
	SetDiffuse(sColor4f(0,0,0,1)); 
}
cLight::~cLight()
{ 
}

#include "Object.h"
#include "Maths.h"
#include "Base.h"

#define PREC_OMNI									10

cOmni::cOmni(float xOmni,float yOmni,float zOmni,float radius,float r,float g,float b)
{ 
	Visible=0;
	SetPosition(Vect3f(xOmni,yOmni,zOmni));
	SetVisible(CONST_VISIBLE_NULL); 
	SetColor(sColor4f(r,g,b,1.f));
	SetRadius(radius);
	ID=0; 
	memset(TotalVisibleCamera,CONST_VISIBLE_SHOW,NUMBER_CAMERA_MAX);
}
cOmni::~cOmni()
{ 
}
void cOmni::DrawID(cCamera *Camera)
{
	Vect3f pos=Pos,pw(0,0,0),pv,pe;
	WorldToCameraCutting(pos,Camera);
	Camera->ConvertPoint(pos,pw,pv,pe);
//	extern void DrawID(cCamera *Camera,int xe,int ye,unsigned int ID,char *string="");
//	DrawID(Camera,round(pe.x),round(pe.y),ID);
	extern void DrawID(int xe,int ye,unsigned int ID,char *string="");
	DrawID(round(pe.x),round(pe.y),ID);
}
void cOmni::Draw(cUnknownClass *UCameraList)
{
	MatXf matrix;
	if(Visible==CONST_VISIBLE_NULL) return;
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &ArrayCamera=*(cUnkClassDynArrayPointer*)UCameraList;
	int visible=0;
	for(int nCamera=0;nCamera<ArrayCamera.length();nCamera++)
	{
		assert(ArrayCamera[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)ArrayCamera[nCamera];
		Camera->BuildMatrix(matrix,GetPos());
		int test=Camera->TestVisible(matrix,GetRadius());
		GetVisibleTotal(nCamera)=(CONST_VISIBLE_SHOW&GetVisibleTotal(nCamera))|test;
		visible|=test;
	}
	if(visible) P3D->Draw(UCameraList,this);
}
