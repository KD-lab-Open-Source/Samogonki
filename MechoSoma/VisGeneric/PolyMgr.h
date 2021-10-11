#ifndef __POLYMGR_H__
#define __POLYMGR_H__

#include "BaseClass.h"
#include "UnkLibrary.h"
#include "IGraph3d.h"
#include "umath.h"
#include "DrawPolygonShade.h"

#define POINT_SCALE_W				0.9f

#define GET_FRUSTUM_CLIP(a)							((a)&(CLIP_XMIN|CLIP_XMAX|CLIP_YMIN|CLIP_YMAX|CLIP_ZMIN|CLIP_ZMAX))
enum eAttributeClipPoint
{
	CLIP_NILL					=	0,
	CLIP_XMIN					=	1<<0,
	CLIP_XMAX					=	1<<1,
	CLIP_YMIN					=	1<<2,
	CLIP_YMAX					=	1<<3,
	CLIP_ZMIN					=	1<<4,
	CLIP_ZMAX					=	1<<5,
};

#define GET_RENDER(a)								((a)&0x00FFFFFF)
#define GET_RENDER_TYPE(a)							((a)&0x0000FFFF)
// type render 1..16
enum eAttributeRenderPolygon
{
	RENDER_COLOR_MOD_DIFFUSE					=	MAT_COLOR_MOD_DIFFUSE,
	RENDER_COLOR_MOD_TEXTURE1					=	MAT_COLOR_MOD_TEXTURE1,
	RENDER_COLOR_ADD_TEXTURE1					=	MAT_COLOR_ADD_TEXTURE1,
	RENDER_COLOR_ADD_SPECULAR					=	MAT_COLOR_ADD_SPECULAR,
	RENDER_ALPHA_MOD_DIFFUSE					=	MAT_ALPHA_MOD_DIFFUSE,
	RENDER_ALPHA_MOD_TEXTURE1					=	MAT_ALPHA_MOD_TEXTURE1,
	RENDER_ALPHA_MASK_TEXTURE1					=	MAT_ALPHA_MASK_TEXTURE1,
	RENDER_ALPHA_ADD_DIFFUSE					=	MAT_COLOR_ADD_DIFFUSE,
	// special render 16..31
	RENDER_MULTICANAL							=	1<<17,
	RENDER_REFLECTION							=	1<<18,
	// special render 24..31
	RENDER_CLIPPING3D							=	1<<24,
};

#define GET_INT_RGBA(r,g,b,a)		(((a)<<24)|((r)<<16)|((g)<<8)|(b))
#define GET_INT_R(rgba)				(((rgba)>>16)&0xff)
#define GET_INT_G(rgba)				(((rgba)>> 8)&0xff)
#define GET_INT_B(rgba)				(((rgba)>> 0)&0xff)
#define GET_INT_A(rgba)				(((rgba)>>24)&0xff)

#define HARDWARE_Z(z)			(1-(float)POLYGON_CUTTING_W/(z))
#define HARDWARE_ZW(zh,wh,z)	{ wh=(1/POLYGON_SCALE_W)/z; zh=1-(POLYGON_CUTTING_W*POLYGON_SCALE_W)*wh; }
#define HARDWARE_W(w)			(POLYGON_SCALE_W*w)
#define SOFTWARE_Z(wh)			((1/POLYGON_SCALE_W)/(wh))

//private
#define GET_CLIP(xe,ye,zv)													\
	(	(xClipMin>xe?CLIP_XMIN : (xClipMax<xe?CLIP_XMAX:0)) |				\
		(yClipMin>ye?CLIP_YMIN : (yClipMax<ye?CLIP_YMAX:0)) |				\
		(zClipMin>zv?CLIP_ZMIN : (zClipMax<zv?CLIP_ZMAX:0)) )

