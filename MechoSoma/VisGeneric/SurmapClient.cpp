#include "SurmapClient.h"
#include "Dispatcher.h"
#include "IGraph3d.h"

#ifdef _SURMAP_

cInterfaceVisGeneric	*gb_IVisGeneric=0; 
cInterfaceGraph3d		*gb_IGraph3d=0; 
cUnknownClass			*gb_URenderDevice=0;
cUnknownClass			*gb_UScene=0;
cUnknownClass			*gb_UCamera=0;
cUnkClassDynArrayPointer gb_UArrayCamera(KIND_ARRAYCAMERA);

extern unsigned short	*zBuffer;
unsigned int			PolyCountMask=0,PolyCountTransparency=0;
char					MeshShowID=0,OmniShowID=0,MeshShowBound=0;
unsigned int			MeshTypeShowID=M3D_STATIC;
int hCamera=512,fCamera=512;

///////////////////////////////
int editM3D=-1;
int editM3Dyh=0,editM3Dyd=0;
int editCLS=-1;
float xCLS,yCLS,zCLS,radCLS,rCLS,gCLS,bCLS;
char* dirM3D = "MODEL3D";
char* dirM3Dtexture = "MODEL3D\\IMAGES";
cMesh	*MeshPoint=0;
char	*MeshPointName="cursor.3ds";

void initM3D(int xScr,int yScr,int FullScr)
{
	gb_IVisGeneric=CreateIVisGeneric();
	extern int XGR_HighColorMode;
	int OldXGR_HighColorMode=XGR_HighColorMode; XGR_HighColorMode=0;
	gb_URenderDevice=gb_IVisGeneric->CreateGraph(xScr,yScr,0,0,16);
	XGR_HighColorMode=OldXGR_HighColorMode;
	gb_IGraph3d=gb_IVisGeneric->GetIGraph3d(gb_URenderDevice);
	gb_IVisGeneric->SetGraphClipping(gb_URenderDevice,
		&sRectangle4f(xScr*0.01f,yScr*0.01f,xScr*0.99f,yScr*0.99f));
	zBuffer=(unsigned short*)gb_IGraph3d->GetZBuffer();
	gb_UScene=gb_IVisGeneric->CreateScene();
	gb_IVisGeneric->SetScene(gb_UScene);
	gb_UCamera=gb_IVisGeneric->CreateCamera();
	gb_IVisGeneric->AttachCamera(gb_UCamera);		// ������������� �����v � �������� �����
	gb_IVisGeneric->SetCameraAttribute(gb_UCamera,ATTRIBUTE_CAMERA_WORLD_CUTTING); // ����������� �v���� � �����������
	gb_IVisGeneric->SetCameraPosition(gb_UCamera,&Vect3f(0,0,512),&Vect3f(0,0,0));
	gb_IVisGeneric->SetCameraFrustum(gb_UCamera,	// ��������������� �������� ���������
		&Vect2f(0.5f,0.5f),						// ����� �����v
		&sRectangle4f(-0.49f,-0.49f,0.49f,0.49f),		// ������ ���𸪹 �����v
		&Vect2f(1.0f,1.0f),						// ����� �����v
		&Vect2f(10.0f,3000.0f),					// ��������� � ������� z-��������� ���������
		&Vect2f(0.2f,0.90f));						// zNear � zFar ��� ����������� � zBuffer
	gb_IVisGeneric->AttachCameraViewPort(gb_UCamera,gb_URenderDevice);
	gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_OBJECT_LIGHTING_CONST);
	gb_UArrayCamera(0)=gb_UCamera;
	//������ ������� START POINTOW
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	if(MeshPoint) delete MeshPoint; 
	MeshPoint=M3D->CreateObject(MeshPointName,dirM3Dtexture, M3D_CONTROL_POINT);
	M3D->Detach(MeshPoint);
	M3D->dSetScale(MeshPoint,0.2f,0.2f,0.2f);
	LMatrix.NewMatrix();
}
void doneM3D()
{
	gb_UArrayCamera(0)=0;
	gb_UArrayCamera.Delete();
	gb_IVisGeneric->ReleaseWorld(); // �������� ����
	if(gb_UCamera)	gb_IVisGeneric->ReleaseCamera(gb_UCamera); // �������� �������� ������
	if(gb_UScene) gb_IVisGeneric->ReleaseScene(gb_UScene); // �������� �����
	gb_IVisGeneric->ReleaseGraph(gb_URenderDevice); // �������� ���� ������
	gb_IVisGeneric->Release();	// �������� ����������
	gb_UCamera=0; gb_UScene=0; gb_URenderDevice=0; gb_IGraph3d=0; gb_IVisGeneric=0;
}
void setCamera(float x,float y,float z,int xCenter,int yCenter,int xSize,int ySize,float ax,float ay,float az,int focus,char perspective)
{
	int xScrMin=xCenter-xSize, xScrMax=xCenter+xSize;
	int yScrMin=yCenter-ySize, yScrMax=yCenter+ySize;
	gb_IGraph3d->SetClipRect(xScrMin,yScrMin,xScrMax,yScrMax);
	float xScr=gb_IGraph3d->GetSizeX(),yScr=gb_IGraph3d->GetSizeY();
	gb_IVisGeneric->SetCameraFrustum(gb_UCamera,&Vect2f(0.5f,0.5f),&sRectangle4f(-0.499f,-0.499f,0.499f,0.499),&Vect2f((float)focus/xScr,(float)focus/xScr),&Vect2f(10,5000),&Vect2f(0.01f,0.99f));
	if(perspective)	gb_IVisGeneric->SetCameraAttribute(gb_UCamera,ATTRIBUTE_CAMERA_PERSPECTIVE);
	else gb_IVisGeneric->ClearCameraAttribute(gb_UCamera,ATTRIBUTE_CAMERA_PERSPECTIVE);
	gb_IVisGeneric->SetCameraPosition(gb_UCamera,&Vect3f(x,y,z),&Vect3f(ax,ay,az));
}

