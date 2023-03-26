#include "StdAfx.h"

#include "aci_parser.h"
#include "Levin.h"
#include "Object3d.h"
#include "IVisGeneric.h"
#include "sound.h"

#include "TERRA.H"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "XTList.h"
#include "fxlabApplication.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientModel.h"

//---------------------------------------------------------

void fxlabColorModel::Open()
{
	ModelPoint = NULL;
	Matrix.set(Mat3f::ZERO,Vect3f::ZERO);
	fxlabClientKeyObjectType::Open();
};

void fxlabColorModel::Start()
{
	sColor4f c;

	fxlabClientKeyObjectType::Start();
	CalcColor(c);

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	int id=CreateObjectID();
	ModelPoint=(cMesh*)IVisGeneric->CreateObject(M3D_KIND(id),M3D_TYPE(id));
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint,&Matrix);
	IVisGeneric->SetObjectAttribute((cUnknownClass*)ModelPoint,CalcAttribute());
	IVisGeneric->SetObjectColor((cUnknownClass*)ModelPoint,&c);
	IVisGeneric->Release();
};

void fxlabColorModel::Close()
{
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->ReleaseObject((cUnknownClass*)ModelPoint);
	fxlabClientKeyObjectType::Close();
	IVisGeneric->Release();
};

void fxlabColorModel::Quant()
{
	sColor4f c;

	fxlabClientKeyObjectType::Quant();
	CalcColor(c);

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint,&Matrix);
	IVisGeneric->SetObjectColor((cUnknownClass*)ModelPoint,&c);
	IVisGeneric->Release();
};

void fxlabColorModel::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_COLOR_MODEL_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabColorModel");
};

void fxlabColorModel::SetMatrix(const class MatXf& m)
{ 
	Matrix = m;
	Position = Matrix.trans();
};

void fxlabColorModel::SetPosition(const class Vect3f& v)
{
	fxlabClientKeyObjectType::SetPosition(v);
	Matrix.trans() = v;
};

int fxlabColorModel::CreateObjectID()
{
	return M3D_TOTAL_TYPE(round(KeyData[FXLAB_COLOR_MODEL_DATA_MODEL]),M3D_EFFECT);
};

void fxlabColorModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA];
};

int fxlabColorModel::CalcAttribute()
{
	return (MESH_NOT_SHADE | MESH_NOT_LIGHTING | MESH_LIGHTING_MUL | MESH_USE_OPACITY | MESH_NOT_WRITEZBUFFER);
};

//----------------------------------------------------------

void fxlabControlModel::Open()
{
	fxlabColorModel::Open();

	ModelAngle = Vect3f(0,0,0);
	ModelSize = Vect3f(0,0,0);
	ModelRadius = 0;

	Matrix.rot() = Mat3f(ModelAngle.x, X_AXIS)*Mat3f(ModelAngle.y, Y_AXIS)*Mat3f(ModelAngle.z, Z_AXIS);
};

void fxlabControlModel::Start()
{
	Vect3f s;
	fxlabColorModel::Start();
	CalcSize(s);

	ModelSize.x = ModelPoint->xmaxTotal();
	ModelSize.y = ModelPoint->ymaxTotal();
	ModelSize.z = ModelPoint->zmaxTotal();

	ModelRadius = ModelPoint->rmaxTotal();
	ModelPoint->SetScale(Vect3f(s.x / ModelRadius,s.y / ModelRadius,s.z / ModelRadius));
};

void fxlabControlModel::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_CONTROL_MODEL_DATA_MAX)
		ErrH.Abort("Bad fxGlowSpot Key");
};

void fxlabControlModel::Quant()
{
	Vect3f s;

	CalcMatrix();
	fxlabColorModel::Quant();
	CalcSize(s);

	ModelPoint->SetScale(Vect3f(s.x / ModelRadius,s.y / ModelRadius,s.z / ModelRadius));
};

void fxlabControlModel::CalcSize(Vect3f& size)
{
	size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
};

