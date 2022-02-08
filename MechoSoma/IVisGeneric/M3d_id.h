//  Типы объектов
#define M3D_ENGINE					1
#define M3D_FRONT					2
#define M3D_RF_WHEEL				4
#define M3D_RB_WHEEL				8

#define M3D_LF_WHEEL				16
#define M3D_LB_WHEEL				32

// Объекты "bound"
#define M3D_BOUND					128

// Steeler
#define M3D_MECHOSOMA				256
// Kron
#define M3D_EFFECT					512
// Шура
#define M3D_XREAL					1024

#define M3D_STATIC_NON_COLLIDING	(1<<11)
#define M3D_STATIC_COLLIDING		(1<<12)
#define M3D_STATIC					(M3D_STATIC_NON_COLLIDING | M3D_STATIC_COLLIDING)
#define M3D_MORPH					(1<<13)
#define M3D_iSCREEN					16384
#define M3D_ONLYSCENE				(1<<15)

// Всего деталей
#define NUM_PARTS					6
// Активных (расположены в начале массива)
#define NUM_ACTIVE_PARTS			4

// Мехос в сборе
#define M3D_TOTAL_MECHOS (M3D_ENGINE | M3D_FRONT | M3D_RF_WHEEL | M3D_RB_WHEEL | M3D_LF_WHEEL | M3D_LB_WHEEL)

// Объекты M3D_MECHOSOMA
#define M3D_MOUSE3D_KIND		1
#define M3D_CONTROL_POINT_KIND		2
#define M3D_NITRO_BONUS_KIND		3
#define M3D_JUMP_BONUS_KIND		4
#define M3D_ENERGY_BONUS_KIND		5
#define M3D_ISCREEN_BACK_KIND		6
#define M3D_SPELL_BONUS_KIND		7
#define M3D_STAR_BONUS_KIND		8
#define M3D_ROBOT_KIND				9

// Объекты M3D_iSCREEN
#define M3D_CHAR0_KIND			1
#define M3D_WORLD0_KIND			300

#define MESH_DESTROYMECHOS				(1<<12)
#define MESH_CAMERAEFFECT				(1<<13)
#define MESH_HIERARCHY					(1<<14)
#define MESH_NOT_SHADE					(1<<15)
#define MESH_STATIC_SHADE				(1<<16)
#define MESH_WIRE_SCALE					(1<<17)
#define MESH_USE_OPACITY				(1<<18)
#define MESH_LIGHTING_MAX				(1<<19)
#define MESH_REFLECTION					(1<<20)
#define MESH_FIRST_DRAW					(1<<21)
#define MESH_NOT_METALL					(1<<22)
#define MESH_NOT_WRITEZBUFFER			(1<<23)
#define MESH_NOT_LIGHTING				(1<<24)
#define MESH_VISIBLE_SENSOR				(1<<25)
#define MESH_UNVISIBLE_SENSOR			(1<<26)
#define MESH_CAMERA_MASK_A				(1<<27)
#define MESH_CAMERA_MASK_B				(1<<28)
#define MESH_SHADE_ALWAYS				(1<<29)
#define MESH_CREATETILE					(1<<31)