void SaveO3DMap(char *fname)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	O3D->SaveMap(fname);
}
void LoadO3DMap(char *fname)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	O3D->LoadMap(fname);
}
int CreateOmni(float x,float y,float z,float radius,float r,float g,float b)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	cOmni *Omni=O3D->CreateOmni(x,y,z,radius,r,g,b);
	editCLS=Omni->ID;
	radCLS=radius;rCLS=r,gCLS=g,bCLS=b;xCLS=x,yCLS=y,zCLS=z;
	return Omni->ID;
}
void DeleteOmni(int number)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	cOmni *Omni=(*O3D)[number];
	if(Omni) O3D->Delete(Omni);
}
void SetOmni(int number,float radius,float r,float g,float b)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	editCLS=number;radCLS=radius;rCLS=r,gCLS=g,bCLS=b;
	cOmni *Omni=(*O3D)[number];
	if(Omni) O3D->SetOmni(Omni,radius,r,g,b);	
}
void SetOmni(int number,float x,float y,float z)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	editCLS=number;xCLS=x,yCLS=y,zCLS=z;
	cOmni *Omni=(*O3D)[number];
	if(Omni) O3D->SetPosition(Omni,x,y,z);
}
void editPropertisCLS(int number)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	editCLS=number;
	cOmni *Omni=(*O3D)[number];
	if(Omni)
	{ 
		radCLS=Omni->GetRadius();rCLS=Omni->GetColor().r,gCLS=Omni->GetColor().g,bCLS=Omni->GetColor().b;
		xCLS=Omni->GetPos().x,yCLS=Omni->GetPos().y,zCLS=Omni->GetPos().z;
	}
}
void SetOmniVisible(int number,char Visible)
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	cOmni *Omni=(*O3D)[number];
	if(Omni) O3D->SetVisible(Omni,Visible);
}
void delO3DAll()
{
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	O3D->Release();
}

