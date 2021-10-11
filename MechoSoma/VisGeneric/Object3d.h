#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include "Math3d.h"
#include "Object.h"
#include "base.h"
#include "BaseClass.h"
#include "BoundingBox.h"
#include "Material.h"
#include "Dummy.h"

#ifdef _MECHOSOMA_
#include "arcane.h"
#endif

#define GET_POLYGON(p,nPolygon,nPoint,nTexel)				(sPolygon*)&((char*)p)[0]
#define GET_POINT(p,nPolygon,nPoint,nTexel)					(sPoint*)&((char*)p)[nPolygon*sizeof(sPolygon)]
#define GET_TEXEL(p,nPolygon,nPoint,nTexel)					(sTexel*)&((char*)p)[nPoint*sizeof(sPoint)+nPolygon*sizeof(sPolygon)]
#define GET_TEXPOLY(p,nPolygon,nPoint,nTexel)				(sPolygon*)&((char*)p)[nTexel*sizeof(sTexel)+nPoint*sizeof(sPoint)+nPolygon*sizeof(sPolygon)]
#define GET_POINTSIZE(nPolygon,nPoint,nTexel,nTexPoly)		((int)nTexPoly*sizeof(sPolygon)+nTexel*sizeof(sTexel)+nPoint*sizeof(sPoint)+nPolygon*sizeof(sPolygon))

enum eAttributeTile
{
	ATTRTILE_NULL			=	0,
	ATTRTILE_VERTEX			=	1<<0,	// NumberPoint=NumberVertex - NumberTexPoly=0, NumberTexel=0
	ATTRTILE_VERTEX_TEXEL	=	1<<1,	// NumberPoint=NumberVertex - NumberTexPoly=NumberPolygon, NumberVertex=NumberTexel
	ATTRTILE_TEXEL			=	1<<2,	// NumberPoint=3*NumberPolygon
	ATTRTILE_COPY			=	1<<31	// ������ ����� ������� ������������ � ���������� ������
};

class cFrame;

class cLink
{
public:
	cLink	*next,*prev;
	
	cLink()													{ next=prev=0; }
	~cLink()												{ Detach(); }
	void Attach(cLink *Link)
	{
		cLink *Next=next;	Link->next=Next;
		Link->prev=this; next=Link;
		if(Next) Next->prev=Link;
	}
	void AttachEnd(cLink *Link)
	{
		cLink *tmp=this;
		while(tmp->next) tmp=tmp->next;
		Link->next=0;
		Link->prev=tmp;
		tmp->next=Link;
	}
	void Detach()
	{
		if(prev) prev->next=next;
		if(next) next->prev=prev;
		next=prev=0;
	}
};

class cMemBuffer
{
public:
	int xs,ys,x,y,xmax;
	unsigned char *buf;
	cMemBuffer(int xsize,int ysize,int xpos,int ypos)		{ buf=0; xmax=xs=xsize; ys=ysize; x=xpos; y=ypos; }
	~cMemBuffer()											{ if(buf) delete buf; }
};
class cBound
{
public:
	cBoundingBox Total,Real;

	cBound()												{}
	~cBound()												{}
	inline float xminReal()									{ return Real.Min.x; }
	inline float yminReal()									{ return Real.Min.y; }
	inline float zminReal()									{ return Real.Min.z; }
	inline float xmaxReal()									{ return Real.Max.x; }
	inline float ymaxReal()									{ return Real.Max.y; }
	inline float zmaxReal()									{ return Real.Max.z; }
	inline float rmaxReal()									{ return Real.Radius; }
	inline float xminTotal()								{ return Total.Min.x; }
	inline float yminTotal()								{ return Total.Min.y; }
	inline float zminTotal()								{ return Total.Min.z; }
	inline float xmaxTotal()								{ return Total.Max.x; }
	inline float ymaxTotal()								{ return Total.Max.y; }
	inline float zmaxTotal()								{ return Total.Max.z; }
	inline float rmaxTotal()								{ return Total.Radius; }
	void SetReal(const Vect3f &Min,const Vect3f &Max)		{ Real.SetOriginal(Min,Max); }
	void SetTotal(const Vect3f &Min,const Vect3f &Max)	{ Total.SetOriginal(Min,Max); }
	void Scale(const Vect3f &scale)						{ Real.SetScale(scale); Total.SetScale(scale); }
	void SetWireScale(const Vect3f &scale)					{ Real.SetScaleOriginal(scale); Total.SetScaleOriginal(scale); }
};