#define SET_POINTFIX_XYW(p,fxe,fye,fwh)									{ p.xe=fxe; p.ye=fye; p.z=fwh; }
#define SET_POINTFIX_D(p,rd,gd,bd,ad)									{ p.dr()=rd; p.dg()=gd; p.db()=bd; p.da()=ad; }
#define SET_POINTFIX_S(p,rs,gs,bs,as)									{ p.sr()=rs; p.sg()=gs; p.sb()=bs; p.sa()=as; }
#define SET_POINTFIX_UV1(p,fu1,fv1)										{ p.u1()=fu1; p.v1()=fv1; }

#define SET_POLYGONFIX(f,v1,v2,v3)										{ f.p1=v1; f.p2=v2; f.p3=v3; }
#define SWAP_POLYGONFIX(f1,f2)											{ int tmp=f2.p1; f2.p1=f1.p1; f1.p1=tmp; tmp=f2.p2; f2.p2=f1.p2; f1.p2=tmp; tmp=f2.p3; f2.p3=f1.p3; f1.p3=tmp; }
#define RESET_POLYGONFIX(f1,f2)											{ f1.p1=f2.p1; f1.p2=f2.p2; f1.p3=f2.p3; }

class cSun;
class cOmni;
class cMesh;
class cMatrix;
class cCamera;
class cTileMap;
class cPolyGrid;
class cMaterial;
class cTileWater;
class cConvertor;
class cTangentTrail;
class cRenderDevice;
class cBasePolyGrid;
class cWorldPolyGrid;
class cSurfaceReflectionMultiMaterial;
struct sTextureChild;
struct sTileMap;
struct sTexture;

struct sSpriteFX
{
	unsigned char	rD,gD,bD,aD;
	Vect2f			a,b;
	sSpriteFX()															{ rD=gD=bD=aD=255; a.set(0,0); b.set(0,0); }
	void Init(sColor4f &diffuse,float angle,float scale)				{ a.set(scale*cosf(angle+0.7854f),scale*sinf(angle+0.7854f)); b.set(scale*cosf(angle+2.3562f),scale*sinf(angle+2.3562f)); rD=diffuse.GetR(); gD=diffuse.GetG(); bD=diffuse.GetB(); aD=diffuse.GetA(); }
};

struct sPointAttribute
{
	Vect3f			pv;		// координаты в прострастве камеры
	unsigned char	clip;	// флаги клиппирования
	unsigned int	edge;	// указывает на две вершины породивших при клиповке данную
	inline void Set(float fxv,float fyv,float fzv)						{ pv.x=fxv; pv.y=fyv; pv.z=fzv; }
	inline void Set(sPointAttribute &pa1,sPointAttribute &pa2,float t)	{ pv.x=pa1.pv.x+(pa2.pv.x-pa1.pv.x)*t; pv.y=pa1.pv.y+(pa2.pv.y-pa1.pv.y)*t;	pv.z=pa1.pv.z+(pa2.pv.z-pa1.pv.z)*t; };
};

class cPolyDispatcher : public sTracePolygon
{
	cBaseArray <sPlane4f>			PlaneClip3d;
	float							xClipMin,xClipMax,yClipMin,yClipMax,zClipMin,zClipMax;
	cCamera							*CurrentCamera;
	cConvertor						*CurrentConvertorObjectToScreen;
	int								AlphaForSprite;
	Vect2f							Center,Focus;	
public:
	int								Attribute;
	cBaseArray <sVertexFix>			PointFix;
	cBaseArray <sPolygonFix>		PolygonFix;
	cBaseArray <sPointAttribute>	PointAttribute;
	cUnknownDynArrayPointer			RenderDeviceArray;
	cBaseArray <sVertexD3D>			VertexD3D;

	cPolyDispatcher();
	~cPolyDispatcher();