void fxlabControlModel::CalcMatrix()
{
	ModelAngle.x += KeyData[FXLAB_CONTROL_MODEL_DATA_ANGLEX] * DeltaTime;
	ModelAngle.y += KeyData[FXLAB_CONTROL_MODEL_DATA_ANGLEY] * DeltaTime;
	ModelAngle.z += KeyData[FXLAB_CONTROL_MODEL_DATA_ANGLEZ] * DeltaTime;

	Matrix.rot() = Mat3f(ModelAngle.x, X_AXIS)*Mat3f(ModelAngle.y, Y_AXIS)*Mat3f(ModelAngle.z, Z_AXIS);
};

//---------------------------------------------------------

void fxlabControlOmni::Open()
{
	OmniPoint = NULL;
	fxlabClientKeyObjectType::Open();
};

void fxlabControlOmni::Start()
{
	fxlabClientKeyObjectType::Start();
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	Vect3f v1(Position.x,Position.y,Position.z);
	sColor4f c1(
		KeyData[FXLAB_CONTROL_OMNI_DATA_RED],
		KeyData[FXLAB_CONTROL_OMNI_DATA_GREEN],
		KeyData[FXLAB_CONTROL_OMNI_DATA_BLUE]
	);
	OmniPoint=(cOmni*)IVisGeneric->CreateOmni(&v1,KeyData[FXLAB_CONTROL_OMNI_DATA_RADIUS],&c1);
	IVisGeneric->Release();
};

void fxlabControlOmni::Close()
{
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->ReleaseOmni((cUnknownClass*)OmniPoint);
	IVisGeneric->Release();
	fxlabClientKeyObjectType::Close();
};

void fxlabControlOmni::Quant()
{
	fxlabClientKeyObjectType::Quant();
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	Vect3f v1(Position.x,Position.y,Position.z);
	IVisGeneric->SetOmniPosition((cUnknownClass*)OmniPoint,&v1);
	IVisGeneric->SetOmniSize((cUnknownClass*)OmniPoint,KeyData[FXLAB_CONTROL_OMNI_DATA_RADIUS]);
	sColor4f c1(
		KeyData[FXLAB_CONTROL_OMNI_DATA_RED],
		KeyData[FXLAB_CONTROL_OMNI_DATA_GREEN],
		KeyData[FXLAB_CONTROL_OMNI_DATA_BLUE]
	);
	IVisGeneric->SetOmniColor((cUnknownClass*)OmniPoint,&c1);
	IVisGeneric->Release();
};

void fxlabControlOmni::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_CONTROL_OMNI_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabControlOmni");
};


void fxlabControlOmni::SetVisible(int v)
{
	if(OmniPoint)
	{
		cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
		IVisGeneric->SetOmniVisible((cUnknownClass*)OmniPoint,v);
		IVisGeneric->Release();
	}
};

//--------------------------------------------------

void fxlabBonusModel::Start()
{
	ModelAngle = Vect3f(fxlabClientRND.UnitRND()*360.0f,fxlabClientRND.UnitRND()*360.0f,fxlabClientRND.UnitRND()*360.0f);
	fxlabControlModel::Start();
};

//-------------------------------------------------

void fxlabBonusPart::Open()
{
	fxlabBonusModel::Open();
	BonusPartID = M3D_TOTAL_TYPE(0,M3D_ENGINE);
};

//-------------------------------------------------

void fxlabAnimationModel::Quant()
{
	float phase;
	fxlabControlModel::Quant();

	CalcDeltaPhase(phase);
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->SetObjectAnimation((cUnknownClass*)ModelPoint,10000.0f,phase);
	IVisGeneric->Release();
};

void fxlabAnimationModel::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_ANIMATION_MODEL_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabAnimationModel");
};

void fxlabAnimationModel::CalcDeltaPhase(float& delta)
{
	delta = KeyData[FXLAB_ANIMATION_MODEL_DATA_DELTA_PHASE] * KeyTime;
};


//---------------------------------------------------

void fxlabSlimeModel::Start()
{
	Phase = 1.0f;
	fxlabAnimationModel::Start();
};

