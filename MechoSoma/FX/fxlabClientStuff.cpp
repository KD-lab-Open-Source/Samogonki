#include "StdAfx.h"

#include "aci_parser.h"
// #include "win32f.h"

#include "Handle.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "OwnerProtection.h"
#include "Meteorite.h"

#include "Levin.h"
#include "TileMap.h"
#include "Object3d.h"
#include "PolyGrid.h"
#include "TileWater.h"
#include "IVisGeneric.h"
#include "TERRA.H"
#include "race.h"
#include "sound.h"
#include "PolyMgr.h"

#include "CameraDispatcher.h"


#include "sur_scr.h"
#include "SST_Reader.h"

#include "M3d_effects.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"

#include "fxlabApplication.h"

#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientEffect.h"
#include "fxlabClientLauncher.h"
#include "Hermit.h"
#include "fxlabClientModel.h"
#include "fxlabClientStuff.h"

#include "fxlabClientLightning.h"

//------------------------------------------

int fxlabCheckPointIndex = 0;
int fxlabCheckPointMax = 0;

MatXf getPlacementPose(const Vect3f& center);

void fxlabCheckPoint::Open(void)
{
	fxlabClientSpaceType::Open();

	KeyPoint[FXLAB_CHECK_POINT_MODE_CURRENT] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT0);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_CURRENT]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint CurrentMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_FADE] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT1);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_FADE]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint FadeMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_FUTURE] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT2);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_FUTURE]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint FutureMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_MOVE] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT4);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_MOVE]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint MoveMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_SKIP] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT5);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_SKIP]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint SkipMode");


	KeyPoint[FXLAB_CHECK_POINT_MODE_PAST] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT3);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_PAST]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint PastMode");


	KeyPoint[FXLAB_CHECK_POINT_MODE_ACTIVE_START] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT6);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_ACTIVE_START]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint ActiveStartMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_PASSIVE_START] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT7);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_PASSIVE_START]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint PassiveStartMode");


	KeyPoint[FXLAB_CHECK_POINT_MODE_FADE_START] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT8);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_FADE_START]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint FadeStartMode");

	KeyPoint[FXLAB_CHECK_POINT_MODE_MOVE_START] = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,FXLAB_ID_KEY_CHECKPOINT9);
	if(KeyPoint[FXLAB_CHECK_POINT_MODE_MOVE_START]->GetKeyNum() != FXLAB_CHECK_POINT_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabCheckPoint MoveStartMode");

	AnglePosition = Vect3f(0,0,fxlabClientRND.UnitRND()*360.0f);
	Velocity = Vect3f::ZERO;

	ModelIndex = 0;
	ModelPoint[0] = ModelPoint[1] = NULL;
	ModelID[0] = M3D_CHECK_POINT0;
	ModelID[1] = M3D_CHECK_POINT1;
	ModelRadius = 1.0f;

	AddPhase = fxlabClientRND.UnitRND() * M_PI * 2.0f;
	
	Mode = FXLAB_CHECK_POINT_MODE_FUTURE;
	Scale = 1.0f;
	KeyStatus = 1;
	nCamera = 0;
};

void fxlabCheckPoint::Close(void)
{	
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->ReleaseObject((cUnknownClass*)ModelPoint[0]);
	IVisGeneric->ReleaseObject((cUnknownClass*)ModelPoint[1]);
	IVisGeneric->Release();
	fxlabClientSpaceType::Close();
};

void fxlabCheckPoint::Start(void)
{
	fxlabClientSpaceType::Start();
	KeyUpdate();

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	ModelPoint[0]=(cMesh*)IVisGeneric->CreateObject(M3D_KIND(ModelID[0]),M3D_TYPE(ModelID[0]));
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint[0],&Position,&AnglePosition);
	IVisGeneric->SetObjectAttribute((cUnknownClass*)ModelPoint[0],/*MESH_LIGHTING_MAX|*/MESH_USE_OPACITY | MESH_LIGHTING_MUL/* | MESH_NOT_WRITEZBUFFER*/ | MESH_NOT_LIGHTING | nCamera);
	ModelPoint[1]=(cMesh*)IVisGeneric->CreateObject(M3D_KIND(ModelID[1]),M3D_TYPE(ModelID[1]));
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint[1],&Position,&AnglePosition);
	IVisGeneric->SetObjectAttribute((cUnknownClass*)ModelPoint[1],/*MESH_LIGHTING_MAX|*/MESH_USE_OPACITY | MESH_LIGHTING_MUL/* | MESH_NOT_WRITEZBUFFER*/ | MESH_NOT_LIGHTING | nCamera);

	switch(Mode){
		case FXLAB_CHECK_POINT_MODE_CURRENT:
		case FXLAB_CHECK_POINT_MODE_PAST:
		case FXLAB_CHECK_POINT_MODE_FADE:
		case FXLAB_CHECK_POINT_MODE_MOVE:
		case FXLAB_CHECK_POINT_MODE_SKIP:
		case FXLAB_CHECK_POINT_MODE_ACTIVE_START:
		case FXLAB_CHECK_POINT_MODE_MOVE_START:
		case FXLAB_CHECK_POINT_MODE_FADE_START:
			ModelIndex = 0;
			break;
		case FXLAB_CHECK_POINT_MODE_PASSIVE_START:
		case FXLAB_CHECK_POINT_MODE_FUTURE:
			ModelIndex = 1;
			break;
	};
	IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],1);
	IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[1-ModelIndex],0);
	IVisGeneric->Release();

	ModelRadius = ModelPoint[ModelIndex]->rmaxTotal();
	ModelUpdate();
};

void fxlabCheckPoint::StopQuant(void)
{
	fxlabClientSpaceType::Quant();
	KeyUpdate();
	switch(Mode){
		case FXLAB_CHECK_POINT_MODE_MOVE:
			{
				cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
				IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],0);
				IVisGeneric->Release();
			}
 			break;
		case FXLAB_CHECK_POINT_MODE_PAST:
		case FXLAB_CHECK_POINT_MODE_SKIP:
		case FXLAB_CHECK_POINT_MODE_MOVE_START:
			if(!KeyStatus)
			{
				cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
				IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],0);
				IVisGeneric->Release();
			}
 			break;
	};

	ModelUpdate();
	AnglePosition.z += KeyData[FXLAB_CHECK_POINT_DATA_DELTA_ANGLE] * DeltaTime;
	if(AnglePosition.z > 360.0f) AnglePosition.z -= 360.0f;
	AddPhase += KeyData[FXLAB_CHECK_POINT_DATA_PHASE_STEP] * DeltaTime;
};

void fxlabCheckPoint::Quant(void)
{
	fxlabClientSpaceType::Quant();
	KeyUpdate();
	switch(Mode){
		case FXLAB_CHECK_POINT_MODE_MOVE:
			{
				cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
				IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],0);
				IVisGeneric->Release();
			}
 			break;
		case FXLAB_CHECK_POINT_MODE_PAST:
		case FXLAB_CHECK_POINT_MODE_SKIP:
		case FXLAB_CHECK_POINT_MODE_MOVE_START:
			if(!KeyStatus)
			{
				cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
				IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],0);
				IVisGeneric->Release();
			}
 			break;
	};


	ModelUpdate();
	AnglePosition.z += KeyData[FXLAB_CHECK_POINT_DATA_DELTA_ANGLE] * DeltaTime;
	if(AnglePosition.z > 360.0f) AnglePosition.z -= 360.0f;
	AddPhase += KeyData[FXLAB_CHECK_POINT_DATA_PHASE_STEP] * DeltaTime;
};


void fxlabCheckPoint::KeyUpdate(void)
{
	KeyStatus = KeyPoint[Mode]->GetKey(KeyData,CurrentTime * fxlabGlobalTimeRate);
};

void fxlabCheckPoint::ModelUpdate(void)
{
	float scale;
	float phase;

	phase = KeyData[FXLAB_CHECK_POINT_DATA_MOVE_PHASE] + KeyData[FXLAB_CHECK_POINT_DATA_PHASE_DELTA] * sinf(AddPhase);
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();

	Vect3f v1(Position.x + Velocity.x * phase,Position.y + Velocity.y * phase,Position.z + Velocity.z * phase);
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint[ModelIndex],&v1,&AnglePosition);

	scale = KeyData[FXLAB_CHECK_POINT_DATA_RADIUS] * Scale / ModelRadius;
	Vect3f v2(scale,scale,scale);
	IVisGeneric->SetObjectScale((cUnknownClass*)ModelPoint[ModelIndex],&v2);

	sColor4f c1(KeyData[FXLAB_CHECK_POINT_DATA_RED],KeyData[FXLAB_CHECK_POINT_DATA_GREEN],KeyData[FXLAB_CHECK_POINT_DATA_BLUE],KeyData[FXLAB_CHECK_POINT_DATA_ALPHA]);
	IVisGeneric->SetObjectColor((cUnknownClass*)ModelPoint[ModelIndex],&c1);

	IVisGeneric->Release();
};


void fxlabCheckPoint::SetScale(float scale)
{
	Scale = scale;
};

void fxlabCheckPoint::SetMode(int mode)
{
	Mode = mode;
};

void fxlabCheckPoint::SetVelocity(const Vect3f& velocity)
{
	Velocity = velocity;
};

void fxlabCheckPoint::SetAngle(float angle)
{
	AnglePosition.z = angle;
};

void fxlabCheckPoint::SetCheckPointModels(int id0,int id1)
{
	ModelID[0] = id0;
	ModelID[1] = id1;
};