	// RenderDevice function
	void Attach(cUnknownClass *URenderDevice)			{ assert(URenderDevice->GetKind(KIND_RENDERDEVICE)); RenderDeviceArray.Attach(URenderDevice); }
	void Detach(cUnknownClass *URenderDevice)			{ assert(URenderDevice->GetKind(KIND_RENDERDEVICE)); RenderDeviceArray.Detach(URenderDevice); }
	inline cRenderDevice* GetRenderDevice(int number)	{ return (cRenderDevice*)RenderDeviceArray[number]; }
	inline int GetNumberRenderDevice()					{ return RenderDeviceArray.length(); }
	void SetClippingPlane(cCamera *Camera);

	int Draw(cUnknownClass *UCamera,cUnknownClass *UViewPort,int hTexture=0,int hLightMap=0);
	void BeginScene(cUnknownClass *UCameraList);
	void EndScene(cUnknownClass *UCameraList);
	// Texture operation
	void CreateTexture(cMaterial *Material,cRenderDevice *RenderDevice);
	void DeleteTexture(sTexture *Texture);

	// Fix-format function
	void InitFix(int attribute,int NumberPoint=0);
	__forceinline void AddPolygonFixTestPointFix(int i1,int i2,int i3);// добавление в конецполигона после теста
	__forceinline void SetPointFix(int i,const Vect2f &tex);
	__forceinline void SetPointFix(int i,int dr,int dg,int db,int da);
	__forceinline void SetPointFix(int i,int dr,int dg,int db,int da,const Vect2f &tex);
	__forceinline void SetPointFix(int i,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa);
	__forceinline void SetPointFix(int i,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect2f &tex);
	__forceinline void SetPointFix(int i,float xe,float ye,float w,float u,float v,float xv,float yv,float zv);
	__forceinline void SetPointFix(int i,float xe,float ye,float w,int dr,int dg,int db,int da,float xv,float yv,float zv);
	__forceinline void SetPointFix(int i,float xe,float ye,float w,int dr,int dg,int db,int da,float u,float v,float xv,float yv,float zv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,const Vect3f &pv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,const Vect2f &tex,const Vect3f &pv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,const Vect3f &pv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,const Vect2f &tex,const Vect3f &pv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect3f &pv);
	__forceinline void SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect2f &tex,const Vect3f &pv);
	// функции растеризации
	void Draw(cUnknownClass *UCameraList,cOmni *Omni);
	
	void Draw(cUnknownClass *UScene,cUnknownClass *UCameraList,cMesh *Mesh,Vect3f *vReflection=0);

	void BuildShade(short *shade,int xShade,int yShade,Vect3f &PosShade,cTileMap *TileMap);
	void UnlockTexture(cUnknownClass *UCameraList,cTileMap *TileMap);
	void CreateTexture(cTileMap *TileMap);
	void Draw(cUnknownClass *UCameraList,cTileMap *TileMap);
	void DrawReflection(cUnknownClass *UCameraList,cTileMap *TileMap);
	
	void Draw(cUnknownClass *UCameraList,cPolyGrid *PolyGrid);

	void Draw(cUnknownClass *UCameraList,cTileWater *TileWater);

	void Draw(cUnknownClass *UCameraList,cWorldPolyGrid *WorldPolyGrid);

	void Draw(cUnknownClass *UCameraList,cSurfaceReflectionMultiMaterial *Surface);
	
	void Draw(cUnknownClass *UCameraList,cUnkClassDynArrayPointer *SunArray);

	void Draw(cUnknownClass *UCameraList,cTangentTrail *TangentTrail);

	void SetViewColor(cUnknownClass *UCamera,sColor4f &Diffuse,sColor4f &Specular,int zWrite=0);
	void BeginList(cUnknownClass *UCamera,int idTextureChild,MatXf &Matrix); // для трехмерных эффектов
	void AttachCenter(const Vect3f &pos,float angle,float scale,int rgbaDiffuse,int idTextureChild);
	void AttachCenter(const Vect3f &pos,sSpriteFX *SpriteFX,int idTextureChild);
	void BeginList(cUnknownClass *UCamera,int idTextureChild); // для плоских эффектов
	void AttachCenter(const Vect2f &pos,sSpriteFX *SpriteFX,int idTextureChild);
	void BeginListShare(cUnknownClass *UCamera,int idTextureChild,MatXf &Matrix);
	void AttachCenterShare(const Vect3f &pos,sSpriteFX *SpriteFX,int idTextureChild);
	void EndList();