void fxlabSlimeModel::Quant()
{
	fxlabAnimationModel::Quant();
	if(!Alive)
		Phase *= 0.9f;
};

void fxlabSlimeModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA] * Phase;
};

void fxlabSlimeModel::CalcSize(Vect3f& size)
{	
	size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] * Phase;
};

int fxlabSlimeModel::GetAlive()
{
	if(!Alive && Phase < 0.01)
		return 0;
	return 1;
};

//-----------------------------------

void fxlabLashModel::Open()
{
	fxlabAnimationModel::Open();
	ScaleDelta = 0;
	CurrentScale = 1.0f;
	ScalingFlag = 0;
	DeltaPhase = 0.8f;
};

void fxlabLashModel::Start()
{
	Phase = 1.0f;
	AnimationRate = 0.5f + fxlabClientRND.UnitRND() * 1.5f;
	ModelAngle = Vect3f(0,0,fxlabClientRND.UnitRND()*360.0f);
	fxlabAnimationModel::Start();
};

void fxlabLashModel::Quant()
{
	fxlabAnimationModel::Quant();
	
	if((!fxlabGetWorldReflectionEnable()) && Position.z < vMap->LevelH2O && (GetAt(XCYCL(int(round(Position.x))),YCYCL(int(round(Position.y)))) & At_WATER)){
		DeltaPhase = 0.95f;
		SetAlive(0);
	};

	if(!Alive)
		Phase *= DeltaPhase;
};

int fxlabLashModel::GetAlive()
{
	if(!Alive && Phase < 0.01)
		return 0;
	return 1;
};

void fxlabLashModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA] * Phase;
};

void fxlabLashModel::CalcDeltaPhase(float& delta)
{
	delta = KeyData[FXLAB_ANIMATION_MODEL_DATA_DELTA_PHASE] * KeyTime * AnimationRate;
};


void fxlabLashModel::CalcSize(Vect3f& size)
{
	if(ScalingFlag){
		size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
		size.z *= (CurrentScale + ScaleDelta * (float)(ScaleTimer()));
		size.x *= (CurrentScale + ScaleDelta * (float)(ScaleTimer())) * 0.35f;
		size.y *= (CurrentScale + ScaleDelta * (float)(ScaleTimer())) * 0.35f;
	}else
		size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
};


void fxlabLashModel::SetScaleAnimation(float start_scale,float end_scale,int time)
{
	CurrentScale = end_scale;
	ScaleDelta = (start_scale - end_scale) / (float)(time);
	ScaleTimer.start(time);
	ScalingFlag = 1;
};

//------------------------------------------------------

void fxlabVoodooHeadModel::Start()
{
	Phase = 1.0f;
	fxlabControlModel::Start();
};

void fxlabVoodooHeadModel::Quant()
{
	fxlabControlModel::Quant();
	if(!Alive)
		Phase *= 0.7f;
};

void fxlabVoodooHeadModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA] * Phase;
};

void fxlabVoodooHeadModel::CalcSize(Vect3f& size)
{
	size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] + (1.0f - Phase) * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] * 2.0f;
};

int fxlabVoodooHeadModel::GetAlive()
{
	if(!Alive && Phase < 0.01)
		return 0;
	return 1;
};

//---------------------------

void fxlabPressureModel::Open()
{
	PressureSize = Vect3f(1.0f,1.0f,1.0f);
	fxlabControlModel::Open();
};

