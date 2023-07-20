//#ifndef _ONLY_DIRECT3D_
#include "PolyMgr.h"
#include "Camera.h"
#include "Object.h"
#include "Object3d.h"
#include "RenderDevice.h"
#include "BasePolyGrid.h"
#include "BaseReflection.h"
#include "BaseTrail.h"
#include "Scene.h"
#include "TexMgr.h"

#ifdef _USE_TILEMAP_
#include "TileMap.h"
#include "TileWater.h"
#endif //_USE_TILEMAP_

float gb_LodValue=0.01f;
#define LOD_VALUE						0.01f
//#define LOD_VALUE						gb_LodValue

extern void ResetTextureMultiMaterialSurface565(cInterfaceGraph3d *IGraph3d,cMaterial *Material,cSurfaceReflectionMultiMaterial *Surface);

int cPolyDispatcher::Draw(cUnknownClass *UCamera,cUnknownClass *URenderDevice,int hTexture,int hLightMap)
{
	assert(UCamera->GetKind(KIND_CAMERA));
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cCamera *Camera=(cCamera*)UCamera;
	if(PolygonFix.length()<=0) return 0;
	sPointAttribute *pa=&PointAttribute[0];
	Vect2f &zPlane=Camera->GetZPlane(),zBufferScale;
	Vect2f focus=Focus;
	if((Attribute&RENDER_REFLECTION)==0) zBufferScale=Camera->GetZBufferScale(); 
	else zBufferScale.set(Camera->GetZBufferScale().y,1.f), focus=Focus*0.995f;
//	if((Attribute&RENDER_MULTICANAL)==0)
		for(sVertexFix *bp=&PointFix[0],*ep=&PointFix[PointFix.length()];bp<ep;bp++,pa++)
		{
			bp->xe=Center.x+focus.x*bp->xe; bp->ye=Center.y+focus.y*bp->ye;
			bp->w=bp->z*POINT_SCALE_W/FLOAT_CONST_PREC; 
#ifndef _ZBUFFER_LINEAR_
			//bp->z=zBufferScale.max()-(zBufferScale.max()-zBufferScale.min())*zPlane.min()*bp->z/FLOAT_CONST_PREC;
			bp->z=zBufferScale.y-(zBufferScale.y-zBufferScale.x)*zPlane.x*bp->z/FLOAT_CONST_PREC;
#else //_ZBUFFER_LINEAR_
			bp->z=pa->pv.z/65535;
#endif //_ZBUFFER_LINEAR_
//				assert(((zBufferScale.min()-0.0001f)<bp->z)&&(bp->z<(zBufferScale.max()+0.0001f)));
//				assert((0<bp->w)&&(bp->w<1));
		}

	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	cInterfaceGraph3d *Graph3d=RenderDevice->GetIGraph3d();
	Graph3d->SetMaterial(eMaterialMode(GET_RENDER_TYPE(Attribute)));
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
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,false);
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
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) div_zv=(pv.z+height); 
				else div_zv=Camera->GetPos().z;
				assert(div_zv!=0);
				div_zv=1/div_zv; 
				Vect3f pv1(pv.x,pv.y,pv.z+height);
				Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
				SetPointFix(CurrentPoint++,pe1,rc,gc,bc,255,pv1);
				for(int k=1;k<(NumberAngle+1);k++)
				{
					int angle=(k-1)*GRAD_TO_DGRAD(360)/NumberAngle;
					Vect3f pv1(pv.x+size*COS_FLOAT_DGRAD(angle),pv.y+size*SIN_FLOAT_DGRAD(angle),pv.z+height);
					Vect3f pe1(pv1.x*div_zv,pv1.y*div_zv,div_zv);
					SetPointFix(CurrentPoint++,pe1,0,0,0,0,pv1);
				}
				AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+1,PreviousPoints+NumberAngle);
				for(int i=1;i<NumberAngle;i++)
					AddPolygonFixTestPointFix(PreviousPoints+0,PreviousPoints+i+1,PreviousPoints+i);
			}
			Draw(Camera,RenderDevice);
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,true);
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
__forceinline void SetPointMesh(int i,cPolyDispatcher *P3D,cLight *LightObject,
						cConvertor &ConvertorObjectToScreen,cConvertor &ConvertorObjectToWorld,
						Vect3f &vLight,Vect3f &vView,float cosLightView,
						int RenderDiffuse,int RenderMetal,int RenderShade,
						sColor4s &AmbientMesh,sColor4s &MulTile,sColor4s &AddTile,sColor4s &MetalColor,
						sPoint *Point,sTexel *Texel=0)
{
	int dr,dg,db,ar,ag,ab;
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
	if(RenderMetal)
	{
		int ToneSpecular=LightObject->CalcToneMetal(Point->normal,vLight,vView,cosLightView);
		assert(0<=ToneSpecular&&ToneSpecular<=255);
		if((ar=AddTile.r+((ToneSpecular*MetalColor.r)>>7))>255) ar=255;
		if((ag=AddTile.g+((ToneSpecular*MetalColor.g)>>7))>255) ag=255;
		if((ab=AddTile.b+((ToneSpecular*MetalColor.b)>>7))>255) ab=255;
	}
	else 
		ar=AddTile.r,ag=AddTile.g,ab=AddTile.b;
	ConvertorObjectToScreen.ConvertPoint(Point->pos,P3D->PointAttribute[i].pv,*(Vect3f*)&P3D->PointFix[i]);
	if(Texel)
		P3D->SetPointFix(i,dr,dg,db,MulTile.a,ar,ag,ab,255,Vect2f(Texel->u(),Texel->v()));
	else
		P3D->SetPointFix(i,dr,dg,db,MulTile.a,ar,ag,ab,255);
}
__forceinline void SetPointMesh(int i,cPolyDispatcher *P3D,sVertexFix &p,sPointAttribute &pa,sTexel &Texel)
{
	P3D->PointFix[i]=p;
	P3D->PointAttribute[i]=pa;
	P3D->PointFix[i].u1()=Texel.u();
	P3D->PointFix[i].v1()=Texel.v();
}
//int gb_ShowMesh=1;
void cPolyDispatcher::Draw(cUnknownClass *UScene,cUnknownClass *UCameraList,cMesh *Mesh,Vect3f *vReflection)
{
//	if(gb_ShowMesh==0&&M3D_TYPE(Mesh->Type)==M3D_ENGINE)return;
	assert(UScene->GetKind(KIND_SCENE));
	cScene *Scene=(cScene*)UScene;
	cLight *LightObject=Scene->GetLight();
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;

	// расчет освещенности объекта 
	sColor4s AmbientMesh,MulMesh,AddMesh,MetalColor(255,255,255,255);
	int RenderDiffuse;
	if(GET_RENDER_TUNING(RENDER_TUNING_DYNAMIC_LIGHTING)&&(Mesh->GetAttribute(MESH_NOT_LIGHTING)==0))
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
			cConvertor ConvertorObjectToScreen;
			if(vReflection==0)
				ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*Mesh->BuildDrawMatrix(Camera,ConvertorObjectToScreen.GetMatrix());
			else 
			{
				ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*Mesh->BuildDrawReflectionMatrix(Camera,ConvertorObjectToScreen.GetMatrix(),*vReflection);
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
			}
			ConvertorObjectToScreen.BuildMatrix(Mesh->GetScale());
			ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
			if(Mesh->GetAttribute(MESH_NOT_WRITEZBUFFER))
				Graph3d->SetRenderState(RENDERSTATE_ZWRITE,false);
			// собственно расчет координат и освещенности
			for(int i=0;i<Mesh->GetNumberTile();i++)
				{
					sTile *tile=Mesh->GetTile(i);
					sPoint	*Point=tile->GetPoint();
					sPolygon *Polygon=tile->GetPolygon();
					Vect3f pv,pe; // pv.z-z-координата расстояния до характерной точки объекта
//					float zDistance=ConvertorObjectToScreen.Matrix[6]*Point->pos.x+ConvertorObjectToScreen.Matrix[7]*Point->pos.y+ConvertorObjectToScreen.Matrix[8]*Point->pos.z+ConvertorObjectToScreen.Matrix[11];
					ConvertorObjectToScreen.ConvertPoint(Point->pos,pv,pe);
					float fLOD=Mesh->Scale.x*tile->size/pv.z;
					if(fLOD>=0&&fLOD<LOD_VALUE) 
						continue;
					if(tile->GetAttribute(ATTRMAT_TEXTURE_PAL)) 
					{
						if(tile->Texture->nTexture==0) CreateTexture((cMaterial*)tile,RenderDevice);
						Graph3d->SetTexture(tile->Texture->nTexture); // óñòàíîâêà òåêñòóðû
					}
					sColor4s MulTile(round(tile->MulCol.r*MulMesh.r),round(tile->MulCol.g*MulMesh.g),
						round(tile->MulCol.b*MulMesh.b),round(Alpha*tile->MulCol.a)),
						AddTile(AddMesh.r+tile->AddCol.GetR(),AddMesh.g+tile->AddCol.GetG(),AddMesh.b+tile->AddCol.GetB(),255);
					if(AddTile.r>255) AddTile.r=255;
					if(AddTile.g>255) AddTile.g=255;
					if(AddTile.b>255) AddTile.b=255;
					// инициализация буффера диспетчера растеризации
					int AttributeTile=MATERIAL_ATTRIBUTE(tile->GetAttribute()|Mesh->Attribute);
//					int RenderMetal=(GET_RENDER_TUNING(RENDER_TUNING_METAL)&&((Attribute&MESH_NOT_METALL)==0)&&(tile->GetAttribute(ATTRMAT_METAL)))&&RenderLighting;
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

					switch(tile->Attribute.GetAttribute(ATTRTILE_VERTEX|ATTRTILE_VERTEX_TEXEL|ATTRTILE_TEXEL))
					{
						case ATTRTILE_VERTEX:
							{
								InitFix(BaseAttribute,tile->GetNumberPoint());
								for(int i=0;i<tile->GetNumberPoint();i++)
									SetPointMesh(i,this,LightObject,
										ConvertorObjectToScreen,ConvertorObjectToWorld,
										vLight,vView,cosLightView,
										RenderDiffuse,RenderMetal,RenderShade,
										AmbientMesh,MulTile,AddTile,MetalTile,
										&Point[i]);
								// вставляются полигоны и одновременно идет отсортировка back-полигонов (невидимых, соответствующих обратной стороне объекта)
								if(PolygonOrientationCCW)
									for(sPolygon *p=&Polygon[0],*end=&p[tile->GetNumberPolygon()];p<end;p++)
										AddPolygonFixTestPointFix(p->p1,p->p2,p->p3);
								else
									for(sPolygon *p=&Polygon[0],*end=&p[tile->GetNumberPolygon()];p<end;p++)
										AddPolygonFixTestPointFix(p->p1,p->p3,p->p2);
							}
							break;
						case ATTRTILE_VERTEX_TEXEL:
							{
								sTexel	*Texel=tile->GetTexel();
								InitFix(BaseAttribute,tile->GetNumberPoint());
								for(int i=0;i<tile->GetNumberPoint();i++)
									SetPointMesh(i,this,LightObject,
										ConvertorObjectToScreen,ConvertorObjectToWorld,
										vLight,vView,cosLightView,
										RenderDiffuse,RenderMetal,RenderShade,
										AmbientMesh,MulTile,AddTile,MetalTile,
										&Point[i],&Texel[i]);
								// вставляются полигоны и одновременно идет отсортировка back-полигонов (невидимых, соответствующих обратной стороне объекта)
								if(PolygonOrientationCCW)
									for(sPolygon *p=&Polygon[0],*end=&p[tile->GetNumberPolygon()];p<end;p++)
										AddPolygonFixTestPointFix(p->p1,p->p2,p->p3);
								else
									for(sPolygon *p=&Polygon[0],*end=&p[tile->GetNumberPolygon()];p<end;p++)
										AddPolygonFixTestPointFix(p->p1,p->p3,p->p2);
							}
							break;
						case ATTRTILE_TEXEL:
							{
								sTexel	*Texel=tile->GetTexel();
								sPolygon *TexPoly=tile->GetTexPoly();
								InitFix(BaseAttribute,3*tile->GetNumberPolygon());
								assert((PointFix.MaxSize-tile->GetNumberPoint()-1)>0);
								sVertexFix *BufPoint=&PointFix[PointFix.MaxSize-tile->GetNumberPoint()-1];
								sPointAttribute *BufPointAttribute=&PointAttribute[PointAttribute.MaxSize-tile->GetNumberPoint()-1];

								for(int i=0;i<tile->GetNumberPoint();i++)
								{
									int dr,dg,db;
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
									BufPoint[i].dr()=dr; BufPoint[i].dg()=dg; BufPoint[i].db()=db; BufPoint[i].da()=MulTile.a;
									if(RenderMetal)
									{
										int ToneSpecular=LightObject->CalcToneMetal(Point[i].normal,vLight,vView,cosLightView),ar,ag,ab;
										assert(0<=ToneSpecular&&ToneSpecular<=255);
										if((ar=AddTile.r+((ToneSpecular*MetalColor.r)>>7))>255) ar=255;
										if((ag=AddTile.g+((ToneSpecular*MetalColor.g)>>7))>255) ag=255;
										if((ab=AddTile.b+((ToneSpecular*MetalColor.b)>>7))>255) ab=255;
										BufPoint[i].sr()=ar,BufPoint[i].sg()=ag,BufPoint[i].sb()=ab,BufPoint[i].sa()=255;
									}
									else 
										BufPoint[i].sr()=AddTile.r,BufPoint[i].sg()=AddTile.g,BufPoint[i].sb()=AddTile.b,BufPoint[i].sa()=255;
									ConvertorObjectToScreen.ConvertPoint(Point[i].pos,BufPointAttribute[i].pv,*(Vect3f*)&BufPoint[i]);
									BufPointAttribute[i].clip=GET_CLIP(BufPoint[i].xe,BufPoint[i].ye,BufPointAttribute[i].pv.z);
								}
								for(int i=0;i<tile->GetNumberPolygon();i++)
								{ 
									// передача геометрических координат
									int i3=i*3;
									Vect3f pv,pe;
									sPolygon &p=Polygon[i];
									sPolygon &t=TexPoly[i];
									SetPointMesh(i3+0,P3D,BufPoint[p.p1],BufPointAttribute[p.p1],Texel[t.p1]);
									SetPointMesh(i3+1,P3D,BufPoint[p.p2],BufPointAttribute[p.p2],Texel[t.p2]);
									SetPointMesh(i3+2,P3D,BufPoint[p.p3],BufPointAttribute[p.p3],Texel[t.p3]);
								}
								// вставляются полигоны и одновременно идет отсортировка back-полигонов (невидимых, соответствующих обратной стороне объекта)
								if(PolygonOrientationCCW)
									for(int i3=0;i3<3*tile->GetNumberPolygon();i3+=3)
										AddPolygonFixTestPointFix(i3+0,i3+1,i3+2);
								else
									for(int i3=0;i3<3*tile->GetNumberPolygon();i3+=3)
										AddPolygonFixTestPointFix(i3+0,i3+2,i3+1);
							}
							break;
					}
					Draw(Camera,RenderDevice);
				}
			if(Mesh->GetAttribute(MESH_NOT_WRITEZBUFFER))
				Graph3d->SetRenderState(RENDERSTATE_ZWRITE,true);
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
						ConvertorObjectToScreen.GetMatrix()=bTile->DrawMatrix[nCamera],ShareOfs=bTile->ShareOfs[nCamera];
						ConvertorObjectToScreen.BuildMatrix();
						ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
						cBaseDynArray <sPointTile> &Point=bTile->PointReflection;
						cBaseDynArray <sPolygonFix> &Polygon=bTile->PolygonReflection;
						int AttributeRender=RENDER_COLOR_MOD_TEXTURE1|RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
						if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) AttributeRender|=RENDER_CLIPPING3D;
						InitFix(AttributeRender,Point.length());
						int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
						int i;
						for(i=0;i<Point.length();i++)
						{
							sPointTile &p=Point[i];
							int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy, xr=xofs+x,yr=yofs+y;
							ConvertorObjectToScreen.ConvertPoint(Vect3f((float)x,(float)y,(float)((int)p.zw+(int)p.dz)-SHARE_FLOAT(xr*xr+yr*yr)),PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
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
				ConvertorObjectToScreen.GetMatrix()=bTile->DrawMatrix[nCamera],ShareOfs=bTile->ShareOfs[nCamera];
				ConvertorObjectToScreen.BuildMatrix();
				ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
				cBaseDynArray <sPointTile> &Point=bTile->Point;
				cBaseDynArray <sPolygonFix> &Polygon=bTile->Polygon;
				int AttributeRender=RENDER_COLOR_MOD_TEXTURE1;
				if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) AttributeRender|=RENDER_CLIPPING3D;
				InitFix(AttributeRender,Point.length());
				int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y);
				int i;
				for(i=0;i<Point.length();i++)
				{
					sPointTile &p=Point[i];
					int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy,xr=xofs+x, yr=yofs+y;
					ConvertorObjectToScreen.ConvertPoint(Vect3f(x,y,(float)((int)p.zw+(int)p.dz)-SHARE_FLOAT(xr*xr+yr*yr)),PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
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
/*
				Draw(Camera,RenderDevice);
				if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
					if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
					{
						Graph3d->SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
						this->Attribute=RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_MULTICANAL;
						Draw(Camera,RenderDevice);
					}
*/			}
		Graph3d->SetRenderState(RENDERSTATE_TEXTUREADDRESS,TADDRESS_WRAP);
	}
	for(sTileMap *bTile=TileMap->GetTile(0,0),*eTile=TileMap->GetTile(0,TileMap->NumberTileY());bTile<eTile;bTile++)
		bTile->GetLightMap()->ClearStatus(STATUS_TEXTURE_SHADOW);
