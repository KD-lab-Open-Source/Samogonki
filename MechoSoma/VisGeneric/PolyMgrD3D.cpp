#ifdef _ONLY_DIRECT3D_
#include "PolyMgr.h"
#include "Camera.h"
#include "object.h"
#include "object3d.h"
#include "RenderDevice.h"
#include "BasePolyGrid.h"
#include "BaseReflection.h"
#include "BaseTrail.h"
#include "Scene.h"
#include "TexMgr.h"

#include "Graph3d_Direct3d.h"
#include "md3d.h"

#ifdef _USE_TILEMAP_
#include "TileMap.h"
#include "TileWater.h"
#endif //_USE_TILEMAP_

#define RDCALL(a)						{ HRESULT hr=a; assert(!FAILED(hr)); }

float gb_LodValue=0.01f;
#define LOD_VALUE						0.01f
//#define LOD_VALUE						gb_LodValue

extern void ResetTextureMultiMaterialSurface565(cInterfaceGraph3d *IGraph3d,cMaterial *Material,cSurfaceReflectionMultiMaterial *Surface);

extern LPDIRECTDRAWSURFACE7 g_pddsFrontBuffer;  // The primary surface
extern LPDIRECTDRAWSURFACE7 g_pddsBackBuffer;   // The backbuffer surface
extern LPDIRECTDRAWSURFACE7 m_pddsZBuffer;      // The zbuffer surface
extern LPDIRECT3DDEVICE7    g_pd3dDevice;       // The D3D device
extern LPDIRECTDRAW7        g_pDD;              // The DirectDraw object
extern cBaseArrayManager <IDirectDrawSurface7>	TextureLibrary;
extern int					gb_CurrentTexture;

__forceinline void cD3DRender_SetTexture(int dwHandle,int dwStage=0)
{
	if(gb_CurrentTexture!=dwHandle)
		RDCALL(g_pd3dDevice->SetTexture(dwStage,TextureLibrary[gb_CurrentTexture=dwHandle]));
}
__forceinline void cD3DRender_SetMaterial(cInterfaceGraph3d *IGraph3d,eMaterialMode material)
{
	cGraph3dDirect3D *Graph3d=(cGraph3dDirect3D*)IGraph3d;
	if(Graph3d->MaterialMode==material) return;
	// восстановление материалов
	if(Graph3d->MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE,FALSE));
	if(Graph3d->MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,FALSE));
	if(Graph3d->MaterialMode&MAT_COLOR_ADD_SPECULAR)
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE,FALSE));
	if(Graph3d->MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA));
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA));
	}
	// установка материалов
	Graph3d->MaterialMode=material;
	if(Graph3d->MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE));
	if(Graph3d->MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE,TRUE));
	if(Graph3d->MaterialMode&MAT_COLOR_ADD_SPECULAR)
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE,TRUE));
	if(Graph3d->MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_ONE));
		RDCALL(g_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE));
	}
	switch(Graph3d->MaterialMode&(MAT_COLOR_MOD_DIFFUSE|MAT_COLOR_MOD_TEXTURE1))
	{
		case MAT_COLOR_MOD_DIFFUSE:
			if(gb_CurrentTexture==0) break;
			d3dSetTextureBlendMode(MD3DTB_DIFFUSE,MD3DTB_DIFFUSE);
			RDCALL(g_pd3dDevice->SetTexture( 0, 0 ));
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 ));
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 ));
			gb_CurrentTexture=0;
			break;
		case MAT_COLOR_MOD_TEXTURE1:
			d3dSetTextureBlendMode(MD3DTB_TEXTURE1,MD3DTB_TEXTURE1);
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 ));
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 ));
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1:
			d3dSetTextureBlendMode(MD3DTB_TEXTURE1_MOD_DIFFUSE,MD3DTB_TEXTURE1_MOD_DIFFUSE);
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE ));
//			RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE ));
			break;
	}
}
__forceinline void cD3DRender_SetMatrix(int type,const MatXf &m,const Vect3f &s=Vect3f(1,1,1))
{
	D3DMATRIX mat;
	mat._11=m.rot()[0][0]*s.x,	mat._12=m.rot()[1][0]*s.y,	mat._13=m.rot()[2][0]*s.z,	mat._14=0;
	mat._21=m.rot()[0][1]*s.x,	mat._22=m.rot()[1][1]*s.y,	mat._23=m.rot()[2][1]*s.z,	mat._24=0;
	mat._31=m.rot()[0][2]*s.x,	mat._32=m.rot()[1][2]*s.y,	mat._33=m.rot()[2][2]*s.z,	mat._34=0;
	mat._41=m.trans().x,		mat._42=m.trans().y,		mat._43=m.trans().z,		mat._44=1;
	RDCALL(g_pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)type,&mat));
}
__forceinline void cD3DRender_SetDrawNode(cCamera *DrawNode,cInterfaceGraph3d *IGraph3d,int flReflection=0)
{
	cGraph3dDirect3D *Graph3d=(cGraph3dDirect3D*)IGraph3d;
	float d=((float)Graph3d->xScr)/Graph3d->yScr;
	float xmin=(DrawNode->GetCenter().x+DrawNode->GetClipping().xmin()*DrawNode->GetFocus().x)*Graph3d->xScr,
		ymin=(DrawNode->GetCenter().y+DrawNode->GetClipping().ymin()*DrawNode->GetFocus().y*d)*Graph3d->yScr,
		xmax=(DrawNode->GetCenter().x+DrawNode->GetClipping().xmax()*DrawNode->GetFocus().x)*Graph3d->xScr,
		ymax=(DrawNode->GetCenter().y+DrawNode->GetClipping().ymax()*DrawNode->GetFocus().y*d)*Graph3d->yScr;
	if(xmin<Graph3d->xScrMin) xmin=Graph3d->xScrMin; if(xmax>=Graph3d->xScrMax) xmax=Graph3d->xScrMax-1;
	if(ymin<Graph3d->yScrMin) ymin=Graph3d->yScrMin; if(ymax>=Graph3d->yScrMax) ymax=Graph3d->yScrMax-1;
	D3DVIEWPORT7 vp={xmin,ymin,xmax-xmin,ymax-ymin,0,1};
	RDCALL(g_pd3dDevice->SetViewport(&vp));
	float dx=((float)(2*Graph3d->xScr))/vp.dwWidth,dy=((float)(2*Graph3d->xScr))/vp.dwHeight;

	D3DMATRIX mat;
	memset(&mat,0,sizeof(D3DMATRIX));
/*
	xv=m11*x+m21*y+m31*z+m41*1;
	yv=m12*x+m22*y+m32*z+m42*1;
	zv=m13*x+m23*y+m33*z+m43*1;
	w =m14*x+m24*y+m34*z+m44*1;
	xe=xv/w=(m11*x+m21*y+m31*z+m41)/(m14*x+m24*y+m34*z+m44);
	ye=yw/w=(m12*x+m22*y+m32*z+m42)/(m14*x+m24*y+m34*z+m44);
	zv=zv/w=(m13*x+m23*y+m33*z+m43)/(m14*x+m24*y+m34*z+m44);
	Perspective:
	xe=(m11*x+m31*z)/(m34*z+m44)	<=>		xe=Center.x+Focus.x*xv/zv
	ye=(m22*y+m32*z)/(m34*z+m44)	<=>		ye=Center.y+Focus.y*yv/zv
	zv=(m33*z+m43)/(m34*z+m44)		<=>		z=zBufferScale.y-(zBufferScale.y-zBufferScale.x)*zPlane.x/zv
	w=m34*z+m44
	xe=(m11*x+m31*z)/(m34*z+m44)=(m11/m34)*x/z+m31/m34	<=>	m44=0, m11/m34=Focus.x, m31/m34=Center.x
	ye=(m22*y+m32*z)/(m34*z+m44)=(m22/m34)*y/z+m32/m34	<=>	m44=0, m22/m34=Focus.y, m32/m34=Center.y
	zv=(m33*z+m43)/(m34*z+m44)=m33/m34+(m43/m34)/z		<=> m33/m34=zBufferScale.y, m43/m34=-(zBufferScale.y-zBufferScale.x)*zPlane.x
	w=m34*z
	let m34=1, then
	xe=m11*x/z+m31	<=>	m11=Focus.x, m31=Center.x
	ye=m22*y/z+m32	<=>	m22=Focus.y, m32=Center.y
	zv=m33+m43/z	<=> m33=zBufferScale.y, m43=-(zBufferScale.y-zBufferScale.x)*zPlane.x
	w=z
*/
	mat._11=+DrawNode->GetFocus().x*dx;
	mat._22=-DrawNode->GetFocus().y*dy;
	mat._31=2*(DrawNode->GetCenter().x*Graph3d->xScr-vp.dwX)/vp.dwWidth-1;
	mat._32=1-2*(DrawNode->GetCenter().y*Graph3d->yScr-vp.dwY)/vp.dwHeight;
	mat._34=1;
	if(flReflection==0)
	{
		mat._33=DrawNode->GetZBufferScale().y;
		mat._43=-(DrawNode->GetZBufferScale().y-DrawNode->GetZBufferScale().x)*DrawNode->GetZPlane().x;
	}
	else
	{		
		mat._33=1;
		mat._43=-(1-DrawNode->GetZBufferScale().y)*DrawNode->GetZPlane().x;
	}
	RDCALL(g_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION,&mat));
	MatXf m=DrawNode->GetMatrix();
	m.rot()[2][0]=-m.rot()[2][0];
	m.rot()[2][1]=-m.rot()[2][1];
	m.rot()[2][2]=-m.rot()[2][2];
	m.trans().z=-m.trans().z;
	cD3DRender_SetMatrix(D3DTRANSFORMSTATE_VIEW,m);
}
void cD3DRender_DrawBound(const MatXf &Matrix,Vect3f &min,Vect3f &max)
{
	sVertexD3D v[8];
	v[0].GetVect3f().set(min.x,min.y,min.z);
	v[1].GetVect3f().set(max.x,min.y,min.z);
	v[2].GetVect3f().set(min.x,max.y,min.z);
	v[3].GetVect3f().set(max.x,max.y,min.z);
	v[4].GetVect3f().set(min.x,min.y,max.z);
	v[5].GetVect3f().set(max.x,min.y,max.z);
	v[6].GetVect3f().set(min.x,max.y,max.z);
	v[7].GetVect3f().set(max.x,max.y,max.z);
	v[0].diffuse()=RGBA_MAKE(150,155,155,64);
	v[1].diffuse()=RGBA_MAKE(170,170,170,64);
	v[2].diffuse()=RGBA_MAKE(170,170,170,64);
	v[3].diffuse()=RGBA_MAKE(200,200,200,64);
	v[4].diffuse()=RGBA_MAKE(170,170,170,64);
	v[5].diffuse()=RGBA_MAKE(200,200,200,64);
	v[6].diffuse()=RGBA_MAKE(200,200,200,64);
	v[7].diffuse()=RGBA_MAKE(255,255,255,64);
	v[0].specular()=v[1].specular()=v[2].specular()=v[3].specular()=v[4].specular()=v[5].specular()=v[6].specular()=v[7].specular()=0;
	sPolygon p[12];
	p[0].p1=1, p[0].p2=2, p[0].p3=0;
	p[1].p1=1, p[1].p2=3, p[1].p3=2;
	p[2].p1=4, p[2].p2=6, p[2].p3=5;
	p[3].p1=6, p[3].p2=7, p[3].p3=5;
	p[4].p1=0, p[4].p2=6, p[4].p3=4;
	p[5].p1=0, p[5].p2=2, p[5].p3=6;
	p[6].p1=0, p[6].p2=4, p[6].p3=5;
	p[7].p1=0, p[7].p2=5, p[7].p3=1;
	p[8].p1=1, p[8].p2=7, p[8].p3=3;
	p[9].p1=1, p[9].p2=5, p[9].p3=7;
	p[10].p1=2, p[10].p2=3, p[10].p3=7;
	p[11].p1=2, p[11].p2=7, p[11].p3=6;
	cD3DRender_SetMatrix(D3DTRANSFORMSTATE_WORLD,Matrix);
	RDCALL(g_pd3dDevice->SetTexture(0,0));
	RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0));  
	RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ));  
	RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG2));
	RDCALL(g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG2));
	RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,v->fmt,v,8,(unsigned short*)&p,3*12,0));
}