struct sVertex
{
	Vect3f		pos;	// ���������� � ����������� ������� ���������
	inline float xw()										{ return pos.x; }
	inline float yw()										{ return pos.y; }
	inline float zw()										{ return pos.z; }
	inline void SetVertex(float xw,float yw,float zw)		{ pos.x=xw; pos.y=yw; pos.z=zw; }
};
struct sNormal
{
	sVect3c		normal;	// �������
	inline float nx()										{ return (float)normal.x/127.f; }
	inline float ny()										{ return (float)normal.y/127.f; }
	inline float nz()										{ return (float)normal.z/127.f; }
	inline void SetNormal(float nx,float ny,float nz)		{ normal.x=(int)(nx*127.f); normal.y=(int)(ny*127.f); normal.z=(int)(nz*127.f); }
};
struct sPoint : public sVertex, public sNormal
{
};
struct sTexel
{
	Vect2s		texel;	// ���������� ����������
	inline float u()										{ return texel.x/4096.f; }
	inline float v()										{ return texel.y/4096.f; }
	inline void SetTexel(float u,float v)					{ texel.x=(u*4096.f); texel.y=round(v*4096.f); }
};
struct sPolygon
{
	short			p1,p2,p3;
	inline void set(int i1,int i2,int i3)					{ p1=i1; p2=i2; p3=i3; }
};

struct sObjTri
{
	friend class cTile;
	cString					name;
	char					*p;
	short					nPoint,nTexel;
	sObjTri();
	~sObjTri();
	inline sPoint* GetPoint()								{ return GET_POINT(p,0,nPoint,nTexel); }
	inline sTexel* GetTexel()								{ return GET_TEXEL(p,0,nPoint,nTexel); }
	inline short& GetNumberPoint()							{ return nPoint; }
	inline short& GetNumberTexel()							{ return nTexel; }
	inline void NewTri(int NumberPoint,int NumberTexel)
	{
		nTexel=NumberTexel;
		nPoint=NumberPoint;
		if(p) { delete p; p=0; }
		if(NumberTexel==0)
			p=new char[GET_POINTSIZE(0,nPoint,0,0)]; // ATTRTILE_VERTEX
		else 
			p=new char[GET_POINTSIZE(0,nPoint,nTexel,0)]; // ATTRTILE_TEXEL
	}
};

struct sTile : public cMaterial, public sObjTri
{
	sAttribute				Attribute;
	short					nPolygon;
	float					size;
	int						ID;
	sTile()													{ nPolygon=0; size=0; ID=0; }
	~sTile()												{ }
	inline sPolygon* GetPolygon()							{ return GET_POLYGON(p,nPolygon,nPoint,nTexel); }
	inline sPoint* GetPoint()								{ return GET_POINT(p,nPolygon,nPoint,nTexel); }
	inline sTexel* GetTexel()								{ return GET_TEXEL(p,nPolygon,nPoint,nTexel); }
	inline sPolygon* GetTexPoly()							{ return GET_TEXPOLY(p,nPolygon,nPoint,nTexel); }
	inline short& GetNumberPolygon()						{ return nPolygon; }
	inline short& GetNumberPoint()							{ return nPoint; }
	inline short& GetNumberTexel()							{ return nTexel; }
	inline short GetNumberTexPoly()							{ if(Attribute.GetAttribute(ATTRTILE_TEXEL)) return GetNumberPolygon(); return 0; }
	inline cString& GetName()								{ return name; }
	__forceinline int& GetID()								{ return ID; }
	void CalcNormal();
	void CalcLOD();
	void GetCopy(sTile *Tile);
	inline cMaterial* GetMaterial()							{ return (cMaterial*)this; }
	inline void NewTri(int NumberPolygon,int NumberPoint,int NumberTexel,int NumberTexPoly)
	{
		nTexel=NumberTexel;
		nPoint=NumberPoint;
		nPolygon=NumberPolygon;
		if(p) { delete p; p=0; Attribute.ClearAttribute(ATTRTILE_VERTEX|ATTRTILE_VERTEX_TEXEL|ATTRTILE_TEXEL); }
		if(NumberTexel==0)
		{
			Attribute.SetAttribute(ATTRTILE_VERTEX);
			p=new char[GET_POINTSIZE(nPolygon,nPoint,0,0)];
		}
		else if((NumberTexPoly==0)&&(NumberTexel==NumberPoint))
		{
			Attribute.SetAttribute(ATTRTILE_VERTEX_TEXEL);
			p=new char[GET_POINTSIZE(nPolygon,nPoint,nTexel,0)];
		}
		else 
		{
			if(NumberTexPoly<0) NumberTexPoly=0;
			Attribute.SetAttribute(ATTRTILE_TEXEL);
			p=new char[GET_POINTSIZE(nPolygon,nPoint,nTexel,NumberTexPoly)];
		}
	}
};

