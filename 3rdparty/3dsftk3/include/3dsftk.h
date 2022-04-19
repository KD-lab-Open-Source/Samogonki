/**************************************************************************
 * 3DSFTK.H - 3D Studio File Toolkit for Release 3 library header 
 * 
 * (C) Copyright 1997 by Autodesk, Inc. 
 *
 * License Agreement
 *
 * This Autodesk Program is copyrighted by Autodesk, Inc. and is
 * licensed to you (individual or a legal entity) under the following
 * conditions:
 *
 * You may use, modify, copy, reproduce, distribute, sell, and market
 * the Autodesk Program, incorporated in whole or a portion thereof,
 * solely as a part of a Larger Work ("Larger Work" is defined as a
 * work which contains the Autodesk Program or portions thereof with
 * software/programs not governed by the terms of this License) provided
 * such Larger Works:
 *     (i)   are designed and intended to work solely with Autodesk, Inc.
 *           products,
 *     (ii.) conspicuously contain Autodesk's copyright notice
 *           "(C) Copyright 1995 by Autodesk, Inc.",
 *     (iii) contain a copy of this license along with the Autodesk
 *           Program, (iv) contain the disclaimer of warranty and all
 *           notices that refer to this License and to the absence of
 *           any warranty;
 *     (v)   add substantial value in addition to the Autodesk Program. 
 *
 * Any derivative or modification of this Autodesk Program must be
 * distributed, published and licensed under the same conditions as
 * this License.  
 *
 * You may not license or distribute the Autodesk Program as a standalone
 * program or product including OEM and private label.
 *
 * You may not use, copy, modify, sublicense or distribute the Autodesk
 * Program or any portion thereof in any form if such use or distribution
 * is not expressly licensed and or is expressly prohibited under this
 * License. 
 * 
 * You acknowledge and agree that Autodesk shall own all right, title
 * and interest in the Autodesk Program and all rights in patents whether
 * now known or hereafter discovered. You do not hold and shall not claim
 * any interest whatsoever in the Autodesk Program.  
 *
 * You agree that the Autodesk Program, any portion or derivative
 * thereof will not be shipped, transferred or exported into any country
 * or used in any manner prohibited by the United States Export
 * Administration Act or any other applicable export control law,
 * restriction or regulation.
 *
 * NO WARRANTY.
 * AUTODESK PROVIDES THIS PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * NON-INFRINGEMENT OF THIRD PARTY RIGHTS, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE. AUTODESK, INC. DOES
 * NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE UNINTERRUPTED
 * OR ERROR FREE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF
 * THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 * (NOT AUTODESK) ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR
 * OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
 * PART OF THIS LICENSE. NO USE OF THE PROGRAM IS AUTHORIZED HEREUNDER
 * EXCEPT UNDER THIS DISCLAIMER.
 *
 * LIMITATION OF LIABILITY.
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW WILL AUTODESK, OR ANY
 * OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THIS PROGRAM AS
 * PERMITTED HEREIN, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL,
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE
 * OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 * DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR
 * THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
 * SOFTWARE), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES. 
 *
 * This License will be governed by the laws of the State of California,
 * U.S.A., excluding the application of its conflicts of law rules.
 * If any part of this License is found void and unenforceable, it will
 * not affect the validity of the balance of the License, which shall
 * remain valid and enforceable according to its terms.
 *
 * This License and the rights granted hereunder will terminate
 * automatically if you fail to comply with the terms herein.  All
 * sublicenses to the Autodesk Program which are properly granted shall
 * survive any termination of this license.
 *************************************************************************/ 


#ifndef FTK3DS_H
#define FTK3DS_H

#include <stdio.h>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
					
/* Note: Item's prefixed with a "Priv" comment are internally used, are not
   part of the toolkit's published interface, and should not be used
   by non-ftk library functions */


/* Data Type Synonyms For Portability */

#define char3ds char
#define uchar3ds unsigned char
#define byte3ds char
#define ubyte3ds unsigned char
#define short3ds int16_t
#define ushort3ds uint16_t
#define long3ds int32_t
#define ulong3ds uint32_t
#define float3ds float
#define double3ds double
#define int3ds   int
#define void3ds void

/* 3DS File Toolkit revision level.  Future revisions will increment this
   integer number */
#define FTKRevisionLevel3ds 2

/*------------------------------------
  Error reporting and testing macros
  ----------------------------------*/

/* Pending error flag values for ftkerr3ds global */
#define FTK_ERROR True3ds
#define FTK_NO_ERROR False3ds

/*--- If ftkerr3ds shows that an error has occured, return */
#define ON_ERROR_RETURN {if(ftkerr3ds && !ignoreftkerr3ds) return;}
#define ON_ERROR_RETURNR(rval) {if(ftkerr3ds && !ignoreftkerr3ds) return rval;}

/*--- If ftkerr3ds shows that an error has occured, push error and return */
#define ADD_ERROR_RETURN(id) {if(ftkerr3ds){PushErrList3ds(id); if (!ignoreftkerr3ds) return;}}
#define ADD_ERROR_RETURNR(id, rval) {if(ftkerr3ds){PushErrList3ds(id); if (!ignoreftkerr3ds) return rval;}}

/*--- If ftkerr3ds shows that an error has occured, push error */
#define ADD_ERROR(id) {if (ftkerr3ds){PushErrList3ds(id);}}

/*--- Reset the the error flag */
#define CLEAR_ERROR {if (ftkerr3ds){ClearErrList3ds();}}

/*--- Simply set the error state: Good if you need to clean-up before returning  */
#define SET_ERROR {ftkerr3ds=True3ds;}

/*--- Set error state, push an error ID and return */
#define SET_ERROR_RETURN(id) {PushErrList3ds(id); if (!ignoreftkerr3ds) return;}
#define SET_ERROR_RETURNR(id, rval) {PushErrList3ds(id); if (!ignoreftkerr3ds) return rval;}

/* Prints errors and exits application */
#define PRINT_ERRORS_EXIT(file) {if(ftkerr3ds){DumpErrList3ds(file); if (!ignoreftkerr3ds) exit(-1);}}

/*----------------------------------------
  Commonly used structures and constants
  --------------------------------------*/

/* Constants to define True and False in a compiler independant way. */
#define TRUE3DS (1==1)
#define FALSE3DS (1!=1)
static const byte3ds True3ds = (1==1);
static const byte3ds False3ds = (1!=1);

/* point3ds - 3D point structure */
typedef struct 
{
   float3ds x, y, z;
} point3ds;

/* fcolor3ds - Floating point color definition */
typedef struct {
   float3ds r;   /* RGB Color components */
   float3ds g;
   float3ds b;
} fcolor3ds;       

/* face3ds - Face list element */
typedef struct
{
   ushort3ds v1, v2, v3, flag;
} face3ds;

/* textvert3ds - Texture assignment coordinate */
typedef struct
{
   float3ds u, v;
} textvert3ds;
/* Priv */ /* trackheader3ds - Global track settings */
/* Priv */ typedef struct
/* Priv */ {
/* Priv */    ushort3ds flags;
/* Priv */    ulong3ds nu1, nu2;
/* Priv */    ulong3ds keycount; /* Number of keys in the track */
/* Priv */ } trackheader3ds;

/* keyheader3ds - Animation key settings */
typedef struct
{
   ulong3ds time;       /* Key's frame position */
   ushort3ds rflags;    /* Spline terms used flag */
   float3ds tension;    /* Flagged with 0x01 */
   float3ds continuity; /* Flagged with 0x02 */
   float3ds bias;       /* Flagged with 0x04 */
   float3ds easeto;     /* Flagged with 0x08 */
   float3ds easefrom;   /* Flagged with 0x10 */
} keyheader3ds;

/* kfrotkey3ds - Rotation key */
typedef struct {
   float3ds angle; /* angle of rotation */
   float3ds x;     /* rotation axis vector */
   float3ds y;
   float3ds z;
} kfrotkey3ds;

/* kfmorphkey3ds - Object morph key */
typedef struct {
   char3ds name[13]; /* name of target morph object */
} kfmorphkey3ds;


/* Error codes generated by toolkit */
typedef enum
{
   NO_FTK_ERRORS = 0, /* No errors are pending */
   ERR_NO_MEM, /* Not enough memory to complete operation */
   ERR_INVALID_ARG, /* The argument passed to the function invalid.  Usually */
		    /* caused by a NULL pointer or an out of range numeric */
		    /* argument. */
   ERR_INVALID_DATA, /* The structure passed as an argument to the function has */
                     /* invalid or out of range data in its fields. */
   ERR_INVALID_CHUNK, /* An invalid chunk structure was encountered while reading */
		      /* the database. Usually caused by a corrupt database or */
		      /* file */
   ERR_INVALID_DATABASE, /* The database passed as an argument has not be created */
			 /* yet. */
   ERR_WRONG_DATABASE, /* The database passed as an argument is the wrong kind of */
		       /* database for this function. */
   ERR_UNFOUND_CHUNK, /* The database is missing important file chunks needed to */
		      /* fill out the requested structure.  Usually caused by a */
		      /* corrupt database or file */
   ERR_WRONG_OBJECT, /* The name passed to the functions exists, but is not the */
		     /* type of object asked for.  For example, asking for a mesh */
		     /* object with the GetCameraByName3ds function. */
   ERR_NO_SUCH_FILE, /* The filename passed as an argument for reading does not exist */
   ERR_INIT_FAILED,  /* Failed to initialize structure passed as an argument */
   ERR_OPENING_FILE, /* Could not open requested file */
   ERR_CLOSING_FILE, /* Could not close requested file */
   ERR_READING_FILE, /* Error occured while reading file */
   ERR_CREATING_DATABASE, /* Error occured while creating database */
   ERR_READING_DATABASE, /* Error occured while reading database */
   ERR_WRITING_DATABASE, /* Error occured while writing database */
   ERR_WRITING_FILE, /* Error occured while writing file */
   ERR_STRING_TOO_LONG, /* String encountered in file, structure, or as an */
			/* argument was longer than expected.  Possibly caused by */
			/* an uninitialed pointer, corrupt file or database */
   ERR_TOO_MANY_FILES, /* The toolkit has reached its maximum open file limit of */
		       /* 252 files. */
   ERR_GET_FAIL, /* Failed to get new data from database */
   ERR_PUT_FAIL, /* Failed to add new data to the database */
   N_ERRORS
}errorid3ds;

typedef struct {
  errorid3ds  id;
  const char3ds *desc;
} ErrRec3ds;


