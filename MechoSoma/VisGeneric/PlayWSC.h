#ifndef __PLAYWSC_H__
#define __PLAYWSC_H__

#include <filesystem>

#include "Unknown.h"
#include "cString.h"
#include "BaseClass.h"
#include "UMATH.H"

enum eAttributeConnectWSC
{
	ATTRCONNECTWSC_NULL					=	0,
	ATTRCONNECTWSC_OBJECT				=	1<<0,
	ATTRCONNECTWSC_SPECIALEFFECT		=	1<<2,
	ATTRCONNECTWSC_CAMERA				=	1<<3,
};

class CameraDispatcher;
class cElementWorldScript;
class cInterfaceVisGeneric;
struct scrDataBlock;

class cWorldScriptPlay
{
	cInterfaceVisGeneric	*IVisGeneric;
	cBaseDynArrayPointer<cElementWorldScript> ConnectArray;
	int						MusicTrack;
	CameraDispatcher		*CameraDisp;
	scrDataBlock			*root,*current;
	cString					DefaultPathObject3d,DefaultPathTexture,CurrentPathObject3d,SoundScriptName,CutName;
	float					StartTime,CurrentTime,FinishTime;
public:
	cWorldScriptPlay();
	~cWorldScriptPlay();
	void Free();
	int OpenWorldScript(const std::filesystem::path &path);
	int LoadNextWorldScript();
	void CloseWorldScript();
	void Animate();
	void Draw();
	inline void SetVisGeneric(cInterfaceVisGeneric *pIVisGeneric)	{ IVisGeneric=pIVisGeneric; }
	inline void SetCameraDispatcher(CameraDispatcher *pCameraDisp)	{ CameraDisp=pCameraDisp; }
	inline int isPlay()												{ return StartTime>=0; }
	inline int& GetMusicTrack()										{ return MusicTrack; }
	inline char* GetName()											{ return CutName; }
private:
	inline int GetNumberConnect()									{ return  ConnectArray.length(); }
	inline cElementWorldScript*& GetConnect(int number)				{ return  ConnectArray[number]; }
	int PreLoadWSC(void *buffer,int length);
	int LoadWSC(void *buffer,int length);
	void HideAllObject();
	void UnhideAllObject();
	cElementWorldScript* FindElement(char *ConnectName);
};

extern cWorldScriptPlay TestWorldScriptPlay;

#endif//__PLAYWSC_H__