void fxlabCheckPoint::SetCheckPointRacer(int mode,struct mchRacer* p,int init_flag)
{
	fxlabGeneralObjectType* t;
	float scale;

	if(Mode != mode || init_flag){
		if(mode == FXLAB_CHECK_POINT_MODE_MOVE && Mode == FXLAB_CHECK_POINT_MODE_FADE)
			Mode = FXLAB_CHECK_POINT_MODE_SKIP;
		else{
			if(mode == FXLAB_CHECK_POINT_MODE_PAST && Mode == FXLAB_CHECK_POINT_MODE_FADE_START)
				Mode = FXLAB_CHECK_POINT_MODE_MOVE_START;
			else
				Mode = mode;
		};

		StartTime = fxlabClientD->GetTime();

		if(init_flag)
			StartTime = KeyPoint[Mode]->GetScale();
		else
			CurrentTime = 0;

		switch(Mode){
			case FXLAB_CHECK_POINT_MODE_CURRENT:
			case FXLAB_CHECK_POINT_MODE_PAST:
			case FXLAB_CHECK_POINT_MODE_FADE:
			case FXLAB_CHECK_POINT_MODE_MOVE:
			case FXLAB_CHECK_POINT_MODE_SKIP:
			case FXLAB_CHECK_POINT_MODE_ACTIVE_START:
			case FXLAB_CHECK_POINT_MODE_MOVE_START:
			case FXLAB_CHECK_POINT_MODE_FADE_START:
				ModelIndex = 0;
				break;
			case FXLAB_CHECK_POINT_MODE_PASSIVE_START:
			case FXLAB_CHECK_POINT_MODE_FUTURE:
				ModelIndex = 1;
				break;
		};

		cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
		IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[ModelIndex],1);
		IVisGeneric->SetObjectVisible((cUnknownClass*)ModelPoint[1 - ModelIndex],0);
		IVisGeneric->Release();
		ModelUpdate();

		if(Mode == FXLAB_CHECK_POINT_MODE_MOVE && p && !init_flag){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_POLAR_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(Position);
			t->SetKeyID(FXLAB_ID_KEY_POLAR_STAR);

			scale = KeyData[FXLAB_CHECK_POINT_DATA_RADIUS] * Scale / ModelRadius;
			Vect3f v1(scale,scale,scale);
			t->SetScaleVector(v1);

			t->SetAngleVector(AnglePosition);

			Vect3f v2(KeyData[FXLAB_CHECK_POINT_DATA_RED],KeyData[FXLAB_CHECK_POINT_DATA_GREEN],KeyData[FXLAB_CHECK_POINT_DATA_BLUE]);
			t->SetColorVector(v2);

			t->SetTransparency(KeyData[FXLAB_CHECK_POINT_DATA_ALPHA]);
			t->SetModel(ModelID[0]);
			t->SetRacerPoint(p);
			t->SetAngle(2.0 * M_PI * fxlabCheckPointIndex / fxlabCheckPointMax);
			t->SetCheckPointCamera(nCamera);
			t->Start();
			fxlabCheckPointIndex++;
		};
	};
};

void fxlabCheckPoint::SetCheckPointCamera(int camera)
{
	nCamera = camera;
};

//----------------------------------------------------------


void fxlabPolarModel::Open(void)
{
	fxlabClientKeyObjectType::Open();
	ModelPoint = NULL;
	ModelScale = Vect3f(1.0f,1.0f,1.0f);
	ModelAngle = Vect3f::ZERO;
	StartPosition = Vect3f::ZERO;
	nCamera = 0;
};

void fxlabPolarModel::Close(void)
{
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->ReleaseObject((cUnknownClass*)ModelPoint);
	IVisGeneric->Release();
	fxlabClientKeyObjectType::Close();
};

void fxlabPolarModel::Start(void)
{
	fxlabClientKeyObjectType::Start();

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	ModelPoint=(cMesh*)IVisGeneric->CreateObject(M3D_KIND(ModelID),M3D_TYPE(ModelID));
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint,&Position,&ModelAngle);
	IVisGeneric->SetObjectAttribute((cUnknownClass*)ModelPoint,CalcAttribute());

	Vect3f v1(ModelScale * KeyData[FXLAB_POLAR_MODEL_DATA_SCALE]);
	IVisGeneric->SetObjectScale((cUnknownClass*)ModelPoint,&v1);

	sColor4f c1(Color.x,Color.y,Color.z,Alpha*KeyData[FXLAB_POLAR_MODEL_DATA_ALPHA]);
	IVisGeneric->SetObjectColor((cUnknownClass*)ModelPoint,&c1);

	IVisGeneric->Release();
};

void fxlabPolarModel::Quant(void)
{
	Vect3f v,d;
	fxlabClientKeyObjectType::Quant();	
	
	if(Center){
		v.setSpherical(KeyData[FXLAB_POLAR_MODEL_DATA_PSI] + OffsetAngle,KeyData[FXLAB_POLAR_MODEL_DATA_THETTA],KeyData[FXLAB_POLAR_MODEL_DATA_RADIUS]);
		v += Center->R();
		v.z += Center->radius() * 2.0f;
		v.x = XCYCL(int(round(v.x)));
		v.y = YCYCL(int(round(v.y)));

		d.x = getDistX(v.x,StartPosition.x);
		d.y = getDistY(v.y,StartPosition.y);
		d.z = v.z - StartPosition.z;
		Position = StartPosition + d * KeyData[FXLAB_POLAR_MODEL_DATA_PHASE];
		Position.x = XCYCL(int(round(Position.x)));
		Position.y = YCYCL(int(round(Position.y)));
	};

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IVisGeneric->SetObjectPosition((cUnknownClass*)ModelPoint,&Position,&ModelAngle);

	sColor4f c1(Color.x,Color.y,Color.z,Alpha*KeyData[FXLAB_POLAR_MODEL_DATA_ALPHA]);
	IVisGeneric->SetObjectColor((cUnknownClass*)ModelPoint,&c1);

	Vect3f v1(ModelScale * KeyData[FXLAB_POLAR_MODEL_DATA_SCALE]);
	IVisGeneric->SetObjectScale((cUnknownClass*)ModelPoint,&v1);

	IVisGeneric->Release();
};

void fxlabPolarModel::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_POLAR_MODEL_DATA_MAX)
		ErrH.Abort("Bad Key of fxPolarModel");
};

void fxlabPolarModel::SetScaleVector(const Vect3f& v)
{
	ModelScale = v;
};

void fxlabPolarModel::SetAngleVector(const Vect3f& v)
{
	ModelAngle = v;
};

void fxlabPolarModel::SetColorVector(const Vect3f& v)
{
	Color = v;
};

void fxlabPolarModel::SetTransparency(float a)
{
	Alpha = a;
};

void fxlabPolarModel::SetModel(int id)
{
	ModelID = id;
};

void fxlabPolarModel::SetRacerPoint(struct mchRacer* p)
{
	Center = p;
	StartPosition = Position;
};

void fxlabPolarModel::SetAngle(float angle)
{
	OffsetAngle = angle;
};

int fxlabPolarModel::CalcAttribute(void)
{
	return (MESH_NOT_SHADE/* | MESH_LIGHTING_MAX*/ | MESH_LIGHTING_MUL | MESH_NOT_WRITEZBUFFER | MESH_NOT_LIGHTING | nCamera);
};

void fxlabPolarModel::SetCheckPointCamera(int camera)
{
	nCamera = camera;
};

//----------------------------------------------

void fxlabWaveGround::Open(void)
{
	fxlabClientEvolutionType::Open();
	PolyGrid = NULL;
	TilePoint = NULL;

	Red = 128;
	Green = 128;
	Blue = 128;
	Alpha = 128;

	DeltaRed = 127;
	DeltaGreen = 127;
	DeltaBlue = 127;
	DeltaAlpha = 127;

	DeltaHeight = 0;
	DeltaUV = 0;
};


