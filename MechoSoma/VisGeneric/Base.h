#ifndef	__BASE_H__
#define __BASE_H__

//  Типы объектов
#include "m3d_id.h"

// Тип объекта
#define M3D_TYPE(type)		((unsigned)(type) & 0xffff)
// Разновидность объекта
#define M3D_KIND(type)		((unsigned)(type) >> 16)
// Полный тип объекта
#define M3D_TOTAL_TYPE(part_kind, part_type)	(((unsigned)(part_kind) << 16) | (part_type))
// Статический объект
#define IS_STATIC(type)	(M3D_TYPE(type) & (M3D_STATIC_COLLIDING | M3D_STATIC_NON_COLLIDING))
// Тип родителя
inline int M3D_PARENT(int childType)
{
	switch(childType)
	{
		case M3D_ENGINE:
			return 0;
		case M3D_FRONT:
		case M3D_RB_WHEEL:
		case M3D_LB_WHEEL:
			return M3D_ENGINE;
		case M3D_RF_WHEEL:
		case M3D_LF_WHEEL:
			return M3D_FRONT;
	}
	return 0;
}
// Имена деталей
#define M3D_BOUND_STRING		"bound"

// Динамические объекты
#define M3D_MOUSE3D				M3D_TOTAL_TYPE(M3D_MOUSE3D_KIND, M3D_MECHOSOMA)
#define M3D_CONTROL_POINT		M3D_TOTAL_TYPE(M3D_CONTROL_POINT_KIND, M3D_MECHOSOMA)

#define MAX_SIZE_OBJECT			113
#define MAX_SIZE_NORMAL_BIT		7	
#define COLOR_GRADATION_BIT		3			/* ъюышўхёЄтю юЄЄхэъют ЎтхЄр <= MAX_SIZE_NORMAL_BIT */
#define COS_DEGREE				9			/* ёЄхяхэ№ cos яЁш чхЁъры№эюь юЄЁрцхэшш */
#define PALETTE_MAX_SIZE		6			/* for dos palette 0..63, for win palette 0..255 */
#define Z_SHADE_MAX				40
#define	TEXTURE_COORD			11
#define PALETTE_GRADATION_SIZE	(256*((2<<COLOR_GRADATION_BIT)+1))

#define FILE3DS_NAME_SIZE		255
#define TEXTURE_PATH_SIZE		255
#define TEXTURE_NAME_SIZE		30
#define TEXTURE_DIRECTORY		"DATA\\"
#define TEXTURE_EXTENSION		"bmp"
#define PALETTE_EXTENSION		"pal"
#define FILE_MAP_POLY			"output.pol"
#define FILE_MAP_TILE			"output.tlm"

#define ATTRIBUTE_LIGHT			0
#define ATTRIBUTE_SHADOW		(1<<0)
#define ATTRIBUTE_WATER			(1<<1)

#define MATERIAL_ATTRIBUTE(a)	((a)&0x00000FFF)
#define MESH_ATTRIBUTE_NULL		(0)
#define MESH_LIGHTING_MUL		(0)
#define POLYGON_SCALE_W			1.2f
#define POLYGON_CUTTING_W		10
#define MESH_GLOBAL_TRACK		0x80000000

#define COUNT_TRY_CACHE					10
#define CONST_SHADE_MESH				1.2f
#define CONST_DYNAMIC_SHADE_MESH		2.0f

#define GET_RENDER_TUNING(a)					((GlobalRenderTuning&(a))==(a))
enum eRenderTuningAttribute
{
	RENDER_TUNING_REFLECTION					=	1<<0,
	RENDER_TUNING_REFLECTION_SURFACE2			=	1<<1,
	RENDER_TUNING_REFLECTION_SURFACE_SHADOW		=	1<<2,
	RENDER_TUNING_REFLECTION_SHADOW				=	1<<3,
	RENDER_TUNING_WATER_SURFACE					=	1<<4,
	RENDER_TUNING_SHADOW						=	1<<5,
	RENDER_TUNING_REFLECTION_MECHOS				=	1<<6,
	RENDER_TUNING_REFLECTION_STATIC_OBJECT		=	1<<7,
	RENDER_TUNING_REFLECTION_DYNAMIC_OBJECT		=	1<<8,
	RENDER_TUNING_DYNAMIC_LIGHTING				=	1<<9,
	RENDER_TUNING_METAL							=	1<<10,
	RENDER_TUNING_OBJECT_REFLECTION_LIGHTING	=	1<<11,
	RENDER_TUNING_OBJECT_REFLECTION_MAP			=	1<<12,
	RENDER_TUNING_OBJECT_REFLECTION_SURFACE		=	1<<13,
	RENDER_TUNING_OBJECT_LIGHTING_CONST			=	1<<14,

	RENDER_TUNING_TILEMAP						=	1<<20,
	RENDER_TUNING_WATER							=	1<<21,
	RENDER_TUNING_SKY							=	1<<22,
	RENDER_TUNING_CLOUD							=	1<<23,
	RENDER_TUNING_SUN							=	1<<24,
	RENDER_TUNING_OMNI							=	1<<25,
	RENDER_TUNING_BASETRAIL						=	1<<26,
};

#define MATRIX_SIGN_X(a)		( (a))
#define MATRIX_SIGN_Y(a)		( (a))
#define MATRIX_SIGN_Z(a)		(-(a))

/******************************* HI COLOR MODE ************************************/
#define RGB1					0		/* R-11 G-5 B-0 */
#define R1_SHIFT				11
#define G1_SHIFT				5
#define B1_SHIFT				0
#define R1_MASK					31
#define G1_MASK					63
#define B1_MASK					31
#define GET_R1(color)			(((color)>>R1_SHIFT)&R1_MASK)
#define GET_G1(color)			(((color)>>G1_SHIFT)&G1_MASK)
#define GET_B1(color)			(((color)>>B1_SHIFT)&B1_MASK)

#define RGB2					1		/* R-10 G-5 B-0 */
#define R2_SHIFT				10
#define G2_SHIFT				5
#define B2_SHIFT				0
#define R2_MASK					31
#define G2_MASK					31
#define B2_MASK					31
#define GET_R2(color)			(((color)>>R2_SHIFT)&R2_MASK)
#define GET_G2(color)			(((color)>>G2_SHIFT)&G2_MASK)
#define GET_B2(color)			(((color)>>B2_SHIFT)&B2_MASK)

#define R_MASK_SHIFT(n)			(R##n##_MASK<<R##n##_SHIFT)
#define G_MASK_SHIFT(n)			(G##n##_MASK<<G##n##_SHIFT)
#define B_MASK_SHIFT(n)			(B##n##_MASK<<B##n##_SHIFT)
#define RGB_MASK_INV(n)			(0xFFFF^((1<<R##n##_SHIFT)+(1<<G##n##_SHIFT)+(1<<B##n##_SHIFT)))
#define FR_PAL16(r,n)			(round((r)*R##n##_MASK)<<R##n##_SHIFT)
#define FG_PAL16(g,n)			(round((g)*G##n##_MASK)<<G##n##_SHIFT)
#define FB_PAL16(b,n)			(round((b)*B##n##_MASK)<<B##n##_SHIFT)

const int NUMBER_CAMERA_MAX		=2;
const int NUMBER_3DDEVICE_MAX	=2;

extern int GlobalRenderTuning;

#pragma warning( disable : 4244 )  
#pragma warning( disable : 4056 )  

#endif // __BASE_H__

