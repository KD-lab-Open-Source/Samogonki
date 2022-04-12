#include "StdAfx.h"

#include "aci_parser.h"
#include "TERRA.H"

#include "BodyDispatcher.h"
#include "Mechos.h"

#include "race.h"
#include "sound.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientLauncher.h"

void fxlabClientSetObjectType::Open(void)
{
	fxlabClientKeyObjectType::Open();

	Mode = FXLAB_CLIENT_SET_MODE_NONE;
	AtomData = NULL;
	NumAtom = 0;
	ActiveCnt = 0;

	Velocity = Vect3f::ZERO;
};

void fxlabClientSetObjectType::Start(void)
{
	fxlabClientKeyObjectType::Start();
	Mode = round(KeyData[FXLAB_CLIENT_SET_OBJECT_MODE]);
	ObjectGenerate();
};

void fxlabClientSetObjectType::Close(void)
{
	int i;
	if(Mode & FXLAB_CLIENT_SET_MODE_DESTROY){
		for(i = 0;i < NumAtom;i++){
			if(AtomData[i].ObjectPoint.Process)
				AtomData[i].ObjectPoint.Process->SetAlive(0);
		};
	}else{
		for(i = 0;i < NumAtom;i++){
			if(AtomData[i].ObjectPoint.Process)
				AtomData[i].ObjectPoint.Process->SetProcessInterface(NULL);
		};
	};
	delete AtomData;
	fxlabClientKeyObjectType::Close();
};

void fxlabClientSetObjectType::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	ObjectGenerate();
};

void fxlabClientSetObjectType::ObjectGenerate(void)
{
	int i;
	Vect3f v;
	fxlabClientSetAtomType* p;
	fxlabGeneralObjectType* t;

	p = AtomData;
	ActiveCnt = 0;

	v = Vect3f(KeyData[FXLAB_CLIENT_SET_OBJECT_VELOCITYX],KeyData[FXLAB_CLIENT_SET_OBJECT_VELOCITYY],KeyData[FXLAB_CLIENT_SET_OBJECT_VELOCITYZ]);
	v += Velocity;

	for(i = 0;i < NumAtom;i++){
		if(p->ObjectPoint.Process){
			if(p->KeyID < 0){
				p->UseFlag = 0;
				p->ObjectPoint.Process->SetAlive(0);
			}else{
				ActiveCnt++;
				p->ObjectPoint.Process->SetVelocity(v);
				p->ObjectPoint.Process->SetPosition(Position);
			};
		}else{
			if(p->KeyID >= 0){
				if(Alive && !(p->UseFlag)){
					ActiveCnt++;
					p->UseFlag = 1;
					t = fxlabClientD->CreateObject(p->ObjectType);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(p->KeyID);
					t->SetVelocity(Velocity);
					t->SetPosition(Position);
					t->SetProcessInterface(&(p->ObjectPoint));
					t->Start();
				};
			}else{
				p->UseFlag = 0;
			};
		};
		p++;
	};
};

int fxlabClientSetObjectType::GetAlive(void)
{
	if(!Alive && (!(Mode & FXLAB_CLIENT_SET_MODE_WAIT) || !ActiveCnt))
		return 0;
	return 1;
};

void fxlabClientSetObjectType::KeyUpdate(void)
{
	int i;
	float* n;
	fxlabClientSetAtomType* p;

	fxlabClientKeyObjectType::KeyUpdate();

	if(!AtomData){
		NumAtom = round(KeyData[FXLAB_CLIENT_SET_OBJECT_NUM]);
		AtomData = new fxlabClientSetAtomType[NumAtom];
	};

	p = AtomData;
	n = KeyData + FXLAB_CLINET_SET_OBJECT_MAX;
	for(i = 0;i < NumAtom;i++){
		p->ObjectType = round(*(n + FXLAB_CLIENT_SET_ATOM_DATA_TYPE));
		p->KeyID = round(*(n + FXLAB_CLIENT_SET_ATOM_DATA_KEY_ID));
		p++;
		n += FXLAB_CLIENT_SET_ATOM_DATA_MAX;
	};
};