void fxlabWaveGround::Start(void)
{
	int i,j;
	float t;
	int l;
	sPointPolyGrid *p5;

	fxlabClientEvolutionType::Start();

	t = 0;
	p5 = PolyGrid->Point;
	for(i = 0;i < PolyGrid->ysize;i++){
		for(j = 0;j < PolyGrid->xsize;j++){
			p5->Delta = FXLAB_2PI / 50;
			p5->Time = fxlabClientRND.RND(FXLAB_2PI);

			if(p5->attribute){
				l = FXLAB_COS(p5->Time);
							   
				p5->r = Red + ((DeltaRed * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
				p5->g = Green + ((DeltaGreen * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
				p5->b = Blue + ((DeltaBlue * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
				p5->a = ((Alpha + ((DeltaAlpha * l) >> FXLAB_WAVEGROUND_RANGE_POWER)) * fxlabPolyGridAlpha) >> 8;
//				p5->dz = (DeltaHeight * l) >> FXLAB_WAVEGROUND_RANGE_POWER;
				p5->dv = 127 + ((DeltaUV * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
				p5->du = 127 + ((DeltaUV * FXLAB_SIN(p5->Time)) >> FXLAB_WAVEGROUND_RANGE_POWER);
			}else{
				p5->a = 0;
				p5->r = Red;
				p5->g = Green;
				p5->b = Blue;
				p5->du = 0;
				p5->dv = 0;
//				p5->dz = 0;
			};
			p5++;
		};
	};
};

void fxlabWaveGround::SetGridPoint(void* pTileMap,void* pPolyGrid)
{
	TilePoint = (sTileMap*)pTileMap;
	PolyGrid = (cPolyGrid*)pPolyGrid;
};

void fxlabWaveGround::SetGridUV(float uv)
{ 
	DeltaUV = round(uv * 128.0f);
};

void fxlabWaveGround::Quant(void)
{
	int i;
	int l,sz;
	sPointPolyGrid *p5;

	fxlabClientEvolutionType::Quant();
	
	if(PolyGrid){
		p5 = PolyGrid->Point;
		if(TilePoint->GetVisible()){
			sz = PolyGrid->ysize * PolyGrid->xsize;
			for(i = 0;i < sz;i++){
				if(p5->attribute){
					p5->Time += p5->Delta;

					l = FXLAB_COS(p5->Time);
					p5->r = Red + ((DeltaRed * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
					p5->g = Green + ((DeltaGreen * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
					p5->b = Blue + ((DeltaBlue * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
					p5->a = ((Alpha + ((DeltaAlpha * l) >> FXLAB_WAVEGROUND_RANGE_POWER)) * fxlabPolyGridAlpha) >> 8;
//					p5->dz = (DeltaHeight * l) >> FXLAB_WAVEGROUND_RANGE_POWER;
					p5->dv = 127 + (round(DeltaUV * l) >> FXLAB_WAVEGROUND_RANGE_POWER);
					p5->du = 127 + (round(DeltaUV * FXLAB_SIN(p5->Time)) >> FXLAB_WAVEGROUND_RANGE_POWER);
				};
				p5++;
			};
		};
	};
};

//---------------------------------------------

void fxlabStreamCluster::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Num = 0;
	StreamPoint = NULL;
};

void fxlabStreamCluster::Start(void)
{
	int i;
	int key_id;
	float rate,radius,psi,delta_psi,thetta;
	Vect3f v;
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();

	Num = KeyData[FXLAB_STREAM_CLUSTER_DATA_NUM];
	StreamPoint = new fxlabProcessInterface[Num];

	key_id = round(KeyData[FXLAB_STREAM_CLUSTER_DATA_KEY_ID]);

	rate = KeyData[FXLAB_STREAM_CLUSTER_DATA_RATE];
	radius = KeyData[FXLAB_STREAM_CLUSTER_DATA_RADIUS];
	psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_PSI];
	delta_psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_DELTA_PSI] / (float)(Num);
	thetta = KeyData[FXLAB_STREAM_CLUSTER_DATA_THETTA];

	for(i = 0;i < Num;i++){
		v.setSpherical(psi,thetta,radius);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(key_id);
		t->SetPosition(Position);
		t->SetVelocity(v);
		t->SetScale(rate);
		t->SetProcessInterface(&(StreamPoint[i]));
		t->Start();
		psi += delta_psi;
	};
};

void fxlabStreamCluster::Close(void)
{
	int i;
	for(i = 0;i < Num;i++){
		if(StreamPoint[i].Process)
			StreamPoint[i].Process->SetAlive(0);
	};
	delete StreamPoint;
	fxlabClientKeyObjectType::Close();
};

void fxlabStreamCluster::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_STREAM_CLUSTER_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabStramCluster");
};

void fxlabStreamCluster::Quant(void)
{
	int i;
	float rate,radius,psi,delta_psi,thetta;
	Vect3f v;

	fxlabClientKeyObjectType::Quant();

	rate = KeyData[FXLAB_STREAM_CLUSTER_DATA_RATE];
	radius = KeyData[FXLAB_STREAM_CLUSTER_DATA_RADIUS];
	psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_PSI];
	delta_psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_DELTA_PSI] / (float)(Num);
	thetta = KeyData[FXLAB_STREAM_CLUSTER_DATA_THETTA];

	for(i = 0;i < Num;i++){
		v.setSpherical(psi,thetta,radius);
		StreamPoint[i].Process->SetVelocity(v);
		StreamPoint[i].Process->SetScale(rate);
		psi += delta_psi;
	};
};

//------------------------------------------

void fxlabClientStreamFountain::Open(void)
{
	StreamMatrix = Mat3f(0,Z_AXIS);
	fxlabStreamCluster::Open();
};

void fxlabClientStreamFountain::Quant(void)
{
	int i;
	float rate,radius,psi,delta_psi,thetta;
	Vect3f v;

	fxlabClientKeyObjectType::Quant();

	rate = KeyData[FXLAB_STREAM_CLUSTER_DATA_RATE];
	radius = KeyData[FXLAB_STREAM_CLUSTER_DATA_RADIUS];
	psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_PSI];
	delta_psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_DELTA_PSI] / (float)(Num);
	thetta = KeyData[FXLAB_STREAM_CLUSTER_DATA_THETTA];
	for(i = 0;i < Num;i++){
		v.setSpherical(psi,thetta,radius);
		v *= StreamMatrix;

		StreamPoint[i].Process->SetPosition(Position);
		StreamPoint[i].Process->SetVelocity(v);
		StreamPoint[i].Process->SetScale(rate);
		psi += delta_psi;
	};
};

void fxlabClientStreamFountain::Start(void)
{
	int i;
	int key_id;
	float rate,radius,psi,delta_psi,thetta;
	Vect3f v;
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();

	Num = KeyData[FXLAB_STREAM_CLUSTER_DATA_NUM];
	StreamPoint = new fxlabProcessInterface[Num];

	key_id = round(KeyData[FXLAB_STREAM_CLUSTER_DATA_KEY_ID]);

	rate = KeyData[FXLAB_STREAM_CLUSTER_DATA_RATE];
	radius = KeyData[FXLAB_STREAM_CLUSTER_DATA_RADIUS];
	psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_PSI];
	delta_psi = KeyData[FXLAB_STREAM_CLUSTER_DATA_DELTA_PSI] / (float)(Num);
	thetta = KeyData[FXLAB_STREAM_CLUSTER_DATA_THETTA];

	for(i = 0;i < Num;i++){
		v.setSpherical(psi,thetta,radius);
		v *= StreamMatrix;

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(key_id);
		t->SetPosition(Position);
		t->SetVelocity(v);
		t->SetScale(rate);
		t->SetProcessInterface(&(StreamPoint[i]));
		t->Start();
		psi += delta_psi;
	};
};

//------------------------------------------

void fxlabClientWildClaw::Open(void)
{	
	fxlabClientRemoteAim::Open();
   	Velocity = Vect3f::ZERO;
	PointPhase = Vect3f(M_PI * fxlabClientRND.UnitRND(),M_PI * fxlabClientRND.UnitRND(),M_PI * fxlabClientRND.UnitRND());
	Point[3] = Point[2] = Point[1] = Point[0] = Vect3f::ZERO;

	Spline.push_back(Point[0]);
	Spline.push_back(Point[1]);
	Spline.push_back(Point[2]);
	Spline.push_back(Point[3]);

	Alpha = 0;
	ScaleFactor = 1.0f;
	ErectionTimer.start(0);
};

void fxlabClientWildClaw::Start(void)
{
	int i;
	list<Vect3f>::iterator p;
	fxlabGeneralObjectType* t;

	fxlabClientRemoteAim::Start();

	CalcCenter();

	for(p = Spline.begin(),i = 0;p != Spline.end();++p,i++)
		*p = Point[i];

	t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_SPLINE_TYPE]));
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(round(KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_SPLINE_ID]));
	t->SetPosition(Position);
	t->SetSplineOwner(&Spline);
	t->SetConversion(Alpha);
	t->SetProcessInterface(&SplinePoint);
	t->Start();
};

void fxlabClientWildClaw::Close(void)
{
	if(SplinePoint.Process)
		SplinePoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientRemoteAim::Close();
};

const int FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA = 32;
const float FXLAB_CLIENT_WILD_CLAW_Z_OFFSET = 20.0f;

void fxlabClientWildClaw::Quant(void)
{
	int i;
	Vect3f v;
	list<Vect3f>::iterator p;
	fxlabGeneralObjectType* t;

	fxlabClientRemoteAim::Quant();
	CalcVelocity();

	if(Alive){
		if(Target && !ErectionTimer()){
			if(Alpha < 255 - FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime)
				Alpha += FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime;
			else
				Alpha = 255;

			if(SoundPoint.Process)
				SoundPoint.Process->SetPosition(Position);
			else{
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetSoundID(EFF_TENTACLE_ADD);
				t->SetPosition(Position);
				t->SetProcessInterface(&SoundPoint);
				t->Start();
			};
		}else{
			if(Alpha > FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime)
				Alpha -= FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime;
			else
				Alpha = 0;

			if(SoundPoint.Process)
				SoundPoint.Process->SetAlive(0);
		};
	}else{
		ScaleFactor *= 0.7f;
		Target = NULL;
		TargetID = -1;

		if(Alpha > FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime)
			Alpha -= FXLAB_CLIENT_WILD_CLAW_DELTA_ALPHA * DeltaTime;
		else
			Alpha = 0;

		if(SoundPoint.Process)
			SoundPoint.Process->SetAlive(0);
	};

	SplinePoint.Process->SetPosition(Position);
	SplinePoint.Process->SetConversion(Alpha);

	CalcCenter();

	for(p = Spline.begin(),i = 0;p != Spline.end();++p,i++)
		*p = Point[i];
};

void fxlabClientWildClaw::CalcCenter(void)
{
	Vect3f v,c;
	float z,k0,k1;
	double r;

	if(Target){
		Point[3] = getDist(Target->R(),Vect3f(Position.x,Position.y,Position.z));

		r = Point[3].norm();
		if(r > 0.001f)
			Point[3] /= r;

		r -= (r - KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_RADIUS]) * (double)(ErectionTimer()) / KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_ERECTION_TIME];
		Point[3] *= r;

		k0 = r / KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_RADIUS];
		k1 = k0 * KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS1];
		k0 *= KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS0];
	}else{
		r = KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_RADIUS];
		Point[3].x = cosf(KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_ANGLE]) * r;
		Point[3].y = sinf(KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_ANGLE]) * r;
		Point[3].z = 0;

		k0 = KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS0];
		k1 = KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS1];
	};

	r /= 3.0;
	v = (Point[3] - Point[0]) / 3;
	Point[1] = Velocity * r;
	Point[1].x += cosf(PointPhase.x)*k0;
	Point[1].y += cosf(PointPhase.y)*k0;
	Point[1].z += cosf(PointPhase.z)*k0;

	c = Point[3] - v;
	Point[2].x = c.x + sinf(PointPhase.x)*k1;
	Point[2].y = c.y + sinf(PointPhase.y)*k1;
	Point[2].z = c.z + sinf(PointPhase.z)*k1;

	PointPhase.x += KeyData[FXLAB_CLINET_WILD_CLAW_DATA_DELTA_PHASE]*fxlabClientRND.UnitRND();
	PointPhase.y += KeyData[FXLAB_CLINET_WILD_CLAW_DATA_DELTA_PHASE]*fxlabClientRND.UnitRND();
	PointPhase.z += KeyData[FXLAB_CLINET_WILD_CLAW_DATA_DELTA_PHASE]*fxlabClientRND.UnitRND();

	Point[1] *= ScaleFactor;
	Point[2] *= ScaleFactor;
	Point[3] *= ScaleFactor;

	z = (float)(fxlabGetLevel(
		XCYCL(int(round(Position.x + Point[1].x))),
		YCYCL(int(round(Position.y + Point[1].y))),
		Position.z
	));
	if(z > (Point[1].z + Position.z))
		Point[1].z = z - Position.z;

	z = (float)(fxlabGetLevel(
		XCYCL(int(round(Position.x + Point[2].x))),
		YCYCL(int(round(Position.y + Point[2].y))),
		Position.z
	));
	if(z > (Point[2].z + Position.z))
		Point[2].z = z - Position.z;

	z = (float)(fxlabGetLevel(
		XCYCL(int(round(Position.x + Point[3].x))),
		YCYCL(int(round(Position.y + Point[3].y))),
		Position.z
	));
	if(z > (Point[3].z + Position.z))
		Point[3].z = z - Position.z;

	Point[3].z += FXLAB_CLIENT_WILD_CLAW_Z_OFFSET;
	Point[2].z += FXLAB_CLIENT_WILD_CLAW_Z_OFFSET;
	Point[1].z += FXLAB_CLIENT_WILD_CLAW_Z_OFFSET;
};