void DrawID(int xe,int ye,unsigned int ID,char *string="")
{
	int xScrMin,yScrMin,xScrMax,yScrMax;
	int xScr=gb_IGraph3d->GetSizeX();
	gb_IGraph3d->GetClipRect(&xScrMin,&yScrMin,&xScrMax,&yScrMax);
	do {
		int id=ID%10;
		XBuffer Buf; Buf<=id; 
		if((xe+8<xScrMax)&&(xe>=xScrMin)&&(ye>=yScrMin)&&(ye+16<yScrMax)) 
		{
			if(zBuffer) 
				for(int j=ye;j<(ye+16);j++)
					for(int i=xe;i<(xe+8);i++)
						zBuffer[i+j*xScr]=0;
			gb_IGraph3d->DrawRectangle(xe,ye,8,16,0,0,0,255);
			gb_IGraph3d->OutText(xe,ye,Buf.address(),255,255,255,255); 
		}
		ID/=10;
		xe-=8;
	} while(ID>0);
	if((xe+8<xScrMax)&&(xe>=xScrMin)&&(ye>=yScrMin)&&(ye+16<yScrMax))
		gb_IGraph3d->OutText(xe,ye,string,255,255,255,255);
}
void DrawSurmapClient()
{
	gb_IGraph3d->BeginScene();
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=M3D->First();
	while(Mesh)
	{
		if((MeshShowID!=0)/*&&(M3D_TYPE(MeshTypeShowID)&cMesh::Type)*/)
		{
			Vect3f pv,pe;
			gb_IVisGeneric->ConvertorWorldToViewPort(gb_UCamera,&Mesh->GlobalMatrix.trans(),&pv,&pe);
			if(Mesh->NumberTrack&STATIC_NUMBER)
				DrawID(round(pe.x),round(pe.y),Mesh->ID);
			else
				DrawID(round(pe.x),round(pe.y),Mesh->ID,"t");
		}
		if((MeshShowBound!=0)&&(Mesh->GetVisibleTotal(0)&CONST_VISIBLE_FRUSTUM)&&(Mesh->Bound))
		{
			Mesh->Bound->GlobalMatrix=Mesh->GlobalMatrix;
			Mesh->Bound->GetVisibleTotal(0)=Mesh->GetVisibleTotal(0);
			Mesh->Bound->Draw(gb_UScene,&gb_UArrayCamera);
		}
		Mesh=M3D->Next();
	}
	cO3D *O3D=(cO3D*)gb_IVisGeneric->GetO3D();
	assert(O3D);
	if(OmniShowID)
		O3D->DrawID(&gb_UArrayCamera);
	gb_IGraph3d->EndScene();
}

cMatrix LMatrix;
float Lz=100,Lax=0,Lay=0,Laz=0,Lsx=1,Lsy=1,Lsz=1;
int  loadM3D(char* name,char flCollision,float x,float y,float z,float ax,float ay,float az,float sx,float sy,float sz)
{
	Lz=z;Lax=ax;Lay=ay;Laz=az;Lsx=sx;Lsy=sy;Lsz=sz;
	XBuffer buf;
	buf < dirM3D < "\\" < name;
	cMesh *Mesh=(cMesh*)gb_IVisGeneric->CreateObject((char*)buf,dirM3Dtexture);
	if(Mesh)
	{ 
		LMatrix(0,3)=x;	LMatrix(1,3)=y;	LMatrix(2,3)=z;
		gb_IVisGeneric->SetObjectPosition((cUnknownClass*)Mesh,&LMatrix);
		gb_IVisGeneric->SetObjectScale((cUnknownClass*)Mesh,&Vect3f(sx,sy,sz));
		Mesh->NumberTrack=MESH_GLOBAL_TRACK;
		editM3D=Mesh->ID;
		extern void RenderShadovM3D(int number,float y);
		RenderShadovM3D(editM3D,Mesh->y()); 	//+�������� ����
		if(flCollision) Mesh->Type=M3D_TOTAL_TYPE(M3D_KIND(Mesh->Type),M3D_STATIC_COLLIDING);
		else Mesh->Type=M3D_TOTAL_TYPE(M3D_KIND(Mesh->Type),M3D_STATIC_NON_COLLIDING);
		return Mesh->ID;
	}
	return 0;
}
void delM3D(int number)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh;
	if ((Mesh=(*M3D)[number])) 
		M3D->Delete(Mesh);
}
void delM3DAll(void)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	M3D->Release();
	M3D->NumberID=0;
}
void setScaleMesh(int number,float sx,float sy,float sz)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh) M3D->SetScale(Mesh,sx,sy,sz);
}

