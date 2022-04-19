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


#include <stdlib.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "3dserr.h"
#include "3dsfile.h"
#include "chunkinf.h"
#include "3dsftkst.h"
#include "3dsrange.h"
#include "seekchil.h"
#include "chunk3ds.h"

/* Function skips to next chunk on disk by seeking the next file position */

void SeekChild3ds(chunk3ds *chunk)
{
   long3ds offset=0;
   char3ds dummy[50];

   switch(chunk->tag)
   {
   case M3DMAGIC:
   case SMAGIC:
   case LMAGIC:
   case MATMAGIC:
   case MLIBMAGIC:
   case MDATA:
   case AMBIENT_LIGHT:
   case SOLID_BGND:
   case DEFAULT_VIEW:
   case MAT_ENTRY:
   case MAT_AMBIENT:
   case MAT_DIFFUSE:
   case MAT_SPECULAR:
   case MAT_SHININESS:
   case MAT_SHIN2PCT:
	case MAT_SHIN3PCT:
   case MAT_TRANSPARENCY:
   case MAT_XPFALL:
   case MAT_REFBLUR:
   case MAT_SELF_ILPCT:   
   case MAT_TEXMAP:
   case MAT_TEXMASK:
   case MAT_TEX2MAP:
   case MAT_TEX2MASK:   
   case MAT_OPACMAP:
   case MAT_OPACMASK:   
   case MAT_REFLMAP:
   case MAT_REFLMASK:   
   case MAT_BUMPMAP:
   case MAT_BUMPMASK:
   case MAT_SPECMAP:
   case MAT_SPECMASK:
   case MAT_SHINMAP:
   case MAT_SHINMASK:
   case MAT_SELFIMAP:
   case MAT_SELFIMASK:
   case N_TRI_OBJECT:
   case XDATA_SECTION:
   case XDATA_ENTRY: 
   case KFDATA:
   case OBJECT_NODE_TAG:
   case CAMERA_NODE_TAG:
   case TARGET_NODE_TAG:
   case LIGHT_NODE_TAG:
   case SPOTLIGHT_NODE_TAG:
   case L_TARGET_NODE_TAG:
   case AMBIENT_NODE_TAG:
   case CMAGIC:
      break;
   case M3D_VERSION:
      offset = LongSize3ds;
      break;
   case COLOR_F:
      offset = 3*(FloatSize3ds);
      break;
   case COLOR_24:
      offset = 3*(ByteSize3ds);
      break;
   case INT_PERCENTAGE:
      offset = ShortSize3ds;
      break;
   case FLOAT_PERCENTAGE:
      offset = FloatSize3ds;
      break;
   case MAT_MAPNAME:
      ReadString3ds(dummy, MatMapNameStrMax3ds + 1);
      break;
   case MESH_VERSION:
      offset = LongSize3ds;
      break;
   case MASTER_SCALE:
      offset = FloatSize3ds;
      break;
   case LO_SHADOW_BIAS:
      offset = FloatSize3ds;
      break;
   case HI_SHADOW_BIAS:
      offset = FloatSize3ds;
      break;
   case SHADOW_MAP_SIZE:
      offset = ShortSize3ds;
      break;
   case SHADOW_SAMPLES:
      offset = ShortSize3ds;
      break;
   case O_CONSTS:
      offset = PointSize3ds;
      break;
   case V_GRADIENT:
      offset = FloatSize3ds;
      break;
   case NAMED_OBJECT:
      ReadString3ds(dummy, NamedObjectStrMax3ds + 1);
      break;
   case BIT_MAP:
      ReadString3ds(dummy, BitMapStrMax3ds + 1);
      break;
   case FOG:
      offset = 4*(FloatSize3ds);
      break;
   case LAYER_FOG:
      offset = 3*(FloatSize3ds)+LongSize3ds;
      break;
   case DISTANCE_CUE:
      offset = 4*(FloatSize3ds);
      break;
   case N_DIRECT_LIGHT:
      offset = PointSize3ds;
      break;
   case DL_SPOTLIGHT:
      offset = PointSize3ds + 2*(FloatSize3ds);
      break;
   case N_CAMERA:
      offset = 2*(PointSize3ds) + 2*(FloatSize3ds);
      break;
   case VIEWPORT_LAYOUT:
      offset = 7*ShortSize3ds;
      break;
   case VIEW_TOP:
   case VIEW_BOTTOM:
   case VIEW_LEFT:
   case VIEW_RIGHT:
   case VIEW_FRONT:
   case VIEW_BACK:
      offset = PointSize3ds + FloatSize3ds;
      break;
   case VIEW_USER:
      offset = PointSize3ds + 4*(FloatSize3ds);
      break;
   case VIEW_CAMERA:
      ReadString3ds(dummy, ViewCameraStrMax3ds + 1);
      break;
   case MAT_NAME:
      ReadString3ds(dummy, MatNameStrMax3ds + 1);
      break;
   case MAT_ACUBIC:
      offset = 2*(ByteSize3ds) + 2*(LongSize3ds) + ShortSize3ds;
      break;
   case POINT_ARRAY:
   case POINT_FLAG_ARRAY:
      offset = chunk->size - 6;
      break;
   case FACE_ARRAY:
   {
      ushort3ds faces;
      faces = ReadUShort3ds();
      offset = faces * ShortSize3ds * 4;
      break;
   }
   case MSH_MAT_GROUP:
      offset = chunk->size - 6;
      break;
   case SMOOTH_GROUP:
      offset = chunk->size - 6;
      break;
   case TEX_VERTS:
      offset = chunk->size - 6;
      break;
   case MESH_MATRIX:
      offset = 12*(FloatSize3ds);
      break;
   case MESH_TEXTURE_INFO:
      offset = chunk->size - 6;
      break;
   case PROC_NAME:
      ReadString3ds(dummy, ProcNameStrMax3ds + 1);
      break;
   case DL_LOCAL_SHADOW2:
      offset = 2*(FloatSize3ds) + ShortSize3ds;
      break;
   case KFHDR:
      ReadShort3ds();
      ReadString3ds(dummy, FileNameStrMax3ds + 1);
      ReadLong3ds();
      break;
   case KFSEG:
      offset = 2*(LongSize3ds);
      break;
   case KFCURTIME:
      offset = LongSize3ds;
      break;
   case NODE_HDR:
      ReadString3ds(dummy, NodeHdrStrMax3ds + 1);
      offset = 2*(ShortSize3ds) + ShortSize3ds;
      break;
    case NODE_ID:
      offset = ShortSize3ds;
      break;
   case PIVOT:
      offset = PointSize3ds;
      break;
   case INSTANCE_NAME:
      ReadString3ds(dummy, InstanceNameStrMax3ds + 1);
      break;
   case MORPH_SMOOTH:
      offset = FloatSize3ds;
      break;
   case BOUNDBOX:
      offset = 2*PointSize3ds;
      break;
   case MAT_SXP_TEXT_DATA:
   case MAT_SXP_TEXT2_DATA:
   case MAT_SXP_OPAC_DATA:
   case MAT_SXP_BUMP_DATA:
   case MAT_SXP_SPEC_DATA:
   case MAT_SXP_SHIN_DATA:
   case MAT_SXP_SELFI_DATA:
   case MAT_SXP_TEXT_MASKDATA:
   case MAT_SXP_TEXT2_MASKDATA:
   case MAT_SXP_OPAC_MASKDATA:
   case MAT_SXP_BUMP_MASKDATA:
   case MAT_SXP_SPEC_MASKDATA:
   case MAT_SXP_SHIN_MASKDATA:
   case MAT_SXP_SELFI_MASKDATA:
   case MAT_SXP_REFL_MASKDATA:
   case PROC_DATA:
      offset = (chunk->size - 6);
      break;
   case POS_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case COL_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case ROT_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case SCL_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case MORPH_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case FOV_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case ROLL_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case HOT_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
   case FALL_TRACK_TAG: 
      offset = (chunk->size - 6);
      break;
  case VPDATA: 
      offset = LongSize3ds;
      break;
   default:
      offset = chunk->size - 6;
      break;
   }
   FileSeek3ds(GetFilePosition3ds()+offset);
   
}

