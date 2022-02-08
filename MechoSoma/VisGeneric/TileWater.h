#ifndef __TILEWATER_H__
#define __TILEWATER_H__

#define TILEWATER_MOVTEXTURE							1

#include "Unknown.h"
#include "UMATH.H"

class cTileMap;
class cPolyGrid;

class cTileWater : public cUnknownClass
{
	cPolyGrid	**PolyGrid;
	int			xTile,yTile;							// число тайлов по осям
	int			_xTile,_yTile;							// битовый размер тайла
	int			Attribute;
	int			LevelWater;								// уровень воды
	
	Vect2f		OfsTex;									// смещение текстуры 
	Vect2f		dOfsTex;	
	Vect2f		ScaleTex;								// масштаб текстуры PolyGrid
	cTileMap	*Map;									// вода принадлежит карте мира
public:
	cTileWater();
	~cTileWater();
	void Release();

	void Load(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,char *TextureName,char *TexturePath,int attribute);
	void PreDraw(cUnknownClass *UScene);
	void Draw(cUnknownClass *UScene);
	void Attach(cTileMap *TileMap=0);

	inline int NumberTileX()							{ return xTile; }
	inline int NumberTileY()							{ return yTile; }
	inline int _SizeTileX()								{ return _xTile; }
	inline int _SizeTileY()								{ return _yTile; }
	inline int GetLevelWater()							{ return LevelWater; }
	inline int SetLevelWater(int level)					{ return LevelWater=level; }
	cPolyGrid*& GetPolyGrid(int i,int j)				{ assert(PolyGrid); return PolyGrid[j*xTile+i]; }
	cTileMap* GetTileMap()								{ return Map; }
	inline Vect2f& GetOfsTex()							{ return OfsTex; }
	inline Vect2f& GetdOfsTex()						{ return dOfsTex; }
	inline Vect2f& GetScaleTex()						{ return ScaleTex; }
	inline void SetAttribute(int attribute)				{ Attribute|=attribute; }
	inline void ClearAttribute(int attribute)			{ Attribute&=~attribute; }
	inline int GetAttribute(int attribute=0xFFFFFFFF)	{ return Attribute&attribute; }
};

#endif //__TILEWATER_H__