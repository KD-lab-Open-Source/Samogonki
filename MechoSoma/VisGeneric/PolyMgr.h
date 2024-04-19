#ifndef __POLYMGR_H__
#define __POLYMGR_H__

#include "BaseClass.h"
#include "UnkLibrary.h"
#include "IGraph3d.h"
#include "UMATH.H"
#include "DrawPolygonShade.h"

#ifndef __forceinline
#define __forceinline inline
#endif

#define POINT_SCALE_W				0.9f

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
	void Init(const sColor4f &diffuse,float angle,float scale)				{ a.set(scale*cosf(angle+0.7854f),scale*sinf(angle+0.7854f)); b.set(scale*cosf(angle+2.3562f),scale*sinf(angle+2.3562f)); rD=diffuse.GetR(); gD=diffuse.GetG(); bD=diffuse.GetB(); aD=diffuse.GetA(); }
};

class cPolyDispatcher : public sTracePolygon
{
	cCamera							*CurrentCamera;
	cConvertor						*CurrentConvertorObjectToScreen;
	int								AlphaForSprite;

	M3D_DRAW_COMMAND				CurrentDrawCommand;
	int 							CurrentListPointIndex = 0;
public:
	int								Attribute;
	cBaseArray <sVertexFix>			PointFix;
	cUnknownDynArrayPointer			RenderDeviceArray;

	cPolyDispatcher();
	~cPolyDispatcher();

	// RenderDevice function
	void Attach(cUnknownClass *URenderDevice)			{ assert(URenderDevice->GetKind(KIND_RENDERDEVICE)); RenderDeviceArray.Attach(URenderDevice); }
	void Detach(cUnknownClass *URenderDevice)			{ assert(URenderDevice->GetKind(KIND_RENDERDEVICE)); RenderDeviceArray.Detach(URenderDevice); }
	inline cRenderDevice* GetRenderDevice(int number)	{ return (cRenderDevice*)RenderDeviceArray[number]; }
	inline int GetNumberRenderDevice()					{ return RenderDeviceArray.length(); }

	void BeginScene(cUnknownClass *UCameraList);
	void EndScene(cUnknownClass *UCameraList);
	// Texture operation
	void CreateTexture(cMaterial *Material,cRenderDevice *RenderDevice);
	void DeleteTexture(sTexture *Texture);

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

	void SetViewColor(cUnknownClass *UCamera,const sColor4f &Diffuse,const sColor4f &Specular,int zWrite=0);
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

	void CreateTexture565(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture1555(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture4444(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture8888(cMaterial *Material,cRenderDevice *RenderDevice);
	void CreateTexture4444(sTexture *Texture,cRenderDevice *RenderDevice);

	void ResetTextureTileMap565(cMaterial *Material,cRenderDevice *RenderDevice);

	void DrawTilePolyGrid(cRenderDevice *RenderDevice,cCamera *Camera,cBasePolyGrid *WorldPolyGrid,int i1,int j1,int i2,int j2,cMaterial *Material);
};

extern cPolyDispatcher *P3D;

inline int cPolyDispatcher::AssertValid()
{
	return 1;
}

#endif // __POLYMGR_H__