/* Priv */ /* chunktag3ds - enumeration of all possible chunk type identifiers. See appendix for */
/* Priv */ /* more information concerning chunk structures */   
/* Priv */ typedef enum
/* Priv */ {
/* Priv */         /* Trick Chunk Flags For ChunkSyntax function */

/* Priv */    NULL_CHUNK             = 0x0000,
/* Priv */    ChunkType              = 0x0995,
/* Priv */    ChunkUnique            = 0x0996,
/* Priv */    NotChunk               = 0x0997,
/* Priv */    Container              = 0x0998,
/* Priv */    IsChunk                = 0x0999,

/* Priv */    /* Dummy Chunk that sometimes appears in 3ds files created by prerelease 3D Studio R2 */
/* Priv */    DUMMY = 0xFFFF,

/* Priv */    /* Trick Chunk Types For Open*, Write*, Close* functions */

/* Priv */    POINT_ARRAY_ENTRY      = 0xF110,
/* Priv */    POINT_FLAG_ARRAY_ENTRY = 0xF111,
/* Priv */    FACE_ARRAY_ENTRY       = 0xF120,
/* Priv */    MSH_MAT_GROUP_ENTRY    = 0xF130,
/* Priv */    TEX_VERTS_ENTRY        = 0xF140,
/* Priv */    SMOOTH_GROUP_ENTRY     = 0xF150,
/* Priv */    POS_TRACK_TAG_KEY      = 0xF020,
/* Priv */    ROT_TRACK_TAG_KEY      = 0xF021,
/* Priv */    SCL_TRACK_TAG_KEY      = 0xF022,
/* Priv */    FOV_TRACK_TAG_KEY      = 0xF023,
/* Priv */    ROLL_TRACK_TAG_KEY     = 0xF024,
/* Priv */    COL_TRACK_TAG_KEY      = 0xF025,
/* Priv */    MORPH_TRACK_TAG_KEY    = 0xF026,
/* Priv */    HOT_TRACK_TAG_KEY      = 0xF027,
/* Priv */    FALL_TRACK_TAG_KEY     = 0xF028,
/* Priv */    
/* Priv */    /* 3DS File Chunk IDs */

/* Priv */    M3DMAGIC               = 0x4D4D,
/* Priv */    SMAGIC                 = 0x2D2D,
/* Priv */    LMAGIC                 = 0x2D3D,
/* Priv */    MLIBMAGIC              = 0x3DAA,
/* Priv */    MATMAGIC               = 0x3DFF,
/* Priv */    M3D_VERSION            = 0x0002,
/* Priv */    M3D_KFVERSION          = 0x0005,

/* Priv */    /* Mesh Chunk Ids */

/* Priv */    MDATA                  = 0x3D3D,
/* Priv */    MESH_VERSION           = 0x3D3E,
/* Priv */    COLOR_F                = 0x0010,
/* Priv */    COLOR_24               = 0x0011,
/* Priv */    LIN_COLOR_24           = 0x0012,
/* Priv */    LIN_COLOR_F            = 0x0013,
/* Priv */    INT_PERCENTAGE         = 0x0030,
/* Priv */    FLOAT_PERCENTAGE       = 0x0031,

/* Priv */    MASTER_SCALE           = 0x0100,

/* Priv */    BIT_MAP                = 0x1100,
/* Priv */    USE_BIT_MAP            = 0x1101,
/* Priv */    SOLID_BGND             = 0x1200,
/* Priv */    USE_SOLID_BGND         = 0x1201,
/* Priv */    V_GRADIENT             = 0x1300,
/* Priv */    USE_V_GRADIENT         = 0x1301,

/* Priv */    LO_SHADOW_BIAS         = 0x1400,
/* Priv */    HI_SHADOW_BIAS         = 0x1410,
/* Priv */    SHADOW_MAP_SIZE        = 0x1420,
/* Priv */    SHADOW_SAMPLES         = 0x1430,
/* Priv */    SHADOW_RANGE           = 0x1440,
/* Priv */    SHADOW_FILTER          = 0x1450,
/* Priv */    RAY_BIAS               = 0x1460,
/* Priv */  
/* Priv */    O_CONSTS               = 0x1500,
/* Priv */  
/* Priv */    AMBIENT_LIGHT          = 0x2100,

/* Priv */    FOG                    = 0x2200,
/* Priv */    USE_FOG                = 0x2201,
/* Priv */    FOG_BGND               = 0x2210,
/* Priv */    DISTANCE_CUE           = 0x2300,
/* Priv */    USE_DISTANCE_CUE       = 0x2301,
/* Priv */    LAYER_FOG              = 0x2302,
/* Priv */    USE_LAYER_FOG          = 0x2303,
/* Priv */    DCUE_BGND              = 0x2310,

/* Priv */    DEFAULT_VIEW           = 0x3000,
/* Priv */    VIEW_TOP               = 0x3010,
/* Priv */    VIEW_BOTTOM            = 0x3020,
/* Priv */    VIEW_LEFT              = 0x3030,
/* Priv */    VIEW_RIGHT             = 0x3040,
/* Priv */    VIEW_FRONT             = 0x3050,
/* Priv */    VIEW_BACK              = 0x3060,
/* Priv */    VIEW_USER              = 0x3070,
/* Priv */    VIEW_CAMERA            = 0x3080,
/* Priv */    VIEW_WINDOW            = 0x3090,

/* Priv */    NAMED_OBJECT           = 0x4000,
/* Priv */    OBJ_HIDDEN             = 0x4010,
/* Priv */    OBJ_VIS_LOFTER         = 0x4011,
/* Priv */    OBJ_DOESNT_CAST        = 0x4012,
/* Priv */    OBJ_MATTE              = 0x4013,
/* Priv */    OBJ_FAST               = 0x4014,
/* Priv */    OBJ_PROCEDURAL         = 0x4015,
/* Priv */    OBJ_FROZEN             = 0x4016,
/* Priv */    OBJ_DONT_RCVSHADOW     = 0x4017,

/* Priv */    N_TRI_OBJECT           = 0x4100,

/* Priv */    POINT_ARRAY            = 0x4110,
/* Priv */    POINT_FLAG_ARRAY       = 0x4111,
/* Priv */    FACE_ARRAY             = 0x4120,
/* Priv */    MSH_MAT_GROUP          = 0x4130,
/* Priv */    OLD_MAT_GROUP          = 0x4131,
/* Priv */    TEX_VERTS              = 0x4140,
/* Priv */    SMOOTH_GROUP           = 0x4150,
/* Priv */    MESH_MATRIX            = 0x4160,
/* Priv */    MESH_COLOR             = 0x4165,   
/* Priv */    MESH_TEXTURE_INFO      = 0x4170,
/* Priv */    PROC_NAME              = 0x4181,
/* Priv */    PROC_DATA              = 0x4182,
/* Priv */    MSH_BOXMAP             = 0x4190,

/* Priv */    N_D_L_OLD              = 0x4400,

/* Priv */    N_CAM_OLD              = 0x4500,   

/* Priv */    N_DIRECT_LIGHT         = 0x4600,
/* Priv */    DL_SPOTLIGHT           = 0x4610,
/* Priv */    DL_OFF                 = 0x4620,
/* Priv */    DL_ATTENUATE           = 0x4625,
/* Priv */    DL_RAYSHAD             = 0x4627,
/* Priv */    DL_SHADOWED            = 0x4630,
/* Priv */    DL_LOCAL_SHADOW        = 0x4640,
/* Priv */    DL_LOCAL_SHADOW2       = 0x4641,
/* Priv */    DL_SEE_CONE            = 0x4650,
/* Priv */    DL_SPOT_RECTANGULAR    = 0x4651,
/* Priv */    DL_SPOT_OVERSHOOT      = 0x4652,
/* Priv */    DL_SPOT_PROJECTOR      = 0x4653,
/* Priv */    DL_EXCLUDE             = 0x4654,
/* Priv */    DL_RANGE               = 0x4655, /* Not used in R3 */
/* Priv */    DL_SPOT_ROLL           = 0x4656,
/* Priv */    DL_SPOT_ASPECT         = 0x4657,
/* Priv */    DL_RAY_BIAS            = 0x4658,
/* Priv */    DL_INNER_RANGE = 0x4659,
/* Priv */    DL_OUTER_RANGE = 0x465A,
/* Priv */    DL_MULTIPLIER = 0x465B,

/* Priv */    N_AMBIENT_LIGHT        = 0x4680,

/* Priv */    N_CAMERA               = 0x4700,
/* Priv */    CAM_SEE_CONE           = 0x4710,
/* Priv */    CAM_RANGES             = 0x4720,

/* Priv */    HIERARCHY              = 0x4F00,
/* Priv */    PARENT_OBJECT          = 0x4F10,
/* Priv */    PIVOT_OBJECT           = 0x4F20,
/* Priv */    PIVOT_LIMITS           = 0x4F30,
/* Priv */    PIVOT_ORDER            = 0x4F40,
/* Priv */    XLATE_RANGE            = 0x4F50,

/* Priv */    POLY_2D                = 0x5000,

/* Priv */    /* Flags in shaper file that tell whether polys make up an ok shape */

/* Priv */    SHAPE_OK               = 0x5010,
/* Priv */    SHAPE_NOT_OK           = 0x5011,

/* Priv */    SHAPE_HOOK             = 0x5020,

/* Priv */    PATH_3D                = 0x6000,
/* Priv */    PATH_MATRIX            = 0x6005,
/* Priv */    SHAPE_2D               = 0x6010,
/* Priv */    M_SCALE                = 0x6020,
/* Priv */    M_TWIST                = 0x6030,
/* Priv */    M_TEETER               = 0x6040,
/* Priv */    M_FIT                  = 0x6050,
/* Priv */    M_BEVEL                = 0x6060,
/* Priv */    XZ_CURVE               = 0x6070,
/* Priv */    YZ_CURVE               = 0x6080,
/* Priv */    INTERPCT               = 0x6090,
/* Priv */    DEFORM_LIMIT           = 0x60A0,

/* Priv */    /* Flags for Modeler options */

/* Priv */    USE_CONTOUR            = 0x6100,
/* Priv */    USE_TWEEN              = 0x6110,
/* Priv */    USE_SCALE              = 0x6120,
/* Priv */    USE_TWIST              = 0x6130,
/* Priv */    USE_TEETER             = 0x6140,
/* Priv */    USE_FIT                = 0x6150,
/* Priv */    USE_BEVEL              = 0x6160,

/* Priv */    /* Viewport description chunks */

/* Priv */    VIEWPORT_LAYOUT_OLD    = 0x7000,
/* Priv */    VIEWPORT_DATA_OLD      = 0x7010,
/* Priv */    VIEWPORT_LAYOUT        = 0x7001,
/* Priv */    VIEWPORT_DATA          = 0x7011,
/* Priv */    VIEWPORT_DATA_3        = 0x7012,
/* Priv */    VIEWPORT_SIZE          = 0x7020,
/* Priv */    NETWORK_VIEW           = 0x7030,

/* Priv */    /* External Application Data */

/* Priv */    XDATA_SECTION          = 0x8000,
/* Priv */    XDATA_ENTRY            = 0x8001,
/* Priv */    XDATA_APPNAME          = 0x8002,
/* Priv */    XDATA_STRING           = 0x8003,
/* Priv */    XDATA_FLOAT            = 0x8004,
/* Priv */    XDATA_DOUBLE           = 0x8005,
/* Priv */    XDATA_SHORT            = 0x8006,
/* Priv */    XDATA_LONG = 0x8007,
/* Priv */    XDATA_VOID = 0x8008,
/* Priv */    XDATA_GROUP = 0x8009,
/* Priv */    XDATA_RFU6 = 0x800A,
/* Priv */    XDATA_RFU5 = 0x800B,
/* Priv */    XDATA_RFU4 = 0x800C,
/* Priv */    XDATA_RFU3 = 0x800D,
/* Priv */    XDATA_RFU2 = 0x800E,
/* Priv */    XDATA_RFU1 = 0x800F,
/* Priv */    
/* Priv */    PARENT_NAME = 0x80F0,

/* Priv */    /* Material Chunk IDs */

/* Priv */    MAT_ENTRY              = 0xAFFF,
/* Priv */    MAT_NAME               = 0xA000,
/* Priv */    MAT_AMBIENT            = 0xA010,
/* Priv */    MAT_DIFFUSE            = 0xA020,
/* Priv */    MAT_SPECULAR           = 0xA030,
/* Priv */    MAT_SHININESS          = 0xA040,
/* Priv */    MAT_SHIN2PCT			  = 0xA041,
/* Priv */    MAT_SHIN3PCT		     = 0xA042,
/* Priv */    MAT_TRANSPARENCY  	  = 0xA050,
/* Priv */    MAT_XPFALL             = 0xA052,
/* Priv */    MAT_REFBLUR            = 0xA053,

/* Priv */    MAT_SELF_ILLUM         = 0xA080,
/* Priv */    MAT_TWO_SIDE           = 0xA081,
/* Priv */    MAT_DECAL              = 0xA082,
/* Priv */    MAT_ADDITIVE           = 0xA083,
/* Priv */    MAT_SELF_ILPCT  	  = 0xA084,
/* Priv */    MAT_WIRE		  = 0xA085,
/* Priv */    MAT_SUPERSMP		  = 0xA086,
/* Priv */    MAT_WIRESIZE		  = 0xA087,
/* Priv */    MAT_FACEMAP		  = 0xA088,
/* Priv */    MAT_XPFALLIN		  = 0xA08A,
/* Priv */    MAT_PHONGSOFT	  = 0xA08C,
/* Priv */    MAT_WIREABS  	= 0xA08E,

/* Priv */    MAT_SHADING            = 0xA100,

/* Priv */    MAT_TEXMAP             = 0xA200,
/* Priv */    MAT_OPACMAP            = 0xA210,
/* Priv */    MAT_REFLMAP            = 0xA220,
/* Priv */    MAT_BUMPMAP            = 0xA230,
/* Priv */    MAT_SPECMAP            = 0xA204,
/* Priv */    MAT_USE_XPFALL         = 0xA240,
/* Priv */    MAT_USE_REFBLUR        = 0xA250,
/* Priv */    MAT_BUMP_PERCENT       = 0xA252,

/* Priv */    MAT_MAPNAME            = 0xA300,
/* Priv */    MAT_ACUBIC             = 0xA310,

/* Priv */    MAT_SXP_TEXT_DATA      = 0xA320,
/* Priv */    MAT_SXP_TEXT2_DATA     = 0xA321,
/* Priv */    MAT_SXP_OPAC_DATA      = 0xA322,
/* Priv */    MAT_SXP_BUMP_DATA      = 0xA324,
/* Priv */    MAT_SXP_SPEC_DATA      = 0xA325,
/* Priv */    MAT_SXP_SHIN_DATA 	  = 0xA326,
/* Priv */    MAT_SXP_SELFI_DATA 	  = 0xA328,
/* Priv */    MAT_SXP_TEXT_MASKDATA  = 0xA32A,
/* Priv */    MAT_SXP_TEXT2_MASKDATA = 0xA32C,
/* Priv */    MAT_SXP_OPAC_MASKDATA  = 0xA32E,
/* Priv */    MAT_SXP_BUMP_MASKDATA  = 0xA330,
/* Priv */    MAT_SXP_SPEC_MASKDATA  = 0xA332,
/* Priv */    MAT_SXP_SHIN_MASKDATA  = 0xA334,
/* Priv */    MAT_SXP_SELFI_MASKDATA = 0xA336,
/* Priv */    MAT_SXP_REFL_MASKDATA  = 0xA338,
/* Priv */    MAT_TEX2MAP 		  = 0xA33A,
/* Priv */    MAT_SHINMAP 		  = 0xA33C,
/* Priv */    MAT_SELFIMAP 	  = 0xA33D,
/* Priv */    MAT_TEXMASK 		  = 0xA33E,
/* Priv */    MAT_TEX2MASK 	  = 0xA340,
/* Priv */    MAT_OPACMASK 	  = 0xA342,
/* Priv */    MAT_BUMPMASK 	  = 0xA344,
/* Priv */    MAT_SHINMASK 	  = 0xA346,
/* Priv */    MAT_SPECMASK 	  = 0xA348,
/* Priv */    MAT_SELFIMASK 	  = 0xA34A,
/* Priv */    MAT_REFLMASK 	  = 0xA34C,
/* Priv */    MAT_MAP_TILINGOLD 	  = 0xA350,
/* Priv */    MAT_MAP_TILING 	  = 0xA351,
/* Priv */    MAT_MAP_TEXBLUR_OLD 	  = 0xA352,
/* Priv */    MAT_MAP_TEXBLUR 	  = 0xA353,
/* Priv */    MAT_MAP_USCALE 	  = 0xA354,
/* Priv */    MAT_MAP_VSCALE 	  = 0xA356,
/* Priv */    MAT_MAP_UOFFSET 	  = 0xA358,
/* Priv */    MAT_MAP_VOFFSET 	  = 0xA35A,
/* Priv */    MAT_MAP_ANG 		  = 0xA35C,
/* Priv */    MAT_MAP_COL1 	  = 0xA360,
/* Priv */    MAT_MAP_COL2 	  = 0xA362,
/* Priv */    MAT_MAP_RCOL 	  = 0xA364,
/* Priv */    MAT_MAP_GCOL 	  = 0xA366,
/* Priv */    MAT_MAP_BCOL 	  = 0xA368,

/* Priv */         /* Keyframe Chunk IDs */

/* Priv */    KFDATA                 = 0xB000,
/* Priv */    KFHDR                  = 0xB00A,
/* Priv */    AMBIENT_NODE_TAG       = 0xB001,
/* Priv */    OBJECT_NODE_TAG        = 0xB002,
/* Priv */    CAMERA_NODE_TAG        = 0xB003,
/* Priv */    TARGET_NODE_TAG        = 0xB004,
/* Priv */    LIGHT_NODE_TAG         = 0xB005,
/* Priv */    L_TARGET_NODE_TAG      = 0xB006,
/* Priv */    SPOTLIGHT_NODE_TAG     = 0xB007,

/* Priv */    KFSEG                  = 0xB008,
/* Priv */    KFCURTIME              = 0xB009,
/* Priv */    NODE_HDR               = 0xB010,
/* Priv */    INSTANCE_NAME          = 0xB011,
/* Priv */    PRESCALE               = 0xB012,
/* Priv */    PIVOT                  = 0xB013,
/* Priv */    BOUNDBOX               = 0xB014,
/* Priv */    MORPH_SMOOTH           = 0xB015,
/* Priv */    POS_TRACK_TAG          = 0xB020,
/* Priv */    ROT_TRACK_TAG          = 0xB021,
/* Priv */    SCL_TRACK_TAG          = 0xB022,
/* Priv */    FOV_TRACK_TAG          = 0xB023,
/* Priv */    ROLL_TRACK_TAG         = 0xB024,
/* Priv */    COL_TRACK_TAG          = 0xB025,
/* Priv */    MORPH_TRACK_TAG        = 0xB026,
/* Priv */    HOT_TRACK_TAG          = 0xB027,
/* Priv */    FALL_TRACK_TAG         = 0xB028,
/* Priv */    HIDE_TRACK_TAG         = 0xB029,
/* Priv */    NODE_ID                = 0xB030,
/* Priv */    
/* Priv */    CMAGIC = 0xC23D,

/* Priv */    C_MDRAWER = 0xC010,
/* Priv */    C_TDRAWER = 0xC020,
/* Priv */    C_SHPDRAWER = 0xC030,
/* Priv */    C_MODDRAWER = 0xC040,
/* Priv */    C_RIPDRAWER = 0xC050,
/* Priv */    C_TXDRAWER = 0xC060,
/* Priv */    C_PDRAWER = 0xC062,
/* Priv */    C_MTLDRAWER = 0xC064,
/* Priv */    C_FLIDRAWER = 0xC066,
/* Priv */    C_CUBDRAWER = 0xC067,
/* Priv */    C_MFILE = 0xC070,
/* Priv */    C_SHPFILE = 0xC080,
/* Priv */    C_MODFILE = 0xC090,
/* Priv */    C_RIPFILE = 0xC0A0,
/* Priv */    C_TXFILE = 0xC0B0,
/* Priv */    C_PFILE = 0xC0B2,
/* Priv */    C_MTLFILE = 0xC0B4,
/* Priv */    C_FLIFILE = 0xC0B6,
/* Priv */    C_PALFILE = 0xC0B8,
/* Priv */    C_TX_STRING = 0xC0C0,
/* Priv */    C_CONSTS = 0xC0D0,
/* Priv */    C_SNAPS = 0xC0E0,
/* Priv */    C_GRIDS = 0xC0F0,
/* Priv */    C_ASNAPS = 0xC100,
/* Priv */    C_GRID_RANGE = 0xC110,
/* Priv */    C_RENDTYPE = 0xC120,
/* Priv */    C_PROGMODE = 0xC130,
/* Priv */    C_PREVMODE = 0xC140,
/* Priv */    C_MODWMODE = 0xC150,
/* Priv */    C_MODMODEL = 0xC160,
/* Priv */    C_ALL_LINES = 0xC170,
/* Priv */    C_BACK_TYPE = 0xC180,
/* Priv */    C_MD_CS = 0xC190,
/* Priv */    C_MD_CE = 0xC1A0,
/* Priv */    C_MD_SML = 0xC1B0,
/* Priv */    C_MD_SMW = 0xC1C0,
/* Priv */    C_LOFT_WITH_TEXTURE = 0xC1C3,
/* Priv */    C_LOFT_L_REPEAT = 0xC1C4,
/* Priv */    C_LOFT_W_REPEAT = 0xC1C5,
/* Priv */    C_LOFT_UV_NORMALIZE = 0xC1C6,
/* Priv */    C_WELD_LOFT = 0xC1C7,
/* Priv */    C_MD_PDET = 0xC1D0,
/* Priv */    C_MD_SDET = 0xC1E0,
/* Priv */    C_RGB_RMODE = 0xC1F0,
/* Priv */    C_RGB_HIDE = 0xC200,
/* Priv */    C_RGB_MAPSW = 0xC202,
/* Priv */    C_RGB_TWOSIDE = 0xC204,
/* Priv */    C_RGB_SHADOW = 0xC208,
/* Priv */    C_RGB_AA = 0xC210,
/* Priv */    C_RGB_OVW = 0xC220,
/* Priv */    C_RGB_OVH = 0xC230,
/* Priv */    C_RGB_PICTYPE = 0xC240,
/* Priv */    C_RGB_OUTPUT = 0xC250,
/* Priv */    C_RGB_TODISK = 0xC253,
/* Priv */    C_RGB_COMPRESS = 0xC254,
/* Priv */    C_JPEG_COMPRESSION = 0xC255,
/* Priv */    C_RGB_DISPDEV = 0xC256,
/* Priv */    C_RGB_HARDDEV = 0xC259,
/* Priv */    C_RGB_PATH = 0xC25A,
/* Priv */    C_BITMAP_DRAWER = 0xC25B,
/* Priv */    C_RGB_FILE = 0xC260,
/* Priv */    C_RGB_OVASPECT = 0xC270,

/* Priv */    C_RGB_ANIMTYPE = 0xC271,
/* Priv */    C_RENDER_ALL = 0xC272,
/* Priv */    C_REND_FROM = 0xC273,
/* Priv */    C_REND_TO = 0xC274,
/* Priv */    C_REND_NTH = 0xC275,
/* Priv */    C_PAL_TYPE = 0xC276,
/* Priv */    C_RND_TURBO = 0xC277,
/* Priv */    C_RND_MIP	= 0xC278,
/* Priv */    C_BGND_METHOD = 0xC279,
/* Priv */    C_AUTO_REFLECT = 0xC27A,
/* Priv */    C_VP_FROM = 0xC27B,
/* Priv */    C_VP_TO = 0xC27C,
/* Priv */    C_VP_NTH = 0xC27D,
/* Priv */    C_REND_TSTEP = 0xC27E,
/* Priv */    C_VP_TSTEP = 0xC27F,

/* Priv */    C_SRDIAM = 0xC280,
/* Priv */    C_SRDEG = 0xC290,
/* Priv */    C_SRSEG = 0xC2A0,
/* Priv */    C_SRDIR = 0xC2B0,
/* Priv */    C_HETOP = 0xC2C0,
/* Priv */    C_HEBOT = 0xC2D0,
/* Priv */    C_HEHT = 0xC2E0,
/* Priv */    C_HETURNS = 0xC2F0,
/* Priv */    C_HEDEG = 0xC300,
/* Priv */    C_HESEG = 0xC310,
/* Priv */    C_HEDIR = 0xC320,
/* Priv */    C_QUIKSTUFF = 0xC330,
/* Priv */    C_SEE_LIGHTS = 0xC340,
/* Priv */    C_SEE_CAMERAS = 0xC350,
/* Priv */    C_SEE_3D = 0xC360,
/* Priv */    C_MESHSEL = 0xC370,
/* Priv */    C_MESHUNSEL = 0xC380,
/* Priv */    C_POLYSEL = 0xC390,
/* Priv */    C_POLYUNSEL = 0xC3A0,
/* Priv */    C_SHPLOCAL = 0xC3A2,
/* Priv */    C_MSHLOCAL = 0xC3A4,
/* Priv */    C_NUM_FORMAT = 0xC3B0,
/* Priv */    C_ARCH_DENOM = 0xC3C0,
/* Priv */    C_IN_DEVICE = 0xC3D0,
/* Priv */    C_MSCALE = 0xC3E0,
/* Priv */    C_COMM_PORT = 0xC3F0,
/* Priv */    C_TAB_BASES = 0xC400,
/* Priv */    C_TAB_DIVS = 0xC410,
/* Priv */    C_MASTER_SCALES = 0xC420,
/* Priv */    C_SHOW_1STVERT = 0xC430,
/* Priv */    C_SHAPER_OK = 0xC440,
/* Priv */    C_LOFTER_OK = 0xC450,
/* Priv */    C_EDITOR_OK = 0xC460,
/* Priv */    C_KEYFRAMER_OK = 0xC470,
/* Priv */    C_PICKSIZE = 0xC480,
/* Priv */    C_MAPTYPE = 0xC490,
/* Priv */    C_MAP_DISPLAY = 0xC4A0,
/* Priv */    C_TILE_XY = 0xC4B0,
/* Priv */    C_MAP_XYZ = 0xC4C0,
/* Priv */    C_MAP_SCALE = 0xC4D0,
/* Priv */    C_MAP_MATRIX_OLD = 0xC4E0,
/* Priv */    C_MAP_MATRIX = 0xC4E1,
/* Priv */    C_MAP_WID_HT = 0xC4F0,
/* Priv */    C_OBNAME = 0xC500,
/* Priv */    C_CAMNAME = 0xC510,
/* Priv */    C_LTNAME = 0xC520,
/* Priv */    C_CUR_MNAME = 0xC525,
/* Priv */    C_CURMTL_FROM_MESH = 0xC526,
/* Priv */    C_GET_SHAPE_MAKE_FACES = 0xC527,
/* Priv */    C_DETAIL = 0xC530,
/* Priv */    C_VERTMARK = 0xC540,
/* Priv */    C_MSHAX = 0xC550,
/* Priv */    C_MSHCP = 0xC560,
/* Priv */    C_USERAX = 0xC570,
/* Priv */    C_SHOOK = 0xC580,
/* Priv */    C_RAX = 0xC590,
/* Priv */    C_STAPE = 0xC5A0,
/* Priv */    C_LTAPE = 0xC5B0,
/* Priv */    C_ETAPE = 0xC5C0,
/* Priv */    C_KTAPE = 0xC5C8,
/* Priv */    C_SPHSEGS = 0xC5D0,
/* Priv */    C_GEOSMOOTH = 0xC5E0,
/* Priv */    C_HEMISEGS = 0xC5F0,
/* Priv */    C_PRISMSEGS = 0xC600,
/* Priv */    C_PRISMSIDES = 0xC610,
/* Priv */    C_TUBESEGS = 0xC620,
/* Priv */    C_TUBESIDES = 0xC630,
/* Priv */    C_TORSEGS = 0xC640,
/* Priv */    C_TORSIDES = 0xC650,
/* Priv */    C_CONESIDES = 0xC660,
/* Priv */    C_CONESEGS = 0xC661,
/* Priv */    C_NGPARMS = 0xC670,
/* Priv */    C_PTHLEVEL = 0xC680,
/* Priv */    C_MSCSYM = 0xC690,
/* Priv */    C_MFTSYM = 0xC6A0,
/* Priv */    C_MTTSYM = 0xC6B0,
/* Priv */    C_SMOOTHING = 0xC6C0,
/* Priv */    C_MODICOUNT = 0xC6D0,
/* Priv */    C_FONTSEL = 0xC6E0,
/* Priv */    C_TESS_TYPE = 0xC6f0,
/* Priv */    C_TESS_TENSION = 0xC6f1,

/* Priv */    C_SEG_START = 0xC700,
/* Priv */    C_SEG_END = 0xC705,
/* Priv */    C_CURTIME = 0xC710,
/* Priv */    C_ANIMLENGTH = 0xC715,
/* Priv */    C_PV_FROM = 0xC720,
/* Priv */    C_PV_TO = 0xC725,
/* Priv */    C_PV_DOFNUM = 0xC730,
/* Priv */    C_PV_RNG = 0xC735,
/* Priv */    C_PV_NTH = 0xC740,
/* Priv */    C_PV_TYPE = 0xC745,
/* Priv */    C_PV_METHOD = 0xC750,
/* Priv */    C_PV_FPS = 0xC755,
/* Priv */    C_VTR_FRAMES = 0xC765,
/* Priv */    C_VTR_HDTL = 0xC770,
/* Priv */    C_VTR_HD = 0xC771,
/* Priv */    C_VTR_TL = 0xC772,
/* Priv */    C_VTR_IN = 0xC775,
/* Priv */    C_VTR_PK = 0xC780,
/* Priv */    C_VTR_SH = 0xC785,

/* Priv */ /* Material chunks */

/* Priv */    C_WORK_MTLS = 0xC790,	  /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_2 = 0xC792, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_3 = 0xC793, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_4 = 0xC794, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_5 = 0xCB00, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_6 = 0xCB01, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_7 = 0xCB02, /* Old-style -- now ignored */
/* Priv */    C_WORK_MTLS_8 = 0xCB03, /* Old-style -- now ignored */
/* Priv */    C_WORKMTL = 0xCB04,
/* Priv */    C_SXP_TEXT_DATA = 0xCB10,
/* Priv */    C_SXP_TEXT2_DATA = 0xCB20,
/* Priv */    C_SXP_OPAC_DATA = 0xCB11,
/* Priv */    C_SXP_BUMP_DATA = 0xCB12,
/* Priv */    C_SXP_SPEC_DATA = 0xCB24,
/* Priv */    C_SXP_SHIN_DATA = 0xCB13,
/* Priv */    C_SXP_SELFI_DATA = 0xCB28,
/* Priv */    C_SXP_TEXT_MASKDATA = 0xCB30,
/* Priv */    C_SXP_TEXT2_MASKDATA = 0xCB32,
/* Priv */    C_SXP_OPAC_MASKDATA = 0xCB34,
/* Priv */    C_SXP_BUMP_MASKDATA = 0xCB36,
/* Priv */    C_SXP_SPEC_MASKDATA = 0xCB38,
/* Priv */    C_SXP_SHIN_MASKDATA = 0xCB3A,
/* Priv */    C_SXP_SELFI_MASKDATA = 0xC3C,
/* Priv */    C_SXP_REFL_MASKDATA = 0xCB3E,

/* Priv */    C_BGTYPE = 0xC7A1,
/* Priv */    C_MEDTILE = 0xC7B0,

/* Priv */ /* Contrast */

/* Priv */    C_LO_CONTRAST = 0xC7D0,
/* Priv */    C_HI_CONTRAST = 0xC7D1,

/* Priv */ /* 3d frozen display */

/* Priv */    C_FROZ_DISPLAY = 0xC7E0,

/* Priv */ /* Booleans */
/* Priv */    C_BOOLWELD = 0xc7f0,
/* Priv */    C_BOOLTYPE = 0xc7f1,

/* Priv */    C_ANG_THRESH = 0xC900,
/* Priv */    C_SS_THRESH = 0xC901,
/* Priv */    C_TEXTURE_BLUR_DEFAULT = 0xC903,

/* Priv */    C_MAPDRAWER = 0xCA00,
/* Priv */    C_MAPDRAWER1 = 0xCA01,
/* Priv */    C_MAPDRAWER2 = 0xCA02,
/* Priv */    C_MAPDRAWER3 = 0xCA03,
/* Priv */    C_MAPDRAWER4 = 0xCA04,
/* Priv */    C_MAPDRAWER5 = 0xCA05,
/* Priv */    C_MAPDRAWER6 = 0xCA06,
/* Priv */    C_MAPDRAWER7 = 0xCA07,
/* Priv */    C_MAPDRAWER8 = 0xCA08,
/* Priv */    C_MAPDRAWER9 = 0xCA09,
/* Priv */    C_MAPDRAWER_ENTRY = 0xCA10,

/* Priv */ /* system options */
/* Priv */    C_BACKUP_FILE = 0xCA20,
/* Priv */    C_DITHER_256 = 0xCA21,
/* Priv */    C_SAVE_LAST = 0xCA22,
/* Priv */    C_USE_ALPHA = 0xCA23,
/* Priv */    C_TGA_DEPTH = 0xCA24,
/* Priv */    C_REND_FIELDS = 0xCA25,
/* Priv */    C_REFLIP = 0xCA26,
/* Priv */    C_SEL_ITEMTOG = 0xCA27,
/* Priv */    C_SEL_RESET = 0xCA28,
/* Priv */    C_STICKY_KEYINF = 0xCA29,
/* Priv */    C_WELD_THRESHOLD = 0xCA2A,
/* Priv */    C_ZCLIP_POINT = 0xCA2B,
/* Priv */    C_ALPHA_SPLIT = 0xCA2C,
/* Priv */    C_KF_SHOW_BACKFACE = 0xCA30,
/* Priv */    C_OPTIMIZE_LOFT = 0xCA40,
/* Priv */    C_TENS_DEFAULT = 0xCA42,
/* Priv */    C_CONT_DEFAULT = 0xCA44,
/* Priv */    C_BIAS_DEFAULT = 0xCA46,

/* Priv */    C_DXFNAME_SRC  = 0xCA50,
/* Priv */    C_AUTO_WELD  = 0xCA60,
/* Priv */    C_AUTO_UNIFY  = 0xCA70,
/* Priv */    C_AUTO_SMOOTH  = 0xCA80,
/* Priv */    C_DXF_SMOOTH_ANG  = 0xCA90,
/* Priv */    C_SMOOTH_ANG  = 0xCAA0,

/* Priv */ /* Special network-use chunks */

/* Priv */    C_NET_USE_VPOST = 0xCC00,
/* Priv */    C_NET_USE_GAMMA = 0xCC10,
/* Priv */    C_NET_FIELD_ORDER = 0xCC20,

/* Priv */    C_BLUR_FRAMES = 0xCD00,
/* Priv */    C_BLUR_SAMPLES = 0xCD10,
/* Priv */    C_BLUR_DUR = 0xCD20,
/* Priv */    C_HOT_METHOD = 0xCD30,
/* Priv */    C_HOT_CHECK = 0xCD40,
/* Priv */    C_PIXEL_SIZE = 0xCD50,
/* Priv */    C_DISP_GAMMA = 0xCD60,
/* Priv */    C_FBUF_GAMMA = 0xCD70,
/* Priv */    C_FILE_OUT_GAMMA = 0xCD80,
/* Priv */    C_FILE_IN_GAMMA = 0xCD82,
/* Priv */    C_GAMMA_CORRECT = 0xCD84,
/* Priv */    C_APPLY_DISP_GAMMA = 0xCD90  /* OBSOLETE */,
/* Priv */    C_APPLY_FBUF_GAMMA = 0xCDA0  /* OBSOLETE */,
/* Priv */    C_APPLY_FILE_GAMMA = 0xCDB0  /* OBSOLETE */,
/* Priv */    C_FORCE_WIRE = 0xCDC0,
/* Priv */    C_RAY_SHADOWS = 0xCDD0,
/* Priv */    C_MASTER_AMBIENT = 0xCDE0,
/* Priv */    C_SUPER_SAMPLE = 0xCDF0,
/* Priv */    C_OBJECT_MBLUR = 0xCE00,
/* Priv */    C_MBLUR_DITHER = 0xCE10,
/* Priv */    C_DITHER_24 = 0xCE20,
/* Priv */    C_SUPER_BLACK = 0xCE30,
/* Priv */    C_SAFE_FRAME = 0xCE40,
/* Priv */    C_VIEW_PRES_RATIO = 0xCE50,
/* Priv */    C_BGND_PRES_RATIO = 0xCE60,
/* Priv */    C_NTH_SERIAL_NUM = 0xCE70,

/* Priv */    /* Video Post */
/* Priv */    VPDATA = 0xd000,

/* Priv */    P_QUEUE_ENTRY = 0xd100,
/* Priv */    P_QUEUE_IMAGE = 0xd110,
/* Priv */    P_QUEUE_USEIGAMMA = 0xd114,
/* Priv */    P_QUEUE_PROC = 0xd120,
/* Priv */    P_QUEUE_SOLID = 0xd130,
/* Priv */    P_QUEUE_GRADIENT = 0xd140,
/* Priv */    P_QUEUE_KF = 0xd150,
/* Priv */    P_QUEUE_MOTBLUR = 0xd152,
/* Priv */    P_QUEUE_MB_REPEAT = 0xd153,
/* Priv */    P_QUEUE_NONE = 0xd160,

/* Priv */    P_QUEUE_RESIZE = 0xd180,
/* Priv */    P_QUEUE_OFFSET = 0xd185,
/* Priv */    P_QUEUE_ALIGN = 0xd190,

/* Priv */    P_CUSTOM_SIZE = 0xd1a0,

/* Priv */    P_ALPH_NONE = 0xd210,
/* Priv */    P_ALPH_PSEUDO = 0xd220	/* Old chunk */,
/* Priv */    P_ALPH_OP_PSEUDO = 0xd221	/* Old chunk */,
/* Priv */    P_ALPH_BLUR = 0xd222		/* Replaces pseudo */,
/* Priv */    P_ALPH_PCOL = 0xd225,
/* Priv */    P_ALPH_C0 = 0xd230,
/* Priv */    P_ALPH_OP_KEY = 0xd231,
/* Priv */    P_ALPH_KCOL = 0xd235,
/* Priv */    P_ALPH_OP_NOCONV = 0xd238,
/* Priv */    P_ALPH_IMAGE = 0xd240,
/* Priv */    P_ALPH_ALPHA = 0xd250,
/* Priv */    P_ALPH_QUES = 0xd260,
/* Priv */    P_ALPH_QUEIMG = 0xd265,
/* Priv */    P_ALPH_CUTOFF = 0xd270,
/* Priv */    P_ALPHANEG = 0xd280,

/* Priv */    P_TRAN_NONE = 0xd300,
/* Priv */    P_TRAN_IMAGE = 0xd310,
/* Priv */    P_TRAN_FRAMES = 0xd312,
/* Priv */    P_TRAN_FADEIN = 0xd320,
/* Priv */    P_TRAN_FADEOUT = 0xd330,
/* Priv */    P_TRANNEG = 0xd340,

/* Priv */    P_RANGES = 0xd400,

/* Priv */    P_PROC_DATA = 0xd500

/* Priv */ } chunktag3ds;


