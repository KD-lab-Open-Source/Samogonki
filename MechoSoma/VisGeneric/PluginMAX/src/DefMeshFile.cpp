#include "DefMeshFile.h"

sTypeField GlobalFieldType[256]=
{
	{	MF_TYPE_BLOCK,						"$BLOCK",					MF_TYPE_STRING,	},
	{	MF_TYPE_COMMENT,					"Comment",					MF_TYPE_STRING	},
	{	MF_TYPE_ID,							"ID",						MF_TYPE_UINT	},
	{	MF_TYPE_NAME,						"Name",						MF_TYPE_STRING	},
	{	MF_TYPE_FILENAME,					"FileName",					MF_TYPE_STRING	},
	{	MF_TYPE_AMBIENTCOLOR,				"AmbientColor",				MF_TYPE_FLOAT3	},
	{	MF_TYPE_DIFFUSECOLOR,				"DiffuseColor",				MF_TYPE_FLOAT3	},
	{	MF_TYPE_SPECULARCOLOR,				"SpecularColor",			MF_TYPE_FLOAT3	},
	{	MF_TYPE_SHININESS,					"Shininess",				MF_TYPE_FLOAT	},
	{	MF_TYPE_SHINSTRENGTH,				"ShinStrength",				MF_TYPE_FLOAT	},
	{	MF_TYPE_TRANSPARENCY,				"Transparency",				MF_TYPE_FLOAT	},
	{	MF_TYPE_NUMBER,						"Number",					MF_TYPE_UINT	},
	{	MF_TYPE_KEYVISIBILITY,				"KeyVisibility",			MF_TYPE_FLOAT2	},
	{	MF_TYPE_KEYPOSITION,				"KeyPosition",				MF_TYPE_FLOAT4	},
	{	MF_TYPE_KEYROTATION,				"KeyRotation",				MF_TYPE_FLOAT5	},
	{	MF_TYPE_KEYSCALE,					"KeyScale",					MF_TYPE_FLOAT4	},
	{	MF_TYPE_PARENT,						"Parent",					MF_TYPE_STRING	},
	{	MF_TYPE_MATRIX,						"Matrix",					MF_TYPE_FLOAT12	},
	{	MF_TYPE_NUMBERSUBOBJECT,			"NumberSubObject",			MF_TYPE_UINT	},
	{	MF_TYPE_VERTEXMESH,					"Vertex",					MF_TYPE_FLOAT3	},
	{	MF_TYPE_FACEMESH,					"Face",						MF_TYPE_INT4	},
	{	MF_TYPE_FIRSTFRAME,					"FirstFrame",				MF_TYPE_UINT	},
	{	MF_TYPE_LASTFRAME,					"LastFrame",				MF_TYPE_UINT	},
	{	MF_TYPE_FRAMESPEED,					"FrameSpeed",				MF_TYPE_UINT	},
	{	MF_TYPE_TICKSPERFRAME,				"TicksPerFrame",			MF_TYPE_UINT	},
	{	MF_TYPE_TIME,						"Time",						MF_TYPE_UINT	},
	{	MF_TYPE_LOD,						"LOD",						MF_TYPE_UINT	},
	{	MF_TYPE_TEXVERTEXMESH,				"TexVertex",				MF_TYPE_FLOAT3	},
	{	MF_TYPE_TEXFACEMESH,				"TexFace",					MF_TYPE_INT3	},
	{	MF_TYPE_CHANNELNUMBER,				"ChannelNumber",			MF_TYPE_UINT	},
	{	MF_TYPE_HELPERNAME,					"HelperName",				MF_TYPE_STRING	},
	{	MF_TYPE_NUMBERFRAME,				"NumberFrame",				MF_TYPE_UINT	},
	{	MF_TYPE_RESERV1,					"Reserv1",					MF_TYPE_UINT	},
	{	MF_TYPE_RESERV2,					"Reserv2",					MF_TYPE_UINT	},
	{	MF_TYPE_RESERV3,					"Reserv3",					MF_TYPE_UINT	},
	{	MF_TYPE_RESERV4,					"Reserv4",					MF_TYPE_UINT	},
	{	MF_TYPE_SHADING,					"Shading",					MF_TYPE_UINT	},

	{	0,									0,							0				}
};

sTypeField GlobalBlockID[]=
{
	{	MF_ID_BLOCK_NONE,					"NONE",						MF_TYPE_STRING	},
	{	MF_ID_BLOCK_SCENE,					"SCENE",					MF_TYPE_STRING	},
	{	MF_ID_BLOCK_LOD,					"LOD",						MF_TYPE_STRING	},
	{	MF_ID_BLOCK_CHANNEL,				"CHANNEL",					MF_TYPE_STRING	},
	{	MF_ID_BLOCK_MATERIAL,				"MATERIAL",					MF_TYPE_STRING	},
	{	MF_ID_BLOCK_SUBTEXMAP,				"SUBTEXMAP",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_OBJECT,					"OBJECT",					MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONVISIBILITY,	"ANIMATION_VISIBILITY",		MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONPOSITION,		"ANIMATION_POSITION",		MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONROTATION,		"ANIMATION_ROTATION",		MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONSCALE,			"ANIMATION_SCALE",			MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONMESH,			"ANIMATION_MESH",			MF_TYPE_STRING	},
	{	MF_ID_BLOCK_VERTEXMESH,				"VERTEX_MESH",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_FACEMESH,				"FACE_MESH",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_CHANNELMAPPING,			"CHANNEL_MAPPING",			MF_TYPE_STRING	},
	{	MF_ID_BLOCK_TEXVERTEXMESH,			"TEXVERTEX",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_TEXFACEMESH,			"TEXFACE",					MF_TYPE_STRING	},
	{	MF_ID_BLOCK_VERTEXNORMALMESH,		"VERTEXNORMAL",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_FACENORMALMESH,			"FACENORMAL",				MF_TYPE_STRING	},
	{	MF_ID_BLOCK_HELPEROBJECT,			"HELPER_OBJECT",			MF_TYPE_STRING	},
	{	MF_ID_BLOCK_ANIMATIONMATERIAL,		"ANIMATION_MATERIAL",		MF_TYPE_STRING	},

	{	0,									0,							0				}
};

sTypeVariable GlobalTypeVariable[]=
{
	{	0,						0		},
	{	MF_TYPE_CHAR,			sizeof(char)				},
	{	MF_TYPE_UCHAR,			sizeof(unsigned char)		},
	{	MF_TYPE_SHORT,			sizeof(short)				},
	{	MF_TYPE_USHORT,			sizeof(unsigned short)		},
	{	MF_TYPE_INT,			sizeof(int)					},
	{	MF_TYPE_UINT,			sizeof(unsigned int)		},
	{	MF_TYPE_FLOAT,			sizeof(float)				},
	{	MF_TYPE_DOUBLE,			sizeof(double)				},
	{	MF_TYPE_STRING,			-1							},
	{	MF_TYPE_FLOAT2,			2*sizeof(float)				},
	{	MF_TYPE_FLOAT3,			3*sizeof(float)				},
	{	MF_TYPE_FLOAT4,			4*sizeof(float)				},
	{	MF_TYPE_FLOAT5,			5*sizeof(float)				},
	{	MF_TYPE_FLOAT12,		12*sizeof(float)			},
	{	MF_TYPE_INT3,			3*sizeof(int)				},
	{	MF_TYPE_INT4,			4*sizeof(int)				},
	
	{	0,						0		}
};
