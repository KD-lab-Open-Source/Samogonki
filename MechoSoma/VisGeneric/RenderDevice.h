#ifndef __RENDERDEVICE_H__
#define __RENDERDEVICE_H__

#include "IGraph3d.h"
#include "Unknown.h"
#include "UnkLibrary.h"
#include "UMATH.H"

const int NUMBER_TEXTURE_MAX=1000;

class cCamera;
struct sTexture;

struct sTextureDevice
{
	int							hTexture;		// номер текстуры
	int							Status;			// статус текстуры
	sTexture					*Texture;		

	inline int& GetStatus()								{ return Status; }
	inline int GetStatus(int status)					{ return Status&status; }
	inline void SetStatus(int status)					{ Status|=status; }
	inline void ClearStatus(int status)					{ Status&=~status; }
};

class cRenderDevice : public cUnknownClass
{
	Vect2f							Size;
	sTextureFormatData				Texture565;
	sTextureFormatData				Texture1555;
	sTextureFormatData				Texture4444;
	sTextureFormatData				Texture8888;
	cInterfaceGraph3d				*IGraph3d;
	cUnknownDynArrayPointer			CameraList;
public:
//	cBaseDynArray<sTextureDevice>	TextureDevice;

	cRenderDevice();
	~cRenderDevice();
	void Release();

	void Attach(cInterfaceGraph3d *iGraph3d);
	// функции для работы со списком камер для данного устройства
	void Attach(cCamera *Camera);
	void Detach(cCamera *Camera);
	inline cCamera* GetCamera(int number)					{ return (cCamera*)CameraList[number]; }
	inline int GetNumberCamera()							{ return CameraList.length(); }

	inline Vect2f& GetSize()								{ return Size; }
	inline sTextureFormatData&	GetTexFmt565()				{ return Texture565; };
	inline sTextureFormatData&	GetTexFmt1555()				{ return Texture1555; };
	inline sTextureFormatData&	GetTexFmt4444()				{ return Texture4444; };
	inline sTextureFormatData&	GetTexFmt8888()				{ return Texture8888; };
	inline cInterfaceGraph3d*& GetIGraph3d()				{ return IGraph3d; }
//	inline int GetFreeSlot()								{ for(int i=1;i<TextureDevice.length();i++) if((TextureDevice[i].hTexture==0)&&(TextureDevice[i].Status==0)) return i; return 0; }

	friend class cPolyDispatcher;
};

#endif //__RENDERDEVICE_H__
