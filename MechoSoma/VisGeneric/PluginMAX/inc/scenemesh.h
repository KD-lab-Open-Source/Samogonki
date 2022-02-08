#ifndef __SCENEMESH_H__
#define __SCENEMESH_H__

#include "meshfile.h"

enum eNodeObjectType
{
	NODEOBJECT_NULL				=	0,
	NODEOBJECT_MESH				=	1,
	NODEOBJECT_CAMERA			=	2,
	NODEOBJECT_LIGHT			=	3,
	NODEOBJECT_SHAPE			=	4,
	NODEOBJECT_HELPER			=	5,
};

enum eTextureMapType
{ /* texture map in 3dSMAX */
	TEXMAP_AM					=	0,   // ambient
	TEXMAP_DI					=	1,   // diffuse
	TEXMAP_SP					=	2,   // specular
	TEXMAP_SH					=	3,   // shininess
	TEXMAP_SS					=	4,   // shininess strength
	TEXMAP_SI					=	5,   // self-illumination
	TEXMAP_OP					=	6,   // opacity
	TEXMAP_FI					=	7,   // filter color
	TEXMAP_BU					=	8,   // bump 
	TEXMAP_RL					=	9,   // reflection
	TEXMAP_RR					=	10,  // refraction 
};

#define HELPERNAME_DUMMY			"Dummy"
#define HELPERNAME_GROUP			"Group"

typedef cBaseFileMeshArray <sFloat4,MF_ID_BLOCK_ANIMATIONPOSITION,MF_TYPE_KEYPOSITION> sAnimationPosition;
typedef cBaseFileMeshArray <sFloat5,MF_ID_BLOCK_ANIMATIONROTATION,MF_TYPE_KEYROTATION> sAnimationRotation;
typedef cBaseFileMeshArray <sFloat4,MF_ID_BLOCK_ANIMATIONSCALE,MF_TYPE_KEYSCALE> sAnimationScale;
typedef cBaseFileMeshArray <sFloat2,MF_ID_BLOCK_ANIMATIONVISIBILITY,MF_TYPE_KEYVISIBILITY> sAnimationVisibility;

typedef cBaseFileMeshArray <sFloat3,MF_ID_BLOCK_VERTEXMESH,MF_TYPE_VERTEXMESH> sVertexMesh;
typedef cBaseFileMeshArray <sInt4,MF_ID_BLOCK_FACEMESH,MF_TYPE_FACEMESH> sFaceMesh;
typedef cBaseFileMeshArray <sFloat3,MF_ID_BLOCK_VERTEXNORMALMESH,MF_TYPE_VERTEXMESH> sVertexNormalMesh;
typedef cBaseFileMeshArray <sFloat3,MF_ID_BLOCK_FACENORMALMESH,MF_TYPE_VERTEXMESH> sFaceNormalMesh;

typedef cBaseFileMeshArray <sFloat3,MF_ID_BLOCK_TEXVERTEXMESH,MF_TYPE_TEXVERTEXMESH> sTexVertexMesh;
typedef cBaseFileMeshArray <sInt3,MF_ID_BLOCK_TEXFACEMESH,MF_TYPE_TEXFACEMESH> sTexFaceMesh;

struct sChannelMapping
{
	unsigned int		ID;
	unsigned int		ChannelNumber;			// 0-ColorVertex, 1-TexelVertex, 2..-Add TexelVertex
	sTexVertexMesh		TexVertex;				// массив текстурных uvw-координат вершин
	sTexFaceMesh		TexFace;				// массив текстурных вершин полигонов

	sChannelMapping()						{ ID=0; }
	~sChannelMapping()						{ Release(); }
	void Release()							{ ID=0; TexVertex.Release(); TexFace.Release(); }
	inline int cmp(void *Key)				{ return (*(unsigned int*)Key)!=ID; }
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};
typedef cBaseMeshPointerLibrary <sChannelMapping> sChannelMappingLibrary;

struct sAnimationMesh
{
	unsigned int			ID;
	int						time;			// время
	sVertexMesh				Vertex;			// массив xyz-координат вершин
	sFaceMesh				Face;			// массив вершин полигонов
	sVertexNormalMesh		VertexNormal;	// нормали для каждой вершиины полигона
	sFaceNormalMesh			FaceNormal;		// нормали для каждого полигона
	sChannelMappingLibrary	ChannelMappingLibrary;
	float					Box[6];		// характерный размер

	sAnimationMesh()						{ ID=0; time=0; }
	~sAnimationMesh()						{ Release(); }
	void Release()							{ ID=0; time=0; Vertex.Release(); Face.Release(); VertexNormal.Release(); FaceNormal.Release(); ChannelMappingLibrary.Release(); }
	inline int cmp(void *Key)				{ return (*(int*)Key)!=time; }
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};
typedef cBaseMeshPointerLibrary <sAnimationMesh> cAnimationMeshLibrary;

struct sNodeObject
{
	unsigned int			ID;
	unsigned int			type;
	cString					name;
	cString					parent;
	sAnimationVisibility	AnimationVisibility;
	sAnimationPosition		AnimationPosition;
	sAnimationRotation		AnimationRotation;
	sAnimationScale			AnimationScale;
	float					matrix[12];

	sNodeObject(int Type=NODEOBJECT_NULL)	{ ID=0; type=Type; memset(matrix,0,12*sizeof(float)); }
	virtual ~sNodeObject()					{ Release(); }
	void Release();
	inline int cmp(void *Key)				{ return stricmp((char*)Key,name); }
	virtual int Write(cMeshFile &f)			{ return 1; }
};
typedef cBaseMeshPointerLibrary <sNodeObject> cNodeObjectLibrary;

