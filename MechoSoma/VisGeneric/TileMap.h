#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "Unknown.h"

#include "Object.h"
#include "BaseClass.h"
#include "Object3d.h"
#include "PolyGrid.h"

#include "IncTerra.h"
#include "IGraph3d.h"

#define MAP_TILE_SIZE_BIT			7

#define MAP_LIGHTMAP_STEP_BIT		1
#define MAP_LIGHTMAP_STEP			(1<<MAP_LIGHTMAP_STEP_BIT)

enum eTileMapAttribute
{	// cTileMap::attribute 
	ATTR_TILEMAP_POLYGRID_MOVTEXTURE	=	1<<0,
	ATTR_TILEMAP_OVERCAST_MOVTEXTURE	=	1<<1,
	ATTR_TILEMAP_SKY_MOVTEXTURE			=	1<<2,
	ATTR_TILEMAP_WORLD_SUN				=	1<<3,
	ATTR_TILEMAP_WORLD_REFLECTION		=	1<<4,
	ATTR_TILEMAP_WIREFRAME				=	1<<5,
};
enum eTileAttribute
{	// sTileMap::attribute
	ATTR_TILE_NULL					=	0,
	ATTR_TILE_SURFACE_WATER			=	1<<0,
	ATTR_TILE_SURFACE_REFLECTION	=	1<<1,
	ATTR_TILE_REFLECTION			=	1<<2
};

struct sPointTile
{
	unsigned short	xw,yw,zw;
	char			dx,dy,dz;
	unsigned char	a;
	float			u,v;
	inline int da()						{ return (int)a; }
	inline int da(int alfa)				{ return a=(unsigned char)alfa; }
};
class cSun : public cMaterial
{
public:
	Vect3f Pos;
	sColor4f Ambient;
	float RadiusLight,RadiusSun;

	cMaterial* GetMaterial()								{ return (cMaterial*)this; }
	sTexture* GetTexture()									{ return this->Texture; }
	float GetRadiusLight()									{ return RadiusLight; }
	float GetRadiusSun()									{ return RadiusSun; }
	Vect3f& GetPos()										{ return Pos; }
	sColor4f& GetAmbient()									{ return Ambient; }
};
struct sTileMap : public sAttribute
{
	cMaterial					Material;
	cBaseDynArray <sPointTile>	Point;
	cBaseDynArray <sPolygonFix> Polygon;
	cBaseDynArray <char>		AttrPolygon;
	cBaseDynArray <sPointTile>	PointReflection;
	cBaseDynArray <sPolygonFix>	PolygonReflection;
	cBaseDynArray <char>		AttrPolygonReflection;
	int							zReflectionSurface;
	int							xPos,yPos,zwMin,zwMax;
	char						TotalVisibleCamera[NUMBER_CAMERA_MAX];	// видимость из камеры 
	MatXf						DrawMatrix[NUMBER_CAMERA_MAX];
	Vect3f						ShareOfs[NUMBER_CAMERA_MAX];

	sTileMap()												{ xPos=yPos=0; zReflectionSurface=0; memset(TotalVisibleCamera,CONST_VISIBLE_SHOW,NUMBER_CAMERA_MAX); }
	~sTileMap()												{ if(GetMaterial()->Opacity) GetMaterial()->Opacity->texture=0; } 
	inline int GetVisible()									{ int test=0; for(int i=0;i<NUMBER_CAMERA_MAX;i++) test|=GetVisibleTotal(i); return test&CONST_VISIBLE_FRUSTUM; }
	inline int isVisibleTotal(cUnknownClass *UCameraList)	{ int test=0; for(int i=0;i<((cUnkClassDynArrayPointer*)UCameraList)->length();i++) test|=GetVisibleTotal(i); return test; }
	inline char& GetVisibleTotal(int number)				{ assert(number<NUMBER_CAMERA_MAX); return TotalVisibleCamera[number]; }
	inline cMaterial* GetMaterial()							{ return &Material; }
	inline sTexture*& GetTexture()							{ return GetMaterial()->Texture; }
	inline sTexture*& GetLightMap()							{ return GetMaterial()->Opacity; }
	__forceinline MatXf& BuildDrawMatrix(cCamera *Camera,MatXf &DrawMatrix,Vect3f &TilePos)
	{
		DrawMatrix.trans().set(xPos,yPos,0);
		DrawMatrix.rot()=Mat3f::ID;
		WorldToCameraCutting(DrawMatrix,Camera);
		TilePos=DrawMatrix.trans();
		return DrawMatrix;
	}
};
class cTileMap : public cUnknownClass, public sAttribute
{
	sTileMap	*Tile;
	int			xTile,yTile;							// число тайлов по осям
	int			_xTile,_yTile;							// битовый размер тайла
	int			zReflectionSurface;
	// для итеррации по полигонам карты
	int			nTile,nPolygon;
public:
	cTileMap();
	~cTileMap();
	void Release();
	// вывод
	void TestVisible(cUnknownClass *UCameraList);
	void UnlockTexture(cUnknownClass *UCameraList);
	void Draw(cUnknownClass *UCameraList);
	void DrawReflection(cUnknownClass *UCameraList);
	// загрузка и т.д.
	void Load(const char *fname);
	inline int GetVisible(int x,int y)					{ int i=x>>_xTile, j=y>>_yTile; UNKNOWN_ASSERT(i>=0&&i<xTile&&j>=0&&j<yTile); return Tile[i+j*xTile].GetVisible(); }
	inline int NumberTileX()							{ return xTile; }
	inline int NumberTileY()							{ return yTile; }
	inline int _SizeTileX()								{ return _xTile; }
	inline int _SizeTileY()								{ return _yTile; }
	inline int& GetZReflectionSurface()					{ return zReflectionSurface; }
	inline sTileMap* GetTile(int i,int j)				{ assert(Tile); return &Tile[i+j*xTile]; }
	int SetPolygonFirst();
	int GetPolygonNext(Vect3f &v1,Vect3f &v2,Vect3f &v3,int &attribute);
private:
	int Load3ds(const char *fname);
};

// переменные для работы c cPolyMgr
extern int PosBufTileMap;						// место в карте цветов
extern unsigned short *GlobalColorBufferMap;	// глобальная переменная хранит указатель на текущую карту цветов мира vMap->ClBuf

extern int GlobalRenderTuning;
extern float GlobalDistFlexureProjectionWorld,GlobalPointViewReflection;
extern int GlobalDistFlexureProjectionWorld2;
extern Vect2f GlobalVectorCenterToView;
extern Vect3f GlobalPointView;

#endif //__TILEMAP_H__