void fxlabClientSetObjectType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLINET_SET_OBJECT_MAX + FXLAB_CLIENT_SET_ATOM_DATA_MAX*NumAtom)
		ErrH.Abort("Bad Key of fxClientSetObject");
};

void fxlabClientSetObjectType::SetVelocity(Vect3f& v)
{
	Velocity = v;
};

//------------------------------------------------

void fxlabClientBaseBodyType::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientBaseBodyType::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	fxlabClientKeyObjectType::Close();
};

void fxlabClientBaseBodyType::Quant(void)
{
	fxlabGeneralObjectType* t;

	fxlabClientKeyObjectType::Quant();
	if(Alive){
		if(Core){
			if(!(Core->alive())){
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
							t->SetPosition(Position);
							t->SetVelocity(Velocity);
							t->SetKeyID(TailID);
							t->SetProcessInterface(&TailPoint);
							t->Start();
						}else{						
							TailPoint.Process->SetPosition(Position);
							TailPoint.Process->SetVelocity(Velocity);
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

void fxlabClientBaseBodyType::Destroy(void)
{
	fxlabGeneralObjectType* t;

	if(ExplodeID >= 0){
		t = fxlabClientD->CreateObject(ExplodeType);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetKeyID(ExplodeID);
		t->Start();
	};
	Alive = 0;
};

void fxlabClientBaseBodyType::SetBody(class Body* p)
{
	Core = p;
	Position = Core->R();
	Velocity = Vect3f::ZERO;
};

void fxlabClientBaseBodyType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_BODY_LINK_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabBodyLink");
};

void fxlabClientBaseBodyType::KeyUpdate(void)
{
	fxlabClientKeyObjectType::KeyUpdate();

	TailType = round(KeyData[FXLAB_CLIENT_BODY_LINK_DATA_TAIL_TYPE]);
	TailID = round(KeyData[FXLAB_CLIENT_BODY_LINK_DATA_TAIL_ID]);
	ExplodeType = round(KeyData[FXLAB_CLIENT_BODY_LINK_DATA_EXPLODE_TYPE]);
	ExplodeID = round(KeyData[FXLAB_CLIENT_BODY_LINK_DATA_EXPLODE_ID]);
};

//------------------------------------------------

void fxlabClientBodyLinkType::Open(void)
{
	fxlabClientBaseBodyType::Open();
	LastPosition = Vect3f::ZERO;
};

void fxlabClientBodyLinkType::CalcVelocity(void)
{
	Position = Core->R();
	Velocity = getDist(LastPosition,Position);
	LastPosition = Position;
};

void fxlabClientBodyLinkType::SetBody(class Body* p)
{
	fxlabClientBaseBodyType::SetBody(p);
	LastPosition = Position;
};

//----------------------------------------------

void fxlabClientMechosLinkType::Open(void)
{
	fxlabClientBodyLinkType::Open();
	MechosOwner = NULL;
};

void fxlabClientMechosLinkType::Quant(void)
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
								t->SetPosition(Position);
								t->SetVelocity(Velocity);
								t->SetKeyID(TailID);
								t->SetProcessInterface(&TailPoint);
								t->Start();
							}else{						
								TailPoint.Process->SetPosition(Position);
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

void fxlabClientMechosLinkType::SetBody(class Body* p)
{
	MechosOwner = dynamic_cast<Mechos*>(p);
	if(!MechosOwner)
		ErrH.Abort("Bad Owner Body in fxlabClientMechosLinkType");
	fxlabClientBodyLinkType::SetBody(p);
};

//----------------------------------------------

void fxlabClientFrontLink::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_FRONT);
		Velocity = MechosOwner->Yglobal();
	};
};

void fxlabClinetBackLink::CalcVelocity(void)
{
	if(Core->completed()){
		Position = MechosOwner->part_coords(M3D_FRONT);
		Velocity = -MechosOwner->Yglobal();
	};
};