/* Priv */ /* chunk3ds - Internal database representation of file information */
/* Priv */ typedef struct chunk3ds {
/* Priv */    chunktag3ds tag;           /* Type of chunk */
/* Priv */    ulong3ds     size;         /* Number of bytes used by chunk */
/* Priv */    ulong3ds     position;     /* Offset in source file */
/* Priv */    void        *data;         /* Memory copy of file data */
/* Priv */    struct chunk3ds *sibling;  /* Next chunk in database */
/* Priv */    struct chunk3ds *children; /* Chunks contained within this chunk */
/* Priv */    ubyte3ds readindex;        /* Index of source file in file table */
/* Priv */    ubyte3ds writeindex;       /* Index of destination file in file table */
/* Priv */ } chunk3ds;

/* Priv */ /* chunklistentry3ds - Cross reference between name and chunk */
/* Priv */ typedef struct {
/* Priv */    char3ds *name;                  /* Chunk Name list */
/* Priv */    chunk3ds *chunk;                /* Corresponding Pos */
/* Priv */ } chunklistentry3ds;

/* Priv */ /* chunklist3ds - List of cross references */
/* Priv */ typedef struct 
/* Priv */ {
/* Priv */    ulong3ds count; /* Number of entries in list */
/* Priv */    chunklistentry3ds *list; /* Contents of list */
/* Priv */ } chunklist3ds;


