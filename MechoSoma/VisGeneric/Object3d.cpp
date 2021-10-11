#include "BaseDefine.h"
#include "Object3d.h"
#include "PolyMgr.h"

#include "Math3d.h"
#include "SoftWare16.h"
#include "Dispatcher.h"
#include "TileMap.h"
#include "DrawPolygonShade.h"

#include "Scene.h"
#include "Frame.h"
#include "TexMgr.h"
#include "GameClient.h"
#include "GameClient.h"
#include "Win32f.h"
#include "my_STL.h"

#ifdef _MECHOSOMA_
#include "mch_common.h" // For far target
#include "MemoryHeap.h"

#include "XTList.h"
#include "aci_parser.h"
#include <list>
int m3dOpenResource(char* fname,XStream& fh);
#endif

unsigned int	CountUnique=1;
unsigned int	PolyCount=0,MeshCountTest=0,MeshCountDraw=0;
unsigned int	DebugPoly=0;
float PolyArea=0;
#ifdef _MECHOSOMA_
MemoryHeap MemoryHeapDynamicShade(150000);
#endif

sObjTri::sObjTri()												
{ 
	nPoint=nTexel=0; 
	p=0; 
}
sObjTri::~sObjTri()												
{ 
	if(p) delete p; p=0; 
}

void sTile::CalcLOD()
{
	Vect3f Min(1e30f,1e30f,1e30f),Max(-1e30f,-1e30f,-1e30f);
	sPoint *Point=GetPoint();
	for(int i=0;i<GetNumberPoint();i++)
	{
		if(Min.x>Point[i].pos.x) Min.x=Point[i].pos.x;
		if(Min.y>Point[i].pos.y) Min.y=Point[i].pos.y;
		if(Min.z>Point[i].pos.z) Min.z=Point[i].pos.z;
		if(Max.x<Point[i].pos.x) Max.x=Point[i].pos.x;
		if(Max.y<Point[i].pos.y) Max.y=Point[i].pos.y;
		if(Max.z<Point[i].pos.z) Max.z=Point[i].pos.z;
	}
	size=Max.distance(Min);
}
void sTile::CalcNormal()
{ // создание нормалей объекта
	if(GetNumberPoint()==0||GetNumberPolygon()==0) return;
	Vect3f *Normal3f=new Vect3f[GetNumberPoint()];
	sPoint *Point=GetPoint();
	sPolygon *Polygon=GetPolygon();
	int i;
	for(i=0;i<GetNumberPoint();i++)
		Normal3f[i].set(0,0,0);
	for(i=0;i<GetNumberPolygon();i++)
	{
		Vect3f n;
		sPolygon &p=Polygon[i];
		GetNormal((float*)&Point[p.p1].pos.x,(float*)&Point[p.p2].pos.x,(float*)&Point[p.p3].pos.x,(float*)&n.x);
		Normal3f[p.p1]+=n; Normal3f[p.p2]+=n; Normal3f[p.p3]+=n;
	}
	for(i=0;i<GetNumberPoint();i++)
		if(Normal3f[i].norm2()>0.001f) 
		{
			Normal3f[i].normalize(1);
			Point[i].SetNormal(Normal3f[i].x,Normal3f[i].y,Normal3f[i].z);
		}
		else Point[i].SetNormal(0,0,0);
	delete Normal3f;
}
void sTile::GetCopy(sTile *Tile)
{ 
	Tile->size=size;
	((cMaterial*)this)->GetCopy((cMaterial*)Tile);
	Tile->ClearAttribute();
	Tile->SetAttribute(ATTRTILE_COPY|GetAttribute());
	Tile->NewTri(GetNumberPolygon(),GetNumberPoint(),GetNumberTexel(),GetNumberTexPoly());
	memcpy(Tile->p,p,GET_POINTSIZE(GetNumberPolygon(),GetNumberPoint(),GetNumberTexel(),GetNumberTexPoly()));
	Tile->name=name;
	Tile->ID=ID;
}

sTile* cTile::AddTile(sTile *tile)
{
	assert(tile->p);
	this->Attach(tile);
	tile->CalcNormal();
	tile->CalcLOD();
	return tile;
}

cList* cList::Attach(cList *Link)
{
	cList *Next=next;
	Link->next=Next;
	Link->prev=this;
	next=Link;
	if(Next) Next->prev=Link;
	return Link;
}
cList* cList::Detach()
{
	if(prev) prev->next=next;
	if(next) next->prev=prev;
	next=prev=0;
	return this;
}
cMesh* cList::FindMesh(char *Name)
{
	cList *start=this;
	cMesh *tmp;
	do{
		if(start->Mesh->GetName()==Name) return start->Mesh;
		if((start->Mesh->Child)&&((tmp=start->Mesh->Child->FindMesh(Name))!=0)) 
			return tmp;
		start=start->next;
	} while(start);
	return 0;
}
cMesh* cList::FindMesh(unsigned int type)
{
	cList *start=this;
	cMesh *tmp;
	do{
		if(M3D_TYPE(start->Mesh->Type) == M3D_TYPE(type)) return start->Mesh;
		if((start->Mesh->Child)&&((tmp=start->Mesh->Child->FindMesh(type))!=0)) 
			return tmp;
		start=start->next;
	} while(start);
	return 0;
}

#define PREC_MESH			10

void sStaticMesh::Release()
{
	if(DecRef()) return;
	delete this;
}
void sStaticMesh::GetCopy(sStaticMesh* &StaticMesh)
{
	if(StaticMesh) StaticMesh->Release(); StaticMesh=this;
	IncRef(); 
}