void fxlabClientWildClaw::SetRemoteID(int id)
{
	if(Alive){
		if(!Target){
			fxlabClientRemoteAim::SetRemoteID(id);
			if(Target)
				ErectionTimer.start(round(KeyData[FXLAB_CLIENT_WILD_CLAW_DATA_ERECTION_TIME]));
		}else
			fxlabClientRemoteAim::SetRemoteID(id);
	};
};

void fxlabClientWildClaw::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_WILD_CLAW_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientWildClaw");
};

int fxlabClientWildClaw::GetAlive(void)
{
	if(!Alive && ScaleFactor < 0.01f)
		return 0;
	return 1;
};

//--------------------

void fxlabClientWildClawFrontLink::Open(void)
{
	fxlabClientWildClaw::Open();
	MechosOwner = NULL;
};

void fxlabClientWildClawFrontLink::CalcVelocity(void)
{
	Vect3f p;
	if(MechosOwner->completed()){
		p = MechosOwner->R();
		Position = MechosOwner->part_coords(M3D_FRONT);
		Velocity = getDist(Position,p);
		Velocity.normalize();
	};
};

void fxlabClientWildClawFrontLink::SetBody(class Body* p)
{
	MechosOwner = dynamic_cast<Mechos*>(p);
	CalcVelocity();
};

//------------------------------------------


void fxlabClientBowMachine::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientRemoteAim::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_LIGHTNING_SPARK);
	t->SetPosition(Position);
	t->SetProcessInterface(&SparkPoint);
	t->Start();	
};

void fxlabClientBowMachine::Close(void)
{
	if(ActivePoint.Process)
		ActivePoint.Process->SetAlive(0);

	if(PassivePoint.Process)
		PassivePoint.Process->SetAlive(0);

	if(SparkPoint.Process)
		SparkPoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientRemoteAim::Close();
};

void fxlabClientBowMachine::Quant(void)
{
	fxlabClientRemoteAim::Quant();
	CalcVelocity();

	if(SparkPoint.Process)
		SparkPoint.Process->SetPosition(Position);

	if(Target)
		EnablePhase();
	else
		DisablePhase();
};

void fxlabClientBowMachine::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_BOW_MACHINE_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientBowMachine");
};

void fxlabClientBowMachine::EnablePhase(void)
{
	Vect3f v;
	fxlabGeneralObjectType* t;

	if(PassivePoint.Process)
		PassivePoint.Process->SetAlive(0);

	v = getDist(Target->R() + Vect3f(0,0,20.0f),Vect3f(Position.x,Position.y,Position.z));

	if(!ActivePoint.Process){
		t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_BOW_MACHINE_DATA_ACTIVE_TYPE]));
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(round(KeyData[FXLAB_CLIENT_BOW_MACHINE_DATA_ACTIVE_ID]));
		t->SetPosition(Position);
		t->SetVelocity(v);
		t->SetProcessInterface(&ActivePoint);
		t->Start();
	}else{
		ActivePoint.Process->SetPosition(Position);
		ActivePoint.Process->SetVelocity(v);
	};
};

void fxlabClientBowMachine::DisablePhase(void)
{
	fxlabGeneralObjectType* t;

	if(ActivePoint.Process)
		ActivePoint.Process->SetAlive(0);

	if(!PassivePoint.Process){
		t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_BOW_MACHINE_DATA_PASSIVE_TYPE]));
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(round(KeyData[FXLAB_CLIENT_BOW_MACHINE_DATA_PASSIVE_ID]));
		t->SetPosition(Position);
		t->SetProcessInterface(&PassivePoint);
		t->Start();
	}else
		PassivePoint.Process->SetPosition(Position);
};

//------------------

void fxlabClientBowMachineFrontLink::Open(void)
{
	fxlabClientBowMachine::Open();
	MechosOwner = NULL;
};

void fxlabClientBowMachineFrontLink::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientBowMachine::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_STATIC_ELECTRICITY);
	t->SetPosition(Position);
	t->SetProcessInterface(&BackgroundPoint);
	t->Start();
};

void fxlabClientBowMachineFrontLink::Close(void)
{
	if(BackgroundPoint.Process)
		BackgroundPoint.Process->SetAlive(0);

	fxlabClientBowMachine::Close();
};

void fxlabClientBowMachineFrontLink::CalcVelocity(void)
{
	if(MechosOwner->completed()){
		Position = MechosOwner->part_coords(M3D_FRONT);
		if(BackgroundPoint.Process)
			BackgroundPoint.Process->SetPosition(Position);
	};
};

void fxlabClientBowMachineFrontLink::SetBody(class Body* p)
{
	MechosOwner = dynamic_cast<Mechos*>(p);
	CalcVelocity();
};

void fxlabClientBowMachineFrontLink::EnablePhase(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientBowMachine::EnablePhase();
	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
	else{
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_STATIC_ELECTRICITY_ADD);
		t->SetPosition(Position);
		t->SetProcessInterface(&SoundPoint);
		t->Start();
	};
};

void fxlabClientBowMachineFrontLink::DisablePhase(void)
{
	fxlabClientBowMachine::DisablePhase();
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
};

//----------------------------

void fxlabClientLightningSeed::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientBowMachine::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_ELM);
	t->SetPosition(Position);
	t->SetProcessInterface(&BackgroundPoint);
	t->Start();
};

void fxlabClientLightningSeed::Close(void)
{
	if(BackgroundPoint.Process)
		BackgroundPoint.Process->SetAlive(0);

	fxlabClientBowMachine::Close();
};

void fxlabClientLightningSeed::EnablePhase(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientBowMachine::EnablePhase();
	if(!SoundPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_STATIC_ELECTRICITY_ADD);
		t->SetPosition(Position);
		t->SetProcessInterface(&SoundPoint);
		t->Start();
	};
};

void fxlabClientLightningSeed::DisablePhase(void)
{
	fxlabClientBowMachine::DisablePhase();
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
};


//-----------------------

void fxlabClientBallWindLink::Open(void)
{
	fxlabClientKeyObjectType::Open();
	MechosOwner = NULL;
	Position = Vect3f::ZERO;
	Velocity = Vect3f::ZERO;
};

void fxlabClientBallWindLink::Close(void)
{
	if(ActivePoint.Process)
		ActivePoint.Process->SetAlive(0);

	if(PassivePoint.Process)
		PassivePoint.Process->SetAlive(0);

	fxlabClientKeyObjectType::Close();
};

void fxlabClientBallWindLink::Start(void)
{
	fxlabClientKeyObjectType::Start();
	StartTimer.start(500);
};

void fxlabClientBallWindLink::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	CalcVelocity();

	if(MechosOwner->completed()){
		Velocity = getDist(Position,Vect3f(MechosOwner->part_coords(M3D_FRONT)));
		Velocity.Normalize(KeyData[FXLAB_CLIENT_BALL_WIND_DATA_SPEED]);
	};		

	if(!StartTimer()){
		if(PassivePoint.Process)
			PassivePoint.Process->SetAlive(0);

		if(!ActivePoint.Process)
			CreateActive();
		else{
			ActivePoint.Process->SetPosition(Position);
			ActivePoint.Process->SetVelocity(Velocity);
		};
	}else{
		if(ActivePoint.Process)
			ActivePoint.Process->SetAlive(0);

		if(!PassivePoint.Process)
			CreatePassive();
		else{
			PassivePoint.Process->SetPosition(Position);
			PassivePoint.Process->SetVelocity(Velocity);
		};
	};
};

void fxlabClientBallWindLink::SetBody(class Body* p)
{
	MechosOwner = dynamic_cast<Mechos*>(p);
	CalcVelocity();
};

void fxlabClientBallWindLink::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_BALL_WIND_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientBallWindLink");
};

void fxlabClientBallWindLink::CreateActive(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BALL_WIND_ACTIVE_FIRE);
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&ActivePoint);
	t->Start();
};

void fxlabClientBallWindLink::CreatePassive(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BALL_WIND_PASSIVE_FIRE);
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&PassivePoint);
	t->Start();
};


void fxlabClientBallWindLinkLeft::CalcVelocity(void)
{
	if(MechosOwner->completed())
		Position = MechosOwner->part_coords(M3D_LF_WHEEL);
};

void fxlabClientBallWindLinkRight::CalcVelocity(void)
{
	if(MechosOwner->completed())
		Position = MechosOwner->part_coords(M3D_RF_WHEEL);
};


//-----------------------------------------------------------------------

void fxlabClientVoodooMaster::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Angle = 0;
};

void fxlabClientVoodooMaster::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_MASTER_VOODOO);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientVoodooMaster::Close(void)
{
	int i;
	for(i = 0;i < FXLAB_CLIENT_VOODOO_HEAD_NUM;i++){
		if(HeadPoint[i].Process)
			HeadPoint[i].Process->SetAlive(0);
		if(TailPoint[i].Process)
			TailPoint[i].Process->SetAlive(0);
		if(OmniPoint[i].Process)
			OmniPoint[i].Process->SetAlive(0);
	};
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientKeyObjectType::Close();
};