/* namelistentry3ds - Name of database object */
typedef struct 
{
   char3ds *name; /* Name of object */
} namelistentry3ds;

/* namelist3ds - List of database object names */
typedef struct
{
   ulong3ds count;          /*--- How many entries are in list */
   ulong3ds spaces;         /*--- How much space for entries */
   namelistentry3ds *list;  /*--- char ** to access pointers */
} namelist3ds;


/* Priv */ /* filestate3ds - Read/write state of a file */
/* Priv */ typedef enum 
/* Priv */ {
/* Priv */    StateNotKnown = 0, /* No state determined yet, file not open */
/* Priv */    ReadFromFile = 1,  /* Open for read */
/* Priv */    WriteToFile = 2,   /* Open for write */
/* Priv */    ReadWriteFile = 3  /* Open for read and write, not used by toolkit */
/* Priv */ } filestate3ds;

/* file3ds - Handle to 3D Studio files */
typedef struct
{
   FILE *file;         /* stdio file pointer */
   char3ds *filename;  /* name of file */
   filestate3ds state; /* file open state */
   ubyte3ds index;     /* Position of pointer in the FileContextList, if the
                          index eq InvalidFileContext3ds, then it is not in the
                          list */
   void *buffer;       /* Buffered I/O buffer */
   ulong3ds buffersize;
   byte3ds istempfile; /* True3ds if file is a temp file */
} file3ds;