int cPolyDispatcher::Draw(cUnknownClass *UCamera,cUnknownClass *URenderDevice,int hTexture,int hLightMap)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cCamera *Camera=(cCamera*)UCamera;
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	if(PolygonFix.length()<=0) return 0;
	sPointAttribute *pa=&PointAttribute[0];
	Vect2f &ScrSize=RenderDevice->GetSize();
	Vect2f Center=Camera->GetCenter()*ScrSize,Focus=Camera->GetFocus()*ScrSize.x/FLOAT_CONST_PREC;	
	Vect2f &zPlane=Camera->GetZPlane(),zBufferScale;
	if((Attribute&RENDER_REFLECTION)==0) zBufferScale=Camera->GetZBufferScale(); else zBufferScale.set(Camera->GetZBufferScale().y,1.f);
//	if((Attribute&RENDER_MULTICANAL)==0)
		for(sVertexFix *bp=&PointFix[0],*ep=&PointFix[PointFix.length()];bp<ep;bp++,pa++)
		{
			bp->xe=Center.x+Focus.x*bp->xe; bp->ye=Center.y+Focus.y*bp->ye;
			bp->w=bp->z*POINT_SCALE_W/FLOAT_CONST_PREC; 
#ifndef _ZBUFFER_LINEAR_
			bp->z=zBufferScale.max()-(zBufferScale.max()-zBufferScale.min())*zPlane.min()*bp->z/FLOAT_CONST_PREC;
#else //_ZBUFFER_LINEAR_
			bp->z=pa->pv.z/65535;
#endif //_ZBUFFER_LINEAR_
//				assert(((zBufferScale.min()-0.0001f)<bp->z)&&(bp->z<(zBufferScale.max()+0.0001f)));
//				assert((0<bp->w)&&(bp->w<1));
		}

	eMaterialMode MaterialMode=eMaterialMode(GET_RENDER_TYPE(Attribute));
	cD3DRender_SetMaterial(Graph3d,MaterialMode);
//	Graph3d->SetMaterial(MaterialMode);
	if((Attribute&RENDER_MULTICANAL)==0)
		Graph3d->PolygonIndexed(&PolygonFix[0],PolygonFix.length(),&PointFix[0],PointFix.length());
	else
		Graph3d->PolygonIndexed2(&PolygonFix[0],PolygonFix.length(),&PointFix[0],PointFix.length(),hTexture,hLightMap);
	return 1;
}

