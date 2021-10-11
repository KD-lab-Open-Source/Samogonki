#include "RenderDevice.h"

cRenderDevice::cRenderDevice() : cUnknownClass(KIND_RENDERDEVICE)	
{
	memset(&Size,0,sizeof(Size));
	memset(&Texture565,0,sizeof(Texture565));
	memset(&Texture1555,0,sizeof(Texture1555));
	memset(&Texture4444,0,sizeof(Texture4444));
	memset(&Texture8888,0,sizeof(Texture8888));
/*	TextureDevice.New(NUMBER_TEXTURE_MAX);
	for(int i=0;i<TextureDevice.length();i++)
	{
		TextureDevice[i].hTexture=0;
		TextureDevice[i].Status=0;
		TextureDevice[i].Texture=0;
	}
*/	IGraph3d=0;
}
cRenderDevice::~cRenderDevice()
{
	UNKNOWN_DESTRUCTOR;
//	assert(TextureDevice.length()==0);
	assert(GetNumberCamera()==0);
	assert(IGraph3d==0);
}
void cRenderDevice::Release()
{
	if(IGraph3d) IGraph3d->Release();
	IGraph3d=0;
}
void cRenderDevice::Attach(cInterfaceGraph3d *iGraph3d)
{
	IGraph3d=iGraph3d;
	Size.set((float)GetIGraph3d()->GetSizeX(),(float)GetIGraph3d()->GetSizeY());
	Texture565.TextureFormat=TEXFMT_RGB565;
	GetIGraph3d()->GetTextureFormatData(Texture565);
	Texture1555.TextureFormat=TEXFMT_ARGB1555;
	GetIGraph3d()->GetTextureFormatData(Texture1555);
	Texture4444.TextureFormat=TEXFMT_ARGB4444;
	GetIGraph3d()->GetTextureFormatData(Texture4444);
	Texture8888.TextureFormat=TEXFMT_ARGB8888;
	GetIGraph3d()->GetTextureFormatData(Texture8888);
}
void cRenderDevice::Attach(cCamera *Camera)
{
	CameraList.Attach((cUnknownClass*)Camera);
}
void cRenderDevice::Detach(cCamera *Camera)
{
	CameraList.Detach((cUnknownClass*)Camera);
}