/* Priv */ /* dbtype3ds - Database type settings */
/* Priv */ typedef enum
/* Priv */ {
/* Priv */    Unknown,      /* Database has not be created yet */
/* Priv */    MeshFile,     /* 3D Studio .3DS file */
/* Priv */    ProjectFile,  /* 3D STudio .PRJ file */
/* Priv */    MaterialFile  /* 3D Studio .MLI file */
/* Priv */ } dbtype3ds;

/* database3ds - File database */
typedef struct
{
   chunk3ds *topchunk; /* top chunk in the file */
   byte3ds objlistdirty; /* If true, than *objlist needs to be recreated */
   byte3ds matlistdirty;
   byte3ds nodelistdirty;
   chunklist3ds *objlist; /* Quick Cross references between names and database chunks */
   chunklist3ds *matlist;
   chunklist3ds *nodelist;
} database3ds;
typedef enum
{
   NoView3ds,
   TopView3ds,
   BottomView3ds,
   LeftView3ds,
   RightView3ds,
   FrontView3ds,
   BackView3ds,
   UserView3ds,
   CameraView3ds,
   SpotlightView3ds
} viewtype3ds;

typedef struct
{
   ushort3ds xpos;
   ushort3ds ypos;
   ushort3ds width;
   ushort3ds height;
} viewsize3ds;

/* Used to describe top, bottom, left, right, front and back views */
typedef struct
{
   point3ds center; /* Center of orthagonal view */
   float3ds zoom; /* View zoom factor */
} orthoview3ds;

/* Used to describe user views */
typedef struct
{
   point3ds center; /* Center of user view */
   float3ds zoom; /* View zoom factor */
   float3ds horang; /* Horizontal angle of view */
   float3ds verang; /* Veritical angle of view */
} userview3ds;

/* Used to describe camera views */
typedef struct
{
   char3ds name[11]; /* Name of the camera used in the view */
} cameraview3ds;

/* Viewport structure details the kind of view in a viewport */
typedef struct
{
   viewtype3ds type; /* top, bottom, left, right, front, back, user and camera */
   viewsize3ds size; /* Size of the viewport */
   orthoview3ds ortho; /* used for top, bottom, left, right, front, and back views */
   userview3ds user; /* Used for user views */
   cameraview3ds camera; /* Used for camera views */
} viewport3ds;
typedef enum 
{
   UseShadowMap,
   UseRayTraceShadow
} shadowstyle3ds;

typedef struct { /* global shadow settings */
   shadowstyle3ds type; /* Either UseShadowMaps or UseRayTraceShadows */
   float3ds bias; /* shadow bias factor. */
   float3ds raybias; /* shadow ray bais factor. Used in R3 */
   short3ds mapsize; /* shadow map size */
   float3ds filter; /* shadow filter */
} shadowsets3ds;
   
typedef struct {
   float3ds masterscale; /* Master mesh scale factor */
   shadowsets3ds shadow; /* Global shadow settings */
   fcolor3ds ambientlight;  /* ambient light color */
   point3ds oconsts; /* default object constructing axis */
} meshset3ds;
typedef enum
{
   NoAtmo, /* no active astmospherics */
   UseFog, /* fog astmostpheric */
   UseLayerFog, /* layer fog astmostpheric */
   UseDistanceCue /* distance cue astmostpheric */
} atmospheretype3ds;

typedef enum
{
   NoFall, /* no falloff */
   TopFall, /* falloff to the top */
   BottomFall /* falloff to the bottom */
} layerfogfalloff3ds;

typedef struct /* fog atmosphere parameters */
{
   float3ds nearplane; /* near radius of fog effect */
   float3ds neardensity; /* near fog density */
   float3ds farplane; /* far radius of fog effect */
   float3ds fardensity; /* far fog density */
   fcolor3ds     fogcolor; /* color of fog effect */
   byte3ds    fogbgnd; /* True3ds / False3ds "fog background" flag */
} fogsettings3ds;

typedef struct /* layered fog atmosphere parameters */
{
   float3ds zmin; /* lower bounds of fog */
   float3ds zmax; /* upper bounds of fog */
   float3ds density; /* fog density */
   fcolor3ds fogcolor; /* fog color */
   layerfogfalloff3ds falloff; /* falloff style */
   byte3ds fogbgnd; /* True3ds / False3ds "fog background" flag */
} layerfogsettings3ds;

typedef struct /* distance cue atmosphere parameters */
{
   float3ds nearplane; /* near radius of effect */
   float3ds neardim; /* near dimming factor */
   float3ds farplane; /* far radius of effect */
   float3ds fardim; /* far dimming factor */
   byte3ds dcuebgnd; /* True3ds / False3ds effect the background flag */
} dcuesettings3ds;

typedef struct {
   fogsettings3ds fog; /* fog atmostphere settings */
   layerfogsettings3ds layerfog;  /* layered fog atmosphere parameters */
   dcuesettings3ds dcue;  /* distance cue atmosphere parameters */
   atmospheretype3ds activeatmo; /* The active astmospheric */
} atmosphere3ds;

/* enumerate list of possible backgrounds used in file */
typedef enum
{
   NoBgnd,
   UseSolidBgnd,
   UseVGradientBgnd, 
   UseBitmapBgnd
} backgroundtype3ds;

typedef struct 
{
   char3ds *name; /* name of background bitmap */
} bitmapbgnd3ds;

typedef struct 
{
   fcolor3ds color; /* color of solid background */
} solidbgnd3ds;

typedef struct 
{
   float3ds gradpercent; /* Placement of mid color band, ranges from 0-1 */ 
   fcolor3ds  top; /* color of top band */
   fcolor3ds  mid; /* color of mid background band */
   fcolor3ds  bottom; /* color of bottom band */
} vgradientbgnd3ds;

