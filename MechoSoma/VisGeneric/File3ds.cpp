#include <stdlib.h>
#include "File3ds.h"
#include <math.h>
#include "Math3d.h"
#include "BaseDefine.h"

#ifdef _MECHOSOMA_
int m3dOpenResource(char* fname,XStream& fh);
int vmapOpenResource(char* fname,XStream& fh);
extern "C" {
void openLogFile(char* fname);
void closeLogFile(void);
};
#endif

#define RAD_GRAD		57.296f

char cFile3ds::Open(char *fname,int OpenDirection)
{
	const ErrRec3ds *ErrList=ReturnErrorList3ds();

#ifdef _MECHOSOMA_
	int buf_len;
	char* buf;

	XStream fh;

	if(OpenDirection==1)
		m3dOpenResource(fname,fh);
	else if(OpenDirection==2)
		vmapOpenResource(fname,fh);

	buf_len = fh.size();
	buf = (char*)malloc(buf_len);
	fh.read(buf,buf_len);
	fh.close();
/*
	char* tmp_buf = strdup(fname);
	tmp_buf[strlen(tmp_buf) - 3] = 'l';
	tmp_buf[strlen(tmp_buf) - 2] = 'o';
	tmp_buf[strlen(tmp_buf) - 1] = 'g';

	openLogFile(tmp_buf);
	free(tmp_buf);
*/

	f=OpenFile3ds_buf(buf,buf_len);
//	f=OpenFile3ds(fname,"rb");
#else
	f=OpenFile3ds(fname,"rb");
#endif
	if(f==0) { CLEAR_ERROR; CloseAllFiles3ds(); /* CLEAR_INFO_MESSAGE; /*ERROR_MESSAGE("3ds file not found",fname,0);*/ XBuffer msg; msg < "3ds file not found " < fname; ErrH.Abort(msg); return 0; }
	InitDatabase3ds(&db);
	CreateDatabase3ds(f,db);
	return 1;
}
void cFile3ds::Close()
{
//	SHOW_INFO_MESSAGE(f->filename);
	ReleaseDatabase3ds(&db);
	CloseFile3ds(f);

//	closeLogFile();

	CLEAR_ERROR; 
	CloseAllFiles3ds();
}
//******************************** MESH **********************************
int cFile3ds::OpenBaseMesh()
{
	return GetMeshCount3ds(db);
}
void cFile3ds::OpenMesh(int nObject,int *nPoint,int *nFace,char *NameObject)
{
	mesh=NULL;
	GetMeshByIndex3ds(db,nObject,&mesh);
	(*nPoint)=(int)mesh->nvertices;		/* Vertice count */
	int nTexel=(int)mesh->ntextverts;	/* Number of texture vertices */
	(*nFace)=(int)mesh->nfaces;			/* Face count	*/
	strcpy(NameObject,mesh->name);
//	InfoBuf<"3d object "<NameObject<" - vertex:"<=(*nPoint)<", texel:"<=nTexel<", face:"<=(*nFace);
	if(nTexel>(*nPoint)) 
	{ XBuffer buf; buf<"Error : cFile3ds::OpenMesh()\r\nBad 3ds file "<f->filename<" (number texel != number point) in mesh "<mesh->name; ErrH.Abort(buf.address()); }
//		ERROR_MESSAGE(f->filename,"cFile3ds::OpenMesh()\r\nError - bad 3ds file (number texel != number point)\r\n",0);
}
float cFile3ds::MaxSizeMesh(int nObject,float NewSize)
{
	float Max=0;
	for(int j=0;j<nObject;j++)
	{
		mesh=NULL;
		GetMeshByIndex3ds(db,j,&mesh);
		Max=GetMaxSizeMesh(Max);
		RelMeshObj3ds(&mesh);
	}
	Scale=NewSize/Max;
	return Max;
}
float cFile3ds::GetMaxSizeMesh(float Max)
{
	for(int i=0;i<mesh->nvertices;i++)	
	{
		if(fabs(mesh->vertexarray[i].x)>Max) Max=fabs(mesh->vertexarray[i].x);
		if(fabs(mesh->vertexarray[i].y)>Max) Max=fabs(mesh->vertexarray[i].y);
		if(fabs(mesh->vertexarray[i].z)>Max) Max=fabs(mesh->vertexarray[i].z);
	}
	return Max;
}
void cFile3ds::ReadMesh(float *PointXYZUV,int *FaceP1P2P3,char *NameMaterial,float *Matrix)
{
	point3ds *point=mesh->vertexarray;	/* List of vertices */
	textvert3ds *texel=mesh->textarray;	/* List of texture coordinates */
	face3ds *face=mesh->facearray;		/* List of faces	*/

	int i;
	for(i=0;i<mesh->nfaces;i++)	
	{
		FaceP1P2P3[3*i+0]=face[i].v1;
		FaceP1P2P3[3*i+1]=face[i].v2;
		FaceP1P2P3[3*i+2]=face[i].v3;
	}

	for(i=0;i<mesh->nvertices;i++)	
	{
		PointXYZUV[5*i+0]=point[i].x*Scale;
		PointXYZUV[5*i+1]=point[i].y*Scale;
		PointXYZUV[5*i+2]=point[i].z*Scale;
		int test11=*((int*)&point[i].x);
		if((test11==0xFFFFFFFF)||(test11==0xFFFFFF7F)) 
			test11=test11;
	}
	for(i=0;i<mesh->ntextverts;i++)	
	{
		PointXYZUV[5*i+3]=texel[i].u;
		PointXYZUV[5*i+4]=texel[i].v;
	}
	for(i=0;i<9;i++) Matrix[i]=mesh->locmatrix[i];
	for(i=9;i<12;i++) Matrix[i]=mesh->locmatrix[i]*Scale;
	if(mesh->matarray) 
		strcpy(NameMaterial,mesh->matarray->name);
	else for(i=0;i<17;i++) NameMaterial[i]=0;

//	InfoBuf<", material: "<mesh->matarray->name<"\r\n";
}

