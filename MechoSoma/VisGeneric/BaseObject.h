#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

#include "BaseClass.h"
#include "Material.h"
#include "Object.h"

#ifndef msqrt
#define msqrt(a)												sqrt(a)
#endif
#ifndef mabs
#define mabs(a)													fabs(a)
#endif

#ifndef FREE_MEM
#define FREE_MEM(p)												{ if(p) delete p; p=0; }
#endif
#ifndef NEW_MEM
#define NEW_MEM(p,type,size)									{ p=new type[size]; memset(p,0,size*sizeof(type)); }
#endif

#define BASEOBJECT_KIND_ALL										0xFFFFFFFF
#define BASEOBJECT_KIND_DRAW_REFLECTION							(1<<0)		/* объект используется для вывода в 1-ю очередь - отражения*/
#define BASEOBJECT_KIND_DRAW_WORLD								(1<<1)		/* вывод во 2-ю очередь - задник мира */
#define BASEOBJECT_KIND_DRAW_OBJECT								(1<<2)		/* вывод во 3-ю очередь - просто объекты */
#define BASEOBJECT_KIND_DRAW_EFFECT								(1<<3)		/* вывод во 4-ю очередь - прозрачные объекты и спецэффекты */
#define BASEOBJECT_KIND_VOLUME									(1<<4)		/* объект для Volumetric Render */
#define BASEOBJECT_KIND_PHYSICPROCESS							(1<<5)		/* объект физических процессов */
#define BASEOBJECT_KIND_HELPER									(1<<6)		/* вспомогательный объект */

#define BASEOBJECT_TYPE_ALL										0x7FFFFFFF
#define BASEOBJECT_TYPE_DELETE									(1<<31)		/* объект должен быть удален */
#define BASEOBJECT_TYPE_BASELIGHT_OMNI							(1<<0)		/* точечный источник света */
#define BASEOBJECT_TYPE_BASETRAIL_TANGENT						(1<<1)		/* ориентированный след */
#define BASEOBJECT_TYPE_BASEREFLECTION_SURFACE					(1<<2)		/* отражающая z-плоскость */
#define BASEOBJECT_TYPE_BASEPOLYGRID_WORLD						(1<<3)		/* полигональная сетка для задника */
#define BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL	(1<<4)		/* отражающая z-плоскость */
#define BASEOBJECT_TYPE_BASEWAVEPROCESS							(1<<5)		/* волновой процесс */
// not used
#define BASEOBJECT_TYPE_BASELIGHT_LIGHT							(1<<16)		/* направленный источник света */
#define BASEOBJECT_TYPE_BASELIGHT_PROJECT						(1<<17)		/* направленный источник света проецирующий изображение */
///////////////////// DRAW ///////////////////
#define BASEOBJECT_ATTRIBUTE_DRAW_CYCL							(1<<0)		/* зацикленность координат */
#define BASEOBJECT_ATTRIBUTE_DRAW_SPECIALCYCL					(1<<1)		/* зацикленность координат по другому циклу */
#define BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCCW					(1<<2)		/* обход по против часовой стрелке */
#define BASEOBJECT_ATTRIBUTE_DRAW_POLYGONCW						(1<<3)		/* обход по часовой стрелке */
#define BASEOBJECT_ATTRIBUTE_DRAW_ANIMATEMATERIAL				(1<<4)		/* объект содержит анимированную материал */
#define BASEOBJECT_ATTRIBUTE_DRAW_COLOR							(1<<5)		/* объект имеет динамически изменяемое вертексное освещение */
#define BASEOBJECT_ATTRIBUTE_DRAW_UVWARP						(1<<6)		/* объект имеет динамически изменяемое возмущение текстурных координат вершин */
#define BASEOBJECT_ATTRIBUTE_DRAW_XYZWARP						(1<<7)		/* объект имеет динамически изменяемое возмущение пространственных координат вершин */
#define BASEOBJECT_ATTRIBUTE_DRAW_TIMEWARP						(1<<8)		/* возмущение объекта описывает только время */
#define BASEOBJECT_ATTRIBUTE_DRAW_WAVEWARP						(1<<9)		/* возмущение объекта описывает волновой процесс */
#define BASEOBJECT_ATTRIBUTE_DRAW_MULTIMATERIAL					(1<<10)		/* объект содержит несколько материалов */
////////////////// VOLUME ////////////////////
#define BASEOBJECT_ATTRIBUTE_VOLUME_CYCL						(1<<0)		/* зацикленность координат */
#define BASEOBJECT_ATTRIBUTE_VOLUME_SPHERE						(1<<1)		/* BaseVolumeObject определяет попадание точки в сферу, с центром в VolumeCenter и радиусом dist */
#define BASEOBJECT_ATTRIBUTE_VOLUME_BOX							(1<<2)		/* BaseVolumeObject определяет попадание точки в куб, с центром в VolumeCenter и стороной 2*dist */
#define BASEOBJECT_ATTRIBUTE_VOLUME_2D							(1<<3)		/* BaseVolumeObject определяет попадание точки в проекцию Volume на плоскость OXY */