void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cOmni *Omni)
{
	float size=Omni->GetRadius();
	if(size<=1) return;
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
		if(Omni->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
		{
			assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
			cCamera *Camera=(cCamera*)CameraArray[nCamera];
			cRenderDevice *RenderDevice=Camera->GetViewPort();
			cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
			SetClippingPlane(Camera);
			// подготовка к расчету координат объекта
			cConvertor ConvertorObjectToScreen;
			Omni->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE);
			ConvertorObjectToScreen.BuildMatrix();
			ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
			int NumberAngle=(int)(size/50)+8,NumberPlane=(int)(size/40)+3;
			if(NumberAngle>16) NumberAngle=16; if(NumberPlane>16) NumberPlane=16;
			float KoofAttenuation=1.2f/(NumberPlane-1);
			int rc=Omni->GetColor().GetR(),gc=Omni->GetColor().GetG(),bc=Omni->GetColor().GetB();
			rc*=KoofAttenuation; gc*=KoofAttenuation; bc*=KoofAttenuation;
			float limit=(float)size*(NumberPlane-1)/NumberPlane, dlimit=(float)size/NumberPlane;
			int RenderAttribute=MAT_COLOR_ADD_DIFFUSE|MAT_ALPHA_MOD_DIFFUSE|RENDER_COLOR_MOD_DIFFUSE;
			if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
			InitFix(RenderAttribute,round((NumberAngle+1)*(limit+limit+dlimit)/dlimit));
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
			int CurrentPoint=0,PreviousPoints=0;
			Vect3f pw(0,0,0),pv,pe;
			ConvertorObjectToScreen.ConvertPoint(pw,pv,pe);
			for(float height=+limit;height>=-limit;height-=dlimit)
			{
				PreviousPoints=CurrentPoint;
				float radius=size;
				if(height>0) radius=size-height; else radius=size+height; 
				if((pv.z+height)<=Camera->GetZPlane().x) continue;
				float div_zv;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) div_zv=1/(pv.z+height); 
				else div_zv=1/Camera->GetPos().z;
				Vect3f pv1(pv.x,pv.y,pv.z+height);
				Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
				SetPointFix(CurrentPoint++,pe1,rc,gc,bc,255,pv1);
				for(int k=1;k<(NumberAngle+1);k++)
				{
					int angle=(k-1)*GRAD_TO_DGRAD(360)/NumberAngle;
					Vect3f pv1(pv.x+size*COS_FLOAT_DGRAD(angle),pv.y+size*SIN_FLOAT_DGRAD(angle),pv.z+height);
					Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
					SetPointFix(CurrentPoint++,pe1,0,0,0,255,pv1);
				}
				AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+1,PreviousPoints+NumberAngle);
				for(int i=1;i<NumberAngle;i++)
					AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+i+1,PreviousPoints+i);
			}
			Draw(Camera,RenderDevice);
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
		}
}
inline int GetAttributePoint(int xg,int yg,int zg)
{ 
	int Attribute=ATTRIBUTE_LIGHT;
#ifdef _MECHOSOMA_
	extern int acsShopFlag;
	if(!acsShopFlag) 
	{
		int xv=XCYCL(xg), yv=YCYCL(yg), zv=zg;
		unsigned char attribute=GetAt(xv,yv);
		int zMap=GetAlt(xv,yv);
		if(((attribute&At_SHADOW)&&((zMap+Z_SHADE_MAX)>zv))||(zMap>zv))
			Attribute|=ATTRIBUTE_SHADOW;
	}
#elif _SURMAP_
		int xv=XCYCL(xg), yv=YCYCL(yg), zv=zg;
		unsigned char attribute=GetAt(xv,yv);
		int zMap=GetAlt(xv,yv);
		if((attribute&At_WATER)&&(vMap->LevelH2O>zv))
			Attribute|=ATTRIBUTE_WATER;
		if(((attribute&At_SHADOW)&&((zMap+Z_SHADE_MAX)>zv))||(zMap>zv))
			Attribute|=ATTRIBUTE_SHADOW;
#endif 
	return Attribute;
}
__forceinline void SetDiffuse(sVertexD3D &v,cLight *LightObject,int RenderShade,sColor4s &AmbientMesh,sColor4s &MulTile,cConvertor &ConvertorObjectToWorld,Vect3f &vLight,sPoint *Point)
{
	int ToneDiffuse=LightObject->CalcToneDiffuse(Point->normal,vLight);
	if(RenderShade)
	{
		Vect3f pw;
		ConvertorObjectToWorld.ConvertPoint(Point->pos,pw);
		if(GetAttributePoint(round(pw.x),round(pw.y),round(pw.z))&ATTRIBUTE_SHADOW)
			ToneDiffuse>>=1;
	}
	assert(0<=ToneDiffuse&&ToneDiffuse<=255);
	int dr=((ToneDiffuse*MulTile.r)>>8)+AmbientMesh.r; 
	int dg=((ToneDiffuse*MulTile.g)>>8)+AmbientMesh.g; 
	int db=((ToneDiffuse*MulTile.b)>>8)+AmbientMesh.b; 
	if(dr>255) dr=255; if(dg>255) dg=255; if(db>255) db=255; 
	v.diffuse()=RGBA_MAKE(dr,dg,db,MulTile.a);
}
__forceinline void SetSpecular(sVertexD3D &v,cLight *LightObject,Vect3f &vLight,Vect3f &vView,float cosLightView,
						sColor4s &AddTile,sColor4s &MetalColor,sPoint *Point)
{
	int ToneSpecular=LightObject->CalcToneMetal(Point->normal,vLight,vView,cosLightView);
	assert(0<=ToneSpecular&&ToneSpecular<=255);
	int ar=AddTile.r+((ToneSpecular*MetalColor.r)>>7);
	int ag=AddTile.g+((ToneSpecular*MetalColor.g)>>7);
	int ab=AddTile.b+((ToneSpecular*MetalColor.b)>>7);
	if(ar>255) ar=255; if(ag>255) ag=255; if(ab>255) ab=255;
	v.specular()=RGBA_MAKE(ar,ag,ab,255);
}
__forceinline void SetPointMeshD3D(sVertexD3D &v,cLight *LightObject,
						cConvertor &ConvertorObjectToWorld,
						Vect3f &vLight,Vect3f &vView,float cosLightView,
						int RenderDiffuse,int RenderMetal,int RenderShade,
						sColor4s &AmbientMesh,sColor4s &MulTile,sColor4s &AddTile,sColor4s &MetalColor,
						sPoint *Point)
{
	int dr,dg,db;
	if(RenderDiffuse)
	{
		int ToneDiffuse=LightObject->CalcToneDiffuse(Point->normal,vLight);
		if(RenderShade)
		{
			Vect3f pw;
			ConvertorObjectToWorld.ConvertPoint(Point->pos,pw);
			if(GetAttributePoint(round(pw.x),round(pw.y),round(pw.z))&ATTRIBUTE_SHADOW)
				ToneDiffuse>>=1;
		}
		assert(0<=ToneDiffuse&&ToneDiffuse<=255);
		if((dr=((ToneDiffuse*MulTile.r)>>8)+AmbientMesh.r)>255) dr=255; 
		if((dg=((ToneDiffuse*MulTile.g)>>8)+AmbientMesh.g)>255) dg=255; 
		if((db=((ToneDiffuse*MulTile.b)>>8)+AmbientMesh.b)>255) db=255;
	} 
	else
	{
		if((dr=MulTile.r+AmbientMesh.r)>255) dr=255; 
		if((dg=MulTile.g+AmbientMesh.g)>255) dg=255; 
		if((db=MulTile.b+AmbientMesh.b)>255) db=255;
	}
	v.diffuse()=RGBA_MAKE(dr,dg,db,MulTile.a);
	if(RenderMetal)
	{
		int ToneSpecular=LightObject->CalcToneMetal(Point->normal,vLight,vView,cosLightView),ar,ag,ab;
		assert(0<=ToneSpecular&&ToneSpecular<=255);
		if((ar=AddTile.r+((ToneSpecular*MetalColor.r)>>7))>255) ar=255;
		if((ag=AddTile.g+((ToneSpecular*MetalColor.g)>>7))>255) ag=255;
		if((ab=AddTile.b+((ToneSpecular*MetalColor.b)>>7))>255) ab=255;
		v.specular()=RGBA_MAKE(ar,ag,ab,255);
	}
	else 
		v.specular()=RGBA_MAKE(AddTile.r,AddTile.g,AddTile.b,255);
}
void cPolyDispatcher::Draw(cUnknownClass *UScene,cUnknownClass *UCameraList,cMesh *Mesh,Vect3f *vReflection)
{
	assert(UScene->GetKind(KIND_SCENE));
	cScene *Scene=(cScene*)UScene;
	cLight *LightObject=Scene->GetLight();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;

	// расчет освещенности объекта 
	int RenderLighting=GET_RENDER_TUNING(RENDER_TUNING_DYNAMIC_LIGHTING)&&(Mesh->GetAttribute(MESH_NOT_LIGHTING)==0);
	sColor4s AmbientMesh,MulMesh,AddMesh,MetalColor(255,255,255,255);
	int RenderDiffuse;
	if(RenderLighting)
	{ // расчет освещенности для объекта по источникам света сцены
		sColor4f Ambient,Diffuse,Illumination=Mesh->GetSpecular();
		if(IS_STATIC(Mesh->Type)) Ambient.set(0,0,0,1),Diffuse.set(0,0,0,1);
		else Ambient=LightObject->GetAmbient(),Diffuse=LightObject->GetDiffuse();
		if((Mesh->GetAttribute(MESH_LIGHTING_MAX)==0)&&((GlobalRenderTuning&RENDER_TUNING_OBJECT_LIGHTING_CONST)==0))
		{
			sColor4f OmniIllumination(0,0,0,1);
			Scene->GetOmniLight(Mesh->GlobalMatrix.trans(),Diffuse,OmniIllumination); 
			MetalColor.set(OmniIllumination.GetR(),OmniIllumination.GetG(),OmniIllumination.GetB(),255);
			if(!IS_STATIC(Mesh->Type)) Diffuse+=Scene->GetAmbientObjectAll();
			if(M3D_TYPE(Mesh->Type)&(M3D_ENGINE|M3D_FRONT|M3D_RB_WHEEL|M3D_LB_WHEEL|M3D_RF_WHEEL|M3D_LF_WHEEL))
				Diffuse+=Scene->GetAmbientObjectMechos();
		}
		else
		{ Diffuse+=Scene->GetDiffuseLightingMax(); Ambient+=Scene->GetAmbientLightingMax(); }
		Diffuse*=Mesh->GetDiffuse(); // Diffuse=(DiffuseLight+...+DiffuseOmni)*DiffuseMesh+(AmbientLight+...+AmbientOmni)
		AmbientMesh.set(Ambient.GetR(),Ambient.GetG(),Ambient.GetB(),255);
		MulMesh.set(Diffuse.GetR(),Diffuse.GetG(),Diffuse.GetB(),Mesh->GetDiffuse().GetA());
		AddMesh.set(Illumination.GetR(),Illumination.GetG(),Illumination.GetB(),255);
		RenderDiffuse=1;
	}
	else
	{ // на объект не действуют источники света, не происходит рендер освещения
		AmbientMesh.set(0,0,0,255);
		MulMesh.set(Mesh->GetDiffuse().GetR(),Mesh->GetDiffuse().GetG(),Mesh->GetDiffuse().GetB(),Mesh->GetDiffuse().GetA());
		AddMesh.set(Mesh->GetSpecular().GetR(),Mesh->GetSpecular().GetG(),Mesh->GetSpecular().GetB(),255);
		RenderDiffuse=0;
	}
	// подготовка к расчету освещенности объекта
	cConvertor ConvertorObjectToWorld;
	ConvertorObjectToWorld.GetMatrix()=Mesh->GlobalMatrix;
//	ConvertorObjectToWorld.BuildMatrix();
	Vect3f vView,vLight;
	ConvertorObjectToWorld.InverseConvertVector(LightObject->GetTangent(),vLight);
	vLight.normalize(1.f);
	int PolygonOrientationCCW=(vReflection==0);

	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
		if(Mesh->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
		{
			assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
			cCamera *Camera=(cCamera*)CameraArray[nCamera];
			if(Camera->GetAttribute(ATTRIBUTE_CAMERA_MASK_A|ATTRIBUTE_CAMERA_MASK_B)&Mesh->Attribute)
				continue;
			cRenderDevice *RenderDevice=Camera->GetViewPort();
			cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
			SetClippingPlane(Camera);
			sColor4s MetalTile=MetalColor;

			// подготовка к расчету освещенности объекта - расчет направления камеры
			int Alpha=MulMesh.a;
			if(Camera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_SHARE))
			{
				int dx=GetShortDistX(round(Camera->GetPos().x),round(Mesh->GlobalMatrix.trans().x)),
					dy=GetShortDistY(round(Camera->GetPos().y),round(Mesh->GlobalMatrix.trans().y));
				int l=(dx*dx+dy*dy)-512*512;
				if(l>0)
				{
					int da=255-255*l/(3*512*512);
					if(da<0) da=0;
					Alpha=(Alpha*da)>>8;
					MetalTile.r=(MetalTile.r*da)>>8;
					MetalTile.g=(MetalTile.g*da)>>8;
					MetalTile.b=(MetalTile.b*da)>>8;
				}
			}
			Vect3f dPos;
			Camera->GetMatrix().invXformVect(Camera->GetMatrix().trans(),dPos);
			dPos.negate();
			dPos.x=GetShortDistX(dPos.x,Camera->GetPos().x,GlobalWorldSize.x);
			dPos.y=GetShortDistY(dPos.y,Camera->GetPos().y,GlobalWorldSize.y);
			dPos.x+=GetShortDistX(Camera->GetPos().x,Mesh->GlobalMatrix.trans().x,GlobalWorldSize.x);
			dPos.y+=GetShortDistY(Camera->GetPos().y,Mesh->GlobalMatrix.trans().y,GlobalWorldSize.y);
			dPos.z=dPos.z-Mesh->GlobalMatrix.trans().z;
			ConvertorObjectToWorld.InverseConvertVector(dPos,vView);
			vView.normalize(1.f);
			float cosLightView=dot(vLight,vView);
			// подготовка к расчету координат объекта
			if(vReflection==0)
			{
				MatXf DrawMatrix=Mesh->BuildDrawMatrix(Camera,DrawMatrix);
				cD3DRender_SetMatrix(D3DTRANSFORMSTATE_WORLD,DrawMatrix,Mesh->Scale);
				cD3DRender_SetDrawNode(Camera,Graph3d);
			}
			else 
			{
				if((GlobalRenderTuning&RENDER_TUNING_OBJECT_REFLECTION_MAP)&&Scene->GetTileMap())
				{
					Vect3f &v=Mesh->GlobalMatrix.trans();
					Vect3f PointView;
					Camera->GetMatrix().invXformVect(Camera->GetMatrix().trans(),PointView);
					PointView=-PointView;
					Vect3f VectorCenterToView(PointView.x-Camera->GetPos().x,PointView.y-Camera->GetPos().y,PointView.z+::GlobalWorldRadius);
					float RadiusReflectionIce=::GlobalWorldRadius+Scene->GetTileMap()->GetZReflectionSurface();
					float DistGorizont2=VectorCenterToView.norm2()-RadiusReflectionIce*RadiusReflectionIce;
					if(DistGorizont2<0) continue;
					float dx=GetShortDistX(v.x,PointView.x),dy=GetShortDistX(v.y,PointView.y);
					if((dx*dx+dy*dy)>DistGorizont2) continue;
					float f=(dx*dx+dy*dy)/DistGorizont2-0.8f;
					if(f>0)
					{
						int da=round(255*(1-f/(1-0.8f)));
						if(da<0) da=0; else if(da>255) da=255;
						Alpha=(Alpha*da)>>8;
						MetalTile.r=(MetalTile.r*da)>>8;
						MetalTile.g=(MetalTile.g*da)>>8;
						MetalTile.b=(MetalTile.b*da)>>8;
					}
				}
				MatXf DrawMatrix=Mesh->BuildDrawReflectionMatrix(Camera,DrawMatrix,*vReflection);
				cD3DRender_SetMatrix(D3DTRANSFORMSTATE_WORLD,DrawMatrix,Mesh->Scale);
				cD3DRender_SetDrawNode(Camera,Graph3d,1);
				Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_CCW);
			}
			if(Mesh->GetAttribute(MESH_NOT_WRITEZBUFFER))
				Graph3d->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
			for(int i=0;i<Mesh->GetNumberTile();i++)
			{ // собственно расчет координат и освещенности
				sTile *tile=Mesh->GetTile(i);
				sPoint	*Point=tile->GetPoint();
				sPolygon *Polygon=tile->GetPolygon();
				if(tile->GetAttribute(ATTRMAT_TEXTURE_PAL)) 
				{
					if(tile->Texture->nTexture==0) CreateTexture((cMaterial*)tile,RenderDevice);
					cD3DRender_SetTexture(tile->Texture->nTexture); // установка текстуры
				}
				sColor4s MulTile(round(tile->MulCol.r*MulMesh.r),round(tile->MulCol.g*MulMesh.g),
					round(tile->MulCol.b*MulMesh.b),round(Alpha*tile->MulCol.a)),
					AddTile(AddMesh.r+tile->AddCol.GetR(),AddMesh.g+tile->AddCol.GetG(),AddMesh.b+tile->AddCol.GetB(),255);
				if(AddTile.r>255) AddTile.r=255;
				if(AddTile.g>255) AddTile.g=255;
				if(AddTile.b>255) AddTile.b=255;
				// инициализация буффера диспетчера растеризации
				int AttributeTile=MATERIAL_ATTRIBUTE(tile->GetAttribute()|Mesh->Attribute);
//				int RenderMetal=(GET_RENDER_TUNING(RENDER_TUNING_METAL)&&((Attribute&MESH_NOT_METALL)==0)&&(tile->GetAttribute(ATTRMAT_METAL)))&&RenderLighting;
				int RenderMetal=(GET_RENDER_TUNING(RENDER_TUNING_METAL)&&((Attribute&MESH_NOT_METALL)==0)&&(tile->GetAttribute(ATTRMAT_METAL)));
				int RenderShade=((Attribute&MESH_NOT_SHADE)==0);
				int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
				if(vReflection)
					BaseAttribute|=RENDER_REFLECTION;
				if((RenderMetal)||(AddTile.r|AddTile.g|AddTile.b))
					BaseAttribute|=RENDER_COLOR_ADD_SPECULAR; 
				if(AttributeTile&ATTRMAT_MASK)
					BaseAttribute|=RENDER_ALPHA_MASK_TEXTURE1; 
				if(AttributeTile&ATTRMAT_OPACITY)
					BaseAttribute|=RENDER_ALPHA_MOD_TEXTURE1;
				if(AttributeTile&ATTRMAT_TRANSPARENCY||MulTile.a!=255) 
					BaseAttribute|=RENDER_ALPHA_MOD_DIFFUSE;
				if(MulTile.a<2) continue;
				if(AttributeTile&ATTRMAT_ALFABLEND)
					BaseAttribute|=RENDER_ALPHA_MOD_DIFFUSE;
				if(AttributeTile&ATTRMAT_TEXTURE_PAL)		
					BaseAttribute|=RENDER_COLOR_MOD_TEXTURE1;
				eMaterialMode MaterialMode=eMaterialMode(GET_RENDER_TYPE(BaseAttribute));
//				Graph3d->SetMaterial(MaterialMode);
				cD3DRender_SetMaterial(Graph3d,MaterialMode);

				if(tile->Attribute.GetAttribute()==ATTRTILE_VERTEX)
				{
					VertexD3D.length()=tile->GetNumberPoint();
					for(int i=0;i<tile->GetNumberPoint();i++)
					{
						VertexD3D[i].GetVect3f()=Point[i].pos;
						SetPointMeshD3D(VertexD3D[i],LightObject,ConvertorObjectToWorld,
							vLight,vView,cosLightView,RenderDiffuse,RenderMetal,RenderShade,
							AmbientMesh,MulTile,AddTile,MetalTile,&Point[i]);
					}
					RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,sVertexD3D::fmt,&VertexD3D[0],tile->GetNumberPoint(),
						(unsigned short*)&Polygon[0],3*tile->GetNumberPolygon(),0));
				}
				else if(tile->Attribute.GetAttribute()==ATTRTILE_VERTEX_TEXEL)
				{
					sTexel	*Texel=tile->GetTexel();
					VertexD3D.length()=tile->GetNumberPoint();
					for(int i=0;i<tile->GetNumberPoint();i++)
					{
						VertexD3D[i].GetVect3f()=Point[i].pos;
						SetPointMeshD3D(VertexD3D[i],LightObject,ConvertorObjectToWorld,
							vLight,vView,cosLightView,RenderDiffuse,RenderMetal,RenderShade,
							AmbientMesh,MulTile,AddTile,MetalTile,&Point[i]);
						VertexD3D[i].u1()=Texel[i].u(); VertexD3D[i].v1()=Texel[i].v();
					}
					RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,sVertexD3D::fmt,&VertexD3D[0],tile->GetNumberPoint(),
						(unsigned short*)&Polygon[0],3*tile->GetNumberPolygon(),0));