private:
	inline int AssertValid();
	// PolygonFix
	inline int  AddPolygonFix(int p1,int p2,int p3);		// добавление в конец
	inline void DelPolygonFix(int i);						// удаление полигона из позиции
	inline void MovPolygonFix(int i);						// перемещение из положения i в конец
	inline void SetPolygonFix(int i,int p1,int p2,int p3);	// установка значений полигона

	inline sVertexFix& NextPointFix()					{ return PointFix[PointFix.CurrentSize++]; }
	inline sPolygonFix& NextPolygonFix()				{ return PolygonFix[PolygonFix.CurrentSize++]; }
	inline sPointAttribute& NextPointAttribute()		{ return PointAttribute[PointAttribute.CurrentSize++]; }
	// AddPoint
	int AddPointFix(sPlane4f &PlaneClip3d,int i1,int i2);
	// push
	inline int PushPolygonFix(int ofs=0)		{ int tmp=PolygonFix.length(); PolygonFix.Base=&PolygonFix.Base[PolygonFix.length()]; PolygonFix.length()=ofs; return tmp; } 
	inline int PushPointFix(int ofs=0)			{ int tmp=PointFix.length(); PointFix.Base=&PointFix.Base[PointFix.length()]; PointAttribute.Base=&PointAttribute.Base[PointAttribute.length()]; PointFix.length()=PointAttribute.length()=ofs; return tmp; } 
	// pop
	inline int PopPolygonFix(int ofs)			{ int tmp=PolygonFix.length(); PolygonFix.Base=&PolygonFix.Base[-(PolygonFix.length()=ofs)]; return tmp; }
	inline int PopPointFix(int ofs)				{ int tmp=PointFix.length(); PointFix.Base=&PointFix.Base[-(PointFix.length()=ofs)]; PointAttribute.Base=&PointAttribute.Base[-(PointAttribute.length()=ofs)]; return tmp; }

	void Clip_PolygonFix(int i,sPlane4f &PlaneClip3d,int mask);
	void ClipPolygonFix(cCamera *Camera);

	void CreateTexture565(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture1555(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture4444(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture8888(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture4444(sTexture *Texture,cRenderDevice *RenderDevice);

	void ResetTextureTileMap565(cMaterial *Material,cRenderDevice *RenderDevice);

	void DrawTilePolyGrid(cRenderDevice *RenderDevice,cCamera *Camera,cBasePolyGrid *WorldPolyGrid,int i1,int j1,int i2,int j2,cMaterial *Material);
};

extern cPolyDispatcher *P3D;

#define ADD_POINT_ATTRIBUTE(i,xe,ye,xv,yv,zv)			\
	{													\
		assert(i<PointAttribute.length());				\
		sPointAttribute &pa=PointAttribute[i];			\
		pa.Set(xv,yv,zv);								\
		pa.clip=GET_CLIP(xe,ye,zv);						\
	}

__forceinline void cPolyDispatcher::AddPolygonFixTestPointFix(int i1,int i2,int i3)	
{	// добавление в конец полигона после теста
	assert((i1<PointFix.length())&&(i2<PointFix.length())&&(i3<PointFix.length()));
	sVertexFix &p1=PointFix[i1],&p2=PointFix[i2],&p3=PointFix[i3];
	if(PointAttribute[i1].clip&PointAttribute[i2].clip&PointAttribute[i3].clip) return;
	if((PointAttribute[i1].clip|PointAttribute[i2].clip|PointAttribute[i3].clip)&(CLIP_ZMIN|CLIP_ZMAX))
	{
		sPolygonFix &pFix=NextPolygonFix();
		SET_POLYGONFIX(pFix,i1,i2,i3);
	}
	else if(((p3.xe-p2.xe)*(p3.ye-p1.ye)-(p3.xe-p1.xe)*(p3.ye-p2.ye))>0)
	{
		sPolygonFix &pFix=NextPolygonFix();
		SET_POLYGONFIX(pFix,i1,i2,i3);
	}
	else return;
	if((Attribute&RENDER_CLIPPING3D)==0) return;
	int start=PolygonFix.length()-1,end=PolygonFix.length();
	int i;
	for(i=start;i<end;i++)
	{
		sPolygonFix &p=PolygonFix[i];
		int a1=PointAttribute[p.p1].clip, a2=PointAttribute[p.p2].clip, a3=PointAttribute[p.p3].clip;
		if(a1&a2&a3&CLIP_ZMIN) 
		{ DelPolygonFix(i--); end--; }
		else if((a1|a2|a3)&CLIP_ZMIN)
			Clip_PolygonFix(i,PlaneClip3d[0],CLIP_ZMIN);
	}
	end=PolygonFix.length();
	for(i=start;i<end;i++)
	{
		sPolygonFix &p=PolygonFix[i];
		int a1=PointAttribute[p.p1].clip, a2=PointAttribute[p.p2].clip, a3=PointAttribute[p.p3].clip;
		if(a1&a2&a3&CLIP_XMIN) 
		{ DelPolygonFix(i--); end--; }
		else if((a1|a2|a3)&CLIP_XMIN)
			Clip_PolygonFix(i,PlaneClip3d[1],CLIP_XMIN);
	}
	end=PolygonFix.length();
	for(i=start;i<end;i++)
	{
		sPolygonFix &p=PolygonFix[i];
		int a1=PointAttribute[p.p1].clip, a2=PointAttribute[p.p2].clip, a3=PointAttribute[p.p3].clip;
		if(a1&a2&a3&CLIP_XMAX) 
		{ DelPolygonFix(i--); end--; }
		else if((a1|a2|a3)&CLIP_XMAX)
			Clip_PolygonFix(i,PlaneClip3d[2],CLIP_XMAX);
	}
	end=PolygonFix.length();
	for(i=start;i<end;i++)
	{
		sPolygonFix &p=PolygonFix[i];
		int a1=PointAttribute[p.p1].clip, a2=PointAttribute[p.p2].clip, a3=PointAttribute[p.p3].clip;
		if(a1&a2&a3&CLIP_YMIN) 
		{ DelPolygonFix(i--); end--; }
		else if((a1|a2|a3)&CLIP_YMIN)
			Clip_PolygonFix(i,PlaneClip3d[3],CLIP_YMIN);
	}
	end=PolygonFix.length();
	for(i=start;i<end;i++)
	{
		sPolygonFix &p=PolygonFix[i];
		int a1=PointAttribute[p.p1].clip, a2=PointAttribute[p.p2].clip, a3=PointAttribute[p.p3].clip;
		if(a1&a2&a3&CLIP_YMAX) 
		{ DelPolygonFix(i--); end--; }
		else if((a1|a2|a3)&CLIP_YMAX)
			Clip_PolygonFix(i,PlaneClip3d[4],CLIP_YMAX);
	}
}
inline void cPolyDispatcher::DelPolygonFix(int i)
{
	int end=PolygonFix.length()-1;
	if(i!=end) SWAP_POLYGONFIX(PolygonFix[i],PolygonFix[end]);
	PolygonFix.CurrentSize--; 
}
inline void cPolyDispatcher::SetPolygonFix(int i,int p1,int p2,int p3)
{
	assert(i<PolygonFix.length());
	SET_POLYGONFIX(PolygonFix[i],p1,p2,p3);
}
inline int cPolyDispatcher::AddPolygonFix(int p1,int p2,int p3)
{
	sPolygonFix &pFix=NextPolygonFix();
	SET_POLYGONFIX(pFix,p1,p2,p3);
	return PolygonFix.length()-1;
}
inline void cPolyDispatcher::MovPolygonFix(int i)
{
	assert(i<PolygonFix.length());
	sPolygonFix &pFix=NextPolygonFix();
	RESET_POLYGONFIX(pFix,PolygonFix[i]);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,int dr,int dg,int db,int da)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length()&&i<PointAttribute.length());
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	PointAttribute[i].clip=GET_CLIP(PointFix[i].xe,PointFix[i].ye,PointAttribute[i].pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect2f &tex)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length()&&i<PointAttribute.length());
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	PointAttribute[i].clip=GET_CLIP(PointFix[i].xe,PointFix[i].ye,PointAttribute[i].pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,int dr,int dg,int db,int da,const Vect2f &tex)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length()&&i<PointAttribute.length());
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	PointAttribute[i].clip=GET_CLIP(PointFix[i].xe,PointFix[i].ye,PointAttribute[i].pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length()&&i<PointAttribute.length());
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_S(PointFix[i],sr,sg,sb,sa);
	PointAttribute[i].clip=GET_CLIP(PointFix[i].xe,PointFix[i].ye,PointAttribute[i].pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect2f &tex)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length()&&i<PointAttribute.length());
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_S(PointFix[i],sr,sg,sb,sa);
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	PointAttribute[i].clip=GET_CLIP(PointFix[i].xe,PointFix[i].ye,PointAttribute[i].pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,float xe,float ye,float w,float u,float v,float xv,float yv,float zv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],xe,ye,w);
	SET_POINTFIX_UV1(PointFix[i],u,v);
	ADD_POINT_ATTRIBUTE(i,xe,ye,xv,yv,zv);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,float xe,float ye,float w,int r,int g,int b,int a,float xv,float yv,float zv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],xe,ye,w);
	SET_POINTFIX_D(PointFix[i],r,g,b,a);
	ADD_POINT_ATTRIBUTE(i,xe,ye,xv,yv,zv);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,float xe,float ye,float w,int r,int g,int b,int a,float u,float v,float xv,float yv,float zv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],xe,ye,w);
	SET_POINTFIX_D(PointFix[i],r,g,b,a);
	SET_POINTFIX_UV1(PointFix[i],u,v);
	ADD_POINT_ATTRIBUTE(i,xe,ye,xv,yv,zv);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,const Vect2f &tex,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,const Vect2f &tex,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_S(PointFix[i],sr,sg,sb,sa);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,int dr,int dg,int db,int da,int sr,int sg,int sb,int sa,const Vect2f &tex,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	SET_POINTFIX_D(PointFix[i],dr,dg,db,da);
	SET_POINTFIX_S(PointFix[i],sr,sg,sb,sa);
	SET_POINTFIX_UV1(PointFix[i],tex.x,tex.y);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
__forceinline void cPolyDispatcher::SetPointFix(int i,const Vect3f &pe,const Vect3f &pv)
{
	assert(PointFix.length()<PointFix.MaxSize&&i<PointFix.length());
	SET_POINTFIX_XYW(PointFix[i],pe.x,pe.y,pe.z);
	ADD_POINT_ATTRIBUTE(i,pe.x,pe.y,pv.x,pv.y,pv.z);
}
inline int cPolyDispatcher::AssertValid()
{
#ifdef _DEBUG
	for(int i=0;i<PolygonFix.length();i++)
	{
		sPolygonFix &p=PolygonFix[i];
		if((p.p1>=PointFix.length())||(p.p2>=PointFix.length())||(p.p3>=PointFix.length()))
			return 0;
	}
#endif
	return 1;
}

#endif // __POLYMGR_H__