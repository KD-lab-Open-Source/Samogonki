#include <math.h>
#include <float.h>
#include "stddef.h"
#include <fstream>
#include "File3ds.h"
#include "Dispatcher.h"
#include "base.h"
#include "TileMap.h"
#include "BaseObject.h"
#include "BaseDefine.h"
#include "TexMgr.h"

#include "Scene.h"
#include "SceneMesh.h"	// для чтения импортированных файлов
#include "Frame.h"

#ifdef _MECHOSOMA_
#include "mechosoma.h"
#include "SynchroTimer.h"
#include "MemoryHeap.h"
extern MemoryHeap MemoryHeapDynamicShade;
#endif

cAnimChain *AnimChain=0;
cMesh *tmpMesh;
float TmpPivot[3];
int NumberTrackShowMask=0xFFFFFFFF;
extern cTextureBuffer *TextureBuffer;

Vect3f gb_sign(1,-1,1);

extern void allocation_tracking(char*);
#ifdef _MECHOSOMA_
int ResourceFileRead(char *fname,void *&buf,int &size)
{
	buf=0; size=0;
	XStream f(0);
	extern int m3dOpenResource(char* fname,XStream& fh);
	if(m3dOpenResource(fname,f)==0) return -1<<0;
	if((size=f.size())<=0) { f.close(); return -1<<1; }
	buf=new char[size];
	if(buf==0) { f.close(); return -1<<2; }
	f.read((char*)buf,size);
	f.close();
	return 0;
}
#else
int ResourceFileRead(char *fname,void *&buf,int &size)
{
	buf=0; size=0;
	fstream f(fname,ios::in|ios::binary);
	if(f.is_open()==0) { f.close(); return -1<<0; }
	int pos=f.tellg(); 
	f.seekg(0,ios::end); 
	if((size=f.tellg())<=0) { f.close(); return -1<<1; }
	f.seekg(pos);
	buf=new char[size];
	if(buf==0) { f.close(); return -1<<2; }
	f.read((char*)buf,size);
	f.close();
	return 0;
}
#endif

inline char* GetFileName(char *str)
{
	if(str==0) return 0;
	int l=strlen(str);
	if((l==0)||(str[l-1]=='\\')) return 0;
	for(int i=l-1;i>=0;i--)
		if(str[i]=='\\')
			return &str[i+1];
	return str;
}
cMesh* cMeshLibrary::Get(unsigned int Type)
{
	cMesh *Mesh;
	if((Mesh=Find(Type))==0) { 
		XBuffer buf; buf<"Error - MeshLibrary::Get\r\nMesh not found: Kind="<=M3D_KIND(Type)<", Type="<=M3D_TYPE(Type); ErrAbort(buf.address()); return 0; 
	}
	Mesh=CopyObject(Mesh,Type);
	return Mesh;
}
cMesh* cMeshLibrary::Get(char *fname,char *TexturePath,unsigned int Type,float SizeObject)
{	
	cMesh *Mesh;
	if((Mesh=Find(fname))==0) Mesh=Load3ds(fname,TexturePath,Type,SizeObject);
	if(Mesh==0){ 
		XBuffer buf; buf<"Error - MeshLibrary::Get\r\nMesh not found: Kind="<=M3D_KIND(Type)<", Type="<=M3D_TYPE(Type)< " " < fname; 
		ErrAbort(buf.address()); return 0; 
	}
	if(IS_STATIC(Mesh->Type)) { Mesh=CopyObject(Mesh,Mesh->Type); Mesh->Type=Type; }
	else Mesh=CopyObject(Mesh,Type); 
	return Mesh;
}
cMesh* cMeshLibrary::Find(char *fname)
{
	for(cList *start=MeshList->next; start; start=start->next)
		if(start->Mesh->GetFileName()==fname) return start->Mesh;
	return 0;
}
cMesh* cMeshLibrary::Find(unsigned int Type)
{
	for(cList *start=MeshList->next; start; start=start->next)
		if(start->Mesh->Type==Type)
			return start->Mesh;
	return 0;
}
cMesh* cMeshLibrary::CopyObject(cMesh *Mesh,unsigned int Type)
{
	tmpMesh=new cMesh();
	Mesh->CreateCopy(Type,tmpMesh);
	return tmpMesh;
}