typedef struct {
   bitmapbgnd3ds bitmap;
   solidbgnd3ds solid;
   vgradientbgnd3ds vgradient;
   backgroundtype3ds bgndused; /* background in effect */
} background3ds;

/* Used for shading field in material3ds structure */
typedef enum
{
   Wire = 0,
   Flat = 1,
   Gouraud = 2,
   Phong = 3,
   Metal = 4
} shadetype3ds;

/* Used for tiling field in bitmap3ds structure */
typedef enum 
{
   Tile = 1,
   Decal = 2,
   Both = 3
} tiletype3ds;

typedef enum
{
   Pyramidal,
   SummedArea
} filtertype3ds;

typedef enum
{
   RGB,
   Alpha,
   RGBLumaTint,
   AlphaTint,
   RGBTint
} tinttype3ds;


/* Used By AddMaterial3ds */

typedef struct {
   ubyte3ds firstframe; /* True for First Frame Only */
   ubyte3ds flat; /* True for flat mirror reflection */
   ulong3ds size;     /* Map resolution */
   ulong3ds nthframe; /* Map update period */
} acubic3ds;          /* Cubic reflection map defintion */

typedef struct {
   char3ds  name[13];  /* Bitmap file name */
   float3ds percent;  /* Strength percentage */
   tiletype3ds tiling;  /* Tile/Decal/Both */
   byte3ds ignorealpha;
   filtertype3ds filter; /* Pyramidal/Summed Area */
   float3ds blur;
   byte3ds mirror;
   byte3ds negative;
   float3ds uscale, vscale;
   float3ds uoffset, voffset;
   float3ds rotation;
   tinttype3ds source; /* RGB/RGB Luma Tint/Alpha Tint/RGB Tint */
   fcolor3ds tint1;
   fcolor3ds tint2;
   fcolor3ds redtint;
   fcolor3ds greentint;
   fcolor3ds bluetint;
   ulong3ds datasize; /* Size of procedural data */
   void3ds *data;     /* Procedural data */
} bitmap3ds;          /* Bit map definition */

/* Structure to all map settings */
typedef struct {
   bitmap3ds map; /* The map settings */
   bitmap3ds mask; /* The mask settings */
} mapset3ds;

typedef struct {
   bitmap3ds map; /* The map settings */
   byte3ds useauto; /* True3ds if automatic reflections are being used */
   acubic3ds automap; /* Automatic reflection definitions */
   bitmap3ds mask; /* The mask settings */
} rmapset3ds;
   
typedef struct {
   char3ds name[17];      /* Name */
   fcolor3ds ambient;     /* Ambient light color */
   fcolor3ds diffuse;     /* Diffuse light color */
   fcolor3ds specular;    /* Specular light color */
   float3ds shininess;    /* Shininess factor */
   float3ds shinstrength; /* Shininess strength */
   float3ds blur;         /* Blur factor */
   float3ds transparency; /* Trasparency factor */
   float3ds transfalloff; /* Fallof factor */
   float3ds selfillumpct;     /* Self illumination percentage */
   float3ds wiresize;     /* Width of wireframe */
   shadetype3ds shading;      /* Shading type */
   byte3ds useblur;       /* Blurring flag */
   byte3ds usefall;       /* Transparency falloff flag */
   byte3ds twosided;      /* Two sided material flag */
   byte3ds selfillum;     /* Self illumination flag */
   byte3ds additive;      /* Additive transparency flag */
   byte3ds usewire;       /* Use wireframe rendering */
   byte3ds usewireabs;    /* Wire size is in units, not pixels. */
   byte3ds facemap;       /* Face mapping switch */
   byte3ds soften;        /* Soften switch */
   mapset3ds texture;     /* Texture map settings */
   mapset3ds texture2;    /* Second texture map settings */
   mapset3ds opacity;     /* Opacity map settings */
   mapset3ds bump;       /* Bump map settings */
   mapset3ds specmap;     /* Specularity map settings */
   mapset3ds shinmap;     /* Shininess map settings */
   mapset3ds illummap;    /* Self illumination map settings */
   rmapset3ds reflect;    /* Reflection map settings */
} material3ds;            /* Material definition */

/* mapinfo3ds - Texture map icon placement */
typedef struct {
   ushort3ds maptype;    /* Icon type */
   float3ds tilex;       /* Tiling */
   float3ds tiley;
   float3ds cenx;        /* Position of center */
   float3ds ceny;
   float3ds cenz;
   float3ds scale;       /* Icon scaling factor */
   float3ds matrix[12];  /* Orientation matrix */
   float3ds pw;          /* Planar icon width */
   float3ds ph;          /* Planar icon height */
   float3ds ch;          /* Cylinder icon height */
} mapinfo3ds;            

/* objmat3ds - Material assignments by face */
typedef struct {
   char3ds name[17];     /* Material name */
   ushort3ds nfaces;     /* Number of faces using material */
   ushort3ds *faceindex; /* List of faces using material */
} objmat3ds;             

/* mesh3ds - Mesh object definition */
typedef struct {
   char3ds name[11];       /* Object name */
   byte3ds ishidden;       /* Hidden object flag */
   byte3ds isvislofter;    /* Lofter visibility flag */
   byte3ds ismatte;        /* Matte object flag */
   byte3ds isnocast;       /* Doesn't cast shadow's flag */
   byte3ds isfast;         /* Fast display flag */
   byte3ds isnorcvshad;    /* Doesn't recieve shadows */
   byte3ds isfrozen;       /* Frozen object flag */
   ushort3ds nvertices;    /* Vertice count */
   point3ds *vertexarray;  /* List of vertices */
   ushort3ds nvflags;      /* Number of vertex flags */
   ushort3ds *vflagarray;  /* List of vertex flags */
   ushort3ds ntextverts;   /* Number of texture vertices */   
   textvert3ds *textarray; /* List of texture coordinates */
   byte3ds usemapinfo;     /* Boolean for use of mapping icon information */
   mapinfo3ds map;         /* Mapping icon info */
   float3ds locmatrix[12]; /* Object orientation matrix */
   ushort3ds nfaces;       /* Face count */
   face3ds *facearray;     /* List of faces */
   ulong3ds *smootharray;  /* Smoothing group assignment list */
   byte3ds useboxmap;     /* Boolean used to indicate the use of box mapping */
   char3ds boxmap[6][17];  /* Material names used in boxmapping */
   ubyte3ds meshcolor;     /* UI color assigned to the mesh */
   ushort3ds nmats;        /* Assigned materials count */
   objmat3ds *matarray;    /* Material assignment list */
   byte3ds useproc;        /* Use animated stand-in flag */
   ulong3ds procsize;      /* Size of animated stand-in data */
   char3ds procname[13];   /* Name of animated stand-in procedure */
   void3ds *procdata;      /* Animated stand-in data */
} mesh3ds;                  

/* conestyle3ds - Spolight projection cone shape. */
typedef enum
{
   Circular,
   Rectangular
} conestyle3ds;

/* spotshadow - Spotlight shadow settings */
typedef struct {
   byte3ds cast; /* True if spotlight casts shadows */
   shadowstyle3ds type; /* UseShadow or UseRayTrace */
   byte3ds local; /* True if local shadow settings are being used */
   float3ds bias; /* Shadow bias */
   float3ds filter; /* Shadow filter */
   ushort3ds mapsize; /* Shadow map size */
   float3ds raybias; /* Ray tracing shadow bias */
} spotshadow3ds; 

/* spotcond3ds - cone visability settings */
typedef struct {
   conestyle3ds type; /* Circular or rectangular light cone */
   byte3ds show; /* True if cone is visable */
   byte3ds overshoot;  /* True if cone overshoot is on */
} spotcone3ds;

/* spotproject3ds - spotlight projectio bitmap */
typedef struct {
   byte3ds use; /* True if using projector */
   char3ds *bitmap; /* Name of projector bitmap */
} spotprojector3ds;

/* spotlight3ds - spotlight settings */
typedef struct
{
   point3ds target;  /* Spotlight target */
   float3ds hotspot; /* Hotspot angle */
   float3ds falloff; /* Hotspot falloff */
   float3ds roll;    /* Roll angle */
   float3ds aspect;  /* Aspect ratio */
   spotshadow3ds shadows; 
   spotcone3ds cone;
   spotprojector3ds projector;
} spotlight3ds;

/* liteattenuate3ds - Light attenuation settings */
typedef struct {
   byte3ds on; /* True if light attenuation is on */
   float3ds inner; /* Inner range of attenuation */
   float3ds outer; /* Outer range of attenuation */ 
} liteattenuate3ds;

/* light3ds - omni and spotlight settings */
typedef struct {
   char3ds name[11];   /* Light name */
   point3ds pos; /* Light position */
   fcolor3ds color; /* Light color */
   float3ds multiplier;/* Light intensity multiplier */
   byte3ds dloff; /* True if light is off */
   liteattenuate3ds attenuation;
   namelist3ds *exclude;
   spotlight3ds *spot; /* If not null, then struct is a spotlight, else omni. */
} light3ds;

/* camrange3ds - Camera atomosphere ranges */
typedef struct {
   float3ds cam_near; /* Nearest effect radiu */
   float3ds cam_far; /* Farthest effect radius */
} camranges3ds;

typedef struct 
{
   char3ds name[11];
   point3ds position;
   point3ds target;
   float3ds roll;
   float3ds fov;
   byte3ds showcone;
   camranges3ds ranges;
} camera3ds;

typedef struct {
  ulong3ds length;
  ulong3ds curframe;
} kfkeyinfo3ds;

typedef struct {
  byte3ds use;
  ulong3ds begin;
  ulong3ds end;
} kfsegment3ds;

typedef struct
{
  kfkeyinfo3ds anim;
  kfsegment3ds seg;
} kfsets3ds;


typedef struct {
  char3ds name[11];    /* Name of camera object */
  char3ds parent[22];  /* Name of parent object */
  
  ushort3ds flags1;    /* flags field from node header -fixup later */
  ushort3ds flags2;    /* flags2 field from node header -fixup later */
  
  ulong3ds npkeys;     /* Number of camera position keys */
  ushort3ds npflag;    /* Loop control flag for camera position keys */
  keyheader3ds *pkeys; /* Spline values for camera position keys */
  point3ds *pos;       /* Camera position keys */
  
  ulong3ds nfkeys;     /* Number of camera FOV keys */
  ushort3ds nfflag;    /* Loop control flag for camera FOV keys */
  keyheader3ds *fkeys; /* Spline values for camera FOV keys */
  float3ds *fov;       /* Camera FOV keys */
  
  ulong3ds nrkeys;     /* Number of camera roll keys */
  ushort3ds nrflag;    /* Loop control flag for camera roll keys */
  keyheader3ds *rkeys; /* Spline values for camera roll keys */
  float3ds *roll;      /* Camera roll keys */
  
  char3ds tparent[22]; /* Index of parent object for target */
  ulong3ds ntkeys;     /* Number of target position keys */
  ushort3ds ntflag;    /* Loop control flag for target position keys */
  keyheader3ds *tkeys; /* Spline values for target position keys */
  point3ds *tpos;      /* Target position keys */
  ushort3ds tflags1;   /* flags field from target node header */
  ushort3ds tflags2;   /* flags field from target node header */
} kfcamera3ds;

/* kfambient3ds - Ambient light animation */
typedef struct {
   ushort3ds flags1;    /* flags field from node header -fixup later     */
   ushort3ds flags2;    /* flags2 field from node header -fixup later    */

   ulong3ds nckeys;     /* Number of color keys                          */
   ushort3ds ncflag;    /* Loop control flag for color keys              */
   keyheader3ds *ckeys; /* Spline values for position keys               */
   fcolor3ds *color;    /* Color keys                                    */
} kfambient3ds;
/* Used By ObjectMotion3ds */