//					cD3DRender_DrawBound(Mesh->GlobalMatrix,Vect3f(-100,-100,-100),Vect3f(100,100,100));
//					cD3DRender_DrawBound(MatXf(Mat3f::ID,Vect3f(-500,-500,0)),Vect3f(-100,-100,-100),Vect3f(100,100,100));
				}
				else if(tile->Attribute.GetAttribute()==ATTRTILE_TEXEL)
				{
					sTexel	*Texel=tile->GetTexel();
					sPolygon *TexPoly=tile->GetTexPoly();
					assert((PointFix.MaxSize-tile->GetNumberPoint()-1)>0);
					unsigned char *BufColor=(unsigned char*)&PointFix[0];

					for(int i=0;i<tile->GetNumberPoint();i++)
					{
						int i8=i*8;
						BufColor[i8+3]=MulTile.a; BufColor[i8+7]=0;
						if(RenderDiffuse)
						{
							int ToneDiffuse=LightObject->CalcToneDiffuse(Point[i].normal,vLight);
							if(RenderShade)
							{
								Vect3f pw;
								ConvertorObjectToWorld.ConvertPoint(Point[i].pos,pw);
								if(GetAttributePoint(round(pw.x),round(pw.y),round(pw.z))&ATTRIBUTE_SHADOW)
									ToneDiffuse>>=1;
							}
							assert(0<=ToneDiffuse&&ToneDiffuse<=255);
							int dr=AmbientMesh.r+((ToneDiffuse*MulTile.r)>>8);
							int dg=AmbientMesh.g+((ToneDiffuse*MulTile.g)>>8);
							int db=AmbientMesh.b+((ToneDiffuse*MulTile.b)>>8);
							if(dr<255) BufColor[i8+2]=dr; else BufColor[i8+2]=255; 
							if(dg<255) BufColor[i8+1]=dg; else BufColor[i8+1]=255; 
							if(db<255) BufColor[i8+0]=db; else BufColor[i8+0]=255; 
						}
						else
						{
							int dr=AmbientMesh.r+MulTile.r;
							int dg=AmbientMesh.g+MulTile.g;
							int db=AmbientMesh.b+MulTile.b;
							if(dr<255) BufColor[i8+2]=dr; else BufColor[i8+2]=255; 
							if(dg<255) BufColor[i8+1]=dg; else BufColor[i8+1]=255; 
							if(db<255) BufColor[i8+0]=db; else BufColor[i8+0]=255; 
						}
						if(RenderMetal)
						{
							int ToneSpecular=LightObject->CalcToneMetal(Point[i].normal,vLight,vView,cosLightView);
							assert(0<=ToneSpecular&&ToneSpecular<=255);
							int ar=AddTile.r+((ToneSpecular*MetalTile.r)>>7);
							int ag=AddTile.g+((ToneSpecular*MetalTile.g)>>7);
							int ab=AddTile.b+((ToneSpecular*MetalTile.b)>>7);
							if(ar<255) BufColor[i8+6]=ar; else BufColor[i8+6]=255;
							if(ag<255) BufColor[i8+5]=ag; else BufColor[i8+5]=255;
							if(ab<255) BufColor[i8+4]=ab; else BufColor[i8+4]=255;
						}
						else
							BufColor[i8+6]=AddTile.r,
							BufColor[i8+5]=AddTile.g,
							BufColor[i8+4]=AddTile.b;
					}

					VertexD3D.length()=3*tile->GetNumberPolygon();
					PolygonFix.length()=tile->GetNumberPolygon();
					for(i=0;i<tile->GetNumberPolygon();i++)
					{	// передача геометрических координат
						int i3=i*3;
						Vect3f pv,pe;
						sPolygon &p=Polygon[i];
						sPolygon &t=TexPoly[i];
						VertexD3D[i3+0].GetVect3f()=Point[p.p1].pos;
						VertexD3D[i3+0].diffuse()=*(int*)&BufColor[p.p1*8+0];
						VertexD3D[i3+0].specular()=*(int*)&BufColor[p.p1*8+4];
						VertexD3D[i3+0].GetTexel().set(Texel[t.p1].u(),Texel[t.p1].v());
						VertexD3D[i3+1].GetVect3f()=Point[p.p2].pos;
						VertexD3D[i3+1].diffuse()=*(int*)&BufColor[p.p2*8+0];
						VertexD3D[i3+1].specular()=*(int*)&BufColor[p.p2*8+4];
						VertexD3D[i3+1].GetTexel().set(Texel[t.p2].u(),Texel[t.p2].v());
						VertexD3D[i3+2].GetVect3f()=Point[p.p3].pos;
						VertexD3D[i3+2].diffuse()=*(int*)&BufColor[p.p3*8+0];
						VertexD3D[i3+2].specular()=*(int*)&BufColor[p.p3*8+4];
						VertexD3D[i3+2].GetTexel().set(Texel[t.p3].u(),Texel[t.p3].v());
						PolygonFix[i].p1=i3+0; PolygonFix[i].p2=i3+1; PolygonFix[i].p3=i3+2;
					}
					RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,sVertexD3D::fmt,&VertexD3D[0],VertexD3D.length(),
						(unsigned short*)&PolygonFix[0],3*tile->GetNumberPolygon(),0));
				}
			}
			if(Mesh->GetAttribute(MESH_NOT_WRITEZBUFFER))
				Graph3d->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
			if(vReflection)
				Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_CW);
		}
}

void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cTileMap *TileMap)
{
#ifdef _USE_TILEMAP_
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cRenderDevice *RenderDevice=Camera->GetViewPort();
		cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_CLAMP);
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;

		if(TileMap->GetZReflectionSurface()>=0)
			for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
				if(bTile->GetAttribute(ATTR_TILE_SURFACE_REFLECTION))
					if(bTile->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
					{
						if(bTile->GetTexture()->GetStatus(STATUS_TEXTURE_RESET|STATUS_TEXTURE_LOAD))
							ResetTextureTileMap565(bTile->GetMaterial(),RenderDevice);
						Graph3d->SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
						Vect3f ShareOfs;
						cConvertor ConvertorObjectToScreen;
						ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*bTile->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),ShareOfs);
						ConvertorObjectToScreen.BuildMatrix();
						ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
						cBaseDynArray <sPointTile> &Point=bTile->PointReflection;
						cBaseDynArray <sPolygonFix> &Polygon=bTile->PolygonReflection;
						int AttributeRender=RENDER_COLOR_MOD_TEXTURE1|RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
						if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) AttributeRender|=RENDER_CLIPPING3D;
						InitFix(AttributeRender,Point.length());
						int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
						for(int i=0;i<Point.length();i++)
						{
							sPointTile &p=Point[i];
							int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
							float z=(float)p.zw+(float)p.dz;
							if(FlagShareWorld)
							{
								int xr=xofs+x, yr=yofs+y;
								z-=SHARE_FLOAT(xr*xr+yr*yr);
							}
							Vect3f po((float)x,(float)y,(float)z);
							ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
							SetPointFix(i,255,255,255,p.da(),Vect2f(p.u,p.v));
						}
						for(i=0;i<Polygon.length();i++)
							AddPolygonFixTestPointFix(Polygon[i].p1,Polygon[i].p2,Polygon[i].p3);	
						if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
						{
							if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
							{
//								Graph3d->SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
//								this->Attribute=RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_MULTICANAL;
								this->Attribute|=RENDER_MULTICANAL;
								Draw(Camera,RenderDevice,bTile->GetTexture()->nTexture,bTile->GetLightMap()->nTexture);
							}
							else
								Draw(Camera,RenderDevice);
						}
						else
							Draw(Camera,RenderDevice);
					}
		for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
			if(bTile->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
			{
				Graph3d->SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
				Vect3f ShareOfs;
				cConvertor ConvertorObjectToScreen;
				ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*bTile->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),ShareOfs);
				ConvertorObjectToScreen.BuildMatrix();
				ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
				cBaseDynArray <sPointTile> &Point=bTile->Point;
				cBaseDynArray <sPolygonFix> &Polygon=bTile->Polygon;
				int AttributeRender=RENDER_COLOR_MOD_TEXTURE1;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) AttributeRender|=RENDER_CLIPPING3D;
				InitFix(AttributeRender,Point.length());
				int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
				for(int i=0;i<Point.length();i++)
				{
					sPointTile &p=Point[i];
					int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
					float z=(float)((int)p.zw+(int)p.dz);
					if(FlagShareWorld)
					{
						int xr=xofs+x, yr=yofs+y;
						z-=SHARE_FLOAT(xr*xr+yr*yr);
					}
					Vect3f po((float)x,(float)y,(float)z);
					ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
					SetPointFix(i,255,255,255,255,Vect2f(p.u,p.v));
//					ConvertorObjectToScreen.ConvertPoint(po,pv,pe);
//					SetPointFix(i,pe,255,255,255,p.da(),Vect2f(p.u,p.v),pv);
				}
				for(i=0;i<Polygon.length();i++)
					AddPolygonFixTestPointFix(Polygon[i].p1,Polygon[i].p2,Polygon[i].p3);	
				if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
				{
					if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
					{
						Graph3d->SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
//								this->Attribute=RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_MULTICANAL;
						this->Attribute|=RENDER_MULTICANAL;
						Draw(Camera,RenderDevice,bTile->GetTexture()->nTexture,bTile->GetLightMap()->nTexture);
					}
					else
						Draw(Camera,RenderDevice);
				}
				else
					Draw(Camera,RenderDevice);
			}
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_WRAP);
	}
	for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
		bTile->GetLightMap()->ClearStatus(STATUS_TEXTURE_SHADOW);
