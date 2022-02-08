#include "PolyMgr.h"
#include "Material.h"
#include "BaseDefine.h"
#include "RenderDevice.h"
#include "PolyGrid.h"
#include "BasePolyGrid.h"
#include "BaseReflection.h"

#ifdef _USE_TILEMAP_
#include "TileMap.h"
#endif //_USE_TILEMAP_

inline void SetCreateTexture(cInterfaceGraph3d *Graph3d,sTexture *Texture,int xs,int ys,eTextureFormat format)
{
	assert(Texture->nTexture==0);
	Texture->nTexture=Graph3d->CreateTexture(xs,ys,format);
	assert(Texture->nTexture);
	Texture->SetStatus(STATUS_TEXTURE_CREATE);
	Texture->ClearStatus(STATUS_TEXTURE_LOAD);
}
inline void SetDeleteTexture(cInterfaceGraph3d *Graph3d,sTexture *Texture)
{
	assert(Texture->GetStatus(STATUS_TEXTURE_CREATE)&&Texture->nTexture);
	Texture->ClearStatus(STATUS_TEXTURE_CREATE|STATUS_TEXTURE_CACHE);
	Graph3d->DeleteTexture(Texture->nTexture);
	Texture->nTexture=0;
}
inline void SetLockTexture(cInterfaceGraph3d *Graph3d,sTexture *Texture,void **bTexture)
{
	assert(Texture->GetStatus(STATUS_TEXTURE_CREATE)&&Texture->nTexture);
	int bpl=0;
	Graph3d->LockTexture(Texture->nTexture,bTexture,&bpl);
	Texture->bpl=bpl;
	assert(bTexture);
	Texture->SetStatus(STATUS_TEXTURE_LOCK);
}
inline void SetUnlockTexture(cInterfaceGraph3d *Graph3d,sTexture *Texture)
{
	assert(Texture->GetStatus(STATUS_TEXTURE_CREATE)&&Texture->GetStatus(STATUS_TEXTURE_LOCK)&&Texture->nTexture);
	Texture->ClearStatus(STATUS_TEXTURE_CACHE|STATUS_TEXTURE_LOCK|STATUS_TEXTURE_LOAD);
	Graph3d->UnlockTexture(Texture->nTexture);
}