typedef struct {
   char3ds name[11];     /* Name of mesh */
   char3ds parent[22];   /* Name of parent object */
   ushort3ds flags1;     /* flags field from node header  */
   ushort3ds flags2;     /* flags2 field from node header  */

   point3ds pivot;       /* Object pivot point */
   char3ds instance[11]; /* Object instance name */
   point3ds boundmin;    /* Minimum bounding box point for dummy objects */
   point3ds boundmax;    /* Maximum bounding box point for dummy objects */

   ulong3ds npkeys;      /* Number of position keys */
   short3ds npflag;      /* Loop control flag for position keys */
   keyheader3ds *pkeys;  /* Spline values for position keys */
   point3ds *pos;        /* Mesh position keys */

   ulong3ds nrkeys;      /* Number of rotation keys */
   short3ds nrflag;      /* Loop control flag for rotation keys */
   keyheader3ds *rkeys;  /* Spline values for rotation keys */
   kfrotkey3ds *rot;     /* Rotation keys */

   ulong3ds nskeys;      /* Number of scaling keys */
   short3ds nsflag;      /* Loop control flag for scaling keys */
   keyheader3ds *skeys;  /* Spline values for scaling */
   point3ds *scale;      /* Mesh scaling keys */

   ulong3ds nmkeys;      /* Number of morph keys */
   short3ds nmflag;      /* Loop control flag for morph keys */
   keyheader3ds *mkeys;  /* Spline values for morph keys */
   kfmorphkey3ds *morph; /* Morph keys */

   ulong3ds nhkeys;      /* Number of hide keys */
   short3ds nhflag;      /* Loop control flag for hide keys */
   keyheader3ds *hkeys;  /* Spline values for hide keys */
   float3ds msangle;     /* Morph smoothing group angle */
} kfmesh3ds;

/* Used By OmnilightMotion3ds */

typedef struct {
   char3ds name[11];    /* Name of the light object node */
   char3ds parent[22];  /* Name of the parent object */

   ushort3ds flags1;    /* flags field from node header -fixup later */
   ushort3ds flags2;    /* flags2 field from node header -fixup later */

   ulong3ds npkeys;     /* Number of position keys */
   ushort3ds npflag;     /* Loop control flag for position keys */
   keyheader3ds *pkeys; /* Spline values for position keys */
   point3ds *pos;       /* Position keys */

   ulong3ds nckeys;     /* Number of color keys */
   ushort3ds ncflag;     /* Loop control flag for color keys */
   keyheader3ds *ckeys; /* Spline values for position keys */
   fcolor3ds *color;   /* Color keys */

} kfomni3ds;

typedef struct {
   char3ds name[11];    /* Name of camera object */
   char3ds parent[22];  /* Parent name */

   ushort3ds flags1;    /* flags field from node header -fixup later */
   ushort3ds flags2;    /* flags2 field from node header -fixup later */

#ifdef broken
   short3ds visable;    /* Flags to control visability */
#endif
   ulong3ds npkeys;     /* Number of light position keys */
   ushort3ds npflag;     /* Loop control flag for position keys */
   keyheader3ds *pkeys; /* Spline values for light position keys */
   point3ds *pos;       /* Light position keys */

   ulong3ds nckeys;     /* Number of color keys */
   ushort3ds ncflag;     /* Loop control flag color keys */
   keyheader3ds *ckeys; /* Spline values for color keys */
   fcolor3ds *color;    /* Color keys */

   ulong3ds nhkeys;     /* Number of hotspot angle keys */
   ushort3ds nhflag;     /* Loop control flag for hotspot angle keys */
   keyheader3ds *hkeys; /* Spline values for hotspot angle keys */
   float3ds *hot;       /* Hotspot angle keys */

   ulong3ds nfkeys;     /* Number of falloff angle keys */
   ushort3ds nfflag;     /* Loop control flag for falloff angle keys */
   keyheader3ds *fkeys; /* Spline values for falloff angle keys */
   float3ds *fall;      /* Falloff angle keys */

   ulong3ds nrkeys;     /* Number of light roll keys */
   ushort3ds nrflag;     /* Loop control flag for light roll keys */
   keyheader3ds *rkeys; /* Spline values for light roll keys */
   float3ds *roll;      /* Light roll keys */

   char3ds tparent[22];    /* Name of target's parent object */
   ulong3ds ntkeys;     /* Number of target position keys */
   ushort3ds ntflag;     /* Loop control flag for target position keys */
   keyheader3ds *tkeys; /* Spline values for target position keys */
   point3ds *tpos;      /* Target position keys */
   ushort3ds tflags1;   /* flags field from target node header */
   ushort3ds tflags2;   /* flags field from target node header */

} kfspot3ds;
typedef struct {
   long3ds size;
   void3ds *data;
} xdataraw3ds;


/*----------------------------------------
  Constants for commonly used structures
  --------------------------------------*/

/* 3DS File Toolkit revision level.  Future revisions will increment this
   integer number */
static const long3ds __FTKRevisionLevel3ds = FTKRevisionLevel3ds; 

/* Flags used by the flag field of the mesh3ds structure */
static const ushort3ds FaceCAVisable3ds = 0x0001;  /* Flags the CA edge as visable */
static const ushort3ds FaceBCVisable3ds = 0x0002;  /* Flags the BC edge as visable */
static const ushort3ds FaceABVisable3ds = 0x0004;  /* Flags the AB edge as visable */
static const ushort3ds FaceUWrap3ds     = 0x0008;  /* Flags the face as being at
						      a texture coord u wrap
						      seam */
static const ushort3ds FaceVWrap3ds     = 0x0010;  /* Flags the face as being at
						      a texture coord v wrap
						      seam */

/* Flags used by the rflags field of the keyheader3ds structure */
static const ushort3ds KeyUsesTension3ds = 0x01;
static const ushort3ds KeyUsesCont3ds    = 0x02;
static const ushort3ds KeyUsesBias3ds    = 0x04;
static const ushort3ds KeyUsesEaseTo3ds  = 0x08;
static const ushort3ds KeyUsesEaseFrom3ds= 0x10;

/* Flags used by the track flags field */
static const ushort3ds TrackSingle3ds  = 0x0000;
static const ushort3ds TrackLoops3ds   = 0x0003;
static const ushort3ds TrackRepeats3ds = 0x0002;
static const ushort3ds TrackLockX3ds   = 0x0008;
static const ushort3ds TrackLockY3ds   = 0x0010;
static const ushort3ds TrackLockZ3ds   = 0x0020;
static const ushort3ds TrackNoLinkX3ds = 0x0100;
static const ushort3ds TrackNoLinkY3ds = 0x0200;
static const ushort3ds TrackNoLinkZ3ds = 0x0400;

