#include "StdAfx.h"

#include "aci_parser.h"
#include "Mesh3ds.h"
#include "Levin.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "XTList.h"
#include "fxlabApplication.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientModel.h"
#include "fxlabClientLightning.h"

void fxlabLightningSphere::Start()
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();

	if(round(KeyData[FXLAB_LIGHTNING_SPHERE_OMNI_ID]) >= 0){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(round(KeyData[FXLAB_LIGHTNING_SPHERE_OMNI_ID]));
		t->SetPosition(Position);
		t->SetProcessInterface(&OmniPoint);
		t->Start();
	};
};

void fxlabLightningSphere::Close()
{
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);
	fxlabClientKeyObjectType::Close();
};

void fxlabLightningSphere::Quant()
{
	fxlabClientKeyObjectType::Quant();

	if(OmniPoint.Process)
		OmniPoint.Process->SetPosition(Position);
};

void fxlabLightningSphere::Show()
{
	int i;
	Vect3f v;

	Bow.SetColor(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_RED],
				KeyData[FXLAB_LIGHTNING_SPHERE_DATA_GREEN],
				KeyData[FXLAB_LIGHTNING_SPHERE_DATA_BLUE],0.5f);

	Bow.SetLevin(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_STOP],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_NASCENCY],
				 KeyData[FXLAB_LIGHTNING_SPHERE_DATA_XP],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_YP],
				 KeyData[FXLAB_LIGHTNING_SPHERE_DATA_XLEVEL],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_YLEVEL]);

	Bow.size = KeyData[FXLAB_LIGHTNING_SPHERE_DATA_RADIUS];

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	for(i = 0;i < round(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_NUM]);i++){
		CalcVelocity(v);
		Bow.SetPosition(Position.x,Position.y,Position.z,v.x,v.y,v.z);
		Bow.Draw(IVisGeneric->GetCameraList(),4);
	};
	IVisGeneric->Release();
};

void fxlabLightningSphere::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_LIGHTNING_SPHERE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabLightningSphere");
};

void fxlabLightningSphere::CalcVelocity(Vect3f& v)
{
	v.setSpherical(2.0f * M_PI * fxlabClientRND.UnitRND(),2.0f * M_PI * fxlabClientRND.UnitRND(),1.0f);
};

//------------------------------------

void fxlabLightningLine::Open()
{
	fxlabLightningSphere::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabLightningLine::CalcVelocity(Vect3f& v)
{
	v = Velocity;
};

void fxlabLightningLine::SetVelocity(const Vect3f& v)
{
	Velocity = v;
	Velocity.normalize();
};

//--------------------------------------

void fxlabLightningRadius::Open()
{
	fxlabLightningLine::Open();
	Radius = 0;
};

void fxlabLightningRadius::SetVelocity(const Vect3f& v)
{
	Velocity = v;
	Radius = Velocity.norm();
	if(Radius > 0.01f)
		Velocity *= (1.0f / Radius);
	else
		Velocity = Vect3f::ZERO;
};

void fxlabLightningRadius::Show()
{
	int i;
	Vect3f v;

	if(Radius > 0.01f){
		Bow.SetColor(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_RED],
					KeyData[FXLAB_LIGHTNING_SPHERE_DATA_GREEN],
					KeyData[FXLAB_LIGHTNING_SPHERE_DATA_BLUE],0.5f);

		Bow.SetLevin(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_STOP],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_NASCENCY],
					 KeyData[FXLAB_LIGHTNING_SPHERE_DATA_XP],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_YP],
					 KeyData[FXLAB_LIGHTNING_SPHERE_DATA_XLEVEL],KeyData[FXLAB_LIGHTNING_SPHERE_DATA_YLEVEL]);

		Bow.size = Radius;

		cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
		for(i = 0;i < round(KeyData[FXLAB_LIGHTNING_SPHERE_DATA_NUM]);i++){
			CalcVelocity(v);
			Bow.SetPosition(Position.x,Position.y,Position.z,v.x,v.y,v.z);
			Bow.Draw(IVisGeneric->GetCameraList(),4);
		};
		IVisGeneric->Release();
	};
};

