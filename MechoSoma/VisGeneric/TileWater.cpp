#include "TileWater.h"
#include "PolyMgr.h"
#include "PolyGrid.h"
#include "TexMgr.h"
#include "TileMap.h"
#include "BaseDefine.h"
#include "Scene.h"

cTileWater::cTileWater() : cUnknownClass(KIND_WORLD_TILEWATER)
{
	PolyGrid=0;
	Attribute=0;
	LevelWater=0;
	xTile=yTile=0;
	_xTile=_yTile=0;
	OfsTex.set(0,0);
	dOfsTex.set(0,0);	
	ScaleTex.set(0,0);
	Attach();
}
cTileWater::~cTileWater()
{
	UNKNOWN_DESTRUCTOR;
	Release();
}
void cTileWater::Release()
{
	if(PolyGrid)	
	{
		for(int k=0;k<NumberTileX()*NumberTileY();k++)
			if(PolyGrid[k])
				delete PolyGrid[k];
		delete PolyGrid;
		PolyGrid=0;
	}
	Attach();
}
void cTileWater::Attach(cTileMap *TileMap)
{
	Map=TileMap;
	if(!Map) return;
	_xTile=Map->_SizeTileX(); _yTile=Map->_SizeTileY();
	PolyGrid=new cPolyGrid*[(xTile=Map->NumberTileX())*(yTile=Map->NumberTileY())];
}
void cTileWater::Load(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,const char *TextureName,const char *TexturePath,int attribute)
{
	assert(Map);
	assert(PolyGrid);
	if(attribute) SetAttribute(TILEWATER_MOVTEXTURE);
	assert((((1<<_xTile)%xstep)==0)&&(((1<<_yTile)%ystep)==0));
	int xsize=(1<<_xTile)/xstep, ysize=(1<<_yTile)/ystep;
	ScaleTex.set(uScale,vScale);
	int j;
	for(j=0;j<NumberTileY();j++)
		for(int i=0;i<NumberTileX();i++)
		{
			sTileMap *Tile=Map->GetTile(i,j);
			if(Tile->GetAttribute(ATTR_TILE_SURFACE_WATER))
			{
				cPolyGrid *water=GetPolyGrid(i,j)=new cPolyGrid;
				water->LocalMatrix.trans().set(Tile->xPos,Tile->yPos,GetLevelWater());
			}
			else GetPolyGrid(i,j)=0;
		}
	for(j=0;j<NumberTileY();j++)
		for(int i=0;i<NumberTileX();i++)
			if(GetPolyGrid(i,j))
			{
				cPolyGrid *PolyGrid=GetPolyGrid(i,j);
				PolyGrid->usize=1.f/uScale;
				PolyGrid->vsize=1.f/vScale;
				PolyGrid->uofs=fmod(i,uScale)/uScale;
				PolyGrid->vofs=fmod(j,vScale)/vScale;
				PolyGrid->New(xsize+1,ysize+1,xstep,ystep);
				int iL=(xTile+i-1)%xTile, iR=(xTile+i+1)%xTile;
				int jU=((yTile+j-1)%yTile)*xTile, jD=((yTile+j+1)%yTile)*xTile;
				PolyGrid->pgL=this->PolyGrid[j*xTile+iL];
				PolyGrid->pgR=this->PolyGrid[j*xTile+iR];
				PolyGrid->pgU=this->PolyGrid[jU+i];
				PolyGrid->pgD=this->PolyGrid[jD+i];
				PolyGrid->pgLU=this->PolyGrid[jU+iL];
				PolyGrid->pgRU=this->PolyGrid[jU+iR];
				PolyGrid->pgLD=this->PolyGrid[jD+iL];
				PolyGrid->pgRD=this->PolyGrid[jD+iR];
				if(TextureName) { PolyGrid->SetAttribute(ATTRMAT_TEXTURE_PAL); PolyGrid->Texture=TextureBuffer->Get(TextureName,TexturePath); }
				else PolyGrid->SetAttribute(ATTRMAT_COLOR);
				for(int k=0;k<PolyGrid->ysize;k++)
					for(int i=0;i<PolyGrid->xsize;i++)
					{
						int xg=round(PolyGrid->x()+i*PolyGrid->xstep);
						int yg=round(PolyGrid->y()+k*PolyGrid->ystep);
						if(vMap_GetAt(xg,yg)&TERRA_At_WATER)
						{
							PolyGrid->Point[i+k*PolyGrid->xsize].attribute=POLYGRID_USE;
							PolyGrid->Point[i+k*PolyGrid->xsize].r=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].g=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].b=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].a=200;
						}
						else
						{
							PolyGrid->Point[i+k*PolyGrid->xsize].attribute=POLYGRID_NOT_USE;
							PolyGrid->Point[i+k*PolyGrid->xsize].r=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].g=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].b=100;
							PolyGrid->Point[i+k*PolyGrid->xsize].a=0;
						}
					}
			}
}
void cTileWater::PreDraw(cUnknownClass *UScene)
{
	assert(P3D);
	assert(UScene->GetKind(KIND_SCENE));
	cScene *Scene=(cScene*)UScene;
	cLight *LightObject=Scene->GetLight();
	cUnknownClass *UCameraList=Scene->GetCameraList();
	if(!GET_RENDER_TUNING(RENDER_TUNING_WATER)) return;
	sTileMap *bTile=GetTileMap()->GetTile(0,0);
	cPolyGrid **bWater=&GetPolyGrid(0,0),**eWater=&GetPolyGrid(0,NumberTileY());
	for(;bWater<eWater;bWater++,bTile++)
		if(((*bWater)!=0)&&(bTile->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM))
			(*bWater)->PreDraw(LightObject);
}
void cTileWater::Draw(cUnknownClass *UCameraList)
{
	assert(P3D&&Map);
	if(!GET_RENDER_TUNING(RENDER_TUNING_WATER)) return;
	P3D->Draw(UCameraList,this);
/*
	sVect2f dTex(fmod(GetOfsTex().x+GetdOfsTex().x,1.f),fmod(GetOfsTex().y+GetdOfsTex().y,1.f));
	GetOfsTex()=dTex;
//	if(GetAttribute(TILEWATER_MOVTEXTURE)) { dTex.x-=Camera->GetPos().x/(GetScaleTex().x*(1<<_SizeTileX())); dTex.y-=Camera->GetPos().y/(GetScaleTex().y*(1<<_SizeTileY())); }
//	dTex.x=dTex.x-(int)dTex.x; dTex.y=dTex.y-(int)dTex.y;
	sTileMap *bTile=GetTileMap()->GetTile(0,0);
	cPolyGrid **bWater=&GetPolyGrid(0,0),**eWater=&GetPolyGrid(0,NumberTileY());
	for(;bWater<eWater;bWater++,bTile++)
		if(((*bWater)!=0)&&(bTile->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM))
		{
//			(*bWater)->du=dTex.x; (*bWater)->dv=dTex.y;
			P3D->Draw(UCameraList,*bWater);
		}
*/
}
