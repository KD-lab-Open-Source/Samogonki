#include "StdAfx.h"

#include "aci_parser.h"
#include "TERRA.H"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "Mesh3ds.h"

void fxlabClientEvolutionType::Start(void)
{
	CurrentTime = fxlabClientD->GetTime() - StartTime;
	DeltaTime = fxlabClientD->GetDeltaTime() * fxlabGlobalTimeRate;
	fxlabApplicationObjectType::Start();
};

void fxlabClientEvolutionType::Quant(void)
{
	CurrentTime = fxlabClientD->GetTime() - StartTime;
	DeltaTime = fxlabClientD->GetDeltaTime() * fxlabGlobalTimeRate;
	fxlabApplicationObjectType::Quant();
};

//-----------------------------------------

void fxlabClientKeyObjectType::Start(void)
{
	fxlabClientSpaceType::Start();

	if(KeyID >= 0){
		KeyPoint = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,KeyID);
		KeyData = new float[KeyPoint->GetKeyNum()];
		CalcKeyTime();
		KeyUpdate();
		KeyCheck();
	}else
		CalcKeyTime();
};

void fxlabClientKeyObjectType::CalcKeyTime(void)
{
	KeyTime = CurrentTime * fxlabGlobalTimeRate;
};

void fxlabClientKeyObjectType::Close(void)
{
	if(KeyData) delete KeyData;
	fxlabClientSpaceType::Close();
};

void fxlabClientKeyObjectType::Quant(void)
{
	fxlabClientSpaceType::Quant();
	CalcKeyTime();

	if(KeyPoint){
		if(KeyPoint->GetKeyAttribute() & FXLAB_KEY_ATTRIBUTE_STORAGE)
			Alive &= KeyPoint->GetKeyStatus(KeyTime);
		else
			KeyUpdate();
	};
};

void fxlabClientKeyObjectType::KeyUpdate(void)
{
	Alive &= KeyPoint->GetKey(KeyData,KeyTime);
};

//-------------------------------------

void fxlabParticleToolType::OpenParticleTool(void)
{
	ParticlePoint = NULL;
};

void fxlabParticleToolType::CloseParticleTool(void)
{
	fxlabParticleType* n,*nn;

	n = ParticleList.first();
	while(n){
		nn = n->next;
		if(!(n->Cluster))
			ParticleList.remove(n);
		n = nn;
	};
	ParticleList.delete_all();
};

fxlabParticleType* fxlabParticleToolType::AddParticle(void)
{
	fxlabParticleType* p;
	int i;
	if(ParticlePoint){
		p = ParticlePoint;
		ParticlePoint = ParticlePoint->next;
		return p;
	}else{
		ParticlePoint = new fxlabParticleType[FXLAB_SWARM_PARTICLE_CLUSTER_SIZE];
		ParticlePoint->Cluster = ParticlePoint;
		p = ParticlePoint;
		for(i = 0;i < FXLAB_SWARM_PARTICLE_CLUSTER_SIZE;i++){
			ParticleList.append(p);
			p++;
		};
		ParticlePoint = ParticlePoint->next;		
		return(ParticlePoint->prev);
	};
};

void fxlabParticleToolType::DeleteParticle(fxlabParticleType* p)
{
	if(!ParticlePoint) ParticlePoint = p;
	ParticleList.remove(p);
	ParticleList.append(p);
};

void fxlabParticleToolType::DeleteAllParticle(void)
{
	ParticlePoint = ParticleList.first();
};

//-------------------------------------

void fxlabParticleCore::Open(void)
{
	fxlabClientKeyObjectType::Open();

	Visibility = 1;
	MaxRecallTime = 30;
	RecallTime = MaxRecallTime;

	VsL = Vect3f::ZERO;
	VsS = Vect3f::ZERO;

	OpenParticleTool();

	ProcessMaxX = 0;
	ProcessMaxY = 0;
	ProcessMinX = 0;
	ProcessMinY = 0;
	ProcessMaxZ = 0;
	ProcessMinZ = 0;

	Summoning = 1;
};