inline sObjectMesh *GetObjectByName(sLodObject *LodObject,char *name)
{ // функция ищет в LOD'е объект поимени и времени анимации
	for(int nNodeObject=0;nNodeObject<LodObject->NodeObjectLibrary.length();nNodeObject++)
	{ 
		sNodeObject *NodeObject=LodObject->NodeObjectLibrary[nNodeObject];
		switch(NodeObject->type)
		{
			case NODEOBJECT_MESH:
				sObjectMesh *ObjectMesh=(sObjectMesh*)NodeObject;
				if(ObjectMesh->name==name)
					return ObjectMesh;
				break;
		}
	}
	return 0;
}
/*fstream fxxx("xxx.xxx",ios::out);
void GetMatrix(cMatrix &Matrix,sNodeObject *StartNodeObject,sLodObject *LodObject,int time)
{
	for(sNodeObject *NodeObject=StartNodeObject;NodeObject;NodeObject=LodObject->NodeObjectLibrary.Get((char*)NodeObject->parent))
	{
		cMatrix AnimationMatrix; // матрица положения объекта относительного родителя
		AnimationMatrix.NewMatrix(); // установка матрицы анимации
		sAnimationPosition &AnimationPosition=NodeObject->AnimationPosition;
		sAnimationRotation &AnimationRotation=NodeObject->AnimationRotation;
		sAnimationScale &AnimationScale=NodeObject->AnimationScale;
		if((AnimationPosition.length()==0)||(AnimationRotation.length()==0)||(AnimationScale.length()==0))
			AnimationMatrix.set(NodeObject->matrix);
		else 
		{
			if(AnimationPosition.length()==1)
				AnimationMatrix.Translate(AnimationPosition[0][1],AnimationPosition[0][2],AnimationPosition[0][3]);
			else
				for(int i=0;i<AnimationPosition.length();i++)
					if(AnimationPosition[i][0]==time)
					{
						AnimationMatrix.Translate(AnimationPosition[i][1],AnimationPosition[i][2],AnimationPosition[i][3]);
						break;
					}
			if(AnimationRotation.length()==1) 
				AnimationMatrix.Rotate(AnimationRotation[0][4],AnimationRotation[0][1],AnimationRotation[0][2],AnimationRotation[0][3]);
			else 
				for(int i=0;i<AnimationRotation.length();i++)
					if(AnimationRotation[i][0]==time)
					{
						AnimationMatrix.Rotate(AnimationRotation[i][4],AnimationRotation[i][1],AnimationRotation[i][2],AnimationRotation[i][3]);
						break;
					}
			if(AnimationScale.length()==1) 
				AnimationMatrix.Scale(AnimationScale[0][1],AnimationScale[0][2],AnimationScale[0][3]);
			else
				for(int i=0;i<AnimationScale.length();i++)
					if(AnimationScale[i][0]==time)
					{
						AnimationMatrix.Scale(AnimationScale[i][1],AnimationScale[i][1],AnimationScale[i][2]);
						break;
					}
		}
		Matrix=AnimationMatrix*Matrix;
		if(NodeObject==StartNodeObject)
		{
			fxxx<<StartNodeObject->name<<"\n";
			fxxx<<"Time="<<time<<"\n";
			fxxx<<"LocalMatrix\n";
			fxxx<<Matrix(0,0)<<"	"<<Matrix(0,1)<<"	"<<Matrix(0,2)<<"\n";
			fxxx<<Matrix(1,0)<<"	"<<Matrix(1,1)<<"	"<<Matrix(1,2)<<"\n";
			fxxx<<Matrix(2,0)<<"	"<<Matrix(2,1)<<"	"<<Matrix(2,2)<<"\n";
			fxxx<<Matrix(0,3)<<"	"<<Matrix(1,3)<<"	"<<Matrix(2,3)<<"\n";
		}
		if(NodeObject->parent==0) break;
	}
}
*/
int gb_TileID=1;
sTile* ReadAnimationMesh(char *fname,sLodObject *LodObject,sObjectMesh *ObjectMesh,sAnimationMesh *AnimationMesh,int time,char *TexturePath,int IsStatic)
{
	sTile *Tile=new sTile;
	Tile->GetName()=ObjectMesh->name;
	sVertexMesh	&Vertex=AnimationMesh->Vertex;
	sFaceMesh &Face=AnimationMesh->Face;
	assert(Face.length()); // тест на ликвидность модели
	assert(Vertex.length());
	assert(Face[0][3]<LodObject->MaterialLibrary.length());
	int nMapping=AnimationMesh->ChannelMappingLibrary.length();
	sChannelMapping *ChannelMapping=0;
	if(nMapping) ChannelMapping=AnimationMesh->ChannelMappingLibrary[0];
	if((nMapping==0)||(ChannelMapping->ChannelNumber==0)) // объект не содержит текстурных координат (нет текстуры)
		if(IsStatic) Tile->NewTri(Face.length(),Vertex.length(),0,0);
		else 
			((sObjTri*)Tile)->NewTri(Vertex.length(),0);
//			Tile->NewTri(Face.length(),Vertex.length(),0,0);
	else
	{ // импорт текстурных координат
		sTexFaceMesh &TexFace=ChannelMapping->TexFace;
		sTexVertexMesh &TexVertex=ChannelMapping->TexVertex;
		assert(TexFace.length()==Face.length());
//		assert(TexVertex.length()>=Vertex.length());
		if(IsStatic) Tile->NewTri(Face.length(),Vertex.length(),TexVertex.length(),TexFace.length());
		else 
			((sObjTri*)Tile)->NewTri(Vertex.length(),TexVertex.length());
//		Tile->NewTri(Face.length(),Vertex.length(),TexVertex.length(),TexFace.length());
		sTexel *pTexel=Tile->GetTexel();
		int k;
		for(k=0;k<Tile->GetNumberTexel();k++)
			pTexel[k].SetTexel(TexVertex[k][0]*((1<<TEXTURE_COORD)-1)/(1<<TEXTURE_COORD),(((1<<13)-1)-TexVertex[k][1]*((1<<TEXTURE_COORD)-1))/(1<<TEXTURE_COORD));
		if(IsStatic)
		{
			sPolygon *pTexPoly=Tile->GetTexPoly();
			for(k=0;k<Tile->GetNumberTexPoly();k++)
				pTexPoly[k].set(TexFace[k][0],TexFace[k][1],TexFace[k][2]);
		}
	}
	sPoint *pPoint=Tile->GetPoint();
	int k;
	for(k=0;k<Tile->GetNumberPoint();k++)
		pPoint[k].pos.set(Vertex[k][0],Vertex[k][1],Vertex[k][2]);
	if(IsStatic)
	{
		sPolygon *pPolygon=Tile->GetPolygon();
		for(k=0;k<Tile->GetNumberPolygon();k++)
			pPolygon[k].set(Face[k][0],Face[k][1],Face[k][2]);
	}
	// импорт свойств материала объекта
	if((LodObject->MaterialLibrary.length()==0)||(Face[0][3]<0))
	{ XBuffer buf; buf<"Error: ReadAnimationMesh()\r\nNot found material by object <"<ObjectMesh->name<"> in file <"<fname<">\r\n"; ErrH.Abort(buf.address()); }
	sMaterialObject *Material=LodObject->MaterialLibrary[Face[0][3]];
	sAnimationMaterial *AnimationMaterial=Material->AnimationMaterialLibrary[0];
	int dt=0x0FFFFFFF;
	for(int nAnimationMaterial=0;nAnimationMaterial<Material->AnimationMaterialLibrary.length();nAnimationMaterial++)
		if(abs(Material->AnimationMaterialLibrary[nAnimationMaterial]->time-time)<dt)
		{ // поиск ближайшего по времени материала 
			AnimationMaterial=Material->AnimationMaterialLibrary[nAnimationMaterial];
			dt=abs(Material->AnimationMaterialLibrary[nAnimationMaterial]->time-time);
		}
	float ShinStrength=AnimationMaterial->ShinStrength;
	if(Material->Shading==2) 
		Tile->GetMaterial()->SetAttribute(ATTRMAT_METAL);
	Tile->GetDiffuse().set(AnimationMaterial->DiffuseColor[0],AnimationMaterial->DiffuseColor[1],AnimationMaterial->DiffuseColor[2],1.f-AnimationMaterial->Transparency);
	Tile->GetSpecular().set(AnimationMaterial->SpecularColor[0],AnimationMaterial->SpecularColor[1],AnimationMaterial->SpecularColor[2],1.f);
	sTexture *Texture=0,*Opacity=0;
	if(IsStatic)
		for(int nSubTex=0;nSubTex<AnimationMaterial->SubTexmap.length();nSubTex++)
			switch(AnimationMaterial->SubTexmap[nSubTex]->ID)
			{
				case TEXMAP_DI:
					if(Texture==0) Texture=TextureBuffer->Get(GetFileName(AnimationMaterial->SubTexmap[nSubTex]->name),TexturePath,fname);
					break;
				case TEXMAP_OP:
					if(Opacity==0) Opacity=TextureBuffer->Get(GetFileName(AnimationMaterial->SubTexmap[nSubTex]->name),TexturePath,fname);
					break;
				default:
					assert(0);
			}
	switch(round(ShinStrength*100))
	{
		case 100: Tile->GetMaterial()->SetAttribute(ATTRMAT_MASK); break;
		default:
			if(Tile->GetDiffuse().GetA()<255) Tile->GetMaterial()->SetAttribute(ATTRMAT_TRANSPARENCY);
			break;
	}
	Tile->GetMaterial()->SetTexture(Texture,Opacity);
	// импорт анимации корневого объекта ..
	
	// .. пока не реализовано
	// !!! и еще нужно добавить кватернион SCALEAXIS как в ASCII !!!
	cMatrix Matrix; Matrix.NewMatrix();
	for(sNodeObject *NodeObject=ObjectMesh;NodeObject;NodeObject=LodObject->NodeObjectLibrary.Get((char*)NodeObject->parent))
	{
		cMatrix AnimationMatrix; // матрица положения объекта относительного родителя
		AnimationMatrix.NewMatrix(); // установка матрицы анимации
		sAnimationPosition &AnimationPosition=NodeObject->AnimationPosition;
		sAnimationRotation &AnimationRotation=NodeObject->AnimationRotation;
		sAnimationScale &AnimationScale=NodeObject->AnimationScale;
		if((AnimationPosition.length()==0)||(AnimationRotation.length()==0)||(AnimationScale.length()==0))
			AnimationMatrix.set(NodeObject->matrix);
		else 
		{
			if(AnimationPosition.length()==1)
				AnimationMatrix.Translate(AnimationPosition[0][1],AnimationPosition[0][2],AnimationPosition[0][3]);
			else
				for(int i=0;i<AnimationPosition.length();i++)
					if(AnimationPosition[i][0]==time)
					{
						AnimationMatrix.Translate(AnimationPosition[i][1],AnimationPosition[i][2],AnimationPosition[i][3]);
						break;
					}
			if(AnimationRotation.length()==1) 
				AnimationMatrix.Rotate(-AnimationRotation[0][4],AnimationRotation[0][1],AnimationRotation[0][2],AnimationRotation[0][3]);
			else 
				for(int i=0;i<AnimationRotation.length();i++)
					if(AnimationRotation[i][0]==time)
					{
						AnimationMatrix.Rotate(-AnimationRotation[i][4],AnimationRotation[i][1],AnimationRotation[i][2],AnimationRotation[i][3]);
						break;
					}
			if(AnimationScale.length()==1) 
				AnimationMatrix.Scale(AnimationScale[0][1],AnimationScale[0][2],AnimationScale[0][3]);
			else
				for(int i=0;i<AnimationScale.length();i++)
					if(AnimationScale[i][0]==time)
					{
						AnimationMatrix.Scale(AnimationScale[i][1],AnimationScale[i][1],AnimationScale[i][2]);
						break;
					}
		}
		Matrix=AnimationMatrix*Matrix;
		if(NodeObject->parent==0) break;
	}
	Matrix(0,0)*=gb_sign.x; Matrix(0,1)*=gb_sign.x; Matrix(0,2)*=gb_sign.x; Matrix(0,3)*=gb_sign.x;
	Matrix(1,0)*=gb_sign.y; Matrix(1,1)*=gb_sign.y; Matrix(1,2)*=gb_sign.y; Matrix(1,3)*=gb_sign.y;
	Matrix(2,0)*=gb_sign.z; Matrix(2,1)*=gb_sign.z; Matrix(2,2)*=gb_sign.z; Matrix(2,3)*=gb_sign.z;
	for(int i=0;i<Tile->GetNumberPoint();i++)
		pPoint[i].pos=Matrix.xformPoint(pPoint[i].pos);
	return Tile;
}
inline int TestFirstName(char *name,char *string)
{
	if(name==0||string==0) return 0;
	int LengthName=strlen(name),LengthString=strlen(string);
	if(LengthName<LengthString) return 0;
	for(int i=0;i<LengthString;i++)
		if(name[i]!=string[i]) return 0;
	return 1;
}
inline sAnimationMesh* GetAnimationMesh(sObjectMesh *ObjectMesh,int time)
{
	float dt=1e30f;
	sAnimationMesh *tmp=0;
	for(int nFFD=0;nFFD<ObjectMesh->AnimationMeshLibrary.length();nFFD++)
	{
		sAnimationMesh *AnimationMesh=ObjectMesh->AnimationMeshLibrary[nFFD];
		if(dt>fabs((float)(AnimationMesh->time-time)))
		{ tmp=AnimationMesh; dt=fabs((float)(AnimationMesh->time-time)); }
	}
	return tmp;
}
inline int IsAnimate(sLodObject *LodObject,sObjectMesh *ObjectMesh,sAnimationMesh *AnimationMesh,int time)
{
	for(sNodeObject *NodeObject=ObjectMesh;NodeObject;NodeObject=LodObject->NodeObjectLibrary.Get((char*)NodeObject->parent))
	{
		if((time==AnimationMesh->time)||(NodeObject->AnimationPosition.length()>1)||(NodeObject->AnimationRotation.length()>1)||(NodeObject->AnimationScale.length()>1)) 
			return 1;
		if(NodeObject->parent==0) break;
	}
	if((LodObject->MaterialLibrary.length()==0)||(AnimationMesh->Face[0][3]<0)) 
		return -1;
	sMaterialObject *Material=LodObject->MaterialLibrary[AnimationMesh->Face[0][3]];
	if(Material->AnimationMaterialLibrary.length()<=1) return 0;
	for(int i=0;i<Material->AnimationMaterialLibrary.length();i++)
	{
		sAnimationMaterial &AnimMaterial=*Material->AnimationMaterialLibrary[i];
		sAnimationMaterial &AnimMaterialStart=*Material->AnimationMaterialLibrary[0];
		if(fabs(AnimMaterial.AmbientColor[0]-AnimMaterialStart.AmbientColor[0])>0.001f||fabs(AnimMaterial.AmbientColor[1]-AnimMaterialStart.AmbientColor[1])>0.001f||fabs(AnimMaterial.AmbientColor[2]-AnimMaterialStart.AmbientColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.DiffuseColor[0]-AnimMaterialStart.DiffuseColor[0])>0.001f||fabs(AnimMaterial.DiffuseColor[1]-AnimMaterialStart.DiffuseColor[1])>0.001f||fabs(AnimMaterial.DiffuseColor[2]-AnimMaterialStart.DiffuseColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.SpecularColor[0]-AnimMaterialStart.SpecularColor[0])>0.001f||fabs(AnimMaterial.SpecularColor[1]-AnimMaterialStart.SpecularColor[1])>0.001f||fabs(AnimMaterial.SpecularColor[2]-AnimMaterialStart.SpecularColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.Shininess-AnimMaterialStart.Shininess)>0.001f)
			return 1;
		if(fabs(AnimMaterial.ShinStrength-AnimMaterialStart.ShinStrength)>0.001f)
			return 1;
		if(fabs(AnimMaterial.Transparency-AnimMaterialStart.Transparency)>0.001f)
			return 1;
	}
	return 0;
}
/*
inline int IsAnimate(cMaterialObjectLibrary &MaterialLibrary,sObjectMesh *ObjectMesh,sAnimationMesh *AnimationMesh,int time)
{
	if((time==AnimationMesh->time)||(ObjectMesh->AnimationPosition.length()>1)||(ObjectMesh->AnimationRotation.length()>1)||(ObjectMesh->AnimationScale.length()>1)) 
		return 1;
	if((MaterialLibrary.length()==0)||(AnimationMesh->Face[0][3]<0)) 
		return -1;
	sMaterialObject *Material=MaterialLibrary[AnimationMesh->Face[0][3]];
	if(Material->AnimationMaterialLibrary.length()>1) 
		return 1;
	return 0;
}
/*
inline int IsAnimate(cMaterialObjectLibrary &MaterialLibrary,sObjectMesh *ObjectMesh,sAnimationMesh *AnimationMesh,int time)
{
	if((time==AnimationMesh->time)||(ObjectMesh->AnimationPosition.length()>1)||(ObjectMesh->AnimationRotation.length()>1)||(ObjectMesh->AnimationScale.length()>1)) 
		return 1;
	if((MaterialLibrary.length()==0)||(AnimationMesh->Face[0][3]<0)) 
		return -1;
	sMaterialObject *Material=MaterialLibrary[AnimationMesh->Face[0][3]];
	if(Material->AnimationMaterialLibrary.length()<=1) return 0;
	for(int i=0;i<Material->AnimationMaterialLibrary.length();i++)
	{
		sAnimationMaterial &AnimMaterial=*Material->AnimationMaterialLibrary[i];
		sAnimationMaterial &AnimMaterialStart=*Material->AnimationMaterialLibrary[0];
		if(fabs(AnimMaterial.AmbientColor[0]-AnimMaterialStart.AmbientColor[0])>0.001f||fabs(AnimMaterial.AmbientColor[1]-AnimMaterialStart.AmbientColor[1])>0.001f||fabs(AnimMaterial.AmbientColor[2]-AnimMaterialStart.AmbientColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.DiffuseColor[0]-AnimMaterialStart.DiffuseColor[0])>0.001f||fabs(AnimMaterial.DiffuseColor[1]-AnimMaterialStart.DiffuseColor[1])>0.001f||fabs(AnimMaterial.DiffuseColor[2]-AnimMaterialStart.DiffuseColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.SpecularColor[0]-AnimMaterialStart.SpecularColor[0])>0.001f||fabs(AnimMaterial.SpecularColor[1]-AnimMaterialStart.SpecularColor[1])>0.001f||fabs(AnimMaterial.SpecularColor[2]-AnimMaterialStart.SpecularColor[2])>0.001f)
			return 1;
		if(fabs(AnimMaterial.Shininess-AnimMaterialStart.Shininess)>0.001f)
			return 1;
		if(fabs(AnimMaterial.ShinStrength-AnimMaterialStart.ShinStrength)>0.001f)
			return 1;
		if(fabs(AnimMaterial.Transparency-AnimMaterialStart.Transparency)>0.001f)
			return 1;
	}
	return 0;
}
*/
cMesh* cMeshLibrary::Loadm3d(char *fname,char *TexturePath,unsigned int Type,float SizeObject)
{
	cMeshScene MeshScene;
	// загрузка MeshScene сцены из файла
	cMeshFile f;
	int	size=0;
	void *buf=0;
	if(ResourceFileRead(fname,buf,size)) return 0;
	if(f.OpenRead(buf,size)==MESHFILE_NOT_FOUND) return 0;
	f.ReadHeaderFile();
	MeshScene.Read(f);
	f.Close();
	// создание объекта по описаннию в сцене
	cMesh *tmpMesh=0,*BaseMesh=0,*Bound=0;
	int UseAnimation=0;
	cFrame *Frame=new cFrame;
	// поиск и установка первым канала анимации с именем "main"
	int nChannelMain=-1;
	int nChannel;
	for(nChannel=0;nChannel<MeshScene.ChannelLibrary.length();nChannel++)
		if(stricmp(MeshScene.ChannelLibrary[nChannel]->name,"main")==0)
			nChannelMain=nChannel;
	if(nChannelMain>=0) 
	{
		sChannelAnimation *tmp=MeshScene.ChannelLibrary[0];
		MeshScene.ChannelLibrary[0]=MeshScene.ChannelLibrary[nChannelMain];
		MeshScene.ChannelLibrary[nChannelMain]=tmp;
	}
	for(nChannel=0;nChannel<MeshScene.ChannelLibrary.length();nChannel++)
	{ // импорт канала анимации
		sChannelAnimation *Channel=MeshScene.ChannelLibrary[nChannel];
		AnimChain=Frame->AddAnimChain();
		float FirstFrame=Channel->FirstFrame*Channel->TicksPerFrame;
		AnimChain->GetName()=_strlwr(Channel->name);
		AnimChain->SetTimeChain((Channel->LastFrame-Channel->FirstFrame)*Channel->TicksPerFrame);
		for(int LevelDetail=0;LevelDetail<Channel->LodLibrary.length();LevelDetail++)
		{ // импорт уровня детализации
			sLodObject *LodObject=Channel->LodLibrary[LevelDetail];
			for(int nNodeObject=0;nNodeObject<LodObject->NodeObjectLibrary.length();nNodeObject++)
			{ // импорт корневого объекта
				sNodeObject *NodeObject=LodObject->NodeObjectLibrary[nNodeObject];
				char NameMesh[256],NameParent[256];
				strcpy(NameMesh,NodeObject->name); 
				if(NodeObject->parent) strcpy(NameParent,NodeObject->parent); else NameParent[0]=0;
				switch(NodeObject->type)
				{
					case NODEOBJECT_MESH:
						{ // импорт 3d-объекта
							sObjectMesh *ObjectMesh=(sObjectMesh*)NodeObject;
							if((ObjectMesh->name)&&(TestFirstName(ObjectMesh->name,"Bip")))
								continue; // пропустить "Bip" из CharacterStudio
							if(nChannel==0)
							{ // импорт статического объекта
								sColor4f Diffuse,Specular;
								void *Pointer=0;
								float Shininess=0;
								int nVertex=0,nPolygon=0,nTexel=0,nTexPoly=0,AttributeMaterial=ATTRMAT_ATTRIBUTE_NULL;
								char TextureName[256],OpacityName[256]; TextureName[0]=0; OpacityName[0]=0;
								sTile *Tile=ReadAnimationMesh(fname,LodObject,ObjectMesh,ObjectMesh->AnimationMeshLibrary[0],FirstFrame,TexturePath,1);
								Tile->GetID()=gb_TileID++;
								if(strcmp(NameMesh,M3D_BOUND_STRING)==0)
								{ // объект является границей
									Bound=new cMesh();
									Bound->SetName(NameMesh);
									Bound->AddTile(Tile);
								}
								else if((BaseMesh==0)||((tmpMesh=BaseMesh->FindMesh(NameParent))==0))
								{ // объект главный родитель
									if(BaseMesh==0)
									{
										BaseMesh=tmpMesh=AddMesh(NameMesh);
										BaseMesh->Type=Type;
										BaseMesh->SetFileName(fname);
										BaseMesh->SetTexturePath(TexturePath);
									}
									BaseMesh->AddTile(Tile);
								}
								else // объект - неподвижная часть
									tmpMesh->AddTile(Tile);
							}
//							if((ObjectMesh->AnimationMeshLibrary.length()>1)||(MeshScene.ChannelLibrary.length()>1))
							{ // импорт морфинга объекта
								UseAnimation=1;
								for(int nFrame=0;nFrame<Channel->NumberFrame;nFrame++)
								{
									assert(Channel->NumberFrame>0);
									int TimeFrame;
									if(Channel->NumberFrame>1) TimeFrame=FirstFrame+nFrame*((Channel->LastFrame-Channel->FirstFrame)*Channel->TicksPerFrame/(Channel->NumberFrame-1)); else TimeFrame=FirstFrame;
									sAnimationMesh *AnimationMesh=GetAnimationMesh(ObjectMesh,TimeFrame);
									if(IsAnimate(LodObject,ObjectMesh,AnimationMesh,TimeFrame)||(nFrame==0))
									{
										sColor4f Diffuse,Specular;
										void *Pointer=0;
										float Shininess=0;
										int nVertex=0,nPolygon=0,nTexel=0,nTexPoly=0,AttributeMaterial=ATTRMAT_ATTRIBUTE_NULL;
										char TextureName[256],OpacityName[256]; TextureName[0]=0; OpacityName[0]=0;
										sTile *Tile=ReadAnimationMesh(fname,LodObject,ObjectMesh,AnimationMesh,TimeFrame,TexturePath,0);
										sTile *OriginalTile=BaseMesh->GetTileByName(NameMesh);
										if((nChannel)&&(OriginalTile==0))
											{ XBuffer buf; buf<"Error: cMeshLibrary::Loadm3d()\r\nNot found object <"<NameMesh<"> in channel <"<AnimChain->GetName()<"> file "<fname; ErrAbort(buf.address()); }
										else
											if(Tile->GetName()!=M3D_BOUND_STRING)
												Tile->GetID()=OriginalTile->GetID();

										int AddTile=0;
										float AnimationMeshTime=TimeFrame-FirstFrame;
										if(AnimationMeshTime<0) AnimationMeshTime=0;
										for(int i=0;i<AnimChain->GetNumberAnimKey();i++)
											if(AnimChain->GetAnimKey(i)->Time==AnimationMeshTime)
											{
												((cTile*)AnimChain->GetAnimKey(i)->Key)->AddTile(Tile);
												AddTile=1;
											}
										if(AddTile==0)
										{
											sKeyTri *AnimKey=AnimChain->AddAnimKey();
											cTile *tile=new cTile;
											tile->AddTile(Tile);
											AnimKey->set(tile,AnimationMeshTime);
										}
									}
								}
							}
						}
						break;
					case NODEOBJECT_HELPER:
						break;
					default:
						assert(NodeObject->type==NODEOBJECT_MESH||NodeObject->type==NODEOBJECT_HELPER);
				}
			}
		}
	}
	for(nChannel=0;nChannel<MeshScene.ChannelLibrary.length()&&nChannel<1;nChannel++)
	{ // импорт дамми
		sChannelAnimation *Channel=MeshScene.ChannelLibrary[nChannel];
		for(int LevelDetail=0;LevelDetail<Channel->LodLibrary.length();LevelDetail++)
		{ // импорт уровня детализации
			sLodObject *LodObject=Channel->LodLibrary[LevelDetail];
			for(int nNodeObject=0;nNodeObject<LodObject->NodeObjectLibrary.length();nNodeObject++)
			{ // импорт корневого объекта
				sNodeObject *NodeObject=LodObject->NodeObjectLibrary[nNodeObject];
				switch(NodeObject->type)
				{
					case NODEOBJECT_MESH:
						break;
					case NODEOBJECT_HELPER:
						if(nChannel==0) // импорт только статических вспомогательных объектов
						{ // импорт вспомогательного-объекта
							sHelperObject *HelperObject=(sHelperObject*)NodeObject;
							if(stricmp(HelperObject->HelperName,"Dummy")==0)
							{ // импорт вспомогательного объекта - дамми
								Vect3f v(gb_sign.x*HelperObject->matrix[9],
									gb_sign.y*HelperObject->matrix[10],
									gb_sign.z*HelperObject->matrix[11]);
								if(BaseMesh) 
									BaseMesh->Dummies.Add(v,HelperObject->name);
							}
//							else 
//								assert(0);
						}
						break;
					default:
						assert(NodeObject->type==NODEOBJECT_MESH||NodeObject->type==NODEOBJECT_HELPER);
				}
			}
		}
	}
	if(UseAnimation==0) delete Frame; else if(BaseMesh) 
	{
		BaseMesh->Frame=Frame;
		Frame->SetCurrentChain("main");
	}
	if(BaseMesh) { BaseMesh->Alignment(); BaseMesh->ReCalcTotalBound(); BaseMesh->SetFrame(); } 
	else { XBuffer buf; buf<"Error: cMeshLibrary::Loadm3d/r/nObject not found "<fname; ErrAbort(buf.address());  return 0; }
	if(Bound) 
	{ 
		BaseMesh->Bound=Bound; 
		Bound->Alignment(); 
		Bound->Displace(-BaseMesh->x()+Bound->x(),-BaseMesh->y()+Bound->y(),-BaseMesh->z()+Bound->z()); 
		Bound->CalcTotalBound(); 
		Bound->SetParent(BaseMesh); 
	}
	return BaseMesh;
}
cMesh* cMeshLibrary::Load3ds(char *fname,char *TexturePath,unsigned int Type,float SizeObject)
{
	_strlwr(fname);
	int l=strlen(fname);
	while((l>=0)&&(fname[l]!='.')) l--;
	if(stricmp(&fname[l],".m3d")==0) return Loadm3d(fname,TexturePath,Type,SizeObject);
	cFile3ds f;
	if(f.Open(fname)==0) return 0;
	char TextureName[17],NameMesh[22],Shading,OpacityName[17];
	int NumberObject3ds=f.OpenBaseMesh(),nPoint,nPolygon;
	if(SizeObject!=0) f.MaxSizeMesh(NumberObject3ds,SizeObject);	// єёЄрэютър ьрё°Єрср чруЁєцрхьюую юс·хъЄр
	int NumberKeyFrame=f.OpenBaseKeyFrame();
	cMesh *tmpMesh=0,*BaseMesh=0,*Bound=0;
	int i;
	for(i=0;i<NumberObject3ds;i++)
	{
		void *Pointer=0;
		f.OpenMesh(i,&nPoint,&nPolygon,NameMesh);
		unsigned int AttributeMaterial=ATTRMAT_ATTRIBUTE_NULL;
		float *Vertex=new float[5*nPoint],Matrix[12],ShinStrength,Shininess;
		int *Face=new int[3*nPolygon];
		sTile *Tile=new sTile;
		Tile->GetName()=NameMesh;
		Tile->GetID()=gb_TileID++;
		f.ReadMesh(Vertex,Face,(char*)TextureName,Matrix);
		f.ReadMaterial(TextureName,TextureName,OpacityName,&Tile->GetDiffuse().a,&ShinStrength,&Shininess,&Shading,&Tile->GetDiffuse().r,&Tile->GetDiffuse().g,&Tile->GetDiffuse().b);
		if(Tile->GetDiffuse().a>0) Tile->GetDiffuse().a=1-Tile->GetDiffuse().a; else Tile->GetDiffuse().a=1+Tile->GetDiffuse().a;
		if(Tile->GetDiffuse().a<0) Tile->GetDiffuse().a=0; else if(Tile->GetDiffuse().a>1) Tile->GetDiffuse().a=1;

		if(Shading==4) Tile->GetMaterial()->SetAttribute(ATTRMAT_METAL);
		switch(round(ShinStrength*100))
		{
			case 100:	Tile->GetMaterial()->SetAttribute(ATTRMAT_MASK); break;
			default:
				if(fabs((float)(Tile->GetDiffuse().GetA()))<255) Tile->GetMaterial()->SetAttribute(ATTRMAT_TRANSPARENCY);
				break;
		}
		Tile->SetTexture(TextureBuffer->Get(TextureName,TexturePath,fname),TextureBuffer->Get(OpacityName,TexturePath,fname));
		
		if(TextureName[0])
		{
			Tile->NewTri(nPolygon,nPoint,nPoint,0);
			sTexel *pTexel=Tile->GetTexel();
			for(int k=0;k<nPoint;k++)
				pTexel[k].SetTexel(Vertex[5*k+3]*((1<<TEXTURE_COORD)-1)/(1<<TEXTURE_COORD),(((1<<13)-1)-Vertex[5*k+4]*((1<<TEXTURE_COORD)-1))/(1<<TEXTURE_COORD));
		}
		else
			Tile->NewTri(nPolygon,nPoint,0,0);
		sPoint *pPoint=Tile->GetPoint();
		int k;
		for(k=0;k<nPoint;k++)
		{
			pPoint[k].SetVertex(gb_sign.x*Vertex[5*k+0],gb_sign.y*Vertex[5*k+1],gb_sign.z*Vertex[5*k+2]);
			if(!_finite(pPoint[k].xw())) 
				XCon<"Warning: error coord in 3ds file = "<fname<"\r\n";
		}
		sPolygon *pPolygon=Tile->GetPolygon();
		for(k=0;k<nPolygon;k++)
			pPolygon[k].set(Face[3*k+0],Face[3*k+1],Face[3*k+2]);

		char NameParent[30];
		float *Pos=0,*Rot=0,*Scale,Pivot[3];
		int NumberPos=0,NumberRot=0,NumberScale=0;
		f.OpenKeyFrame(NameMesh);
		f.ReadKeyFrame(NameParent,&Pos,&NumberPos,&Rot,&NumberRot,&Scale,&NumberScale,Pivot);
		f.CloseKeyFrame();

		for(k=0;k<NumberPos;k++) { Pos[4*k+1]*=gb_sign.x; Pos[4*k+2]*=gb_sign.y; Pos[4*k+3]*=gb_sign.z; }
		for(k=0;k<NumberRot;k++) { Rot[5*k+2]*=gb_sign.x; Rot[5*k+3]*=gb_sign.y; Rot[5*k+4]*=gb_sign.z; }
		Pivot[0]*=gb_sign.x; Pivot[1]*=gb_sign.y; Pivot[2]*=gb_sign.z;
		Matrix[9]*=gb_sign.x; Matrix[10]*=gb_sign.y; Matrix[11]*=gb_sign.z;
		Vect3f vPivot(Pivot[0]*Matrix[0]+Pivot[1]*Matrix[3]+Pivot[2]*Matrix[6],
			Pivot[0]*Matrix[1]+Pivot[1]*Matrix[4]+Pivot[2]*Matrix[7],
			Pivot[0]*Matrix[2]+Pivot[1]*Matrix[5]+Pivot[2]*Matrix[8]);
		for(k=0;k<nPoint;k++)
			pPoint[k].pos-=vPivot;

		if(strcmp(NameMesh,M3D_BOUND_STRING)==0) 
		{	// шэшЎшрышчрЎш  ЁрчьхЁют
			Bound=new cMesh();
			Bound->SetName(NameMesh);
			Bound->AddTile(Tile);
		}
		else if((BaseMesh==0)||((tmpMesh=BaseMesh->FindMesh(NameParent))==0))
		{	// шэшЎшрышчрЎш  срчютюую ЁюфшЄхы 
			if(BaseMesh==0)
			{
				BaseMesh=tmpMesh=AddMesh(NameMesh);
				BaseMesh->Type=Type;
				BaseMesh->SetFileName(fname);
				BaseMesh->SetTexturePath(TexturePath);
			}
			BaseMesh->AddTile(Tile);
		}
		else if((NumberPos<=1)&&(NumberRot<=1)&&(NumberScale<=1)&&!strstr(NameMesh, "#"))
		// фюсртыхэшх ярёёштэюую яюЄюьър
			tmpMesh->AddTile(Tile);
		else
		{	// ёючфрэшх ш яЁшёюхфшэхэшх ръЄштэюую яюЄюьър
			cMesh *ParentMesh=BaseMesh->FindMesh(NameParent);
			tmpMesh=new cMesh();
			tmpMesh->SetName(NameMesh);
			if(strstr(NameMesh, "#"))
			{
				tmpMesh->SetFileName(fname);
				tmpMesh->SetTexturePath(TexturePath);
			}
			tmpMesh->Type=Type;
			tmpMesh->SetParent(ParentMesh);
			cMesh *p=ParentMesh;
//			if(ParentMesh->Frame==0) tmpMesh->SetFrame(NameMesh,NameParent,xPivot,yPivot,zPivot,Matrix[9],Matrix[10],Matrix[11],Pos,NumberPos,Rot,NumberRot);
//			else tmpMesh->SetFrame(NameMesh,NameParent,xPivot,yPivot,zPivot,Matrix[9],Matrix[10],Matrix[11],Pos,NumberPos,Rot,NumberRot);
			if(ParentMesh->Frame==0) tmpMesh->SetFrame(NameMesh,NameParent,0,0,0,Matrix[9],Matrix[10],Matrix[11],Pos,NumberPos,Rot,NumberRot,Scale,NumberScale);
			else tmpMesh->SetFrame(NameMesh,NameParent,0,0,0,Matrix[9],Matrix[10],Matrix[11],Pos,NumberPos,Rot,NumberRot,Scale,NumberScale);
			tmpMesh->AddTile(Tile);
			ParentMesh->AddChild(tmpMesh);
		}
		
		if(Pos) delete Pos; 
		if(Rot) delete Rot;
		if(Scale) delete Scale; 
		delete Vertex; delete Face;
		f.CloseMesh();
	}
	for(i=NumberObject3ds;i<NumberKeyFrame;i++)
	{	// set dummy
		Vect3f v;
		char *DummyName=f.OpenDummy(i);
		f.ReadDummy(&v.x,&v.y,&v.z);
		v.x*=gb_sign.x; v.y*=gb_sign.y; v.z*=gb_sign.z;
		char *NameDummyParent=f.GetDummyParent();
		cMesh *Mesh=0;
		if(NameDummyParent[0]==0) Mesh=BaseMesh;
		else Mesh=BaseMesh->FindMesh(NameDummyParent);
		if(Mesh) Mesh->Dummies.Add(v,DummyName);
		f.CloseDummy();
	}
	if(BaseMesh) { BaseMesh->Alignment(); BaseMesh->ReCalcTotalBound(); BaseMesh->SetFrame(); } 
	else { XBuffer buf; buf<"Error: cMeshLibrary::Load3ds/r/nNot create "<fname; ErrAbort(buf.address());  return 0; }
	if(Bound) 
	{ 
		BaseMesh->Bound=Bound; 
		Bound->Alignment(); 
		Bound->Displace(-BaseMesh->x()+Bound->x(),-BaseMesh->y()+Bound->y(),-BaseMesh->z()+Bound->z()); 
		Bound->CalcTotalBound(); 
		Bound->SetParent(BaseMesh); 
	}
	f.CloseBaseKeyFrame();
	f.CloseBaseMesh();
	f.Close();
	return BaseMesh;
}
int FindTile(cMesh *Mesh,char *name)
{
	for(int i=0;i<Mesh->GetNumberTile();i++)
		if(Mesh->GetTile(i)->GetName()==name) return i;
	return -1;
}
cMesh* cMeshLibrary::LoadMorph(unsigned int Type,int NumberMorph,float *time,char **name3dsMorph,char *FilePath,char *TexturePath)
{
	cMesh *Mesh=Find(Type);
	if(NumberMorph==0) return Mesh;
	cMesh **Morph=new cMesh*[NumberMorph];
	XBuffer buf; buf<FilePath<name3dsMorph[0];
	Morph[0]=Load3ds(buf.address(),TexturePath,Type);
	if(Mesh==0)
	{
		Mesh=AddMesh(Morph[0]->GetName());
		Morph[0]->CreateCopyMem(Type,Mesh);
		Mesh->Type=Type;
	}
	Morph[0]->Type=M3D_TOTAL_TYPE(M3D_KIND(Type),M3D_MORPH);
	if(Mesh->Frame==0) Mesh->Frame=new cFrame;
	int i;
	for(i=1;i<NumberMorph;i++)
	{
		XBuffer buf; buf<FilePath<name3dsMorph[i];
		cMesh *mesh=0;
		for(cList *start=MeshList->next; start; start=start->next)
			if((start->Mesh->Type==Morph[0]->Type)&&(start->Mesh->GetFileName()==buf.address()))
				{ mesh=start->Mesh; break; }
		if(mesh==0)
		{
			mesh=this->Load3ds(buf.address(),TexturePath,M3D_TOTAL_TYPE(M3D_KIND(Type),M3D_MORPH));
			for(int nTile=0;nTile<Mesh->GetNumberTile();nTile++)
			{
				int n=-1;
				sTile *tmp=mesh->GetTile(nTile);
				if((n=FindTile(mesh,Mesh->GetTile(nTile)->GetName()))<0) 
				{ 
					XBuffer bufs(1000); 
					bufs<"Error: cMeshLibrary::LoadMorph()\r\nName tile not found = "<Mesh->GetTile(nTile)->GetName()<" in file "<buf.address(); 
					ErrAbort(bufs.address()); 
				}
				mesh->Tile->Swap(nTile,n);
				sTile *a=Mesh->GetTile(nTile),*b=mesh->GetTile(nTile);
				if((a->GetNumberPoint()!=b->GetNumberPoint())||(a->GetNumberPolygon()!=b->GetNumberPolygon()))
				{
					XBuffer buf(1000);
					buf<"Error: cMeshLibrary::LoadMorph()\r\nError morph object\r\n3dsfile="<Mesh->GetFileName()<" object="<mesh->GetTile(nTile)->GetName()<" nPoint="<=mesh->GetTile(nTile)->GetNumberPoint()<" nFace="<=mesh->GetTile(nTile)->GetNumberPolygon()<"\r\n";
					buf<"3dsfile="<Mesh->GetFileName()<" object="<a->GetName()<" nPoint="<=a->GetNumberPoint()<" nFace="<=a->GetNumberPolygon();
					ErrAbort(buf.address());
				}
			}
		}
		Morph[i]=mesh;
	}
	for(i=0;i<NumberMorph;i++)
	{
		cMesh *mesh=Morph[i];
		Vect3f dv(Mesh->x()-mesh->x(),Mesh->y()-mesh->y(),Mesh->z()-mesh->z());
		mesh->Tile->TranslateTri(-dv);
		mesh->x()=Mesh->x();
		mesh->y()=Mesh->y();
		mesh->z()=Mesh->z();
	}
	Mesh->Frame->SetMKFrame(Morph,time,NumberMorph);
	delete Morph;
	return Mesh;
}
cMesh* cMeshLibrary::AddMesh(char *NameMesh)
{ 
	cMesh *Mesh=new cMesh();
	Mesh->SetName(NameMesh);
	return Attach(Mesh)->Mesh;
}
cList* cMeshLibrary::Attach(cMesh *Mesh)
{
	cList *List=new cList(Mesh);
	MeshList->Attach(List);
	return List;
}
void cMeshLibrary::Detach(cMesh *Mesh)
{
	cList *start;
	for(start=MeshList->next; start; start=start->next)
		if(start->Mesh==Mesh) break;
	if(start) delete start;
}
void cMeshLibrary::Delete(cMesh *Mesh)
{
	Detach(Mesh);
	delete Mesh; 
}
void cMeshLibrary::Release(unsigned int Type)
{
	cList *start=MeshList->next,*tmp; 
	while((tmp=start)!=0)
	{
		start=start->next;
		if((Type==0)||((M3D_TYPE(Type)&M3D_TYPE(tmp->Mesh->Type))!=0)) 
			Delete(tmp->Mesh);
	}
	if(Type==0) 
		delete MeshList;
}