#endif //_USE_TILEMAP_
}
/*
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cTileMap *TileMap)
{
#ifdef _USE_TILEMAP_
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cRenderDevice *RenderDevice=Camera->GetViewPort();
		cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_CLAMP);
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;
		cD3DRender_SetDrawNode(Camera,Graph3d);

		if(TileMap->GetZReflectionSurface()>=0)
			for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
				if(bTile->GetAttribute(ATTR_TILE_SURFACE_REFLECTION))
					if(bTile->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
					{
						if(bTile->GetTexture()->GetStatus(STATUS_TEXTURE_RESET|STATUS_TEXTURE_LOAD))
							ResetTextureTileMap565(bTile->GetMaterial(),RenderDevice);
						Vect3f ShareOfs;
						cD3DRender_SetMaterial(Graph3d,(eMaterialMode)(MAT_COLOR_MOD_TEXTURE1|MAT_COLOR_MOD_DIFFUSE|MAT_ALPHA_MOD_DIFFUSE));
						cD3DRender_SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
						MatXf DrawMatrix=bTile->BuildDrawMatrix(Camera,DrawMatrix,ShareOfs);
						cD3DRender_SetMatrix(D3DTRANSFORMSTATE_WORLD,DrawMatrix);
						cBaseDynArray <sPointTile> &Point=bTile->PointReflection;
						cBaseDynArray <sPolygonFix> &Polygon=bTile->PolygonReflection;
						VertexD3D.length()=Point.length();
						int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
						for(int i=0;i<Point.length();i++)
						{
							sPointTile &p=Point[i];
							int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
							float z=(float)p.zw+(float)p.dz;
							if(FlagShareWorld)
							{
								int xr=xofs+x, yr=yofs+y;
								z-=SHARE_FLOAT(xr*xr+yr*yr);
							}
							VertexD3D[i].GetVect3f().set(x,y,z);
							VertexD3D[i].GetTexel().set(p.u,p.v);
							VertexD3D[i].diffuse()=RGBA_MAKE(255,255,255,p.da());
						}
						if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW)&&bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
							d3dTrianglesIndexed2(sVertexD3D::fmt,(void*)&VertexD3D[0],VertexD3D.length(),
								(unsigned short*)&Polygon[0],3*Polygon.length(),
								bTile->GetTexture()->nTexture,bTile->GetLightMap()->nTexture);
						else
							RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,sVertexD3D::fmt,&VertexD3D[0],VertexD3D.length(),
								(unsigned short*)&Polygon[0],3*Polygon.length(),0));
					}
		for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
			if(bTile->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
			{
				Vect3f ShareOfs;
				cBaseDynArray <sPointTile> &Point=bTile->Point;
				cBaseDynArray <sPolygonFix> &Polygon=bTile->Polygon;
				if(Polygon.length()<=0) continue;
				cD3DRender_SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
				cD3DRender_SetMaterial(Graph3d,(eMaterialMode)(MAT_COLOR_MOD_TEXTURE1));
				MatXf DrawMatrix=bTile->BuildDrawMatrix(Camera,DrawMatrix,ShareOfs);
				cD3DRender_SetMatrix(D3DTRANSFORMSTATE_WORLD,DrawMatrix);
				VertexD3D.length()=Point.length();
				int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
				for(int i=0;i<Point.length();i++)
				{
					sPointTile &p=Point[i];
					int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
					float z=(float)((int)p.zw+(int)p.dz);
					if(FlagShareWorld)
					{
						int xr=xofs+x, yr=yofs+y;
						z-=SHARE_FLOAT(xr*xr+yr*yr);
					}
					VertexD3D[i].GetVect3f().set(x,y,z);
					VertexD3D[i].GetTexel().set(p.u,p.v);
					VertexD3D[i].diffuse()=RGBA_MAKE(255,255,255,255);
				}
				if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW)&&bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
					d3dTrianglesIndexed2(sVertexD3D::fmt,(void*)&VertexD3D[0],VertexD3D.length(),
						(unsigned short*)&Polygon[0],3*Polygon.length(),
						bTile->GetTexture()->nTexture,bTile->GetLightMap()->nTexture);
				else
					RDCALL(g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,sVertexD3D::fmt,&VertexD3D[0],VertexD3D.length(),
						(unsigned short*)&Polygon[0],3*Polygon.length(),0));
			}
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_WRAP);
	}
	for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
		bTile->GetLightMap()->ClearStatus(STATUS_TEXTURE_SHADOW);
#endif //_USE_TILEMAP_
}
*/
void cPolyDispatcher::DrawReflection(cUnknownClass *UCameraList,cTileMap *TileMap)
{
#ifdef _USE_TILEMAP_
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	if(TileMap->GetZReflectionSurface()<0) return;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cRenderDevice *RenderDevice=Camera->GetViewPort();
		cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_CLAMP);
		SetClippingPlane(Camera);
		SetViewColor(Camera,sColor4f(0,0,0,1),sColor4f(0,0,0,1));
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;

		Vect3f PointView;
		Camera->GetMatrix().invXformVect(Camera->GetMatrix().trans(),PointView);
		PointView=-PointView;
		Vect3f VectorCenterToView(PointView.x-Camera->GetPos().x,PointView.y-Camera->GetPos().y,PointView.z+::GlobalWorldRadius);
		float RadiusReflectionIce=::GlobalWorldRadius+TileMap->GetZReflectionSurface();
		float DistGorizont2=VectorCenterToView.norm2()-RadiusReflectionIce*RadiusReflectionIce;
		if(DistGorizont2<0) continue;
		float DistGorizont=sqrt(DistGorizont2);
		Vect3f pEye(PointView.x-Camera->GetPos().x,PointView.y-Camera->GetPos().y,PointView.z);
		float zEyeHint=pEye.z*(1-pEye.z*0.002f);

		for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
			if(bTile->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)
				if(bTile->GetAttribute(ATTR_TILE_REFLECTION))
				{
					cD3DRender_SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
					Vect3f ShareOfs;
					cConvertor ConvertorObjectToScreen;
					ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*bTile->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),ShareOfs);
					ConvertorObjectToScreen.BuildMatrix();
					ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
					cBaseDynArray <sPointTile> &Point=bTile->Point;
					cBaseDynArray <sPolygonFix> &Polygon=bTile->Polygon;
					int BaseAttribute=RENDER_REFLECTION|RENDER_COLOR_MOD_TEXTURE1;
					if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
					if(bTile->GetAttribute(ATTR_TILE_SURFACE_REFLECTION)) BaseAttribute|=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
					InitFix(BaseAttribute,Point.length());
					int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y),flag=0;
					for(int i=0;i<Point.length();i++)
					{
						sPointTile &p=Point[i];
						int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
						float z=((int)p.zw+(int)p.dz);
						if(FlagShareWorld)
						{
							Vect3f orientation(xofs+x-pEye.x,yofs+y-pEye.y,z-pEye.z);
							float norm2=orientation.norm2();
							if(norm2<DistGorizont2)
							{
								int xr=xofs+x, yr=yofs+y;
								z=(2*TileMap->GetZReflectionSurface()-z)-SHARE_FLOAT(xr*xr+yr*yr);
								flag=1;
							}
							else 
							{
								float f=DistGorizont/sqrt(norm2);
								int xr=round((xofs+x)*f), yr=round((yofs+y)*f);
								float zReflectionGorizont=TileMap->GetZReflectionSurface()-SHARE_FLOAT(xr*xr+yr*yr);
								float dzReflection=zReflectionGorizont-zEyeHint;
								float zReflectionSurface=zEyeHint+dzReflection/f;
								xr=xofs+x, yr=yofs+y;
								z=2*zReflectionSurface-(z-SHARE_FLOAT(xr*xr+yr*yr));
								if(z>zReflectionSurface) z=zReflectionSurface; else flag=1;
							}
						}
						else
						{ z=2*TileMap->GetZReflectionSurface()-z; flag=1; }
						Vect3f po((float)x,(float)y,(float)z);
						ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
						SetPointFix(i,255,255,255,p.da(),Vect2f(p.u,p.v));
//						ConvertorObjectToScreen.ConvertPoint(po,pv,pe);
//						SetPointFix(i,pe,255,255,255,p.da(),Vect2f(p.u,p.v),pv);
					}
					if(flag==0) continue;
					for(i=0;i<Polygon.length();i++)
						AddPolygonFixTestPointFix(Polygon[i].p2,Polygon[i].p1,Polygon[i].p3);
					if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
					{
						if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
						{
							cD3DRender_SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
	//								this->Attribute=RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_MULTICANAL;
							this->Attribute|=RENDER_MULTICANAL;
							Draw(Camera,RenderDevice,bTile->GetTexture()->nTexture,bTile->GetLightMap()->nTexture);
						}
						else
							Draw(Camera,RenderDevice);
					}
					else
						Draw(Camera,RenderDevice);
/*
					Draw(Camera,RenderDevice);
					if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
						if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
						{
							cD3DRender_SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
							this->Attribute=RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_MULTICANAL;
							Draw(Camera,RenderDevice);
						}
*/
				}
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_WRAP);
	}
#endif //_USE_TILEMAP_
}
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cPolyGrid *PolyGrid)
{
#ifdef _USE_TILEMAP_
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cRenderDevice *RenderDevice=Camera->GetViewPort();
		cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;
		// создание конвертера из объектного в экранное пространство
		Vect3f ShareOfs;
		cConvertor ConvertorObjectToScreen;
		PolyGrid->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),ShareOfs);
		ConvertorObjectToScreen.BuildMatrix();
		ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
		int RadiusWorldShare=GlobalWorldRadius, RadiusWorldShare2=RadiusWorldShare*RadiusWorldShare;
		// подготовка к рендеру
		int xsize=PolyGrid->xsize,ysize=PolyGrid->ysize;
		int	xstep=PolyGrid->xstep,ystep=PolyGrid->ystep;
		float du=PolyGrid->du,dv=PolyGrid->dv;
		float uofs=PolyGrid->uofs,vofs=PolyGrid->vofs;
		float usize=PolyGrid->usize,vsize=PolyGrid->vsize;
		sPointPolyGrid *Point=PolyGrid->Point;
		int AttributeMaterial=MATERIAL_ATTRIBUTE(PolyGrid->GetAttribute());
		int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
		if(AttributeMaterial&ATTRMAT_TEXTURE_PAL)
		{
			if(PolyGrid->Texture->nTexture==0)
				CreateTexture((cMaterial*)PolyGrid,RenderDevice);
			cD3DRender_SetTexture(PolyGrid->Texture->nTexture);
			BaseAttribute|=RENDER_COLOR_MOD_TEXTURE1;
		}
		InitFix(BaseAttribute,xsize*ysize);
		// установка вершин
		float xpos=ShareOfs.x,ypos=ShareOfs.y,duPoint=du+uofs,dvPoint=dv+vofs,ddu=usize/(xsize-1),ddv=vsize/(ysize-1),uLimit=usize/255,vLimit=vsize/255;
		sPointPolyGrid *bPoint=Point,*ePoint=&Point[xsize*ysize];
		for(;bPoint<ePoint;xpos=ShareOfs.x,ypos+=ystep,duPoint=du+uofs,dvPoint+=ddv)
			for(sPointPolyGrid *lPoint=&bPoint[xsize];bPoint<lPoint;bPoint++,xpos+=xstep,duPoint+=ddu)
			{
				Vect3f po(xpos,ypos,0),pv,pe;
				if(FlagShareWorld)
				{
   					int l=round(po.x*po.x+po.y*po.y);
					if(l<RadiusWorldShare2)
						po.z-=SHARE_FLOAT(l);
					else
					{
						float d=sqrt(l);
						float k=(2*RadiusWorldShare-d)/d;
						po.x*=k; po.y*=k;
	   					l=round(l*k*k);
						po.z-=2*RadiusWorldShare-SHARE_FLOAT(l);
					}
				}
/*				ConvertorObjectToScreen.ConvertPoint(po,pv,pe);
				SetPointFix(bPoint-Point,pe,bPoint->r,bPoint->g,bPoint->b,bPoint->a,
					Vect2f(duPoint+bPoint->du*uLimit,dvPoint+bPoint->dv*vLimit),pv);
*/				ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[bPoint-Point].pv,*(Vect3f*)&PointFix[bPoint-Point]);
				SetPointFix(bPoint-Point,bPoint->r,bPoint->g,bPoint->b,bPoint->a,
					Vect2f(duPoint+bPoint->du*uLimit,dvPoint+bPoint->dv*vLimit));
			}
		// установка полигонов
		for(int j=0,jend=(ysize-1)*xsize;j<jend;j+=xsize)
			for(int i=j,iend=j+xsize-1;i<iend;i++)
			{
				sPointPolyGrid *p=&Point[i];
				if(p[0].attribute|p[1].attribute|p[xsize].attribute)
					AddPolygonFixTestPointFix(i,i+xsize,i+1);
				if(p[1].attribute|p[xsize].attribute|p[1+xsize].attribute)
					AddPolygonFixTestPointFix(i+1,i+xsize,i+1+xsize);
			}
		Draw(Camera,RenderDevice);
	}
