#ifndef __SURMAPCLIENT_H__
#define __SURMAPCLIENT_H__

#define saveCLSAll SaveO3DMap
#define loadCLSAll LoadO3DMap
#define createCLS CreateOmni
#define delCLS DeleteOmni
#define setCLS SetOmni
#define delCLSAll delO3DAll

#include "IVisGeneric.h"
#include "Math3d.h"

extern void initM3D(int xScr,int yScr,int FullScr);
extern void doneM3D();
extern void setCamera(float x,float y,float z,int xCenter,int yCenter,int xSize,int ySize,float ax,float ay,float az,int focus,char perspective);
extern void DrawSurmapClient();
// OMNI - OPERATION
extern void SaveO3DMap(char *fname);
extern void LoadO3DMap(char *fname);
extern int CreateOmni(float x,float y,float z,float radius,float r,float g,float b);
extern void DeleteOmni(int number);
extern void SetOmni(int number,float radius,float r,float g,float b);
extern void SetOmni(int number,float x,float y,float z);
extern void editPropertisCLS(int number);
extern void SetOmniVisible(int number,char Visible);
extern void delO3DAll();

// ***********************************
extern void loadM3DAll(char*mmapname, unsigned int track_mask=0xFFffFFff);
extern void saveM3DAll(char*mmapname);
//
extern cMatrix LMatrix;
extern float Lz,Lax,Lay,Laz,Lsx,Lsy,Lsz;
extern int  loadM3D(char* name,char flCollision,float x,float y,float z=Lz,float ax=Lax,float ay=Lay,float az=Laz,float sx=Lsx,float sy=Lsy,float sz=Lsz);
extern void delM3D(int number);
extern void delM3DAll(void);
extern void setMesh(int number,float x,float y,float z,float ax,float ay,float az);
extern inline int GetXM3D(int number);
extern inline int GetYM3D(int number);
extern inline int GetZM3D(int number);
extern char* GetNameM3D(int number);

extern void ScaleMesh(int number,float sx,float sy,float sz);
extern void dsetMesh(int number,float dx,float dy,float dz,float ax,float ay,float az);
extern void SetPosMesh(int number,float x,float y,float z);
extern void RotateMeshScr(int number,float ax=0,float ay=0,float az=0);
extern void DoneMeshBuffer();
#ifdef _SURMAP_
extern unsigned short		*zBuffer;
extern int					hCamera,fCamera;
extern char					MeshShowID,MeshShowBound;
extern unsigned int			MeshTypeShowID;

extern cInterfaceVisGeneric	*gb_IVisGeneric; 
extern cInterfaceGraph3d	*gb_IGraph3d; 
extern cUnknownClass		*gb_URenderDevice;
extern cUnknownClass		*gb_UScene;
	class cShadow
	{
	public:
		short			*shade;
		float			dzx;
		int				x,y,z;
		int				xShade,yShade;
		int				number;
		
		cShadow()														{ shade=0; x=y=z=0; xShade=yShade=0; }
		~cShadow()														{ if(shade) delete shade; }
	};
	extern void DrawMeshShade(cShadow *Shadow,int xLight, int yLight, float zLight,float Radius);//int zLight);
	extern void DrawMeshShadeP(cShadow *Shadow,int xLight, int yLight, float zLight);//int zLight);
	extern void ShowIDAll(char ShowID);
	extern void RenderShadovM3DAll(void);
extern int editM3Dyh,editM3Dyd,editM3D,editCLS;
extern float xCLS,yCLS,zCLS,radCLS,rCLS,gCLS,bCLS;
void DrawMeshPoint(unsigned int ID,float x,float y,float z,float ax=0,float ay=0,float az=0);
	extern float PolyArea;
	extern unsigned int PolyCount,PolyCountMask,PolyCountTransparency;
	extern unsigned int	DebugPoly;
#endif // _SURMAP_

#endif //__SURMAPCLIENT_H__