void fxlabPressureModel::CalcSize(Vect3f& size)
{
	float t;

	t = KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_PHASE] * KeyTime;
	size = PressureSize;
	size *= KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];

	size.x = size.x * (1 + cosf(t) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
	size.y = size.y * (1 + cosf(t + M_PI / 2) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
	size.z = size.z * (1 + cosf(t + M_PI) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
};


void fxlabPressureModel::KeyCheck()
{
	if(KeyPoint->GetKeyNum() != FXLAB_PRESSURE_MODEL_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabPressureModel");
};

void fxlabPressureModel::SetVelocity(const Vect3f& v)
{ 
	PressureSize = v;
};

//-------------------------------------------------

void fxlabRevoltSpaceModel::Start()
{
	Phase = 1.0f;
	fxlabPressureModel::Start();
};

void fxlabRevoltSpaceModel::Quant()
{
	fxlabPressureModel::Quant();
	if(!Alive)
		Phase *= 0.7f;
};

void fxlabRevoltSpaceModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA] * Phase;
};

void fxlabRevoltSpaceModel::CalcSize(Vect3f& size)
{
	float t;

	t = KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_PHASE] * KeyTime;
	size = PressureSize;
	size *= KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] + (1.0f - Phase) * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] * 2.0f;

	size.x = size.x * (1 + cosf(t) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
	size.y = size.y * (1 + cosf(t + M_PI / 2) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
	size.z = size.z * (1 + cosf(t + M_PI) * KeyData[FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS]);
};

int fxlabRevoltSpaceModel::GetAlive()
{
	if(!Alive && Phase < 0.01)
		return 0;
	return 1;
};


//-------------------------------------------------

void fxlabMassShifterModel::Quant()
{
	Matrix.trans() = Vect3f(Position.x,Position.y,Position.z + KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS]);
	fxlabControlModel::Quant();
};

void fxlabMassShifterModel::CalcSize(Vect3f& size)
{
	size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
	size.x = size.y = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
};


//---------------------------------------------------

void fxlabTrapGroundModel::Start()
{
	Phase = 1.0f;
	fxlabControlModel::Start();
};

void fxlabTrapGroundModel::Quant()
{
	fxlabControlModel::Quant();
	if(!Alive)
		Phase *= 0.6f;
};

void fxlabTrapGroundModel::CalcColor(sColor4f& color)
{
	color.r = KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA] * Phase;
};

void fxlabTrapGroundModel::CalcSize(Vect3f& size)
{
	size.x = size.y = size.z = KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] + (1.0f - Phase) * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS] * 2.0f;
};

int fxlabTrapGroundModel::GetAlive()
{
	if(!Alive && Phase < 0.01)
		return 0;
	return 1;
};

void fxlabTrapGroundModel::CalcMatrix()
{
	Matrix.rot() = Matrix.rot()*Mat3f(KeyData[FXLAB_CONTROL_MODEL_DATA_ANGLEZ] * DeltaTime, Z_AXIS);
};

//---------------------------------------------------

Mat3f turn_cross(const Vect3f& target, const Vect3f& current)
{
	Vect3f cross = target % current;
	float len = cross.norm();
	if(len > 1e-8f){
		cross /= len;
		return Mat3f(cross, acosf(dot(target, current)/(target.norm()*current.norm() + 1e-8f)));
		}
	else{
		if(dot(target, current) > 0)
			return Mat3f::ID;
		Vect3f tmp;
		decomposition(target, fabs(target.x) > fabs(target.y) ? Vect3f::J : Vect3f::I, tmp, cross);
		cross.normalize();
		return Mat3f(cross, M_PI);
		}
}


void fxlabBulletModel::Open()
{
	fxlabControlModel::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabBulletModel::Start()
{
	fxlabControlModel::Start();

	SourcePosition = Position;

	NormVelocity = Velocity;
	NormVelocity.normalize();
};

void fxlabBulletModel::Quant()
{
	Position = Velocity * CurrentTime * fxlabGlobalTimeRate;
	Position += SourcePosition;
	Position = CYCLE(Position);

	Matrix.trans() = Position;

	fxlabControlModel::Quant();
};

void fxlabBulletModel::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

void fxlabBulletModel::CalcMatrix()
{
	Matrix.rot() = turn_cross(Vect3f(Velocity.x,Velocity.y,-Velocity.z),Vect3f(0,0,-1.0f));
};

int fxlabBulletModel::CalcAttribute()
{
	return (MESH_NOT_LIGHTING | MESH_LIGHTING_MUL | MESH_USE_OPACITY);
};

//--------------------------------

void fxlabPetardModel::Start()
{
	fxlabGeneralObjectType* t;

	fxlabBulletModel::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
	t->SetStartTime(fxlabClientD->GetTime());

	Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
	t->SetPosition(CYCLE(v1));

	Vect3f v2 = -NormVelocity;
	t->SetVelocity(v2);

	t->SetKeyID(FXLAB_ID_KEY_PETARD_TAIL);
	t->SetProcessInterface(&TailPoint);
	t->Start();
};

void fxlabPetardModel::Close()
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	fxlabBulletModel::Close();
};