#endif //_USE_TILEMAP_
}
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cTileWater *TileWater)
{
#ifdef _USE_TILEMAP_
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		cRenderDevice *RenderDevice=Camera->GetViewPort();
		cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;
		// создание конвертера из объектного в экранное пространство
		Vect2f dTex(0,0);
		if(TileWater->GetAttribute(TILEWATER_MOVTEXTURE)) { dTex.x-=Camera->GetPos().x/(TileWater->GetScaleTex().x*(1<<TileWater->_SizeTileX())); dTex.y-=Camera->GetPos().y/(TileWater->GetScaleTex().y*(1<<TileWater->_SizeTileY())); }
		dTex.x=dTex.x-(int)dTex.x; dTex.y=dTex.y-(int)dTex.y;
		int RadiusWorldShare=GlobalWorldRadius, RadiusWorldShare2=RadiusWorldShare*RadiusWorldShare;
		sTileMap *bTile=TileWater->GetTileMap()->GetTile(0,0);
		cPolyGrid **bWater=&TileWater->GetPolyGrid(0,0),**eWater=&TileWater->GetPolyGrid(0,TileWater->NumberTileY());
		for(;bWater<eWater;bWater++,bTile++)
			if(((*bWater)!=0)&&(bTile->isVisibleTotal(UCameraList)&CONST_VISIBLE_FRUSTUM))
			{
//				(*bWater)->du=dTex.x; (*bWater)->dv=dTex.y;
				cPolyGrid *PolyGrid=(*bWater);
				Vect3f ShareOfs;
				cConvertor ConvertorObjectToScreen;
				PolyGrid->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),ShareOfs);
				ConvertorObjectToScreen.BuildMatrix();
				ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
				// подготовка к рендеру
				int xsize=PolyGrid->xsize,ysize=PolyGrid->ysize;
				int	xstep=PolyGrid->xstep,ystep=PolyGrid->ystep;
				float du=PolyGrid->du+dTex.x,dv=PolyGrid->dv+dTex.y;
				float uofs=PolyGrid->uofs,vofs=PolyGrid->vofs;
				float usize=PolyGrid->usize,vsize=PolyGrid->vsize;
				sPointPolyGrid *Point=PolyGrid->Point;
				int AttributeMaterial=MATERIAL_ATTRIBUTE(PolyGrid->GetAttribute());
				int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
				if(AttributeMaterial&ATTRMAT_TEXTURE_PAL)
				{
					BaseAttribute|=RENDER_COLOR_MOD_TEXTURE1;
					if(PolyGrid->Texture->nTexture==0)
						CreateTexture((cMaterial*)PolyGrid,RenderDevice);
					cD3DRender_SetTexture(PolyGrid->Texture->nTexture);
				}
				InitFix(BaseAttribute,xsize*ysize);
				// установка вершин
				float xpos=ShareOfs.x,ypos=ShareOfs.y,duPoint=du+uofs,dvPoint=dv+vofs,ddu=usize/(xsize-1),ddv=vsize/(ysize-1),uLimit=usize/255,vLimit=vsize/255;
				sPointPolyGrid *bPoint=Point,*ePoint=&Point[xsize*ysize];
				for(;bPoint<ePoint;xpos=ShareOfs.x,ypos+=ystep,duPoint=du+uofs,dvPoint+=ddv)
					for(sPointPolyGrid *lPoint=&bPoint[xsize];bPoint<lPoint;bPoint++,xpos+=xstep,duPoint+=ddu)
					{
						Vect3f po(xpos,ypos,0);
						if(FlagShareWorld)
						{
   							int l=round(po.x*po.x+po.y*po.y);
							if(l<RadiusWorldShare2)
								po.z-=SHARE_FLOAT(l);
							else
							{
								float d=sqrt(l);
								float k=(2*RadiusWorldShare-d)/d;
								po.x*=k; po.y*=k;
	   							l=round(l*k*k);
								po.z-=2*RadiusWorldShare-SHARE_FLOAT(l);
							}
						}
/*
						ConvertorObjectToScreen.ConvertPoint(po,pv,pe);
						SetPointFix(bPoint-Point,pe,bPoint->r,bPoint->g,bPoint->b,bPoint->a,
							Vect2f(duPoint+bPoint->du*uLimit,dvPoint+bPoint->dv*vLimit),pv);
*/						ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[bPoint-Point].pv,*(Vect3f*)&PointFix[bPoint-Point]);
						SetPointFix(bPoint-Point,bPoint->r,bPoint->g,bPoint->b,bPoint->a,
							Vect2f(duPoint+bPoint->du*uLimit,dvPoint+bPoint->dv*vLimit));
					}
				// установка полигонов
				for(int j=0,jend=(ysize-1)*xsize;j<jend;j+=xsize)
					for(int i=j,iend=j+xsize-1;i<iend;i++)
					{
						sPointPolyGrid *p=&Point[i];
						if(p[0].attribute|p[1].attribute|p[xsize].attribute)
							AddPolygonFixTestPointFix(i,i+xsize,i+1);
						if(p[1].attribute|p[xsize].attribute|p[1+xsize].attribute)
							AddPolygonFixTestPointFix(i+1,i+xsize,i+1+xsize);
					}
				Draw(Camera,RenderDevice);
			}
	}