void fxlabClientVoodooMaster::Quant(void)
{
	int i;
	float a,da;
	Vect3f n,p;
	MatXf mat;
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Quant();

	a = Angle;

	da = 2.0f * M_PI / (float)(FXLAB_CLIENT_VOODOO_HEAD_NUM);

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);

	for(i = 0;i < FXLAB_CLIENT_VOODOO_HEAD_NUM;i++){
		p.x = Position.x + KeyData[FXLAB_CLIENT_VOODOO_MASTER_DATA_RADIUS] * cosf(a);
		p.y = Position.y + KeyData[FXLAB_CLIENT_VOODOO_MASTER_DATA_RADIUS] * sinf(a);
		p.z = Position.z;
		CYCLE(p);

		n.x = cosf(a - M_PI * 0.5f);
		n.y = sinf(a - M_PI * 0.5f);
		n.z = 0;

		mat.rot() = Mat3f(M_PI - a,Z_AXIS);
		mat.trans() = p;

		if(HeadPoint[i].Process){
			HeadPoint[i].Process->SetMatrix(mat);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_VOODOO_HEAD_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_MODEL);
			t->SetPosition(p);
			t->SetProcessInterface(&HeadPoint[i]);
			t->Start();
		};

		if(TailPoint[i].Process){
			TailPoint[i].Process->SetPosition(p);
			TailPoint[i].Process->SetVelocity(n);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_TAIL);
			t->SetPosition(p);
			t->SetVelocity(n);
			t->SetProcessInterface(&TailPoint[i]);
			t->Start();
		};

		if(OmniPoint[i].Process){
			OmniPoint[i].Process->SetPosition(p);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_OMNI);
			t->SetPosition(p);
			t->SetProcessInterface(&OmniPoint[i]);
			t->Start();
		};
		a += da;
	};
	Angle += KeyData[FXLAB_CLIENT_VOODOO_MASTER_DATA_DELTA_ANGLE] * DeltaTime;
};

void fxlabClientVoodooMaster::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_VOODOO_MASTER_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientVoodooMaster");
};


//------------------------------------------------


void fxlabTerrainDeformator::Quant(void)
{
	int i,j;
	int r,rmax2,rmin2;
	int x,y,d;
	int dx,dy;
	float d2;
	sPointTile* n;
	sTileMap* s;
	int xp,yp;
	float a;	

	fxlabClientKeyObjectType::Quant();

	x = round(Position.x);
	y = round(Position.y);

	a = KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_AMPLITUDE];

	r = round(KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_RADIUS] + KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_DELTA]);
	rmax2 = r * r;
	rmin2 = round(KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_RADIUS] - KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_DELTA]);
	rmin2 *= rmin2;

	d2 = 1.0f / (KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_DELTA]*KeyData[FXLAB_TERRAIN_DEFORMATOR_DATA_DELTA]);

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	cTileMap *tMap=(cTileMap*)IVisGeneric->GetTileMap();
	if(tMap==0) { IVisGeneric->Release(); return; }
	for(i = 0;i < tMap->NumberTileX()*tMap->NumberTileY();i++){
		s = tMap->GetTile(i,0);
		if(s->Point.length()<=0) continue;
		n = &(s->Point[0]);
		xp = s->xPos;
		yp = s->yPos;

		for(j = 0;j < s->Point.length();j++){
			dx = getDistX(XCYCL(n->xw + xp),x);
			if(dx < r){
				dy = getDistY(YCYCL(n->yw + yp),y);
				if(dy < r){
					d = dx*dx + dy*dy;
					if(d < rmax2 && d > rmin2){
						n->dz += a * sinf(M_PI * (float)(d - rmin2) * d2);
					};
				};
			};
			n++;
		};
	};
	IVisGeneric->Release();
};

void fxlabTerrainDeformator::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_TERRAIN_DEFORMATOR_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabTerrainDeformator");
};

//----------------------

void fxlabClientRevoltSpaceLink::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientMechosLinkType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_MAGNETIC_CUSHION);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientRevoltSpaceLink::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabClientMechosLinkType::Close();
};

void fxlabClientRevoltSpaceLink::CalcVelocity(void)
{
	if(Core->completed()){
		Position = Core->R();
		Velocity = Vect3f(Core->width(),Core->length(),Core->radius());
		if(SoundPoint.Process)
			SoundPoint.Process->SetPosition(Position);
	};
};

void fxlabClientRevoltSpaceLink::Quant(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Quant();
	if(Alive){
		if(Core){
			if(!Core->alive()){
				Destroy();
				Alive = 0;
			}else{
				if(!Core->completed()){
					Destroy();
					Alive = 0;
				}else{
					CalcVelocity();
					if(Velocity.norm() < KeyData[FXLAB_CLIENT_BODY_LINK_DATA_BORDER_SPEED]){
						if(TailPoint.Process)
							TailPoint.Process->SetAlive(0);
					}else{
						if(TailID >= 0){
							if(!TailPoint.Process){
								t = fxlabClientD->CreateObject(TailType);
								t->SetStartTime(fxlabClientD->GetTime());

								MatXf m1(Core->Alg(),Core->R());
								t->SetMatrix(m1);

								t->SetVelocity(Velocity);
								t->SetKeyID(TailID);
								t->SetProcessInterface(&TailPoint);
								t->Start();
							}else{
								MatXf m1(Core->Alg(),Core->R());
								TailPoint.Process->SetMatrix(m1);

								TailPoint.Process->SetVelocity(Velocity);
							};
						};
					};
				};
			};
		}else Destroy();
	}else{
		if(Core && Core->alive())
			Destroy();
	};
};

//------------------------------------------------

void fxlabClientMassShifter::Quant(void)
{
	int n,i;
	float num;
	float a,da;
	Vect3f v;
	Mat3f m;
	float r;

	fxlabClientKeyObjectType::Quant();

	r = KeyData[FXLAB_CLIENT_MASS_SHIFTER_DATA_RATE] * DeltaTime;
	num = floor(KeyData[FXLAB_CLIENT_MASS_SHIFTER_DATA_RATE]*r);
	if(fxlabClientRND.UnitRND() < r - num)
		num++;
	if(num < 0.01)
		return;
	n = round(num);

	a = fxlabClientRND.UnitRND() * M_PI * 2.0f;
	da = 2.0f * M_PI / num;

	for(i = 0;i < n;i++){
		v = Position;
		v.x += cosf(a) * KeyData[FXLAB_CLIENT_MASS_SHIFTER_DATA_RADIUS];
		v.y += sinf(a) * KeyData[FXLAB_CLIENT_MASS_SHIFTER_DATA_RADIUS];
		CYCLE(v);
		v.z = fxlabGetLevel(v.x,v.y,v.z);
		m = /*getPlacementPose(Position).rot() * */Mat3f(a + M_PI * 0.5f,Z_AXIS);
		Generate(m,v);
		a += da;
	};
};

void fxlabClientMassShifter::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_MASS_SHIFTER_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientMassShifter");
};

void fxlabClientMassShifter::Generate(const Mat3f& m,const Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MASS_SHIFTER_MODEL);
	t->SetPosition(v);

	MatXf m1(m,v);
	t->SetMatrix(m1);

	t->Start();
};

void fxlabClientMassShifterNitro::Generate(const Mat3f& m,const Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MASS_SHIFTER_MODEL_NITRO);
	t->SetPosition(v);

	MatXf m1(m,v);
	t->SetMatrix(m1);

	t->Start();
};

void fxlabClientMassShifterCharacter::Generate(const Mat3f& m,const Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MASS_SHIFTER_MODEL_CHARACTER);
	t->SetPosition(v);

	MatXf m1(m,v);
	t->SetMatrix(m1);

	t->Start();
};

void fxlabClientMassShifterMovie::Generate(const Mat3f& m,const Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MASS_SHIFTER_MODEL_MOVIE);
	t->SetPosition(v);

	MatXf m1(m,v);
	t->SetMatrix(m1);

	t->Start();
};

//-----------------------

void fxlabClientJumpActionLink::Start(void)
{
	fxlabClientActionType::Start();
	ModelCount = 0;
};


void fxlabClientJumpActionLink::Quant(void)
{
	int i;
	int cnt;

	fxlabClientKeyObjectType::Quant();
	if(Alive){
		if(Core){
			if(!Core->alive()){
				Destroy();
				Alive = 0;
			}else{
				if(!Core->completed() || (MechosOwner->features() & Mechos::STATIC_NULIFICATOR)){
					Destroy();
					Alive = 0;
				}else{
					CalcVelocity();
					
					cnt = 0;
					for(i = 0;i < FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL;i++){
						if(ModelData[i].Process){
							ModelData[i].Process->SetMatrix(zMatrix);
							ModelData[i].Process->SetVelocity(zVelocity);
							cnt++;
						};
					};
					
					if(ModelCount < FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL){
						if(!DelayTimer()){
							CreateModel();
							ModelCount++;
						};
					}else{
						if(!cnt)
							SetAlive(0);
					};
				};
			};
		}else Destroy();
	}else{
		if(Core && Core->alive())
			Destroy();
	};
};

void fxlabClientJumpActionLink::CalcVelocity(void)
{
	fxlabClientActionType::CalcVelocity();
	zMatrix = MechosOwner->geometry -> GlobalMatrix;
	zMatrix.trans().z -= MechosOwner->radius();
	zVelocity = -MechosOwner->Zglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
};

void fxlabClientJumpActionLink::Close(void)
{
	int i;
	for(i = 0;i < FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL;i++){
		if(ModelData[i].Process)
			ModelData[i].Process->SetAlive(0);
	};

	fxlabClientActionType::Close();
};

void fxlabClientJumpActionLink::CreateModel(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(TailType);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(TailID);
	t->SetMatrix(zMatrix);
	t->SetVelocity(zVelocity);
	t->SetProcessInterface(&(ModelData[ModelCount]));
	t->Start();
//	DelayTimer.start(300);
	DelayTimer.start(150);
};

//---------------------------------------

void fxlabClientNitroActionLink::CalcVelocity(void)
{
	fxlabClientActionType::CalcVelocity();
	zMatrix = MechosOwner->geometry -> GlobalMatrix;
	zMatrix.trans() -= MechosOwner->Yglobal() * MechosOwner->radius();
	zMatrix.trans() = CYCLE(zMatrix.trans());
	zVelocity = -MechosOwner->Yglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
};

void fxlabClientNitroActionLink::CreateModel(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(TailType);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(TailID);
	t->SetMatrix(zMatrix);
	t->SetVelocity(zVelocity);
	t->SetProcessInterface(&(ModelData[ModelCount]));
	t->Start();	
	DelayTimer.start(80);
};

//--------------------------------------

void fxlabClientActionType::KeyCheck(void)
{	
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_ACTION_TYPE_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientActionType");
};

void fxlabClientJumpActionLeft::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_LB_WHEEL);
		Velocity = -Core->Zglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
	};
};

void fxlabClientJumpActionRight::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_RB_WHEEL);
		Velocity = -Core->Zglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
	};
};

void fxlabClientNitroActionLeft::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_LB_WHEEL);
		Velocity = -Core->Yglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
	};
};

void fxlabClientNitroActionRight::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_RB_WHEEL);
		Velocity = -Core->Yglobal() * KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED];
	};
};