#include "aci_parser.h"
#include "m3dsetup.h"

cMesh* cM3D::CreateObject(unsigned int *vType,int number)
{
	if(number<=0) return 0;
	int i;
	for(i=0;i<number;i++)
		for(int j=i;j<number;j++)
			if(M3D_TYPE(vType[i])>M3D_TYPE(vType[j])) { int l=vType[i]; vType[i]=vType[j]; vType[j]=l; }
	cMesh *TotalParent=CreateObject(vType[0]);
	for(i=1;i<number;i++)
	{
		int ptype = M3D_PARENT(M3D_TYPE(vType[i]));
		cMesh* child = MeshLibrary->Get(vType[i]);
		cMesh *Parent=TotalParent->FindMesh(ptype);
		Vect3f r = child -> CalcDummyOffset(Parent);
		Parent->AddChild(child);
		SetPosition(child,r.x,r.y,r.z,0,0,0);
	}
	TotalParent->ReCalcTotalBound();
	TotalParent->ReCalcMatrix();
	return TotalParent;
}

cMesh* cM3D::RebuildObject(cMesh *Mesh,unsigned int OldType,unsigned int NewType)
{
	cMesh *OldMesh=Mesh->FindMesh(OldType),*tmp;
	if(OldMesh==0) { XBuffer buf; buf<"Error: cM3D::RebuildObject()\r\nMesh not found type = "<=OldType; ErrAbort(buf.address()); }
	
	OldMesh->Type=0; // очистка объекта
	while((tmp=OldMesh->FindMesh(OldType))!=0)
	{ tmp->DetachChild(); delete tmp; }
	if(OldMesh->Frame) { delete OldMesh->Frame; OldMesh->Frame=0; }
	if(OldMesh->Bound) { delete OldMesh->Bound; OldMesh->Bound=0; }

	cMesh *NewMesh=MeshLibrary->Find(NewType);
	if(NewMesh==0) { XBuffer buf; buf<"Error: cM3D::RebuildObject()\r\nMesh not found type = "<=NewType; ErrAbort(buf.address()); }
	NewMesh->CreateCopyMesh(NewType,OldMesh);
	OldMesh->Type=NewType;
	return Mesh;
}
cMesh* cM3D::FindUnique(unsigned int unique)
{
	cMesh *tmp;
	for(cList *start=MeshList->next;start;start=start->next)
		if((tmp=start->Mesh->FindUnique(unique))!=0) return tmp;
	return 0;
}
void cM3D::SetAttribute(unsigned int Type,int attribute)
{
	for(cList *start=MeshList->next;start;start=start->next)
		if((M3D_TYPE(start->Mesh->Type)&M3D_TYPE(Type))!=0) start->Mesh->SetAttribute(attribute);
}
#ifdef _MECHOSOMA_
void cM3D::SetArcane(void (*FunctionSetArcane)(mchArcaneData*))
{
	for(cList *start=MeshList->next;start;start=start->next)
		start->Mesh->SetArcane(FunctionSetArcane);
}
void cM3D::TraceStaticObjectMechos(cUnknownClass *UCameraList)
{
	extern int acsShopFlag;
	if(acsShopFlag) return;
	cMesh *Mechos=0;
	for(cList *start=MeshList->next;start;start=start->next)
		if(start->Mesh->Attribute&MESH_FIRST_DRAW)
			Mechos=start->Mesh;
	if(Mechos==0) return;
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &ArrayCamera=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int i=0;i<ArrayCamera.length();i++)
	{
		MatXf Matrix;
		Vect3f pvMechos,peMechos;
		assert(ArrayCamera[i]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)ArrayCamera[i];
		Camera->BuildMatrix(Matrix,Mechos->GlobalMatrix.trans());
		Camera->ConvertPoint(Matrix,Vect3f(0,0,0),pvMechos,peMechos);
		float rMechos=Mechos->rmaxTotal()*peMechos.z;
		for(cList *start=MeshList->next;start;start=start->next)
			if(IS_STATIC(start->Mesh->Type))
			{
				cMesh *Mesh=start->Mesh;
				if((Mesh->GetVisibleTotal(i)&CONST_VISIBLE_FRUSTUM)&&(!(Mesh->Attribute&MESH_USE_OPACITY))&&(Mesh->Attribute&MESH_CAMERAEFFECT))
				{
					Vect3f pvMesh,peMesh;
					Camera->BuildMatrix(Matrix,Mesh->GlobalMatrix.trans());
					Camera->ConvertPoint(Matrix,Vect3f(0,0,0),pvMesh,peMesh);
					float r=Mesh->rmaxTotal()*peMesh.z;
					float dx=peMesh.x-peMechos.x, dy=peMesh.y-peMechos.y;
					float dist=sqrtf(dx*dx+dy*dy);
					float a=dist/(r+rMechos);
					if(a<0) a=0;
					sColor4f &Diffuse=Mesh->GetDiffuse();
					if(a>=1.f) Mesh->SetColor(Diffuse.r,Diffuse.g,Diffuse.b,1.f);
					else Mesh->SetColor(Diffuse.r,Diffuse.g,Diffuse.b,(a+1)*0.5f);
				}
			}
	}
}
#endif