void fxlabClinetLeftForwardWheelLink::CalcVelocity(void)
{
//	Vect3f p;
	if(Core->completed()){
//		p = Core->R();
		Position = MechosOwner->part_coords(M3D_LF_WHEEL);
		Velocity = MechosOwner->Xglobal();
//		Velocity = getDist(p,Position);
//		Velocity.normalize();
	};
};

void fxlabClientRightForwardWheelLink::CalcVelocity(void)
{
//	Vect3f p;
	if(Core->completed()){
//		p = Core->R();
		Position = MechosOwner->part_coords(M3D_RF_WHEEL);
		Velocity = -MechosOwner->Xglobal();
//		Velocity = getDist(p,Position);
//		Velocity.normalize();
	};
};

void fxlabClientLeftBackWheelLink::CalcVelocity(void)
{
//	Vect3f p;
	if(Core->completed()){
//		p = Core->R();
		Position = MechosOwner->part_coords(M3D_LB_WHEEL);
		Velocity = MechosOwner->Xglobal();
//		Velocity = getDist(p,Position);
//		Velocity.normalize();
	};
};

void fxlabClientRightBackWheelLink::CalcVelocity(void)
{
//	Vect3f p;
	if(Core->completed()){
//		p = Core->R();
		Position = MechosOwner->part_coords(M3D_RB_WHEEL);
		Velocity = -MechosOwner->Xglobal();
//		Velocity = getDist(p,Position);
//		Velocity.normalize();
	};
};

//------------------------------------------

void fxlabClientObjectLauncher::Open(void)
{
	fxlabClientKeyObjectType::Open();
	Velocity = Vect3f::ZERO;
};


void fxlabClientObjectLauncher::Start(void)
{
	fxlabClientKeyObjectType::Start();
	ObjectTimer.start(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_START_TIME]));
};

void fxlabClientObjectLauncher::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	if(!ObjectTimer())
		CreateObject();
};

void fxlabClientObjectLauncher::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabClientObjectLauncher");
};

void fxlabClientObjectLauncher::CreateObject(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_TYPE]));
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_ID]));
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->Start();

	ObjectTimer.start(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_DISABLE_TIME]));
};

//----------------------------------------------------

void fxlabClientObjectSwitcher::Open(void)
{
	fxlabClientObjectLauncher::Open();
	EnableFlag = 0;
	Velocity = Vect3f::ZERO;
};

void fxlabClientObjectSwitcher::Close(void)
{
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetAlive(0);
	fxlabClientObjectLauncher::Close();
};

void fxlabClientObjectSwitcher::Quant(void)
{
	fxlabClientKeyObjectType::Quant();
	if(EnableFlag){
		if(!ObjectTimer()){
			CreateObject();
			EnableFlag = 0;
		};
	}else{
		if(ObjectTimer()){
			UpdateObject();
		}else{
			DestroyObject();
			EnableFlag = 1;
		};
	};
};
	
void fxlabClientObjectSwitcher::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_OBJECT_SWITCHER_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabClientSwitcherLauncher");
};


void fxlabClientObjectSwitcher::CreateObject(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_TYPE]));
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_ID]));
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&ObjectPoint);
	t->Start();

	ObjectTimer.start(round(KeyData[FXLAB_CLIENT_OBJECT_SWITCHER_DATA_ENABLE_TIME]));
};

void fxlabClientObjectSwitcher::UpdateObject(void)
{
	if(ObjectPoint.Process){
		ObjectPoint.Process->SetPosition(Position);
		ObjectPoint.Process->SetVelocity(Velocity);
	};
};

void fxlabClientObjectSwitcher::DestroyObject(void)
{
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetAlive(0);
	ObjectTimer.start(round(KeyData[FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_DISABLE_TIME]));
};

//-------------------------------------------

void fxlabClientRemoteSwitcher::Close(void)
{
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetAlive(0);
	fxlabClientRemoteCotrol::Close();
};