#endif //_USE_TILEMAP_
}
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
					Graph3d->SetTexture(bTile->GetTexture()->nTexture);				// установка текстуры
					Vect3f ShareOfs;
					cConvertor ConvertorObjectToScreen;
					ConvertorObjectToScreen.GetMatrix()=bTile->DrawMatrix[nCamera],ShareOfs=bTile->ShareOfs[nCamera];
					ConvertorObjectToScreen.BuildMatrix();
					ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
					cBaseDynArray <sPointTile> &Point=bTile->Point;
					cBaseDynArray <sPolygonFix> &Polygon=bTile->Polygon;
					int BaseAttribute=RENDER_REFLECTION|RENDER_COLOR_MOD_TEXTURE1;
					if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) BaseAttribute|=RENDER_CLIPPING3D;
					if(bTile->GetAttribute(ATTR_TILE_SURFACE_REFLECTION)) BaseAttribute|=RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE;
					InitFix(BaseAttribute,Point.length());
					int xofs=round(ShareOfs.x-Camera->GetPos().x),yofs=round(ShareOfs.y-Camera->GetPos().y),flag=0;
					int i;
					for(i=0;i<Point.length();i++)
					{
						sPointTile &p=Point[i];
						int x=(int)p.xw+(int)p.dx,y=(int)p.yw+(int)p.dy;
						float z=(float)((int)p.zw+(int)p.dz);
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
						ConvertorObjectToScreen.ConvertPoint(Vect3f(x,y,z),PointAttribute[i].pv,*(Vect3f*)&PointFix[i]);
						SetPointFix(i,255,255,255,p.da(),Vect2f(p.u,p.v));
					}
					if(flag==0) continue;
					for(i=0;i<Polygon.length();i++)
						AddPolygonFixTestPointFix(Polygon[i].p2,Polygon[i].p1,Polygon[i].p3);
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
/*
					Draw(Camera,RenderDevice);
					if(GET_RENDER_TUNING(RENDER_TUNING_SHADOW))
						if(bTile->GetLightMap()->GetStatus(STATUS_TEXTURE_SHADOW))
						{
							Graph3d->SetTexture(bTile->GetLightMap()->nTexture);				// установка текстуры
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
			Graph3d->SetTexture(PolyGrid->Texture->nTexture);
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
   				int l=round(po.x*po.x+po.y*po.y);
				if(l<RadiusWorldShare2)
					po.z-=SHARE_FLOAT(l);
				else
				{
					float d=sqrt((float)l);
					float k=(2*RadiusWorldShare-d)/d;
					po.x*=k; po.y*=k;
	   				l=round(l*k*k);
					po.z-=2*RadiusWorldShare-SHARE_FLOAT(l);
				}
				ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[bPoint-Point].pv,*(Vect3f*)&PointFix[bPoint-Point]);
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
					Graph3d->SetTexture(PolyGrid->Texture->nTexture);
				}
				InitFix(BaseAttribute,xsize*ysize);
				// установка вершин
				float xpos=ShareOfs.x,ypos=ShareOfs.y,duPoint=du+uofs,dvPoint=dv+vofs,ddu=usize/(xsize-1),ddv=vsize/(ysize-1),uLimit=usize/255,vLimit=vsize/255;
				sPointPolyGrid *bPoint=Point,*ePoint=&Point[xsize*ysize];
				for(;bPoint<ePoint;xpos=ShareOfs.x,ypos+=ystep,duPoint=du+uofs,dvPoint+=ddv)
					for(sPointPolyGrid *lPoint=&bPoint[xsize];bPoint<lPoint;bPoint++,xpos+=xstep,duPoint+=ddu)
					{
						Vect3f po(xpos,ypos,0);
   						int l=round(po.x*po.x+po.y*po.y);
						if(l<RadiusWorldShare2)
							po.z-=SHARE_FLOAT(l);
						else
						{
							float d=sqrt((float)l);
							float k=(2*RadiusWorldShare-d)/d;
							po.x*=k; po.y*=k;
	   						l=round(l*k*k);
							po.z-=2*RadiusWorldShare-SHARE_FLOAT(l);
						}
						ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[bPoint-Point].pv,*(Vect3f*)&PointFix[bPoint-Point]);
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
	if(Material->GetSpecular().r||Material->GetSpecular().g||Material->GetSpecular().b)
		BaseAttribute|=RENDER_COLOR_ADD_SPECULAR;
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
			ConvertorObjectToScreen.ConvertPoint(po,PointAttribute[ij].pv,*(Vect3f*)&PointFix[ij]);
			SetPointFix(ij,p.r,p.g,p.b,p.a,Vect2f(duPoint,dvPoint));
		}
	if(BaseAttribute&RENDER_COLOR_ADD_SPECULAR)
	{
		unsigned int specular=(Material->GetSpecular().GetR()<<16)|(Material->GetSpecular().GetG()<<8)|(Material->GetSpecular().GetB()<<0);
		for(int i=0;i<PointFix.length();i++)
			PointFix[i].specular()=specular;
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

		cMaterial *Material=&(WorldPolyGrid->MaterialArray[0]);
		if(Material->GetAttribute(ATTRMAT_TEXTURE_PAL))
		{
			if(Material->Texture->nTexture==0)
				CreateTexture(Material,RenderDevice);
			Graph3d->SetTexture(Material->Texture->nTexture); // установка текстуры
		}
		Vect2f CameraPosScale(Camera->GetPos().x*GlobalWorldScale.x,Camera->GetPos().y*GlobalWorldScale.y);
		int xView=Cycl(CameraPosScale.x+GlobalWorldSize.x*GlobalWorldScale.x/2,GlobalWorldSize.x*GlobalWorldScale.x),
			yView=Cycl(CameraPosScale.y+GlobalWorldSize.y*GlobalWorldScale.y/2,GlobalWorldSize.y*GlobalWorldScale.y),
			iView=xView/WorldPolyGrid->xstep+1, jView=yView/WorldPolyGrid->ystep+1;
		Graph3d->SetRenderState(RENDERSTATE_ZWRITE,false);
		Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_CW);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,0,0,iView,jView,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,iView,0,WorldPolyGrid->xsize+1,jView,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,0,jView,iView,WorldPolyGrid->ysize+1,Material);
		DrawTilePolyGrid(RenderDevice,Camera,WorldPolyGrid,iView,jView,WorldPolyGrid->xsize+1,WorldPolyGrid->ysize+1,Material);
		Graph3d->SetRenderState(RENDERSTATE_CULLMODE,RENDERSTATE_CULL_NONE);
		Graph3d->SetRenderState(RENDERSTATE_ZWRITE,true);
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
					Graph3d->SetTexture(Material->Texture->nTexture); // установка текстуры
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
				Graph3d->SetTexture(Material->Texture->nTexture);
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
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,false);
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
			Graph3d->SetRenderState(RENDERSTATE_ZWRITE,true);
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
                if (TangentTrail->MaterialArray.length() == 0) {
                   continue;
                }
		cMaterial *Material=&TangentTrail->MaterialArray[0];
		sColor4f &Diffuse1=TangentTrail->Diffuse1,&Diffuse2=TangentTrail->Diffuse2;
		float Phase=TangentTrail->Timer()/TangentTrail->Duration,Intensity=255.f;
//		float Phase=TangentTrail->CurrentTime/TangentTrail->FinishTime,Intensity=255.f;
		if(TangentTrail->AnimTime<1e10f)
			Intensity=128+127*sin(2*M_PI*global_time()/TangentTrail->AnimTime);
//			Intensity=128+127*sin(2*M_PI*(TangentTrail->StartTime+TangentTrail->CurrentTime)/TangentTrail->AnimTime);
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
			Graph3d->SetTexture(Material->Texture->nTexture); // установка текстуры
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
	Graph3d->SetTexture(TextureChild.Texture->nTexture);
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
	Graph3d->SetTexture(TextureChild.Texture->nTexture);
	int RenderAttribute=0;
	if(CurrentCamera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) RenderAttribute|=RENDER_CLIPPING3D;
	InitFix(RenderAttribute|RENDER_COLOR_MOD_DIFFUSE|RENDER_COLOR_MOD_TEXTURE1|RENDER_ALPHA_MOD_DIFFUSE|RENDER_ALPHA_MOD_TEXTURE1);
}
void cPolyDispatcher::AttachCenterShare(const Vect3f &pos,sSpriteFX *SpriteFX,int idTextureChild)
{
	cRenderDevice *RenderDevice=GetRenderDevice(0);
	sTextureChild &TextureChild=TextureBuffer->GetTextureChild(idTextureChild);
	Vect3f pv,pe,pv1,pe1,Pos=pos;
	int xr=GetShortDistX(int(round(pos.x)),int(round(CurrentCamera->GetPos().x)),2048),
		yr=GetShortDistY(int(round(pos.y)),int(round(CurrentCamera->GetPos().y)),2048);
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
	Graph3d->SetTexture(TextureChild.Texture->nTexture);
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
	GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,false);
	Draw(CurrentCamera,GetRenderDevice(0));
	GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,true);
	CurrentCamera=0;
	AlphaForSprite=255;
}
void cPolyDispatcher::SetViewColor(cUnknownClass *UCamera,const sColor4f &Diffuse,const sColor4f &Specular,int zWrite)
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
	if(!zWrite) GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,false);
	Draw(Camera,GetRenderDevice(0));
	if(!zWrite) GetRenderDevice(0)->GetIGraph3d()->SetRenderState(RENDERSTATE_ZWRITE,true);
}
//#endif _ONLY_DIRECT3D_
