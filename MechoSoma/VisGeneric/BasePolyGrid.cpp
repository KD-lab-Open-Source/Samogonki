#include "BasePolyGrid.h"
#include "BaseDefine.h"
//#include "BaseReflection.h"

void cBasePolyGrid::New(int xSize,int ySize,float xStep,float yStep)
{
	delete[] pColor;
	delete[] pUVWarp;
	delete[] pXYZWarp;

        if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP)) {
          delete[] reinterpret_cast<sBaseWarpTime *>(pStructWarp);
        }
        if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP)) {
          delete[] reinterpret_cast<sBaseWarpWave *>(pStructWarp);
        }

	xsize=xSize; ysize=ySize;
	xstep=xStep; ystep=yStep;
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_COLOR))
		NEW_MEM(pColor,sBaseColor4c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_UVWARP))
		NEW_MEM(pUVWarp,sBasePoint2c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP))
		NEW_MEM(pXYZWarp,sBasePoint3c,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP))
		NEW_MEM(pStructWarp,sBaseWarpTime,xsize*ysize);
	if(BaseDrawObject()->GetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP))
		NEW_MEM(pStructWarp,sBaseWarpWave,xsize*ysize);
}
void cBasePolyGrid::SetPosition(float x,float y,float z)
{
	Matrix.trans().set(x,y,z);
}