void cMesh::Displace(float x,float y,float z)
{
	AssertValid();
	cBound::Real.Translate(Vect3f(x,y,z));
	Tile->TranslateTri(Vect3f(x,y,z));
	//cMesh::SetBound();
	Dummies.Translate(x,y,z);
	if(Bound) Bound->Displace(x,y,z);
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->Displace(x,y,z);
}
void cMesh::TestVisible(cUnknownClass *UCameraList)
{ // проводит тест на видимость
	cUnkClassDynArrayPointer &ArrayCamera=*(cUnkClassDynArrayPointer*)UCameraList;
	int TotalVisible=0;
	for(int nCamera=0;nCamera<ArrayCamera.length();nCamera++)
	{
		assert(ArrayCamera[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)ArrayCamera[nCamera];
		cMatrix DrawMatrix;
		DrawMatrix=Camera->GetMatrix()*BuildDrawMatrix(Camera,DrawMatrix);
		int test=Camera->TestVisible(DrawMatrix,Total.Min,Total.Max);
		GetVisibleTotal(nCamera)=(CONST_VISIBLE_SHOW&GetVisibleTotal(nCamera))|test;
		TotalVisible|=test;
	}
	if(!TotalVisible) return;
	if(Frame) 
		Frame->GetAnimKey(Tile);
	for(cList *start=Child; start; start=start->next)
		if(start->Mesh->isVisibleTotal(UCameraList)&CONST_VISIBLE_SHOW)
			start->Mesh->TestVisible(UCameraList);
}
void cMesh::Animate(float dt)
{	
	AssertValid();
	if(Frame && Frame->IsAnimated())
	{
		Frame->AddPhase(dt);
		if(Frame->mkf) Frame->GetMorph(this);
		else if(Frame->pkf||Frame->rkf||Frame->skf)
		{
			Vect3f dPos,dScale;
			float dwq,dxq,dyq,dzq;
			LocalMatrix.NewMatrix();
			char ch=Frame->GetScale(dScale);
			Vect3f s=dScale*Scale;
			LocalMatrix.Translate((Frame->xOfs-Frame->xCenter)*s.x,(Frame->yOfs-Frame->yCenter)*s.y,(Frame->zOfs-Frame->zCenter)*s.z);
			if(Frame->GetPosition(dPos)) LocalMatrix.Translate(dPos.x*s.x,dPos.y*s.y,dPos.z*s.z);
			if(Frame->GetRotation(&dwq,&dxq,&dyq,&dzq)) LocalMatrix.Rotate(dwq,dxq,dyq,dzq);
			LocalMatrix.Translate((Frame->xCenter-Frame->xPivot)*s.x,(Frame->yCenter-Frame->yPivot)*s.y,(Frame->zCenter-Frame->zPivot)*s.z);
			if(ch)LocalMatrix.Scale(dScale.x,dScale.y,dScale.z);
			CalcMatrix();
		}
	}
	for(cList *start=Child; start; start=start->next)
		start->Mesh->Animate(dt);
}
void cMesh::SetFrame(char *name,char *parent,float xpivot,float ypivot,float zpivot,float xofs,float yofs,float zofs,
			  float *PosKXYZ,int nPos,float *RotKWXYZ,int nRot,float *ScaleKXYZ,int nScale)
{ 
	Frame=new cFrame(name,parent,xpivot,ypivot,zpivot,xofs,yofs,zofs);
	Frame->SetPKFrame(PosKXYZ,nPos);
	Frame->SetRKFrame(RotKWXYZ,nRot);
	Frame->SetSKFrame(ScaleKXYZ,nScale); 
}
void cMesh::CalcMatrix()
{	
	AssertValid();
	if(Parent) GlobalMatrix=Parent->GlobalMatrix*LocalMatrix; else GlobalMatrix=LocalMatrix;
	if(Bound) { Bound->LocalMatrix=LocalMatrix; Bound->GlobalMatrix=GlobalMatrix; }
	for(cList *start=Child;start;start=start->next)
		start->Mesh->CalcMatrix();
}
void cMesh::SetVisible(int visible)
{ 
	AssertValid();
	if(visible)
		for(int i=0;i<NUMBER_CAMERA_MAX;i++)
			GetVisibleTotal(i)|=visible;
	else
		for(int i=0;i<NUMBER_CAMERA_MAX;i++)
			GetVisibleTotal(i)=CONST_VISIBLE_NULL;
	for(cList *start=Child; start; start=start->next)
		start->Mesh->SetVisible(visible);
}
void cMesh::Draw(cUnknownClass *UScene,cUnknownClass *UCameraList)
{
	AssertValid();
	assert(UScene->GetKind(KIND_SCENE));
	if(UCameraList==0) UCameraList=((cScene*)UScene)->GetCameraList();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	// отрисовка потомков, если видим хотя бы в одной камере
	for(cList *start=Child; start; start=start->next)
		if(start->Mesh->isVisibleTotal(UCameraList))
			start->Mesh->Draw(UScene,UCameraList);
	// если видим хотя бы в одной камере
	P3D->Draw(UScene,UCameraList,this);
	// статистическая информация, число отрисованных объектов
	MeshCountDraw++;
}
void cMesh::DrawReflection(cUnknownClass *UScene,cUnknownClass *UCameraList)
{
	AssertValid();
	assert(UScene->GetKind(KIND_SCENE));
	if(UCameraList==0) UCameraList=((cScene*)UScene)->GetCameraList();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	// отрисовка потомков, если видим хотя бы в одной камере
#ifdef _MECHOSOMA_
	if(GlobalRenderTuning&RENDER_TUNING_OBJECT_REFLECTION_MAP)
	{
		Vect3f pos(0,0,0);
		cScene *Scene=(cScene*)UScene;
		cTileMap *tMap=Scene->GetTileMap();
		if(Parent==0)
		{
			int xTile=XCYCL(round(x()))>>tMap->_SizeTileX(),yTile=YCYCL(round(y()))>>tMap->_SizeTileY();
			if(!tMap->GetTile(xTile,yTile)->GetAttribute(ATTR_TILE_REFLECTION)) return;
		}
		if(GlobalMatrix.trans().z<(pos.z=tMap->GetZReflectionSurface())) return;
		for(cList *start=Child; start; start=start->next)
			if(start->Mesh->isVisibleTotal(UCameraList))
				start->Mesh->DrawReflection(UScene,UCameraList);
		// если видим хотя бы в одной камере
		P3D->Draw(UScene,UCameraList,this,&pos);
	}
	else if(GlobalRenderTuning&RENDER_TUNING_OBJECT_REFLECTION_SURFACE)
	{
		Vect3f pos;
		BaseObject_SurfaceReflection_Pos(GlobalMatrix.trans(),pos);
		if((z()-rmaxTotal()*0.3f)<pos.z) return;
		Vect3f TransOld=GlobalMatrix.trans();
		GlobalMatrix.trans().x+=pos.x; GlobalMatrix.trans().y+=pos.y;
		for(cList *start=Child; start; start=start->next)
			if(start->Mesh->isVisibleTotal(UCameraList))
				start->Mesh->DrawReflection(UScene,UCameraList);
		// если видим хотя бы в одной камере
		P3D->Draw(UScene,UCameraList,this,&pos);
		GlobalMatrix.trans()=TransOld;
	}
#endif
	// статистическая информация, число отрисованных объектов
	MeshCountDraw++;
}
void cMesh::AttachChild(cMesh *child)
{
	AssertValid();
	ReCalcMatrix();
	child->ReCalcMatrix();
	SET_SHORT_DIST(child->GlobalMatrix.trans().x,child->GlobalMatrix.trans().y,GlobalMatrix.trans().x,GlobalMatrix.trans().y);
	float	dx=child->GlobalMatrix(0,3)-GlobalMatrix(0,3), 
			dy=child->GlobalMatrix(1,3)-GlobalMatrix(1,3), 
			dz=child->GlobalMatrix(2,3)-GlobalMatrix(2,3);
	cMatrix a; a.invert(GlobalMatrix);
	child->LocalMatrix=a*child->GlobalMatrix;
	AddChild(child);
	ReCalcTotalBound();
	ReCalcMatrix();
}
void cMesh::DetachChild()
{
	AssertValid();
	cMesh *tmp=GetGeneralParent();
	ReCalcMatrix();
	LocalMatrix=GlobalMatrix;
	if(Parent) Parent->DelChild(this);
	tmp->CalcTotalBound();
	tmp->Attribute^=MESH_HIERARCHY;
}
cList* cMesh::AddChild(cMesh *child)
{
	AssertValid();
	child->SetParent(this);
	if(Child==0) return Child=new cList(child);
	cList *NewChild=new cList(child);
	return Child->Attach(NewChild);
}
cMesh* cMesh::FindMesh(unsigned int type)
{		
	AssertValid();
	if(M3D_TYPE(Type) == M3D_TYPE(type)) return this;
	if(Child) return Child->FindMesh(type);
	return 0;
}

cMesh* cMesh::GetNumberMesh(int &number)
{
	AssertValid();
	if(number==0) return this;
	cMesh *tmp=0;
	for(cList *start=this->Child;start;start=start->next)
		if((start->Mesh->GetFileName()[0])&&((tmp=(*start->Mesh)[--number])!=0)) return tmp;
	return 0;
}

inline int GetNumberParent(cMesh *Mesh)
{
	int NumberParent=0;
	cMesh *Parent=Mesh->Parent;
	while(Parent) { NumberParent++; Parent=Parent->Parent; }
	return NumberParent;
}


cMesh* cMesh::operator[] (int number)
{
	AssertValid();
	if(GetFileName()[0]==0) return 0;
	if(number==0) return this;
	cMesh *tmp=0;
	for(cList *start=this->Child;start;start=start->next)
		if((start->Mesh->GetFileName()[0])&&((tmp=start->Mesh->GetNumberMesh(--number))!=0)) return tmp;
	return 0;
}
int cMesh::NumMesh()
{
	AssertValid();
	if(GetFileName()[0]==0) return 0;
	int count=1;
	for(cList *start=this->Child;start;start=start->next)
		count+=start->Mesh->NumMesh();
	return count;
}

//////////////////////////////////////////////////////////
//	Animation utils
//////////////////////////////////////////////////////////
class FindFrameOp
{
	cFrame* frame;
public:
	FindFrameOp(){ frame = 0; }
	int operator()(cMesh& mesh)
	{ 
		if(mesh.Frame){ 
			frame = mesh.Frame; 
			return 0; 
			}
		else
			return 1;
	}
	cFrame* result() const { return frame; }
};

cFrame* cMesh::FindFrame()
{
	FindFrameOp ff;
	if(ff(*this))
		ScanHierarchy(ff);
	return ff.result();
}

class SetAnimationOp
{
	int type;
	float time_of_animation;
	float start_phase;
	float finish_phase;
public:
	SetAnimationOp(int type_, float time_of_animation_, float start_phase_, float finish_phase_)
	{
		type = type_;
		time_of_animation = time_of_animation_;
		start_phase = start_phase_;
		finish_phase = finish_phase_;
	}

	int operator()(cMesh& mesh)
	{ 
		if(mesh.Frame && mesh.Type == type){
			mesh.Frame -> Set(time_of_animation, start_phase, finish_phase);
			mesh.CalcMatrix();
			cFrame* Frame = mesh.Frame;
			cMatrix& LocalMatrix = mesh.LocalMatrix;
			if(Frame->IsAnimated())
				if(Frame->pkf||Frame->rkf||Frame->skf)
				{
					Vect3f dPos,dScale;
					float dwq,dxq,dyq,dzq;
					LocalMatrix.NewMatrix();
					char ch=Frame->GetScale(dScale);
					Vect3f s=dScale*mesh.Scale;
					LocalMatrix.Translate((Frame->xOfs-Frame->xCenter)*s.x,(Frame->yOfs-Frame->yCenter)*s.y,(Frame->zOfs-Frame->zCenter)*s.z);
					if(Frame->GetPosition(dPos)) LocalMatrix.Translate(dPos.x*s.x,dPos.y*s.y,dPos.z*s.z);
					if(Frame->GetRotation(&dwq,&dxq,&dyq,&dzq)) LocalMatrix.Rotate(dwq,dxq,dyq,dzq);
					LocalMatrix.Translate((Frame->xCenter-Frame->xPivot)*s.x,(Frame->yCenter-Frame->yPivot)*s.y,(Frame->zCenter-Frame->zPivot)*s.z);
					if(ch)LocalMatrix.Scale(dScale.x,dScale.y,dScale.z);
				}
			}
		return 1;
	}
};

void cMesh::SetFirstAnimation(float time_of_animation, float start_phase, float finish_phase)
{
	SetAnimationOp op(Type, time_of_animation, start_phase, finish_phase);
	op(*this);
	ScanHierarchy(op);
}

void cMesh::SetAnimation(float time_of_animation, float start_phase, float finish_phase,int mask,int count)
{
	if(mask&Type)
	{
		if(Frame)  
		{
			Frame->Set(time_of_animation, start_phase, finish_phase);
			if(Frame->IsAnimated())
				if(Frame->pkf||Frame->rkf||Frame->skf)
				{
					Vect3f dPos,dScale;
					float dwq,dxq,dyq,dzq;
					LocalMatrix.NewMatrix();
					char ch=Frame->GetScale(dScale);
					Vect3f s=dScale*Scale;
					LocalMatrix.Translate((Frame->xOfs-Frame->xCenter)*s.x,(Frame->yOfs-Frame->yCenter)*s.y,(Frame->zOfs-Frame->zCenter)*s.z);
					if(Frame->GetPosition(dPos)) LocalMatrix.Translate(dPos.x*s.x,dPos.y*s.y,dPos.z*s.z);
					if(Frame->GetRotation(&dwq,&dxq,&dyq,&dzq)) LocalMatrix.Rotate(dwq,dxq,dyq,dzq);
					LocalMatrix.Translate((Frame->xCenter-Frame->xPivot)*s.x,(Frame->yCenter-Frame->yPivot)*s.y,(Frame->zCenter-Frame->zPivot)*s.z);
					if(ch)LocalMatrix.Scale(dScale.x,dScale.y,dScale.z);
				}
		}
	}
	else count=-1;
	for(cList *start=this->Child;start;start=start->next)
		start->Mesh->SetAnimation(time_of_animation, start_phase, finish_phase, mask, count+1);
	if(!count) 
		CalcMatrix();
}
void cMesh::GetAnimation(float &animation_period,float &start_phase,float &finish)
{
	animation_period=start_phase=finish=0;
	if(Frame)
	{ animation_period=Frame->GetPeriod(); start_phase=Frame->GetStartPhase(); finish=Frame->GetFinish(); return; }
	for(cList *start=this->Child;start;start=start->next)
		start->Mesh->GetAnimation(animation_period,start_phase,finish);
}
void cMesh::GetAnimation(float &animation_period,float &start_phase)
{
	animation_period=start_phase=0;
	if(Frame)
	{ animation_period=Frame->GetPeriod(); start_phase=Frame->GetStartPhase(); return; }
	for(cList *start=this->Child;start;start=start->next)
		start->Mesh->GetAnimation(animation_period,start_phase);
}
cMesh::cMesh() : cUnknownClass(KIND_MESH)
{ 
	Frame=0; Parent=0; Child=0; Bound=0; description=0; StaticMesh=0; Tile=0;
	Scale.set(1,1,1); Diffuse.set(1,1,1,1); Specular.set(0,0,0,1);
	memset(TotalVisibleCamera,CONST_VISIBLE_SHOW,NUMBER_CAMERA_MAX);
	Attribute=MESH_ATTRIBUTE_NULL;
	SetVisible(CONST_VISIBLE_SHOW); ID=0; Type=0; Unique=0;
	GlobalMatrix.NewMatrix();
	LocalMatrix.NewMatrix();
#ifdef _SURMAP_
	NumberTrack=STATIC_NUMBER;
#endif
	AssertValid();
}
cMesh::~cMesh()												
{ 
	UNKNOWN_DESTRUCTOR; 
//	Release(); 
}

void cMesh::DelChild(cMesh *child)
{
	AssertValid();
	if(this==0) 
		return;
	cList *tmp=Child;
	while((tmp)&&(tmp->Mesh!=child))
		tmp=tmp->next;
	if(tmp==Child) Child=tmp->next; 
	if(tmp) delete tmp; 
	child->Parent=0; 
}
cMesh* cMesh::FindMesh(char *Name)
{
	AssertValid();
	if(Name[0]==0) return 0;
	if(GetName()==Name) return this;
	if(Child) return Child->FindMesh(Name);
	return 0;
}
cMesh* cMesh::FindUnique(unsigned int unique)
{
	AssertValid();
	cMesh *tmp;
	if(unique==Unique) return this;
	for(cList *start=Child;start;start=start->next)
		if((tmp=start->Mesh->FindUnique(unique))!=0) return tmp;
	return 0;
}
void cMesh::Release()	// єфрыхэшх юс·хъЄр ё хую яюЄюьърьш
{ 
	AssertValid();
	if(description) delete description; description=0;
	if(Tile)
	{
		if(StaticMesh) { StaticMesh->Release(); StaticMesh=0; }
		if(Parent) Parent->DelChild(this);
		if(Bound) { Bound->SetParent(0); delete Bound; } Bound=0;
		if(Frame) {	delete Frame; Frame=0; }
		if(Child)
		{
			cList *start=Child,*tmp;
			while((tmp=start)!=0)
			{
				start=start->next;
				delete tmp->Mesh;
			}
			Child=0;
		}
		if(GetAttribute(MESH_CREATETILE))
			delete Tile;
		Tile=0;
		DBGCHECK;
	}
}
void cMesh::ConvertPointShadeDynamic(sBox6f &box,cMatrix &LightMatrix)
{
	AssertValid();
	cConvertor Convertor;
	Convertor.GetMatrix()=LightMatrix*GlobalMatrix;
	Convertor.BuildMatrix(Scale);
	for(int nTile=0;nTile<GetNumberTile();nTile++)
	{
		sTile *tmp=GetTile(nTile);
		if(!tmp->GetAttribute(ATTRMAT_MASK))
		{
			sPoint *Point=tmp->GetPoint();
			for(int nVertex=0;nVertex<tmp->GetNumberPoint();nVertex++)
			{
				sVertexFix *vFix=&P3D->PointFix[P3D->PointFix.length()+nVertex];
				Convertor.ConvertPoint(Point[nVertex].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=tmp->GetNumberPoint();
		}
		else if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX_TEXEL))
		{
			sPoint *Point=tmp->GetPoint();
			sTexel *Texel=tmp->GetTexel();
			for(int nVertex=0;nVertex<tmp->GetNumberPoint();nVertex++)
			{
				sVertexFix *vFix=&P3D->PointFix[P3D->PointFix.length()+nVertex];
				Convertor.ConvertPoint(Point[nVertex].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				vFix->u1()=Texel[nVertex].u();
				vFix->v1()=Texel[nVertex].v();
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=tmp->GetNumberPoint();
		}
		else
		{
			sPoint *Point=tmp->GetPoint();
			sTexel *Texel=tmp->GetTexel();
			sPolygon *Polygon=tmp->GetPolygon();
			sPolygon *TexPoly=tmp->GetTexPoly();
			for(int nPolygon=0;nPolygon<tmp->GetNumberPolygon();nPolygon++)
			{
				int i3=3*nPolygon;
				sVertexFix *vFix;
				sPolygon &p=Polygon[nPolygon];
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+0];
				Convertor.ConvertPoint(Point[p.p1].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				vFix->u1()=Texel[p.p1].u();
				vFix->v1()=Texel[p.p1].v();
				box.SetBound(*(Vect3f*)vFix);
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+1];
				Convertor.ConvertPoint(Point[p.p2].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				vFix->u1()=Texel[p.p2].u();
				vFix->v1()=Texel[p.p2].v();
				box.SetBound(*(Vect3f*)vFix);
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+2];
				Convertor.ConvertPoint(Point[p.p3].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				vFix->u1()=Texel[p.p3].u();
				vFix->v1()=Texel[p.p3].v();
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=3*tmp->GetNumberPolygon();
		}
	}
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->ConvertPointShadeDynamic(box,LightMatrix);
}
void cMesh::ConvertPointShadeDynamic(const sBox6f &box,float dzx)
{
	AssertValid();
	for(int nPoint=0;nPoint<P3D->PointFix.length();nPoint++)
	{
		sVertexFix *vFix=&P3D->PointFix[nPoint];
		vFix->xe=(vFix->z-box.zmin())*dzx+(box.xmax()-vFix->xe);
		vFix->ye=vFix->ye-box.ymin();
	}
}
void cMesh::ConvertPointShadeStatic(sBox6f &box,cMatrix &LightMatrix,Vect3f &PosLight,float zProjection)
{
	AssertValid();
	cConvertor Convertor;
	Convertor.GetMatrix()=LightMatrix*GlobalMatrix;
	Convertor.BuildMatrix(Scale);
	for(int nTile=0;nTile<GetNumberTile();nTile++)
	{
		sTile *tmp=GetTile(nTile);
		if(!tmp->GetAttribute(ATTRMAT_MASK))
		{
			sPoint *Point=tmp->GetPoint();
			for(int nVertex=0;nVertex<tmp->GetNumberPoint();nVertex++)
			{
				sVertexFix *vFix=&P3D->PointFix[P3D->PointFix.length()+nVertex];
				Convertor.ConvertPoint(Point[nVertex].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				float f=(zProjection-vFix->z)/(PosLight.z-vFix->z);
				vFix->xe=vFix->xe+f*(PosLight.x-vFix->xe);
				vFix->ye=vFix->ye+f*(PosLight.y-vFix->ye);
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=tmp->GetNumberPoint();
		}
		else if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX_TEXEL))
		{
			sPoint *Point=tmp->GetPoint();
			sTexel *Texel=tmp->GetTexel();
			for(int nVertex=0;nVertex<tmp->GetNumberPoint();nVertex++)
			{
				sVertexFix *vFix=&P3D->PointFix[P3D->PointFix.length()+nVertex];
				Convertor.ConvertPoint(Point[nVertex].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				float f=(zProjection-vFix->z)/(PosLight.z-vFix->z);
				vFix->xe=vFix->xe+f*(PosLight.x-vFix->xe);
				vFix->ye=vFix->ye+f*(PosLight.y-vFix->ye);
				vFix->u1()=Texel[nVertex].u();
				vFix->v1()=Texel[nVertex].v();
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=tmp->GetNumberPoint();
		}
		else
		{
			sPoint *Point=tmp->GetPoint();
			sTexel *Texel=tmp->GetTexel();
			sPolygon *Polygon=tmp->GetPolygon();
			for(int nPolygon=0;nPolygon<tmp->GetNumberPolygon();nPolygon++)
			{
				float f;
				sVertexFix *vFix;
				int i3=nPolygon*3;
				sPolygon &p=Polygon[nPolygon];
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+0];
				Convertor.ConvertPoint(Point[p.p1].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				f=(zProjection-vFix->z)/(PosLight.z-vFix->z);
				vFix->xe=vFix->xe+f*(PosLight.x-vFix->xe);
				vFix->ye=vFix->ye+f*(PosLight.y-vFix->ye);
				vFix->u1()=Texel[p.p1].u();
				vFix->v1()=Texel[p.p1].v();
				box.SetBound(*(Vect3f*)vFix);
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+1];
				Convertor.ConvertPoint(Point[p.p2].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				f=(zProjection-vFix->z)/(PosLight.z-vFix->z);
				vFix->xe=vFix->xe+f*(PosLight.x-vFix->xe);
				vFix->ye=vFix->ye+f*(PosLight.y-vFix->ye);
				vFix->u1()=Texel[p.p2].u();
				vFix->v1()=Texel[p.p2].v();
				box.SetBound(*(Vect3f*)vFix);
				vFix=&P3D->PointFix[P3D->PointFix.length()+i3+2];
				Convertor.ConvertPoint(Point[p.p3].pos,*(Vect3f*)vFix);
				vFix->z=-vFix->z;
				f=(zProjection-vFix->z)/(PosLight.z-vFix->z);
				vFix->xe=vFix->xe+f*(PosLight.x-vFix->xe);
				vFix->ye=vFix->ye+f*(PosLight.y-vFix->ye);
				vFix->u1()=Texel[p.p3].u();
				vFix->v1()=Texel[p.p3].v();
				box.SetBound(*(Vect3f*)vFix);
			}
			P3D->PointFix.length()+=3*tmp->GetNumberPolygon();
		}
	}
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->ConvertPointShadeStatic(box,LightMatrix,PosLight,zProjection);
}
void cMesh::ConvertPointShadeStatic(const sBox6f &box)
{
	AssertValid();
	for(int nPoint=0;nPoint<P3D->PointFix.length();nPoint++)
	{
		sVertexFix *vFix=&P3D->PointFix[nPoint];
		vFix->xe=box.xmax()-vFix->xe; 
		vFix->ye=vFix->ye-box.ymin();
	}
}
void cMesh::DrawShadeDynamic(short *shade,int xShade,int yShade,int &nPointBegin)
{
	AssertValid();
	for(int nTile=0;nTile<GetNumberTile();nTile++)
	{
		sTile *tmp=GetTile(nTile);
		if(tmp->Texture) P3D->SetTextureShade(tmp->Texture);
		if(!tmp->GetAttribute(ATTRMAT_MASK))
		{
			for(sPolygon *p=tmp->GetPolygon(),*end=&p[tmp->GetNumberPolygon()];p<end;p++)
				P3D->DrawPolygonShade(P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
			nPointBegin+=tmp->GetNumberPoint();
		}
		else if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX|ATTRTILE_VERTEX_TEXEL))
		{
			for(sPolygon *p=tmp->GetPolygon(),*end=&p[tmp->GetNumberPolygon()];p<end;p++)
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
			nPointBegin+=tmp->GetNumberPoint();
		}
		else
		{
			for(int i3=0;i3<3*tmp->GetNumberPolygon();i3+=3)
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+i3+0],
					P3D->PointFix[nPointBegin+i3+1],
					P3D->PointFix[nPointBegin+i3+2],
					shade,0,0,xShade,yShade);
			nPointBegin+=3*tmp->GetNumberPolygon();
		}
	}
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->DrawShadeDynamic(shade,xShade,yShade,nPointBegin);
}
void cMesh::DrawShadeStatic(short *shade,int xShade,int yShade,int &nPointBegin)
{
	AssertValid();
	for(int nTile=0;nTile<GetNumberTile();nTile++)
	{
		sTile *tmp=GetTile(nTile);
		if(tmp->Texture) P3D->SetTextureShade(tmp->Texture);
		if(!tmp->GetAttribute(ATTRMAT_MASK))
		{
			for(sPolygon *p=tmp->GetPolygon(),*end=&p[tmp->GetNumberPolygon()];p<end;p++)
			{
				P3D->DrawPolygonShade(P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
				P3D->DrawPolygonShade(P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
			}
			nPointBegin+=tmp->GetNumberPoint();
		}
		else if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX|ATTRTILE_VERTEX_TEXEL))
		{
			for(sPolygon *p=tmp->GetPolygon(),*end=&p[tmp->GetNumberPolygon()];p<end;p++)
			{
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+p->p2],
					P3D->PointFix[nPointBegin+p->p1],
					P3D->PointFix[nPointBegin+p->p3],
					shade,0,0,xShade,yShade);
			}
			nPointBegin+=tmp->GetNumberPoint();
		}
		else
		{
			for(int i3=0;i3<3*tmp->GetNumberPolygon();i3+=3)
			{
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+i3+0],
					P3D->PointFix[nPointBegin+i3+1],
					P3D->PointFix[nPointBegin+i3+2],
					shade,0,0,xShade,yShade);
				P3D->DrawPolygonShadeMask(P3D->PointFix[nPointBegin+i3+1],
					P3D->PointFix[nPointBegin+i3+0],
					P3D->PointFix[nPointBegin+i3+2],
					shade,0,0,xShade,yShade);
			}
			nPointBegin+=3*tmp->GetNumberPolygon();
		}
	}
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->DrawShadeStatic(shade,xShade,yShade,nPointBegin);
}
void cMesh::BuildShadeDynamic(short **shade,int *xShade,int *yShade,Vect3f &PosShade,Vect3f &PosLight)
{
	AssertValid();
	float dx=GetShortDistX(PosLight.x,GlobalMatrix.trans().x),dy=GetShortDistY(PosLight.y,GlobalMatrix.trans().y);
//	float dzx=sqrt(dx*dx+dy*dy)/(PosLight.z-GlobalMatrix.trans().z);
	float dzx=PosLight.z;
	cMatrix LightMatrix;
	LightMatrix.NewMatrix();
	LightMatrix.Translate(cMesh::x(),cMesh::y(),cMesh::z());
	LightMatrix.Rotate(0,0,-atan2(dy,dx)/GRAD_RAD);
	LightMatrix.Translate(-cMesh::x(),-cMesh::y(),-cMesh::z());
	sBox6f Box(1e30f,1e30f,1e30f,-1e30f,-1e30f,-1e30f);
	P3D->PointFix.length()=0;
	ConvertPointShadeDynamic(Box,LightMatrix);
	ConvertPointShadeDynamic(Box,dzx);
	(*xShade)=round((Box.zmax()-Box.zmin())*dzx+(Box.xmax()-Box.xmin()))+1; (*yShade)=(Box.ymax()-Box.ymin())+1;
	if(Box.zmax()<0)		   
	{ 
#ifdef _DEBUG
//		XCon<"zvMax shade = "<=zvMax<"\r\n"; 
#endif
		*xShade=*yShade=0;
		(*shade)=0; return; 
	}
	if(((*xShade)>1024)||((*yShade)>1024)) 
	{ // очень большая тень отсекается
#ifdef _DEBUG
		if((*xShade)>1024) XCon<"Error : cMesh::BuildShadeStatic\r\nBig xSize shade = "<=(*xShade)<"\r\n"; 
		if((*yShade)>1024) XCon<"Error : cMesh::BuildShadeStatic\r\nBig ySize shade = "<=(*yShade)<"\r\n"; 
#endif
		*xShade=*yShade=0;
		(*shade)=0; return; 
	}
#ifdef _MECHOSOMA_
	if(!IS_STATIC(Type))
		(*shade)=(short*)MemoryHeapDynamicShade.malloc(((*xShade)*(*yShade)+1)*sizeof(short));
	else
#endif
		(*shade)=new short [(*xShade)*(*yShade)+1];
	assert(shade);
	memfill((unsigned long*)(*shade),((*xShade)*(*yShade)+1)>>1,0x80008000);
	int nPointFix=0;
	DrawShadeDynamic((*shade),*xShade,*yShade,nPointFix);
	PosShade.set(Box.xmin()+*xShade,Box.ymin(),Box.zmax());
	PosShade=LightMatrix.invXformPoint(PosShade);
	PosShade.x=Cycl(PosShade.x,2048.f); PosShade.y=Cycl(PosShade.y,2048.f);
/*	extern unsigned short *zBuffer;
	extern cInterfaceGraph3d *gb_IGraph3d;
	memset(zBuffer,0xFF,640*480*2);
	unsigned short *scr=(unsigned short*)gb_IGraph3d->GetScrBuffer();
	for(int j=0;j<*yShade;j++)
		for(int i=0;i<*xShade;i++)
		{
			zBuffer[640*(j+50)+i+10]=0;
			if((*shade)[i+j*(*xShade)]>0)
				scr[640*(j+50)+i+10]=0xFFFF;
			else
				scr[640*(j+50)+i+10]=0x0000;
		}
		*/
}
void cMesh::BuildShadeStatic(short **shade,int *xShade,int *yShade,Vect3f &PosShade,Vect3f &PosLight,float Radius)
{
	AssertValid();
	float dx=GetShortDistX(PosLight.x,GlobalMatrix.trans().x),
		dy=GetShortDistY(PosLight.y,GlobalMatrix.trans().y),
//		zDist=cMesh::z()+rmaxTotal();
		zDist=0;
	PosLight.x=GlobalMatrix.trans().x+dx,
	PosLight.y=GlobalMatrix.trans().y+dy;
	Vect3f PosLightWorld=PosLight;
	float dzx=PosLight.z;
	PosLight.z=cMesh::z()+sqrt(dx*dx+dy*dy)/dzx;
	if(PosLight.z<cMesh::z()) 	{ *xShade=*yShade=0; (*shade)=0; return; }
//	PosLight.z=sqrt(dx*dx+dy*dy)/dzx;
	cMatrix LightMatrix;
	LightMatrix.NewMatrix();
	LightMatrix.Translate(cMesh::x(),cMesh::y(),cMesh::z());
	LightMatrix.Rotate(0,0,-atan2(dy,dx)/GRAD_RAD);
	LightMatrix.Translate(-cMesh::x(),-cMesh::y(),-cMesh::z());
//	PosLight=LightMatrix.invXformPoint(PosLight);
	PosLight.x=cMesh::x()+sqrt(dx*dx+dy*dy);
	PosLight.y=cMesh::y(); 
	sBox6f Box(1e30f,1e30f,1e30f,-1e30f,-1e30f,-1e30f);
	P3D->PointFix.length()=0;
	ConvertPointShadeStatic(Box,LightMatrix,PosLight,zDist);
	ConvertPointShadeStatic(Box);
	(*xShade)=round(Box.xmax()-Box.xmin())+1; (*yShade)=round(Box.ymax()-Box.ymin())+1;
	PosShade.set(Box.xmin()+*xShade,Box.ymin(),zDist);
	PosShade=LightMatrix.invXformPoint(PosShade);
	dx=PosShade.x-(cMesh::x()+dx), dy=PosShade.y-(cMesh::y()+dy);
	if(sqrt(dx*dx+dy*dy)>=(2048.f*0.5f))
	{ *xShade=*yShade=0; (*shade)=0; return; }
//	PosShade.x=Cycl(PosShade.x,2048.f); PosShade.y=Cycl(PosShade.y,2048.f);
	PosShade.x=dx; PosShade.y=dy;

	Vect3f CenterShade(Box.xmin()+*xShade,Box.ymin()+(*yShade)/2,zDist);
	CenterShade=LightMatrix.invXformPoint(CenterShade);
	dx=CenterShade.x-(cMesh::x()+dx), dy=CenterShade.y-(cMesh::y()+dy);
	CenterShade.x=Cycl(CenterShade.x,2048.f); CenterShade.y=Cycl(CenterShade.y,2048.f);
	int x=GetShortDistX(CenterShade.x,PosLightWorld.x),y=GetShortDistX(CenterShade.y,PosLightWorld.y);
	float r=sqrt((float)(x*x+y*y));
	if((r+(*xShade))>Radius) *xShade=Radius-r;
	if((*xShade)<0) { *xShade=*yShade=0; (*shade)=0; return; }
	if(Box.zmax()<0)		   
	{ 
#ifdef _DEBUG
		XCon<"zvMax shade = "<=Box.zmax()<"\r\n"; 
#endif
		*xShade=*yShade=0;
		(*shade)=0; return; 
	}
	if((!IS_STATIC(Type))&&((*xShade)>1024)) 
	{ 
#ifdef _DEBUG
		XCon<"Error : cMesh::BuildShadeStatic\r\nBig xSize shade = "<=(*xShade)<"\r\n"; 
#endif
		*xShade=*yShade=0;
		(*shade)=0; return; 
	}
	if((!IS_STATIC(Type))&&((*yShade)>1024)) 
	{ 
#ifdef _DEBUG
		XCon<"Error : cMesh::BuildShadeStatic\r\nBig ySize shade = "<=(*yShade)<"\r\n"; 
#endif
		*xShade=*yShade=0;
		(*shade)=0; return; 
	}
#ifdef _MECHOSOMA_
	if(!IS_STATIC(Type))
		(*shade)=(short*)MemoryHeapDynamicShade.malloc(((*xShade)*(*yShade)+1)*sizeof(short));
	else
#endif
		(*shade)=new short [(*xShade)*(*yShade)+1];
	assert(shade);
	memfill((unsigned long*)(*shade),((*xShade)*(*yShade)+1)>>1,0x80008000);
	int nPointFix=0;
	DrawShadeStatic((*shade),*xShade,*yShade,nPointFix);
/*	if(strstr(fname,"ngine")!=0)
	{
		for(int j=0;j<*yShade;j++)
			for(int i=0;i<*xShade;i++)
				if((*shade)[i+j*(*xShade)]>0)
					scr[j*xScr+i+100]=0xFFFF;
				else
					scr[j*xScr+i+100]=0;
		XGR_Flush(0,0,640,480);
	}*/
}
void cMesh::ShadeDynamic(cUnknownClass *UTileMap)
{
	AssertValid();
	if(Attribute&(MESH_NOT_SHADE|MESH_STATIC_SHADE)) return;
	short *shade;
	Vect3f PosShade;
	int xShade,yShade;
	Vect3f PosLight(GlobalMatrix.trans().x+100,GlobalMatrix.trans().y,1/CONST_DYNAMIC_SHADE_MESH);
	BuildShadeDynamic(&shade,&xShade,&yShade,PosShade,PosLight);
//	BuildShadeStatic(&shade,&xShade,&yShade,PosShade,PosLight);
	if(shade) 
		P3D->BuildShade(shade,xShade,yShade,PosShade,(cTileMap*)UTileMap);
}
void cMesh::ScaleMesh(Vect3f &dScale)
{
	AssertValid();
	Scale*=dScale;
	cBound::Scale(Scale); 
	Dummies.Scale(dScale.x,dScale.y,dScale.z);
	if(Bound) Bound->ScaleMesh(dScale);
	if(Parent) LocalMatrix.trans()*=dScale.x;
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->ScaleMesh(dScale);
}
void cMesh::CreateCopy(unsigned int type,cMesh *Mesh,cMesh *MeshParent)
{
	AssertValid();
	if((type!=Type)&&(Type)) { XBuffer buf; buf<"Error: cMesh::CreateCopy\r\nType mesh != type ( "<=Type<" , "<=type<" )"; ErrAbort(buf.address()); return; }
	if(Mesh->StaticMesh) Mesh->StaticMesh->Release();
	if(Mesh->GetAttribute(MESH_CREATETILE)) { delete Mesh->Tile; Mesh->Tile=0; }
	*Mesh = *this; 
	Mesh->Unique=CountUnique++; 
	assert(CountUnique<0x0FFFFFFF);
	Mesh->Child=0; Mesh->StaticMesh=0; Mesh->Bound=0;
	if(StaticMesh) StaticMesh->GetCopy(Mesh->StaticMesh);
	if(Bound) { Mesh->Bound=new cMesh; Bound->CreateCopy(M3D_BOUND,Mesh->Bound,0); Mesh->Bound->SetParent(Mesh); }
	if(Frame) Mesh->Frame=new cFrame(Frame);
	if((Frame)&&((Frame->mkf)||(Frame->GetNumberAnimChain())))
	{
		Mesh->Tile=0;
		Tile->CreateCopy(Mesh->Tile);
		Mesh->SetAttribute(MESH_CREATETILE);
	}
	else 
	{
		Mesh->Tile=Tile;
		Mesh->ClearAttribute(MESH_CREATETILE);
	}
	if(Parent) Mesh->SetParent(MeshParent);
	for(cList *startList=Child; startList; startList=startList->next)
	{
		cMesh *tmp=new cMesh;
		startList->Mesh->CreateCopy(type,tmp,Mesh);
		Mesh->AddChild(tmp);
	}
}