void cPolyDispatcher::CreateTexture8888(cMaterial *Material,cRenderDevice *RenderDevice)
{
	sTexture *Texture=Material->Texture,*Opacity=Material->Opacity;
	assert(Opacity);
	assert(Opacity->texture);
	assert(Opacity->nTexture==0);
	assert(Opacity->_x==Texture->_x);
	assert(Opacity->_y==Texture->_y);
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	SetCreateTexture(IGraph3d,Texture,1<<Texture->_x,1<<Texture->_y,TEXFMT_ARGB8888);
	unsigned char *bTexture=0;
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt8888().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);
	unsigned char *buf_texture=Texture->texture,*buf_opacity=Opacity->texture,*buf_rgb=Texture->GetPal();
	for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
	{
		unsigned int *buf=(unsigned int*)bTexture;
		for(int i=0;i<(1<<Texture->_x);i++,buf_texture++,buf_opacity++)
		{
			unsigned char *rgb=&buf_rgb[3*buf_texture[0]];
			int r=rgb[0]<<(8-PALETTE_MAX_SIZE),g=rgb[1]<<(8-PALETTE_MAX_SIZE),b=rgb[2]<<(8-PALETTE_MAX_SIZE);
			if(r>255) r=255; if(g>255) g=255; if(b>255) b=255;
			buf[i]=((buf_opacity[0]>>(8-aCount))<<aShift)|
				((r>>(8-rCount))<<rShift)|
				((g>>(8-gCount))<<gShift)|
				((b>>(8-bCount))<<bShift);
		}
	}
	SetUnlockTexture(IGraph3d,Texture);
}
void cPolyDispatcher::CreateTexture4444(cMaterial *Material,cRenderDevice *RenderDevice)
{
	sTexture *Texture=Material->Texture,*Opacity=Material->Opacity;
	assert(Opacity&&Opacity->texture&&Opacity->nTexture==0);
	assert(Opacity->_x==Texture->_x&&Opacity->_y==Texture->_y);
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	SetCreateTexture(IGraph3d,Texture,1<<Texture->_x,1<<Texture->_y,TEXFMT_ARGB4444);
	unsigned char *bTexture=0;
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt4444().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);;
	unsigned char *buf_texture=Texture->texture,*buf_opacity=Opacity->texture,*buf_rgb=Texture->GetPal();
	for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
	{
		unsigned short *buf=(unsigned short*)bTexture;
		for(int i=0;i<(1<<Texture->_x);i++,buf_texture++,buf_opacity++)
		{
			unsigned char *rgb=&buf_rgb[3*buf_texture[0]];
			int r=rgb[0],g=rgb[1],b=rgb[2];
			if(r>63) r=63; if(g>63) g=63; if(b>63) b=63;
			buf[i]=((buf_opacity[0]>>(8-aCount))<<aShift)|
				((r>>(PALETTE_MAX_SIZE-rCount))<<rShift)|
				((g>>(PALETTE_MAX_SIZE-gCount))<<gShift)|
				((b>>(PALETTE_MAX_SIZE-bCount))<<bShift);
		}
	}
	SetUnlockTexture(IGraph3d,Texture);
}
void cPolyDispatcher::CreateTexture4444(sTexture *Texture,cRenderDevice *RenderDevice)
{
	assert(Texture&&Texture->texture&&Texture->nTexture==0);
	assert(Texture->_x==Texture->_x);
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	SetCreateTexture(IGraph3d,Texture,1<<Texture->_x,1<<Texture->_y,TEXFMT_ARGB4444);
	unsigned char *bTexture=0;
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt4444().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);;
	unsigned char *buf_texture=Texture->texture;
	for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
	{
		unsigned short *buf=(unsigned short*)bTexture;
		for(int i=0;i<(1<<Texture->_x);i++,buf_texture+=4)
		{
			buf[i]=((buf_texture[3]>>(8-aCount))<<aShift)|
				((buf_texture[2]>>(8-rCount))<<rShift)|
				((buf_texture[1]>>(8-gCount))<<gShift)|
				((buf_texture[0]>>(8-bCount))<<bShift);
		}
	}
	SetUnlockTexture(IGraph3d,Texture);
}
void cPolyDispatcher::CreateTexture1555(cMaterial *Material,cRenderDevice *RenderDevice)
{
	sTexture *Texture=Material->Texture;
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	SetCreateTexture(IGraph3d,Texture,1<<Texture->_x,1<<Texture->_y,TEXFMT_ARGB1555);
	unsigned char *bTexture=0;
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt1555().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);;
	switch(Texture->GetAttribute(ATTRTEX_PAL8|ATTRTEX_RGB565|ATTRTEX_RGB888|ATTRTEX_RGBA8888))
	{
		case ATTRTEX_PAL8:
			{
				unsigned char *buf_texture=Texture->texture,*buf_rgb=Texture->GetPal();
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture++)
					{
						int number=3*buf_texture[0];
						unsigned char *rgb=&buf_rgb[number];
						int r=rgb[0],g=rgb[1],b=rgb[2];
						if(r>63) r=63; if(g>63) g=63; if(b>63) b=63;
						int a;
						if(number) a=255; else a=0;
						buf[i]=((a>>(8-aCount))<<aShift)|
							((r>>(PALETTE_MAX_SIZE-rCount))<<rShift)|
							((g>>(PALETTE_MAX_SIZE-gCount))<<gShift)|
							((b>>(PALETTE_MAX_SIZE-bCount))<<bShift);
					}
				}
				break;
			}
		case ATTRTEX_RGBA8888:
			{
				assert(Texture->GetPal()==0);
				unsigned char *buf_texture=Texture->texture;
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture+=4)
					{
						int a;
						if(buf_texture[3]) a=255; else a=0;
						buf[i]=((a>>(8-aCount))<<aShift)|
							((buf_texture[0]>>(8-rCount))<<rShift)|
							((buf_texture[1]>>(8-gCount))<<gShift)|
							((buf_texture[2]>>(8-bCount))<<bShift);
					}
				}
				break;
			}
		default: 
			assert(0);
	}
	SetUnlockTexture(IGraph3d,Texture);
}
void cPolyDispatcher::CreateTexture565(cMaterial *Material,cRenderDevice *RenderDevice)
{
	sTexture *Texture=Material->Texture;
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	SetCreateTexture(IGraph3d,Texture,1<<Texture->_x,1<<Texture->_y,TEXFMT_RGB565);
	unsigned char *bTexture=0;
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt565().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);
	switch(Texture->GetAttribute(ATTRTEX_PAL8|ATTRTEX_RGB565|ATTRTEX_RGB888|ATTRTEX_RGBA8888))
	{
		case ATTRTEX_PAL8:	
			{
				assert(Texture->GetPal());
				unsigned char *buf_texture=Texture->texture,*buf_rgb=Texture->GetPal();
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture++)
					{
						unsigned char *rgb=&buf_rgb[3*buf_texture[0]];
						int r=rgb[0],g=rgb[1],b=rgb[2];
						if(r>((1<<PALETTE_MAX_SIZE)-1)) r=((1<<PALETTE_MAX_SIZE)-1); 
						if(g>((1<<PALETTE_MAX_SIZE)-1)) g=((1<<PALETTE_MAX_SIZE)-1); 
						if(b>((1<<PALETTE_MAX_SIZE)-1)) b=((1<<PALETTE_MAX_SIZE)-1);
						buf[i]=	((255>>(8-aCount))<<aShift)|
								((r>>(PALETTE_MAX_SIZE-rCount))<<rShift)|
								((g>>(PALETTE_MAX_SIZE-gCount))<<gShift)|
								((b>>(PALETTE_MAX_SIZE-bCount))<<bShift);
					}
				}
				break;
			}
		case ATTRTEX_RGB565:	
			{
				assert(Texture->GetPal()==0);
				unsigned short *buf_texture=(unsigned short*)Texture->texture;
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture++)
					{
						int r=(((*buf_texture)>>10)&31)<<(8-5),
							g=(((*buf_texture)>>5)&63)<<(8-6),
							b=(((*buf_texture)>>0)&31)<<(8-5);
						buf[i]=	((255>>(8-aCount))<<aShift)|
								((r>>(8-rCount))<<rShift)|
								((g>>(8-gCount))<<gShift)|
								((b>>(8-bCount))<<bShift);
					}
				}
				break;
			}
		case ATTRTEX_RGB888:
			{
				assert(Texture->GetPal()==0);
				unsigned char *buf_texture=Texture->texture;
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture+=3)
						buf[i]=((255>>(8-aCount))<<aShift)|
							((buf_texture[0]>>(8-rCount))<<rShift)|
							((buf_texture[1]>>(8-gCount))<<gShift)|
							((buf_texture[2]>>(8-bCount))<<bShift);
				}
				break;
			}
		case ATTRTEX_RGBA8888:
			{
				assert(Texture->GetPal()==0);
				unsigned char *buf_texture=Texture->texture;
				for(int j=0;j<(1<<Texture->_y);j++,bTexture+=Texture->bpl)
				{
					unsigned short *buf=(unsigned short*)bTexture;
					for(int i=0;i<(1<<Texture->_x);i++,buf_texture+=4)
					{
						int a;
						if(buf_texture[3]) a=255; else a=0;
						buf[i]=((a>>(8-aCount))<<aShift)|
							((buf_texture[0]>>(8-rCount))<<rShift)|
							((buf_texture[1]>>(8-gCount))<<gShift)|
							((buf_texture[2]>>(8-bCount))<<bShift);
					}
				}
				break;
			}
		default:
			assert(0);
	}
	SetUnlockTexture(IGraph3d,Texture);
}

