#ifndef __BASEREFLECTION_H__
#define __BASEREFLECTION_H__

#include "BaseObject.h"
#include "BasePolyGrid.h"

class cSurfaceReflection : public cBasePolyGrid
{	// отражающая плоскость z=const
public:
	float			KoofReflection;		// коофициент отражения

	cSurfaceReflection(	unsigned int type=BASEOBJECT_TYPE_BASEREFLECTION_SURFACE,
						unsigned int kind=BASEOBJECT_KIND_DRAW_REFLECTION) : cBasePolyGrid(type,kind)	{ BaseDrawObject()->SetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_CYCL|BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW|BASEOBJECT_ATTRIBUTE_DRAW_COLOR|BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP|BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP); }
	int inVolume(Vect3f &pos);
};

class cSurfaceReflectionMultiMaterial : public cSurfaceReflection
{	// отражающая плоскость z=const мультитекстурированная
public:
	int				xMaterial,yMaterial;
	int				WarpWavePlane;
	int				AmplitudeColor;
	float			PeriodWave,dPeriodWave;
	cSurfaceReflectionMultiMaterial(unsigned int type=BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL,
						unsigned int kind=BASEOBJECT_KIND_DRAW_REFLECTION) : cSurfaceReflection(type,kind)	{ BaseDrawObject()->SetAttribute(BASEOBJECT_ATTRIBUTE_DRAW_CYCL|BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW|BASEOBJECT_ATTRIBUTE_DRAW_COLOR|BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP|BASEOBJECT_ATTRIBUTE_DRAW_MULTIMATERIAL|BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP); xMaterial=yMaterial=WarpWavePlane=0; PeriodWave=0; dPeriodWave=0.5f; }
};

#endif __BASEREFLECTION_H__