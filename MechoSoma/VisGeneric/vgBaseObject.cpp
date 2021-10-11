#include "VisGeneric.h"
#include "BaseObject.h"
#include "BaseTrail.h"
#include "Scene.h"

cUnknownClass* cVisGeneric::CreateTangentTrail(const Vect3f *pos,const sColor4f *c1,const Vect3f *dpos,const sColor4f *c2,float Time,float dAnimTime,float Width)
{
	assert(Scene);
	if(!GET_RENDER_TUNING(RENDER_TUNING_BASETRAIL) && dAnimTime > 1e10f) return 0;
	Vect3f Pos=*pos,dPos=*dpos;
	cTangentTrail *TangentTrail=(cTangentTrail*)Scene->GetBaseObjectMgr()->New(BASEOBJECT_TYPE_BASETRAIL_TANGENT,BASEOBJECT_KIND_DRAW_EFFECT);
	TangentTrail->Diffuse1=*c1;
	TangentTrail->Diffuse2=*c2;
//	Pos.z+=3;
	TangentTrail->Set(Pos,dPos,Width);
//	TangentTrail->SetTexture(c1.r*((1<<PALETTE_MAX_SIZE)-1),c1.g*((1<<PALETTE_MAX_SIZE)-1),c1.b*((1<<PALETTE_MAX_SIZE)-1));
	TangentTrail->Timer.start();
	TangentTrail->Duration = Time;
//	TangentTrail->StartTime=CurrentTime;
//	TangentTrail->CurrentTime=0.f;
//	TangentTrail->FinishTime=Time;
	TangentTrail->AnimTime=dAnimTime;
	TangentTrail->tMap=Scene->GetTileMap();
	return 0;
}
int cVisGeneric::ReleaseTangentTrail(cUnknownClass* UTangentTrail)
{
	return 0;
}