void cPolyDispatcher::ResetTextureTileMap565(cMaterial *Material,cRenderDevice *RenderDevice)
{
	sTexture *Texture=Material->Texture;
	unsigned char *bTexture;
	int _x=Texture->_x,_y=Texture->_y;
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	if(Texture->nTexture==0) SetCreateTexture(IGraph3d,Texture,1<<_x,1<<_y,TEXFMT_RGB565);
	SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
	assert(bTexture);
	assert(_x);
	assert(_x==_y);
	int TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	RenderDevice->GetTexFmt565().Get(TexFmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);
	unsigned short *color=&GlobalColorBufferMap[PosBufTileMap];
	unsigned int dx=1<<(MAP_TILE_SIZE_BIT-_x),dy=1<<(MAP_TILE_SIZE_BIT-_y);
	for(int j=0;j<(1<<_y);j++,bTexture+=Texture->bpl,color+=(dy<<H_SIZE_POWER))
	{
		unsigned short *buf=(unsigned short*)bTexture,*buf_end=buf+(1<<_x);
		unsigned short *cl_buf=color;
		while(buf<buf_end)
		{
			int r=cl_buf[0]>>11,g=(cl_buf[0]>>5)&63,b=*(cl_buf)&31;	cl_buf+=dx;
			*buf++= ((255>>(8-aCount))<<aShift)|
					((r>>(5-rCount))<<rShift)|
					((g>>(6-gCount))<<gShift)|
					((b>>(5-bCount))<<bShift);
		}
	}
	SetUnlockTexture(IGraph3d,Texture);
}
void cPolyDispatcher::CreateTexture(cMaterial *Material,cRenderDevice *RenderDevice)
{
	switch(Material->GetAttribute(ATTRMAT_TEXTURE_PAL|ATTRMAT_TILE_MAP))
	{
		case ATTRMAT_TEXTURE_PAL:
			switch(Material->GetAttribute(ATTRMAT_TEXTURE_PAL|ATTRMAT_OPACITY|ATTRMAT_MASK|ATTRMAT_RGBA8888))
			{
				case ATTRMAT_TEXTURE_PAL:
					CreateTexture565(Material,RenderDevice);
					break;
				case ATTRMAT_TEXTURE_PAL|ATTRMAT_MASK:
					CreateTexture1555(Material,RenderDevice);
					break;
				case ATTRMAT_TEXTURE_PAL|ATTRMAT_OPACITY:
					CreateTexture4444(Material,RenderDevice);
					break;
				case ATTRMAT_TEXTURE_PAL|ATTRMAT_OPACITY|ATTRMAT_RGBA8888:
					CreateTexture8888(Material,RenderDevice);
					break;
				default: assert(0);
			}
			break;
#ifdef _USE_TILEMAP_
		case ATTRMAT_TILE_MAP:
			ResetTextureTileMap565(Material,RenderDevice);
			break;
#endif //_USE_TILEMAP_
		default:
			assert(0);
	}
}
void cPolyDispatcher::DeleteTexture(sTexture *Texture)
{
	if(GetNumberRenderDevice()==0) 
	{
		Texture->ClearStatus(STATUS_TEXTURE_CREATE|STATUS_TEXTURE_CACHE);
		Texture->nTexture=0;
		return;
	}
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	SetDeleteTexture(Graph3d,Texture);
}
//////////////////// SURFACEREFLECTION TEXTURE OPERATION ////////////////////
void ResetTextureMultiMaterialSurface565(cInterfaceGraph3d *IGraph3d,cMaterial *Material,cSurfaceReflectionMultiMaterial *Surface)
{
#ifdef _MECHOSOMA_
	extern void acsGetTexture(int x,int y,unsigned char* p);
	for(int j=0;j<Surface->yMaterial;j++)
	for(int i=0;i<Surface->xMaterial;i++)
		if(&Surface->MaterialArray[i+j*Surface->xMaterial]==Material)
		{
			int xPosition=Surface->x()+Surface->xsize*Surface->xstep/Surface->xMaterial;
			int yPosition=Surface->y()+Surface->ysize*Surface->ystep/Surface->yMaterial;
			int	xsize=0;
			sTexture *Texture=Material->Texture;
			if(Texture->nTexture==0) return;
			unsigned char *bTexture=0;
			SetLockTexture(IGraph3d,Texture,(void**)&bTexture);
			assert(bTexture);
			unsigned char *buf_texture=Texture->texture,*buf_rgb=Texture->GetPal();
			acsGetTexture(i,j,bTexture);
/*
			for(int j=0;j<(1<<Texture->_y);j++,bTexture+=xsize)
			{
				unsigned short *buf=(unsigned short*)bTexture;
				for(int i=0;i<(1<<Texture->_x);i++,buf_texture++)
				{
					unsigned char *rgb=&buf_rgb[3*buf_texture[0]];
					int r=rgb[0],g=rgb[1],b=rgb[2];
					if(r>63) r=63; if(g>63) g=63; if(b>63) b=63;
					if(i%2)
						buf[i]=	((r>>(PALETTE_MAX_SIZE-rCount565))<<rShift565)|
								((g>>(PALETTE_MAX_SIZE-gCount565))<<gShift565)|
								((b>>(PALETTE_MAX_SIZE-bCount565))<<bShift565);
					else
						buf[i]=0xFFFF;
				}
			}
*/
			SetUnlockTexture(IGraph3d,Texture);
		}
#endif
}
//////////////////// TILEMAP TEXTURE OPERATION ////////////////////
void cPolyDispatcher::CreateTexture(cTileMap *TileMap)
{
#ifdef _USE_TILEMAP_
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	GlobalColorBufferMap=(unsigned short*)vMap_ClBuf();
	int count=0;
	for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
	{
		PosBufTileMap=TERRA_XCYCL(bTile->xPos)+(TERRA_YCYCL(bTile->yPos)<<TERRA_H_SIZE_POWER);
		sTexture *Texture=bTile->GetTexture(),*LightMap=bTile->GetLightMap();
		if(!Texture->GetStatus(STATUS_TEXTURE_CREATE))
			ResetTextureTileMap565(bTile->GetMaterial(),RenderDevice);
		if(!LightMap->GetStatus(STATUS_TEXTURE_CREATE))
			SetCreateTexture(Graph3d,LightMap,1<<LightMap->_x,1<<LightMap->_y,TEXFMT_ARGB4444);
	}
#endif //_USE_TILEMAP_
}
void cPolyDispatcher::UnlockTexture(cUnknownClass *UCameraList,cTileMap *TileMap)
{ // только для однодевайсного устройства
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
		if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_LOCK))
		{
			SetUnlockTexture(Graph3d,bTile->GetLightMap());
			bTile->GetLightMap()->ClearStatus(STATUS_TEXTURE_RESET);
			bTile->GetLightMap()->SetStatus(STATUS_TEXTURE_SHADOW);
		}
}
void cPolyDispatcher::BuildShade(short *shade,int xShade,int yShade,Vect3f &PosShade,cTileMap *TileMap)
{ // только для однодевайсного устройства
	unsigned char *MemTextureTile=0;
	int iTileMap=0,jTileMap=0;
	sTexture *CurrentLightMap=0;
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	int aShift4444=RenderDevice->GetTexFmt4444().aBitShift;
	int dwShade=(int(round(1.f*((1<<RenderDevice->GetTexFmt4444().aBitCount)-1)))<<RenderDevice->GetTexFmt4444().aBitShift)|
		(int(round(0.5f*((1<<RenderDevice->GetTexFmt4444().rBitCount)-1)))<<RenderDevice->GetTexFmt4444().rBitShift)|
		(int(round(0.5f*((1<<RenderDevice->GetTexFmt4444().gBitCount)-1)))<<RenderDevice->GetTexFmt4444().gBitShift)|
		(int(round(0.5f*((1<<RenderDevice->GetTexFmt4444().bBitCount)-1)))<<RenderDevice->GetTexFmt4444().bBitShift);

	int xSh=TERRA_XCYCL(round(PosShade.x)),ySh=TERRA_YCYCL(round(PosShade.y)),zSh=round(PosShade.z);
	short *shadeBuf=shade;
	int dzx=(1<<16)/CONST_DYNAMIC_SHADE_MESH;
	iTileMap=jTileMap=-1;
#ifdef _USE_TILEMAP_
	int ReflectionShade=0;
	if(TileMap) ReflectionShade=TileMap->GetAttribute(ATTR_TILEMAP_WORLD_REFLECTION);
	for (int j=0;j<yShade;j+=(1<<MAP_LIGHTMAP_STEP_BIT),shadeBuf+=(xShade<<MAP_LIGHTMAP_STEP_BIT))  
	{
		int y=YCYCL(ySh+j), x=xSh,x_cycl=xSh,v;
		unsigned char *pv0=&(vMap_VxBuf()[(y>>TERRA_kmVxBuf)<<(TERRA_H_SIZE_POWER-TERRA_kmVxBuf)]);
		unsigned char *pa0=&(vMap_AtBuf()[(y>>TERRA_kmVxBuf)<<(TERRA_H_SIZE_POWER-TERRA_kmVxBuf)]);
		if(!ReflectionShade) // тень отбрасывается на дно водоема
			while((zSh<(v=(pv0[x_cycl>>TERRA_kmVxBuf]<<TERRA_SHIFT_VX)))&&((xSh-x)<xShade))
				x_cycl=XCYCL(x-=(1<<MAP_LIGHTMAP_STEP_BIT));
		else
			while((zSh<(v=(((pa0[x_cycl>>TERRA_kmVxBuf]&At_WATER)==0)?(pv0[x_cycl>>TERRA_kmVxBuf]<<TERRA_SHIFT_VX):vMap_LevelH2O())))&&((xSh-x)<xShade))
				x_cycl=XCYCL(x-=(1<<MAP_LIGHTMAP_STEP_BIT));
		if(v>zSh) continue;
		int xshade=(xSh-x)-(((zSh-v)*dzx)>>16);
		while(xshade<xShade)
		{
			if(!ReflectionShade) // тень отбрасывается на дно водоема
				v=pv0[x_cycl>>TERRA_kmVxBuf]<<TERRA_SHIFT_VX;
			else
				v=(((pa0[x_cycl>>TERRA_kmVxBuf]&TERRA_At_WATER)==0)?(pv0[x_cycl>>TERRA_kmVxBuf]<<TERRA_SHIFT_VX):vMap_LevelH2O());
			if((xshade>0)&&(shadeBuf[xshade]>=v)&&((pa0[x_cycl>>TERRA_kmVxBuf]&TERRA_At_SHADOW)==0))
//			if((xshade>0)&&(shadeBuf[xshade]>=v)&&((pa0[x_cycl>>TERRA_kmVxBuf]&TERRA_At_SHADOW)==0)&&((pa0[x_cycl>>TERRA_kmVxBuf]&TERRA_Tr_MASK)!=15))
			{
				int i=x_cycl>>TileMap->_SizeTileX(), j=y>>TileMap->_SizeTileY();
				if((i!=iTileMap)||(j!=jTileMap))
				{
					iTileMap=i; jTileMap=j;
					CurrentLightMap=TileMap->GetTile(iTileMap,jTileMap)->GetLightMap();
					if(CurrentLightMap->GetStatus(STATUS_TEXTURE_LOCK))
						MemTextureTile=CurrentLightMap->texture;
					else 
					{
						if(!CurrentLightMap->GetStatus(STATUS_TEXTURE_CREATE))
							SetCreateTexture(Graph3d,CurrentLightMap,1<<CurrentLightMap->_x,1<<CurrentLightMap->_y,TEXFMT_ARGB4444);
						SetLockTexture(Graph3d,CurrentLightMap,(void**)&MemTextureTile);
						memfill((uint32_t*)MemTextureTile,CurrentLightMap->bpl<<(CurrentLightMap->_y-2),0xFFFFFFFF);
//						memfill((unsigned long*)MemTextureTile,CurrentLightMap->bpl<<(CurrentLightMap->_y-2),0x00000000);
						CurrentLightMap->texture=MemTextureTile;
						CurrentLightMap->SetStatus(STATUS_TEXTURE_RESET);
					}
				}
				int u=(x_cycl>>MAP_LIGHTMAP_STEP_BIT)&((1<<CurrentLightMap->_x)-1);
				int v=(y>>MAP_LIGHTMAP_STEP_BIT)&((1<<CurrentLightMap->_y)-1);
				unsigned short *buf=(unsigned short*)&MemTextureTile[v*CurrentLightMap->bpl];
//				buf[u]=(6<<aShift4444);
				buf[u]=dwShade;
			}
			x_cycl=XCYCL(x-=(1<<MAP_LIGHTMAP_STEP_BIT));
			xshade=(xSh-x)-(((zSh-v)*dzx)>>16);
		}
	}
#endif
#ifndef _MECHOSOMA_
	delete shade;
#endif
}