void fxlabClientRemoteSwitcher::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CLIENT_REMOTE_SWITCHER_MAX)
		ErrH.Abort("Bad Key of fxlabClientRemoteSwitcher");
};

void fxlabClientRemoteSwitcher::SetRemoteID(int id)
{
	if(id > 0)
		CreateObject();
	else
		DestroyObject();
};

void fxlabClientRemoteSwitcher::Open(void)
{
	fxlabClientRemoteCotrol::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientRemoteSwitcher::SetVelocity(const Vect3f& v)
{
	Velocity = v;
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetVelocity(v);
};

void fxlabClientRemoteSwitcher::SetPosition(const Vect3f& v)
{
	fxlabClientRemoteCotrol::SetPosition(v);
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetPosition(v);
};

void fxlabClientRemoteSwitcher::CreateObject(void)
{
	fxlabGeneralObjectType* t;
	if(!ObjectPoint.Process){
		t = fxlabClientD->CreateObject(round(KeyData[FXLAB_CLIENT_REMOTE_SWITCHER_DATA_TYPE]));
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(round(KeyData[FXLAB_CLIENT_REMOTE_SWITCHER_DATA_ID]));
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetProcessInterface(&ObjectPoint);
		t->Start();
	};
};

void fxlabClientRemoteSwitcher::DestroyObject(void)
{
	if(ObjectPoint.Process)
		ObjectPoint.Process->SetAlive(0);
};


//----------------------------------------------------------

void fxlabClientRemoteAim::Open(void)
{
	fxlabClientRemoteCotrol::Open();
	TargetID = -1;
	Target = NULL;
};

void fxlabClientRemoteAim::SetRemoteID(int id)
{
	mchRacer* mp;

	if(id != TargetID){
		if(id == -1){
			Target = NULL;
			TargetID = -1;
		}else{
			mp = mch_raceD->racerLst->first();
			while(mp){
				if(mp->ID == id){
					Target = mp;
					TargetID = id;
					break;
				};
				mp = mp->next;
			};
		};
	};
};

//------------------------------------------------

void fxlabClientBaseBindingType::Open(void)
{
	fxlabClientSpaceType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabClientBaseBindingType::Quant(void)
{
	fxlabClientSpaceType::Quant();
	if(Alive){
		if(Core){
			if(!(Core->alive())){
				Destroy();
				Alive = 0;
			}else{
				CalcVelocity();
				Update();
			};
		}else Destroy();
	}else{
		if(Core && Core->alive())
			Destroy();
	};
};

void fxlabClientBaseBindingType::Destroy(void)
{
	SetAlive(0);
};

void fxlabClientBaseBindingType::SetBody(class Body* p)
{
	Core = p;
	Position = Core->R();
	Velocity = Vect3f::ZERO;
};

//------------------------------------------------

void fxlabClientCompareBindingType::Open(void)
{
	fxlabClientBaseBindingType::Open();
	LastPosition = Vect3f::ZERO;
};

void fxlabClientCompareBindingType::CalcVelocity(void)
{
	Position = Core->R();
	Velocity = getDist(LastPosition,Position);
	LastPosition = Position;
};

void fxlabClientCompareBindingType::SetBody(class Body* p)
{
	fxlabClientBaseBindingType::SetBody(p);
	LastPosition = Position;
};

//------------------------------------------------

const float FXLAB_CLIENT_SUICIDE_SPEED = 10.0f;

void fxlabClientSuicideLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	fxlabClientCompareBindingType::Close();
};

void fxlabClientSuicideLink::CalcVelocity(void)
{
	float d;
	Position = Core->R();
	Velocity = Core->Yglobal();
	Velocity = -Velocity;
	d = 30.0f;
	Position.x = XCYCL(int(round(Position.x + Velocity.x * d)));
	Position.y = YCYCL(int(round(Position.y + Velocity.y * d)));
	Position.z = Position.z + Velocity.z * d;
	Velocity *= FXLAB_CLIENT_SUICIDE_SPEED;
};

void fxlabClientSuicideLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_SUICIDE_FIRE);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};

//-----------------------------------------