#endif //_USE_TILEMAP_
}
//////////////////// POLYGRID RASTERIZATION ////////////////////
void cPolyDispatcher::DrawTilePolyGrid(cRenderDevice *RenderDevice,cCamera *Camera,cBasePolyGrid *WorldPolyGrid,int i1,int j1,int i2,int j2,cMaterial *Material)
{
	cConvertor ConvertorObjectToScreen;
	float xstep=WorldPolyGrid->xstep,ystep=WorldPolyGrid->ystep;
	int xsize=WorldPolyGrid->xsize,ysize=WorldPolyGrid->ysize;
	ConvertorObjectToScreen.GetMatrix()=WorldPolyGrid->Matrix;
	ConvertorObjectToScreen.GetMatrix().trans().x+=xstep*i1; 
	ConvertorObjectToScreen.GetMatrix().trans().y+=ystep*j1;
	WorldToCameraCutting(ConvertorObjectToScreen.GetMatrix().trans(),Camera);
	Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
	float xofs=ConvertorObjectToScreen.GetMatrix().trans().x-CameraPosScale.x, 
		yofs=ConvertorObjectToScreen.GetMatrix().trans().y-CameraPosScale.y;
	float RadiusWorldShare=GlobalWorldRadius,DivRadiusWorldShare=GRAD_TO_DGRAD(1)/(GRAD_RAD*RadiusWorldShare);
	ConvertorObjectToScreen.GetMatrix().trans().x=Camera->GetPos().x; 
	ConvertorObjectToScreen.GetMatrix().trans().y=Camera->GetPos().y;
	ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*ConvertorObjectToScreen.GetMatrix();
	ConvertorObjectToScreen.BuildMatrix();
	ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));

	int isize=i2-i1, jsize=j2-j1, ij=0;
	assert(isize<=xsize);
	assert(jsize<=ysize);
	int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
	if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
	if(Material->GetAttribute(ATTRMAT_TEXTURE_PAL))
		BaseAttribute|=RENDER_COLOR_MOD_TEXTURE1;
	InitFix(BaseAttribute,isize*jsize);
	float xpos=xofs,ypos=yofs, ustep, duPoint0,vstep, duPoint,dvPoint;
	if(WorldPolyGrid->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_MULTIMATERIAL))
	{ // убирается зацикливание, константы подобраны имперически для текстуры 128
		ustep=WorldPolyGrid->u/xsize, vstep=WorldPolyGrid->v/ysize;
		duPoint0=WorldPolyGrid->du+WorldPolyGrid->uofs+ustep*i1;
		dvPoint=WorldPolyGrid->dv+WorldPolyGrid->vofs+vstep*j1;
		duPoint0=fmod(duPoint0+1,1); dvPoint=fmod(dvPoint+1,1); duPoint=duPoint0;
		duPoint0+=0.0035f; dvPoint+=0.0035f; ustep*=(1-0.007f); vstep*=(1-0.007f);
	}
	else
	{
		ustep=WorldPolyGrid->u, duPoint0=WorldPolyGrid->du+WorldPolyGrid->uofs+ustep*i1,
		vstep=WorldPolyGrid->v, duPoint=duPoint0, dvPoint=WorldPolyGrid->dv+WorldPolyGrid->vofs+vstep*j1;
	}
	sBaseColor4c *pColor=WorldPolyGrid->pColor;
	for(int j=j1;j<j2;j++,xpos=xofs,ypos+=ystep,duPoint=duPoint0,dvPoint+=vstep)
		for(int i=i1;i<i2;i++,xpos+=xstep,duPoint+=ustep,ij++)
		{
			sBaseColor4c &p=pColor[(i%xsize)+(j%ysize)*xsize];
			Vect3f po(xpos,ypos,0),pv,pe;
			ShareMapping2(Camera,po,RadiusWorldShare,DivRadiusWorldShare);
/*			ConvertorObjectToScreen.ConvertPoint(Vect3f(xPos,yPos,zw),pv,pe);
			SetPointFix(ij++,pe,p.r,p.g,p.b,p.a,Vect2f(duPoint,dvPoint),pv);
*/			ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[ij].pv,*(Vect3f*)&PointFix[ij]);
			SetPointFix(ij,p.r,p.g,p.b,p.a,Vect2f(duPoint,dvPoint));
		}
	if(WorldPolyGrid->BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW))
		for(int j=0;j<(jsize-1)*isize;j+=isize)
			for(int i=j;i<(j+isize-1);i++)
			{
				AddPolygonFixTestPointFix(i,i+isize,i+1);
				AddPolygonFixTestPointFix(i+1,i+isize,i+1+isize);
			}
	if(WorldPolyGrid->BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW))
		for(int j=0;j<(jsize-1)*isize;j+=isize)
			for(int i=j;i<(j+isize-1);i++)
			{
				AddPolygonFixTestPointFix(i+isize,i,i+1);
				AddPolygonFixTestPointFix(i+isize,i+1,i+1+isize);
			}
	Draw(Camera,RenderDevice);
}
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cWorldPolyGrid *WorldPolyGrid)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;

		if(!FlagShareWorld)continue;
		cMaterial *Material=&(WorldPolyGrid->MaterialArray[0]);
		if(Material->GetAttribute(ATTRMAT_TEXTURE_PAL))
		{
			if(Material->Texture->nTexture==0)
				CreateTexture(Material,RenderDevice);
			cD3DRender_SetTexture(Material->Texture->nTexture); // установка текстуры
		}
		Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
		int xView=Cycl(CameraPosScale.x+GlobalWorldSize.x*GlobalWorldScale.x/2,GlobalWorldSize.x*GlobalWorldScale.x),
			yView=Cycl(CameraPosScale.y+GlobalWorldSize.y*GlobalWorldScale.y/2,GlobalWorldSize.y*GlobalWorldScale.y),
			iView=xView/WorldPolyGrid->xstep+1, jView=yView/WorldPolyGrid->ystep+1;
		Graph3d->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
		Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_CW);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,0,0,iView,jView,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,iView,0,WorldPolyGrid->xsize+1,jView,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,0,jView,iView,WorldPolyGrid->ysize+1,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,iView,jView,WorldPolyGrid->xsize+1,WorldPolyGrid->ysize+1,Material);
		Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_NONE);
		Graph3d->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
	}
}
//////////////////// SURFACEREFLECTION RASTERIZATION ////////////////////
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cSurfaceReflectionMultiMaterial *Surface)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	int istep=Surface->xsize/Surface->xMaterial,jstep=Surface->ysize/Surface->yMaterial;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_CLAMP);
		for(int j=0,j1=0,j2;j<Surface->yMaterial;j++,j1+=jstep)
		{
			if(j!=(Surface->yMaterial-1)) j2=j1+jstep+1; else j2=j1+jstep;
			for(int i=0,i1=0,i2;i<Surface->xMaterial;i++,i1+=istep)
			{
				if(i!=(Surface->xMaterial-1)) i2=i1+istep+1; else i2=i1+istep;
				cMaterial *Material=&(Surface->MaterialArray[i+j*Surface->xMaterial]);
				if(Material->GetAttribute(ATTRMAT_TEXTURE_PAL))
				{
					if(Material->Texture->nTexture==0)
						CreateTexture(Material,RenderDevice);
					if(Material->Texture->GetStatus(STATUS_TEXTURE_RESET))
						ResetTextureMultiMaterialSurface565(Graph3d,Material,Surface);
					cD3DRender_SetTexture(Material->Texture->nTexture); // установка текстуры
				}
				DrawTilePolyGrid(RenderDevice,Camera,Surface,i1,j1,i2,j2,Material);
			}
		}
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_WRAP);
	}
}
//////////////////// SUN RASTERIZATION ////////////////////
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cUnkClassDynArrayPointer *SunArray)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	cUnkClassDynArrayPointer &SunDynArray=*SunArray;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		SetClippingPlane(Camera);
		int FlagShareWorld=Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)==ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE;

		if(!FlagShareWorld)continue;
		for(int i=0;i<SunDynArray.length();i++)
		{
			cSun &Sun=*(cSun*)SunDynArray[i];
			float RadiusLight=Sun.GetRadiusLight();
			cMaterial *Material=Sun.GetMaterial();
			if(RadiusLight<=1) continue;

			cConvertor ConvertorObjectToScreen;
			ConvertorObjectToScreen.GetMatrix().set(Mat3f::ID,Sun.GetPos());
			WorldToCameraCutting(ConvertorObjectToScreen.GetMatrix().trans(),Camera);
			Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
			ConvertorObjectToScreen.GetMatrix().trans().x-=CameraPosScale.x, 
			ConvertorObjectToScreen.GetMatrix().trans().y-=CameraPosScale.y;
			float RadiusWorldShare=GlobalWorldRadius,DivRadiusWorldShare=GRAD_TO_DGRAD(1)/(GRAD_RAD*RadiusWorldShare);
			ShareMapping2(Camera,ConvertorObjectToScreen.GetMatrix().trans(),RadiusWorldShare,DivRadiusWorldShare);
			ConvertorObjectToScreen.GetMatrix().trans().x+=Camera->GetPos().x;
			ConvertorObjectToScreen.GetMatrix().trans().y+=Camera->GetPos().y;
			ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*ConvertorObjectToScreen.GetMatrix();
			ConvertorObjectToScreen.BuildMatrix();
			ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
			Vect3f pw(0,0,0),pv,pe;
			ConvertorObjectToScreen.ConvertPoint(pw,pv,pe);

			// инициализация буффера диспетчера растеризации
			if(Material->GetAttribute(ATTRMAT_TEXTURE_PAL))
			{
				float RadiusSun=Sun.GetRadiusSun();
				int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
				if(Material->GetAttribute(ATTRMAT_MASK))
					BaseAttribute|=RENDER_ALPHA_MASK_TEXTURE1;
				if(Material->GetAttribute(ATTRMAT_OPACITY))
					BaseAttribute|=RENDER_ALPHA_MOD_TEXTURE1;
				if(Material->Texture->nTexture==0)
					CreateTexture(Material,RenderDevice);
				cD3DRender_SetTexture(Material->Texture->nTexture);
				InitFix(BaseAttribute,4);
				Vect3f pv1(pv.x-RadiusSun,pv.y-RadiusSun,pv.z);
				Vect3f pe1(pv1.x*pe.z,pv1.y*pe.z,pe.z);
				SetPointFix(0,pe1,255,255,255,255,Vect2f(0,0),pv1);
				pv1.set(pv.x-RadiusSun,pv.y+RadiusSun,pv1.z);
				pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe1.z);
				SetPointFix(1,pe1,255,255,255,255,Vect2f(0,1),pv1);
				pv1.set(pv.x+RadiusSun,pv.y+RadiusSun,pv1.z);
				pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe1.z);
				SetPointFix(2,pe1,255,255,255,255,Vect2f(1,1),pv1);
				pv1.set(pv.x+RadiusSun,pv.y-RadiusSun,pv1.z);
				pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe1.z);
				SetPointFix(3,pe1,255,255,255,255,Vect2f(1,0),pv1);
				AddPolygonFixTestPointFix(0,1,2);
				AddPolygonFixTestPointFix(2,3,0);
				Draw(Camera,RenderDevice);
			}

			int CurrentPoint=0,PreviousPoints=0;
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
			int NumberAngle=(int)(RadiusLight/50)+8,NumberPlane=(int)(RadiusLight/40)+3;
			float KoofAttenuation=1.2f/(NumberPlane-1);
			int rc=Sun.GetSpecular().GetR(),gc=Sun.GetSpecular().GetG(),bc=Sun.GetSpecular().GetB();
			rc*=KoofAttenuation; gc*=KoofAttenuation; bc*=KoofAttenuation;
			float limit=(float)RadiusLight*(NumberPlane-1)/NumberPlane, dlimit=(float)RadiusLight/NumberPlane;
			int RenderAttribute=MAT_COLOR_ADD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE|RENDER_COLOR_MOD_DIFFUSE;
			if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
			InitFix(RenderAttribute,round((NumberAngle+1)*(limit+limit+dlimit)/dlimit));
			for(float height=+limit;height>=-limit;height-=dlimit)
			{
				PreviousPoints=CurrentPoint;
				float radius=RadiusLight;
				if(height>0) radius=RadiusLight-height; else radius=RadiusLight+height;
				if((pv.z+height)<=Camera->GetZPlane().x) continue;
				float div_zv;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) div_zv=1/(pv.z+height);
				else div_zv=1/Camera->GetPos().z;
				Vect3f pv1(pv.x,pv.y,pv.z+height);
				Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
				SetPointFix(CurrentPoint++,pe1,rc,gc,bc,255,pv1);
				for(int k=1;k<(NumberAngle+1);k++)
				{
					int angle=(k-1)*GRAD_TO_DGRAD(360)/NumberAngle;
					Vect3f pv1(pv.x+RadiusLight*COS_FLOAT_DGRAD(angle),pv.y+RadiusLight*SIN_FLOAT_DGRAD(angle),pv.z+height);
					Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
					SetPointFix(CurrentPoint++,pe1,0,0,0,0,pv1);
				}
				AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+1,PreviousPoints+NumberAngle);
				for(int i=1;i<NumberAngle;i++)
					AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+i+1,PreviousPoints+i);
			}
			Draw(Camera,RenderDevice);
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
		}
	}
}
//////////////////// TRAIL RASTERIZATION ////////////////////
void cPolyDispatcher::Draw(cUnknownClass *UCameraList,cTangentTrail *TangentTrail)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		SetClippingPlane(Camera);

		cTileMap *TileMap=TangentTrail->tMap;
		int i=(int)(TangentTrail->Pos.x/(1<<TileMap->_SizeTileX()));
		int j=(int)(TangentTrail->Pos.y/(1<<TileMap->_SizeTileY()));
		if((TileMap->GetTile(i,j)->GetVisibleTotal(nCamera)&CONST_VISIBLE_FRUSTUM)==0)
			continue;
		cMaterial *Material=&TangentTrail->MaterialArray[0];
		sColor4f &Diffuse1=TangentTrail->Diffuse1,&Diffuse2=TangentTrail->Diffuse2;
		float Phase=TangentTrail->CurrentTime/TangentTrail->FinishTime,Intensity=255.f;
		if(TangentTrail->AnimTime<1e10f)
			Intensity=128+127*sin(2*M_PI*(TangentTrail->StartTime+TangentTrail->CurrentTime)/TangentTrail->AnimTime);
		if(Phase>=1) { TangentTrail->Type|=BASEOBJECT_TYPE_DELETE; continue; }
		cConvertor ConvertorObjectToScreen;
		ConvertorObjectToScreen.GetMatrix().set(Mat3f::ID,TangentTrail->Pos);
		WorldToCameraCutting(ConvertorObjectToScreen.GetMatrix(),Camera);
		WorldShareMapping(ConvertorObjectToScreen.GetMatrix().trans(),Camera);
		ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*ConvertorObjectToScreen.GetMatrix();
		ConvertorObjectToScreen.BuildMatrix();
		ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
		int r1=round(Diffuse1.r*Intensity),
			g1=round(Diffuse1.g*Intensity),
			b1=round(Diffuse1.b*Intensity),
			a1=round(Diffuse1.a*(1-Phase)*255);
		int r2=round(Diffuse2.r*Intensity),
			g2=round(Diffuse2.g*Intensity),
			b2=round(Diffuse2.b*Intensity),
			a2=round(Diffuse2.a*(1-Phase)*255);

		int BaseAttribute=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
		if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
		if(Material&&Material->GetAttribute(ATTRMAT_TEXTURE_PAL)) 
		{
			cD3DRender_SetTexture(Material->Texture->nTexture); // установка текстуры
			BaseAttribute|=RENDER_COLOR_MOD_TEXTURE1;
		}
		InitFix(BaseAttribute,4);