//---------------------------

void fxlabClientMechosFire::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientActionType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_FIRE);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientMechosFire::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabClientActionType::Close();
};

void fxlabClientMechosFire::CalcVelocity(void)
{
	float d;
	if(Core->completed()){
		Position = Core->R();
		Velocity = -Core->Vglobal();
		d = Velocity.norm();
		if(d > 0.001f)
			Velocity *= KeyData[FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED] / d;
		Velocity += Vect3f(0,0,3.0f);

		if(SoundPoint.Process)
			SoundPoint.Process->SetPosition(Position);
	};
};

//-----------------------------


void fxlabClientMechosController::Open(void)
{
	fxlabClientEvolutionType::Open();
	Owner = NULL;
};

extern int mchCurrentWorld;

float fxlabGetTrackWaveRed(void);
float fxlabGetTrackWaveGreen(void);
float fxlabGetTrackWaveBlue(void);
float fxlabGetTrackWaveAlpha(void);


void fxlabClientMechosController::Quant(void)
{
	float r,g,b,a;
	float s;
	fxlabGeneralObjectType* t;
	Vect3f v;
	int i;
	cMesh* mp;
	fxlabMechosColorInformation* cp;
	float n;
	const int mesh_id[NUM_PARTS] = { M3D_ENGINE,M3D_FRONT,M3D_RF_WHEEL,M3D_RB_WHEEL,M3D_LF_WHEEL,M3D_LB_WHEEL};
	int tx,ty;

	fxlabClientEvolutionType::Quant();

	s = round(Owner->Vglobal().norm());
	if(s > 10) s = 10;

	if(Owner->submersion() > 0.15f){
		if(mchCurrentWorld != 4){
//			Owner->fxlabMechosInfo.WaterEnable = 1;
			if(Owner->completed()){
				if(!fxlabClientRND.RND(11 - s)){
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_MECHOS_BUBBLE);

					Vect3f v1(Owner->R());
					t->SetPosition(v1);

					t->Start();
				};
				
				if(!WaveTimer()){
					v = Owner->part_coords(M3D_RB_WHEEL);
					v -= Owner->Yglobal() * Owner->radius();
					CYCLE(v);

					tx = XCYCL(int(round(v.x)));
					ty = YCYCL(int(round(v.y)));
					if(GetAt(tx,ty) & At_WATER){
						t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
						t->SetStartTime(fxlabClientD->GetTime());
						t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);

						Vect3f v1(v.x,v.y,vMap->LevelH2O + 3.0f);
						t->SetPosition(v1);

						Vect3f v2(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue());
						t->SetColorVector(v2);

						t->SetTransparency(fxlabGetTrackWaveAlpha());
						t->Start();
					};

					v = Owner->part_coords(M3D_LB_WHEEL);
					v -= Owner->Yglobal() * Owner->radius();
					CYCLE(v);
					tx = XCYCL(int(round(v.x)));
					ty = YCYCL(int(round(v.y)));

					if(GetAt(tx,ty) & At_WATER){
						t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
						t->SetStartTime(fxlabClientD->GetTime());
						t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);

						Vect3f v1(v.x,v.y,vMap->LevelH2O + 3.0f);
						t->SetPosition(v1);

						Vect3f v2(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue());
						t->SetColorVector(v2);
						t->SetTransparency(fxlabGetTrackWaveAlpha());
						t->Start();
					};

					WaveTimer.start(100 * (11 - s));
				};
			};
		};
/*	}else{
		if(mchCurrentWorld != 4)
			Owner->fxlabMechosInfo.WaterEnable = 0;*/
	};

	if(Owner->Energy() < Owner->energy_max() * 0.33f && !(Owner->features() & Mechos::NON_DESTRUCTING)){
		if(!DamageWarningPoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_LINK);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_DAMAGE_WARNING_LINK);

			Vect3f v1(Owner->R());
			t->SetPosition(v1);

			t->SetBody(Owner);
			t->SetProcessInterface(&DamageWarningPoint);
			t->Start();
		};
	}else{
		if(DamageWarningPoint.Process)
			DamageWarningPoint.Process->SetAlive(0);
	};

	if(Owner->fxlabMechosInfo.EnergyTimer()){
		if(!EnergyPoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_LINK);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_CHARGE_ENERGY_CYCLE_LINK);
			t->SetBody(Owner);
			t->SetProcessInterface(&EnergyPoint);
			t->Start();

			if(Owner->active()){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetSoundID(EFF_BONUS_ENERGY);

				Vect3f v1(Owner->R());
				t->SetPosition(v1);

				t->Start();
			};
		};
	}else{
		if(EnergyPoint.Process)
			EnergyPoint.Process->SetAlive(0);
	};

	if(Owner->fxlabMechosInfo.ManaTimer()){
		if(!ManaPoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_LINK);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_CHARGE_MANA_CYCLE_LINK);
			t->SetBody(Owner);
			t->SetProcessInterface(&ManaPoint);
			t->Start();

			if(Owner->active()){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetSoundID(EFF_BONUS_SPELL);

				Vect3f v1(Owner->R());
				t->SetPosition(v1);

				t->Start();
			};
		};
	}else{
		if(ManaPoint.Process)
			ManaPoint.Process->SetAlive(0);
	};

	if(Owner->completed() && Owner->geometry){
		cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
		for(i = 0;i < NUM_PARTS;i++){
			mp = Owner->part_by_index(i);
			if(mp){
				cp = Owner->fxlabMechosInfo.ColorData + i;

/*				if(Owner -> alpha < 0.99f){
					cp->RGBACount++;
					cp->Alpha += Owner -> alpha;
				};*/
					
				if(cp->RGBACount){
					n = (float)(cp->RGBACount);
					r = cp->Red / n;
					g = cp->Green / n;
					b = cp->Blue / n;
					a = cp->Alpha / n;

/*					if(!(cp->ColorEnable)){
						IVisGeneric->GetObjectColor((cUnknownClass*)(mp),&(cp->Color0),&(cp->Color1),mesh_id[i]);
//						Owner->getColor(i,&(cp->Color0),&(cp->Color1));
					};
					IVisGeneric->SetObjectColor((cUnknownClass*)(mp),&sColor4f(cp->Color0.r,cp->Color0.g,cp->Color0.b,a),&sColor4f(r,g,b,cp->Color1.a),mesh_id[i]);
					Owner->setColor(i,&sColor4f(cp->Color0.r,cp->Color0.g,cp->Color0.b,a),&sColor4f(r,g,b,cp->Color1.a));*/
					sColor4f c1(1.0f,1.0f,1.0f,a);
					sColor4f c2(r,g,b,1.0f);
					Owner->setColor(i,&c1,&c2);

					cp->ColorEnable = 1;
					cp->Red = 0;
					cp->Green = 0;
					cp->Blue = 0;
					cp->RGBACount = 0;
					cp->Alpha = 0;
				}else{
					if(cp->ColorEnable){
						cp->ColorEnable = 0;
						cp->Red = 0;
						cp->Green = 0;
						cp->Blue = 0;
						cp->RGBACount = 0;
						cp->Alpha = 0;
//						IVisGeneric->SetObjectColor((cUnknownClass*)(mp),&(cp->Color0),&(cp->Color1),mesh_id[i]);
//						Owner->setColor(i,&(cp->Color0),&(cp->Color1));
						sColor4f c1(1.0f,1.0f,1.0f,1.0f);
						sColor4f c2(0,0,0,1.0f);
						Owner->setColor(i,&c1,&c2);
					};
				};
			};
		};
		IVisGeneric->Release();
	};
};

void fxlabClientMechosController::Close(void)
{
	if(DamageWarningPoint.Process)
		DamageWarningPoint.Process->SetAlive(0);

	if(EnergyPoint.Process){
		EnergyPoint.Process->SetAlive(0);
	};

	if(ManaPoint.Process){
		ManaPoint.Process->SetAlive(0);
	};

	fxlabClientEvolutionType::Close();
};

//------------------------------

void fxlabClientColorController::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Owner = NULL;
};


void fxlabClientColorController::Quant(void)
{
	int i;
	fxlabClientKeyObjectType::Quant();

	Position = Owner->R();
	if(Owner->completed() && Owner->geometry){
		for(i = 0;i < NUM_PARTS;i++){
			Owner->fxlabMechosInfo.ColorData[i].Red += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_RED];
			Owner->fxlabMechosInfo.ColorData[i].Green += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_GREEN];
			Owner->fxlabMechosInfo.ColorData[i].Blue += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_BLUE];
			Owner->fxlabMechosInfo.ColorData[i].Alpha += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_ALPHA];
			Owner->fxlabMechosInfo.ColorData[i].RGBACount++;
		};
	}else SetAlive(0);
};

void fxlabClientColorController::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_COLOR_CONTROLLER_DATA_MAX)
		ErrH.Abort("Bad Key in fxlabClientColorContoller");
};


//-------------------------------

void fxlabClientPartController::Open(void)
{
	fxlabClientColorController::Open();
	ControllerPartID = 0;
	ControllerArcaneID = 0;
};

/*
void fxlabClientPartController::CalcKeyTime(void)
{
	cMesh* mp;
	if(Owner){
		mp = Owner->part_by_index(ControllerArcaneID);
		if(mp && mp->arcaneData.Status && mp->arcaneData.Power >= mp->arcaneData.ActivatePower){
//			KeyTime = KeyPoint->GetScale() * (mp->arcaneData.Power / mp->arcaneData.ActivatePower);
			KeyTime = KeyPoint->GetScale();
		}else
			KeyTime = 0;
	}else KeyTime = 0;
};
*/

void fxlabClientPartController::Quant(void)
{
	cMesh* mp;
	cMesh* ap;

	fxlabClientKeyObjectType::Quant();

	Position = Owner->R();
	if(Owner && Owner->completed() && Owner->geometry){
		ap = Owner->part_by_index(ControllerArcaneID);
		if(ap && ap->arcaneData.Status && ap->arcaneData.Power >= ap->arcaneData.ActivatePower){
			mp = Owner->part_by_index(ControllerPartID);
			if(mp){
				Owner->fxlabMechosInfo.ColorData[ControllerPartID].Red += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_RED];
				Owner->fxlabMechosInfo.ColorData[ControllerPartID].Green += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_GREEN];
				Owner->fxlabMechosInfo.ColorData[ControllerPartID].Blue += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_BLUE];
				Owner->fxlabMechosInfo.ColorData[ControllerPartID].Alpha += KeyData[FXLAB_CLIENT_COLOR_CONTROLLER_DATA_ALPHA];
				Owner->fxlabMechosInfo.ColorData[ControllerPartID].RGBACount++;
			};
		}else StartTime = fxlabClientD->GetTime();
	}else StartTime = fxlabClientD->GetTime();
};