void cM3D::Animate(cUnknownClass *UCameraList,int CurrentTime,int PreviousTime)
{ // выставление анимации с пересчетом матрицы
	float dTime=CurrentTime-PreviousTime;
	for(cList *start=MeshList->next;start;start=start->next) 
		start->Mesh->Animate(dTime);
#ifdef _MECHOSOMA_
	TraceStaticObjectMechos(UCameraList);
#endif
}
void cM3D::ShadeDynamic(cUnknownClass *UCameraList,cUnknownClass *UTileMap)
{
	if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
		for(cList *start=MeshList->next;start;start=start->next)
			if(((!start->Mesh->GetAttribute(MESH_NOT_SHADE|MESH_STATIC_SHADE))&&(start->Mesh->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM))||(start->Mesh->GetAttribute(MESH_SHADE_ALWAYS)))
				start->Mesh->ShadeDynamic(UTileMap);
}
void cM3D::TestVisible(cUnknownClass *UCameraList)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	for(cList *start=MeshList->next;start;start=start->next) 
	{ // тест на видимость объекта
		cMesh &Mesh=*start->Mesh;
		if(Mesh.isVisibleTotal(UCameraList)&CONST_VISIBLE_SHOW)
			Mesh.TestVisible(UCameraList);
	}
}
void cM3D::Draw(cUnknownClass *UScene)
{ // отрисовка закэшированных объектов 
	assert(UScene->GetKind(KIND_SCENE));
	cScene *Scene=(cScene*)UScene;
	cUnknownClass *UCameraList=Scene->GetCameraList();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	// отрисовка всех не прозрачных объектов
	cList *start;
	for(start=MeshList->next;start;start=start->next)
		if(start->Mesh->GetAttribute(MESH_USE_OPACITY|MESH_NOT_WRITEZBUFFER)==0)
			if(start->Mesh->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM)
				start->Mesh->Draw(UScene,UCameraList);
	// отрисовка всех прозрачных объектов
	for(start=MeshList->next;start;start=start->next)
		if(start->Mesh->GetAttribute(MESH_USE_OPACITY|MESH_NOT_WRITEZBUFFER))
			if(start->Mesh->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM)
				start->Mesh->Draw(UScene,UCameraList);
}
void cM3D::DrawReflection(cUnknownClass *UScene)
{
	int MeshType=0;
	if(!GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_MECHOS)&&!GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_DYNAMIC_OBJECT)&&!GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_STATIC_OBJECT)) return;
	if(GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_MECHOS)) MeshType|=M3D_ENGINE|M3D_FRONT|M3D_RF_WHEEL|M3D_RB_WHEEL|M3D_LF_WHEEL|M3D_LB_WHEEL;
	if(GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_DYNAMIC_OBJECT)) MeshType|=0xFFFFFFFF^M3D_STATIC;
	if(GET_RENDER_TUNING(RENDER_TUNING_REFLECTION_STATIC_OBJECT)) MeshType|=M3D_STATIC;
	assert(UScene->GetKind(KIND_SCENE));
	cScene *Scene=(cScene*)UScene;
	cUnknownClass *UCameraList=Scene->GetCameraList();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cTileMap *tMap=Scene->GetTileMap();
	if((tMap==0)||(tMap->GetZReflectionSurface()<0)) return;
	GlobalRenderTuning|=RENDER_TUNING_OBJECT_REFLECTION_MAP;
	for(cList *start=MeshList->next;start;start=start->next)
		if((start->Mesh->Type&MeshType)||(start->Mesh->Attribute&MESH_REFLECTION))
			if(start->Mesh->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM)
				start->Mesh->DrawReflection(UScene,UCameraList);
	GlobalRenderTuning&=~RENDER_TUNING_OBJECT_REFLECTION_MAP;
}
void cM3D::DrawMask(cCamera *Camera,unsigned int Mask)
{
/*
	if(Mask==0) 
	{
		for(cList *start=MeshList->next;start;start=start->next)
			if(IS_STATIC(start->Mesh->Type)) 
				start->Mesh->Draw(Camera); 
	}
	else
	{
		for(cList *start=MeshList->next;start;start=start->next)
			if(!IS_STATIC(start->Mesh->Type)) 
				start->Mesh->Draw(Camera); 
	}
*/
}
void cM3D::Release(unsigned int Type)
{ 
	cList *start=MeshList->next,*tmp; 
	while((tmp=start)!=0)
	{
		start=start->next;
		if((Type==0)||((M3D_TYPE(Type)&M3D_TYPE(tmp->Mesh->Type))!=0)) 
			Delete(tmp->Mesh);
	}
	if(Type==0) assert(MeshList->next==0);
	if(Type==0) delete MeshList;
}
cList* cM3D::AttachEnd(cMesh *Mesh)
{
	cList *List=new cList(Mesh);
	cList *start=MeshList; 
	while(start->next!=0) 
		start=start->next;
	start->Attach(List);
	return List;
}
cList* cM3D::Attach(cMesh *Mesh)
{
	if(Mesh->Attribute&MESH_USE_OPACITY) return AttachEnd(Mesh);
	cList *List=new cList(Mesh);
	MeshList->Attach(List);
	return List;
}
void cM3D::Detach(cMesh *Mesh)
{
	cList *start;
	for(start=MeshList->next; start; start=start->next)
		if(start->Mesh==Mesh) break;
	if(start) { if(Window==start) Window=start->next; delete start; }
}
void cM3D::Delete(cMesh *Mesh)
{
	if(Mesh->ID==NumberID) NumberID--;
	Detach(Mesh);
	delete Mesh; 
}
int GetNumber(cList *start,int newID)
{
	for(;start;start=start->next)
		if(start->Mesh->ID==newID)
			return 1;
	return 0;
}
cMesh* cM3D::CreateObject(char *fname,char *TexturePath,unsigned int Type,float SizeObject)
{ // only static object
	cMesh *Mesh;
	if(M3D_KIND(Type)==0) Mesh=MeshLibrary->Get(fname,TexturePath,M3D_TOTAL_TYPE(NumberID+1,M3D_STATIC_NON_COLLIDING),SizeObject);
	else Mesh=MeshLibrary->Get(fname,TexturePath,Type,SizeObject);
	Attach(Mesh);
	int newID;
	for(newID=0;GetNumber(MeshList->next,newID);newID++);
	if(NumberID<newID) NumberID=newID;
	Mesh->ID=newID;
	Mesh->SetVisible(CONST_VISIBLE_SHOW);
	return Mesh;
}
cMesh* cM3D::CreateObject(unsigned int Type,float x,float y,float z,float ax,float ay,float az)
{
	cMesh *Mesh=CreateObject(Type);
	SetPosition(Mesh,x,y,z,ax,ay,az);
	return Mesh;
}
void cM3D::LoadLib(char *fname)
{
//	allocation_tracking("Begin Load Lib");
	int kind, type, eff_id;
	char* p3ds, *pbmp,*p3ds0,*pbmp0,*name3ds,*name3dsMorph[30];
	float time[30];
	double sx,sy,sz;

	XBuffer XBuf;
	cMesh* mesh;

	scrDataBlock* p,*p1,*root,*ap;
	root = loadScript(fname);

	p3ds0 = NULL;
	pbmp0 = NULL;

	p = root -> find(M3D_3DS_PATH);
	if(p) p3ds0 =  p -> c_dataPtr;
	p = root -> find(M3D_BMP_PATH);
	if(p) pbmp0 =  p -> c_dataPtr;
	p = root -> find(M3D_DEFAULT_ATTRIBUTE);
	int default_attribute = p ? *p -> i_dataPtr : 0;

	if(!p3ds0 || !pbmp0) ErrAbort("NULL 3DS Path...");

	p = root -> nextLevel -> first();
	while(p){
		if(p -> ID == M3D_OBJECT){
			ap = NULL;
			eff_id = 0;

			name3ds=0;
			p3ds = p3ds0;
			pbmp = pbmp0;
			sx = sy = sz = 1.0;
			type = *p -> i_dataPtr;
			p1 = p -> nextLevel -> first();
			double size=0;
			char NumberMorph=0,MeshSetPivot=0;
			float animation_phase = 0;
			int animation_period = 0;
			int attribute = default_attribute;
			while(p1){
				switch(p1 -> ID){
					case M3D_SOUND_ID:
#ifdef _MECHOSOMA_
						eff_id = *p1 -> i_dataPtr;
#endif
						break;
					case M3D_ARCANE:
#ifdef _MECHOSOMA_
						ap = p1;
#endif
						break;
					case M3D_Type:
						kind = *p1 -> i_dataPtr;
						break;
					case M3D_3DS_PATH:
						p3ds = p1 -> c_dataPtr;
						break;
					case M3D_BMP_PATH:
						pbmp = p1 -> c_dataPtr;
						break;
					case M3D_3DS_FILE:
						name3ds = p1 -> c_dataPtr;
						break;
					case M3D_SIZE:
						size = *p1 -> d_dataPtr;
						break;
					case M3D_SCALE:
						sx = sy = sz = *p1 -> d_dataPtr;
						break;
					case M3D_SCALE_X:
						sx = *p1 -> d_dataPtr;
						break;
					case M3D_SCALE_Y:
						sy = *p1 -> d_dataPtr;
						break;
					case M3D_SCALE_Z:
						sz = *p1 -> d_dataPtr;
						break;
					case M3D_MORPH_FILE:
						name3dsMorph[NumberMorph] = p1 -> c_dataPtr;
						break;
					case M3D_MORPH_TIME:
						time[NumberMorph++] = (float) *p1 -> d_dataPtr;
						break;

					case M3D_SET_PIVOT:
						MeshSetPivot = *p1 -> i_dataPtr;
						break;
					case M3D_ANIMATION_PHASE:
						animation_phase = (float) *p1 -> d_dataPtr;
						break;
					case M3D_ANIMATION_PERIOD:
						animation_period = *p1 -> i_dataPtr;
						break;
					case M3D_ATTRIBUTE:
						attribute = *p1 -> i_dataPtr;
						break;
				}
				p1 = p1 -> next;
			}
			XBuf.init();
			XBuf < p3ds < name3ds;
#ifdef _MY_TEST_SCR_
			extern unsigned int ArrayType[100000];
			extern int NumberMeshScr;
			ArrayType[NumberMeshScr++]=M3D_TOTAL_TYPE(kind, type);
#endif
			if(MeshLibrary->Find(M3D_TOTAL_TYPE(kind, type))) { XBuffer buf; buf<"Error: cM3D::LoadLib()\r\nMesh double type "<M3D_TOTAL_TYPE(kind, type); ErrAbort(buf.address()); }
				if(name3ds) mesh=MeshLibrary->Load3ds(XBuf.address(),pbmp,M3D_TOTAL_TYPE(kind, type));
			if(NumberMorph>0) mesh=MeshLibrary->LoadMorph(M3D_TOTAL_TYPE(kind, type),NumberMorph,time,name3dsMorph,p3ds,pbmp);
			if(MeshSetPivot) 
			{
				mesh->Displace(mesh->x()-TmpPivot[0],mesh->y()-TmpPivot[1],mesh->z()-TmpPivot[2]);
				mesh->ReCalcTotalBound();
			}
			if(mesh) 
			{
				mesh -> SetAttribute(attribute);
				mesh -> SetAnimation(animation_period, animation_phase);
				if(size!=0) 
					mesh->SetWireSize(Vect3f(size,size,size));
				else 
					mesh->SetWireSize(Vect3f(mesh->rmaxTotal()*sx,mesh->rmaxTotal()*sy,mesh->rmaxTotal()*sz));
				mesh -> SetPosition(MatXf::ID);
				mesh -> CalcMatrix();
			}
#ifdef _MECHOSOMA_
			if(mesh)
			{
				mesh -> SoundID = eff_id;
				mesh->arcaneData.init(ap);
			}
#endif
		}
#ifdef _MECHOSOMA_
		if(p -> ID == M3D_BONUS){
			mchCreateBonusArcane(p);
		}
#endif
		p = p -> next;
	}
	delete root;
//	allocation_tracking("End Load Lib");
}
cMesh* cM3D::CreateObject(unsigned int Type)
{
	cMesh *Mesh=MeshLibrary->Get(Type);
	Mesh->SetVisible(CONST_VISIBLE_SHOW);
	Mesh->ID=++NumberID;
	Attach(Mesh);
	return Mesh;
}
cMesh* cM3D::FindObject(char* name)
{
	cMesh* m = First();
	while(m){
		if(!strcmp(m ->GetName(), name))
			return m;
		m = Next();
		}
	return 0;
}
cMesh* cM3D::FindObjectByFileName(char* fname)
{
	cString fName=fname;
	_strlwr(fName);
	cMesh* m = First();
	while(m)
	{
		if(strstr(m -> GetFileName(), fName))
			return m;
		m = Next();
	}
	return 0;
}
cMesh* cM3D::FindNextObject(char* name)
{
	cMesh* m = Next();
	while(m){
		if(!strcmp(m -> GetName(), name))
			return m;
		m = Next();
		}
	return 0;
}

