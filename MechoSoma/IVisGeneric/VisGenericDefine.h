#ifndef __VISGENERICDEFINE_H__
#define __VISGENERICDEFINE_H__

enum eAttributeCamera
{
	ATTRIBUTE_CAMERA_NULL								=	0,
	ATTRIBUTE_CAMERA_PERSPECTIVE						=	1<<0,
	ATTRIBUTE_CAMERA_WORLD_SHARE						=	1<<1,
	ATTRIBUTE_CAMERA_WORLD_CUTTING						=	1<<2,

	ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE			=	ATTRIBUTE_CAMERA_PERSPECTIVE | ATTRIBUTE_CAMERA_WORLD_SHARE,
	ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_CUTTING			=	ATTRIBUTE_CAMERA_PERSPECTIVE | ATTRIBUTE_CAMERA_WORLD_CUTTING,
	ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE_CUTTING	=	ATTRIBUTE_CAMERA_PERSPECTIVE | ATTRIBUTE_CAMERA_WORLD_SHARE | ATTRIBUTE_CAMERA_WORLD_CUTTING,
	// 28..29
	ATTRIBUTE_CAMERA_MASK_A								=	1<<27,
	ATTRIBUTE_CAMERA_MASK_B								=	1<<28,
};

enum eConstVisible
{ // константы видимости по отношению к пирамиде видимости, видимости в сцене
	CONST_VISIBLE_NULL									=	0,		
	CONST_VISIBLE_SHOW									=	1<<0,	// объект показывать (выводить)
	CONST_VISIBLE_XMIN									=	1<<1,	// видим, нет пересечений с левой плоскостью пирамиды видимости
	CONST_VISIBLE_XMAX									=	1<<2,	// видим, нет пересечений с правой плоскостью пирамиды видимости
	CONST_VISIBLE_YMIN									=	1<<3,	// видим, нет пересечений с верхней плоскостью пирамиды видимости
	CONST_VISIBLE_YMAX									=	1<<4,	// видим, нет пересечений с нижней плоскостью пирамиды видимости
	CONST_VISIBLE_ZMIN									=	1<<5,	// видим, нет пересечений с ближней плоскостью пирамиды видимости
	CONST_VISIBLE_ZMAX									=	1<<6,	// видим, нет пересечений с ближней плоскостью пирамиды видимости
	CONST_VISIBLE_FRUSTUM								= CONST_VISIBLE_XMIN | CONST_VISIBLE_XMAX | CONST_VISIBLE_YMIN | CONST_VISIBLE_YMAX | CONST_VISIBLE_ZMIN | CONST_VISIBLE_ZMAX,
};

// старые определения для совместимости, в процессе переработки должны быть удалены
#define CONST_VISIBLE									1

#endif // __VISGENERICDEFINE_H__