void fxlabClientBolideLink::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabClientCompareBindingType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetSoundID(EFF_METEORITE_SWARM);
	t->Start();
};

void fxlabClientBolideLink::Close(void)
{
//	if(SoundPoint.Process)
//		SoundPoint.Process->SetAlive(0);
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientBolideLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_STONE_TAIL);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};

/*	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
	else{
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetSoundID(EFF_METEORITE_SWARM);
		t->SetProcessInterface(&SoundPoint);
		t->Start();
	};*/
};

//----------------------------------

void fxlabClientVolcanicStoneLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientVolcanicStoneLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_STONE_TAIL);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};

//-----------------------------------

void fxlabClientBombExplodeLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientBombExplodeLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_CANNON_BULLET);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};

//----------------------------------

void fxlabClientFireBallLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientFireBallLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_FIREBALL_TAIL);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};

	if(!OmniPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetKeyID(FXLAB_ID_KEY_FIREBALL_LIGHT);
		t->SetProcessInterface(&OmniPoint);
		t->Start();
	}else
		OmniPoint.Process->SetPosition(Position);
};

//----------------------------

void fxlabClientLightningBoltLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientLightningBoltLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetKeyID(FXLAB_ID_KEY_LIGHTNING_BOLT_BOW);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else
		TailPoint.Process->SetPosition(Position);
};

//----------------------------------------



void fxlabClientMortarBodyLink::Start(void)
{
	fxlabClientCompareBindingType::Start();
	Timer.stop();
};

void fxlabClientMortarBodyLink::Quant(void)
{
	fxlabClientSpaceType::Quant();
	if(Alive){
		if(Core){
			if(!(Core->alive())){
				Destroy();
				Alive = 0;
			}else{
				CalcVelocity();
				Update();
			};
		}else Destroy();
	}else{
		Destroy();
	};
};

void fxlabClientMortarBodyLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientMortarBodyLink::Update(void)
{
	fxlabGeneralObjectType* t;

	if(Timer() < 300){
		if(Timer()){
			if(Velocity.norm() > 2.0f){
				Timer.stop();
			};
		}else{
			if(Velocity.norm() < 2.0f){
				Timer.start();
			};
		};

		if(!TailPoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(Position);
			t->SetVelocity(Velocity);
			t->SetKeyID(FXLAB_ID_KEY_MINE_DUST);
			t->SetProcessInterface(&TailPoint);
			t->Start();
		}else{
			TailPoint.Process->SetPosition(Position);
			TailPoint.Process->SetVelocity(Velocity);
		};
	}else{
		if(TailPoint.Process)
			TailPoint.Process->SetAlive(0);
	};
};

//-----------------------------

void fxlabClientBonusBodyLink::Update(void)
{
	fxlabGeneralObjectType* t;

	if(Timer() < 300){
		if(Timer()){
			if(Velocity.norm() > 2.0f)
				Timer.stop();
		}else{
			if(Velocity.norm() < 2.0f)
				Timer.start();
		};

		if(!TailPoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(Position);
			t->SetVelocity(Velocity);
			t->SetKeyID(FXLAB_ID_KEY_PETARD_BOX_TAIL);
			t->SetProcessInterface(&TailPoint);
			t->Start();
		}else{
			TailPoint.Process->SetPosition(Position);
			TailPoint.Process->SetVelocity(Velocity);
		};

	}else{
		if(TailPoint.Process)
			TailPoint.Process->SetAlive(0);
	};
};

//---------------------------	

void fxlabClientSnowFallLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientSnowFallLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_SNOW_FALL_TAIL);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};

//-----------------------------


void fxlabClientMechosBurnLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientMechosBurnLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_MECHOS_BURN);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};

//-------------------------------

void fxlabClientBombSplinterLink::Close(void)
{
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);

	fxlabClientCompareBindingType::Close();
};

