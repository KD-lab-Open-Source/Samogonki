#ifndef __FILEASC_H__
#define __FILEASC_H__

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

class cFile3ds
{
	float		Scale;
	unsigned int CountDummy;

public:
	static constexpr unsigned int ObjectNameSize = 22;
	static constexpr unsigned int TextureNameSize = 17;
	static constexpr unsigned int OpacityNameSize = 17;
	static constexpr unsigned int ParentNameSize = 30;

	cFile3ds();
	~cFile3ds();

	char Open(const char *fname);
	void Close();

	//****************** Mesh operation ********************
	int		GetMeshCount();
	void	GetMeshParameters(int nObject, int *nPoint, int *nFace, char *NameObject);
	float	MaxSizeMesh(int nObject, float NewSize);
	void	ReadMesh(float *PointXYZUV, int *FaceP1P2P3, float *Matrix);
	void	ReadMaterial(char *TextureName, char *OpacityName, float *Transparency, float *ShinStrength, float *Shininess, char *Shading, float *rDiffuse, float *gDiffuse, float *bDiffuse);
	void	ReadKeyFrame(char *Parent, float **Pos, int *NumberPos, float **Rot, int *NumberRot, float **Scale, int *NumberScale, float *Pivot);

	int GetDummyCount();
	const char*	OpenDummy(int NumberKeyFrame);
	const char*	GetDummyParent();
	void	ReadDummy(float *x, float *y, float *z);
};

#endif //__FILEASC_H__
