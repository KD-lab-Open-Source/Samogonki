#include "TileMap.h"
#include "PolyMgr.h"
#include "DrawPolygonShade.h"
#include "BaseDefine.h"
#include "File3ds.h"

#define TEXTURE_NOT_CYCLIC			0.0035f

// переменные для работы c cPolyMgr
int PosBufTileMap=0;
unsigned short *GlobalColorBufferMap=0;

int GlobalMapTextureDetail=0;
float uvPrecisionNotCyclic=TEXTURE_NOT_CYCLIC;
int GlobalAlfaReflectionSurface=210;
int GlobalRenderTuning=	//RENDER_TUNING_REFLECTION|
						RENDER_TUNING_REFLECTION_SURFACE_SHADOW|
						RENDER_TUNING_SHADOW|
						RENDER_TUNING_REFLECTION_MECHOS|
						RENDER_TUNING_REFLECTION_STATIC_OBJECT|
						RENDER_TUNING_DYNAMIC_LIGHTING|
						RENDER_TUNING_TILEMAP|
						RENDER_TUNING_WATER|
						RENDER_TUNING_SKY|
						RENDER_TUNING_CLOUD|
						RENDER_TUNING_SUN|
						RENDER_TUNING_OMNI|
						RENDER_TUNING_BASETRAIL;

