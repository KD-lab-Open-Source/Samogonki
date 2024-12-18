#include "PolyMgr.h"
#include "Object3d.h"
#include "RenderDevice.h"

//#define SIGN(a)						((a)<0? -1: 1)

#define NUMBER_POINT_FIX			10000

extern void SaveMemoryMessage(const char *message,int i=0);
int					gb_CurrentTexture=0;

cPolyDispatcher::cPolyDispatcher()
{
	SaveMemoryMessage("Begin cPolyDispatcher::cPolyDispatcher()");
	memset(this,0,sizeof(cPolyDispatcher));
	PointFix.New(NUMBER_POINT_FIX);
	CurrentCamera=0;
	CurrentConvertorObjectToScreen=new cConvertor;
	SaveMemoryMessage("End cPolyDispatcher::cPolyDispatcher()");
	AlphaForSprite=255;
}
cPolyDispatcher::~cPolyDispatcher()
{
	PointFix.Delete();
	if(CurrentConvertorObjectToScreen) delete CurrentConvertorObjectToScreen; 
	CurrentConvertorObjectToScreen=0;
}
void cPolyDispatcher::BeginScene(cUnknownClass *UCameraList)
{
	cInterfaceGraph3d *IGraph3d=GetRenderDevice(0)->GetIGraph3d();
	IGraph3d->BeginScene(); 
	CurrentCamera=0;
}
void cPolyDispatcher::EndScene(cUnknownClass *UCameraList)
{
	cInterfaceGraph3d *IGraph3d=GetRenderDevice(0)->GetIGraph3d();
	IGraph3d->EndScene(); 
	CurrentCamera=0;
}
