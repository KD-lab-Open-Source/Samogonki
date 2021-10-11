#ifndef __FILEASC_H__
#define __FILEASC_H__

#include "3dsftk.h"
#include "stddef.h"
#include "stdlib.h"
#include "time.h"

class cFile3ds
{
	float		Scale;
	file3ds		*f;
	database3ds	*db;
	
	mesh3ds		*mesh;

	namelist3ds *objlist;
    kfmesh3ds	*kfmesh;

	unsigned int CountDummy;

public:
	cFile3ds(char *fname=0)				{ CountDummy=0; f=NULL; db=NULL; Scale=1.0f; if(fname) Open(fname); }
	~cFile3ds()							{ }

	char Open(char *fname,int OpenDirection=1);
	void Close();
	//****************** Mesh operation ********************
	int		OpenBaseMesh(); // return Number Object Tri-mesh
	void	OpenMesh(int nObject,int *nPoint,int *nFace,char *NameObject);
	float	MaxSizeMesh(int nObject,float NewSize);
	float	GetMaxSizeMesh(float Max);
	void	ReadMesh(float *PointXYZUV,int *FaceP1P2P3,char *NameMaterial,float *Matrix);
	void	ReadMaterial(char *NameMaterial,char *TextureName,char *OpacityName,float *Transparency,float *ShinStrength,float *Shininess,char *Shading,float *rDiffuse,float *gDiffuse,float *bDiffuse);
	void	CloseMesh();
	void	CloseBaseMesh();
	//**************** KeyFrame operation *******************
	int		OpenBaseKeyFrame();	// return Number Mesh Motion

	void	OpenKeyFrame(char *NameFrame);
	void	ReadKeyFrame(char *Parent,float **Pos,int *NumberPos,float **Rot,int *NumberRot,float **Scale,int *NumberScale,float *Pivot);
	void	CloseKeyFrame();
	
	char*	OpenDummy(int NumberKeyFrame);
	char*	GetDummyParent();
	void	ReadDummy(float *x,float *y,float *z);
	void	CloseDummy();
	
	void	CloseBaseKeyFrame();
};

#endif //__FILEASC_H__