class cBaseObject
{
public:
	unsigned int	Kind;										// сеиейство объектов
	unsigned int	Type;										// тип один из BASEOBJECT_XXXXX
	unsigned int	ID;											// уникальный идентификационный номер

	cBaseObject(unsigned int type=0,unsigned int kind=0)		{ Type=type; Kind=kind; ID=0; }
	inline void SetType(unsigned int type)						{ Type=type; }
	inline void SetKind(unsigned int kind)						{ Kind=kind; }
	inline void SetID(unsigned int id)							{ ID=id; }
	inline int GetKind(unsigned int kind=0)						{ if(kind) return kind&Kind; else return Kind; }
	inline int GetType(unsigned int type=0)						{ if(type) return type&Type; else return Type&BASEOBJECT_TYPE_ALL; }
	inline int isDelete()										{ return Type&BASEOBJECT_TYPE_DELETE; }
	inline int GetID()											{ return ID; }
	int AssertValid()											{ return Type&&Kind; }
	inline cBaseObject* BaseObject()							{ return this; }
};

typedef cBaseList <cBaseObject> cBaseObjectList;
typedef cBaseDispatcher <cBaseObject,cBaseObjectList> cManagerBaseObjectClass;

class cManagerKindObject :  public cManagerBaseObjectClass
{	// менеджер обработки объектов
public:
	cManagerKindObject()													{ NumberID=1; }
	~cManagerKindObject();

	inline void Attach(cBaseObject* BaseObject)								{ cManagerBaseObjectClass::Attach(BaseObject); }
	inline void Detach(cBaseObject* BaseObject)								{ cBaseObjectList *BaseObjectList=FindList(BaseObject); assert(BaseObjectList); delete BaseObjectList; }
	inline cBaseObject* operator[] (unsigned int ID)						{ return FindID(ID); }
	inline void RunFunction(void (*FunctionBaseObject(cBaseObject *BaseObject)),int type=BASEOBJECT_TYPE_ALL)
	{
		for(cBaseObjectList *start=BaseList;start;start=start->next) 
			if(start->Base->GetType(type)) 
				FunctionBaseObject(start->Base);
	}

private:
	inline cBaseObjectList* FindList(cBaseObject* BaseObject)				{ for(cBaseObjectList *start=BaseList;start;start=start->next) if(start->Base==BaseObject) return start; return 0; }
};

class cManagerBaseObject : public cManagerBaseObjectClass
{	// диспетчер хранения объектов
	cManagerKindObject *ManagerDraw1;
	cManagerKindObject *ManagerDraw2;
	cManagerKindObject *ManagerVolume;
public:
	cManagerBaseObject();
	~cManagerBaseObject();