char* strtest(char *str,char ch)
{
	char *buf=str,*tmp=str;
	while((*buf)!=0) { if((*buf)==ch) tmp=(buf+1); buf++; }
	return tmp;
}
void cM3D::SaveMap(char *fname,char Version)
{
	cList *start;
	for(start=MeshList->next;start;start=start->next)
		if(start->Mesh==0) { ErrAbort("Error: cM3D::SaveMap()\r\nUse mesh not init\r\n"); return; }
	XStream f(fname,XS_OUT);
	f.write(&Version,1);
	for(start=MeshList->next;start;start=start->next)
	{
		float x,y,z;
		cMesh *Mesh=start->Mesh;
		char *name3ds,*TexturePath;
		name3ds=strstr(Mesh->GetFileName(),"model3d");
		TexturePath=strstr(Mesh->GetTexturePath(),"model3d");
		switch (Version)
		{
			case 'b':
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				f.write(name3ds,sizeof(Mesh->GetFileName()));
				f.write(TexturePath,sizeof(Mesh->GetTexturePath()));
				x=Mesh->x(); y=Mesh->y(); z=Mesh->z();
				f.write(&x,sizeof(x)); f.write(&y,sizeof(y));	f.write(&z,sizeof(z));
//				f.write(&Mesh->ax,sizeof(Mesh->ax));f.write(&Mesh->ay,sizeof(Mesh->ay));f.write(&Mesh->az,sizeof(Mesh->az));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				break;
			case 't':
				f<"\r\n"<=Mesh->ID<"\r\n";
				f<name3ds<"\r\n";
				f<TexturePath<"\r\n";
				f<=Mesh->x()<" "<=Mesh->y()<" "<=Mesh->z()<"\r\n";
//				f<=Mesh->ax<" "<=Mesh->ay<" "<=Mesh->az<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				break;
			case 'B': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				f.write(tmp,sizeof(Mesh->GetFileName()));
				x=Mesh->x(); y=Mesh->y(); z=Mesh->z();
				f.write(&x,sizeof(x)); f.write(&y,sizeof(y));	f.write(&z,sizeof(z));
//				f.write(&Mesh->ax,sizeof(Mesh->ax));f.write(&Mesh->ay,sizeof(Mesh->ay));f.write(&Mesh->az,sizeof(Mesh->az));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z)); }
				break;
			case 'T': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID<"\r\n";
				f<tmp<"\r\n";
				f<=Mesh->x()<" "<=Mesh->y()<" "<=Mesh->z()<"\r\n";
//				f<=Mesh->ax<" "<=Mesh->ay<" "<=Mesh->az<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n"; }
				break;
			case 'c': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				f.write(tmp,sizeof(Mesh->GetFileName()));
				x=Mesh->x(); y=Mesh->y(); z=Mesh->z();
				f.write(&x,sizeof(x)); f.write(&y,sizeof(y));	f.write(&z,sizeof(z));
//				f.write(&Mesh->ax,sizeof(Mesh->ax));f.write(&Mesh->ay,sizeof(Mesh->ay));f.write(&Mesh->az,sizeof(Mesh->az));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z)); }
				break;
			case 'q': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID<"\r\n";
				f<tmp<"\r\n";
				f<=Mesh->x()<" "<=Mesh->y()<" "<=Mesh->z()<"\r\n";
//				f<=Mesh->ax<" "<=Mesh->ay<" "<=Mesh->az<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n"; }
				break;
			case 'C': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				x=Mesh->x(); y=Mesh->y(); z=Mesh->z();
				f.write(&x,sizeof(x)); f.write(&y,sizeof(y));	f.write(&z,sizeof(z));
//				f.write(&Mesh->ax,sizeof(Mesh->ax));f.write(&Mesh->ay,sizeof(Mesh->ay));f.write(&Mesh->az,sizeof(Mesh->az));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z)); }
				break;
			case 'Q': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->x()<" "<=Mesh->y()<" "<=Mesh->z()<"\r\n";
//				f<=Mesh->ax<" "<=Mesh->ay<" "<=Mesh->az<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n"; }
				break;
			case 'd': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				x=Mesh->x(); y=Mesh->y(); z=Mesh->z();
				f.write(&x,sizeof(x)); f.write(&y,sizeof(y));	f.write(&z,sizeof(z));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
				break; }
			case 'r': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->x()<" "<=Mesh->y()<" "<=Mesh->z()<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
				break; }
			case 'D': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
				break; }
			case 'f': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