void fxlabParticleCore::Start(void)
{
	fxlabClientKeyObjectType::Start();

	CoreGenerate();
	CoreProcess();

//	ConvertPosition();
};

void fxlabParticleCore::StopQuant(void)
{
	int i;

	fxlabClientKeyObjectType::Quant();

	ConvertPosition();
	StopTimeCheckVisibility();

	if(Visibility){
		if(!RecallTime){
			for(i = 0;i < RegenerateTime * 5;i++){
				CoreGenerate();
				CoreProcess();
			};
			ReCall();
		};	
	};
};

void fxlabParticleCore::Quant(void)
{
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	int i;

	fxlabClientKeyObjectType::Quant();

	ConvertPosition();
	CheckVisibility();

	if(Summoning)
		Summoning = 0;
	else{
		if(Visibility){
			if(!RecallTime){
				if(Alive){
					for(i = 0;i < RegenerateTime;i++){
						CoreGenerate();
						CoreProcess();
					};
				}else{
					for(i = 0;i < RegenerateTime;i++){
						CoreProcess();
					};
				};
				ReCall();
			}else{
				if(Alive) 
					CoreGenerate();
				CoreProcess();
			};
		}else{
			if(!RecallTime){
				p = ParticleList.first();
				while(p != ParticlePoint){
					pp = p->next;
					DeleteParticle(p);
					p = pp;
				};
			}else{
				if(Alive) 
					CoreGenerate();
				CoreProcess();
			};
		};
	};
};

int fxlabParticleCore::GetAlive(void)
{
	if(!Alive && ParticleList.first() == ParticlePoint)
		return 0;
	return 1;
};

void fxlabParticleCore::ConvertPosition(void)
{
	VsS = Vect3f(ProcessMaxX - ProcessMinX,ProcessMaxY - ProcessMinY,ProcessMaxZ - ProcessMinZ);
	VsL.x = XCYCL(int(round(Position.x + ProcessMinX)));
	VsL.y = YCYCL(int(round(Position.y + ProcessMinY)));
	VsL.z = Position.z + ProcessMinZ;
};
	
void fxlabParticleCore::KeyUpdate(void)
{
	fxlabClientKeyObjectType::KeyUpdate();
	RegenerateTime = round(KeyData[FXLAB_PARTICLE_CORE_DATA_REGENERATE_TIME]);
};

void fxlabParticleCore::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_PARTICLE_CORE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabParticleCore");
};

#include "VisGeneric.h"
#include "Camera.h"
#include "Scene.h"

void fxlabParticleCore::CheckVisibility(void)
{
	int i;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	cCamera* camera;
    MatXf mat;

	Visibility = 0;
	IVisGeneric=CreateIVisGeneric();
	scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
	assert(scene);
	for(i = 0;i < scene->GetNumberCamera();i++){
		camera = scene->GetCamera(i);
		camera->BuildMatrix(mat,VsL);
		if(camera->TestVisible(mat,VsS))
			Visibility |= 1 << i;
	};
	IVisGeneric->Release();

	if(!Visibility && RecallTime > 0) RecallTime--;
	return;
};

void fxlabParticleCore::StopTimeCheckVisibility(void)
{
	int i;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	cCamera* camera;
    MatXf mat;

	Visibility = 0;
	IVisGeneric=CreateIVisGeneric();
	scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
	assert(scene);
	for(i = 0;i < scene->GetNumberCamera();i++){
		camera = scene->GetCamera(i);
		camera->BuildMatrix(mat,VsL);
		if(camera->TestVisible(mat,VsS))
			Visibility |= 1 << i;
	};
	IVisGeneric->Release();
	return;
};

void fxlabParticleCore::Close(void)
{
	CloseParticleTool();
	fxlabClientKeyObjectType::Close();
};


//--------------------------------