int FindPoint(cBaseDynArray <sPointTile> &Point,int x,int y,int z,float u,float v)
{
	for(int  i=0;i<Point.length();i++)
		if((Point[i].xw==x)&&(Point[i].yw==y)&&(Point[i].zw==z))
/*		if((Point[i].xw==x)&&(Point[i].yw==y)&&(Point[i].zw==z)&&
			((fabs(Point[i].u-u)==1.f)||(fabs(Point[i].v-v)==1.f)))
*/				return i;
	return -1;
}
inline int FindPointAttribute(cBaseDynArray <sPointTile> &Point,sPointTile &pFind,int &NumberPoint,int attribute)
{
	int alfa;
	if((attribute&TERRA_Tr_MASK)==TERRA_At_ICE) 
		alfa=GlobalAlfaReflectionSurface; 
	else 
		alfa=255;
	for(int i=0;i<NumberPoint;i++)
		if((Point[i].xw==pFind.xw)&&(Point[i].yw==pFind.yw)&&(Point[i].zw==pFind.zw)&&(Point[i].da()==alfa))
			return i;
	Point(NumberPoint)=pFind;
	Point(NumberPoint).da(alfa);
	return NumberPoint++;
}
inline float GetPolygonAngle(const Vect3f &a,const Vect3f &b,const Vect3f &c)
{
	Vect3f ab=b-a,ac=c-a;
	Vect3f normal=ab%ac; normal.normalize();
	float angle=(float)acos(fabs(dot(normal,Vect3f(0,0,1))));
	return R2G(angle);
}
inline float GetPolygonZ(const Vect3f &a,const Vect3f &b,const Vect3f &c)
{
	float center=(a.z+b.z+c.z)*0.333f;
	float da=fabs(center-a.z),db=fabs(center-b.z),dc=fabs(center-c.z);
	if(da>db)
		if(da>dc) return da;
		else return dc;
	else 
		if(db>dc) return db;
		else return dc;
}
struct sPointAlpha
{
	short x,y;
	unsigned short alpha;
	unsigned short number;
};
inline int GetPointAlpha(cBaseArray <sPointAlpha> &PointAlpha,short x,short y)
{
	for(int i=0;i<PointAlpha.length();i++)
		if(x==PointAlpha[i].x&&y==PointAlpha[i].y)
			return PointAlpha[i].alpha;
	return 255;
}
inline void AddPointAlpha(cBaseArray <sPointAlpha> &PointAlpha,short x,short y,int alpha)
{
	int i;
	for(i=0;i<PointAlpha.length();i++)
		if(x==PointAlpha[i].x&&y==PointAlpha[i].y)
		{
			PointAlpha[i].alpha+=alpha;
			PointAlpha[i].number++;
			return;
		}
	sPointAlpha &p=PointAlpha(i);
	p.x=x; p.y=y; p.alpha=alpha; p.number=1;
}
int cTileMap::Load3ds(char *fname)
{
	cFile3ds f;
	if(f.Open(fname,2)==0) return 0;
	xTile=yTile=16;
	_xTile=_yTile=7;
	if(Tile) delete [] Tile;
	Tile=new sTileMap[xTile*yTile];
	float dx=1.f/(1<<_xTile), dy=1.f/(1<<_yTile);
	GetZReflectionSurface()=-1;
	char TextureName[17],NameMesh[22]; // Shading,OpacityName[17];
	int NumberObject3ds=f.OpenBaseMesh(),nPoint,nPolygon;
	cBaseArray <sPointAlpha> PointAlpha(50000,10000);
//	P3D->SaveAttribute("attribute.raw");
//	P3D->SaveVoxel("voxel.raw");
	int i;
	for(i=0;i<NumberObject3ds;i++)
	{
		f.OpenMesh(i,&nPoint,&nPolygon,NameMesh);
		unsigned int AttributeMaterial=ATTRMAT_ATTRIBUTE_NULL;
		float *Vertex=new float[5*nPoint],Matrix[12]; //fTransparency,ShinStrength,Shininess,rDiffuse,gDiffuse,bDiffuse;
		int *Face=new int[3*nPolygon];
		f.ReadMesh(Vertex,Face,(char*)TextureName,Matrix);

		float xCount=0,yCount=0;
		for(int n=0;n<nPoint;n++)
		{
			xCount+=Vertex[5*n+0];
			yCount+=Vertex[5*n+1];
		}
		int iTile=round(xCount/nPoint)/(1<<_xTile);
		int jTile=round(yCount/nPoint)/(1<<_yTile);

		sTileMap *tmp=GetTile(iTile,jTile);
		assert(tmp->GetTexture()==0);
		tmp->GetTexture()=new sTexture;
		uvPrecisionNotCyclic=TEXTURE_NOT_CYCLIC*(1+GlobalMapTextureDetail); 
		tmp->GetTexture()->_x=_xTile-GlobalMapTextureDetail; 
		tmp->GetTexture()->_y=_yTile-GlobalMapTextureDetail;
		tmp->GetTexture()->SetStatus(STATUS_TEXTURE_LOAD);

		tmp->GetLightMap()=new sTexture;
		tmp->GetLightMap()->_x=_xTile-ReturnBit(MAP_LIGHTMAP_STEP);
		tmp->GetLightMap()->_y=_yTile-ReturnBit(MAP_LIGHTMAP_STEP);
		tmp->GetLightMap()->SetStatus(STATUS_TEXTURE_LOAD);
		
		cBaseDynArray <sPointTile> PointTmp;
		PointTmp.New(nPoint+1000); 
		tmp->xPos=iTile<<_xTile, tmp->yPos=jTile<<_yTile;
		tmp->zwMin=65536,tmp->zwMax=0;
		int k;
		for(k=0;k<nPoint;k++)
		{
			sPointTile &p=PointTmp[k];
			int xw=round(Vertex[5*k+0])-tmp->xPos,
				yw=round(Vertex[5*k+1])-tmp->yPos,
				zw=round(Vertex[5*k+2]);
			if(xw>USHRT_MAX) xw=USHRT_MAX; else if(xw<0) xw=0;
			if(yw>USHRT_MAX) yw=USHRT_MAX; else if(yw<0) yw=0;
			if(zw>USHRT_MAX) zw=USHRT_MAX; else if(zw<0) zw=0;
			p.xw=xw; p.yw=yw; p.zw=zw; p.dx=p.dy=p.dz=0;
			if(zw<tmp->zwMin) tmp->zwMin=zw; if(zw>tmp->zwMax) tmp->zwMax=zw;
			p.u=p.xw*dx; p.v=p.yw*dy;
			if(p.u<uvPrecisionNotCyclic) p.u=uvPrecisionNotCyclic; else if(p.u>(1.f-uvPrecisionNotCyclic)) p.u=1.f-uvPrecisionNotCyclic;
			if(p.v<uvPrecisionNotCyclic) p.v=uvPrecisionNotCyclic; else if(p.v>(1.f-uvPrecisionNotCyclic)) p.v=1.f-uvPrecisionNotCyclic;
		}
		int NumberPoint=0,NumberPointReflection=0;
		int NumberPolygon=0,NumberPolygonReflection=0;
		cBaseDynArray <sPointTile> &Point=tmp->Point; Point.New(nPoint);
		cBaseDynArray <sPolygonFix> &Polygon=tmp->Polygon;	Polygon.New(nPolygon);
		cBaseDynArray <char> &AttrPolygon=tmp->AttrPolygon;	AttrPolygon.New(nPolygon);
		cBaseDynArray <sPointTile> &PointReflection=tmp->PointReflection; PointReflection.New(nPoint);
		cBaseDynArray <sPolygonFix> &PolygonReflection=tmp->PolygonReflection; PolygonReflection.New(nPolygon);
		cBaseDynArray <char> &AttrPolygonReflection=tmp->AttrPolygonReflection;	AttrPolygonReflection.New(nPolygon);
		for(k=0;k<nPolygon;k++)
		{
			int p1=Face[3*k+0],p2=Face[3*k+2],p3=Face[3*k+1],i1,i2,i3;
			if(p1==p2||p1==p3||p2==p3) continue;
			int attribute=P3D->GetAttribute(PointTmp[p1].xw+tmp->xPos,PointTmp[p1].yw+tmp->yPos,
				PointTmp[p2].xw+tmp->xPos,PointTmp[p2].yw+tmp->yPos,
				PointTmp[p3].xw+tmp->xPos,PointTmp[p3].yw+tmp->yPos);
//			if((attribute&TERRA_Tr_MASK)==TERRA_At_ICE) attribute&=~0xE;
			if(attribute&TERRA_At_WATER)
			{
				tmp->SetAttribute(ATTR_TILE_SURFACE_WATER);
				if(tmp->zwMax<vMap_LevelH2O()) tmp->zwMax=vMap_LevelH2O();
			}
			if(((attribute&TERRA_Tr_MASK)==TERRA_At_ICE)&&(GetPolygonZ(Vect3f(PointTmp[p1].xw,PointTmp[p1].yw,PointTmp[p1].zw),Vect3f(PointTmp[p2].xw,PointTmp[p2].yw,PointTmp[p2].zw),Vect3f(PointTmp[p3].xw,PointTmp[p3].yw,PointTmp[p3].zw))<5))
			{
				i1=FindPointAttribute(PointReflection,PointTmp[p1],NumberPointReflection,attribute);
				i2=FindPointAttribute(PointReflection,PointTmp[p2],NumberPointReflection,attribute);
				i3=FindPointAttribute(PointReflection,PointTmp[p3],NumberPointReflection,attribute);
				tmp->SetAttribute(ATTR_TILE_SURFACE_REFLECTION|ATTR_TILE_REFLECTION);
				GetZReflectionSurface()=tmp->zReflectionSurface=(PointTmp[p1].zw+PointTmp[p2].zw+PointTmp[p3].zw)/3;
				if(i1==i2||i1==i3||i2==i3) continue;
				AttrPolygonReflection[NumberPolygonReflection]=attribute;
				sPolygonFix &p=PolygonReflection[NumberPolygonReflection++];
				p.p1=i1; p.p2=i2; p.p3=i3;
				AddPointAlpha(PointAlpha,PointTmp[p1].xw+tmp->xPos,PointTmp[p1].yw+tmp->yPos,GlobalAlfaReflectionSurface);
				AddPointAlpha(PointAlpha,PointTmp[p2].xw+tmp->xPos,PointTmp[p2].yw+tmp->yPos,GlobalAlfaReflectionSurface);
				AddPointAlpha(PointAlpha,PointTmp[p3].xw+tmp->xPos,PointTmp[p3].yw+tmp->yPos,GlobalAlfaReflectionSurface);
			}
			else
			{
				int i1=FindPointAttribute(Point,PointTmp[p1],NumberPoint,attribute&(~TERRA_At_ICE));
					i2=FindPointAttribute(Point,PointTmp[p2],NumberPoint,attribute&(~TERRA_At_ICE));
					i3=FindPointAttribute(Point,PointTmp[p3],NumberPoint,attribute&(~TERRA_At_ICE));
				if(i1==i2||i1==i3||i2==i3) continue;
				AttrPolygon[NumberPolygon]=attribute;
				sPolygonFix &p=Polygon[NumberPolygon++];
				p.p1=i1; p.p2=i2; p.p3=i3;
				AddPointAlpha(PointAlpha,PointTmp[p1].xw+tmp->xPos,PointTmp[p1].yw+tmp->yPos,10*255);
				AddPointAlpha(PointAlpha,PointTmp[p2].xw+tmp->xPos,PointTmp[p2].yw+tmp->yPos,10*255);
				AddPointAlpha(PointAlpha,PointTmp[p3].xw+tmp->xPos,PointTmp[p3].yw+tmp->yPos,10*255);
			}
		}
		PointTmp.Delete();
		delete Vertex; delete Face;
		Point.Resize(NumberPoint); Polygon.Resize(NumberPolygon); AttrPolygon.Resize(NumberPolygon); 
		PointReflection.Resize(NumberPointReflection); PolygonReflection.Resize(NumberPolygonReflection); AttrPolygonReflection.Resize(NumberPolygonReflection);
		f.CloseMesh();
	}
	f.CloseBaseMesh();
	f.Close();
#ifdef _DEBUG	
	int k;
	for(k=0;k<xTile;k++)
	{
		for(int j=0;j<yTile;j++)
		{
			sTileMap &Tile1=Tile[((k+1)&(xTile-1))+j*xTile];
			sTileMap &Tile2=Tile[k+j*xTile];
			int i;
			for(i=0;i<Tile1.Point.length();i++)
				if(Tile1.Point[i].xw==0)
					if(FindPoint(Tile2.Point,1<<_xTile,Tile1.Point[i].yw,Tile1.Point[i].zw,Tile1.Point[i].u,Tile1.Point[i].v)<0)
						XCon<"Warning: cTileMap::Load() - Point bound not cycled "<=(k<<_xTile)+Tile1.Point[i].xw<" "<=(j<<_yTile)+Tile1.Point[i].yw<"\r\n";
			for(i=0;i<Tile2.Point.length();i++)
				if(Tile2.Point[i].xw==(1<<_xTile))
					if(FindPoint(Tile1.Point,0,Tile2.Point[i].yw,Tile2.Point[i].zw,Tile2.Point[i].u,Tile2.Point[i].v)<0)
						XCon<"Warning: cTileMap::Load() - Point bound not cycled "<=(k<<_xTile)+Tile2.Point[i].xw<" "<=(j<<_yTile)+Tile2.Point[i].yw<"\r\n";
		}
	}
	for(k=0;k<yTile;k++)
	{
		for(int j=0;j<xTile;j++)
		{
			sTileMap &Tile1=Tile[j+((k+1)&(yTile-1))*xTile];
			sTileMap &Tile2=Tile[j+k*xTile];
			int i;
			for(i=0;i<Tile1.Point.length();i++)
				if(Tile1.Point[i].yw==0)
					if(FindPoint(Tile2.Point,Tile1.Point[i].xw,1<<_yTile,Tile1.Point[i].zw,Tile1.Point[i].u,Tile1.Point[i].v)<0)
						XCon<"Warning: cTileMap::Load() - Point bound not cycled "<=(j<<_xTile)+Tile1.Point[i].xw<" "<=(k<<_yTile)+Tile1.Point[i].yw<"\r\n";
			for(i=0;i<Tile2.Point.length();i++)
				if(Tile2.Point[i].yw==(1<<_yTile))
					if(FindPoint(Tile1.Point,Tile2.Point[i].xw,0,Tile2.Point[i].zw,Tile2.Point[i].u,Tile2.Point[i].v)<0)
						XCon<"Warning: cTileMap::Load() - Point bound not cycled "<=(j<<_xTile)+Tile2.Point[i].xw<" "<=(k<<_yTile)+Tile2.Point[i].yw<"\r\n";
		}
	}
#endif
	for(i=0;i<PointAlpha.length();i++)
	{
		PointAlpha[i].alpha=PointAlpha[i].alpha/PointAlpha[i].number;
		if(PointAlpha[i].alpha>255) PointAlpha[i].alpha=255;
	}
	for(int j=0;j<yTile;j++)
		for(int i=0;i<xTile;i++)
			for(int n=0;n<GetTile(i,j)->PointReflection.length();n++)
				GetTile(i,j)->PointReflection[n].a=GetPointAlpha(PointAlpha,
					GetTile(i,j)->PointReflection[n].xw+GetTile(i,j)->xPos,
					GetTile(i,j)->PointReflection[n].yw+GetTile(i,j)->yPos);
	PointAlpha.Delete();
	if(GetTile(0,0)->GetTexture()->GetStatus(STATUS_TEXTURE_LOAD)) 
	{
		P3D->CreateTexture(this);
		vMap_releaseClBuf();
	}
	return 1;
}
void SetFileExtension(char *fname,char *extension)
{
	int end=strlen(fname),count=end;
	while((count>=0)&&((end-count)<=4)&&(fname[count]!='.')) count--;
	if((end-count)<=4) 
	{
		int extlen=strlen(extension);
		for(int i=0;i<extlen;i++)
			fname[count+1+i]=extension[i];
	}
}
void cTileMap::Load(char *fname)
{
#ifdef _MECHOSOMA_
	extern void allocation_tracking(char*);
//	allocation_tracking("Begin cTileMap::cTileMap()");
#endif
	Attribute=0;
	SetFileExtension(fname,"tlm");
	XStream in(0);
	if(!in.open(fname,XS_IN)) 
	{
		SetFileExtension(fname,"3ds");
		if(Load3ds(fname)) return;
	}
	int TotalNumberPoint=0,TotalNumberPolygon=0;
//	if(!in.open(fname,XS_IN)) { XBuffer buf; buf<"Warning: cTileMap::cTileMap()\r\nFile not found "<fname; ErrAbort(buf.address()); }
	if(Tile) delete [] Tile; 
	GetZReflectionSurface()=-1;
	in>=xTile>=yTile;
	_xTile=ReturnBit((1<<TERRA_H_SIZE_POWER)/xTile);
	_yTile=ReturnBit((1<<TERRA_H_SIZE_POWER)/yTile);
	if((_xTile==0)||(_yTile==0)) { XBuffer buf; buf<"Error: cTileMap::cTileMap()\r\nBad size tile in file "<fname; ErrAbort(buf.address()); }
	Tile=new sTileMap[xTile*yTile];
	int j;
	int i;
	for(j=0;j<yTile;j++)
		for(i=0;i<xTile;i++)
		{
			char type;
			float dx=1.f/(1<<_xTile), dy=1.f/(1<<_yTile);
			
			sTileMap *tmp=&Tile[j*xTile+i];
//			tmp->AttributeMaterial=ATTRMAT_TILE_MAP; 

			tmp->GetTexture()=new sTexture;
			uvPrecisionNotCyclic=TEXTURE_NOT_CYCLIC*(1+GlobalMapTextureDetail); 
			tmp->GetTexture()->_x=_xTile-GlobalMapTextureDetail; 
			tmp->GetTexture()->_y=_yTile-GlobalMapTextureDetail;
			tmp->GetTexture()->SetStatus(STATUS_TEXTURE_LOAD);

			tmp->GetLightMap()=new sTexture;
			tmp->GetLightMap()->_x=_xTile-ReturnBit(MAP_LIGHTMAP_STEP);
			tmp->GetLightMap()->_y=_yTile-ReturnBit(MAP_LIGHTMAP_STEP);
			tmp->GetLightMap()->SetStatus(STATUS_TEXTURE_LOAD);

			int nPoint=0,nPolygon=0,count=0,NumberPolygonBad=0;
			tmp->zwMin=65536,tmp->zwMax=0;
			cBaseDynArray <sPointTile> PointTmp;
			PointTmp.New(10000);
			do {
				in>type;
			} while((type=='\r')||(type=='\n')||(in.eof()));
			if(type!='v') 
				{ XBuffer buf; buf<"Error cTileMap::Load()\r\nError in file = "<fname; ErrAbort(buf.address()); }
			in>=nPoint;
			tmp->xPos=i<<_xTile; tmp->yPos=j<<_yTile; 
			for(count=0;(count<nPoint)&&(!in.eof());)
			{
				sPointTile &p=PointTmp(count++);
				int xw,yw,zw;
				in>=xw>=yw>=zw;
				p.dx=p.dy=p.dz=0;
				xw-=tmp->xPos; yw-=tmp->yPos;
				if(xw>USHRT_MAX) xw=USHRT_MAX; else if(xw<0) xw=0;
				if(yw>USHRT_MAX) yw=USHRT_MAX; else if(yw<0) yw=0;
				if(zw>USHRT_MAX) zw=USHRT_MAX; else if(zw<0) zw=0;
				if(zw<tmp->zwMin) tmp->zwMin=zw; if(zw>tmp->zwMax) tmp->zwMax=zw;
				p.xw=xw; p.yw=yw; p.zw=zw;
				p.u=p.xw*dx; p.v=p.yw*dy;
				if(p.u<uvPrecisionNotCyclic) p.u=uvPrecisionNotCyclic; else if(p.u>(1.f-uvPrecisionNotCyclic)) p.u=1.f-uvPrecisionNotCyclic;
				if(p.v<uvPrecisionNotCyclic) p.v=uvPrecisionNotCyclic; else if(p.v>(1.f-uvPrecisionNotCyclic)) p.v=1.f-uvPrecisionNotCyclic;
			}
			nPoint=count;
			do {
				in>type;
			} while((type=='\r')||(type=='\n')||(in.eof()));
			if(type!='f') 
				{ XBuffer buf; buf<"Error cTileMap::Load()\r\nError in file = "<fname; ErrAbort(buf.address()); }
			in>=nPolygon;
			TotalNumberPolygon+=nPolygon;
			int NumberPoint=0,NumberPointReflection=0;
			int NumberPolygon=0,NumberPolygonReflection=0;
			cBaseDynArray <sPointTile> &Point=tmp->Point; Point.New(nPoint+1000);
			cBaseDynArray <sPolygonFix> &Polygon=tmp->Polygon;	Polygon.New(nPolygon+1000);
			cBaseDynArray <char> &AttrPolygon=tmp->AttrPolygon;	AttrPolygon.New(nPolygon+1000);
			cBaseDynArray <sPointTile> &PointReflection=tmp->PointReflection; PointReflection.New(nPoint+1000);
			cBaseDynArray <sPolygonFix> &PolygonReflection=tmp->PolygonReflection; PolygonReflection.New(nPolygon+1000);
			cBaseDynArray <char> &AttrPolygonReflection=tmp->AttrPolygonReflection;	AttrPolygonReflection.New(nPolygon+1000);
			for(count=0;(count<nPolygon)&&(!in.eof());count++)
			{
				int p1,p2,p3;
				in>=p1>=p3>=p2;
				int attribute=P3D->GetAttribute(PointTmp[p1].xw+tmp->xPos,PointTmp[p1].yw+tmp->yPos,
					PointTmp[p2].xw+tmp->xPos,PointTmp[p2].yw+tmp->yPos,
					PointTmp[p3].xw+tmp->xPos,PointTmp[p3].yw+tmp->yPos);
				if(attribute&TERRA_At_WATER)
				{
					tmp->SetAttribute(ATTR_TILE_SURFACE_WATER);
					if(tmp->zwMax<vMap_LevelH2O()) tmp->zwMax=vMap_LevelH2O();
				}
				if((attribute&TERRA_Tr_MASK)==TERRA_At_ICE)
				{
					sPolygonFix &p=PolygonReflection[NumberPolygonReflection++];
					p.p1=FindPointAttribute(PointReflection,PointTmp[p1],NumberPointReflection,attribute);
					p.p2=FindPointAttribute(PointReflection,PointTmp[p2],NumberPointReflection,attribute);
					p.p3=FindPointAttribute(PointReflection,PointTmp[p3],NumberPointReflection,attribute);
					tmp->SetAttribute(ATTR_TILE_SURFACE_REFLECTION|ATTR_TILE_REFLECTION);
					GetZReflectionSurface()=tmp->zReflectionSurface=(PointTmp[p1].zw+PointTmp[p2].zw+PointTmp[p3].zw)/3;
					AttrPolygonReflection[NumberPolygonReflection-1]=attribute;
				}
				else
				{
					sPolygonFix &p=Polygon[NumberPolygon++];
					p.p1=FindPointAttribute(Point,PointTmp[p1],NumberPoint,attribute);
					p.p2=FindPointAttribute(Point,PointTmp[p2],NumberPoint,attribute);
					p.p3=FindPointAttribute(Point,PointTmp[p3],NumberPoint,attribute);
					AttrPolygon[NumberPolygonReflection-1]=attribute;
				}
			}
			tmp->Point.Resize(NumberPoint); tmp->Polygon.Resize(NumberPolygon);	
			tmp->PointReflection.Resize(NumberPointReflection); tmp->PolygonReflection.Resize(NumberPolygonReflection);	
			assert((tmp->Polygon.length()+NumberPolygonBad)==NumberPolygon);
			TotalNumberPoint+=tmp->Point.length();
		}
	in.close();	
	int SizePoint=TotalNumberPoint*sizeof(sPointTile);
	int SizePolygon=TotalNumberPolygon*sizeof(sPolygonFix);
#ifdef _DEBUG	
	int k;
	for(k=0;k<xTile;k++)
	{
		for(j=0;j<yTile;j++)
		{
			sTileMap &Tile1=Tile[((k+1)&(xTile-1))+j*xTile];
			sTileMap &Tile2=Tile[k+j*xTile];
			for(int i=0;i<Tile1.Point.length();i++)
				if(Tile1.Point[i].xw==0)
					if(FindPoint(Tile2.Point,1<<_xTile,Tile1.Point[i].yw,Tile1.Point[i].zw,Tile1.Point[i].u,Tile1.Point[i].v)<0)
							XCon<"Warning: cTileMap::Load()\r\nPoint bound not cycled\r\n";
			for(i=0;i<Tile2.Point.length();i++)
				if(Tile2.Point[i].xw==(1<<_xTile))
					if(FindPoint(Tile1.Point,0,Tile2.Point[i].yw,Tile2.Point[i].zw,Tile2.Point[i].u,Tile2.Point[i].v)<0)
							XCon<"Warning: cTileMap::Load()\r\nPoint bound not cycled\r\n";
		}
	}
	for(k=0;k<yTile;k++)
	{
		for(int j=0;j<xTile;j++)
		{
			sTileMap &Tile1=Tile[j+((k+1)&(yTile-1))*xTile];
			sTileMap &Tile2=Tile[j+k*xTile];
			for(int i=0;i<Tile1.Point.length();i++)
				if(Tile1.Point[i].yw==0)
					if(FindPoint(Tile2.Point,Tile1.Point[i].xw,1<<_yTile,Tile1.Point[i].zw,Tile1.Point[i].u,Tile1.Point[i].v)<0)
							XCon<"Warning: cTileMap::Load()\r\nPoint bound not cycled\r\n";
			for(i=0;i<Tile2.Point.length();i++)
				if(Tile2.Point[i].yw==(1<<_yTile))
					if(FindPoint(Tile1.Point,Tile2.Point[i].xw,0,Tile2.Point[i].zw,Tile2.Point[i].u,Tile2.Point[i].v)<0)
							XCon<"Warning: cTileMap::Load()\r\nPoint bound not cycled\r\n";
		}
	}
#endif
#ifdef _MECHOSOMA_
//	allocation_tracking("End cTileMap::cTileMap()");
#endif
	if(GetTile(0,0)->GetTexture()->GetStatus(STATUS_TEXTURE_LOAD)) 
	{
		P3D->CreateTexture(this);
		vMap_releaseClBuf();
	}
}
cTileMap::cTileMap() : cUnknownClass(KIND_WORLD_TILEMAP)
{ 
	Tile=0; 
}
cTileMap::~cTileMap()								
{ 
	UNKNOWN_DESTRUCTOR;
}
void cTileMap::Release()
{
	if(Tile) delete [] Tile; Tile=0; 
}
void cTileMap::TestVisible(cUnknownClass *UCameraList)
{
	assert(UCameraList->GetKind(KIND_ARRAYCAMERA));
	cUnkClassDynArrayPointer &CameraArray=*(cUnkClassDynArrayPointer*)UCameraList;
	for(int nCamera=0;nCamera<CameraArray.length();nCamera++)
	{
		assert(CameraArray[nCamera]->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)CameraArray[nCamera];
		for(sTileMap *bTile=&Tile[0],*eTile=&Tile[yTile*xTile];bTile<eTile;bTile++)
		{
			assert(nCamera<2);
			bTile->DrawMatrix[nCamera]=Camera->GetMatrix()*bTile->BuildDrawMatrix(Camera,bTile->DrawMatrix[nCamera],bTile->ShareOfs[nCamera]);
			bTile->GetVisibleTotal(nCamera)=Camera->TestVisibleTile(bTile->DrawMatrix[nCamera],bTile->ShareOfs[nCamera],Vect3f(128.f,128.f,bTile->zwMax),GlobalWorldRadius);
		}
	}
}
void cTileMap::UnlockTexture(cUnknownClass *UCameraList)
{
	P3D->UnlockTexture(UCameraList,this);
}
void cTileMap::Draw(cUnknownClass *UCameraList)
{
	P3D->Draw(UCameraList,this);
}
void cTileMap::DrawReflection(cUnknownClass *UCameraList)
{
	P3D->DrawReflection(UCameraList,this);
}
int cTileMap::SetPolygonFirst()
{ 
	nTile=nPolygon=0; 
	return 0;
}
int cTileMap::GetPolygonNext(Vect3f &v1,Vect3f &v2,Vect3f &v3,int &attribute)
{ 
	assert(Tile); 
	if(nTile>=(xTile*yTile)) return -1; 
	if((Tile[nTile].Polygon.length()+Tile[nTile].PolygonReflection.length())<=nPolygon)
		if((++nTile)<(xTile*yTile)) nPolygon=0;
		else return -1;
	sTileMap &tmp=Tile[nTile];
	sPointTile *p1,*p2,*p3;
	sPolygonFix *p; 
	if(nPolygon<tmp.Polygon.length()) 
	{ 
		p=&tmp.Polygon[nPolygon]; 
		attribute=tmp.AttrPolygon[nPolygon];
		p1=&tmp.Point[p->p1]; 
		p2=&tmp.Point[p->p2]; 
		p3=&tmp.Point[p->p3]; 
	}
	else 
	{ 
		p=&tmp.PolygonReflection[nPolygon-tmp.Polygon.length()]; 
		attribute=tmp.AttrPolygonReflection[nPolygon-tmp.Polygon.length()];
		p1=&tmp.PointReflection[p->p1]; 
		p2=&tmp.PointReflection[p->p2]; 
		p3=&tmp.PointReflection[p->p3]; 
	}
	v1.set(p1->xw+tmp.xPos,p1->yw+tmp.yPos,p1->zw);
	v2.set(p2->xw+tmp.xPos,p2->yw+tmp.yPos,p2->zw);
	v3.set(p3->xw+tmp.xPos,p3->yw+tmp.yPos,p3->zw);
	nPolygon++;
	return 0;
}