void fxlabClientBombSplinterLink::Update(void)
{
	fxlabGeneralObjectType* t;
	if(!TailPoint.Process){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetVelocity(Velocity);
		t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_TAIL);
		t->SetProcessInterface(&TailPoint);
		t->Start();
	}else{						
		TailPoint.Process->SetPosition(Position);
		TailPoint.Process->SetVelocity(Velocity);
	};
};


//---------------------------------------

void fxlabClientMechosBindingType::Open(void)
{
	fxlabClientSpaceType::Open();
	MechosOwner = NULL;
};

void fxlabClientMechosBindingType::Quant(void)
{
	fxlabClientSpaceType::Quant();
	if(Alive){
		if(MechosOwner){
			if(!(MechosOwner->alive()) || !(MechosOwner->completed())){
				Destroy();
				Alive = 0;
			}else{
				CalcVelocity();
				Update();
			};
		}else Destroy();
	}else{
		if(MechosOwner && MechosOwner->alive())
			Destroy();
	};
};

void fxlabClientMechosBindingType::Destroy(void)
{
	SetAlive(0);
};

void fxlabClientMechosBindingType::SetMechosPoint(class mchMechosRacer* p)
{
	MechosOwner = p;
	Position = MechosOwner->R();
};

//-------------------------------------

void fxlabClientVoodooMasterLink::Open(void)
{
	fxlabClientMechosBindingType::Open();
	StartAngle = 0;
	Angle = 0;
	DeltaAngle = 0;
	Radius = 0;

	LockTarget = 0;
	LastPosition = Vect3f::ZERO;
	ThetaVelocity = 0;
	RadiusVelocity = 0;
	PsiVelocity = 0;
};

void fxlabClientVoodooMasterLink::Close(void)
{
	if(ModelPoint.Process)
		ModelPoint.Process->SetAlive(0);
	if(TailPoint.Process)
		TailPoint.Process->SetAlive(0);
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabClientMechosBindingType::Close();
};


const float FXLAB_CLIENT_VOODOO_MASTER_THETA_MASS = 0.1f;
const float FXLAB_CLIENT_VOODOO_MASTER_THETA_DAMP = 0.90f;

const float FXLAB_CLIENT_VOODOO_MASTER_RADIUS_MASS = 0.1f;
const float FXLAB_CLIENT_VOODOO_MASTER_RADIUS_DAMP = 0.90f;

const float FXLAB_CLIENT_VOODOO_MASTER_PSI_MASS = 0.1f;
const float FXLAB_CLIENT_VOODOO_MASTER_PSI_DAMP = 0.90f;


