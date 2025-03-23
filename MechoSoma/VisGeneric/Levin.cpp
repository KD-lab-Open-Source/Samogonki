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
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cConvertor ConvertorObjectToScreen;
		Camera->BuildMatrix(ConvertorObjectToScreen.GetMatrix(),Pos);
		ConvertorObjectToScreen.BuildMatrix();
		ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
		ConvertorObjectToScreen.ConvertPoint(Vect3f(0,0,0),pvPos,pePos);
		ConvertorObjectToScreen.ConvertPoint(Direction,pvDirection,peDirection);
		pvDirection-=pvPos; 
		Vect2f width(pvDirection.y,-pvDirection.x);
		width.normalize(0.5);
		pvDirection.normalize(1); 

		Positions.clear();

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

		P3D->Draw(Camera, this);
	}
}
void cLevin::GenerationLevin1(const Vect3f &pos,const Vect3f &dpos,const Vect2f &width,int level,int count)
{
	const int step=10;
	if(count>MAX_SIZE_LEVIN) return;
	if(pos.z>POLYGON_CUTTING_W) 
	{
		Positions.push_back(Vect3f{pos.x+width.x, pos.y+width.y, pos.z});
		Positions.push_back(Vect3f{pos.x-width.x, pos.y-width.y, pos.z});
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
		Positions.push_back(Vect3f{pos.x+width.x, pos.y+width.y, pos.z});
		Positions.push_back(Vect3f{pos.x-width.x, pos.y-width.y, pos.z});
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

const sColor4f &cLevin::GetColor() const
{
	return Color;
}

const std::vector<Vect3f> &cLevin::GetPositions() const
{
	return Positions;
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
