/**************************************************************************
 * 3D Studio File Toolkit for Release 3 
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


/* Various informational type utility functions */

#include "stdlib.h"
#include "string.h"
#include "3dstype.h"
#include "3dsprim.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsutil.h"

char3ds indentstring[] = "|  |  |  |  |  |  |  |  |  |  |  |  |  |  ";

char3ds *indent(short3ds level)
{
   long3ds offset;
   offset = strlen(indentstring)-(level*3);
   return(&(indentstring[offset]));
}

void ChunkHeaderReport3ds(FILE *outfile, chunk3ds *chunk, ushort3ds indentlevel)
{
   fprintf(outfile, "\n%sChunk %s (%0xH) \n%sLength is %d (%0xH) \n", indent(indentlevel),
	   ChunkTagToString3ds(chunk->tag), chunk->tag, indent(indentlevel), chunk->size, chunk->size);
}	

void PrintKeyHeader3ds(FILE *outfile, keyheader3ds *key, ushort3ds indentlevel)
{
   fprintf(outfile, "%sFrame %u", indent(indentlevel), key->time);
   if((key->rflags & KeyUsesTension3ds) == KeyUsesTension3ds) fprintf(outfile, ", Tens %.2f", key->tension);
   if((key->rflags & KeyUsesCont3ds) == KeyUsesCont3ds) fprintf(outfile, ", Cont %.2f", key->continuity);
   if((key->rflags & KeyUsesBias3ds) == KeyUsesBias3ds) fprintf(outfile, ", Bias %.2f", key->bias);
   if((key->rflags & KeyUsesEaseTo3ds) == KeyUsesEaseTo3ds) fprintf(outfile, ", Ease to %.2f", key->easeto);
   if((key->rflags & KeyUsesEaseFrom3ds) == KeyUsesEaseFrom3ds) fprintf(outfile, ", Ease from %.2f", key->easefrom);
   fprintf(outfile, "\n");
}

  
char3ds *ChunkTagToString3ds(chunktag3ds tag)
{
   switch(tag)
   {
   case NULL_CHUNK: return("NULL_CHUNK");
   case ChunkType: return("ChunkType");
   case ChunkUnique: return("ChunkUnique");
   case NotChunk: return("NotChunk");
   case Container: return("Container");
   case IsChunk: return("IsChunk");

       /* Dummy Chunk that sometimes appears in 3ds files created by
          prerelease 3D Studio R2 */
   case DUMMY: return("DUMMY");

       /* Trick Chunk Types For Open*, Write*, Close* functions */

   case POINT_ARRAY_ENTRY: return("POINT_ARRAY_ENTRY");
   case POINT_FLAG_ARRAY_ENTRY: return("POINT_FLAG_ARRAY_ENTRY");
   case FACE_ARRAY_ENTRY: return("FACE_ARRAY_ENTRY");
   case MSH_MAT_GROUP_ENTRY: return("MSH_MAT_GROUP_ENTRY");
   case TEX_VERTS_ENTRY: return("TEX_VERTS_ENTRY");
   case SMOOTH_GROUP_ENTRY: return("SMOOTH_GROUP_ENTRY");
   case POS_TRACK_TAG_KEY: return("POS_TRACK_TAG_KEY");
   case ROT_TRACK_TAG_KEY: return("ROT_TRACK_TAG_KEY");
   case SCL_TRACK_TAG_KEY: return("SCL_TRACK_TAG_KEY");
   case FOV_TRACK_TAG_KEY: return("FOV_TRACK_TAG_KEY");
   case ROLL_TRACK_TAG_KEY: return("ROLL_TRACK_TAG_KEY");
   case COL_TRACK_TAG_KEY: return("COL_TRACK_TAG_KEY");
   case MORPH_TRACK_TAG_KEY: return("MORPH_TRACK_TAG_KEY");
   case HOT_TRACK_TAG_KEY: return("HOT_TRACK_TAG_KEY");
   case FALL_TRACK_TAG_KEY: return("FALL_TRACK_TAG_KEY");

        /* 3DS File Chunk IDs */

   case M3DMAGIC: return("M3DMAGIC");
   case SMAGIC: return("SMAGIC");
   case LMAGIC: return("LMAGIC");
   case MLIBMAGIC: return("MLIBMAGIC");
   case MATMAGIC: return("MATMAGIC");
   case M3D_VERSION: return("M3D_VERSION");
   case M3D_KFVERSION: return("M3D_KFVERSION");

        /* Mesh Chunk Ids */

   case MDATA: return("MDATA");
   case MESH_VERSION: return("MESH_VERSION");
   case COLOR_F: return("COLOR_F");
   case COLOR_24: return("COLOR_24");
   case LIN_COLOR_24: return("LIN_COLOR_24");
   case LIN_COLOR_F: return("LIN_COLOR_F");
   case INT_PERCENTAGE: return("INT_PERCENTAGE");
   case FLOAT_PERCENTAGE: return("FLOAT_PERCENTAGE");

   case MASTER_SCALE: return("MASTER_SCALE");

   case BIT_MAP: return("BIT_MAP");
   case USE_BIT_MAP: return("USE_BIT_MAP");
   case SOLID_BGND: return("SOLID_BGND");
   case USE_SOLID_BGND: return("USE_SOLID_BGND");
   case V_GRADIENT: return("V_GRADIENT");
   case USE_V_GRADIENT: return("USE_V_GRADIENT");

   case LO_SHADOW_BIAS: return("LO_SHADOW_BIAS");
   case HI_SHADOW_BIAS: return("HI_SHADOW_BIAS");
   case SHADOW_MAP_SIZE: return("SHADOW_MAP_SIZE");
   case SHADOW_SAMPLES: return("SHADOW_SAMPLES");
   case SHADOW_RANGE: return("SHADOW_RANGE");
   case SHADOW_FILTER: return("SHADOW_FILTER");
   case RAY_BIAS: return("RAY_BIAS");
 
   case O_CONSTS: return("O_CONSTS");
 
   case AMBIENT_LIGHT: return("AMBIENT_LIGHT");

   case FOG: return("FOG");
   case USE_FOG: return("USE_FOG");
   case FOG_BGND: return("FOG_BGND");
   case DISTANCE_CUE: return("DISTANCE_CUE");
   case USE_DISTANCE_CUE: return("USE_DISTANCE_CUE");
   case LAYER_FOG: return("LAYER_FOG");
   case USE_LAYER_FOG: return("USE_LAYER_FOG");
   case DCUE_BGND: return("DCUE_BGND");

   case DEFAULT_VIEW: return("DEFAULT_VIEW");
   case VIEW_TOP: return("VIEW_TOP");
   case VIEW_BOTTOM: return("VIEW_BOTTOM");
   case VIEW_LEFT: return("VIEW_LEFT");
   case VIEW_RIGHT: return("VIEW_RIGHT");
   case VIEW_FRONT: return("VIEW_FRONT");
   case VIEW_BACK: return("VIEW_BACK");
   case VIEW_USER: return("VIEW_USER");
   case VIEW_CAMERA: return("VIEW_CAMERA");
   case VIEW_WINDOW: return("VIEW_WINDOW");

   case NAMED_OBJECT: return("NAMED_OBJECT");
   case OBJ_HIDDEN: return("OBJ_HIDDEN");
   case OBJ_VIS_LOFTER: return("OBJ_VIS_LOFTER");
   case OBJ_DOESNT_CAST: return("OBJ_DOESNT_CAST");
   case OBJ_MATTE: return("OBJ_MATTE");
   case OBJ_FAST: return("OBJ_FAST");
   case OBJ_PROCEDURAL: return("OBJ_PROCEDURAL");
   case OBJ_FROZEN: return("OBJ_FROZEN");
   case OBJ_DONT_RCVSHADOW: return("OBJ_DONT_RCVSHADOW");

   case N_TRI_OBJECT: return("N_TRI_OBJECT");

   case POINT_ARRAY: return("POINT_ARRAY");
   case POINT_FLAG_ARRAY: return("POINT_FLAG_ARRAY");
   case FACE_ARRAY: return("FACE_ARRAY");
   case MSH_MAT_GROUP: return("MSH_MAT_GROUP");
   case OLD_MAT_GROUP: return("OLD_MAT_GROUP");
   case TEX_VERTS: return("TEX_VERTS");
   case SMOOTH_GROUP: return("SMOOTH_GROUP");
   case MESH_MATRIX: return("MESH_MATRIX");
   case MESH_COLOR: return("MESH_COLOR");
   case MESH_TEXTURE_INFO: return("MESH_TEXTURE_INFO");
   case PROC_NAME: return("PROC_NAME");
   case PROC_DATA: return("PROC_DATA");
   case MSH_BOXMAP: return("MSH_BOXMAP");

   case N_D_L_OLD: return("N_D_L_OLD");

   case N_CAM_OLD: return("N_CAM_OLD");

   case N_DIRECT_LIGHT: return("N_DIRECT_LIGHT");
   case DL_SPOTLIGHT: return("DL_SPOTLIGHT");
   case DL_OFF: return("DL_OFF");
   case DL_ATTENUATE: return("DL_ATTENUATE");
   case DL_RAYSHAD: return("DL_RAYSHAD");
   case DL_SHADOWED: return("DL_SHADOWED");
   case DL_LOCAL_SHADOW: return("DL_LOCAL_SHADOW");
   case DL_LOCAL_SHADOW2: return("DL_LOCAL_SHADOW2");
   case DL_SEE_CONE: return("DL_SEE_CONE");
   case DL_SPOT_RECTANGULAR: return("DL_SPOT_RECTANGULAR");
   case DL_SPOT_OVERSHOOT: return("DL_SPOT_OVERSHOOT");
   case DL_SPOT_PROJECTOR: return("DL_SPOT_PROJECTOR");
   case DL_EXCLUDE: return("DL_EXCLUDE");
   case DL_RANGE: return("DL_RANGE");
   case DL_SPOT_ROLL: return("DL_SPOT_ROLL");
   case DL_SPOT_ASPECT: return("DL_SPOT_ASPECT");
   case DL_RAY_BIAS: return("DL_RAY_BIAS");
   case DL_INNER_RANGE: return("DL_INNER_RANGE");
   case DL_OUTER_RANGE: return("DL_OUTER_RANGE");
   case DL_MULTIPLIER: return("DL_MULTIPLIER");

   case N_AMBIENT_LIGHT: return("N_AMBIENT_LIGHT");

   case N_CAMERA: return("N_CAMERA");
   case CAM_SEE_CONE: return("CAM_SEE_CONE");
   case CAM_RANGES: return("CAM_RANGES");

   case HIERARCHY: return("HIERARCHY");
   case PARENT_OBJECT: return("PARENT_OBJECT");
   case PIVOT_OBJECT: return("PIVOT_OBJECT");
   case PIVOT_LIMITS: return("PIVOT_LIMITS");
   case PIVOT_ORDER: return("PIVOT_ORDER");
   case XLATE_RANGE: return("XLATE_RANGE");

   case POLY_2D: return("POLY_2D");

/* Flags in shaper file that tell whether polys make up an ok shape */

   case SHAPE_OK: return("SHAPE_OK");
   case SHAPE_NOT_OK: return("SHAPE_NOT_OK");

   case SHAPE_HOOK: return("SHAPE_HOOK");

   case PATH_3D: return("PATH_3D");
   case PATH_MATRIX: return("PATH_MATRIX");
   case SHAPE_2D: return("SHAPE_2D");
   case M_SCALE: return("M_SCALE");
   case M_TWIST: return("M_TWIST");
   case M_TEETER: return("M_TEETER");
   case M_FIT: return("M_FIT");
   case M_BEVEL: return("M_BEVEL");
   case XZ_CURVE: return("XZ_CURVE");
   case YZ_CURVE: return("YZ_CURVE");
   case INTERPCT: return("INTERPCT");
   case DEFORM_LIMIT: return("DEFORM_LIMIT");

/* Flags for Modeler options */

   case USE_CONTOUR: return("USE_CONTOUR");
   case USE_TWEEN: return("USE_TWEEN");
   case USE_SCALE: return("USE_SCALE");
   case USE_TWIST: return("USE_TWIST");
   case USE_TEETER: return("USE_TEETER");
   case USE_FIT: return("USE_FIT");
   case USE_BEVEL: return("USE_BEVEL");

   /* Viewport description chunks */

   case VIEWPORT_LAYOUT_OLD: return("VIEWPORT_LAYOUT_OLD");
   case VIEWPORT_DATA_OLD: return("VIEWPORT_DATA_OLD");
   case VIEWPORT_LAYOUT: return("VIEWPORT_LAYOUT");
   case VIEWPORT_DATA: return("VIEWPORT_DATA");
   case VIEWPORT_DATA_3: return("VIEWPORT_DATA_3");
   case VIEWPORT_SIZE: return("VIEWPORT_SIZE");
   case NETWORK_VIEW: return("NETWORK_VIEW");

   /* External Application Data */

   case XDATA_SECTION: return("XDATA_SECTION");
   case XDATA_ENTRY: return("XDATA_ENTRY");
   case XDATA_APPNAME: return("XDATA_APPNAME");
   case XDATA_STRING: return("XDATA_STRING");
   case XDATA_FLOAT: return("XDATA_FLOAT");
   case XDATA_DOUBLE: return("XDATA_DOUBLE");
   case XDATA_SHORT: return("XDATA_SHORT");
   case XDATA_LONG: return("XDATA_LONG");
   case XDATA_VOID: return("XDATA_VOID");
   case XDATA_GROUP: return("XDATA_GROUP");
   case XDATA_RFU6: return("XDATA_RFU6");
   case XDATA_RFU5: return("XDATA_RFU5");
   case XDATA_RFU4: return("XDATA_RFU4");
   case XDATA_RFU3: return("XDATA_RFU3");
   case XDATA_RFU2: return("XDATA_RFU2");
   case XDATA_RFU1: return("XDATA_RFU1");

        /* Material Chunk IDs */

   case MAT_ENTRY: return("MAT_ENTRY");
   case MAT_NAME: return("MAT_NAME");
   case MAT_AMBIENT: return("MAT_AMBIENT");
   case MAT_DIFFUSE: return("MAT_DIFFUSE");
   case MAT_SPECULAR: return("MAT_SPECULAR");
   case MAT_SHININESS: return("MAT_SHININESS");
   case MAT_SHIN2PCT: return("MAT_SHIN2PCT");
   case MAT_SHIN3PCT: return("MAT_SHIN3PCT");
   case MAT_TRANSPARENCY: return("MAT_TRANSPARENCY");
   case MAT_XPFALL: return("MAT_XPFALL");
   case MAT_REFBLUR: return("MAT_REFBLUR");

   case MAT_SELF_ILLUM: return("MAT_SELF_ILLUM");
   case MAT_TWO_SIDE: return("MAT_TWO_SIDE");
   case MAT_DECAL: return("MAT_DECAL");
   case MAT_ADDITIVE: return("MAT_ADDITIVE");
   case MAT_SELF_ILPCT: return("MAT_SELF_ILPCT");
   case MAT_WIRE: return("MAT_WIRE");
   case MAT_SUPERSMP: return("MAT_SUPERSMP");
   case MAT_WIRESIZE: return("MAT_WIRESIZE");
   case MAT_FACEMAP: return("MAT_FACEMAP");
   case MAT_XPFALLIN: return("MAT_XPFALLIN");
   case MAT_PHONGSOFT: return("MAT_PHONGSOFT");
   case MAT_WIREABS: return("MAT_WIREABS");

   case MAT_SHADING: return("MAT_SHADING");

   case MAT_TEXMAP: return("MAT_TEXMAP");
   case MAT_OPACMAP: return("MAT_OPACMAP");
   case MAT_REFLMAP: return("MAT_REFLMAP");
   case MAT_BUMPMAP: return("MAT_BUMPMAP");
   case MAT_SPECMAP: return("MAT_SPECMAP");
   case MAT_USE_XPFALL: return("MAT_USE_XPFALL");
   case MAT_USE_REFBLUR: return("MAT_USE_REFBLUR");
   case MAT_BUMP_PERCENT: return("MAT_BUMP_PERCENT");

   case MAT_MAPNAME: return("MAT_MAPNAME");
   case MAT_ACUBIC: return("MAT_ACUBIC");

   case MAT_SXP_TEXT_DATA: return("MAT_SXP_TEXT_DATA");
   case MAT_SXP_TEXT2_DATA: return("MAT_SXP_TEXT2_DATA");
   case MAT_SXP_OPAC_DATA: return("MAT_SXP_OPAC_DATA");
   case MAT_SXP_BUMP_DATA: return("MAT_SXP_BUMP_DATA");
   case MAT_SXP_SPEC_DATA: return("MAT_SXP_SPEC_DATA");
   case MAT_SXP_SHIN_DATA: return("MAT_SXP_SHIN_DATA");
   case MAT_SXP_SELFI_DATA: return("MAT_SXP_SELFI_DATA");
   case MAT_SXP_TEXT_MASKDATA: return("MAT_SXP_TEXT_MASKDATA");
   case MAT_SXP_TEXT2_MASKDATA: return("MAT_SXP_TEXT2_MASKDATA");
   case MAT_SXP_OPAC_MASKDATA: return("MAT_SXP_OPAC_MASKDATA");
   case MAT_SXP_BUMP_MASKDATA: return("MAT_SXP_BUMP_MASKDATA");
   case MAT_SXP_SPEC_MASKDATA: return("MAT_SXP_SPEC_MASKDATA");
   case MAT_SXP_SHIN_MASKDATA: return("MAT_SXP_SHIN_MASKDATA");
   case MAT_SXP_SELFI_MASKDATA: return("MAT_SXP_SELFI_MASKDATA");
   case MAT_SXP_REFL_MASKDATA: return("MAT_SXP_REFL_MASKDATA");
   case MAT_TEX2MAP: return("MAT_TEX2MAP");
   case MAT_SHINMAP: return("MAT_SHINMAP");
   case MAT_SELFIMAP: return("MAT_SELFIMAP");
   case MAT_TEXMASK: return("MAT_TEXMASK");
   case MAT_TEX2MASK: return("MAT_TEX2MASK");
   case MAT_OPACMASK: return("MAT_OPACMASK");
   case MAT_BUMPMASK: return("MAT_BUMPMASK");
   case MAT_SHINMASK: return("MAT_SHINMASK");
   case MAT_SPECMASK: return("MAT_SPECMASK");
   case MAT_SELFIMASK: return("MAT_SELFIMASK");
   case MAT_REFLMASK: return("MAT_REFLMASK");
   case MAT_MAP_TILINGOLD: return("MAT_MAP_TILINGOLD");
   case MAT_MAP_TILING: return("MAT_MAP_TILING");
   case MAT_MAP_TEXBLUR_OLD: return("MAT_MAP_TEXBLUR_OLD");
   case MAT_MAP_TEXBLUR: return("MAT_MAP_TEXBLUR");
   case MAT_MAP_USCALE: return("MAT_MAP_USCALE");
   case MAT_MAP_VSCALE: return("MAT_MAP_VSCALE");
   case MAT_MAP_UOFFSET: return("MAT_MAP_UOFFSET");
   case MAT_MAP_VOFFSET: return("MAT_MAP_VOFFSET");
   case MAT_MAP_ANG: return("MAT_MAP_ANG");
   case MAT_MAP_COL1: return("MAT_MAP_COL1");
   case MAT_MAP_COL2: return("MAT_MAP_COL2");
   case MAT_MAP_RCOL: return("MAT_MAP_RCOL");
   case MAT_MAP_GCOL: return("MAT_MAP_GCOL");
   case MAT_MAP_BCOL: return("MAT_MAP_BCOL");

        /* Keyframe Chunk IDs */

   case KFDATA: return("KFDATA");
   case KFHDR: return("KFHDR");
   case AMBIENT_NODE_TAG: return("AMBIENT_NODE_TAG");
   case OBJECT_NODE_TAG: return("OBJECT_NODE_TAG");
   case CAMERA_NODE_TAG: return("CAMERA_NODE_TAG");
   case TARGET_NODE_TAG: return("TARGET_NODE_TAG");
   case LIGHT_NODE_TAG: return("LIGHT_NODE_TAG");
   case L_TARGET_NODE_TAG: return("L_TARGET_NODE_TAG");
   case SPOTLIGHT_NODE_TAG: return("SPOTLIGHT_NODE_TAG");

   case KFSEG: return("KFSEG");
   case KFCURTIME: return("KFCURTIME");
   case NODE_HDR: return("NODE_HDR");
   case PARENT_NAME: return("PARENT_NAME");
   case INSTANCE_NAME: return("INSTANCE_NAME");
   case PRESCALE: return("PRESCALE");
   case PIVOT: return("PIVOT");
   case BOUNDBOX: return("BOUNDBOX");
   case MORPH_SMOOTH: return("MORPH_SMOOTH");
   case POS_TRACK_TAG: return("POS_TRACK_TAG");
   case ROT_TRACK_TAG: return("ROT_TRACK_TAG");
   case SCL_TRACK_TAG: return("SCL_TRACK_TAG");
   case FOV_TRACK_TAG: return("FOV_TRACK_TAG");
   case ROLL_TRACK_TAG: return("ROLL_TRACK_TAG");
   case COL_TRACK_TAG: return("COL_TRACK_TAG");
   case MORPH_TRACK_TAG: return("MORPH_TRACK_TAG");
   case HOT_TRACK_TAG: return("HOT_TRACK_TAG");
   case FALL_TRACK_TAG: return("FALL_TRACK_TAG");
   case HIDE_TRACK_TAG: return("HIDE_TRACK_TAG");
   case NODE_ID: return("NODE_ID");
      
   case CMAGIC: return("CMAGIC");

   case C_MDRAWER: return("C_MDRAWER");
   case C_TDRAWER: return("C_TDRAWER");
   case C_SHPDRAWER: return("C_SHPDRAWER");
   case C_MODDRAWER: return("C_MODDRAWER");
   case C_RIPDRAWER: return("C_RIPDRAWER");
   case C_TXDRAWER: return("C_TXDRAWER");
   case C_PDRAWER: return("C_PDRAWER");
   case C_MTLDRAWER: return("C_MTLDRAWER");
   case C_FLIDRAWER: return("C_FLIDRAWER");
   case C_CUBDRAWER: return("C_CUBDRAWER");
   case C_MFILE: return("C_MFILE");
   case C_SHPFILE: return("C_SHPFILE");
   case C_MODFILE: return("C_MODFILE");
   case C_RIPFILE: return("C_RIPFILE");
   case C_TXFILE: return("C_TXFILE");
   case C_PFILE: return("C_PFILE");
   case C_MTLFILE: return("C_MTLFILE");
   case C_FLIFILE: return("C_FLIFILE");
   case C_PALFILE: return("C_PALFILE");
   case C_TX_STRING: return("C_TX_STRING");
   case C_CONSTS: return("C_CONSTS");
   case C_SNAPS: return("C_SNAPS");
   case C_GRIDS: return("C_GRIDS");
   case C_ASNAPS: return("C_ASNAPS");
   case C_GRID_RANGE: return("C_GRID_RANGE");
   case C_RENDTYPE: return("C_RENDTYPE");
   case C_PROGMODE: return("C_PROGMODE");
   case C_PREVMODE: return("C_PREVMODE");
   case C_MODWMODE: return("C_MODWMODE");
   case C_MODMODEL: return("C_MODMODEL");
   case C_ALL_LINES: return("C_ALL_LINES");
   case C_BACK_TYPE: return("C_BACK_TYPE");
   case C_MD_CS: return("C_MD_CS");
   case C_MD_CE: return("C_MD_CE");
   case C_MD_SML: return("C_MD_SML");
   case C_MD_SMW: return("C_MD_SMW");
   case C_LOFT_WITH_TEXTURE: return("C_LOFT_WITH_TEXTURE");
   case C_LOFT_L_REPEAT: return("C_LOFT_L_REPEAT");
   case C_LOFT_W_REPEAT: return("C_LOFT_W_REPEAT");
   case C_LOFT_UV_NORMALIZE: return("C_LOFT_UV_NORMALIZE");
   case C_WELD_LOFT: return("C_WELD_LOFT");
   case C_MD_PDET: return("C_MD_PDET");
   case C_MD_SDET: return("C_MD_SDET");
   case C_RGB_RMODE: return("C_RGB_RMODE");
   case C_RGB_HIDE: return("C_RGB_HIDE");
   case C_RGB_MAPSW: return("C_RGB_MAPSW");
   case C_RGB_TWOSIDE: return("C_RGB_TWOSIDE");
   case C_RGB_SHADOW: return("C_RGB_SHADOW");
   case C_RGB_AA: return("C_RGB_AA");
   case C_RGB_OVW: return("C_RGB_OVW");
   case C_RGB_OVH: return("C_RGB_OVH");
   case C_RGB_PICTYPE: return("C_RGB_PICTYPE");
   case C_RGB_OUTPUT: return("C_RGB_OUTPUT");
   case C_RGB_TODISK: return("C_RGB_TODISK");
   case C_RGB_COMPRESS: return("C_RGB_COMPRESS");
   case C_JPEG_COMPRESSION: return("C_JPEG_COMPRESSION");
   case C_RGB_DISPDEV: return("C_RGB_DISPDEV");
   case C_RGB_HARDDEV: return("C_RGB_HARDDEV");
   case C_RGB_PATH: return("C_RGB_PATH");
   case C_BITMAP_DRAWER: return("C_BITMAP_DRAWER");
   case C_RGB_FILE: return("C_RGB_FILE");
   case C_RGB_OVASPECT: return("C_RGB_OVASPECT");

   case C_RGB_ANIMTYPE: return("C_RGB_ANIMTYPE");
   case C_RENDER_ALL: return("C_RENDER_ALL");
   case C_REND_FROM: return("C_REND_FROM");
   case C_REND_TO: return("C_REND_TO");
   case C_REND_NTH: return("C_REND_NTH");
   case C_REND_TSTEP: return("C_REND_TSTEP"); 				      
   case C_VP_TSTEP: return("C_VP_TSTEP"); 				      

   case C_PAL_TYPE: return("C_PAL_TYPE");
   case C_RND_TURBO: return("C_RND_TURBO");
   case C_RND_MIP: return("C_RND_MIP");
   case C_BGND_METHOD: return("C_BGND_METHOD");
   case C_AUTO_REFLECT: return("C_AUTO_REFLECT");
   case C_VP_FROM: return("C_VP_FROM");
   case C_VP_TO: return("C_VP_TO");
   case C_VP_NTH: return("C_VP_NTH");

   case C_SRDIAM: return("C_SRDIAM");
   case C_SRDEG: return("C_SRDEG");
   case C_SRSEG: return("C_SRSEG");
   case C_SRDIR: return("C_SRDIR");
   case C_HETOP: return("C_HETOP");
   case C_HEBOT: return("C_HEBOT");
   case C_HEHT: return("C_HEHT");
   case C_HETURNS: return("C_HETURNS");
   case C_HEDEG: return("C_HEDEG");
   case C_HESEG: return("C_HESEG");
   case C_HEDIR: return("C_HEDIR");
   case C_QUIKSTUFF: return("C_QUIKSTUFF");
   case C_SEE_LIGHTS: return("C_SEE_LIGHTS");
   case C_SEE_CAMERAS: return("C_SEE_CAMERAS");
   case C_SEE_3D: return("C_SEE_3D");
   case C_MESHSEL: return("C_MESHSEL");
   case C_MESHUNSEL: return("C_MESHUNSEL");
   case C_POLYSEL: return("C_POLYSEL");
   case C_POLYUNSEL: return("C_POLYUNSEL");
   case C_SHPLOCAL: return("C_SHPLOCAL");
   case C_MSHLOCAL: return("C_MSHLOCAL");
   case C_NUM_FORMAT: return("C_NUM_FORMAT");
   case C_ARCH_DENOM: return("C_ARCH_DENOM");
   case C_IN_DEVICE: return("C_IN_DEVICE");
   case C_MSCALE: return("C_MSCALE");
   case C_COMM_PORT: return("C_COMM_PORT");
   case C_TAB_BASES: return("C_TAB_BASES");
   case C_TAB_DIVS: return("C_TAB_DIVS");
   case C_MASTER_SCALES: return("C_MASTER_SCALES");
   case C_SHOW_1STVERT: return("C_SHOW_1STVERT");
   case C_SHAPER_OK: return("C_SHAPER_OK");
   case C_LOFTER_OK: return("C_LOFTER_OK");
   case C_EDITOR_OK: return("C_EDITOR_OK");
   case C_KEYFRAMER_OK: return("C_KEYFRAMER_OK");
   case C_PICKSIZE: return("C_PICKSIZE");
   case C_MAPTYPE: return("C_MAPTYPE");
   case C_MAP_DISPLAY: return("C_MAP_DISPLAY");
   case C_TILE_XY: return("C_TILE_XY");
   case C_MAP_XYZ: return("C_MAP_XYZ");
   case C_MAP_SCALE: return("C_MAP_SCALE");
   case C_MAP_MATRIX_OLD: return("C_MAP_MATRIX_OLD");
   case C_MAP_MATRIX: return("C_MAP_MATRIX");
   case C_MAP_WID_HT: return("C_MAP_WID_HT");
   case C_OBNAME: return("C_OBNAME");
   case C_CAMNAME: return("C_CAMNAME");
   case C_LTNAME: return("C_LTNAME");
   case C_CUR_MNAME: return("C_CUR_MNAME");
   case C_CURMTL_FROM_MESH: return("C_CURMTL_FROM_MESH");
   case C_GET_SHAPE_MAKE_FACES: return("C_GET_SHAPE_MAKE_FACES");
   case C_DETAIL: return("C_DETAIL");
   case C_VERTMARK: return("C_VERTMARK");
   case C_MSHAX: return("C_MSHAX");
   case C_MSHCP: return("C_MSHCP");
   case C_USERAX: return("C_USERAX");
   case C_SHOOK: return("C_SHOOK");
   case C_RAX: return("C_RAX");
   case C_STAPE: return("C_STAPE");
   case C_LTAPE: return("C_LTAPE");
   case C_ETAPE: return("C_ETAPE");
   case C_KTAPE: return("C_KTAPE");
   case C_SPHSEGS: return("C_SPHSEGS");
   case C_GEOSMOOTH: return("C_GEOSMOOTH");
   case C_HEMISEGS: return("C_HEMISEGS");
   case C_PRISMSEGS: return("C_PRISMSEGS");
   case C_PRISMSIDES: return("C_PRISMSIDES");
   case C_TUBESEGS: return("C_TUBESEGS");
   case C_TUBESIDES: return("C_TUBESIDES");
   case C_TORSEGS: return("C_TORSEGS");
   case C_TORSIDES: return("C_TORSIDES");
   case C_CONESIDES: return("C_CONESIDES");
   case C_CONESEGS: return("C_CONESEGS");
   case C_NGPARMS: return("C_NGPARMS");
   case C_PTHLEVEL: return("C_PTHLEVEL");
   case C_MSCSYM: return("C_MSCSYM");
   case C_MFTSYM: return("C_MFTSYM");
   case C_MTTSYM: return("C_MTTSYM");
   case C_SMOOTHING: return("C_SMOOTHING");
   case C_MODICOUNT: return("C_MODICOUNT");
   case C_FONTSEL: return("C_FONTSEL");
   case C_TESS_TYPE: return("C_TESS_TYPE");
   case C_TESS_TENSION: return("C_TESS_TENSION");

   case C_SEG_START: return("C_SEG_START");
   case C_SEG_END: return("C_SEG_END");
   case C_CURTIME: return("C_CURTIME");
   case C_ANIMLENGTH: return("C_ANIMLENGTH");
   case C_PV_FROM: return("C_PV_FROM");
   case C_PV_TO: return("C_PV_TO");
   case C_PV_DOFNUM: return("C_PV_DOFNUM");
   case C_PV_RNG: return("C_PV_RNG");
   case C_PV_NTH: return("C_PV_NTH");
   case C_PV_TYPE: return("C_PV_TYPE");
   case C_PV_METHOD: return("C_PV_METHOD");
   case C_PV_FPS: return("C_PV_FPS");
   case C_VTR_FRAMES: return("C_VTR_FRAMES");
   case C_VTR_HDTL: return("C_VTR_HDTL");
   case C_VTR_HD: return("C_VTR_HD");
   case C_VTR_TL: return("C_VTR_TL");
   case C_VTR_IN: return("C_VTR_IN");
   case C_VTR_PK: return("C_VTR_PK");
   case C_VTR_SH: return("C_VTR_SH");

/* Material chunks */

   case C_WORK_MTLS: return("C_WORK_MTLS");
   case C_WORK_MTLS_2: return("C_WORK_MTLS_2");
   case C_WORK_MTLS_3: return("C_WORK_MTLS_3");
   case C_WORK_MTLS_4: return("C_WORK_MTLS_4");
   case C_WORK_MTLS_5: return("C_WORK_MTLS_5");
   case C_WORK_MTLS_6: return("C_WORK_MTLS_6");
   case C_WORK_MTLS_7: return("C_WORK_MTLS_7");
   case C_WORK_MTLS_8: return("C_WORK_MTLS_8");
   case C_WORKMTL: return("C_WORKMTL");
   case C_SXP_TEXT_DATA: return("C_SXP_TEXT_DATA");
   case C_SXP_TEXT2_DATA: return("C_SXP_TEXT2_DATA");
   case C_SXP_OPAC_DATA: return("C_SXP_OPAC_DATA");
   case C_SXP_BUMP_DATA: return("C_SXP_BUMP_DATA");
   case C_SXP_SPEC_DATA: return("C_SXP_SPEC_DATA");
   case C_SXP_SHIN_DATA: return("C_SXP_SHIN_DATA");
   case C_SXP_SELFI_DATA: return("C_SXP_SELFI_DATA");
   case C_SXP_TEXT_MASKDATA: return("C_SXP_TEXT_MASKDATA");
   case C_SXP_TEXT2_MASKDATA: return("C_SXP_TEXT2_MASKDATA");
   case C_SXP_OPAC_MASKDATA: return("C_SXP_OPAC_MASKDATA");
   case C_SXP_BUMP_MASKDATA: return("C_SXP_BUMP_MASKDATA");
   case C_SXP_SPEC_MASKDATA: return("C_SXP_SPEC_MASKDATA");
   case C_SXP_SHIN_MASKDATA: return("C_SXP_SHIN_MASKDATA");
   case C_SXP_SELFI_MASKDATA: return("C_SXP_SELFI_MASKDATA");
   case C_SXP_REFL_MASKDATA: return("C_SXP_REFL_MASKDATA");

   case C_BGTYPE: return("C_BGTYPE");
   case C_MEDTILE: return("C_MEDTILE");

/* Contrast */

   case C_LO_CONTRAST: return("C_LO_CONTRAST");
   case C_HI_CONTRAST: return("C_HI_CONTRAST");

/* 3d frozen display */

   case C_FROZ_DISPLAY: return("C_FROZ_DISPLAY");

/* Booleans */
   case C_BOOLWELD: return("C_BOOLWELD");
   case C_BOOLTYPE: return("C_BOOLTYPE");

   case C_ANG_THRESH: return("C_ANG_THRESH");
   case C_SS_THRESH: return("C_SS_THRESH");
   case C_TEXTURE_BLUR_DEFAULT: return("C_TEXTURE_BLUR_DEFAULT");

   case C_MAPDRAWER: return("C_MAPDRAWER");
   case C_MAPDRAWER1: return("C_MAPDRAWER1");
   case C_MAPDRAWER2: return("C_MAPDRAWER2");
   case C_MAPDRAWER3: return("C_MAPDRAWER3");
   case C_MAPDRAWER4: return("C_MAPDRAWER4");
   case C_MAPDRAWER5: return("C_MAPDRAWER5");
   case C_MAPDRAWER6: return("C_MAPDRAWER6");
   case C_MAPDRAWER7: return("C_MAPDRAWER7");
   case C_MAPDRAWER8: return("C_MAPDRAWER8");
   case C_MAPDRAWER9: return("C_MAPDRAWER9");
   case C_MAPDRAWER_ENTRY: return("C_MAPDRAWER_ENTRY");

/* system options */
   case C_BACKUP_FILE: return("C_BACKUP_FILE");
   case C_DITHER_256: return("C_DITHER_256");
   case C_SAVE_LAST: return("C_SAVE_LAST");
   case C_USE_ALPHA: return("C_USE_ALPHA");
   case C_TGA_DEPTH: return("C_TGA_DEPTH");
   case C_REND_FIELDS: return("C_REND_FIELDS");
   case C_REFLIP: return("C_REFLIP");
   case C_SEL_ITEMTOG: return("C_SEL_ITEMTOG");
   case C_SEL_RESET: return("C_SEL_RESET");
   case C_STICKY_KEYINF: return("C_STICKY_KEYINF");
   case C_WELD_THRESHOLD: return("C_WELD_THRESHOLD");
   case C_ZCLIP_POINT: return("C_ZCLIP_POINT");
   case C_ALPHA_SPLIT: return("C_ALPHA_SPLIT");
   case C_KF_SHOW_BACKFACE: return("C_KF_SHOW_BACKFACE");
   case C_OPTIMIZE_LOFT: return("C_OPTIMIZE_LOFT");
   case C_TENS_DEFAULT: return("C_TENS_DEFAULT");
   case C_CONT_DEFAULT: return("C_CONT_DEFAULT");
   case C_BIAS_DEFAULT: return("C_BIAS_DEFAULT");

   case C_DXFNAME_SRC : return("C_DXFNAME_SRC ");
   case C_AUTO_WELD : return("C_AUTO_WELD ");
   case C_AUTO_UNIFY : return("C_AUTO_UNIFY ");
   case C_AUTO_SMOOTH : return("C_AUTO_SMOOTH ");
   case C_DXF_SMOOTH_ANG : return("C_DXF_SMOOTH_ANG ");
   case C_SMOOTH_ANG : return("C_SMOOTH_ANG ");

/* Special network-use chunks */

   case C_NET_USE_VPOST: return("C_NET_USE_VPOST");
   case C_NET_USE_GAMMA: return("C_NET_USE_GAMMA");
   case C_NET_FIELD_ORDER: return("C_NET_FIELD_ORDER");

   case C_BLUR_FRAMES: return("C_BLUR_FRAMES");
   case C_BLUR_SAMPLES: return("C_BLUR_SAMPLES");
   case C_BLUR_DUR: return("C_BLUR_DUR");
   case C_HOT_METHOD: return("C_HOT_METHOD");
   case C_HOT_CHECK: return("C_HOT_CHECK");
   case C_PIXEL_SIZE: return("C_PIXEL_SIZE");
   case C_DISP_GAMMA: return("C_DISP_GAMMA");
   case C_FBUF_GAMMA: return("C_FBUF_GAMMA");
   case C_FILE_OUT_GAMMA: return("C_FILE_OUT_GAMMA");
   case C_FILE_IN_GAMMA: return("C_FILE_IN_GAMMA");
   case C_GAMMA_CORRECT: return("C_GAMMA_CORRECT");
   case C_APPLY_DISP_GAMMA: return("C_APPLY_DISP_GAMMA");
   case C_APPLY_FBUF_GAMMA: return("C_APPLY_FBUF_GAMMA");
   case C_APPLY_FILE_GAMMA: return("C_APPLY_FILE_GAMMA");
   case C_FORCE_WIRE: return("C_FORCE_WIRE");
   case C_RAY_SHADOWS: return("C_RAY_SHADOWS");
   case C_MASTER_AMBIENT: return("C_MASTER_AMBIENT");
   case C_SUPER_SAMPLE: return("C_SUPER_SAMPLE");
   case C_OBJECT_MBLUR: return("C_OBJECT_MBLUR");
   case C_MBLUR_DITHER: return("C_MBLUR_DITHER");
   case C_DITHER_24: return("C_DITHER_24");
   case C_SUPER_BLACK: return("C_SUPER_BLACK");
   case C_SAFE_FRAME: return("C_SAFE_FRAME");
   case C_VIEW_PRES_RATIO: return("C_VIEW_PRES_RATIO");
   case C_BGND_PRES_RATIO: return("C_BGND_PRES_RATIO");
   case C_NTH_SERIAL_NUM: return("C_NTH_SERIAL_NUM");

   case VPDATA: return("VPDATA");

   case P_QUEUE_ENTRY: return("P_QUEUE_ENTRY");
   case P_QUEUE_IMAGE: return("P_QUEUE_IMAGE");
   case P_QUEUE_USEIGAMMA: return("P_QUEUE_USEIGAMMA");
   case P_QUEUE_PROC: return("P_QUEUE_PROC");
   case P_QUEUE_SOLID: return("P_QUEUE_SOLID");
   case P_QUEUE_GRADIENT: return("P_QUEUE_GRADIENT");
   case P_QUEUE_KF: return("P_QUEUE_KF");
   case P_QUEUE_MOTBLUR: return("P_QUEUE_MOTBLUR");
   case P_QUEUE_MB_REPEAT: return("P_QUEUE_MB_REPEAT");
   case P_QUEUE_NONE: return("P_QUEUE_NONE");

   case P_QUEUE_RESIZE: return("P_QUEUE_RESIZE");
   case P_QUEUE_OFFSET: return("P_QUEUE_OFFSET");
   case P_QUEUE_ALIGN: return("P_QUEUE_ALIGN");

   case P_CUSTOM_SIZE: return("P_CUSTOM_SIZE");

   case P_ALPH_NONE: return("P_ALPH_NONE");
   case P_ALPH_PSEUDO: return("P_ALPH_PSEUDO");
   case P_ALPH_OP_PSEUDO: return("P_ALPH_OP_PSEUDO");
   case P_ALPH_BLUR: return("P_ALPH_BLUR");
   case P_ALPH_PCOL: return("P_ALPH_PCOL");
   case P_ALPH_C0: return("P_ALPH_C0");
   case P_ALPH_OP_KEY: return("P_ALPH_OP_KEY");
   case P_ALPH_KCOL: return("P_ALPH_KCOL");
   case P_ALPH_OP_NOCONV: return("P_ALPH_OP_NOCONV");
   case P_ALPH_IMAGE: return("P_ALPH_IMAGE");
   case P_ALPH_ALPHA: return("P_ALPH_ALPHA");
   case P_ALPH_QUES: return("P_ALPH_QUES");
   case P_ALPH_QUEIMG: return("P_ALPH_QUEIMG");
   case P_ALPH_CUTOFF: return("P_ALPH_CUTOFF");
   case P_ALPHANEG: return("P_ALPHANEG");

   case P_TRAN_NONE: return("P_TRAN_NONE");
   case P_TRAN_IMAGE: return("P_TRAN_IMAGE");
   case P_TRAN_FRAMES: return("P_TRAN_FRAMES");
   case P_TRAN_FADEIN: return("P_TRAN_FADEIN");
   case P_TRAN_FADEOUT: return("P_TRAN_FADEOUT");
   case P_TRANNEG: return("P_TRANNEG");

   case P_RANGES: return("P_RANGES");

   case P_PROC_DATA: return("P_PROC_DATA");
      
   default 			   : return("UNKNOWN_CHUNK");
   } 			     
} 			     