class cTile : public cBaseDynArrayPointer<sTile>
{
public:
	cTile()												{ }
	~cTile()											{ }

	sTile* AddTile(sTile *tile);
	inline int GetNumberTile()							{ return cBaseDynArrayPointer<sTile>::length(); }
	inline sTile* GetTile(int number)					{ return cBaseDynArrayPointer<sTile>::Base[number]; }
	inline void TranslateTri(const Vect3f &dPos)
	{ 
		for(int nTile=0;nTile<GetNumberTile();nTile++)
		{
			sPoint *Point=GetTile(nTile)->GetPoint();
			for(int i=0;i<GetTile(nTile)->GetNumberPoint();i++) 
				Point[i].pos+=dPos;  
		}
	}
	inline void ScaleTri(const Vect3f &scale)				
	{ 
		float MinScale=scale.x;
		if(MinScale>scale.y) MinScale=scale.y;
		if(MinScale>scale.z) MinScale=scale.z;
		for(int nTile=0;nTile<GetNumberTile();nTile++)
		{
			GetTile(nTile)->size*=MinScale;
			sPoint *Point=GetTile(nTile)->GetPoint();
			for(int i=0;i<GetTile(nTile)->GetNumberPoint();i++) 
				Point[i].pos*=scale; 
		}
	}
	void CreateCopy(cTile* &Tile)
	{
		if(Tile) delete Tile;
		Tile=new cTile;
		Tile->New(GetNumberTile());
		for(int nTile=0;nTile<GetNumberTile();nTile++)
		{
			Tile->Base[nTile]=new sTile;
			GetTile(nTile)->GetCopy(Tile->Base[nTile]);
		}
	}
	inline void Swap(int n1,int n2)
	{
		sTile *tmp=cBaseDynArrayPointer<sTile>::Base[n1];
		cBaseDynArrayPointer<sTile>::Base[n1]=cBaseDynArrayPointer<sTile>::Base[n2];
		cBaseDynArrayPointer<sTile>::Base[n2]=tmp;
	}
	__forceinline sTile* GetTileByName(char *name)
	{
		for(int i=0;i<GetNumberTile();i++)
			if(GetTile(i)->GetName()==name)
				return GetTile(i);
		return 0;
	}
	__forceinline sTile* GetTileByID(int id)
	{
		for(int i=0;i<GetNumberTile();i++)
			if(GetTile(i)->GetID()==id)
				return GetTile(i);
		return 0;
	}
protected:
	friend class cAnimChain;
};

class cList
{
	friend class cMesh;
public:
	cMesh	*Mesh;
	cList	*next;
	cList	*prev;
	
	cList(cMesh *mesh=0)									{ Mesh=mesh; next=prev=0; }
	~cList()												{ Detach(); }
	cList* Attach(cList *Link);
	cList* Detach();
	cMesh* FindMesh(char *Name);
	cMesh* FindMesh(unsigned int type);
};

struct sStaticMesh : public sPointerCount
{
	cString			name,fname,TexturePath;
	void Release();
	void GetCopy(sStaticMesh* &StaticMesh);
};