struct sObjectMesh : public sNodeObject
{
	cAnimationMeshLibrary	AnimationMeshLibrary;

	sObjectMesh() : sNodeObject(NODEOBJECT_MESH)	{ }
	~sObjectMesh()									{ Release(); }
	void Release()									{ AnimationMeshLibrary.Release(); }
	int Read(cMeshFile &f);
	virtual int Write(cMeshFile &f);
};

struct sHelperObject : public sNodeObject
{
	cString					HelperName;

	sHelperObject() : sNodeObject(NODEOBJECT_HELPER){ }
	~sHelperObject()								{ Release(); }
	void Release()									{ HelperName.Release(); }
	int Read(cMeshFile &f);
	virtual int Write(cMeshFile &f);
	inline int isGroup()							{ return stricmp(HelperName,HELPERNAME_GROUP)==0; }
	inline int isDummy()							{ return stricmp(HelperName,HELPERNAME_DUMMY)==0; }
};

struct sSubTexmap
{
	unsigned int		ID;							// идентификатор текстуры 
	cString				name;						// имя текстуры 
	int					ChannelNumber;				// номер канала текстурной генерации
	sSubTexmap()									{ memset(this,0,sizeof(sSubTexmap)); }
	~sSubTexmap()									{ Release(); }
	void Release()						
	{ 
		name.Release(); 
		memset(this,0,sizeof(sSubTexmap));
	}
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
	inline int cmp(void *Key)				{ return stricmp((char*)Key,name); }
};
typedef cBaseMeshPointerLibrary <sSubTexmap> cSubTexmapArray;

struct sAnimationMaterial
{
	unsigned int			ID;
	int						time;				// время
	float					AmbientColor[3];
	float					DiffuseColor[3];
	float					SpecularColor[3];
	float					Shininess;
	float					ShinStrength;
	float					Transparency;
	cSubTexmapArray			SubTexmap;			// список SubTexmap

	sAnimationMaterial()						{ memset(this,0,sizeof(sAnimationMaterial)); }
	~sAnimationMaterial()						{ Release(); }
	void Release()								{ SubTexmap.Release(); memset(this,0,sizeof(sAnimationMaterial)); }
	inline int cmp(void *Key)					{ return (*(int*)Key)!=time; }
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};
typedef cBaseMeshPointerLibrary <sAnimationMaterial> cAnimationMaterialLibrary;

struct sMaterialObject
{
public:
	unsigned int			ID;					// порядковый номер в библиотеке
	cString					name;				// имя материала
	cString					parent;				// родитель, тот который содержит этот материал (MultiSubObjectMaterial)
	int						NumberSubObject;	// число материалов в данном, которые будут ссылаться через parent на данный
	int						Shading;
	cAnimationMaterialLibrary	AnimationMaterialLibrary;
	
	sMaterialObject()						{ ID=NumberSubObject=Shading=0; name=parent=0; }
	~sMaterialObject()						{ Release(); }
	void Release()							{ ID=NumberSubObject=Shading=0; name.Release(); parent.Release(); AnimationMaterialLibrary.Release(); }
	inline int cmp(void *Key)				{ return stricmp((char*)Key,name); }
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};
typedef cBaseMeshPointerLibrary <sMaterialObject> cMaterialObjectLibrary;

struct sLodObject
{
	unsigned int				ID;		
	unsigned int				LevelDetail;	//level of detail
	cNodeObjectLibrary			NodeObjectLibrary;
	cMaterialObjectLibrary		MaterialLibrary;
	
	sLodObject()							{ ID=0; LevelDetail=0; }
	~sLodObject()							{ Release(); }
	void Release()							{ LevelDetail=0; 
		NodeObjectLibrary.Release(); 
		MaterialLibrary.Release(); 
	}
	inline int cmp(void *Key)				{ return (*(unsigned int*)Key)!=LevelDetail; }

	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};
typedef cBaseMeshPointerLibrary <sLodObject> cLodObjectLibrary;

struct sChannelAnimation
{
	unsigned int				ID;
	cString						name;
	int							FirstFrame;
	int							LastFrame;
	int							FrameSpeed;
	int							TicksPerFrame;
	int							NumberFrame;	// временная переменная только для *.m3d с чтением морфинга
	cLodObjectLibrary			LodLibrary;

	sChannelAnimation()						{ ID=1; name=(char*)0; TicksPerFrame=FrameSpeed=LastFrame=FirstFrame=NumberFrame=0; }
	~sChannelAnimation()					{ Release(); }
	void Release()							{ name.Release(); LodLibrary.Release(); }
	inline int cmp(void *Key)				{ return stricmp((char*)Key,name); }
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
	int GetNumberMeshAnimation();
};
typedef cBaseMeshPointerLibrary <sChannelAnimation> cChannelAnimationLibrary;

class cMeshScene
{
public:
	unsigned int				ID;
	cString						name;
	cChannelAnimationLibrary	ChannelLibrary;
	
	cMeshScene()							{ name=(char*)0; ID=1; }
	~cMeshScene()							{ Release(); }
	void Release()							{ name.Release(); ChannelLibrary.Release(); }
	
	int Read(cMeshFile &f);
	int Write(cMeshFile &f);
};

#endif // __SCENEMESH_H__