void loadM3DAll(char*mmapname, unsigned int track_mask)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	M3D->Release(M3D_STATIC);
	char* p = strdup(mmapname);
	M3D->LoadMap(p,track_mask);
	free(p);
	editM3D=-1;
}
void saveM3DAll(char*mmapname)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	M3D->SaveMap(mmapname);
}
void RotateMeshScr(int number,float ax,float ay,float az)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh==0) return;
	Mat3f RotCamera=((cCamera*)gb_UCamera)->GetMatrix().rot();
	Mat3f InvCamera; InvCamera.invert(RotCamera);
	cMatrix Rot; Rot.NewMatrix(); Rot.Rotate(ax,ay,az);
	cMatrix LocalMatrix(InvCamera*Rot.rot()*RotCamera*Mesh->LocalMatrix.rot(),Mesh->LocalMatrix.trans());
	M3D->SetPosition(Mesh,LocalMatrix);
}
void SetPosMesh(int number,float x,float y,float z)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh==0) return;
	Mesh->LocalMatrix.SetTranslate(x,y,z);
	M3D->SetPosition(Mesh,Mesh->LocalMatrix);
}
void setMesh(int number,float x,float y,float z,float ax,float ay,float az)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh==0) return;
	if(Mesh) M3D->SetPosition(Mesh,x,y,z,ax,ay,az);
}
void ScaleMesh(int number,float sx,float sy,float sz)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh==0) return;
	if(Mesh) M3D->SetScale(Mesh,sx,sy,sz);
}
void dsetMesh(int number,float dx,float dy,float dz,float ax,float ay,float az)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if(Mesh==0) return;
	M3D->dSetPosition(Mesh,dx,dy,dz,ax,ay,az);
}

int GetXM3D(int number)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if (Mesh) return round(Mesh->x());
	else return -1;
}
int GetYM3D(int number)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if (Mesh) return round(Mesh->y());
	else return -1;
}
int GetZM3D(int number)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if (Mesh) return round(Mesh->z());
	else return -1;
}
char* GetNameM3D(int number)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[number];
	if (Mesh) return (Mesh->GetFileName());
	else return "none";
}
void ShowIDAll(char ShowID)
{
	MeshShowID=ShowID;
	OmniShowID=ShowID;
}

void DrawMeshShade(cShadow *Shadow,int xLight, int yLight, float zLight,float Radius)//int zLight)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[Shadow->number];
	Vect3f PosShade,PosLight;
	PosLight.set(xLight,yLight,zLight);
	if(Mesh) 
	{
//		Vect3f PosLight(Mesh->GlobalMatrix.trans().x+1,Mesh->GlobalMatrix.trans().y,Mesh->GlobalMatrix.trans().z+1);
		if(fabs(Mesh->x()-PosLight.x)>5000)
			PosLight.set(Mesh->GlobalMatrix.trans().x+1000,Mesh->GlobalMatrix.trans().y,PosLight.z);
		Mesh->BuildShadeStatic(&Shadow->shade,&Shadow->xShade,&Shadow->yShade,PosShade,PosLight,Radius);
	}
	Shadow->x=round(PosShade.x); Shadow->y=round(PosShade.y); Shadow->z=round(PosShade.z);