class cMesh : public cUnknownClass, public cBound
{
public:
	unsigned int	Type,ID,Unique;
	Vect3f			Scale;				// ������ �������� �������
	sColor4f		Diffuse;			// ����������������� ������������ �����
	sColor4f		Specular;			// ���������� ������������ �����
	int				Attribute;			// �������� 
	cList			*Child;
	cMesh			*Parent;
	cFrame			*Frame;
	cTile			*Tile;
	cMatrix			LocalMatrix;
	cMatrix			GlobalMatrix;
	DummyList		Dummies;
	char			TotalVisibleCamera[NUMBER_CAMERA_MAX];	// ��������� �� ������ TotalBoundingBox (������ � Childs)
	char			*description;
	sStaticMesh		*StaticMesh;
	cMesh			*Bound;
#ifdef _SURMAP_
	int				NumberTrack;
#endif

#ifdef _MECHOSOMA_
	int SoundID;
	mchArcaneData arcaneData;
#endif

	cMesh();
	~cMesh();

	void AttachChild(cMesh *child);
	Vect3f CalcDummyOffset(cMesh* parent);
	void attachDummyChildren(const int Kinds[]);
	void DetachChild();
	void SetParent(cMesh *parent)							{ Parent=parent; }
	cMesh* GetGeneralParent()						{ cMesh *tmp=this; while(tmp->Parent) tmp=tmp->Parent; return tmp; }
	// ������� ��� ������ � ����������
	void SetVisible(int visible);
	inline int GetVisible(int visible)						{ for(int i=0;i<NUMBER_CAMERA_MAX;i++) visible&=GetVisibleTotal(i); return visible; }
	inline int isVisibleTotal(cUnknownClass *UCameraList)	{ int test=0; for(int i=0;i<((cUnkClassDynArrayPointer*)UCameraList)->length();i++) test|=GetVisibleTotal(i); return test; }
	inline char& GetVisibleTotal(int number)				{ assert(number<NUMBER_CAMERA_MAX); return TotalVisibleCamera[number]; }
	void TestVisible(cUnknownClass *UCameraList);	// �������� ���� �� ���������
	// ������� ��� ������ ���������
	void Animate(float dt);
	cFrame* FindFrame(); // finds first frame in hierarchy
	void SetFirstAnimation(float animation_period = 0, float start_phase = -1, float finish_phase = -1); // for first frame only
	void SetAnimation(float animation_period = 0, float start_phase = -1, float finish_phase = -1,int mask=0xFFFFFFFF,int count=0); // all the Meshes
	void GetAnimation(float &animation_period,float &start_phase,float &finish); // all the Meshes
	void GetAnimation(float &animation_period,float &start_phase); // all the Meshes
	void SetFrame(char *name,char *parent,float xpivot,float ypivot,float zpivot,float xofs,float yofs,float zofs,
				  float *PosKXYZ,int nPos,float *RotKWXYZ,int nRot,float *ScaleKXYZ,int nScale);
	void SetFrame();
	// ������ � �������� �������
	void dSetScale(float dScale)						{ dSetScale(Vect3f(dScale,dScale,dScale)); }
	void dSetScale(const Vect3f &dScale)				{ Vect3f ds=dScale; if(ds.x<=0) ds.x=1e-10f; if(ds.y<=0) ds.y=1e-10f; if(ds.z<=0) ds.z=1e-10f; ScaleMesh(ds); }
	void SetScale(float Scale)							{ SetScale(Vect3f(Scale,Scale,Scale)); }
	void SetScale(const Vect3f &scale)					{ Vect3f s=scale; if(s.x<=0) s.x=1e-10f; if(s.y<=0) s.y=1e-10f; if(s.z<=0) s.z=1e-10f; ScaleMesh(s/Scale); }
	void SetPosition(const class MatXf &Matrix)			{ LocalMatrix=Matrix; CalcMatrix(); }
	void SetPosition(const class Mat3f& matrix3x3, const class Vect3f& vector3) { SetPosition(MatXf(matrix3x3, vector3)); }
	void SetOrientation(const class Mat3f& matrix3x3)	{ LocalMatrix.rot()=matrix3x3; CalcMatrix(); }
	void SetPosition(const class Vect3f& vector3)		{ LocalMatrix.trans()=vector3; CalcMatrix(); }
	inline Vect3f& GetScale()							{ return Scale; }
	// ������ � ����������� �������
	void SetColor(float r,float g,float b,float a=1.f,unsigned int type=0xFFFFFFFF)	{ if(Type&type) Diffuse.set(r,g,b,a); for(cList *tmp=Child;tmp;tmp=tmp->next) tmp->Mesh->SetColor(r,g,b,a,type); }
	void SetColor(const sColor4f &diffuse,const sColor4f &specular=sColor4f(0,0,0,1),unsigned int type=0xFFFFFFFF) { if(Type&type) { Diffuse=diffuse; Specular=specular; } for(cList *tmp=Child;tmp;tmp=tmp->next) tmp->Mesh->SetColor(diffuse,specular,type); }
	sColor4f& GetDiffuse()								{ return Diffuse; }
	sColor4f& GetSpecular()								{ return Specular; }
	// ������� ������
	void Draw(cUnknownClass *UScene,cUnknownClass *UCameraList);
	void DrawReflection(cUnknownClass *UScene,cUnknownClass *UCameraList);
	void BuildShadeDynamic(short **shade,int *xShade,int *yShade,Vect3f &PosShade,Vect3f &PosLight);
	void BuildShadeStatic(short **shade,int *xShade,int *yShade,Vect3f &PosShade,Vect3f &PosLight,float Radius);
	void ShadeDynamic(cUnknownClass *UTileMap);
	sTile* AddTile(sTile *Tile)	
	{ 
		if(cMesh::Tile==0) 
		{ 
			cMesh::Tile=new cTile; 
			SetAttribute(MESH_CREATETILE); 
		} 
		cMesh::Tile->AddTile(Tile); 
		if(Tile->GetAttribute(ATTRMAT_TRANSPARENCY|ATTRMAT_OPACITY)) 
			GetGeneralParent()->SetAttribute(MESH_USE_OPACITY); 
		return Tile; 
	}
	cMesh* FindMesh(char *Name);
	cMesh* FindMesh(unsigned int type);
	cMesh* FindUnique(unsigned int unique);
	cMesh* operator[] (int number);
	int NumMesh();
	void CreateCopy(unsigned int type,cMesh *Mesh,cMesh *MeshParent=0);
	void CreateCopyMem(unsigned int type,cMesh *Mesh,cMesh *MeshParent=0);
	void CreateCopyMesh(unsigned int type,cMesh *Mesh,cMesh *MeshParent=0);
	void Alignment();
	char TypeMesh()									{ return (Type&0xFFFF0000)!=0; }
	void GetGlobalGeometry(vector<Vect3f>& verts, vector<int>& faces, int recursive); // vectors must be cleared
	void GetGlobalDummy(char* name_mask, vector<Vect3f>& dummies, int recursive = 1); // vectors must be cleared
	cList* AddChild(cMesh *child);
	void Displace(float x,float y,float z);
	void ReCalcMatrix()										{ GetGeneralParent()->CalcMatrix(); }
	void ReCalcTotalBound()									{ cMesh *tmp=GetGeneralParent(); tmp->CalcTotalBound(); tmp->Attribute^=MESH_HIERARCHY; }
	void CalcMatrix();
	void CalcTotalBound();
	void SetAttribute(int attribute)						{ Attribute|=attribute; for(cList *tmp=Child;tmp;tmp=tmp->next) tmp->Mesh->SetAttribute(attribute); }
	void ClearAttribute(int attribute)						{ Attribute&=~attribute; for(cList *tmp=Child;tmp;tmp=tmp->next) tmp->Mesh->ClearAttribute(attribute); }
	int GetAttribute(int attribute)							{ return Attribute&attribute; }
	void GetTotalBound(int types,cMesh *center,sBox6f &box);
	void SetWireSize(const Vect3f &size);
	void SetDescription(char *string);
	inline char* GetDescription()							{ if(description) return description; return ""; }