/* Basic structure default contents for ease of initialization */
static const point3ds DefPoint3ds = {0.0F, 0.0F, 0.0F};
static const textvert3ds DefTextVert3ds = {0.0F, 0.0F};
static const face3ds DefFace3ds = {0, 1, 2, 0};
static const trackheader3ds DefTrackHeader3ds = {0, 0, 0, 1};
static const keyheader3ds DefKeyHeader3ds = {0, 0, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
/* Last toolkit error */
extern byte3ds ftkerr3ds; /* True3ds if error is pending */
extern byte3ds ignoreftkerr3ds; /* True3ds if errors are being ignored by toolkit */
/* Fog flags */
static const ulong3ds LayerFogBgnd = 0x100000;
static const ulong3ds NoFalloff = 0x0;
static const ulong3ds TopFalloff = 0x2;
static const ulong3ds BottomFalloff = 0x1;


/* Flags for initflags parameter */
static const ushort3ds InitNoExtras3ds    = 0x0000;
static const ushort3ds InitVertexArray3ds = 0x0001;
static const ushort3ds InitTextArray3ds   = 0x0002;
static const ushort3ds InitFaceArray3ds   = 0x0004;
static const ushort3ds InitMatArray3ds    = 0x0008;
static const ushort3ds InitSmoothArray3ds = 0x0010;
static const ushort3ds InitProcData3ds    = 0x0020;
static const ushort3ds InitVFlagArray3ds  = 0x0040;


/* Smoothing group flags used in the smootharray field of the mesh3ds structure */
static const ulong3ds Smooth01Group3ds = 0x00000001;
static const ulong3ds Smooth02Group3ds = 0x00000002;
static const ulong3ds Smooth03Group3ds = 0x00000004;
static const ulong3ds Smooth04Group3ds = 0x00000008;
static const ulong3ds Smooth05Group3ds = 0x00000010;
static const ulong3ds Smooth06Group3ds = 0x00000020;
static const ulong3ds Smooth07Group3ds = 0x00000030;
static const ulong3ds Smooth08Group3ds = 0x00000080;
static const ulong3ds Smooth09Group3ds = 0x00000100;
static const ulong3ds Smooth10Group3ds = 0x00000200;
static const ulong3ds Smooth11Group3ds = 0x00000400;
static const ulong3ds Smooth12Group3ds = 0x00000800;
static const ulong3ds Smooth13Group3ds = 0x00001000;
static const ulong3ds Smooth14Group3ds = 0x00002000;
static const ulong3ds Smooth15Group3ds = 0x00004000;
static const ulong3ds Smooth16Group3ds = 0x00008000;
static const ulong3ds Smooth17Group3ds = 0x00010000;
static const ulong3ds Smooth18Group3ds = 0x00020000;
static const ulong3ds Smooth19Group3ds = 0x00040000;
static const ulong3ds Smooth20Group3ds = 0x00080000;
static const ulong3ds Smooth21Group3ds = 0x00100000;
static const ulong3ds Smooth22Group3ds = 0x00200000;
static const ulong3ds Smooth23Group3ds = 0x00400000;
static const ulong3ds Smooth24Group3ds = 0x00800000;
static const ulong3ds Smooth25Group3ds = 0x01000000;
static const ulong3ds Smooth26Group3ds = 0x02000000;
static const ulong3ds Smooth27Group3ds = 0x04000000;
static const ulong3ds Smooth28Group3ds = 0x08000000;
static const ulong3ds Smooth29Group3ds = 0x10000000;
static const ulong3ds Smooth30Group3ds = 0x20000000;
static const ulong3ds Smooth31Group3ds = 0x40000000;
static const ulong3ds Smooth32Group3ds = 0x80000000;
static const char3ds *DummyName3ds = "$$$DUMMY";
/* Flag settings for kfmesh3ds, kfomni3ds, kfcamera3ds, kfambient, and kfspot3ds */

/* For the flags field */
static const ushort3ds KfNodeOff3ds = 1<<3;
static const ushort3ds KfHideNode3ds = 1<<11;
static const ushort3ds KfFastNode3ds = 1<<12;

/* For the flags2 field */
static const ushort3ds KfNodeHasPath3ds = 1;
static const ushort3ds KfNodeAutoSmooth3ds = 1<<1;
static const ushort3ds KfNodeFrozen3ds = 1<<2;
static const ushort3ds KfMotionBlur3ds = 1<<4;
static const ushort3ds KfBlurBranch3ds = 1<<5;
static const ushort3ds KfMorphMtl3ds   = 1<<6;
static const ushort3ds KfMorphOb       = 1<<7;

typedef enum
{
  LightTarget,
  CameraTarget
} targettype3ds;

static const long3ds  TerseDump3ds = 0; /* Just the chunk headers */
static const long3ds MediumDump3ds = 1; /* Selected bits of data (usually not array dumps) */
static const long3ds MaximumDump3ds = 2; /* Everything */
/* Priv */ void PushErrList3ds(errorid3ds id);
/* Priv */ const char3ds *ErrCodeToString3ds(errorid3ds err);
void DumpErrList3ds(FILE *outfile);
void ClearErrList3ds();
const ErrRec3ds *ReturnErrorList3ds();


/* Functions to use with namelist3ds structures */
void InitNameList3ds(namelist3ds **chunk, ulong3ds count);
void ReleaseNameList3ds(namelist3ds **chunk);
void AddToNameList3ds(namelist3ds **list, char3ds *name);

/* File control functions */
file3ds *OpenFile3ds(const char3ds *filename, const char *attrib);
file3ds *OpenFile3ds_buf(char3ds *buf, long3ds len);
void CloseAllFiles3ds();
void CloseFile3ds(file3ds *file);


/* Database functions */
void InitDatabase3ds(database3ds **db);
void ReleaseDatabase3ds(database3ds **db);
void CreateDatabase3ds(file3ds *file, database3ds *db);
void CreateNewDatabase3ds(database3ds *db, dbtype3ds type);
dbtype3ds GetDatabaseType3ds(database3ds *db);
void CopyDatabase3ds(database3ds *idb, database3ds *odb);
void DisconnectDatabase3ds(database3ds *db);
void WriteDatabase3ds(file3ds *file, database3ds *db);
void InitViewport3ds(viewport3ds **vp);
void ReleaseViewport3ds(viewport3ds **vp);
void GetViewport3ds(database3ds *db, viewport3ds **vp);
void PutViewport3ds(database3ds *db, viewport3ds *vp);
void CopyViewport3ds(database3ds *destdb, database3ds *srcdb);
void InitMeshSet3ds(meshset3ds **);
void ReleaseMeshSet3ds(meshset3ds **);
void GetMeshSet3ds(database3ds *, meshset3ds **);
void PutMeshSet3ds(database3ds *db, meshset3ds *settings);
void CopyMeshSet3ds(database3ds *destdb, database3ds *srcdb);

void InitAtmosphere3ds(atmosphere3ds **atmo);
void ReleaseAtmosphere3ds(atmosphere3ds **atmo);
void GetAtmosphere3ds(database3ds *db, atmosphere3ds **atmo);
void PutAtmosphere3ds(database3ds *db, atmosphere3ds *atmo);
void CopyAtmosphere3ds(database3ds *destdb, database3ds *srcdb);

void InitBackground3ds(background3ds **bgnd);
void ReleaseBackground3ds(background3ds **bgnd);
void GetBackground3ds(database3ds *db, background3ds **bgnd);
void PutBackground3ds(database3ds *db, background3ds *bgnd);
void CopyBackground3ds(database3ds *destdb, database3ds *srcdb);

short3ds AddMaterial3ds(const material3ds *mat);
void InitBitmap3ds(bitmap3ds *map);
short3ds InitMaterial3ds(material3ds **mat);
void ReleaseMaterial3ds(material3ds **mat);
ulong3ds GetMaterialCount3ds(database3ds *db);
void GetMaterialNameList3ds(database3ds *db, namelist3ds **list);
void GetMaterialByIndex3ds(database3ds *db, ulong3ds index, material3ds **mat);
void GetMaterialByName3ds(database3ds *db, char3ds *name, material3ds **mat);
void PutMaterial3ds(database3ds *db, material3ds *mat);
void CopyMaterialByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyMaterialByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteMaterialByName3ds(database3ds *db, char3ds *name);
void DeleteMaterialByIndex3ds(database3ds *db, ulong3ds index);
void InitMeshObj3ds(mesh3ds **obj, ushort3ds nvertices, ushort3ds nfaces, ushort3ds iniflags);
void InitMeshObjField3ds(mesh3ds *obj, ushort3ds const initflags);
void InitMatArrayIndex3ds (mesh3ds *obj, ushort3ds mat, ushort3ds nfaces);

/* Mesh object functions */
ulong3ds GetMeshCount3ds(database3ds *db);
void GetMeshNameList3ds(database3ds *db, namelist3ds **list);
void GetMeshByIndex3ds(database3ds *db, ulong3ds index, mesh3ds **mesh);
void GetMeshByName3ds(database3ds *db, char3ds *name, mesh3ds **mesh);
void PutMesh3ds(database3ds *db, mesh3ds *mesh);
void CopyMeshByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyMeshByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteMeshByName3ds(database3ds *db, char3ds *name);
void DeleteMeshByIndex3ds(database3ds *db, ulong3ds index);
void RelMeshObj3ds(mesh3ds **obj);

/* Light related functions */
void InitLight3ds(light3ds **light);
void InitSpotlight3ds(light3ds **spotlight);
void ReleaseLight3ds(light3ds **light);
ulong3ds GetOmnilightCount3ds(database3ds *db);
ulong3ds GetSpotlightCount3ds(database3ds *db);
void GetOmnilightNameList3ds(database3ds *db, namelist3ds **list);
void GetSpotlightNameList3ds(database3ds *db, namelist3ds **list);
void GetSpotlightByIndex3ds(database3ds *db, long3ds index, light3ds **light);
void GetOmnilightByIndex3ds(database3ds *db, long3ds index, light3ds **light);
void GetSpotlightByName3ds(database3ds *db, char3ds *name, light3ds **light);
void GetOmnilightByName3ds(database3ds *db, char3ds *name, light3ds **light);
void PutSpotlight3ds(database3ds *db, light3ds *light);
void PutOmnilight3ds(database3ds *db, light3ds *light);
void CopySpotlightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopySpotlightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyOmnilightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyOmnilightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteSpotlightByIndex3ds(database3ds *db, ulong3ds index);
void DeleteOmnilightByIndex3ds(database3ds *db, ulong3ds index);
void DeleteSpotlightByName3ds(database3ds *db, char3ds *name);
void DeleteOmnilightByName3ds(database3ds *db, char3ds *name);
void InitCamera3ds(camera3ds **cam);
void ReleaseCamera3ds(camera3ds **cam);
ulong3ds GetCameraCount3ds(database3ds *db);
void GetCameraNameList3ds(database3ds *db, namelist3ds **list);
void GetCameraByIndex3ds(database3ds *db, ulong3ds index, camera3ds **cam);
void GetCameraByName3ds(database3ds *db, char3ds *name, camera3ds **cam);
void PutCamera3ds(database3ds *db, camera3ds *cam);

void CopyCameraByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyCameraByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);

void DeleteCameraByName3ds(database3ds *db, char3ds *name);
void DeleteCameraByIndex3ds(database3ds *db, ulong3ds index);

void InitKfSets3ds(kfsets3ds **key);
void ReleaseKfSets3ds(kfsets3ds **key);
void PutKfSets3ds(database3ds *db, kfsets3ds *key);
void GetKfSets3ds(database3ds *db, kfsets3ds **key);
void CopyKfSets3ds(database3ds *destdb, database3ds *srcdb);


void InitCameraMotion3ds(kfcamera3ds **cam, ulong3ds npkeys, ulong3ds nfkeys,
                             ulong3ds nrkeys, ulong3ds ntkeys);

void GetCameraMotionByName3ds(database3ds *db, 
			      const char3ds *name, 
			      kfcamera3ds **kfcam);

void GetCameraMotionByIndex3ds(database3ds *db, 
			       ulong3ds index,
			       kfcamera3ds **kfcam);

void PutCameraMotion3ds(database3ds *db, kfcamera3ds *kfcam);


void ReleaseCameraMotion3ds(kfcamera3ds **cam);

ulong3ds GetCameraNodeCount3ds(database3ds *db);

void GetCameraNodeNameList3ds(database3ds *db, namelist3ds **list);

void DeleteCameraMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteCameraMotionByName3ds(database3ds *db, char3ds *name);
void CopyCameraMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyCameraMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);


/* Ambient Light Animation Functions */
void InitAmbientLightMotion3ds(kfambient3ds **light, ulong3ds nckeys);
void ReleaseAmbientLightMotion3ds(kfambient3ds **light);
void GetAmbientLightMotion3ds(database3ds *db, kfambient3ds **kfambient);
void PutAmbientLightMotion3ds(database3ds *db, kfambient3ds *kfambient);
void DeleteAmbientLightMotion3ds(database3ds *db);
void CopyAmbientLightMotion3ds(database3ds *destdb, database3ds *srcdb);
void InitObjectMotion3ds(kfmesh3ds **obj, 
			 ulong3ds npkeys, 
			 ulong3ds nrkeys, 
			 ulong3ds nskeys, 
			 ulong3ds nmkeys,
			 ulong3ds nhkeys);

void ReleaseObjectMotion3ds(kfmesh3ds **obj);

void GetObjectNodeNameList3ds(database3ds *db, 
			      namelist3ds **list);

ulong3ds GetObjectNodeCount3ds(database3ds *db);

void GetObjectMotionByName3ds(database3ds *db, 
			      char3ds *name, 
			      kfmesh3ds **kfmesh);

void GetObjectMotionByIndex3ds(database3ds *db, 
			       ulong3ds index,
			       kfmesh3ds **kfmesh);

void PutObjectMotion3ds(database3ds *db,
			kfmesh3ds *kfmesh);

void DeleteObjectMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteObjectMotionByName3ds(database3ds *db, char3ds *name);
void CopyObjectMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyObjectMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void InitOmnilightMotion3ds(kfomni3ds **light, 
			    ulong3ds npkeys, 
			    ulong3ds nckeys);

void ReleaseOmnilightMotion3ds(kfomni3ds **light);

void GetOmnilightNodeNameList3ds(database3ds *db, 
				 namelist3ds **list);

void GetOmnilightMotionByName3ds(database3ds *db, 
				 char3ds *name, 
				 kfomni3ds **kfomni);

void GetOmnilightMotionByIndex3ds(database3ds *db, 
				  ulong3ds index,
				  kfomni3ds **kfomni);

ulong3ds GetOmnilightNodeCount3ds(database3ds *db);

void PutOmnilightMotion3ds(database3ds *db, 
			   kfomni3ds *kfomni);

void PutOmnilightMotion3ds(database3ds *db, 
			   kfomni3ds *kfomni);

void DeleteOmnilightMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteOmnilightMotionByName3ds(database3ds *db, char3ds *name);
void CopyOmnilightMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyOmnilightMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void InitSpotlightMotion3ds(kfspot3ds **spot, 
                ulong3ds npkeys, 
                ulong3ds nckeys, 
                ulong3ds nhkeys, 
                ulong3ds nfkeys, 
                ulong3ds nrkeys, 
                ulong3ds ntkeys);

void ReleaseSpotlightMotion3ds(kfspot3ds **spot);

void GetSpotlightNodeNameList3ds(database3ds *db, 
				 namelist3ds **list);

ulong3ds GetSpotlightNodeCount3ds(database3ds *db);

void GetSpotlightMotionByName3ds(database3ds *db, 
                 char3ds *name, 
                 kfspot3ds **kfspot);

void PutSpotlightMotion3ds(database3ds *db, 
			   kfspot3ds *kfspot);

void GetSpotlightMotionByIndex3ds(database3ds *db, 
		  ulong3ds index,
		  kfspot3ds **kfspot);

void DeleteSpotlightMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteSpotlightMotionByName3ds(database3ds *db, char3ds *name);
void CopySpotlightMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopySpotlightMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);

void FillMatrix3ds(mesh3ds *obj);
void SetBoundBox3ds(const mesh3ds *mobj, kfmesh3ds *kobj );
void DumpMapInfo3ds(FILE *out, mapinfo3ds *map, long3ds dumplevel);
void DumpMesh3ds(FILE *out, mesh3ds *mesh, long3ds dumplevel);
void DumpMaterial3ds(FILE *out, material3ds *mat, long3ds dumplevel);
void DumpBitmap3ds(FILE *out, bitmap3ds *map, long3ds dumplevel);
void SetDumpLevel3ds(long3ds newlevel);
void DumpDatabase3ds(FILE *outfile, database3ds *db);
void GetMeshXDataByName3ds(database3ds *db, char3ds *meshname, char3ds *appname, xdataraw3ds **data);
ulong3ds GetMeshXDataCount3ds(database3ds *db, char3ds *meshname);
#ifdef __cplusplus
}
#endif

#endif