#ifdef _SURMAP_
				f.write(&Mesh->NumberTrack,sizeof(Mesh->NumberTrack));
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f.write(&NumberTrack,sizeof(NumberTrack));
#endif
				break; }
			case 'F': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
#ifdef _SURMAP_
				f.write(&Mesh->NumberTrack,sizeof(Mesh->NumberTrack));
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f.write(&NumberTrack,sizeof(NumberTrack));
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f.write(&AnimatePeriod,sizeof(AnimatePeriod));
				f.write(&AnimatePhase,sizeof(AnimatePhase));
				break; }
			case 'g': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				if(Mesh->Type&M3D_STATIC_COLLIDING) f.write("+",1);
				else f.write("-",1);
				f.write(tmp,sizeof(Mesh->GetFileName()));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
#ifdef _SURMAP_
				f.write(&Mesh->NumberTrack,sizeof(Mesh->NumberTrack));
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f.write(&NumberTrack,sizeof(NumberTrack));
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f.write(&AnimatePeriod,sizeof(AnimatePeriod));
				f.write(&AnimatePhase,sizeof(AnimatePhase));
				int MeshAttribute=0;
				if(Mesh->GetAttribute(MESH_CAMERAEFFECT)) MeshAttribute|=1<<0;
				if(Mesh->GetAttribute(MESH_VISIBLE_SENSOR)) MeshAttribute|=1<<1;
				if(Mesh->GetAttribute(MESH_UNVISIBLE_SENSOR)) MeshAttribute|=1<<2;
				if(Mesh->GetAttribute(MESH_NOT_LIGHTING))  MeshAttribute|=1<<3;
				if(Mesh->GetAttribute(MESH_NOT_SHADE)) MeshAttribute|=1<<4;
				if(Mesh->GetAttribute(MESH_DESTROYMECHOS)) MeshAttribute|=1<<5;
				f.write(&MeshAttribute,sizeof(MeshAttribute));
				break; }
			case 'G': {
				char *tmp=strtest(name3ds,'\\');
				f.write(&Mesh->ID,sizeof(Mesh->ID));
				f.write(tmp,sizeof(Mesh->GetFileName()));
				f.write(&Mesh->GetScale().x,sizeof(Mesh->GetScale().x));f.write(&Mesh->GetScale().y,sizeof(Mesh->GetScale().y));f.write(&Mesh->GetScale().z,sizeof(Mesh->GetScale().z));
				
				f.write(&Mesh->LocalMatrix(0,0),sizeof(Mesh->LocalMatrix(0,0)));
				f.write(&Mesh->LocalMatrix(0,1),sizeof(Mesh->LocalMatrix(0,1)));
				f.write(&Mesh->LocalMatrix(0,2),sizeof(Mesh->LocalMatrix(0,2)));
				f.write(&Mesh->LocalMatrix(0,3),sizeof(Mesh->LocalMatrix(0,3)));

				f.write(&Mesh->LocalMatrix(1,0),sizeof(Mesh->LocalMatrix(1,0)));
				f.write(&Mesh->LocalMatrix(1,1),sizeof(Mesh->LocalMatrix(1,1)));
				f.write(&Mesh->LocalMatrix(1,2),sizeof(Mesh->LocalMatrix(1,2)));
				f.write(&Mesh->LocalMatrix(1,3),sizeof(Mesh->LocalMatrix(1,3)));

				f.write(&Mesh->LocalMatrix(2,0),sizeof(Mesh->LocalMatrix(2,0)));
				f.write(&Mesh->LocalMatrix(2,1),sizeof(Mesh->LocalMatrix(2,1)));
				f.write(&Mesh->LocalMatrix(2,2),sizeof(Mesh->LocalMatrix(2,2)));
				f.write(&Mesh->LocalMatrix(2,3),sizeof(Mesh->LocalMatrix(2,3)));
#ifdef _SURMAP_
				f.write(&Mesh->NumberTrack,sizeof(Mesh->NumberTrack));
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f.write(&NumberTrack,sizeof(NumberTrack));
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f.write(&AnimatePeriod,sizeof(AnimatePeriod));
				f.write(&AnimatePhase,sizeof(AnimatePhase));
				int MeshAttribute=0;
				if(Mesh->GetAttribute(MESH_CAMERAEFFECT)) MeshAttribute|=1<<0;
				if(Mesh->GetAttribute(MESH_VISIBLE_SENSOR)) MeshAttribute|=1<<1;
				if(Mesh->GetAttribute(MESH_UNVISIBLE_SENSOR)) MeshAttribute|=1<<2;
				if(Mesh->GetAttribute(MESH_NOT_LIGHTING))  MeshAttribute|=1<<3;
				if(Mesh->GetAttribute(MESH_NOT_SHADE)) MeshAttribute|=1<<4;
				if(Mesh->GetAttribute(MESH_DESTROYMECHOS)) MeshAttribute|=1<<5;
				f.write(&MeshAttribute,sizeof(MeshAttribute));
				f.write("ch",1);
				if(Mesh->description) f.write(Mesh->description,strlen(Mesh->description));
				f.write("ch",1);
				break; }
			case 'R': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
				break; }
			case 's': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
#ifdef _SURMAP_
				f<=Mesh->NumberTrack<"\r\n";
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f.write(&NumberTrack,sizeof(NumberTrack));
#endif
				break; }
			case 'S': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
#ifdef _SURMAP_
				f<=Mesh->NumberTrack<"\r\n";
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f<=NumberTrack;
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f<=AnimatePeriod<" "<=AnimatePhase<"\r\n";
				break; }
			case 'o': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID;
				if(Mesh->Type&M3D_STATIC_COLLIDING) f < " +\r\n";
				else f < " -\r\n";
				f<tmp<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
