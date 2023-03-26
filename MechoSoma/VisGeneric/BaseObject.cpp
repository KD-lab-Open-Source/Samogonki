#include "BaseObject.h"
#include "BasePolyGrid.h"
#include "BaseLight.h"
#include "BaseTrail.h"
#include "BaseReflection.h"
#include "Object3d.h"
#include "TexMgr.h"

int GlobalVariable_DrawAll=0;

cManagerBaseObject::cManagerBaseObject()
{ 
	NumberID=1; 
	ManagerDraw1=new cManagerKindObject;
	ManagerDraw2=new cManagerKindObject;
	ManagerVolume=new cManagerKindObject;
}
cManagerBaseObject::~cManagerBaseObject()
{
	Release(BASEOBJECT_TYPE_ALL,BASEOBJECT_KIND_ALL);
	NumberID=0;
	delete ManagerVolume;
	delete ManagerDraw2;
	delete ManagerDraw1;
}
void cManagerBaseObject::Release(unsigned int Type,unsigned int Kind)
{
	cBaseObjectList *start=BaseList,*tmp; 
	while((tmp=start)!=0)
	{
		start=start->next;
		if(tmp->Base->GetType(Type)&&tmp->Base->GetKind(Kind))
			Delete(tmp->Base);
	}
}
void cManagerBaseObject::Delete(cBaseObject* BaseObject)
{
	cBaseObjectList *BaseObjectList=FindList(BaseObject);
	assert(BaseObjectList);
	if(BaseObject)
		switch(BaseObject->GetType())
		{
			case BASEOBJECT_TYPE_BASELIGHT_OMNI:
				assert(((cOmniLight*)BaseObject)->AssertValid());
				delete ((cOmniLight*)BaseObject);
				break;
			case BASEOBJECT_TYPE_BASETRAIL_TANGENT:
				assert(((cTangentTrail*)BaseObject)->AssertValid());
				delete ((cTangentTrail*)BaseObject);
				break;
			case BASEOBJECT_TYPE_BASEREFLECTION_SURFACE:
//				assert(((cSurfaceReflection*)BaseObject)->AssertValid());
//				delete ((cSurfaceReflection*)BaseObject);
				break;
			case BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL:
				assert(((cSurfaceReflectionMultiMaterial*)BaseObject)->AssertValid());
				delete ((cSurfaceReflectionMultiMaterial*)BaseObject);
				break;
			case BASEOBJECT_TYPE_BASEPOLYGRID_WORLD:
				assert(((cWorldPolyGrid*)BaseObject)->AssertValid());
				delete ((cWorldPolyGrid*)BaseObject);
				break;
			case BASEOBJECT_TYPE_BASEWAVEPROCESS:
//				assert(((cBaseWaveProcess*)BaseObject)->AssertValid());
//				delete ((cBaseWaveProcess*)BaseObject);
				break;
			default:
				assert(BaseObject->AssertValid());
		}
	cManagerBaseObjectClass::Detach(BaseObjectList);
}
cBaseObject* cManagerBaseObject::New(unsigned int Type,unsigned int Kind)
{
	cBaseObject *BaseObject=0;
	switch(Type)
	{
		case BASEOBJECT_TYPE_BASELIGHT_OMNI:
			BaseObject=new cOmniLight(Type,Kind);
			assert(((cOmniLight*)BaseObject)->AssertValid());
			break;
		case BASEOBJECT_TYPE_BASETRAIL_TANGENT:
			BaseObject=new cTangentTrail(Type,Kind);
			assert(((cTangentTrail*)BaseObject)->AssertValid());
			break;
		case BASEOBJECT_TYPE_BASEREFLECTION_SURFACE:
//			BaseObject=new cSurfaceReflection(Type,Kind);
//			assert(((cSurfaceReflection*)BaseObject)->AssertValid());
			break;
		case BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL:
			BaseObject=new cSurfaceReflectionMultiMaterial(Type,Kind);
			assert(((cSurfaceReflectionMultiMaterial*)BaseObject)->AssertValid());
			break;
		case BASEOBJECT_TYPE_BASEPOLYGRID_WORLD:
			BaseObject=new cWorldPolyGrid(Type,Kind);
			assert(((cWorldPolyGrid*)BaseObject)->AssertValid());
			break;
		case BASEOBJECT_TYPE_BASEWAVEPROCESS:
//			BaseObject=new cBaseWaveProcess(Type,Kind);
//			assert(((cBaseWaveProcess*)BaseObject)->AssertValid());
			break;
		default:
			return 0;
	}
	Attach(BaseObject);
	return BaseObject;	
}

cManagerKindObject::~cManagerKindObject()
{
	cBaseObjectList *start=BaseList,*tmp; 
	while((tmp=start)!=0)
	{
		start=start->next;
		Detach(tmp->Base);
	}
	NumberID=0;
}
void GlobalFunctionSetMaterial(cMaterial &Material,const char *TextureName,const char *OpacityName,const char *TexturePath,int attribute)
{
	assert(TextureBuffer);
	if(TextureName) 
	{ 
		Material.SetAttribute(ATTRMAT_TEXTURE_PAL); 
		Material.Texture=TextureBuffer->Get(TextureName,TexturePath); 
		if(OpacityName) 
		{ 
			Material.SetAttribute(ATTRMAT_OPACITY); 
			Material.Opacity=TextureBuffer->Get(OpacityName,TexturePath); 
		}
	}
	else 
	{ 
		Material.SetAttribute(ATTRMAT_COLOR); 
		Material.MulCol.set(0,0,0,1); 
	}
	Material.SetAttribute(attribute);
}

////////////////////////////////////////////////////////////////////
///////////////////////// BASE_DRAW_OBJECT /////////////////////////
////////////////////////////////////////////////////////////////////


