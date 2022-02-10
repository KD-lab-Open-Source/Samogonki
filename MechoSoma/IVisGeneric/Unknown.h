#ifndef __UNKNOWN_H__
#define __UNKNOWN_H__

#include "assert.h"

#define UNKNOWN_ASSERT(a)											assert(a)
#define UNKNOWN_TYPE(a)												((a)&0x000000FF)
#define UNKNOWN_DESTRUCTOR											cUnknownClass::Release(); if(cUnknownClass::GetRef()) Release();
extern unsigned int GlobalUniqueCounter;

enum eKindUnknownClass
{	// unsigned short
	KIND_NULL				=		0,
//	1	..	7	- интерфейсы классов
	KIND_UI_VISGENERIC		=		1,				// базовая библиотека визуализации
	KIND_UI_TEXTURE			=		2,				// интерфейс для работы с текстурами
	KIND_UI_OBJECT			=		3,				// интерфейс для работы с объектами
//	8	..	15	- классы для работы с текстурами
	KIND_TEX_BITMAP			=		8,				// базовый класс текстуры cTextureBitMap 
	KIND_TEX_MIPMAP			=		9,				// класс текстуры cUnknownLOD - поддержка мипмапинга
	KIND_TEX_ANIMATION		=		10,				// класс текстуры cTextureAnimation со счетчиком
//	16	..	23	- классы для работы с объектами
	KIND_OBJ_LOD			=		16,				// cObjectLOD - поддержка уровней детализации
	KIND_OBJ_MATERIAL		=		17,				// cObjectMaterial - класс материала объекта
	KIND_OBJ_NODE			=		18,				// cObjectNode - абстрактный класс-дерево для всех объектов
	KIND_OBJ_ANIMATION		=		19,				// cNodeAnimation - класс единицы канала анимации
//	24	..	31	- специализированные классы объектов Mesh - геометрическое тело 
	KIND_MESH_NODE			=		24,				// cMeshNode - конкретная реализацию полигонального объекта Mesh'а
	KIND_MESH_MAPPING		=		25,				// cMeshMapping - класс едичного канала материала 
//	KIND_MESH_TILE			=		26,				// cMeshTile - класс эелемент объекта
//	32	..	63	- прочие классы 
	KIND_SCENE				=		32,				// cScene - класс сцены 
	KIND_CAMERA				=		33,				// cCamera - класс камера
	KIND_MESH				=		34,				// cMesh - класс объектов
	KIND_WORLD_TILEMAP		=		35,				// cTileMap	- класс тайловой полигональной карты
	KIND_WORLD_TILEWATER	=		36,				// cTileWater - класс тайловой полигональной карты
	KIND_RENDERDEVICE		=		37,				// cRenderDevice - надстройка над cInterfaceGraph3d
	KIND_ARRAYCAMERA		=		38,				// cUnkClassDynArrayPointer - класс накопитель cCamera
	KIND_ARRAYSCENE			=		39,				// cUnkClassDynArrayPointer - класс накопитель cScene
	KIND_ARRAYSUN			=		40,				// cUnkClassDynArrayPointer - класс накопитель cSun
//	64	..	128	- классы библиотеки
	KIND_LIB_TEXTURE		=		64,				// cTextureLibrary 
	KIND_LIB_OBJECT			=		65,				// cObjectLibrary
};
enum eTypeUnknownClass
{	// unsigned short
	TYPE_NULL				=		0,
// bit 1 - bit 31 флаги классов
	TYPE_POINTER			=		1<<0,			// класс используется как счетчик указателей
	TYPE_DELETE				=		1<<31			// признак того, что класс удален
};

// базовый класс для всех
// любой класс наследованный как TYPE_CLASS_POINTER, должен уметь удаляться по обращению к Release()
class cUnknownClass
{	
	int					m_cRef;
	unsigned int		Unique;
	eKindUnknownClass	Kind;
	eTypeUnknownClass	Type;
public:
	cUnknownClass(int kind=KIND_NULL,int type=TYPE_NULL)
	{ 
		m_cRef=1; 
		Kind=eKindUnknownClass(kind);
		Type=eTypeUnknownClass(type);
		Unique=++GlobalUniqueCounter; 
		UNKNOWN_ASSERT(Unique); 
	}
	virtual ~cUnknownClass()
	{ 
		UNKNOWN_ASSERT(GetType(TYPE_DELETE)); 
		if(GetType(TYPE_POINTER))
			UNKNOWN_ASSERT(m_cRef==0); 
	}
	inline int GetKind(int kind)									{ return Kind==kind; }
	inline int GetKind()											{ return Kind; }
	inline int GetType(int type)									{ return Type&type; }
	inline int GetType()											{ return Type; }
	inline int SetType(int type)									{ return Type=eTypeUnknownClass(Type|type); }
	inline int GetRef()												{ return m_cRef; }
	inline int IncRef()												{ return ++m_cRef; }
	inline int DecRef()												{ return --m_cRef; }
	inline int GetUnique()											{ return Unique; }
	inline int Release()
	{
		SetType(TYPE_DELETE);
		return m_cRef;
	}
};

#define UI_BEGIN(UI_Class,UI_Kind)									class UI_Class : public cUnknownInterface { public: UI_Class() : cUnknownInterface(UI_Kind) {} ~UI_Class()	{ UNKNOWN_ASSERT(cUnknownClass::GetRef()==0); }
#define UI_END(UI_Class)											};
#ifndef UI_IMPORT
#define UI_IMPORT(UI_Function)										virtual UI_Function=0
#endif

class cUnknownInterface : public cUnknownClass
{
public:
	explicit cUnknownInterface(int kind,int type=TYPE_POINTER) : cUnknownClass(kind,type)	{ }
	virtual ~cUnknownInterface()
	{ 
		cUnknownClass::Release(); 
	}
	virtual int Release()											{ delete this; return 0; }
	virtual void Delete(cUnknownClass *UnknownClass)				{ assert(UnknownClass==0); }
};

#endif //__UNKNOWN_H__