void fxlabPetardModel::Quant()
{
	fxlabBulletModel::Quant();
	if(TailPoint.Process)
	{
		Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
		TailPoint.Process->SetPosition(CYCLE(v1));
	}
};

//--------------------------------

void fxlabSnowBulletModel::Start()
{
	fxlabGeneralObjectType* t;

	fxlabBulletModel::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
	t->SetStartTime(fxlabClientD->GetTime());

	Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
	t->SetPosition(CYCLE(v1));

	Vect3f v2 = -NormVelocity;
	t->SetVelocity(v2);

	t->SetKeyID(FXLAB_ID_KEY_SNOW_BULLET);
	t->SetProcessInterface(&TailPoint);
	t->Start();
};

void fxlabSnowBulletModel::Close()
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	fxlabBulletModel::Close();
};

void fxlabSnowBulletModel::Quant()
{
	fxlabBulletModel::Quant();
	if(TailPoint.Process)
	{
		Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
		TailPoint.Process->SetPosition(CYCLE(v1));
	}
};

//--------------------------------

void fxlabFireBulletModel::Start()
{
	fxlabGeneralObjectType* t;

	fxlabBulletModel::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
	t->SetStartTime(fxlabClientD->GetTime());

	Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
	t->SetPosition(CYCLE(v1));

	Vect3f v2 = -NormVelocity;
	t->SetVelocity(v2);

	t->SetKeyID(FXLAB_ID_KEY_FIRE_BULLET);
	t->SetProcessInterface(&TailPoint);
	t->Start();
};

void fxlabFireBulletModel::Close()
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	fxlabBulletModel::Close();
};

void fxlabFireBulletModel::Quant()
{
	fxlabBulletModel::Quant();
	if(TailPoint.Process)
	{
		Vect3f v1 = Position - NormVelocity * KeyData[FXLAB_CONTROL_MODEL_DATA_RADIUS];
		TailPoint.Process->SetPosition(CYCLE(v1));
	}
};

//-------------------------------------------------------

void fxlabClientJumpActionModel::Open()
{
	fxlabControlModel::Open();
	LocalPosition = Vect3f::ZERO;
	Velocity = Vect3f::ZERO;
};

void fxlabClientJumpActionModel::Quant()
{
	Vect3f v;

	v = Velocity;
	v *= CurrentTime * fxlabGlobalTimeRate;
	LocalPosition += v;
	Matrix.trans() += LocalPosition;

	fxlabControlModel::Quant();
};


//-------------------------------------------------------

void fxlabClientWaterWaveModel::Open()
{
	fxlabControlModel::Open();
	Red = 1.0f;
	Green = 1.0f;
	Blue = 1.0f;
	Alpha = 1.0f;
};

void fxlabClientWaterWaveModel::CalcColor(sColor4f& color)
{
	color.r = Red * KeyData[FXLAB_COLOR_MODEL_DATA_RED];
	color.g = Green * KeyData[FXLAB_COLOR_MODEL_DATA_GREEN];
	color.b = Blue * KeyData[FXLAB_COLOR_MODEL_DATA_BLUE];
	color.a = Alpha * KeyData[FXLAB_COLOR_MODEL_DATA_ALPHA];
};

void fxlabClientWaterWaveModel::SetPosition(const class Vect3f& v)
{
	Vect3f t = v;
	t.z += 1.0f;
	fxlabClientKeyObjectType::SetPosition(t);
	Matrix.trans() = t;
};
