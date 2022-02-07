#include "VisGeneric.h"
#include "PolyMgr.h"
#include "RenderDevice.h"
#include "XGR.h"
#include "Md3d.h"

// ������� ��� ������ � ����� ������
cUnknownClass* cVisGeneric::CreateGraph(int xScr,int yScr,int GraphMode,int FullScr,int ColorBit)
{
	assert(P3D);
	cInterfaceGraph3d *IGraph3d=0;
	if(GraphMode) IGraph3d=CreateIGraph3D(GRAPH3D_DIRECT3D);
	else IGraph3d=CreateIGraph3D(GRAPH3D_SOFTWARE);
	int mode=GRAPH3D_MODE_Z16;
	if(ColorBit==32) mode|=GRAPH3D_MODE_RGB32; else mode|=GRAPH3D_MODE_RGB16;
	if(FullScr==0) mode|=GRAPH3D_MODE_WINDOW;
	IGraph3d->Init(xScr,yScr,mode,0,0);
	IGraph3d->SetClipRect(1,1,xScr-1,yScr-1);
	cRenderDevice *RenderDevice=new cRenderDevice;
	RenderDevice->Attach(IGraph3d);
	P3D->Attach(RenderDevice);
	RenderDevice->GetIGraph3d()->GetWindowHandle(&XGR_hWnd);
	return RenderDevice;
}
int cVisGeneric::ReInitGraph(cUnknownClass *URenderDevice,int xScr,int yScr,int GraphMode,int FullScr,int ColorBit)
{
	assert(P3D);
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	int mode=GRAPH3D_MODE_Z16;
	if(ColorBit==32) mode|=GRAPH3D_MODE_RGB32; else mode|=GRAPH3D_MODE_RGB16;
	if(FullScr==0) mode|=GRAPH3D_MODE_WINDOW;
	RenderDevice->GetIGraph3d()->ReInit(xScr,yScr,mode,0,0);
	RenderDevice->GetIGraph3d()->SetClipRect(1,1,xScr-1,yScr-1);
	RenderDevice->Attach(RenderDevice->GetIGraph3d());
	RenderDevice->GetIGraph3d()->GetWindowHandle(&XGR_hWnd);
	return 0;
}
int cVisGeneric::GetGraphInfo(cUnknownClass *URenderDevice,int *xScr,int *yScr,int *GraphMode,int *FullScr,int *ColorBit)
{
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	int mode=0;
	RenderDevice->GetIGraph3d()->GetInfo(xScr,yScr,&mode);
	if(ColorBit) *ColorBit=(mode&GRAPH3D_MODE_RGB32)?32:16;
	if(FullScr) *FullScr=(mode&GRAPH3D_MODE_WINDOW)?0:1;
	if(GraphMode) *GraphMode=DIRECT3D_HICOLOR;
	return 0;	
}
int cVisGeneric::SetGraphClipping(cUnknownClass *URenderDevice,const sRectangle4f *clip)
{
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	RenderDevice->GetIGraph3d()->SetClipRect(round(clip->xmin()),round(clip->ymin()),round(clip->xmax()),round(clip->ymax()));
	return 0;
}
int cVisGeneric::GetGraphClipping(cUnknownClass *URenderDevice,sRectangle4f *clip)
{
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	int xmin,ymin,xmax,ymax;
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	RenderDevice->GetIGraph3d()->GetClipRect(&xmin,&ymin,&xmax,&ymax);
	clip->set((float)xmin,(float)ymin,(float)xmax,(float)ymax);
	return 0;
}
int cVisGeneric::ReleaseGraph(cUnknownClass *URenderDevice)
{
	assert(P3D);
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	P3D->Detach(RenderDevice);
	delete RenderDevice;
	return 0;
}
int cVisGeneric::FillGraph(cUnknownClass *URenderDevice,int r,int g,int b)
{
	assert(P3D);
	if(URenderDevice)
	{
		for(int i=0;i<P3D->RenderDeviceArray.length();i++)
		{
			cRenderDevice *RenderDevice=(cRenderDevice*)P3D->RenderDeviceArray[i];
			assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
			RenderDevice->GetIGraph3d()->Fill(r,g,b);
		}
	}
	else
	{
		assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
		cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
		RenderDevice->GetIGraph3d()->Fill(r,g,b);
	}
	return 0;
}
int cVisGeneric::FlushGraph(cUnknownClass *URenderDevice)
{
	assert(P3D);
	if(URenderDevice)
	{
		for(int i=0;i<P3D->RenderDeviceArray.length();i++)
		{
			cRenderDevice *RenderDevice=(cRenderDevice*)P3D->RenderDeviceArray[i];
			assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
			RenderDevice->GetIGraph3d()->Flush();
		}
	}
	else
	{
		assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
		cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
		RenderDevice->GetIGraph3d()->Flush();
	}
	return 0;
}
cInterfaceGraph3d* cVisGeneric::GetIGraph3d(cUnknownClass *URenderDevice)
{
	if(URenderDevice==0) 
	{
		assert(P3D);
		return P3D->GetRenderDevice(0)->GetIGraph3d();
	}
	assert(URenderDevice->GetKind(KIND_RENDERDEVICE));
	cRenderDevice *RenderDevice=(cRenderDevice*)URenderDevice;
	return RenderDevice->GetIGraph3d();
}
