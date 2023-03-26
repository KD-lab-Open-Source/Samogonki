#include "VisGeneric.h"
#include "Dispatcher.h"
#include "Frame.h"

int cVisGeneric::LoadObjectLibrary(const std::string &path)
{
	assert(Scene);
	Scene->GetM3D()->LoadLib(path);
	return 0;
}
int cVisGeneric::FreeObjectLibrary(int kind,int type)
{
	assert(Scene);
	Scene->GetLibMesh()->Release(M3D_TOTAL_TYPE(kind,type));
	return 0;
}
int cVisGeneric::ReleaseObject(int kind,int type)
{
	assert(Scene);
	GetActiveScene()->GetM3D()->Release(M3D_TOTAL_TYPE(kind,type));
	return 0;
}
cUnknownClass* cVisGeneric::CreateObject(int kind,int type)
{
	assert(Scene);
	return (cUnknownClass*)Scene->GetM3D()->CreateObject(M3D_TOTAL_TYPE(kind,type),0,0,0,0,0,0);
}
cUnknownClass* cVisGeneric::CreateObject(const char *fname,const char *TexturePath,int Kind,int Type)
{
	assert(Scene);
	return (cUnknownClass*)Scene->GetM3D()->CreateObject(fname,TexturePath,M3D_TOTAL_TYPE(Kind,Type));
}
int cVisGeneric::ReleaseObject(cUnknownClass* UObject)
{
	assert(Scene);
	Scene->GetM3D()->Delete((cMesh*)UObject);
	return 0;
}
int cVisGeneric::SetObjectAttribute(cUnknownClass *UObject,int attribute)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetAttribute(attribute);
	return 0;
}
int cVisGeneric::GetObjectAttribute(cUnknownClass *UObject,int attribute)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	return Mesh->GetAttribute(attribute);
}
int cVisGeneric::ClearObjectAttribute(cUnknownClass *UObject,int attribute)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->ClearAttribute(attribute);
	return 0;
}
int cVisGeneric::SetObjectPosition(cUnknownClass *UObject,const Vect3f *Pos,const Vect3f *AngleGrad)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->LocalMatrix.NewMatrix();
	Mesh->LocalMatrix.Translate(Pos->x,Pos->y,Pos->z);
	Mesh->LocalMatrix.Rotate(AngleGrad->x,AngleGrad->y,AngleGrad->z);
	Mesh->SetPosition(Mesh->LocalMatrix);
	return 0;
}
int cVisGeneric::SetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetPosition(*Matrix);
	return 0;
}
int cVisGeneric::dSetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetPosition(Mesh->LocalMatrix*(*Matrix));
	return 0;
}
int cVisGeneric::GetObjectPosition(cUnknownClass *UObject,MatXf *Matrix)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	*Matrix=Mesh->LocalMatrix;
	return 0;
}
int cVisGeneric::GetObjectPosition(cUnknownClass *UObject,Vect3f *Pos,Vect3f *AngleGrad)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	*Pos=Mesh->GlobalMatrix.trans();
	AngleGrad->set(0,0,0);
	return 0;
}
int cVisGeneric::dSetObjectPosition(cUnknownClass *UObject,const Vect3f *dPos,const Vect3f *dAngleGrad)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->LocalMatrix.Translate(dPos->x,dPos->y,dPos->z);
	if(fabs(dAngleGrad->x)>=0.01f||fabs(dAngleGrad->y)>=0.01f||fabs(dAngleGrad->z)>=0.01f)
		Mesh->LocalMatrix.Rotate(dAngleGrad->x,dAngleGrad->y,dAngleGrad->z);
	Mesh->SetPosition(Mesh->LocalMatrix);
	return 0;
}
int cVisGeneric::SetObjectScale(cUnknownClass *UObject,const Vect3f *Scale)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetScale(*Scale);
	return 0;
}
int cVisGeneric::dSetObjectScale(cUnknownClass *UObject,const Vect3f *dScale)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->dSetScale(*dScale);
	return 0;
}
int cVisGeneric::GetObjectScale(cUnknownClass *UObject,Vect3f *Scale)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	*Scale=Mesh->GetScale();
	return 0;
}
int cVisGeneric::GetObjectAnimation(cUnknownClass *UObject,float *AnimationPeriod,float *StartPhase)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	float finish;
	Mesh->GetAnimation(*AnimationPeriod,*StartPhase,finish);
	return 0;
}
int cVisGeneric::dSetObjectAnimation(cUnknownClass *UObject,float dTime)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->Animate(dTime);
	return 0;
}
int cVisGeneric::SetObjectAnimation(cUnknownClass *UObject,float AnimationPeriod,float StartPhase,float FinishPhase,int mask)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetAnimation(AnimationPeriod,StartPhase,FinishPhase,mask);
	return 0;
}
int cVisGeneric::SetObjectChannel(cUnknownClass *UObject,int NumberChain,float TransitionTime)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0||Mesh->Frame==0||Mesh->Frame->SetCurrentChain(NumberChain)<0) return 1;
	return 0;
}
int cVisGeneric::GetObjectNumberChannel(cUnknownClass *UObject,const char *NameChainMask)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0||Mesh->Frame==0) return -1;
	return Mesh->Frame->GetChain(NameChainMask);
}
int cVisGeneric::SetObjectChannel(cUnknownClass *UObject,const char *NameChainMask,float TransitionTime,int number)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0||Mesh->Frame==0) return -1;
	return Mesh->Frame->SetCurrentChain(NameChainMask,number);
}
int cVisGeneric::IsObjectAnimationPhasePassed(cUnknownClass *UObject,float Phase)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0||Mesh->Frame==0) return -1;
	Mesh->Frame->IsPhasePassed(Phase);
	return 0;
}
int cVisGeneric::SetObjectColor(cUnknownClass *UObject,const sColor4f *diffuse,const sColor4f *specular,int mask)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	if(diffuse) 
	{
		if(specular) Mesh->SetColor(*diffuse,*specular,mask);
		else Mesh->SetColor(*diffuse,Mesh->GetSpecular(),mask);
	}
	else
		if(specular) Mesh->SetColor(Mesh->GetDiffuse(),*specular,mask);
		else Mesh->SetColor(Mesh->GetDiffuse(),Mesh->GetSpecular(),mask);
	return 0;
}
int cVisGeneric::GetObjectColor(cUnknownClass *UObject,sColor4f *diffuse,sColor4f *specular,int mask)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	if(diffuse) *diffuse=Mesh->GetDiffuse();
	if(specular) *specular=Mesh->GetSpecular();
	return 0;
}
int cVisGeneric::SetObjectVisible(cUnknownClass *UObject,int visible)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	Mesh->SetVisible(visible);
	return 0;
}
int cVisGeneric::GetObjectVisible(cUnknownClass *UObject,int visible)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	return Mesh->GetVisible(visible);
}
int cVisGeneric::SetRenderObjectSwitch(int attribute)
{
	GlobalRenderTuning|=attribute;
//	GlobalObjectRenderSwitch|=attribute;
	return 0;
}
int cVisGeneric::GetRenderObjectSwitch(int attribute)
{
	return GlobalRenderTuning&attribute;
//	return GlobalObjectRenderSwitch&attribute;
}
int cVisGeneric::ClearRenderObjectSwitch(int attribute)
{
	GlobalRenderTuning&=~attribute;
//	GlobalObjectRenderSwitch&=~attribute;
	return 0;
}
cUnknownClass* cVisGeneric::FindObjectByFileName(const char *fname)
{ // поиск объекта на миру по имени файла объекта
	if(fname==0||fname[0]==0) return 0;
	return GetActiveScene()->GetM3D()->FindObjectByFileName(fname);
}
int cVisGeneric::LoadObject(cUnknownClass **UObject,XBuffer &buf,cUnknownClass *UParent)
{
	char marker=0;
	buf.read(&marker,sizeof(marker));
	int Type=0;
	buf.read(&Type,sizeof(Type));
	cMesh *Mesh=(cMesh*)this->CreateObject(M3D_KIND(Type),M3D_TYPE(Type));
	buf.read(&Mesh->ID,sizeof(Mesh->ID));
	buf.read(&Mesh->Unique,sizeof(Mesh->Unique));
	buf.read(&Mesh->Diffuse,sizeof(Mesh->Diffuse));
	buf.read(&Mesh->Specular,sizeof(Mesh->Specular));
	buf.read(&Mesh->Attribute,sizeof(Mesh->Attribute));
	buf.read(&Mesh->Scale,sizeof(Mesh->Scale));
	buf.read(&Mesh->LocalMatrix,sizeof(Mesh->LocalMatrix));
//	buf>Mesh->description;
#ifdef _MECHOSOMA_
	buf.read(&Mesh->arcaneData,sizeof(Mesh->arcaneData));
	buf.read(&Mesh->SoundID,sizeof(Mesh->SoundID));
#endif
	char CurrentChain=0;
	float period,phase,finish;
	buf.read(&CurrentChain,sizeof(CurrentChain));
	buf.read(&period,sizeof(period));
	buf.read(&phase,sizeof(phase));
	buf.read(&finish,sizeof(finish));
	Mesh->SetAnimation(period,phase,finish);
	if(Mesh->Frame) Mesh->Frame->SetCurrentChain(CurrentChain);
	if(marker&(1<<0))
	{
		cMesh *Child=0;
		this->LoadObject((cUnknownClass**)&Child,buf,Mesh);
		Mesh->AddChild(Child);
	}
	if(UParent==0) 
	{
		Mesh->ReCalcTotalBound();
		Mesh->ReCalcMatrix();
	}
	return 0;
}
int cVisGeneric::SaveObject(cUnknownClass *UObject,XBuffer &buf)
{
	cMesh *Mesh=(cMesh*)UObject;
	assert(Mesh);
	if(Mesh==0) return -1;
	if(Mesh->GetFileName()==0||Mesh->GetFileName()[0]==0)
	{
		char marker=0;
		if(Mesh->Child)	marker|=1<<0;
		buf.write(&marker,sizeof(marker));
		buf.write(&Mesh->Type,sizeof(Mesh->Type));
		buf.write(&Mesh->ID,sizeof(Mesh->ID));
		buf.write(&Mesh->Unique,sizeof(Mesh->Unique));
		buf.write(&Mesh->Diffuse,sizeof(Mesh->Diffuse));
		buf.write(&Mesh->Specular,sizeof(Mesh->Specular));
		buf.write(&Mesh->Attribute,sizeof(Mesh->Attribute));
		buf.write(&Mesh->Scale,sizeof(Mesh->Scale));
		buf.write(&Mesh->LocalMatrix,sizeof(Mesh->LocalMatrix));
//		buf<Mesh->description;
#ifdef _MECHOSOMA_
		buf.write(&Mesh->arcaneData,sizeof(Mesh->arcaneData));
		buf.write(&Mesh->SoundID,sizeof(Mesh->SoundID));
#endif
		char Chain=0;
		float period=Mesh->Frame->GetPeriod(),phase=Mesh->Frame->GetPhase(),finish=Mesh->Frame->GetFinish();
		if(Mesh->Frame) Chain=Mesh->Frame->GetCurrentChain();
		buf.write(&Chain,sizeof(Chain));
		buf.write(&period,sizeof(period));
		buf.write(&phase,sizeof(phase));
		buf.write(&finish,sizeof(finish));
	}
	for(cList *startList=Mesh->Child; startList; startList=startList->next)
		this->SaveObject(startList->Mesh,buf);
	return 0;
}