void fxlabClientVoodooMasterLink::Update(void)
{
	Vect3f n,p;
	MatXf mat;
	fxlabGeneralObjectType* t;
	float a;

	Angle = StartAngle + DeltaAngle * CurrentTime * fxlabGlobalTimeRate;

	if(LockTarget){
		p.setSpherical(LastPosition.z,LastPosition.y,LastPosition.x);
		p += Position;
		CYCLE(p);

//		LastPosition.z += 2.0f * DeltaAngle * DeltaTime;
//		if(LastPosition.x > MechosOwner->radius()*2.0f){
//			LastPosition.x += 0.5f * (MechosOwner->radius()*2.0f - LastPosition.x) * DeltaTime;
//		};

		ThetaVelocity += (M_PI * 0.5f - LastPosition.y) * FXLAB_CLIENT_VOODOO_MASTER_THETA_MASS;
		LastPosition.y += ThetaVelocity * DeltaTime;
		ThetaVelocity *= FXLAB_CLIENT_VOODOO_MASTER_THETA_DAMP;

		RadiusVelocity += (MechosOwner->radius()*1.5f - LastPosition.x) * FXLAB_CLIENT_VOODOO_MASTER_RADIUS_MASS;
		LastPosition.x += RadiusVelocity * DeltaTime;
		RadiusVelocity *= FXLAB_CLIENT_VOODOO_MASTER_RADIUS_DAMP;


		a = -MechosOwner->yaw() + M_PI * 1.5f;
		cycle(a,M_PI * 2.0f);
		PsiVelocity += getDist_f(a,LastPosition.z,M_PI * 2.0f) * FXLAB_CLIENT_VOODOO_MASTER_PSI_MASS;
		LastPosition.z += PsiVelocity * DeltaTime;
		cycle(LastPosition.z,M_PI * 2.0f);
		PsiVelocity *= FXLAB_CLIENT_VOODOO_MASTER_PSI_DAMP;

		n.x = cosf(Angle - M_PI * 0.5f);
		n.y = sinf(Angle - M_PI * 0.5f);
		n.z = 0;

		mat.rot() = Mat3f(M_PI - Angle,Z_AXIS);
		mat.trans() = p;

		if(ModelPoint.Process){
			ModelPoint.Process->SetMatrix(mat);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_VOODOO_HEAD_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_MODEL);
			t->SetMatrix(mat);
			t->SetProcessInterface(&ModelPoint);
			t->Start();
		};

		if(TailPoint.Process){
			TailPoint.Process->SetPosition(p);
			TailPoint.Process->SetVelocity(n);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_TAIL);
			t->SetPosition(p);
			t->SetVelocity(n);
			t->SetProcessInterface(&TailPoint);
			t->Start();
		};

		if(OmniPoint.Process){
			OmniPoint.Process->SetPosition(p);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_OMNI);
			t->SetPosition(p);
			t->SetProcessInterface(&OmniPoint);
			t->Start();
		};

		if(SoundPoint.Process)
			SoundPoint.Process->SetPosition(p);
		else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetSoundID(EFF_MASTER_VOODOO);
			t->SetPosition(Position);
			t->SetProcessInterface(&SoundPoint);
			t->Start();
		};
	}else{
		p.x = Position.x + Radius * cosf(Angle);
		p.y = Position.y + Radius * sinf(Angle);
		p.z = Position.z;
		CYCLE(p);

		n.x = cosf(Angle - M_PI * 0.5f);
		n.y = sinf(Angle - M_PI * 0.5f);
		n.z = 0;

		mat.rot() = Mat3f(M_PI - Angle,Z_AXIS);
		mat.trans() = p;

		if(ModelPoint.Process){
			ModelPoint.Process->SetMatrix(mat);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_VOODOO_HEAD_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_MODEL);
			t->SetMatrix(mat);
			t->SetProcessInterface(&ModelPoint);
			t->Start();
		};

		if(TailPoint.Process){
			TailPoint.Process->SetPosition(p);
			TailPoint.Process->SetVelocity(n);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_TAIL);
			t->SetPosition(p);
			t->SetVelocity(n);
			t->SetProcessInterface(&TailPoint);
			t->Start();
		};

		if(OmniPoint.Process){
			OmniPoint.Process->SetPosition(p);
		}else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_HEAD_OMNI);
			t->SetPosition(p);
			t->SetProcessInterface(&OmniPoint);
			t->Start();
		};

		if(SoundPoint.Process)
			SoundPoint.Process->SetPosition(p);
		else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetSoundID(EFF_MASTER_VOODOO);
			t->SetPosition(Position);
			t->SetProcessInterface(&SoundPoint);
			t->Start();
		};
	};
};

void fxlabClientVoodooMasterLink::CalcVelocity(void)
{
	if(MechosOwner->completed())
		Position = MechosOwner->part_coords(M3D_FRONT);
};

void fxlabClientVoodooMasterLink::SetTarget(class mchMechosRacer* p)
{
	Vect3f v;

	LastPosition.x = Position.x + Radius * cosf(Angle);
	LastPosition.y = Position.y + Radius * sinf(Angle);
	LastPosition.z = Position.z;
	CYCLE(LastPosition);

	MechosOwner = p;
	CalcVelocity();

	v = LastPosition;
	v -= Position;
	LastPosition.x = v.norm();

	if(LastPosition.x > 0.0001f){
		LastPosition.y = v.theta();
		LastPosition.z = v.psi();
	}else{
		LastPosition.y = 0;
		LastPosition.z = 0;
	};
	LockTarget = 1;
};