#ifdef _SURMAP_
				f<=Mesh->NumberTrack<"\r\n";
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f<=NumberTrack;
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f<=AnimatePeriod<" "<=AnimatePhase<"\r\n";
				int MeshAttribute=0;
				if(Mesh->GetAttribute(MESH_CAMERAEFFECT)) MeshAttribute=1<<0;
				if(Mesh->GetAttribute(MESH_VISIBLE_SENSOR)) MeshAttribute|=1<<1;
				if(Mesh->GetAttribute(MESH_UNVISIBLE_SENSOR)) MeshAttribute|=1<<2;
				if(Mesh->GetAttribute(MESH_NOT_LIGHTING)) MeshAttribute|=1<<3;
				if(Mesh->GetAttribute(MESH_NOT_SHADE)) MeshAttribute|=1<<4;
				if(Mesh->GetAttribute(MESH_DESTROYMECHOS)) MeshAttribute|=1<<5;
				f<=MeshAttribute<"\r\n";
				break; }
			case 'O': {
				char *tmp=strtest(name3ds,'\\');
				f<"\r\n"<=Mesh->ID<"\r\n";
				f<tmp<"\r\n";
				f<=Mesh->GetScale().x<" "<=Mesh->GetScale().y<" "<=Mesh->GetScale().z<"\r\n";
				f<=Mesh->LocalMatrix(0,0)<" "<=Mesh->LocalMatrix(0,1)<" "<=Mesh->LocalMatrix(0,2)<" "<=Mesh->LocalMatrix(0,3)<"\r\n";
				f<=Mesh->LocalMatrix(1,0)<" "<=Mesh->LocalMatrix(1,1)<" "<=Mesh->LocalMatrix(1,2)<" "<=Mesh->LocalMatrix(1,3)<"\r\n";
				f<=Mesh->LocalMatrix(2,0)<" "<=Mesh->LocalMatrix(2,1)<" "<=Mesh->LocalMatrix(2,2)<" "<=Mesh->LocalMatrix(2,3)<"\r\n";
#ifdef _SURMAP_
				f<=Mesh->NumberTrack<"\r\n";
#else
				int NumberTrack=MESH_GLOBAL_TRACK;
				f<=NumberTrack;
#endif
				float AnimatePeriod=0,AnimatePhase=0,finish;
				Mesh->GetAnimation(AnimatePeriod,AnimatePhase,finish);
				if(!_finite(AnimatePeriod)) AnimatePeriod=0;
				if(!_finite(AnimatePhase)) AnimatePhase=0;
				f<=AnimatePeriod<" "<=AnimatePhase<"\r\n";
				int MeshAttribute=0;
				if(Mesh->GetAttribute(MESH_CAMERAEFFECT)) MeshAttribute=1<<0;
				if(Mesh->GetAttribute(MESH_VISIBLE_SENSOR)) MeshAttribute|=1<<1;
				if(Mesh->GetAttribute(MESH_UNVISIBLE_SENSOR)) MeshAttribute|=1<<2;
				if(Mesh->GetAttribute(MESH_NOT_LIGHTING)) MeshAttribute|=1<<3;
				if(Mesh->GetAttribute(MESH_NOT_SHADE)) MeshAttribute|=1<<4;
				if(Mesh->GetAttribute(MESH_DESTROYMECHOS)) MeshAttribute|=1<<5;
				f<=MeshAttribute<"\r\n";
				f<"\"";
				if(Mesh->description) f<Mesh->description;
				f<"\"\r\n";
				break; }
			default: XBuffer buf; buf<"Error: unknown version file "<fname<" : "<Version; ErrAbort(buf); break;
		}
	}
	f.close();
}
char GlobalBinaryM3DType[]="bBcCdDfFgG", GlobalTextM3DType[]="tTqQrRsSoO";
inline void ReadSpecString(XStream &f,char *string)
{
	char ch=10;
	while(ch==10||ch==13||ch==32||ch==9) f.read(&ch,1);
	assert(ch=='"');
	int i;
	for(i=0;i<1023;i++)
	{
		f.read(&string[i],1);
		if(string[i]=='"') break;
	}
	string[i]=0;
}
void cM3D::LoadMap(char *fname,int LoadNumberTrack)
{	
	XStream f(0);
	NumberID=0;
#ifdef _MECHOSOMA_
	extern int vmapOpenResource(char* fname,XStream& fh);
	if(!vmapOpenResource(fname,f)) return;
	extern char* getIniKey(char* fname,char* section,char* key);
	XBuffer Path3ds; Path3ds<getIniKey(mch_mainINI,"m3d","path3ds")<"\\";
	XBuffer PathBmp; PathBmp<getIniKey(mch_mainINI,"m3d","pathbmp")<"\\";
#elif _SURMAP_
	if(!f.open(fname,XS_IN)) return;
	XBuffer Path3ds; Path3ds<dirM3D<"\\"; 
	XBuffer PathBmp; PathBmp<dirM3Dtexture<"\\";
#else
	if(!f.open(fname,XS_IN)) return;
	XBuffer Path3ds; Path3ds<dirM3D<"\\"; 
	XBuffer PathBmp; PathBmp<dirM3Dtexture<"\\";
#endif
	char Version;
	f.read(&Version,1);
	unsigned int Type;
	if(strchr(GlobalBinaryM3DType,Version)) f.read(&Type,sizeof(Type)); 
	else if(strchr(GlobalTextM3DType,Version)) f>=Type;
	while(!f.eof())
	{
		cMatrix LocalMatrix;
		int NumberTrack=0xFFFFFFFF,AttributeMesh=0;
		float AnimatePhase=0,AnimatePeriod=2000;
		float x=0,y=0,z=0,ax=0,ay=0,az=0,sx,sy,sz;
		char flCollision='+';
		char name3ds[FILE3DS_NAME_SIZE],TexturePath[TEXTURE_PATH_SIZE],description[1024]="";
		switch (Version) 
		{
			case 'b':
				f.read(name3ds,sizeof(name3ds));
				f.read(TexturePath,sizeof(TexturePath));
				f.read(&x,sizeof(x));  f.read(&y,sizeof(y));  f.read(&z,sizeof(z));
				f.read(&ax,sizeof(ax));f.read(&ay,sizeof(ay));f.read(&az,sizeof(az));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz));
				break;
			case 't':
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f.getline(TexturePath,255);
				f>=x>=y>=z;
				f>=ax>=ay>=az;
				f>=sx>=sy>=sz;
				break;
			case 'B':
				f.read(name3ds,sizeof(name3ds));
				f.read(&x,sizeof(x));  f.read(&y,sizeof(y));  f.read(&z,sizeof(z));
				f.read(&ax,sizeof(ax));f.read(&ay,sizeof(ay));f.read(&az,sizeof(az));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz));
				break;
			case 'T':
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=x>=y>=z;
				f>=ax>=ay>=az;
				f>=sx>=sy>=sz;
				break;
			case 'c':
				f.read(name3ds,sizeof(name3ds));
				f.read(&x,sizeof(x));  f.read(&y,sizeof(y));  f.read(&z,sizeof(z));
				f.read(&ax,sizeof(ax));f.read(&ay,sizeof(ay));f.read(&az,sizeof(az));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz));
				break;
			case 'q':
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=x>=y>=z;
				f>=ax>=ay>=az;
				f>=sx>=sy>=sz;
				break;
			case 'C':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&x,sizeof(x));  f.read(&y,sizeof(y));  f.read(&z,sizeof(z));
				f.read(&ax,sizeof(ax));f.read(&ay,sizeof(ay));f.read(&az,sizeof(az));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 
				break;
			case 'Q':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=x>=y>=z;
				f>=ax>=ay>=az;
				f>=sx>=sy>=sz;
				break;
			case 'd':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&x,sizeof(x));  f.read(&y,sizeof(y));  f.read(&z,sizeof(z));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				break;
			case 'r':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=x>=y>=z;
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				break;
			case 'D':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				break;
			case 'f':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				f.read(&NumberTrack,sizeof(NumberTrack));
				break;
			case 'F':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				f.read(&NumberTrack,sizeof(NumberTrack));
				f.read(&AnimatePeriod,sizeof(AnimatePeriod));
				f.read(&AnimatePhase,sizeof(AnimatePhase));
				break;
			case 'g':
				f.read(name3ds,sizeof(name3ds));
				f.read(&flCollision,sizeof(flCollision));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				f.read(&NumberTrack,sizeof(NumberTrack));
				f.read(&AnimatePeriod,sizeof(AnimatePeriod));
				f.read(&AnimatePhase,sizeof(AnimatePhase));
				f.read(&AttributeMesh,sizeof(AttributeMesh));
				break;
			case 'G':
				f.read(name3ds,sizeof(name3ds));
				f.read(&sx,sizeof(sx));f.read(&sy,sizeof(sy));f.read(&sz,sizeof(sz)); 

				f.read(&LocalMatrix(0,0),sizeof(LocalMatrix(0,0)));
				f.read(&LocalMatrix(0,1),sizeof(LocalMatrix(0,1)));
				f.read(&LocalMatrix(0,2),sizeof(LocalMatrix(0,2)));
				f.read(&LocalMatrix(0,3),sizeof(LocalMatrix(0,3)));

				f.read(&LocalMatrix(1,0),sizeof(LocalMatrix(1,0)));
				f.read(&LocalMatrix(1,1),sizeof(LocalMatrix(1,1)));
				f.read(&LocalMatrix(1,2),sizeof(LocalMatrix(1,2)));
				f.read(&LocalMatrix(1,3),sizeof(LocalMatrix(1,3)));

				f.read(&LocalMatrix(2,0),sizeof(LocalMatrix(2,0)));
				f.read(&LocalMatrix(2,1),sizeof(LocalMatrix(2,1)));
				f.read(&LocalMatrix(2,2),sizeof(LocalMatrix(2,2)));
				f.read(&LocalMatrix(2,3),sizeof(LocalMatrix(2,3)));
				f.read(&NumberTrack,sizeof(NumberTrack));
				f.read(&AnimatePeriod,sizeof(AnimatePeriod));
				f.read(&AnimatePhase,sizeof(AnimatePhase));
				f.read(&AttributeMesh,sizeof(AttributeMesh));
				ReadSpecString(f,description);
				break;
			case 'R':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				break;
			case 's':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				f>=NumberTrack;
				break;
			case 'S':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				f>=NumberTrack;
				f>=AnimatePeriod>=AnimatePhase;
				break;
			case 'o':
				f>flCollision;
				if((flCollision=='\r')||(flCollision=='\n')) f.seek(-1,XS_CUR);
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				f>=NumberTrack;
				f>=AnimatePeriod>=AnimatePhase;
				f>=AttributeMesh;
				if(flCollision=='+') { description[0]='D'; description[1]=0; }
				break;
			case 'O':
				f.getline(name3ds,255);
				f.getline(name3ds,255);
				f>=sx>=sy>=sz;
				f>=LocalMatrix(0,0)>=LocalMatrix(0,1)>=LocalMatrix(0,2)>=LocalMatrix(0,3);
				f>=LocalMatrix(1,0)>=LocalMatrix(1,1)>=LocalMatrix(1,2)>=LocalMatrix(1,3);
				f>=LocalMatrix(2,0)>=LocalMatrix(2,1)>=LocalMatrix(2,2)>=LocalMatrix(2,3);
				f>=NumberTrack;
				f>=AnimatePeriod>=AnimatePhase;
				f>=AttributeMesh;
				ReadSpecString(f,description);
				break;
			default: XBuffer buf; buf<"Error: unknown version file "<fname<" : "<Version; ErrAbort(buf); break;
		}
		int FullType=Type;
		while(FindID(Type)!=0) Type++;
		if(flCollision=='-') FullType=M3D_TOTAL_TYPE(Type,M3D_STATIC_NON_COLLIDING);
		else FullType=M3D_TOTAL_TYPE(Type,M3D_STATIC_COLLIDING);
		char Name3dsFull[FILE3DS_NAME_SIZE],TexturePathFull[TEXTURE_PATH_SIZE],*tmp;
		tmp=strtest(name3ds,'\\'); strcpy(Name3dsFull,Path3ds.address()); strcat(Name3dsFull,tmp);
		strcpy(TexturePathFull,PathBmp.address());