	cBaseObject* New(unsigned int Type,unsigned int Kind);
	void Delete(cBaseObject* BaseObject);
	void Release(unsigned int Type=BASEOBJECT_TYPE_ALL,unsigned int Kind=BASEOBJECT_KIND_ALL);
	inline void Attach(cBaseObject* BaseObject)								{ cManagerBaseObjectClass::Attach(BaseObject); }
	inline void Detach(cBaseObject* BaseObject)								{ cBaseObjectList *BaseObjectList=FindList(BaseObject); assert(BaseObject); delete BaseObjectList; }
	inline cBaseObject* operator[] (unsigned int ID)						{ return FindID(ID); }
	inline void RunFunction(void (*FunctionBaseObject)(cBaseObject *BaseObject),unsigned int type=BASEOBJECT_TYPE_ALL,unsigned int kind=0xFFFFFFFF)
	{
		for(cBaseObjectList *start=BaseList;start;start=start->next) 
		{
			assert(start->Base);
			if(start->Base->GetType(type)) 
				FunctionBaseObject(start->Base);
		}
	}

private:
	inline cBaseObjectList* FindList(cBaseObject* BaseObject)				{ for(cBaseObjectList *start=BaseList;start;start=start->next) if(start->Base==BaseObject) return start; return 0; }
	inline int GetNewID()													{ return NumberID++; }
};

extern void GlobalFunctionSetMaterial(cMaterial &Material,const char *TextureName=0,const char *OpacityName=0,const char *TexturePath=0,int attribute=0);
extern int GlobalVariable_DrawAll;

class cBaseAttributeObject
{// базовый объект для работы с аттрибутами
public:
	unsigned int Attribute;

	inline void SetAttribute(unsigned int attribute)						{ Attribute=attribute; }
	inline int GetAttribute(unsigned int attribute=0)						{ if(attribute) return Attribute&attribute; return Attribute; }
	inline cBaseAttributeObject* BaseAttributeObject()						{ return this; }
};

class cBaseDrawObject : public cBaseAttributeObject
{// базовый объект отрисовки
public:
	cBaseArray<cMaterial>	MaterialArray;
	
	cBaseDrawObject()														{ }
	inline cBaseDrawObject* BaseDrawObject()								{ return this; }
	inline void SetTexture(const char *TextureName,const char *TexturePath,const char *OpacityName=0)		{ GlobalFunctionSetMaterial(MaterialArray(MaterialArray.length()),TextureName,OpacityName,TexturePath); }
};

class cBaseVolumeObject : public cBaseAttributeObject
{// базовый объект для работы с объемами
public:
	Vect3f			VolumeCenter;
	float			dist;
	
	inline cBaseVolumeObject* BaseVolumeObject()							{ return this; }
	inline Vect3f& Pos()													{ return VolumeCenter; }
	inline int isVolume(const Vect3f &pos,float &f);
};

////////////////////////////////////////////////////////////////////
//////////////////////// BASE_VOLUME_OBJECT	////////////////////////
////////////////////////////////////////////////////////////////////

inline int cBaseVolumeObject::isVolume(const Vect3f &pos,float &f)
{
	Vect3f d;
	if(Attribute&BASEOBJECT_ATTRIBUTE_VOLUME_CYCL)
	{
		d.x=GetShortDistX(VolumeCenter.x,pos.x);
		d.y=GetShortDistY(VolumeCenter.y,pos.y);
		d.z=VolumeCenter.z-pos.z;
	}
	else
		d=VolumeCenter-pos;
	switch(Attribute&(BASEOBJECT_ATTRIBUTE_VOLUME_SPHERE|BASEOBJECT_ATTRIBUTE_VOLUME_BOX|BASEOBJECT_ATTRIBUTE_VOLUME_2D))
	{
		case BASEOBJECT_ATTRIBUTE_VOLUME_SPHERE|BASEOBJECT_ATTRIBUTE_VOLUME_2D:
			f=msqrt(d.x*d.x+d.y*d.y);
			return f<dist;
		case BASEOBJECT_ATTRIBUTE_VOLUME_BOX|BASEOBJECT_ATTRIBUTE_VOLUME_2D:
			d.x=mabs(d.x);
			d.y=mabs(d.y);
			f=d.x+d.y;
			return (d.x<dist)&&(d.y<dist);
		case BASEOBJECT_ATTRIBUTE_VOLUME_SPHERE:
			f=msqrt(d.x*d.x+d.y*d.y+d.z*d.z);
			return f<dist;
		case BASEOBJECT_ATTRIBUTE_VOLUME_BOX:
			d.x=mabs(d.x);
			d.y=mabs(d.y);
			d.z=mabs(d.z);
			f=d.x+d.y+d.z;
			return (d.x<dist)&&(d.y<dist)&&(d.z<dist);
	}
	return 0;
}

#endif // __BASEOBJECT_H__