//-------------------------------

void fxlabClientTeleportColorController::Start(void)
{
	fxlabClientColorController::Start();
	RestartTime = KeyPoint->GetScale() * 0.5f / fxlabGlobalTimeRate;
};

int fxlabClientTeleportColorController::GetAlive(void)
{
	if(!Alive && KeyTime >= KeyPoint->GetScale())
		return 0;
	return 1;
};

void fxlabClientTeleportColorController::CalcKeyTime(void)
{
	if(Alive){
		if(KeyTime < KeyPoint->GetScale() * 0.5f)
			fxlabClientColorController::CalcKeyTime();
		else
			StartTime = fxlabClientD->GetTime() - RestartTime;
	}else{
		fxlabClientColorController::CalcKeyTime();
	};
};

//-------------------------------

void fxlabClientTeleportOmniController::Start(void)
{
	fxlabControlOmni::Start();
	RestartTime = KeyPoint->GetScale() * 0.5f / fxlabGlobalTimeRate;
};

int fxlabClientTeleportOmniController::GetAlive(void)
{
	if(!Alive && KeyTime >= KeyPoint->GetScale())
		return 0;
	return 1;
};

void fxlabClientTeleportOmniController::CalcKeyTime(void)
{
	if(Alive){
		if(KeyTime < KeyPoint->GetScale() * 0.5f)
			fxlabControlOmni::CalcKeyTime();
		else
			StartTime = fxlabClientD->GetTime() - RestartTime;
	}else{
		fxlabControlOmni::CalcKeyTime();
	};
};


//---------------------

void fxlabClientHeadDustLauncher::CreateObject(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientObjectLauncher::CreateObject();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_BREATH);
	t->SetPosition(Position);
	t->Start();
};

//-----------------------------

void fxlabClientShaftMachineLink::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientFrontLink::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_X_RENTGEN);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();

};

void fxlabClientShaftMachineLink::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabClientFrontLink::Close();
};

void fxlabClientShaftMachineLink::CalcVelocity(void)
{
//	fxlabClientFrontLink::CalcVelocity();
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_FRONT);
		Velocity = Vect3f(0,1.0f,0.15f) * MechosOwner->Alg();
		Velocity.normalize();
	};

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
};

//----------------------

void fxlabClientDragonFireSwitcher::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabClientRemoteSwitcher::Close();
};

void fxlabClientDragonFireSwitcher::CreateObject(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientRemoteSwitcher::CreateObject();

	if(!SoundPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_FLYING_DRAGON1);
		t->SetPosition(Position);
		t->SetProcessInterface(&SoundPoint);
		t->Start();
	};
};

void fxlabClientDragonFireSwitcher::DestroyObject(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabClientRemoteSwitcher::DestroyObject();
};

//---------------

void fxlabClientDragonHeadFireSwitcher::CreateObject(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientRemoteSwitcher::CreateObject();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_DRAGON);
	t->SetPosition(Position);
	t->Start();
};

//--------------------------------------------

#include "VisGeneric.h"
#include "Camera.h"
#include "Scene.h"

void fxlabClientWorldIrradiate::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Owner = NULL;
};

void fxlabClientWorldIrradiate::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TERRAIN_DEFORMATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE_DEFORMATION);
	t->SetPosition(Position);
	t->SetProcessInterface(&DeformationPoint);
	t->Start();

	FireTimer.start(300);

	Angle -= Owner->psi() + M_PI;

	FireEnable = 1;
};

void fxlabClientWorldIrradiate::Close(void)
{
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	if(WavePoint.Process)
		WavePoint.Process->SetAlive(0);

	if(DeformationPoint.Process)
		DeformationPoint.Process->SetAlive(0);
	
	if(Owner->active())
		camera_dispatcher -> setObserver(Owner);

	fxlabClientKeyObjectType::Close();
};

void fxlabClientWorldIrradiate::Quant(void)
{
	Vect3f v;
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Quant();

	v.x = KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_X];
	v.y = KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_Y];
	v.z = KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_Z] + Angle;

	camera_dispatcher -> setStaticObserver(Position,v,KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_RADIUS],0,1000);

	if(FireEnable && !FireTimer()){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE_WAVE);
		t->SetPosition(Position);
		t->SetProcessInterface(&WavePoint);
		t->Start();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE_OMNI);
		t->SetPosition(Position);
		t->SetProcessInterface(&OmniPoint);
		t->Start();

		FireEnable = 0;
	};
};

void fxlabClientWorldIrradiate::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_WORLD_IRRADIATE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabClientWorldIrradiate");
};


void fxlabClientWorldIrradiate::Show(void)
{
	int i;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	cCamera* camera;

	IVisGeneric=CreateIVisGeneric();
	scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
	assert(scene);
	for(i = 0;i < scene->GetNumberCamera();i++){
		camera = scene->GetCamera(i);
		sColor4f c1(
			KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_RED],
			KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_GREEN],
			KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_BLUE],
			KeyData[FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ALPHA]
		);
		sColor4f c2(0,0,0,0);
		P3D->SetViewColor(camera,c1,c2,1);
	};
	IVisGeneric->Release();
};

//-------------------------------------

void fxlabClientSetFaceFire::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientSetFaceFire::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);	
//	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_HEAD_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&FirePoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
//	t->SetSoundID(EFF_BOSS_LAVA_BELCH);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientSetFaceFire::Close(void)
{
	if(FirePoint.Process)
		FirePoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientKeyObjectType::Close();
};

void fxlabClientSetFaceFire::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	if(FirePoint.Process){
		FirePoint.Process->SetPosition(Position);
		FirePoint.Process->SetVelocity(Velocity);
	};

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
};

void fxlabClientSetFaceFire::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_SET_FACE_FIRE_MAX)
		ErrH.Abort("Bad Key of fxlabClientSetFaceFire");	
};

void fxlabClientSetFaceFire::SetVelocity(const Vect3f& v)
{
	if(KeyPoint){
		Velocity = v;
		Velocity.normalize(KeyData[FXLAB_CLIENT_SET_FACE_FIRE_SPEED]);
	}else {
		Velocity = v;
		Velocity.normalize();
	}
};

//--------------------------------------------------------

void fxlabClientMovieBowRepeater::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientMovieBowRepeater::Start(void)
{
	fxlabClientKeyObjectType::Start();
	CreateBow();
	Timer.start();
};

void fxlabClientMovieBowRepeater::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	if(Timer() >= KeyData[FXLAB_CLIENT_MOVIE_BOW_REPEATER_SPEED]){
		CreateBow();
		Timer.start();
	};
};

void fxlabClientMovieBowRepeater::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_MOVIE_BOW_REPEATER_MAX)
		ErrH.Abort("Bad Key of fxlabClientMovieBowRepeater");
};

void fxlabClientMovieBowRepeater::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

void fxlabClientMovieBowRepeater::CreateBow(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_RADIUS);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_STORM_BOW);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_STORM_LIGHT);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->Start();
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_SPARK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);

	Vect3f v1(0,0,0);
	t->SetVelocity(v1);

	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
//	t->SetSoundID(EFF_INTRO_THUNDERSTORM);
	t->SetPosition(Position);
	t->Start();
};

//---------------------------------------------

void fxlabClientMovieWaveRepeater::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientMovieBowRepeater::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_SPRAY_STREAM);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);

	Vect3f v1(0,0,15.0f);
	t->SetVelocity(v1);

	t->Start();
	Cnt = 0;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
//	t->SetSoundID(EFF_INTRO_DUCK);
	t->SetPosition(Position);
	t->Start();
};

void fxlabClientMovieWaveRepeater::CreateBow(void)
{
	fxlabGeneralObjectType* t;

	Cnt--;
	if(Cnt <= 0){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_MOVIE_BUBBLE);
		t->SetPosition(Position);
		t->Start();
		Cnt = 3;
	};

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);
	t->SetPosition(Position);

	Vect3f v1(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue());
	t->SetColorVector(v1);

	t->SetTransparency(fxlabGetTrackWaveAlpha());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
//	t->SetSoundID(EFF_INTRO_SWIMMING);
	t->SetPosition(Position);
	t->Start();
};

//----------------------------------------

void fxlabClientMovieQuandLight::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientSpaceType::Start();

/*	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_ELM);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_QUAD_SPARK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);

	Vect3f v1(0,0,0);
	t->SetVelocity(v1);

	t->SetProcessInterface(&SparkPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_QUAD_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetProcessInterface(&LightPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE);
	t->SetKeyID(FXLAB_ID_KEY_QUAD_BOW);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);

	Vect3f v2(0,0,0);
	t->SetVelocity(v2);

	t->SetProcessInterface(&BowPoint);
	t->Start();
};

void fxlabClientMovieQuandLight::Close(void)
{
/*	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);*/

	if(LightPoint.Process)
		LightPoint.Process->SetAlive(0);

	if(BowPoint.Process)
		BowPoint.Process->SetAlive(0);

	if(SparkPoint.Process)
		SparkPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientMovieQuandLight::Quant(void)
{
	fxlabClientSpaceType::Quant();

/*	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);*/

	if(LightPoint.Process)
		LightPoint.Process->SetPosition(Position);

	if(BowPoint.Process)
		BowPoint.Process->SetPosition(Position);

	if(SparkPoint.Process)
		SparkPoint.Process->SetPosition(Position);	
};

//---------------------------------

void fxlabClientMovieClouds::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
//	t->SetSoundID(EFF_EVIL_STORM);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TWISTER);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_CLOUDS);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetProcessInterface(&CloudPoint);
	t->Start();
};

void fxlabClientMovieClouds::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	if(CloudPoint.Process)
		CloudPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientMovieClouds::Quant(void)
{
	fxlabClientSpaceType::Quant();

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);

	if(CloudPoint.Process)
		CloudPoint.Process->SetPosition(Position);
};

//-------------------------------