	template <class Operation>
	void ScanHierarchy(Operation& op) // operator()(cMesh&) ������ ���������� 1 ��� ����������� ������������
	{							// �������� ��������� ��������!!!  
		cList *startList;
		for(startList=Child; startList; startList=startList->next)
			if(!op(*startList->Mesh))
				return;
		for(startList=Child; startList; startList=startList->next)
			startList->Mesh->ScanHierarchy(op);
	}


#ifdef _MECHOSOMA_
	void SetArcane(void (*FunctionSetArcane)(mchArcaneData*));
#endif
	void Release();
#ifdef _DEBUG
	int AssertValid();
#else
	inline int AssertValid()								{ return 1; }
#endif

	float& x()												{ return LocalMatrix.trans()[0]; }
	float& y()												{ return LocalMatrix.trans()[1]; }
	float& z()												{ return LocalMatrix.trans()[2]; }
	inline const float& x() const 							{ return LocalMatrix.trans()[0]; }
	inline const float& y() const 							{ return LocalMatrix.trans()[1]; }
	inline const float& z() const 							{ return LocalMatrix.trans()[2]; }
	inline cString& GetName()								{ assert(StaticMesh); return StaticMesh->name; }
	inline cString& GetFileName()							{ assert(StaticMesh); return StaticMesh->fname; }
	inline cString& GetTexturePath()						{ assert(StaticMesh); return StaticMesh->TexturePath; }
	inline void SetName(char *str)							{ if(StaticMesh==0) StaticMesh=new sStaticMesh; StaticMesh->name=str; }
	inline void SetFileName(char *str)						{ if(StaticMesh==0) StaticMesh=new sStaticMesh; StaticMesh->fname=str; }
	inline void SetTexturePath(char *str)					{ if(StaticMesh==0) StaticMesh=new sStaticMesh; StaticMesh->TexturePath=str; }
	inline sTile* GetTileByName(char *name)					{ assert(Tile); return Tile->GetTileByName(name); }
	inline sTile* GetTile(int number)						{ assert(Tile); return Tile->GetTile(number); }
	inline int GetNumberTile()								{ assert(Tile); return Tile->GetNumberTile(); }

protected:
	void DelChild(cMesh *child);
	void Init(char *NameMesh=0,char *NameParent=0);
	void ScaleMesh(Vect3f &dScale);
	void WireScale(const Vect3f &scale);
	void GetSize(sBox6f &box);
	void GetTotalBound(cMesh *center,sBox6f &box);
	cMesh* GetNumberMesh(int &number);
	__forceinline MatXf& BuildDrawMatrix(cCamera *Camera,MatXf &DrawMatrix)		
	{
		DrawMatrix=GlobalMatrix;
		WorldToCameraCutting(DrawMatrix,Camera);
		WorldShareMapping(DrawMatrix,Camera);
		return DrawMatrix;
	}
	__forceinline MatXf& BuildDrawReflectionMatrix(cCamera *Camera,MatXf &DrawMatrix,Vect3f &vReflection)
	{
		DrawMatrix=GlobalMatrix;
		WorldToCameraCutting(DrawMatrix,Camera);
		cMatrix MatrixReflection(Mat3f(1,0,0, 0,1,0, 0,0,-1),Vect3f(0,0,2*vReflection.z));
		DrawMatrix=MatrixReflection*DrawMatrix;
		WorldShareMapping(DrawMatrix,Camera);
		DrawMatrix.trans().z-=1.0f;
		return DrawMatrix;
	}

	void ConvertPointShadeDynamic(sBox6f &box,cMatrix &LightMatrix);
	void ConvertPointShadeDynamic(const sBox6f &box,float dzx);
	void ConvertPointShadeStatic(sBox6f &box,cMatrix &LightMatrix,Vect3f &PosLight,float zProjection);
	void ConvertPointShadeStatic(const sBox6f &box);
	void DrawShadeStatic(short *shade,int xShade,int yShade,int &nPointBegin);
	void DrawShadeDynamic(short *shade,int xShade,int yShade,int &nPointBegin);

	friend class cPolyDispatcher;
};

#endif // __OBJECT3D_H__