//	if(Mesh) Shadow->yShade=0;
}
void DrawMeshShadeP(cShadow *Shadow,int xLight, int yLight, float zLight)//int zLight)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=(*M3D)[Shadow->number];
	Vect3f PosShade,PosLight;
	PosLight.set(xLight,yLight,zLight);
	if(Mesh) 
	{
//		Vect3f PosLight(Mesh->GlobalMatrix.trans().x+1,Mesh->GlobalMatrix.trans().y,Mesh->GlobalMatrix.trans().z+1);
		if(fabs(Mesh->x()-PosLight.x)>5000)
			PosLight.set(Mesh->GlobalMatrix.trans().x+1000,Mesh->GlobalMatrix.trans().y,PosLight.z);
		Mesh->BuildShadeDynamic(&Shadow->shade,&Shadow->xShade,&Shadow->yShade,PosShade,PosLight);
	}
	Shadow->x=round(PosShade.x); Shadow->y=round(PosShade.y); Shadow->z=round(PosShade.z);
//	if(Mesh) Shadow->yShade=0;
}

void RenderShadovM3DAll(void)
{
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=M3D->First();
	while(Mesh)
	{
		extern void RenderShadovM3D(int number,float y);
		if(IS_STATIC(Mesh->Type)) 
			RenderShadovM3D(Mesh->ID,Mesh->y());
		Mesh=M3D->Next();
	}
}

void DrawMeshPoint(unsigned int ID,float x,float y,float z,float ax,float ay,float az)
{
	MeshPoint->ID=ID;
	cMatrix LocalMatrix;
	LocalMatrix.NewMatrix();
	LocalMatrix.Translate(x,y,z);
	LocalMatrix.Rotate(ax,ay,az);
	MeshPoint->SetPosition(LocalMatrix);
	MeshPoint->CalcMatrix();
	MeshPoint->TestVisible(&gb_UArrayCamera);
	gb_IGraph3d->BeginScene();
	MeshPoint->Draw(gb_UScene,&gb_UArrayCamera);
	gb_IGraph3d->EndScene();
}

#else //_SURMAP_
char* dirM3D = "";
char* dirM3Dtexture = "TEXTURES";
void initM3D(int xScr,int yScr,int FullScr) { assert(0); }
void doneM3D() { assert(0); }
void setCamera(float x,float y,float z,int xCenter,int yCenter,int xSize,int ySize,float ax,float ay,float az,int focus,char perspective) { assert(0); }
void DrawSurmapClient();
// OMNI - OPERATION
void SaveO3DMap(char *fname) { assert(0); }
void LoadO3DMap(char *fname) { }
int CreateOmni(float x,float y,float z,float radius,float r,float g,float b) { assert(0); return 0; }
void DeleteOmni(int number) { assert(0); }
void SetOmni(int number,float radius,float r,float g,float b) { assert(0); }
void SetOmni(int number,float x,float y,float z) { assert(0); }
void editPropertisCLS(int number) { assert(0); }
void SetOmniVisible(int number,char Visible) { assert(0); }
void delO3DAll() { }
// ***********************************
void DrawID(int xe,int ye,unsigned int ID,char *string="")	{}
void loadM3DAll(char*mmapname, unsigned int track_mask) {}
void saveM3DAll(char*mmapname) { assert(0); }
//
int  loadM3D(char* name,char flCollision,float x,float y,float z,float ax,float ay,float az,float sx,float sy,float sz) { assert(0); return 0; }
void delM3D(int number) { assert(0); }
void delM3DAll(void) { }
void setMesh(int number,float x,float y,float z,float ax,float ay,float az) { assert(0); }
int GetXM3D(int number) { assert(0); return 1; }
int GetYM3D(int number) { assert(0); return 1; }
int GetZM3D(int number) { assert(0); return 1; }
void ScaleMesh(int number,float sx,float sy,float sz) { assert(0); }
void dsetMesh(int number,float dx,float dy,float dz,float ax,float ay,float az) { assert(0); }
void SetPosMesh(int number,float x,float y,float z) { assert(0); }
void RotateMeshScr(int number,float ax,float ay,float az) { assert(0); }
void DoneMeshBuffer() { assert(0); }
#endif //_SURMAP_
