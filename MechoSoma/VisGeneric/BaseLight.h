#ifndef __BASELIGHT_H__
#define __BASELIGHT_H__

#include "BaseObject.h"

#define BASEOBJECT_BASELIGHT									(BASEOBJECT_TYPE_BASELIGHT_OMNI|BASEOBJECT_TYPE_BASELIGHT_LIGHT|BASEOBJECT_TYPE_BASELIGHT_PROJECT)

class cBaseLight : public cBaseObject, public cBaseVolumeObject
{
public:
	int				id;	// номер источника света
	sColor4f		Ambient;
	sColor4f		Diffuse;
	sColor4f		Illumination;

	cBaseLight(unsigned int type,unsigned int kind) : cBaseObject(type,kind)		{ BaseVolumeObject()->SetAttribute(BASEOBJECT_ATTRIBUTE_VOLUME_CYCL|BASEOBJECT_ATTRIBUTE_VOLUME_SPHERE|BASEOBJECT_ATTRIBUTE_VOLUME_2D); id=-1; }
	inline float& Radius()															{ return dist; }
};

class cOmniLight : public cBaseLight
{
public:
	cOmniLight(	unsigned int type=BASEOBJECT_TYPE_BASELIGHT_OMNI,
				unsigned int kind=BASEOBJECT_KIND_VOLUME) : cBaseLight(type,kind)	{ }
};

extern void BaseObject_OmniLight(Vect3f &pos,sColor4f &diffuse,sColor4f &illumination);

#endif __BASELIGHT_H__