Vect3f cMesh::CalcDummyOffset(cMesh* parent)
{
	return parent -> Dummies.Find(M3D_TYPE(Type)) - Dummies.Find(M3D_TYPE(parent -> Type));
}	

void cMesh::CreateCopyMesh(unsigned int type,cMesh *Mesh,cMesh *MeshParent)
{
	AssertValid();
	if((type!=Type)&&(Type)) { XBuffer buf; buf<"Error: cMesh::CreateCopy\r\nType mesh != type ( "<=Type<" , "<=type<" )"; ErrAbort(buf.address()); return; }
	if(Mesh->StaticMesh) Mesh->StaticMesh->Release();
	if(Mesh->GetAttribute(MESH_CREATETILE)) { delete Mesh->Tile; Mesh->Tile=0; }
	memcpy((cBound*)Mesh,(cBound*)this,sizeof(cBound)); Mesh->Dummies=this->Dummies;
	Mesh->Attribute=Attribute; Mesh->Type=Type;
	Mesh->StaticMesh=0; Mesh->Child=0; Mesh->Bound=0;
	if(StaticMesh) StaticMesh->GetCopy(Mesh->StaticMesh); 
	if(Bound) { Mesh->Bound=new cMesh; Bound->CreateCopy(M3D_BOUND,Mesh->Bound,0); Mesh->Bound->SetParent(Mesh); }
	if(Frame) Mesh->Frame=new cFrame(Frame);
	if((Frame)&&(Frame->mkf))
	{
		Mesh->Tile=0;
		Tile->CreateCopy(Mesh->Tile);
		Mesh->SetAttribute(MESH_CREATETILE);
	}
	else 
	{
		Mesh->Tile=Tile;
		Mesh->ClearAttribute(MESH_CREATETILE);
	}
	if((Parent)&&(MeshParent)) Mesh->SetParent(MeshParent);
	for(cList *startList=Child; startList; startList=startList->next)
	{
		cMesh *tmp=new cMesh;
		startList->Mesh->CreateCopy(type,tmp,Mesh);
		Mesh->AddChild(tmp);
	}
	if(MeshParent==0)
	{
		if(Mesh->Parent)
			Mesh->LocalMatrix.trans() = Mesh -> CalcDummyOffset( Mesh->Parent );
		Vect3f scale=Mesh->GetScale();
		if((scale.x!=1.f)||(scale.x!=1.f)||(scale.y!=1.f))
		{
			Mesh->GetScale().set(1,1,1);
			Mesh->ScaleMesh(scale);
		}
	}
}
void cMesh::CreateCopyMem(unsigned int type,cMesh *Mesh,cMesh *MeshParent)
{
	AssertValid();
	if((type!=Type)&&(Type)) { XBuffer buf; buf<"Error: cMesh::CreateCopy\r\nType mesh != type ( "<=Type<" , "<=type<" )"; ErrAbort(buf.address()); return; }
	if(Mesh->StaticMesh) Mesh->StaticMesh->Release();
	if(Mesh->GetAttribute(MESH_CREATETILE)) { delete Mesh->Tile; Mesh->Tile=0; }
	*Mesh = *this; 
	Mesh->Unique=CountUnique++; assert(CountUnique<0x0FFFFFFF);
	Mesh->StaticMesh=0; Mesh->Child=0; Mesh->Bound=0; 
	if(StaticMesh) StaticMesh->GetCopy(Mesh->StaticMesh);
	if(Bound) { Mesh->Bound=new cMesh; Bound->CreateCopy(M3D_BOUND,Mesh->Bound,0); Mesh->Bound->SetParent(Mesh); }
	Mesh->SetAttribute(MESH_CREATETILE);
	if(Frame) Mesh->Frame=new cFrame(Frame);
	Mesh->Tile=0;
	Tile->CreateCopy(Mesh->Tile);
	if(Parent) Mesh->SetParent(MeshParent);
	for(cList *startList=Child; startList; startList=startList->next)
	{
		cMesh *tmp=new cMesh;
		startList->Mesh->CreateCopyMem(type,tmp,Mesh);
		Mesh->AddChild(tmp);
	}
}
void cMesh::GetTotalBound(cMesh *center,sBox6f &box)
{
	AssertValid();
	Vect3f Min,Max;
	cBound::Real.GetOriginal(Min,Max);
	cMesh *tmp=this;
	Vect3f divScale=Vect3f(1,1,1)/GetScale();
	while((tmp->Parent)&&(tmp!=center)) 
	{ 
		if(GetScale().x!=0) { Min.x+=tmp->x()*divScale.x; Max.x+=tmp->x()*divScale.x; }
		if(GetScale().y!=0) { Min.y+=tmp->y()*divScale.y; Max.y+=tmp->y()*divScale.y; }
		if(GetScale().z!=0) { Min.z+=tmp->z()*divScale.z; Max.z+=tmp->z()*divScale.z; }
		tmp=tmp->Parent;
	}
	if(Min.x<box.xmin()) box.xmin()=Min.x; if(Min.y<box.ymin()) box.ymin()=Min.y; if(Min.z<box.zmin()) box.zmin()=Min.z;
	if(Max.x>box.xmax()) box.xmax()=Max.x; if(Max.y>box.ymax()) box.ymax()=Max.y; if(Max.z>box.zmax()) box.zmax()=Max.z;
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->GetTotalBound(center,box);
}
void cMesh::GetTotalBound(int types,cMesh *center,sBox6f &box)
{
	AssertValid();
	Vect3f Min,Max;
	cBound::Real.GetOriginal(Min,Max);
	Min*=Scale; Max*=Scale;
	cMesh *tmp=this;
	if(types & M3D_TYPE(Type))
		while((tmp->Parent)&&(tmp!=center)) 
		{ 
			if(Scale.x!=0) { Min.x+=tmp->x(); Max.x+=tmp->x(); }
			if(Scale.y!=0) { Min.y+=tmp->y(); Max.y+=tmp->y(); }
			if(Scale.z!=0) { Min.z+=tmp->z(); Max.z+=tmp->z(); }
			tmp=tmp->Parent;
		}
	if(Min.x<box.xmin()) box.xmin()=Min.x; if(Min.y<box.ymin()) box.ymin()=Min.y; if(Min.z<box.zmin()) box.zmin()=Min.z;
	if(Max.x>box.xmax()) box.xmax()=Max.x; if(Max.y>box.ymax()) box.ymax()=Max.y; if(Max.z>box.zmax()) box.zmax()=Max.z;
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->GetTotalBound(types,center,box);
}
void cMesh::CalcTotalBound()
{
	AssertValid();
	sBox6f Box;
	cBound::Real.GetOriginal(Box.min,Box.max);
	cList *startList;
	for(startList=Child; startList; startList=startList->next)
		startList->Mesh->GetTotalBound(this,Box);
	cBound::SetTotal(Box.min,Box.max);
	for(startList=Child; startList; startList=startList->next)
		startList->Mesh->CalcTotalBound();
	cBound::Scale(Scale);
}
void cMesh::GetSize(sBox6f &box)
{ 
	AssertValid();
	for(int nTile=0;nTile<GetNumberTile();nTile++)
	{
		sTile *tmp=GetTile(nTile);
		for(sPoint *p=tmp->GetPoint(),*end=&p[tmp->GetNumberPoint()];p<end;p++) 
		{
			if(p->xw()<box.xmin()) box.xmin()=p->xw();
			if(p->yw()<box.ymin()) box.ymin()=p->yw();
			if(p->zw()<box.zmin()) box.zmin()=p->zw();
			if(p->xw()>box.xmax()) box.xmax()=p->xw();
			if(p->yw()>box.ymax()) box.ymax()=p->yw();
			if(p->zw()>box.zmax()) box.zmax()=p->zw();
		}
	}
}
void cMesh::Alignment()
{
	AssertValid();
	sBox6f Box;
	cBound::Real.GetOriginal(Box.min,Box.max);
	GetSize(Box);
	LocalMatrix.trans()=(Box.min+Box.max)*0.5f;
	Tile->TranslateTri(-LocalMatrix.trans());
	Box-=LocalMatrix.trans();
	if(Parent==0) Dummies.Translate(-x(),-y(),-z());
	if(Frame) Frame->Translate(-LocalMatrix.trans());
	cMesh *parent=Parent;
	while(parent) { x()-=parent->x(); y()-=parent->y(); z()-=parent->z(); parent=parent->Parent; }
	if(Frame) { Frame->xOfs=x(); Frame->yOfs=y(); Frame->zOfs=z(); }
	cBound::SetReal(Box.min,Box.max);
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->Alignment();
}
void cMesh::SetFrame()
{
	AssertValid();
	for(int i=0;i<GetNumberTile();i++)
		if(GetTile(i)->GetAttribute(ATTRMAT_TRANSPARENCY|ATTRMAT_OPACITY)) 
			Attribute|=MESH_USE_OPACITY;
	if((Parent)&&(Attribute&MESH_USE_OPACITY)) Parent->Attribute|=MESH_USE_OPACITY;
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->SetFrame();
	if(Frame)
		for(cList *startList=Child; startList; startList=startList->next)
		{
			startList->Mesh->Frame->xCenter-=Frame->xPivot;
			startList->Mesh->Frame->yCenter-=Frame->yPivot;
			startList->Mesh->Frame->zCenter-=Frame->zPivot;
			startList->Mesh->Frame->xPivot-=Frame->xPivot;
			startList->Mesh->Frame->yPivot-=Frame->yPivot;
			startList->Mesh->Frame->zPivot-=Frame->zPivot;
		}
}