void fxlabClientMovieShifting::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TELEPORT_OMNI_CONTROLLER);
	t->SetKeyID(FXLAB_ID_KEY_MENU_TELEPORT_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetProcessInterface(&OmniPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MOVIE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MASS_SHIFTER_MOVIE);
	t->SetPosition(Position);
	t->SetProcessInterface(&ShiftPoint);
	t->Start();	
};

void fxlabClientMovieShifting::Close(void)
{
	if(ShiftPoint.Process)
		ShiftPoint.Process->SetAlive(0);

	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientMovieShifting::Quant(void)
{
	fxlabClientSpaceType::Quant();

	if(ShiftPoint.Process)
		ShiftPoint.Process->SetPosition(Position);

	if(OmniPoint.Process)
		OmniPoint.Process->SetPosition(Position);
};

//--------------------------------

MatXf getPlacementGroundPose(const Vect3f& center);


void fxlabClientMovieTeleport::Start(void)
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	fxlabClientSpaceType::Start();

	v = Position;
	if(fxlabGetWorldReflectionEnable() && GetW(XCYCL(int(round(v.x))),YCYCL(int(round(v.y)))))
		v.z = vMap->LevelH2O + 2.5f;
	else
		v.z = ZV(XCYCL(int(round(v.x))),YCYCL(int(round(v.y)))) + 2.5f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
	t->SetPosition(v);
	t->SetProcessInterface(&WindPoint);
	t->Start();

};

void fxlabClientMovieTeleport::Close(void)
{
	if(WindPoint.Process)
		WindPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

//------------------------------

void fxlabClientMovieSpot::Start(void)
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	fxlabClientSpaceType::Start();

	v = Position;
	if(fxlabGetWorldReflectionEnable() && GetW(XCYCL(int(round(v.x))),YCYCL(int(round(v.y)))))
		v.z = vMap->LevelH2O + 2.5f;
	else
		v.z = ZV(XCYCL(int(round(v.x))),YCYCL(int(round(v.y)))) + 2.5f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MOVIE_MODEL);

	auto m1 = getPlacementGroundPose(Vect3f(Position.x,Position.y,0));
	t->SetMatrix(m1);

	t->SetPosition(v);
	t->SetProcessInterface(&SpotPoint);
	t->Start();
};

void fxlabClientMovieSpot::Close(void)
{
	if(SpotPoint.Process)
		SpotPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

//------------------------------

void fxlabClientFireTreeMovie::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);	
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_TREE_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);

	Vect3f v1(0,0,0);
	t->SetVelocity(v1);

	t->SetProcessInterface(&FirePoint);
	t->Start();
};

void fxlabClientFireTreeMovie::Close(void)
{
	if(FirePoint.Process)
		FirePoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientFireTreeMovie::Quant(void)
{
	fxlabClientSpaceType::Quant();

	if(FirePoint.Process)
		FirePoint.Process->SetPosition(Position);
};

//-----------------------------

void fxlabClientMovieBossChange::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_BOSS_CHANGE_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetProcessInterface(&OmniPoint);
	t->Start();

	DeformationFlag = 1;
	DeformationTimer.start(1000);
};

void fxlabClientMovieBossChange::Close(void)
{
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	if(DeformationPoint.Process)
		DeformationPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientMovieBossChange::Quant(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientSpaceType::Quant();

	if(OmniPoint.Process)
		OmniPoint.Process->SetPosition(Position);

	if(DeformationFlag && !DeformationTimer){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TERRAIN_DEFORMATOR);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_MOVIE_BOSS_CHANGE_DEFORMATION);
		t->SetPosition(Position);
		t->SetProcessInterface(&DeformationPoint);
		t->Start();
		DeformationFlag = 0;
	};
};

//------------------------------

void fxlabClientWindSound::Start(void)
{
	fxlabClientSpaceType::Start();

	WindTimer.start(0);
	ScreamTimer.start(0);
	ScreamFlag = 0;
};

void fxlabClientWindSound::Quant(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Quant();

	if(!WindTimer){
/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_WIND);
		t->SetPosition(Position);
		t->Start();	*/

		WindTimer.start(15000);
		ScreamTimer.start(13000);
		ScreamFlag = 1;
	};

	if(ScreamFlag && !ScreamTimer){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
//		t->SetSoundID(EFF_TEST);
		t->SetSoundID(EFF_WIND_ADD);
		t->SetPosition(Position);
		t->Start();	
		ScreamFlag = 0;
	};
};

//----------------------------------------

extern int fxlabBigBossGeneratorEnable;
extern int fxlabBigBossWorldEnable;

void fxlabClientCharacterWaveGenerator::CoreGenerate(void)
{
	fxlabParticleType* p;
	float num;
	int n,i;
	float delta;
	float r;
	Vect3f v;
	fxlabGeneralObjectType* t;
	int z;

	if(!fxlabBigBossGeneratorEnable)
		return;

	if(Summoning)
		r = Rate * fxlabParticleSystemScale;
	else
		r = Rate * DeltaTime * fxlabParticleSystemScale;

	num = floor(/*Rate*/r);
	if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
		num++;
	n = round(num);

	delta = (EndPower - StartPower) / Age;

	z = vMap->LevelH2O + 1.0f;
	for(i = 0;i < n;i++){
		p = AddParticle();
		EmitterPosition.Get(p->Position);
		EmitterVelocity.Get(p->Velocity);
		p->Velocity += Velocity;

		p->Age = 0;
		p->LifeTime = Age;
		p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));

		p->Phase = StartPower;
		p->DeltaPhase = delta;

		v.z = z;
		v.x = Position.x;
		v.y = Position.y;
		v.x += p->Position.x;
		v.y += p->Position.y;
		v = CYCLE(v);

		if(fxlabClientRND.RND(100) < 35){
			t = fxlabClientD->CreateObject(/*FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL*/FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_CHARACTER_WAVE_MODEL);
			t->SetPosition(v);

			Vect3f v1(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue());
			t->SetColorVector(v1);

			t->SetTransparency(fxlabGetTrackWaveAlpha());
			t->Start();
		};
	};	
};

void fxlabClientCharacterSteamGenerator::CoreGenerate(void)
{
	if(fxlabBigBossGeneratorEnable)
		fxlabCrazyTail::CoreGenerate();
};

void fxlabClientCharacterSmokeGenerator::CoreGenerate(void)
{
	if(fxlabBigBossGeneratorEnable)
		fxlabCrazyDust::CoreGenerate();
};

//---------------------------

void fxlabClientCharacterWaterfallGenerator::Open(void)
{
	fxlabSparkGenerator::Open();
	BigBossWorldMask = 0;
};

void fxlabClientCharacterWaterfallGenerator::CoreGenerate(void)
{
	if(BigBossWorldMask & fxlabBigBossWorldEnable)
		fxlabSparkGenerator::CoreGenerate();
};

void fxlabClientCharacterWaterfallGenerator::SetBigBossWorldStatus(int world)
{
	BigBossWorldMask = world;
};

//--------------------------------

void fxlabClientBossWorldBurst::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientSpaceType::Start();
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CHARACTER_WORLD_OMNI);
	t->SetPosition(Position);
	t->SetProcessInterface(&OmniPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CHARACTER_WORLD_BURST);
	t->SetPosition(Position);

	Vect3f v1(0,0,0);
	t->SetVelocity(v1);

	t->SetProcessInterface(&BurstPoint);
	t->Start();
};

void fxlabClientBossWorldBurst::Close(void)
{
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	if(BurstPoint.Process)
		BurstPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};


//--------------------------------

void fxlabClientBossWorldExplosion::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientSpaceType::Start();
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION);
	t->SetPosition(Position);

	Vect3f v1(0,0,0);
	t->SetVelocity(v1);

	t->SetProcessInterface(&ExplosionPoint);
	t->Start();
};

void fxlabClientBossWorldExplosion::Close(void)
{
	if(ExplosionPoint.Process)
		ExplosionPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

//-------------------------------------

void fxlabClientMovieDragonFire::Open(void)
{
	fxlabClientSpaceType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientMovieDragonFire::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_DRAGON_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&FirePoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_FLYING_DRAGON1);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientMovieDragonFire::Close(void)
{
	if(FirePoint.Process)
		FirePoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientMovieDragonFire::Quant(void)
{
	fxlabClientSpaceType::Quant();

	if(FirePoint.Process){
		FirePoint.Process->SetPosition(Position);
		FirePoint.Process->SetVelocity(Velocity);
	};

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
};

void fxlabClientMovieDragonFire::SetVelocity(const Vect3f& v)
{
	Velocity = v;
	Velocity.normalize(20.0f);
};

//----------------------------------

void fxlabClientBossRocketFire::Open(void)
{
	fxlabClientSpaceType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientBossRocketFire::Start(void)
{
	Vect3f v;
	fxlabGeneralObjectType* t;

	fxlabClientSpaceType::Start();

	v = Normal;
	v *= 70.0f;
	v += Position;
	CYCLE(v);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_ROCKET_OMNI);
	t->SetPosition(v);
	t->SetProcessInterface(&OmniPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_ROCKET_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&FirePoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetKeyID(FXLAB_ID_KEY_MOVIE_ROCKET_SMOKE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&SmokePoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_FLYING_DRAGON1);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabClientBossRocketFire::Close(void)
{
	if(FirePoint.Process)
		FirePoint.Process->SetAlive(0);

	if(SmokePoint.Process)
		SmokePoint.Process->SetAlive(0);

	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientSpaceType::Close();
};

void fxlabClientBossRocketFire::Quant(void)
{
	Vect3f v;
	fxlabClientSpaceType::Quant();

	if(FirePoint.Process){
		FirePoint.Process->SetPosition(Position);
		FirePoint.Process->SetVelocity(Velocity);
	};

	if(SmokePoint.Process){
		SmokePoint.Process->SetPosition(Position);
		SmokePoint.Process->SetVelocity(Velocity);
	};

	if(OmniPoint.Process){
		v = Normal;
		v *= 70.0f;
		v += Position;
		CYCLE(v);

		OmniPoint.Process->SetPosition(v);
	};

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
};

void fxlabClientBossRocketFire::SetVelocity(const Vect3f& v)
{
	Normal = v;
	Normal.normalize();

	Velocity = Normal;
	Velocity *= 20.0f;
};
