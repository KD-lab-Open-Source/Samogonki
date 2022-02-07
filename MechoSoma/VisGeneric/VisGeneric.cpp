#include "VisGeneric.h"
#include "UnkLibrary.h"
#include "Dispatcher.h"
#include "PolyMgr.h"
#include "TexMgr.h"
#include "Maths.h"

// глобальные переменные
cMeshLibrary	*MeshLibrary=0;
cTextureBuffer	*TextureBuffer=0;
cVisGeneric		*gb_VisGeneric=0;
cPolyDispatcher *P3D=0;

cVisGeneric::cVisGeneric() : SceneArray(KIND_ARRAYSCENE)
{
	CurrentTime=PreviousTime=0;
	Scene=0;
	// инициализация глобальных переменых
	InitArray();
	if(P3D) delete P3D; P3D=new cPolyDispatcher; 
	if(TextureBuffer) delete TextureBuffer; TextureBuffer=new cTextureBuffer();
	if(MeshLibrary) delete MeshLibrary; MeshLibrary=new cMeshLibrary();
}
cVisGeneric::~cVisGeneric()
{
	assert(Scene==0);
	assert(GetNumberScene()==0);
	assert(TextureBuffer==0);
	assert(MeshLibrary==0);
	assert(P3D==0);
}
int cVisGeneric::Create(char *CreateBuffer)
{
	return 0;
}
int cVisGeneric::Release()
{
	if(cUnknownInterface::DecRef()) return cUnknownInterface::GetRef();
	for(int i=0;i<GetNumberScene();i++)
	{
		if(GetScene(i)==Scene) Scene=0;
		delete GetScene(i);
		GetScene(i)=0;
	}
	if(MeshLibrary) delete MeshLibrary; MeshLibrary=0;
	if(TextureBuffer) delete TextureBuffer; TextureBuffer=0;
	if(P3D) delete P3D; P3D=0; 
	DoneArray();
	delete this;
	gb_VisGeneric=0;
	return 0;
}
int cVisGeneric::SetTime(int Time)
{
	PreviousTime=CurrentTime; 
	CurrentTime=Time;
	return 0;
}
int cVisGeneric::dSetTime(int dTime)
{
	PreviousTime=CurrentTime; 
	CurrentTime+=dTime;
	return 0;
}
int cVisGeneric::GetTime(int *Time)
{
	*Time=CurrentTime;
	return 0;
}

int cVisGeneric::PreDraw(int mask)
{
	assert(Scene);
	int CameraCount=0;
	if(mask&(1<<0)) Scene->Animate(CurrentTime,PreviousTime); // включение анимации
	if(mask&(1<<30)) Scene->PreDraw(mask);
	if(mask&(1<<31)) P3D->BeginScene(Scene->GetCameraList());
	return 0;
}
int cVisGeneric::Draw(int mask)
{
	if(Scene) Scene->Draw(mask);
	return 0;
}
int cVisGeneric::PostDraw(int mask)
{
	if(Scene) Scene->PostDraw();
	if(mask&(1<<31)) P3D->EndScene(Scene->GetCameraList());
	return 0;
}

cUnknownClass* cVisGeneric::CreateScene()
{
	cScene *Scene=new cScene;
	Scene->Attach(TextureBuffer);
	Scene->Attach(MeshLibrary);
	SceneArray.Attach(Scene);
	return Scene;
}
int cVisGeneric::ReleaseScene(cUnknownClass* UScene)
{
	assert(UScene->GetKind(KIND_SCENE));
	cScene *scene=(cScene*)UScene;
	if(Scene==scene) Scene=0;
	scene->Detach(TextureBuffer);
	scene->Detach(MeshLibrary);
	SceneArray.Detach(scene);
	delete scene;
	return 1;
}
int cVisGeneric::SetScene(cUnknownClass* UScene)
{
	assert(UScene->GetKind(KIND_SCENE));
	Scene=(cScene*)UScene;
	return 1;
}
void cVisGeneric::ReleaseType(unsigned int Type=M3D_STATIC)
{ 
	assert(Scene);
	if(Scene->GetM3D())
		Scene->GetM3D()->Release(Type);
	if(MeshLibrary)
		MeshLibrary->Release(Type);
	if(TextureBuffer)
		TextureBuffer->Release(Type);
}

cInterfaceVisGeneric* CreateIVisGeneric(char *CreateBuffer)
{
	if(gb_VisGeneric) 
		gb_VisGeneric->IncRef();
	else
	{
		gb_VisGeneric=new cVisGeneric;
		gb_VisGeneric->Create(CreateBuffer);
	}
	return gb_VisGeneric;
}