//		if(NumberTrack&LoadNumberTrack)
		{
			cMesh *Mesh=CreateObject(Name3dsFull,TexturePathFull,FullType);
			if(Mesh) { Mesh->SetVisible(CONST_VISIBLE_SHOW); Mesh->ID=Type; }
			if(NumberID<Type) NumberID=Type;
			SetScale(Mesh,sx,sy,sz);
			if((Version=='b')||(Version=='B')||(Version=='t')||(Version=='T'))
			{
				cMatrix Matrix;
				float yr=Mesh->yminReal()+Mesh->ymaxReal();
				Matrix.Rotate(ax,ay,az);
				float dx=-(Matrix(0,1)*yr);
				float dy=-(Matrix(1,1)*yr);
				float dz=-(Matrix(2,1)*yr);
				SetPosition(Mesh,x+dx,y+dy,z+dz,ax,ay,az);
			}
			else // if((Version=='d')||(Version=='r')||(Version=='D')||(Version=='R')||(Version=='s')||(Version=='f')||(Version=='S')||(Version=='F'))
				Mesh->SetPosition(LocalMatrix);
			if(Mesh) Mesh->SetAnimation(AnimatePeriod,AnimatePhase);
#ifdef _SURMAP_
			if(Mesh) Mesh->NumberTrack=NumberTrack;
#else
//			if((Mesh)&&(NumberTrack&STATIC_NUMBER))
				Mesh->Attribute|=MESH_STATIC_SHADE;
			if((Mesh)&&((NumberTrack&STATIC_NUMBER)==0))
			{
				Mesh->Attribute|=MESH_REFLECTION;
/*				short *shade;
				Vect3f PosShade;
				int xShade,yShade;
//				Vect3f PosLight(Mesh->GlobalMatrix.trans().x+100,Mesh->GlobalMatrix.trans().y,1/CONST_DYNAMIC_SHADE_MESH);
				Vect3f PosLight(Mesh->GlobalMatrix.trans().x+100,Mesh->GlobalMatrix.trans().y,1/2.f);
				Mesh->BuildShadeDynamic(&shade,&xShade,&yShade,PosShade,PosLight);
				extern void DrawShadeDynamicToColor16(short *shade,int xShade,int yShade,Vect3f &PosShade);
				if(shade) 
				{
					DrawShadeDynamicToColor16(shade,xShade,yShade,PosShade);
					delete shade;
				}
*/
			}
#endif
			if(Mesh)
			{
				if(AttributeMesh&(1<<0)) Mesh->SetAttribute(MESH_CAMERAEFFECT);
				if(AttributeMesh&(1<<1)) Mesh->SetAttribute(MESH_VISIBLE_SENSOR);
				if(AttributeMesh&(1<<2)) Mesh->SetAttribute(MESH_UNVISIBLE_SENSOR);
				if(AttributeMesh&(1<<3)) Mesh->SetAttribute(MESH_NOT_LIGHTING);
				if(AttributeMesh&(1<<4)) Mesh->SetAttribute(MESH_NOT_SHADE);
				if(AttributeMesh&(1<<5)) Mesh->SetAttribute(MESH_DESTROYMECHOS);
				Mesh->SetDescription(description);
			}
		}
		if(strchr(GlobalBinaryM3DType,Version)) f.read(&Type,sizeof(Type)); 
		else if(strchr(GlobalTextM3DType,Version)) f>=Type;
	}
	f.close();
}
void cM3D::dSetPosition(cMesh *Mesh,float dx,float dy,float dz,float dax,float day,float daz)
{ 
	if(Mesh==0) return; 
	Mesh->LocalMatrix.Translate(dx,dy,dz);
	Mesh->LocalMatrix.Rotate(dax,day,daz);
	Mesh->SetPosition(Mesh->LocalMatrix);
}
void cM3D::SetPosition(cMesh *Mesh,float x,float y,float z,float ax,float ay,float az)
{ 
	if(Mesh==0) return; 
	Mesh->LocalMatrix.NewMatrix();
	Mesh->LocalMatrix.Translate(x,y,z);
	Mesh->LocalMatrix.Rotate(ax,ay,az);
	Mesh->SetPosition(Mesh->LocalMatrix);
}
void cM3D::DrawCircleDynamic(Vect3f &pos,sColor4f &color,float radius)
{
/*	if(!GET_RENDER_TUNING(RENDER_TUNING_SHADOW)) return;
//	assert(tMap);
	iTileMap=jTileMap=-1;
	pos.x=fmod(pos.x+2048.f,2048.f); pos.y=fmod(pos.y+2048.f,2048.f);
	int r=color.GetR(),g=color.GetG(),b=color.GetB(),a=color.GetA();
	int xb=pos.x-radius, xe=pos.x+radius, yb=pos.y-radius, ye=pos.y+radius;
	int dx=(1<<(OMNI_MAP_SIZE+16))/(xe-xb),dy=(1<<(OMNI_MAP_SIZE+16))/(ye-yb);
	dx<<=MAP_LIGHTMAP_STEP_BIT; dy<<=MAP_LIGHTMAP_STEP_BIT;
#ifdef _MECHOSOMA_
	for(int y=yb,jlmap=0;y<ye;y+=(1<<MAP_LIGHTMAP_STEP_BIT),jlmap+=dy)  
	{
		int y_cycl=YCYCL(y);
		unsigned char *OmniLMap=&OmniLightMap[(jlmap>>16)<<OMNI_MAP_SIZE];
		for(int x=xb,ilmap=0;x<xe;x+=(1<<MAP_LIGHTMAP_STEP_BIT),ilmap+=dx)
		{
			int x_cycl=XCYCL(x);
			int Alfa=(a*OmniLMap[ilmap>>16])>>DEPTH_INTENSITY_OMNI_BIT;
			int color4444=(((r*Alfa)>>(16-rCount4444))<<rShift4444)|
				(((g*Alfa)>>(16-gCount4444))<<gShift4444)|
				(((b*Alfa)>>(16-bCount4444))<<bShift4444);
//			SetLightingMap(x_cycl,y_cycl,r,g,b,Alfa);
			int i=x_cycl>>tMap->_xTile, j=y_cycl>>tMap->_yTile;
			if((i!=iTileMap)||(j!=jTileMap)) InitShadowLightMap(i,j);
			int u=(x_cycl>>MAP_LIGHTMAP_STEP_BIT)&((1<<CurrentLightMap->_x)-1); 
			int v=(y_cycl>>MAP_LIGHTMAP_STEP_BIT)&((1<<CurrentLightMap->_y)-1); 
			unsigned short *buf=(unsigned short*)&MemTextureTile[v*xLightMapTile];
			buf[u]=((Alfa>>(8-aCount4444))<<aShift4444)|color4444;
		}
	}
#endif*/
}
void DrawShadeDynamicToColor16(short *shade,int xShade,int yShade,Vect3f &PosShade)
{
//	if(RenderMode==XGRAPH_HICOLOR) return;
	int xSh=TERRA_XCYCL(round(PosShade.x)),ySh=TERRA_YCYCL(round(PosShade.y)),zSh=round(PosShade.z);
	short *shadeBuf=shade;
	int dzx=(1<<16)/CONST_DYNAMIC_SHADE_MESH;
#ifdef _USE_TILEMAP_
	for (int j=0;j<yShade;j+=1,shadeBuf+=xShade)  
	{
		int y=YCYCL(ySh+j), x=xSh,x_cycl=xSh,v;
		while((zSh<(v=GetAlt(x_cycl,y)))&&((xSh-x)<xShade))
			x_cycl=XCYCL(x-=1);
		if(v>zSh) continue;
		int xshade=(xSh-x)-(((zSh-v)*dzx)>>16);
		while(xshade<xShade)
		{
			v=GetAlt(x_cycl,y);
			if((xshade>0)&&(shadeBuf[xshade]>=v)&&((vMap_GetAt(x_cycl,y)&TERRA_At_SHADOW)==0))
			{
				float KoofShade=0.6f;
				unsigned short *color=&(vMap_ClBuf()[(TERRA_YCYCL(y)<<TERRA_H_SIZE_POWER)+TERRA_XCYCL(x_cycl)]);
				int r=(color[0]>>11)&((1<<5)-1),g=(color[0]>>5)&((1<<6)-1),b=(color[0]>>0)&((1<<5)-1);
				r=round(r*KoofShade); g=round(g*KoofShade); b=round(b*KoofShade);
				color[0]=(r<<11)+(g<<5)+(b<<0);
				SetAt(x_cycl,y,TERRA_At_SHADOW);
			}
			x_cycl=TERRA_XCYCL(x-=1);
			xshade=(xSh-x)-(((zSh-v)*dzx)>>16);
		}
	}
#ifdef _MECHOSOMA_
	MemoryHeapDynamicShade.init();
#endif //_MECHOSOMA_
#endif
}
void cM3D::RestoreShadeDynamic()
{
	while(BaseList)
	{
		RestoreShadeDynamic(BaseList->Base);
		Pop();
	}
#ifdef _MECHOSOMA_
	MemoryHeapDynamicShade.init();
#endif
}
void cM3D::RestoreShadeDynamic(cMemBuffer *MemBuffer)
{
	if(MemBuffer==0) return;
#ifndef _MECHOSOMA_
	delete MemBuffer;
#endif
	MemBuffer=0;
}

cOmni* cO3D::CreateOmni(float xOmni,float yOmni,float zOmni,float radius,float r,float g,float b)
{
	cOmni *Omni=new cOmni(xOmni,yOmni,zOmni,radius,r,g,b);
	Omni->SetVisible(1);
#ifdef _SURMAP_
	int newID=0,fl=0;
	do{
		newID++;
		for(cOmniList *start=BaseList;start;start=start->next)
			if(fl=(start->Base->ID==newID)) 
				break;
	}while(fl);
	if(NumberID<newID) NumberID=newID;
	Omni->ID=newID;
#else
	Omni->ID=(++NumberID);
#endif
	Attach(Omni);
	return Omni;
}
void cO3D::Release(int type)													
{  
	cOmniList *start=BaseList,*tmp; 
	while((tmp=start)!=0)
	{
		start=start->next;
		Delete(tmp->Base);
	}
}
void cO3D::Draw(cUnknownClass *UCameraList)
{
	for(cOmniList *start=BaseList;start;start=start->next)
		start->Base->Draw(UCameraList);
}
void cO3D::DrawID(cUnknownClass *UCameraList)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &ArrayCamera=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int i=0;i<ArrayCamera.length();i++)
	{
		assert(ArrayCamera[i]->GetKind(KIND_CAMERA));
		for(cOmniList *start=BaseList;start;start=start->next)
			start->Base->DrawID((cCamera*)ArrayCamera[i]);
	}
}
void cO3D::SaveMap(char *fname,char Version)
{
	cOmniList *start;
	for(start=BaseList;start;start=start->next)
		if(start->Base==0) { ErrAbort("Error: cO3D::SaveMap()\r\nUse omni not init\r\n"); return; }
	XStream f(fname,XS_OUT);
	f.write(&Version,1);
	for(start=BaseList;start;start=start->next)
	{
		cOmni *Omni=start->Base;
		if(Version=='b') {
			f.write(&Omni->ID,sizeof(Omni->ID));
			f.write(&Omni->GetPos().x,sizeof(Omni->GetPos().x));	f.write(&Omni->GetPos().y,sizeof(Omni->GetPos().y));	f.write(&Omni->GetPos().z,sizeof(Omni->GetPos().z));
			f.write(&Omni->GetColor().r,sizeof(Omni->GetColor().r));f.write(&Omni->GetColor().g,sizeof(Omni->GetColor().g));f.write(&Omni->GetColor().b,sizeof(Omni->GetColor().b));
			f.write(&Omni->GetRadius(),sizeof(Omni->GetRadius()));	}
		else if(Version=='t') {
			f<"\r\n"<=Omni->ID<"\r\n";
			f<=Omni->GetPos().x<" "<=Omni->GetPos().y<" "<=Omni->GetPos().z<"\r\n";
			f<=Omni->GetColor().r<" "<=Omni->GetColor().g<" "<=Omni->GetColor().b<" "<=Omni->GetRadius();
		}
	}
	f.close();
}
void cO3D::LoadMap(char *fname)
{
	XStream f(0);
	if(!f.open(fname,XS_IN)) return;
	char Version;
	f.read(&Version,1);
	unsigned int ID;
	NumberID=0;
	if(Version=='b') f.read(&ID,sizeof(ID)); else if(Version=='t') f>=ID;
	while(!f.eof())
	{
		float x,y,z,dist,r,g,b;
		if(Version=='b') {
			f.read(&x,sizeof(x)); f.read(&y,sizeof(y));	f.read(&z,sizeof(z));
			f.read(&r,sizeof(r)); f.read(&g,sizeof(g));	f.read(&b,sizeof(b));
			f.read(&dist,sizeof(dist));	} 
		else if(Version=='t')	{
			f>=x>=y>=z;
			f>=r>=g>=b>=dist;	}
//		else ERROR_MESSAGE("Error","Unknown version = ",Version);
		cOmni *Omni=CreateOmni(x,y,z,dist,r,g,b);
		Omni->SetVisible(CONST_VISIBLE); Omni->ID=ID;
		if(NumberID<ID) NumberID=ID;
		if(Version=='b') f.read(&ID,sizeof(ID)); else if(Version=='t') f>=ID;
	}
	f.close();
}