void cMesh::GetGlobalGeometry(vector<Vect3f>& verts, vector<int>& faces, int recursive)
{
	AssertValid();
	cMatrix m = GlobalMatrix;
	int i;
	for(i=0;i<3;i++){ 
		m.rot()[i][0] *= GetScale().x; 
		m.rot()[i][1] *= GetScale().y; 
		m.rot()[i][2] *= GetScale().z; 
		}
	
	int nPoint = 0, nPolygon = 0;
	int nTile;
	for(nTile=0;nTile<GetNumberTile();nTile++){
		nPoint += GetTile(nTile)->GetNumberPoint();
		nPolygon += GetTile(nTile)->GetNumberPolygon();
		}
	
	verts.reserve(verts.size() + nPoint);
	faces.reserve(faces.size() + 3*nPolygon);

	for(nTile=0;nTile<GetNumberTile();nTile++){
		int first_point = verts.size();

		sPoint *Point=GetTile(nTile)->GetPoint();
		for(i=0;i < GetTile(nTile)->GetNumberPoint();i++){
			Vect3f v=Point[i].pos;
			m.xformPoint(v);
			verts.push_back(v);
			}

		sPolygon *Polygon=GetTile(nTile)->GetPolygon();
		for(i=0;i < GetTile(nTile)->GetNumberPolygon();i++){
			sPolygon &Poly = Polygon[i];
			faces.push_back( Poly.p1 + first_point);
			faces.push_back( Poly.p2 + first_point);
			faces.push_back( Poly.p3 + first_point);
			}
		}

	if(recursive)
		for(cList *startList=Child; startList; startList=startList->next)
			startList -> Mesh -> GetGlobalGeometry(verts, faces, recursive);
}

