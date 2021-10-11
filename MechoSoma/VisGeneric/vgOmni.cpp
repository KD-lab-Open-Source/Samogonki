#include "VisGeneric.h"
#include "Dispatcher.h"

cUnknownClass* cVisGeneric::CreateOmni(const Vect3f *Pos,float radius,const sColor4f *Color)
{
	assert(Scene);
	return (cUnknownClass*) Scene->GetO3D()->CreateOmni(Pos->x,Pos->y,Pos->z,radius,Color->r,Color->g,Color->b);
}
int cVisGeneric::ReleaseOmni(cUnknownClass *UOmni)
{
	assert(Scene);
	cOmni *Omni=(cOmni*)UOmni;
	Scene->GetO3D()->Delete(Omni);
	return 0;
}
int cVisGeneric::ReleaseOmni(int type)
{
	assert(Scene);
	Scene->GetO3D()->Release(type);
	return 0;
}
int cVisGeneric::SetOmniPosition(cUnknownClass *UOmni,const Vect3f *Pos)
{
	cOmni *Omni=(cOmni*)UOmni;
	Omni->SetPosition(*Pos);
	return 0;
}
int cVisGeneric::GetOmniPosition(cUnknownClass *UOmni,Vect3f *Pos)
{
	cOmni *Omni=(cOmni*)UOmni;
	*Pos=Omni->GetPos();
	return 0;
}
int cVisGeneric::dSetOmniPosition(cUnknownClass *UOmni,const Vect3f *dPos)
{
	cOmni *Omni=(cOmni*)UOmni;
	Omni->Translate(*dPos);
	return 0;
}
int cVisGeneric::SetOmniColor(cUnknownClass *UOmni,const sColor4f *Color)
{
	cOmni *Omni=(cOmni*)UOmni;
	Omni->SetColor(*Color);
	return 0;
}
int cVisGeneric::GetOmniColor(cUnknownClass *UOmni,sColor4f *Color)
{
	cOmni *Omni=(cOmni*)UOmni;
	*Color=Omni->GetColor();
	return 0;
}
int cVisGeneric::SetOmniSize(cUnknownClass *UOmni,float radius)
{
	cOmni *Omni=(cOmni*)UOmni;
	Omni->SetRadius(radius);
	return 0;
}
int cVisGeneric::GetOmniSize(cUnknownClass *UOmni,float *radius)
{
	cOmni *Omni=(cOmni*)UOmni;
	*radius=Omni->GetRadius();
	return 0;
}
int cVisGeneric::SetOmniVisible(cUnknownClass *UOmni,int visible)
{
	cOmni *Omni=(cOmni*)UOmni;
	Omni->SetVisible(visible);
	return 0;
}
int cVisGeneric::GetOmniVisible(cUnknownClass *UOmni,int visible)
{
	cOmni *Omni=(cOmni*)UOmni;
	return Omni->GetVisible(visible);
}