void cFile3ds::ReadMaterial(char *NameMaterial,char *TextureName,char *OpacityName,float *Transparency,float *ShinStrength,float *Shininess,char *Shading,float *rDiffuse,float *gDiffuse,float *bDiffuse)
{
	material3ds *mat=NULL;
	GetMaterialByName3ds(db,NameMaterial,&mat);
	if(mat==0) { XBuffer buf; buf<"Error : cFile3ds::ReadMaterial()\r\nBad material 3ds file "<f->filename; ErrH.Abort(buf.address()); }
/*	InfoBuf<"  transparency:"<=((*Transparency)=mat->transparency)<
		 "  shinstrength:"<=((*ShinStrength)=mat->shinstrength)<
		 "  shininess:"<=((*Shininess)=mat->shininess)<
		 "  shading:"<=((*Shading)=(char)mat->shading)<"\r\n";	*/
	(*Transparency)=mat->transparency; (*ShinStrength)=mat->shinstrength; (*Shininess)=mat->shininess; (*Shading)=(char)mat->shading;
	*rDiffuse=mat->diffuse.r; *gDiffuse=mat->diffuse.g;	*bDiffuse=mat->diffuse.b;
	if(mat->texture.map.name) 
		strcpy(TextureName,mat->texture.map.name);
	else for(int i=0;i<17;i++) TextureName[i]=0;
	if(mat->opacity.map.name) 
		strcpy(OpacityName,mat->opacity.map.name);
	else for(int i=0;i<17;i++) OpacityName[i]=0;
	ReleaseMaterial3ds(&mat);
}
void cFile3ds::CloseMesh()
{
	RelMeshObj3ds(&mesh);
}
void cFile3ds::CloseBaseMesh()
{
}
//***************************** KEYFRAME **********************************
int cFile3ds::OpenBaseKeyFrame()
{
	objlist=NULL; 
	GetObjectNodeNameList3ds(db,&objlist);
//	InfoBuf<"Number object motion: "<=objlist->count<"\r\n";
	return (int) objlist->count;
}
void cFile3ds::OpenKeyFrame(char *NameFrame)
{
	kfmesh=NULL;
	for(unsigned int i=0;i<objlist->count;i++)
		if(strcmp(NameFrame,objlist->list[i].name)==0) { GetObjectMotionByIndex3ds(db,i,&kfmesh); return; }
	XBuffer buf; buf<"Error: in 3ds file "<f->filename<" not found keyframe object - "<NameFrame; ErrH.Abort(buf);
}
void cFile3ds::ReadKeyFrame(char *Parent,float **Pos,int *NumberPos,float **Rot,int *NumberRot,float **Scale,int *NumberScale,float *Pivot)
{
	strcpy(Parent,kfmesh->parent);
	(*NumberPos)=kfmesh->npkeys; (*NumberRot)=kfmesh->nrkeys; (*NumberScale)=kfmesh->nskeys;
	if(*NumberPos) (*Pos)=new float[4*(*NumberPos)]; else (*Pos)=0;
	if(*NumberRot) (*Rot)=new float[5*(*NumberRot)]; else (*Rot)=0;
	if(*NumberScale) (*Scale)=new float [4*(*NumberScale)]; else (*Scale)=0;
	int i;
	for(i=0;i<(*NumberPos);i++)
	{
		(*Pos)[4*i+0]=(float)kfmesh->pkeys[i].time;
		(*Pos)[4*i+1]=kfmesh->pos[i].x*cFile3ds::Scale;
		(*Pos)[4*i+2]=kfmesh->pos[i].y*cFile3ds::Scale;
		(*Pos)[4*i+3]=kfmesh->pos[i].z*cFile3ds::Scale;
	}
	double x2=0,y2=0,z2=0,w2=1;
	double Angle=kfmesh->rot[0].angle/2;
	double x1=kfmesh->rot[0].x*sin(Angle);
	double y1=kfmesh->rot[0].y*sin(Angle);
	double z1=kfmesh->rot[0].z*sin(Angle);
	double w1=cos(Angle);
	double scale=1.0f/sqrt(w1*w1+x1*x1+y1*y1+z1*z1);
	(*Rot)[5*0+0]=(float)kfmesh->rkeys[0].time;
	(*Rot)[5*0+1]=(float)(w1*scale);
	(*Rot)[5*0+2]=(float)(x1*scale);
	(*Rot)[5*0+3]=(float)(y1*scale);
	(*Rot)[5*0+4]=(float)(z1*scale);
	for(i=1;i<(*NumberRot);i++)
	{
		double Angle=kfmesh->rot[i].angle/2;
		double x1=kfmesh->rot[i].x*sin(Angle);
		double y1=kfmesh->rot[i].y*sin(Angle);
		double z1=kfmesh->rot[i].z*sin(Angle);
		double w1=cos(Angle);

		double W=w1*w2-x1*x2-y1*y2-z1*z2;
		double X=w1*x2+x1*w2+y1*z2-z1*y2;
		double Y=w1*y2-x1*z2+y1*w2+z1*x2;
		double Z=w1*z2+x1*y2-y1*x2+z1*w2;

		double scale=1.0f/sqrt(W*W+X*X+Y*Y+Z*Z);

		(*Rot)[5*i+0]=(float)kfmesh->rkeys[i].time;
		(*Rot)[5*i+1]=(float)(w2=W*scale);
		(*Rot)[5*i+2]=(float)(x2=X*scale);
		(*Rot)[5*i+3]=(float)(y2=Y*scale);
		(*Rot)[5*i+4]=(float)(z2=Z*scale);
	}
	for(i=0;i<(*NumberScale);i++)
	{
		(*Scale)[4*i+0]=(float)kfmesh->skeys[i].time;
		if(kfmesh->scale[0].x!=0.f) (*Scale)[4*i+1]=(float)kfmesh->scale[i].x/kfmesh->scale[0].x; else (*Scale)[4*i+1]=1.f;
		if(kfmesh->scale[0].y!=0.f) (*Scale)[4*i+2]=(float)kfmesh->scale[i].y/kfmesh->scale[0].y; else (*Scale)[4*i+2]=1.f;
		if(kfmesh->scale[0].z!=0.f) (*Scale)[4*i+3]=(float)kfmesh->scale[i].z/kfmesh->scale[0].z; else (*Scale)[4*i+3]=1.f;
	}
	Pivot[0]=kfmesh->pivot.x*cFile3ds::Scale; Pivot[1]=kfmesh->pivot.y*cFile3ds::Scale; Pivot[2]=kfmesh->pivot.z*cFile3ds::Scale;
/*	InfoBuf<"  Parent name: "<Parent
		<"  pos:"<=(*NumberPos)<" rot:"<=(*NumberRot)
		<"  Pivot: "<=Pivot[0]<", "<=Pivot[1]<", "<=Pivot[2]<"\r\n";*/
}
void cFile3ds::CloseKeyFrame()
{
	ReleaseObjectMotion3ds(&kfmesh);
}
char* cFile3ds::OpenDummy(int NumberKeyFrame)
{
	while(CountDummy<objlist->count)
	{
		kfmesh=NULL;
		GetObjectMotionByName3ds(db,objlist->list[CountDummy++].name,&kfmesh);
		if(strcmp(kfmesh->name,DummyName3ds)==0) return (char*) kfmesh->instance;
		ReleaseObjectMotion3ds(&kfmesh);
	}
	XBuffer buf; buf<"Error: dummy not found - "<f->filename; ErrH.Abort(buf.address()); return 0;
}
char* cFile3ds::GetDummyParent()
{
	return (char*) kfmesh->parent;
}
void cFile3ds::ReadDummy(float *x,float *y,float *z)
{
	if((kfmesh->parent==0)||(kfmesh->parent[0]==0))
	{
		*x=kfmesh->pos[0].x*Scale;
		*y=kfmesh->pos[0].y*Scale;
		*z=kfmesh->pos[0].z*Scale;
	}
	else
	{
		float x_=kfmesh->pos[0].x*Scale;
		float y_=kfmesh->pos[0].y*Scale;
		float z_=kfmesh->pos[0].z*Scale;
		cMatrix Matrix;
		double Angle=kfmesh->rot[0].angle/2;
		double x1=kfmesh->rot[0].x*sin(Angle);
		double y1=kfmesh->rot[0].y*sin(Angle);
		double z1=kfmesh->rot[0].z*sin(Angle);
		double w1=cos(Angle);
		double scale=1.0f/sqrt(w1*w1+x1*x1+y1*y1+z1*z1);
		Matrix.NewMatrix();
		Matrix.Rotate(w1,x1,y1,z1);
		Matrix.Scale(scale/kfmesh->scale[0].x,scale/kfmesh->scale[0].y,scale/kfmesh->scale[0].z);
		(*x)=Matrix.rot()[0][0]*x_+Matrix.rot()[0][1]*y_+Matrix.rot()[0][2]*z_;
		(*y)=Matrix.rot()[1][0]*x_+Matrix.rot()[1][1]*y_+Matrix.rot()[1][2]*z_;
		(*z)=Matrix.rot()[2][0]*x_+Matrix.rot()[2][1]*y_+Matrix.rot()[2][2]*z_;
	}
}
void cFile3ds::CloseDummy()
{
	ReleaseObjectMotion3ds(&kfmesh);
}
void cFile3ds::CloseBaseKeyFrame()
{
	ReleaseNameList3ds(&objlist);
}