void cMesh::GetGlobalDummy(char* name_mask, vector<Vect3f>& dummies, int recursive)
{
	AssertValid();
	DummyList::iterator i;
	FOR_EACH(Dummies, i)
		if(strstr(i -> name, name_mask)){
			Vect3f v = *i;
			GlobalMatrix.xformPoint(v);
			dummies.push_back(v);
			}

	if(recursive)
		for(cList *startList=Child; startList; startList=startList->next)
			startList -> Mesh -> GetGlobalDummy(name_mask, dummies, 1);
}

#ifdef _MECHOSOMA_
void cMesh::SetArcane(void (*FunctionSetArcane)(mchArcaneData*))
{
	AssertValid();
	if(IS_STATIC(Type)) return;
	FunctionSetArcane(&arcaneData);
	for(cList *startList=Child; startList; startList=startList->next)
		startList->Mesh->SetArcane(FunctionSetArcane);
}
#endif
inline sTile* GetTileByName(cTile *Tile,char *name)
{
	for(int  nTile=0;nTile<Tile->GetNumberTile();nTile++)
		if(Tile->GetTile(nTile)->GetName()==name)
			return Tile->GetTile(nTile);
	return 0;
}
void FunctionMorphLinear(void *Key,void *Key1,void *Key2,float t)
{
	assert(t<1.001); assert(t>-0.001);
	cMesh *Mesh=(cMesh*)Key,*a=(cMesh*)Key1,*b=(cMesh*)Key2;
//	Mesh->Real.Interpolate(a->Real,b->Real,t);
	for(int nTile=0;nTile<Mesh->GetNumberTile();nTile++)
	{
		sTile  *tmp=Mesh->GetTile(nTile),*tileA=a->GetTile(nTile),*tileB=b->GetTile(nTile);
		sPoint *Point=tmp->GetPoint(),*PointA=tileA->GetPoint(),*PointB=tileB->GetPoint();
		if((tmp->GetNumberPoint()!=tileB->GetNumberPoint())||(tileA->GetNumberPoint()!=tmp->GetNumberPoint()))
			ErrAbort("Error:FunctionMorphLinear");
		for(int i=0;i<tmp->GetNumberPoint();i++)
			Point[i].pos=PointA[i].pos+t*(PointB[i].pos-PointA[i].pos);
		if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX_TEXEL|ATTRTILE_TEXEL))
		{
			sTexel *Texel=tmp->GetTexel(),*TexelA=tileA->GetTexel(),*TexelB=tileB->GetTexel();
			assert(tmp->GetNumberTexel()==tileA->GetNumberTexel()&&tileA->GetNumberTexel()==tileB->GetNumberTexel());
			for(int i=0;i<tmp->GetNumberTexel();i++)
			{
				float ua=TexelA[i].u(),va=TexelA[i].v(),ub=TexelB[i].u(),vb=TexelB[i].v();
				Texel[i].SetTexel(ua+(ub-ua)*t,va+(vb-va)*t);
			}
		}
	}
}
void DrawLine1(char *buf,int xs,int ys,float x1,float y1,float x2,float y2)
{
	float x,y;
	float dx=x2-x1, dy=y2-y1,dx_,dy_;
	if((dx_=ABS(dx))>(dy_=ABS(dy)))
	{
		float d=dy/dx_;
		if(x1>x2) { x=x1; x1=x2; x2=x; y=y2; d=-d; } else y=y1;
		for(int x=round(x1);x<round(x2);x++)
		{ if((x>=0)&&(x<xs)&&(y>=0)&&(y<ys))buf[x+round(y)*xs]=1; y+=d; }
	}
	else
	{
		float d=dx/dy_;
		if(y1>y2) { x=y1; y1=y2; y2=x; y=x2; d=-d; } else  y=x1;
		for(int x=round(y1);x<round(y2);x++)
		{ if((x>=0)&&(x<ys)&&(y>=0)&&(y<xs))buf[x*xs+round(y)]=1; y+=d; }
	}
}
void cMesh::SetWireSize(const Vect3f &size)
{
	AssertValid();
	if((size.x==size.y)&&(size.y==size.z))
		WireScale(Vect3f(size.x/rmaxTotal(),size.y/rmaxTotal(),size.z/rmaxTotal()));
	else if((size.x!=0)&&(size.y==size.z==0))
		WireScale(Vect3f(size.x/xminTotal(),size.x/xminTotal(),size.x/xminTotal()));
	else if((size.y!=0)&&(size.z==size.x==0))
		WireScale(Vect3f(size.y/yminTotal(),size.y/yminTotal(),size.y/yminTotal()));
	else if((size.z!=0)&&(size.x==size.y==0))
		WireScale(Vect3f(size.z/zminTotal(),size.z/zminTotal(),size.z/zminTotal()));
}
void cMesh::WireScale(const Vect3f &scale)
{
	AssertValid();
	if(Attribute&MESH_WIRE_SCALE) return;
	Attribute|=MESH_WIRE_SCALE;
	Vect3f dScale=scale/Scale;
	cBound::SetWireScale(scale);
	if(Bound) Bound->WireScale(scale);
	if(Parent) { x()*=dScale.x; y()*=dScale.y; z()*=dScale.z; }
	Dummies.Scale(dScale.x,dScale.y,dScale.z);
	if(Frame) 
	{
		Frame->Scale(scale);
		if(Frame->mkf) 
			for(int i=0;i<Frame->mkf->NumberKey;i++)
			{
				char FindCopy=0;
				for(int j=0;j<i;j++)
					if(Frame->mkf->key[i]->GetFileName()==Frame->mkf->key[j]->GetFileName()) 
						FindCopy=1;
				if(!FindCopy) 
					Frame->mkf->key[i]->WireScale(scale);
			}
	}
	Tile->ScaleTri(scale);
	for(cList *tmp=Child;tmp;tmp=tmp->next)
		tmp->Mesh->WireScale(scale);
}
#ifdef _DEBUG
int cMesh::AssertValid()										
{ 
	if(Tile)
		for(int nTile=0;nTile<GetNumberTile();nTile++)
		{
			sTile *tile=GetTile(nTile); 
			if(tile)
			{
				assert(tile->GetNumberPoint()>0);
				assert(tile->GetNumberPolygon()>0);
				assert(tile->GetNumberTexel()>=0);
			}
		}
	for(cList *start=Child; start; start=start->next) 
		start->Mesh->AssertValid(); 
	return 1; 
}
#endif
void cMesh::SetDescription(char *string)						
{ 
	if(description) 
		delete description; 
	if(string&&string[0])
	{
		description=new char[strlen(string)+1];
		strcpy(description,string);
	}
	else
		description=0;
}