/*		
		Vect3f pv,pe,*Point=TangentTrail->Point;
		ConvertorObjectToScreen.ConvertPoint(Point[0],pv,pe);
		SetPointFix(0,pe,r1,g1,b1,a1,Vect2f(0.f,1.f),pv);
		ConvertorObjectToScreen.ConvertPoint(Point[1],pv,pe);
		SetPointFix(1,pe,r2,g2,b2,a2,Vect2f(1.f,1.f),pv);
		ConvertorObjectToScreen.ConvertPoint(Point[2],pv,pe);
		SetPointFix(2,pe,r2,g2,b2,a2,Vect2f(1.f,0.f),pv);
		ConvertorObjectToScreen.ConvertPoint(Point[3],pv,pe);
		SetPointFix(3,pe,r1,g1,b1,a1,Vect2f(0.f,0.f),pv);
*/
		Vect3f *Point=TangentTrail->Point;
		ConvertorObjectToScreen.ConvertPoint(Point[0],PointAttribute[0].pv,*(Vect3f*)&PointFix[0]);
		SetPointFix(0,r1,g1,b1,a1,Vect2f(0.f,1.f));
		ConvertorObjectToScreen.ConvertPoint(Point[1],PointAttribute[1].pv,*(Vect3f*)&PointFix[1]);
		SetPointFix(1,r2,g2,b2,a2,Vect2f(1.f,1.f));
		ConvertorObjectToScreen.ConvertPoint(Point[2],PointAttribute[2].pv,*(Vect3f*)&PointFix[2]);
		SetPointFix(2,r2,g2,b2,a2,Vect2f(1.f,0.f));
		ConvertorObjectToScreen.ConvertPoint(Point[3],PointAttribute[3].pv,*(Vect3f*)&PointFix[3]);
		SetPointFix(3,r1,g1,b1,a1,Vect2f(0.f,0.f));

		AddPolygonFixTestPointFix(1,0,2);
		AddPolygonFixTestPointFix(2,0,3);
		Draw(Camera,RenderDevice);
	}
}
//////////////////// PARTICLE RASTERIZATION ////////////////////
void cPolyDispatcher::BeginList(cUnknownClass *UCamera,int idTextureChild,MatXf &Matrix)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	CurrentCamera=(cCamera*)UCamera;
	SetClippingPlane(CurrentCamera);
	
	CurrentConvertorObjectToScreen->GetMatrix()=*(cMatrix*)&Matrix;
	WorldToCameraCutting(CurrentConvertorObjectToScreen->GetMatrix(),CurrentCamera);
	WorldShareMapping(CurrentConvertorObjectToScreen->GetMatrix().trans(),CurrentCamera);
	CurrentConvertorObjectToScreen->GetMatrix()=CurrentCamera->GetMatrix()*CurrentConvertorObjectToScreen->GetMatrix();
	CurrentConvertorObjectToScreen->BuildMatrix();
	CurrentConvertorObjectToScreen->SetProjection(CurrentCamera,CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
	
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	assert(TextureChild.Texture);
	if(TextureChild.Texture->nTexture==0) CreateTexture4444(TextureChild.Texture,RenderDevice);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	cD3DRender_SetTexture(TextureChild.Texture->nTexture);
	int RenderAttribute=0;
	if(CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
	InitFix(RenderAttribute|RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_ALPHA_MOD_TEXTURE1);
	AlphaForSprite=255;
	if(CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_SHARE))
	{
		int dx=GetShortDistX(round(CurrentCamera->GetPos().x),round(Matrix.trans().x)),
			dy=GetShortDistY(round(CurrentCamera->GetPos().y),round(Matrix.trans().y));
		int l=(dx*dx+dy*dy)-512*512;
		if(l>0)
		{
			AlphaForSprite=255-255*l/(3*512*512);
			if(AlphaForSprite<0) AlphaForSprite=0;
		}
	}
}
void cPolyDispatcher::AttachCenter(const Vect3f &pos,float angle,float scale,int rgbaDiffuse,int idTextureChild)
{
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
//	int Angle=round(RAD_TO_GRAD(fmod(angle+6.283,6.283)));
	Vect3f pv,pe,pv1,pe1;
	CurrentConvertorObjectToScreen->ConvertPoint(pos,pv,pe);
	int CurrentPoint=PointFix.length();
	PointAttribute.length()=(PointFix.length()+=4);
	int rc=GET_INT_R(rgbaDiffuse),
		gc=GET_INT_G(rgbaDiffuse),
		bc=GET_INT_B(rgbaDiffuse),
		ac=(GET_INT_A(rgbaDiffuse)*AlphaForSprite)>>8;
//	Vect2f t1(0,0),t2(1,1);
	Vect2f t1(TextureChild.uofs,TextureChild.vofs),t2(TextureChild.uofs+TextureChild.usize,TextureChild.vofs+TextureChild.vsize);
	pv1.set(pv.x+scale*cos(G2R(45)+angle),pv.y+scale*sin(G2R(45)+angle),pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t1.y),pv1);
	pv1.set(pv.x+scale*cos(G2R(90+45)+angle),pv.y+scale*sin(G2R(90+45)+angle),pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t1.y),pv1);
	pv1.set(pv.x+scale*cos(G2R(90+90+45)+angle),pv.y+scale*sin(G2R(90+90+45)+angle),pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t2.y),pv1);
	pv1.set(pv.x+scale*cos(G2R(90+90+90+45)+angle),pv.y+scale*sin(G2R(90+90+90+45)+angle),pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t2.y),pv1);
	AddPolygonFixTestPointFix(CurrentPoint-1,CurrentPoint-2,CurrentPoint-3);
	AddPolygonFixTestPointFix(CurrentPoint-4,CurrentPoint-1,CurrentPoint-3);
}
void cPolyDispatcher::AttachCenter(const Vect3f &pos,sSpriteFX *SpriteFX,int idTextureChild)
{
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	Vect3f pv,pe,pv1,pe1;
	CurrentConvertorObjectToScreen->ConvertPoint(pos,pv,pe);
	int CurrentPoint=PointFix.length();
	PointAttribute.length()=(PointFix.length()+=4);
	int rc=SpriteFX->rD,gc=SpriteFX->gD,bc=SpriteFX->bD,ac=(SpriteFX->aD*AlphaForSprite)>>8;
	Vect2f &a=SpriteFX->a,&b=SpriteFX->b;
	Vect2f t1(TextureChild.uofs,TextureChild.vofs),t2(TextureChild.uofs+TextureChild.usize,TextureChild.vofs+TextureChild.vsize);
	pv1.set(pv.x+a.x,pv.y+a.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t1.y),pv1);
	pv1.set(pv.x+b.x,pv.y+b.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t1.y),pv1);
	pv1.set(pv.x-a.x,pv.y-a.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t2.y),pv1);
	pv1.set(pv.x-b.x,pv.y-b.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t2.y),pv1);
	AddPolygonFixTestPointFix(CurrentPoint-1,CurrentPoint-2,CurrentPoint-3);
	AddPolygonFixTestPointFix(CurrentPoint-4,CurrentPoint-1,CurrentPoint-3);
}
void cPolyDispatcher::BeginListShare(cUnknownClass *UCamera,int idTextureChild,MatXf &Matrix)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	CurrentCamera=(cCamera*)UCamera;
	SetClippingPlane(CurrentCamera);
	
	CurrentConvertorObjectToScreen->GetMatrix()=*(cMatrix*)&Matrix;
//	WorldToCameraCutting(CurrentConvertorObjectToScreen->GetMatrix(),CurrentCamera);
	CurrentConvertorObjectToScreen->GetMatrix().trans().set(CurrentCamera->GetPos().x,CurrentCamera->GetPos().y,0);
	CurrentConvertorObjectToScreen->GetMatrix()=CurrentCamera->GetMatrix()*CurrentConvertorObjectToScreen->GetMatrix();
	CurrentConvertorObjectToScreen->BuildMatrix();
	CurrentConvertorObjectToScreen->SetProjection(CurrentCamera,CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
	
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	assert(TextureChild.Texture);
	if(TextureChild.Texture->nTexture==0) CreateTexture4444(TextureChild.Texture,RenderDevice);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	cD3DRender_SetTexture(TextureChild.Texture->nTexture);
	int RenderAttribute=0;
	if(CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
	InitFix(RenderAttribute|RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_ALPHA_MOD_TEXTURE1);
}
void cPolyDispatcher::AttachCenterShare(const Vect3f &pos,sSpriteFX *SpriteFX,int idTextureChild)
{
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	Vect3f pv,pe,pv1,pe1,Pos=pos;
	int xr=GetShortDistX(round(pos.x),round(CurrentCamera->GetPos().x),2048),
		yr=GetShortDistY(round(pos.y),round(CurrentCamera->GetPos().y),2048);
	Pos.x=xr; Pos.y=yr;
	Pos.z-=SHARE_FLOAT(xr*xr+yr*yr);
	CurrentConvertorObjectToScreen->ConvertPoint(Pos,pv,pe);
	int CurrentPoint=PointFix.length();
	PointAttribute.length()=(PointFix.length()+=4);
	int rc=SpriteFX->rD,gc=SpriteFX->gD,bc=SpriteFX->bD,ac=SpriteFX->aD;
	Vect2f &a=SpriteFX->a,&b=SpriteFX->b;
	Vect2f t1(TextureChild.uofs,TextureChild.vofs),t2(TextureChild.uofs+TextureChild.usize,TextureChild.vofs+TextureChild.vsize);
	pv1.set(pv.x+a.x,pv.y+a.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t1.y),pv1);
	pv1.set(pv.x+b.x,pv.y+b.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t1.y),pv1);
	pv1.set(pv.x-a.x,pv.y-a.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t2.x,t2.y),pv1);
	pv1.set(pv.x-b.x,pv.y-b.y,pv.z);
	pe1.set(pv1.x*pe.z,pv1.y*pe.z,pe.z);
	SetPointFix(CurrentPoint++,pe1,rc,gc,bc,ac,Vect2f(t1.x,t2.y),pv1);
	AddPolygonFixTestPointFix(CurrentPoint-1,CurrentPoint-2,CurrentPoint-3);
	AddPolygonFixTestPointFix(CurrentPoint-4,CurrentPoint-1,CurrentPoint-3);
}
void cPolyDispatcher::BeginList(cUnknownClass *UCamera,int idTextureChild)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	CurrentCamera=(cCamera*)UCamera;
	SetClippingPlane(CurrentCamera);

	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	assert(TextureChild.Texture);
	if(TextureChild.Texture->nTexture==0) CreateTexture4444(TextureChild.Texture,RenderDevice);
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	cD3DRender_SetTexture(TextureChild.Texture->nTexture);
	int RenderAttribute=0;
	if(CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
	InitFix(RenderAttribute|RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_ALPHA_MOD_TEXTURE1);
//	InitFix(RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_ALPHA_MOD_TEXTURE1);
}
void cPolyDispatcher::AttachCenter(const Vect2f &pos,sSpriteFX *SpriteFX,int idTextureChild)
{
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	Vect3f pv,pe;
	float zNear=CurrentCamera->GetZPlane().x, dez=1/zNear;
	int CurrentPoint=PointFix.length();
	PointAttribute.length()=(PointFix.length()+=4);
	int rc=SpriteFX->rD,gc=SpriteFX->gD,bc=SpriteFX->bD,ac=(SpriteFX->aD*AlphaForSprite)>>8;
	Vect2f &a=SpriteFX->a,&b=SpriteFX->b;
	Vect2f t1(TextureChild.uofs,TextureChild.vofs),t2(TextureChild.uofs+TextureChild.usize,TextureChild.vofs+TextureChild.vsize);
	pe.set(pos.x+a.x,pos.y+a.y,dez);
	pv.set(pe.x*zNear,pe.y*zNear,zNear);
	SetPointFix(CurrentPoint++,pe,rc,gc,bc,ac,Vect2f(t1.x,t1.y),pv);
	pe.set(pos.x+b.x,pos.y+b.y,dez);
	pv.set(pe.x*zNear,pe.y*zNear,zNear);
	SetPointFix(CurrentPoint++,pe,rc,gc,bc,ac,Vect2f(t2.x,t1.y),pv);
	pe.set(pos.x-a.x,pos.y-a.y,dez);
	pv.set(pe.x*zNear,pe.y*zNear,zNear);
	SetPointFix(CurrentPoint++,pe,rc,gc,bc,ac,Vect2f(t2.x,t2.y),pv);
	pe.set(pos.x-b.x,pos.y-b.y,dez);
	pv.set(pe.x*zNear,pe.y*zNear,zNear);
	SetPointFix(CurrentPoint++,pe,rc,gc,bc,ac,Vect2f(t1.x,t2.y),pv);
	AddPolygonFixTestPointFix(CurrentPoint-1,CurrentPoint-2,CurrentPoint-3);
	AddPolygonFixTestPointFix(CurrentPoint-4,CurrentPoint-1,CurrentPoint-3);
}
void cPolyDispatcher::EndList()
{
	GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
	Draw(CurrentCamera,GetRenderDevice(0));
	GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
	CurrentCamera=0;
	AlphaForSprite=255;
}
void cPolyDispatcher::SetViewColor(cUnknownClass *UCamera,sColor4f &Diffuse,sColor4f &Specular,int zWrite)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	SetClippingPlane(Camera);
	int rMul=Diffuse.GetR(),gMul=Diffuse.GetG(),bMul=Diffuse.GetB(),aMul=Diffuse.GetA();
	int rAdd=Specular.GetR(),gAdd=Specular.GetG(),bAdd=Specular.GetB(),aAdd=Specular.GetA();
	int RenderAttribute=RENDER_COLOR_MOD_DIFFUSE;
	if(rAdd|gAdd|bAdd) RenderAttribute|=RENDER_COLOR_ADD_SPECULAR;
	if(aMul<255) RenderAttribute|=RENDER_ALPHA_MOD_DIFFUSE;
	InitFix(RenderAttribute,4);
	Vect3f pe(0,0,1/Camera->GetZPlane().x),pv(0,0,Camera->GetZPlane().x);
	sRectangle4f &Clip=Camera->GetClipping();
	pe.x=Clip.xmin(); pe.y=Clip.ymin();
	SetPointFix(0,pe,rMul,gMul,bMul,aMul,rAdd,gAdd,bAdd,aAdd,pv);
	pe.x=Clip.xmax(); pe.y=Clip.ymin();
	SetPointFix(1,pe,rMul,gMul,bMul,aMul,rAdd,gAdd,bAdd,aAdd,pv);
	pe.x=Clip.xmax(); pe.y=Clip.ymax();
	SetPointFix(2,pe,rMul,gMul,bMul,aMul,rAdd,gAdd,bAdd,aAdd,pv);
	pe.x=Clip.xmin(); pe.y=Clip.ymax();
	SetPointFix(3,pe,rMul,gMul,bMul,aMul,rAdd,gAdd,bAdd,aAdd,pv);
	AddPolygonFixTestPointFix(0,2,1);
	AddPolygonFixTestPointFix(0,3,2);
	if(!zWrite) GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,FALSE);
	Draw(Camera,GetRenderDevice(0));
	if(!zWrite) GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,TRUE);
}
#endif //_ONLY_DIRECT3D_