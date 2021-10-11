#include "VisGeneric.h"
#include "Camera.h"

int cVisGeneric::Release(cUnknownClass *UnkClass)
{
	assert(Scene);
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			Scene->Detach((cCamera*)UnkClass);
			delete ((cCamera*)UnkClass);
			return 0;
	}
	return 1;
}

int cVisGeneric::GetAttribute(cUnknownClass *UnkClass,int Attribute)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			return ((cCamera*)UnkClass)->GetAttribute(Attribute);
	}
	return 0;
}
int cVisGeneric::SetAttribute(cUnknownClass *UnkClass,int Attribute)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			((cCamera*)UnkClass)->SetAttribute(Attribute);
			return 0;
	};
	return 1;
}
int cVisGeneric::ClearAttribute(cUnknownClass *UnkClass,int Attribute)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			((cCamera*)UnkClass)->ClearAttribute(Attribute);
			return 0;
	};
	return 1;
}

int cVisGeneric::SetPosition(cUnknownClass *UnkClass,const Vect3f *Pos,const Vect3f *AngleGrad)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			((cCamera*)UnkClass)->SetPosition(*Pos,*AngleGrad);
			return 0;
	};
	return 1;
}
int cVisGeneric::dSetPosition(cUnknownClass *UnkClass,const Vect3f *dPos,const Vect3f *dAngleGrad)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			((cCamera*)UnkClass)->dSetPosition(*dPos,*dAngleGrad);
			return 0;
	};
	return 1;
}
int cVisGeneric::GetPosition(cUnknownClass *UnkClass,Vect3f *Pos,Vect3f *AngleGrad)
{
	switch(UnkClass->GetKind())
	{
		case KIND_CAMERA:
			((cCamera*)UnkClass)->GetPosition(*Pos,*AngleGrad);
			return 0;
	};
	return 1;
}
