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
#include <assert.h>

#include <string.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "chunkinf.h"
#include "3dsfile.h"
#include "chunk3ds.h"
#include "3dsrange.h"
#include "3dsftkst.h"
#include "3dsutil.h"
#include "3dserr.h"

/* Initializes and allocates memory for a chunk.  */
void InitChunk3ds(chunk3ds **chunk)
{
  if (chunk == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);

   *chunk = malloc(sizeof(chunk3ds));
  if (chunk == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
   
   /* set default values */
   (*chunk)->tag = NULL_CHUNK;
   (*chunk)->size = 0;
   (*chunk)->position = 0;
   (*chunk)->data = NULL;
   (*chunk)->sibling = NULL;
   (*chunk)->children = NULL;
   (*chunk)->readindex = InvalidFileContext3ds;
   (*chunk)->writeindex = InvalidFileContext3ds;
}

/* Initilizes and sets tag in one function */
void InitChunkAs3ds(chunk3ds **chunk, chunktag3ds tag)
{
   sm_tag(ChunkTagToString3ds(tag));
   InitChunk3ds(chunk);
   ON_ERROR_RETURN;
   (*chunk)->tag = tag;
   sm_untag();
}

void ReleaseChunk3ds(chunk3ds **chunk)
{
   /* free memory associated with chunk and substructure */
   if (chunk != NULL)
   {
      if (*chunk != NULL)
      {
	      if ((*chunk)->data != NULL)
			  FreeChunkData3ds(*chunk);
			ReleaseChunk3ds( &(*chunk)->children );
			ReleaseChunk3ds( &(*chunk)->sibling );
      }
      free(*chunk);
      *chunk = NULL;
   }
}

void ReleaseSiblingChunk3ds(chunk3ds **chunk)
{
   if (chunk != NULL)
   {
      if (*chunk != NULL)
      {
	 if ((*chunk)->sibling != NULL) ReleaseSiblingChunk3ds(&((*chunk)->sibling));
	 ReleaseChunk3ds(chunk);
      }
   }
}

/* Delete chunk returns a chunk to its untagged state state, but leaves it
   connected to any siblings it might have */
void DeleteChunk3ds(chunk3ds *chunk)
{
   if (chunk != NULL)
   {
      /* release any children */
      if (chunk->children != NULL) ReleaseChunk3ds(&(chunk->children));

      /* release any data */
      if (chunk->data != NULL) FreeChunkData3ds(chunk);

      /* return to a semi-uninitialized state */
      chunk->tag = NULL_CHUNK; 
      chunk->size = 0;
      chunk->position = 0;
      chunk->readindex = InvalidFileContext3ds;
      chunk->writeindex = InvalidFileContext3ds;
   }
}

/* CheckChunk3ds checksum's a chunk, even the header and children */
long3ds CheckChunk3ds(chunk3ds *chunk)
{
   long3ds sum = 0, nshorts, index;
   ushort3ds *sumbuffer;
      
   SetContextByIndex3ds(chunk->readindex);

   nshorts = chunk->size / sizeof(ushort3ds);

   if ((chunk->size % sizeof(ushort3ds)) > 0) nshorts++;

   sumbuffer = malloc(sizeof(ushort3ds)*nshorts);
   memset(sumbuffer, 0, sizeof(ushort3ds)*nshorts);

   FileSeek3ds(chunk->position);
   ReadVoid3ds(chunk->size, sumbuffer);

   for (index = 0; index < nshorts; index++)
   {
      sum = sum + sumbuffer[index];
   }
   free(sumbuffer);
   
   return sum;
}

/* FindChunk3ds searchs the *top chunk and its children for a match */
void FindChunk3ds(chunk3ds *top, chunktag3ds tag, chunk3ds **chunk)
{
   chunk3ds *child, *match;

   if (chunk == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);


   if (top != NULL)
   {
      if (top->tag == tag)
      {
     *chunk = top;
     return;
      }
      else
     for (child = top->children; child != NULL; child = child->sibling)
     {
        FindChunk3ds(child, tag, &match);
        if (match != NULL)
        {
           *chunk = match;
           return;
        }
     }
     
   }
   *chunk = NULL;
}

void FindNextChunk3ds(chunk3ds *local, chunktag3ds tag, chunk3ds **chunk)
{
   chunk3ds *new=NULL, *current = local;

   myassert(chunk != NULL, "FindNextChunk3ds: arg **chunk cannot be NULL");

   while ((current != NULL) && (new == NULL))
   {
      if (current->tag == tag) new = current;
      current = current->sibling;
   }
   *chunk = new;
}

/* AddChild3ds puts the chunk at then end of the sibling list */
void AddChild3ds(chunk3ds *parent, chunk3ds *child)
{
   chunk3ds *current=NULL;

   if ( parent->children == NULL) {
      parent->children = child;
   }
   else {
      current = parent->children;
      while (current->sibling != NULL) {
     current = current->sibling;
      }
      current->sibling = child;
   }
}

/* AddChildOrdered3ds will insert the child among its siblings depending
   on the order of occurance set by the 3DS file. */
void AddChildOrdered3ds(chunk3ds *parent, chunk3ds *child)
{
   chunk3ds *current, *prev;
   long3ds childvalue;

   if (parent == NULL || child == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   childvalue = GetChunkValue3ds(child->tag);
   
   if (parent->children == NULL)
   {
      parent->children = child;
   } else
   {
      for(current = parent->children, prev = NULL; current->sibling != NULL; prev=current,current=current->sibling)
     {
        if (childvalue > GetChunkValue3ds(current->tag))
          break;
     }

      if (childvalue > GetChunkValue3ds(current->tag))
      {
     child->sibling = current;
     if (prev != NULL) prev->sibling = child;
     else parent->children = child;
      } else
      {
     child->sibling = current->sibling;
     current->sibling = child;
  }
   }
}

/* ReplaceOrAddChild3ds will search *parent's children for a chunk of tag type.  If
   it finds a match, it will delete the existing chunk and prepare it for new data
   to be added.  If there isn't a match, it adds the chunk to the *parent's children
   and returns a reference to the new child in **child */
void ReplaceOrAddChild3ds(chunk3ds *parent, chunktag3ds tag, chunk3ds **child)
{
   if (parent == NULL || child == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Search for child with tag */
   FindNextChunk3ds(parent->children, tag, child);

   /* if child chunk is found */
   if (*child != NULL)
   {
      /* delete the current contents of the chunk while preserving its list position */
      DeleteChunk3ds(*child);
      /* give the deleted chunk the tag */
      (*child)->tag = tag;
   } else /* create new chunk and add it */
   {
      InitChunkAs3ds(child, tag);
      ON_ERROR_RETURN;
      AddChildOrdered3ds(parent, *child);
   }
}

void CopyChunk3ds(chunk3ds *ichunk, chunk3ds **ochunk)
{
  if (ichunk == NULL || ochunk == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  *ochunk = NULL;
  InitChunk3ds(ochunk);
  ON_ERROR_RETURN;

   (*ochunk)->tag = ichunk->tag;
   (*ochunk)->size = ichunk->size;
   (*ochunk)->position = ichunk->position;
   (*ochunk)->readindex = ichunk->readindex;
   (*ochunk)->writeindex = ichunk->writeindex;

   /* In order to minimize memory usage, chunk data that already
      exists in a file mearly has its pointer handed off to the copy.
      The data is only really copied if there is no file image of the
      data */
   if (ichunk->data != NULL)
   {
      if (ichunk->readindex != InvalidFileContext3ds)
      {
     (*ochunk)->data = ichunk->data;
     ichunk->data = NULL;
      } else
     CopyChunkData3ds(ichunk, *ochunk);
      ON_ERROR_RETURN;
   }
   
   if (ichunk->children != NULL)
   {
      chunk3ds *ic;
      chunk3ds **oc;

      for(ic = ichunk->children, oc = &((*ochunk)->children);
	  ic != NULL;
	  ic = ic->sibling, oc = &((*oc)->sibling)
	  )
      {
	 
	 CopyChunk3ds(ic, oc);
	 ON_ERROR_RETURN;
      }
      
   }
  
}

/* DisconnectChunk3ds - reads the data of a chunk and removes its reference to the
   source file */
void DisconnectChunk3ds(chunk3ds *chunk)
{
   myassert(chunk != NULL, "DisconnectChunk3ds: Arg *chunk cannot be NULL");

   ReadChunkData3ds(chunk);
   ON_ERROR_RETURN;
   
   chunk->readindex = InvalidFileContext3ds;
   chunk->writeindex = InvalidFileContext3ds;

   if (chunk->children != NULL)
   {
      chunk3ds *ic;

      for (ic = chunk->children; ic != NULL; ic = ic->sibling)
      {
	 DisconnectChunk3ds(ic);
	 ON_ERROR_RETURN;
      }
   }
}

void InitChunkList3ds(chunklist3ds **list, ulong3ds count)
{
   ulong3ds i;
   
   if (list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (*list == NULL)
   {
      *list = malloc(sizeof(chunklist3ds));
      if (*list == NULL)
	SET_ERROR_RETURN(ERR_NO_MEM)
   }
   
   (*list)->count = count;
   (*list)->list = NULL;
      
   if (count > 0)
   {
      (*list)->list = calloc(count, sizeof(chunklistentry3ds));
      if (*list == NULL)
	SET_ERROR_RETURN(ERR_NO_MEM)
   } 

   for(i = 0; i < count; i++)
   {
      (*list)->list[i].name = NULL;
      (*list)->list[i].chunk = NULL;
   }
}

void ReleaseChunkList3ds(chunklist3ds **list)
{
   ulong3ds i;

    if (list != NULL && *list != NULL)
   {
      if ((*list)->list != NULL)
      {
     for(i = 0; i < (*list)->count; i++)
     {
        free((*list)->list[i].name);
        (*list)->list[i].chunk = NULL;
     }
     free((*list)->list);
      }
      free (*list);
      *list = NULL;
   }
}

/*-------------------------------------------------------------------------
 | InitNameList3ds
 |  If the list is initiated empty, make space for kGRANULE_SIZE entrie
 |  so as items are added we don't have to realloc the list.
 |
 +-------------------------------------------------------------------------*/
static const long3ds kGRANULE_SIZE = 5; 

void InitNameList3ds(namelist3ds **list, ulong3ds count)
{
   ulong3ds i;
   ulong3ds spaces;
   
   if (list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (*list == NULL)
   {
      *list = malloc(sizeof(namelist3ds));
      if (*list == NULL)
	SET_ERROR_RETURN(ERR_NO_MEM);
   }
   
  spaces = (count == 0)? kGRANULE_SIZE: count;

   (*list)->count = count;
   (*list)->list = NULL;
  (*list)->spaces = spaces;
      
  (*list)->list = calloc(spaces, sizeof(namelistentry3ds));
   if ((*list)->list == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   for (i = 0; i < spaces; i++)
   {
      (*list)->list[i].name = NULL;
   }
}

void ReleaseNameList3ds(namelist3ds **list)
{
   ulong3ds i;
   
   if (list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (*list != NULL)
   {
      if ((*list)->list != NULL)
      {
      for(i = 0; i < (*list)->spaces; i++)
     {
          if ((*list)->list[i].name != NULL)
        free((*list)->list[i].name);
     }
     free((*list)->list);
      }
      free (*list);
      *list = NULL;
   }
}

void AddToNameList3ds(namelist3ds **list, char3ds *name)
{
   namelist3ds *pList;

   if (list == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   pList = *list;

   /*--- Just add name to list */
   if (pList->count < pList->spaces)
   {
      pList->list[pList->count].name = strdup3ds(name);
      pList->count++;
   }
   else /*--- Bump list size by kGRANULE_SIZE */
   {
      ulong3ds i, spaces;
      namelistentry3ds *newList;
      
      spaces = pList->spaces + kGRANULE_SIZE;
      newList = calloc(spaces, sizeof(namelistentry3ds));
      
      if (newList == NULL)
	 SET_ERROR_RETURN(ERR_NO_MEM);
      
      /*--- copy over pointers, initialize new space */
      for (i=0; i<spaces; i++){
	 if (i < pList->count)
	    newList[i].name = pList->list[i].name;
	 else
	    newList[i].name = NULL;
      }
      
      /*--- Reassign pointers */
      free((*list)->list);
      (*list)->list = newList;
      (*list)->spaces = spaces;
      
      AddToNameList3ds(list, name);
   }
}


void *InitChunkData3ds(chunk3ds *chunk)
{
   sm_tag(ChunkTagToString3ds(chunk->tag));

   switch (chunk->tag)
   {
   case COLOR_F:
      chunk->data = malloc(sizeof(ColorF));
      break;
   case LIN_COLOR_F:
      chunk->data = malloc(sizeof(LinColorF));
      break;
   case COLOR_24:
      chunk->data = malloc(sizeof(Color24));
      break;
   case LIN_COLOR_24:
      chunk->data = malloc(sizeof(LinColor24));
      break;
   case INT_PERCENTAGE:
      chunk->data = malloc(sizeof(IntPercentage));
      break;
   case FLOAT_PERCENTAGE:
      chunk->data = malloc(sizeof(FloatPercentage));
      break;
   case MAT_MAPNAME:
      chunk->data = malloc(sizeof(MatMapname));
      break;
   case M3D_VERSION:
      chunk->data = malloc(sizeof(M3dVersion));
      break;
   case MESH_VERSION:
      chunk->data = malloc(sizeof(MeshVersion));
      break;
   case MASTER_SCALE:
      chunk->data = malloc(sizeof(MasterScale));
      break;
   case LO_SHADOW_BIAS:
      chunk->data = malloc(sizeof(LoShadowBias));
      break;
   case SHADOW_FILTER:
      chunk->data = malloc(sizeof(ShadowFilter));
      break;
   case SHADOW_RANGE:
      chunk->data = malloc(sizeof(ShadowRange));
      break;
   case HI_SHADOW_BIAS:
      chunk->data = malloc(sizeof(HiShadowBias));
      break;
   case RAY_BIAS:
      chunk->data = malloc(sizeof(RayBias));
      break;
   case SHADOW_MAP_SIZE:
      chunk->data = malloc(sizeof(ShadowMapSize));
      break;
   case SHADOW_SAMPLES:
      chunk->data = malloc(sizeof(ShadowSamples));
      break;
   case O_CONSTS:
      chunk->data = malloc(sizeof(OConsts));
      break;
   case BIT_MAP:
      chunk->data = malloc(sizeof(BitMap));
      break;
   case V_GRADIENT:
      chunk->data = malloc(sizeof(VGradient));
      break;
   case FOG:
      chunk->data = malloc(sizeof(Fog));
      break;
   case LAYER_FOG:
      chunk->data = malloc(sizeof(LayerFog));
      break;
   case DISTANCE_CUE:
      chunk->data = malloc(sizeof(DistanceCue));
      break;
   case VIEW_TOP:
   case VIEW_BOTTOM:
   case VIEW_LEFT:
   case VIEW_RIGHT:
   case VIEW_FRONT:
   case VIEW_BACK:
      chunk->data = malloc(sizeof(ViewStandard));
      break;
   case VIEW_USER:
      chunk->data = malloc(sizeof(ViewUser));
      break;
   case VIEW_CAMERA:
      chunk->data = malloc(sizeof(ViewCamera));
      break;
   case MAT_NAME:
      chunk->data = malloc(sizeof(MatName));
      break;
   case MAT_SHADING:
      chunk->data = malloc(sizeof(MatShading));
      break;
   case MAT_ACUBIC:
      chunk->data = malloc(sizeof(MatAcubic));
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
      chunk->data = malloc(sizeof(IpasData));
      break;
   case MAT_WIRESIZE:
      chunk->data = malloc(sizeof(MatWireSize));
      break;
   case MAT_MAP_TILING:
      chunk->data = malloc(sizeof(MatMapTiling));
      break;
   case MAT_MAP_TEXBLUR:
      chunk->data = malloc(sizeof(MatMapTexblur));
      break;
   case MAT_MAP_USCALE:
      chunk->data = malloc(sizeof(MatMapUScale));
      break;
   case MAT_MAP_VSCALE:
      chunk->data = malloc(sizeof(MatMapVScale));
      break;
   case MAT_MAP_UOFFSET:
      chunk->data = malloc(sizeof(MatMapUOffset));
      break;
   case MAT_MAP_VOFFSET:
      chunk->data = malloc(sizeof(MatMapVOffset));
      break;
   case MAT_MAP_ANG:
      chunk->data = malloc(sizeof(MatMapAng));
      break;
   case MAT_MAP_COL1:
      chunk->data = malloc(sizeof(MatMapCol1));
      break;
   case MAT_MAP_COL2:
      chunk->data = malloc(sizeof(MatMapCol2));
      break;
   case MAT_MAP_RCOL:
      chunk->data = malloc(sizeof(MatMapRCol));
      break;
   case MAT_MAP_GCOL:
      chunk->data = malloc(sizeof(MatMapGCol));
      break;
   case MAT_MAP_BCOL:
      chunk->data = malloc(sizeof(MatMapBCol));
      break;
   case MAT_BUMP_PERCENT:
      chunk->data = malloc(sizeof(MatBumpPercent));
      break;
   case NAMED_OBJECT:
      chunk->data = malloc(sizeof(NamedObject));
      break;
   case POINT_ARRAY:
      chunk->data = malloc(sizeof(PointArray));
      break;
   case POINT_FLAG_ARRAY:
      chunk->data = malloc(sizeof(PointFlagArray));
      break;
   case FACE_ARRAY:
      chunk->data = malloc(sizeof(FaceArray));
      break;
   case MSH_MAT_GROUP:
      chunk->data = malloc(sizeof(MshMatGroup));
      break;
   case MSH_BOXMAP:
      chunk->data = malloc(sizeof(MshBoxmap));
      break;
   case SMOOTH_GROUP:
      chunk->data = malloc(sizeof(SmoothGroup));
      break;
   case TEX_VERTS:
      chunk->data = malloc(sizeof(TexVerts));
      break;
   case MESH_MATRIX:
      chunk->data = malloc(sizeof(MeshMatrix));
      break;
   case MESH_COLOR:
      chunk->data = malloc(sizeof(MeshColor));
      break;
   case MESH_TEXTURE_INFO:
      chunk->data = malloc(sizeof(MeshTextureInfo));
      break;
   case PROC_NAME:
      chunk->data = malloc(sizeof(ProcName));
      break;
   case N_DIRECT_LIGHT:
      chunk->data = malloc(sizeof(NDirectLight));
      break;
   case DL_EXCLUDE:
      chunk->data = malloc(sizeof(DlExclude));
      break;
   case DL_INNER_RANGE:
      chunk->data = malloc(sizeof(DlInnerRange));
      break;
   case DL_OUTER_RANGE:
      chunk->data = malloc(sizeof(DlOuterRange));
      break;
   case DL_MULTIPLIER:
      chunk->data = malloc(sizeof(DlMultiplier));
      break;
   case DL_SPOTLIGHT:
      chunk->data = malloc(sizeof(DlSpotlight));
      break;
   case DL_LOCAL_SHADOW2:
      chunk->data = malloc(sizeof(DlLocalShadow2));
      break;
   case DL_SPOT_ROLL:
      chunk->data = malloc(sizeof(DlSpotRoll));
      break;
   case DL_SPOT_ASPECT:
      chunk->data = malloc(sizeof(DlSpotAspect));
      break;
   case DL_SPOT_PROJECTOR:
      chunk->data = malloc(sizeof(DlSpotProjector));
      break;
   case DL_RAY_BIAS:
      chunk->data = malloc(sizeof(DlRayBias));
      break;
   case N_CAMERA:
      chunk->data = malloc(sizeof(NCamera));
      break;
   case CAM_RANGES:
      chunk->data = malloc(sizeof(CamRanges));
      break;
   case VIEWPORT_LAYOUT:
      chunk->data = malloc(sizeof(ViewportLayout));
      break;
   case VIEWPORT_SIZE:
      chunk->data = malloc(sizeof(ViewportSize));
      break;
   case VIEWPORT_DATA_3:
   case VIEWPORT_DATA:
      chunk->data = malloc(sizeof(ViewportData));
      break;
   case XDATA_ENTRY:
      chunk->data = malloc(sizeof(XDataEntry));
      break;
   case XDATA_APPNAME:
      chunk->data = malloc(sizeof(XDataAppName));
      break;
   case XDATA_STRING:
      chunk->data = malloc(sizeof(XDataString));
      break;
   case KFHDR:
      chunk->data = malloc(sizeof(KFHdr));
      break;
   case KFSEG:
      chunk->data = malloc(sizeof(KFSeg));
      break;
   case KFCURTIME:
      chunk->data = malloc(sizeof(KFCurtime));
      break;
    case NODE_ID:
      chunk->data = malloc(sizeof(KFId));
      break;
   case NODE_HDR:
      chunk->data = malloc(sizeof(NodeHdr));
      break;
   case PIVOT:
      chunk->data = malloc(sizeof(Pivot));
      break;
   case INSTANCE_NAME:
      chunk->data = malloc(sizeof(InstanceName));
      break;
   case PARENT_NAME:
      chunk->data = malloc(sizeof(InstanceName));
      break;
   case MORPH_SMOOTH:
      chunk->data = malloc(sizeof(MorphSmooth));
      break;
   case BOUNDBOX:
      chunk->data = malloc(sizeof(BoundBox));
      break;
   case POS_TRACK_TAG:
      chunk->data = malloc(sizeof(PosTrackTag));
      break;
   case COL_TRACK_TAG:
      chunk->data = malloc(sizeof(ColTrackTag));
      break;
   case ROT_TRACK_TAG:
      chunk->data = malloc(sizeof(RotTrackTag));
      break;
   case SCL_TRACK_TAG:
      chunk->data = malloc(sizeof(ScaleTrackTag));
      break;
   case MORPH_TRACK_TAG:
      chunk->data = malloc(sizeof(MorphTrackTag));
      break;
   case FOV_TRACK_TAG:
      chunk->data = malloc(sizeof(FovTrackTag));
      break;
   case ROLL_TRACK_TAG:
      chunk->data = malloc(sizeof(RollTrackTag));
      break;
   case HOT_TRACK_TAG:
      chunk->data = malloc(sizeof(HotTrackTag));
      break;
   case FALL_TRACK_TAG:
      chunk->data = malloc(sizeof(FallTrackTag));
      break;
   case HIDE_TRACK_TAG:
      chunk->data = malloc(sizeof(HideTrackTag));
      break;
   case M3DMAGIC: /* Chunks who consist entirely of children */
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
   case KFDATA:
   case AMBIENT_NODE_TAG:
   case OBJECT_NODE_TAG:
   case CAMERA_NODE_TAG:
   case TARGET_NODE_TAG:
   case LIGHT_NODE_TAG:
   case SPOTLIGHT_NODE_TAG:
   case L_TARGET_NODE_TAG:
   case CMAGIC:
   case XDATA_SECTION:
   case XDATA_GROUP:
      chunk->data = NULL;
      break;
   default: /* A truely hideous thing to do but it helps with unknown chunks */
   {
      if (chunk->size > 6) /* Don't mess with dataless chunks */
     chunk->data = malloc(chunk->size - 6);
      break;
   }
   } /* End of Case */
   sm_untag();
   return chunk->data; /* returns the pointer should someone want it */
}

/* Initializes a chunk and its data fields in one fell swoop, and
   returns a pointer to the data chunk.  */
void *InitChunkAndData3ds(chunk3ds **chunk, chunktag3ds tag)
{
   sm_tag(ChunkTagToString3ds(tag));
   InitChunk3ds(chunk);
   ON_ERROR_RETURNR(NULL);
   (*chunk)->tag = tag;
   sm_untag();
   return InitChunkData3ds(*chunk);
}

/* Reads the data out of the chunk detailed in *chunk and places a pointer to
   the data into the chunk3ds structure, it will also return that pointer.  */
void *ReadChunkData3ds(chunk3ds *chunk)
{
   myassert(chunk != NULL, "ReadChunkData3ds: arg *chunk cannot be NULL");
   
   if (chunk->data == NULL)  /* don't try to read the data if its already been read */
   {
      /* Set the current file to the file that contain's the chunk's data */
      SetContextByIndex3ds(chunk->readindex);

      /* Seek to the beginning of the chunk's data (harmless if the chunk has no data) */
      FileSeek3ds(chunk->position + 6);

      sm_tag(ChunkTagToString3ds(chunk->tag));
      
      switch (chunk->tag)
      {
      case COLOR_F: 
      {
     ColorF *data; /* local variables are used instead of casts to minimize errors */
     data = malloc(sizeof(ColorF)); /* Allocate the memory to hold the data */
     chunk->data = data; /* copy the pointer into chunk */
     data->red = ReadFloat3ds(); /* Read the data out of the file */
     data->green = ReadFloat3ds();
     data->blue = ReadFloat3ds();
     break;
      }
      case LIN_COLOR_F:
      {
     LinColorF *data;
     data = malloc(sizeof(LinColorF));
     chunk->data = data;
     data->red = ReadFloat3ds();
     data->green = ReadFloat3ds();
     data->blue = ReadFloat3ds();
     break;
      }
      case COLOR_24:
      {
     Color24 *data;
     data = malloc(sizeof(Color24));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case LIN_COLOR_24:
      {
     LinColor24 *data;
     data = malloc(sizeof(LinColor24));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case INT_PERCENTAGE:
      {
     IntPercentage *data;
     data = malloc(sizeof(IntPercentage));
     chunk->data = data;
     data->intpercentage = ReadShort3ds();
     break;
      }
      case FLOAT_PERCENTAGE:
      {
     FloatPercentage *data;
     data = malloc(sizeof(FloatPercentage));
     chunk->data = data;
     data->floatpercentage = ReadFloat3ds();
     break;
      }
      case MAT_MAPNAME:
      {
     MatMapname *data;
     data = malloc(sizeof(MatMapname));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), MatNameStrMax3ds);
     break;
      }
      case M3D_VERSION:
      {
     M3dVersion *data;
     data = malloc(sizeof(M3dVersion));
     chunk->data = data;
     data->version = ReadLong3ds();
     break;
      }
      case MESH_VERSION:
      {
     MeshVersion *data;
     data = malloc(sizeof(MeshVersion));
     chunk->data = data;
     data->version = ReadLong3ds();
     break;
      }
      case MASTER_SCALE:
      {
     MasterScale *data;
     data = malloc(sizeof(MasterScale));
     chunk->data = data;
     data->masterscale = ReadFloat3ds();
     break;
      }
      case LO_SHADOW_BIAS:
      {
     LoShadowBias *data;
     data = malloc(sizeof(LoShadowBias));
     chunk->data = data;
     data->bias = ReadFloat3ds();
     break;
      }
      case SHADOW_FILTER:
      {
     ShadowFilter *data;
     data = (ShadowFilter *) malloc(sizeof(ShadowFilter));
     chunk->data = data;
     data->shadowfilter = ReadFloat3ds();
     break;
      }
      case SHADOW_RANGE:
      {
     ShadowRange *data;
     data = (ShadowRange *) malloc(sizeof(ShadowRange));
     chunk->data = data;
     data->shadowrange = ReadLong3ds();
     break;
      }
      case HI_SHADOW_BIAS:
      {
     HiShadowBias *data;
     data = malloc(sizeof(HiShadowBias));
     chunk->data = data;
     data->bias = ReadFloat3ds();
     break;
      }
      case RAY_BIAS:
      {
     RayBias *data;
     data = malloc(sizeof(RayBias));
     chunk->data = data;
     data->bias = ReadFloat3ds();
     break;
      }
      case SHADOW_MAP_SIZE:
      {
     ShadowMapSize *data;
     data = malloc(sizeof(ShadowMapSize));
     chunk->data = data;
     data->shadowmapsize = ReadShort3ds();
     break;
      }
      case SHADOW_SAMPLES:
      {
     ShadowSamples *data;
     data = malloc(sizeof(ShadowSamples));
     chunk->data = data;
     data->shadowsamples = ReadShort3ds();
     break;
      }
      case O_CONSTS:
      {
     OConsts *data;
     data = malloc(sizeof(OConsts));
     chunk->data = data;
     ReadPoint3ds(&(data->oconsts));
     break;
      }
      case BIT_MAP:
      {
     BitMap *data;
     data = malloc(sizeof(BitMap));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->bitmap), BitMapStrMax3ds);
     break;
      }
      case V_GRADIENT:
      {
     VGradient *data;
     data = (VGradient *) malloc(sizeof(VGradient));
     chunk->data = data;
     data->gradpercent = ReadFloat3ds();
     break;
      }
      case FOG:
      {
     Fog *data;
     data = malloc(sizeof(Fog));
     chunk->data = data;
     data->nearplanedist = ReadFloat3ds();
     data->nearplanedensity = ReadFloat3ds();
     data->farplanedist = ReadFloat3ds();
     data->farplanedensity = ReadFloat3ds();
     break;
      }
      case LAYER_FOG:
      {
     LayerFog *data;
     data = malloc(sizeof(LayerFog));
     chunk->data = data;
     data->zmin = ReadFloat3ds();
     data->zmax = ReadFloat3ds();
     data->density = ReadFloat3ds();
     data->type = ReadULong3ds();
     break;
      } 
      case DISTANCE_CUE:
      {
     DistanceCue *data;
     data = malloc(sizeof(DistanceCue));
     chunk->data = data;
     data->nearplanedist = ReadFloat3ds();
     data->nearplanedimming = ReadFloat3ds();
     data->farplanedist = ReadFloat3ds();
     data->farplanedimming = ReadFloat3ds();
     break;
      }
      case VIEW_TOP:
      case VIEW_BOTTOM:
      case VIEW_LEFT:
      case VIEW_RIGHT:
      case VIEW_FRONT:
      case VIEW_BACK:
      {
     ViewStandard *data;
     data = malloc(sizeof(ViewStandard));
     chunk->data = data;
     data->viewwidth = ReadFloat3ds();
     ReadPoint3ds(&(data->viewtargetcoord));
     break;
      }
      case VIEW_USER:
      {
     ViewUser *data;
     data = malloc(sizeof(ViewUser));
     chunk->data = data;
     data->viewwidth = ReadFloat3ds();
     data->xyviewangle = ReadFloat3ds();
     data->yzviewangle = ReadFloat3ds();
     data->bankangle = ReadFloat3ds();
     ReadPoint3ds(&(data->viewtargetcoord));
     break;
      }
      case VIEW_CAMERA:
      {
     ViewCamera *data;
     data = malloc(sizeof(ViewCamera));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), ViewCameraStrMax3ds);
     break;
      }
      case MAT_NAME:
      {
     MatName *data;
     data = malloc(sizeof(MatName));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), MatNameStrMax3ds);
     break;
      }
      case MAT_SHADING:
      {
     MatShading *data;
     data = malloc(sizeof(MatShading));
     chunk->data = data;
     FileSeek3ds(chunk->position +6);
     data->matshading = ReadShort3ds();
     break;
       }
      case MAT_ACUBIC:
      {
     MatAcubic *data;
     data = malloc(sizeof(MatAcubic));
     chunk->data = data;
     data->shadelevel = ReadByte3ds();
     data->antialias = ReadByte3ds();
     data->flags = ReadShort3ds();
     data->mapsize = ReadULong3ds();
     data->frameinterval = ReadULong3ds();
     break;
      }
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
      {
     IpasData *data;
     data = malloc(sizeof(IpasData));
     chunk->data = data;
     data->size = chunk->size - 6;
     data->data = calloc(data->size, sizeof(byte3ds));
     ReadVoid3ds(data->size, data->data);
     break;
      }
      case MAT_WIRESIZE:
      {
     MatWireSize *data;
     data = malloc(sizeof(MatWireSize));
     chunk->data = data;
     data->wiresize = ReadFloat3ds();
     break;
      }
      case MAT_MAP_TILING:
      {
     MatMapTiling *data;
     data = malloc(sizeof(MatMapTiling));
     chunk->data = data;
     data ->tiling = ReadUShort3ds();
     break;
      }
      case MAT_MAP_TEXBLUR:
      {
     MatMapTexblur *data;
     data = malloc(sizeof(MatMapTexblur));
     chunk->data = data;
     data->percent = ReadFloat3ds();
     break;
      }
      case MAT_MAP_USCALE:
      {
     MatMapUScale *data;
     data = malloc(sizeof(MatMapUScale));
     chunk->data = data;
     data->scale = ReadFloat3ds();
     break;
      }
      case MAT_MAP_VSCALE:
      {
     MatMapVScale *data;
     data = malloc(sizeof(MatMapVScale));
     chunk->data = data;
     data->scale = ReadFloat3ds();
     break;
      }
      case MAT_MAP_UOFFSET:
      {
     MatMapUOffset *data;
     data = malloc(sizeof(MatMapUOffset));
     chunk->data = data;
     data->offset = ReadFloat3ds();
     break;
      }
      case MAT_MAP_VOFFSET:
      {
     MatMapVOffset *data;
     data = malloc(sizeof(MatMapVOffset));
     chunk->data = data;
     data->offset = ReadFloat3ds();
     break;
      }
      case MAT_MAP_ANG:
      {
     MatMapAng *data;
     data = malloc(sizeof(MatMapAng));
     chunk->data = data;
     data->angle = ReadFloat3ds();
     break;
      }
      case MAT_MAP_COL1:
      {
     MatMapCol1 *data;
     data = malloc(sizeof(MatMapCol1));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case MAT_MAP_COL2:
      {
     MatMapCol2 *data;
     data = malloc(sizeof(MatMapCol2));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case MAT_MAP_RCOL:
      {
     MatMapRCol *data;
     data = malloc(sizeof(MatMapRCol));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case MAT_MAP_GCOL:
      {
     MatMapGCol *data;
     data = malloc(sizeof(MatMapGCol));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case MAT_MAP_BCOL:
      {
     MatMapBCol *data;
     data = malloc(sizeof(MatMapBCol));
     chunk->data = data;
     data->red = ReadUByte3ds();
     data->green = ReadUByte3ds();
     data->blue = ReadUByte3ds();
     break;
      }
      case MAT_BUMP_PERCENT:
      {
     MatBumpPercent *data;
     data = malloc(sizeof(MatBumpPercent));
     chunk->data = data;
     data->intpercentage = ReadShort3ds();
     break;
      }
      case NAMED_OBJECT:
      {
     NamedObject *data;
     data = malloc(sizeof(NamedObject));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), NamedObjectStrMax3ds);
     break;
      }
      case POINT_ARRAY:
      {
     PointArray *data;
     ushort3ds        x;
     ushort3ds     vertices;
     data = malloc(sizeof(PointArray));
     chunk->data = data;
     data->vertices = ReadUShort3ds();
     vertices = data->vertices;
     data->pointlist = calloc(vertices, sizeof(point3ds));
     for (x=0; x < vertices; x++) {
        ReadPoint3ds(&(data->pointlist[x]));
     }
     break;
      }
      case POINT_FLAG_ARRAY:
      {
     PointFlagArray *data;
     ushort3ds             x;
     ushort3ds          flags;
     data = malloc(sizeof(PointFlagArray));
     chunk->data = data;
     data->flags = ReadUShort3ds();
     flags = data->flags;
     data->flaglist = calloc(flags, sizeof(short3ds));
     for (x=0; x < flags; x++) {
        *(data->flaglist + x) = ReadShort3ds();
     }
     break;
      }
      case FACE_ARRAY:
      {
     FaceArray *data;
     ushort3ds x;
     ushort3ds faces;
     data = malloc(sizeof(FaceArray));
     chunk->data = data;
     data->faces = ReadUShort3ds();
     faces = data->faces;
     data->facelist = calloc(faces, sizeof(face3ds));
     for (x=0; x < faces; x++) {
        ReadFace3ds(&(data->facelist[x]));
     }
     break;
      }
      case MSH_MAT_GROUP:
      {
     MshMatGroup *data;
     ushort3ds          x;
     ushort3ds       faces;
     data = malloc(sizeof(MshMatGroup));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->matname), MatNameStrMax3ds);
     data->faces = ReadUShort3ds();
     if (data->faces != 0)
     {
        faces = data->faces;
        data->facelist = calloc(faces, sizeof(ushort3ds));
        for (x=0; x < faces; x++) {
           data->facelist[x] = ReadUShort3ds();
        }
     } else
     {
        data->facelist = NULL;
     }
     break;
      }
      case MSH_BOXMAP:
      {
     MshBoxmap *data;
     ushort3ds i;
     data = malloc(sizeof(MshBoxmap));
     chunk->data = data;
         for (i = 0; i < 6; i++)
     {
        ReadAndAllocString3ds(&(data->matnames[i]), MatNameStrMax3ds);
     }
     break;
      }
      case SMOOTH_GROUP:
      {
     SmoothGroup *data;
     ushort3ds x;

     data = malloc(sizeof(SmoothGroup));
     chunk->data = data;
     data->groups = (ushort3ds) ((chunk->size - 6)/4);
     data->grouplist = calloc(data->groups, sizeof(ulong3ds));
     for (x = 0; x < data->groups; x++)
     {
        data->grouplist[x] = ReadULong3ds();
     }
     break;
      }
      case TEX_VERTS:
      {
     TexVerts *data;
     ushort3ds x;
     ushort3ds coords;
     data = malloc(sizeof(TexVerts));
     chunk->data = data;
     data->numcoords = ReadUShort3ds();
     coords = data->numcoords;
     data->textvertlist = calloc(coords, sizeof(textvert3ds));
     for (x=0; x < coords; x++){
        ReadTextVert3ds(&(data->textvertlist[x]));
     }
     break;
      }
      case MESH_MATRIX:
      {
     MeshMatrix *data;
     ushort3ds        x;
     
     data = malloc(sizeof(MeshMatrix));
     chunk->data = data;
         for (x=0; x<=11; x++){
            *(data->xmatrix + x) = ReadFloat3ds();
     }
     break;
      }
      case MESH_COLOR:
      {
     MeshColor *data;
     data = malloc(sizeof(MeshColor));
     chunk->data = data;
     data->color = ReadUByte3ds();
     break;
      }
      case MESH_TEXTURE_INFO:
      {
     MeshTextureInfo *data;
     ushort3ds              x;
     data = malloc(sizeof(MeshTextureInfo));
     chunk->data = data;
     data->maptype = ReadUShort3ds();
     data->xtiling = ReadFloat3ds();
     data->ytiling = ReadFloat3ds();
     ReadPoint3ds(&(data->iconpos));
     data->iconscaling = ReadFloat3ds();
     for (x=0; x<=11; x++) {
        *(data->xmatrix + x) = ReadFloat3ds();
     }
     data->iconwidth = ReadFloat3ds();
     data->iconheight = ReadFloat3ds();
     data->cyliconheight = ReadFloat3ds();
     break;
      }
      case PROC_NAME:
      {
     ProcName *data;
     data = malloc(sizeof(ProcName));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), ProcNameStrMax3ds);
     break;
      }
      case N_DIRECT_LIGHT:
      {
     NDirectLight *data;
     data = malloc(sizeof(NDirectLight));
     chunk->data = data;
     ReadPoint3ds(&(data->lightpos));
     break;
      }
      case DL_EXCLUDE:
      {
     DlExclude *data;
     data = malloc(sizeof(DlExclude));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), NamedObjectStrMax3ds);
     break;
      }
      case DL_INNER_RANGE:
      {
     DlInnerRange *data;
     data = malloc(sizeof(DlInnerRange));
     chunk->data = data;
     data->range = ReadFloat3ds();
     break;
      }
      case DL_OUTER_RANGE:
      {
     DlOuterRange *data;
     data = malloc(sizeof(DlOuterRange));
     chunk->data = data;
     data->range = ReadFloat3ds();
     break;
      }
      case DL_MULTIPLIER:
      {
     DlMultiplier *data;
     data = malloc(sizeof(DlMultiplier));
     chunk->data = data;
     data->multiple = ReadFloat3ds();
     break;
      }
      case DL_SPOTLIGHT:
      {
     DlSpotlight *data;
     data = malloc(sizeof(DlSpotlight));
     chunk->data = data;
     ReadPoint3ds(&(data->spotlighttarg));
     data->hotspotangle = ReadFloat3ds();
     data->falloffangle = ReadFloat3ds();
     break;
      }
      case DL_LOCAL_SHADOW2:
      {
     DlLocalShadow2 *data;
     data = malloc(sizeof(DlLocalShadow2));
     chunk->data = data;
     data->localshadowbias = ReadFloat3ds();
     data->localshadowfilter = ReadFloat3ds();
     data->localshadowmapsize = ReadShort3ds();
     break;
      }
      case DL_SPOT_ROLL:
      {
     DlSpotRoll *data;
     data = malloc(sizeof(DlSpotRoll));
     chunk->data = data;
     data->angle = ReadFloat3ds();
     break;
      }
    case DL_SPOT_ASPECT:
      {
     DlSpotAspect *data;
     data = malloc(sizeof(DlSpotAspect));
     chunk->data = data;
     data->aspect = ReadFloat3ds();
     break;
     }
      case DL_SPOT_PROJECTOR:
      {
     DlSpotProjector *data;
     data = malloc(sizeof(DlSpotProjector));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), BitMapStrMax3ds);
     break;
      }
      case DL_RAY_BIAS:
      {
     DlRayBias *data;
     data = malloc(sizeof(DlRayBias));
     chunk->data = data;
     data->bias = ReadFloat3ds();
     break;
      }
      case N_CAMERA:
      {
     NCamera *data;
     data = malloc(sizeof(NCamera));
     chunk->data = data;
     ReadPoint3ds(&(data->camerapos));
     ReadPoint3ds(&(data->targetpos));
     data->camerabank = ReadFloat3ds();
     data->camerafocallength = ReadFloat3ds();
     break;
      }
      case CAM_RANGES:
      {
     CamRanges *data;
     data = malloc(sizeof(CamRanges));
     chunk->data = data;
     data->nearplane = ReadFloat3ds();
     data->farplane = ReadFloat3ds();
     break;
      }
      case VIEWPORT_LAYOUT:
      {
     ViewportLayout *data;
     data = malloc(sizeof(ViewportLayout));
     chunk->data = data;
     data->form = ReadShort3ds();
     data->top = ReadShort3ds();
     data->ready = ReadShort3ds();
     data->wstate = ReadShort3ds();
     data->swapws = ReadShort3ds();
     data->swapport = ReadShort3ds();
     data->swapcur = ReadShort3ds();
     break;
      }
      case VIEWPORT_SIZE:
      {
     ViewportSize *data;
     data = malloc(sizeof(ViewportSize));
     chunk->data = data;
     data->xpos = ReadUShort3ds();
     data->ypos = ReadUShort3ds();
     data->width = ReadUShort3ds();
     data->height = ReadUShort3ds();
     break;
      }
      case VIEWPORT_DATA_3:
      case VIEWPORT_DATA:
      {
     ViewportData *data;
     data = malloc(sizeof(ViewportData));
     chunk->data = data;
     data->flags = ReadShort3ds();
     data->axislockout = ReadShort3ds();
     data->winxpos = ReadShort3ds();
     data->winypos = ReadShort3ds();
     data->winwidth = ReadShort3ds();
     data->winheight = ReadShort3ds();
     data->view = ReadShort3ds();
     data->zoomfactor = ReadFloat3ds();
     ReadPoint3ds(&(data->center));
     data->horizang = ReadFloat3ds();
     data->vertang = ReadFloat3ds();
     ReadAndAllocFixedString3ds(&(data->camname), NamedObjectStrMax3ds);
     break;
      }
      case XDATA_ENTRY:
      {
         XDataEntry *data;
         data = InitChunkData3ds(chunk);
         data->size = (chunk->size) - 6;
         data->data = malloc(data->size);
         ReadVoid3ds(data->size, data->data);
         break;
      }
      case XDATA_APPNAME:
      {
     XDataAppName *data;
     data = malloc(sizeof(XDataAppName));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), chunk->size);
     break;
      }
      case XDATA_STRING:
      {
     XDataString *data;
     data = malloc(sizeof(XDataString));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->string), chunk->size);
     break;
      }
      case KFHDR:
      {
     KFHdr *data;
     data = malloc(sizeof(KFHdr));
     chunk->data = data;
     data->revision = ReadShort3ds();
     ReadAndAllocString3ds(&(data->filename), FileNameStrMax3ds);
     data->animlength = ReadLong3ds();
     break;
      }
      case KFSEG:
      {
     KFSeg *data;
     data = malloc(sizeof(KFSeg));
     chunk->data = data;
     data->first = ReadLong3ds();
     data->last = ReadLong3ds();
     break;
      }
      case KFCURTIME:
      {
     KFCurtime *data;
     data = malloc(sizeof(KFCurtime));
     chunk->data = data;
     data->currframenum = ReadLong3ds();
     break;
      }
    case NODE_ID:
      {
    KFId *data;
    data = malloc(sizeof(KFId));
    chunk->data = data;
    data->id = ReadShort3ds();
    break;
      }
      case NODE_HDR:
      {
     NodeHdr *data;
     data = malloc(sizeof(NodeHdr));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->objname), NodeHdrStrMax3ds);
     data->flags1 = ReadUShort3ds();
     data->flags2 = ReadUShort3ds();
     data->parentindex = ReadShort3ds();
     break;
      }
      case PIVOT:
      {
     Pivot *data;
     data = malloc(sizeof(Pivot));
     chunk->data = data;
     ReadPoint3ds(&(data->offset));
     break;
      }
      case INSTANCE_NAME:
      {
     InstanceName *data;
     data = malloc(sizeof(InstanceName));
     chunk->data = data;
     ReadAndAllocString3ds(&(data->name), InstanceNameStrMax3ds);
     break;
      }
      case PARENT_NAME:
      {
     break;
      }
      case MORPH_SMOOTH:
      {
     MorphSmooth *data;
     data = malloc(sizeof(MorphSmooth));
     chunk->data = data;
     data->smoothgroupangle = ReadFloat3ds();
     break;
      }
      case BOUNDBOX:
      {
     BoundBox *data;
     data = malloc(sizeof(BoundBox));
     chunk->data = data;
     ReadPoint3ds(&(data->min));
     ReadPoint3ds(&(data->max));
     break;
      }
      case POS_TRACK_TAG:
      {
     PosTrackTag *data;
     ulong3ds        keys;
     ulong3ds          x=0;
     
     data = malloc(sizeof(PosTrackTag));
     chunk->data = data;
     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->positionlist = calloc(keys, sizeof(point3ds));
     
     for(x=0; x<keys; x++)
     {
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        ReadPoint3ds(&(data->positionlist[x]));
     }
     break;
      }
      case COL_TRACK_TAG:
      {
     ColTrackTag *data;
     ulong3ds        keys;
     ulong3ds          x;
     data = malloc(sizeof(ColTrackTag));
     chunk->data = data;
     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     data->colorlist = calloc(keys, sizeof(fcolor3ds));
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        data->colorlist[x].r = ReadFloat3ds();
        data->colorlist[x].g = ReadFloat3ds();
        data->colorlist[x].b = ReadFloat3ds();
     }
     break;
      }
      case ROT_TRACK_TAG:
      {
     RotTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     data = malloc(sizeof(RotTrackTag));
     chunk->data = data;
     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->rotationlist = calloc(keys, sizeof(kfrotkey3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        data->rotationlist[x].angle = ReadFloat3ds();
        data->rotationlist[x].x = ReadFloat3ds();
        data->rotationlist[x].y = ReadFloat3ds();
        data->rotationlist[x].z = ReadFloat3ds();
     }
     break;
      }
      case SCL_TRACK_TAG:
      {
     ScaleTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(ScaleTrackTag));
     chunk->data = data;
     
     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;

     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->scalelist = calloc(keys, sizeof(point3ds));
        
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        data->scalelist[x].x = ReadFloat3ds();
        data->scalelist[x].y = ReadFloat3ds();
        data->scalelist[x].z = ReadFloat3ds();
     }
     break;
      }
      case MORPH_TRACK_TAG:
      {
     MorphTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(MorphTrackTag));
     chunk->data = data;

     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->morphlist = calloc(keys, sizeof(kfmorphkey3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        ReadString3ds(data->morphlist[x].name, NamedObjectStrMax3ds);
     }
     break;
      }
      case FOV_TRACK_TAG:
      {
     FovTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(FovTrackTag));
     chunk->data = data;
     
     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->fovanglelist = calloc(keys, sizeof(float3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        *(data->fovanglelist + x) = ReadFloat3ds();
     }
     break;
      }
      case ROLL_TRACK_TAG:
      {
     RollTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(RollTrackTag));
     chunk->data = data;

     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->rollanglelist = calloc(keys, sizeof(float3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        *(data->rollanglelist + x) = ReadFloat3ds();
     }
     break;
      }
      case HOT_TRACK_TAG:
      {
     HotTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(HotTrackTag));
     chunk->data = data;

     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;

     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->hotspotanglelist = calloc(keys, sizeof(float3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        *(data->hotspotanglelist + x) = ReadFloat3ds();
     }
     break;
      }
      case FALL_TRACK_TAG:
      {
     FallTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(FallTrackTag));
     chunk->data = data;

     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     data->falloffanglelist = calloc(keys, sizeof(float3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
        *(data->falloffanglelist + x) = ReadFloat3ds();
     }
     break;
      }
      case HIDE_TRACK_TAG:
      {
     HideTrackTag *data;
     ulong3ds          x;
     ulong3ds          keys;
     
     data = malloc(sizeof(HideTrackTag));
     chunk->data = data;

     ReadTrackHeader3ds(&(data->trackhdr));
     keys = data->trackhdr.keycount;
     
     data->keyhdrlist = calloc(keys, sizeof(keyheader3ds));
     for(x=0; x<keys; x++){
        ReadKeyHeader3ds(&(data->keyhdrlist[x]));
     }
     break;
      }
      case M3DMAGIC: /* Chunks that do not contain data, or only contain children */
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
      case KFDATA:
      case AMBIENT_NODE_TAG:
      case OBJECT_NODE_TAG:
      case CAMERA_NODE_TAG:
      case TARGET_NODE_TAG:
      case LIGHT_NODE_TAG:
      case SPOTLIGHT_NODE_TAG:
      case L_TARGET_NODE_TAG:
      case CMAGIC:
      case XDATA_SECTION:
      case XDATA_GROUP: 
     break;

      default: /* A truely hideous thing to do but it helps with unknown chunks */
      {
     if (chunk->size > 6) /* Don't mess with dataless chunks */
     {
        chunk->data = malloc(chunk->size - 6);
        ReadVoid3ds(chunk->size - 6, chunk->data);
        break;
     }
      }
      } /* End of Case */
      sm_untag();
   }
   return chunk->data;
}

void WriteChunkData3ds(chunk3ds *chunk)
{

   /* Don't try to write if data field is NULL */
   if (chunk->data != NULL)
   {
      SetContextByIndex3ds(chunk->writeindex);

      switch (chunk->tag)
      {
      case NULL_CHUNK:
      {
     /* don't do anything */
     break;
      }
      case COLOR_F:
      {
     ColorF *data = chunk->data;
     
     WriteFloat3ds(data->red);
     WriteFloat3ds(data->green);
     WriteFloat3ds(data->blue);
     break;
      }
      case LIN_COLOR_F:
      {
     LinColorF *data = chunk->data;
     
     WriteFloat3ds(data->red);
     WriteFloat3ds(data->green);
     WriteFloat3ds(data->blue);
     break;
      }
      case COLOR_24:
      {
     Color24 *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case LIN_COLOR_24:
      {
     LinColor24 *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case INT_PERCENTAGE:
      {
     IntPercentage *data = chunk->data;
     
     WriteShort3ds(data->intpercentage);
     break;
      }
      case FLOAT_PERCENTAGE:
      {
     FloatPercentage *data = chunk->data;
     
     WriteFloat3ds(data->floatpercentage);
     break;
      }
      case MAT_MAPNAME:
      {
     MatMapname *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case M3D_VERSION:
      {
     M3dVersion *data = chunk->data;
     
     WriteLong3ds(data->version);
     break;
      }
      case MESH_VERSION:
      {
     MeshVersion *data = chunk->data;
     
     WriteLong3ds(data->version);
     break;
      }
      case MASTER_SCALE:
      {
     MasterScale *data = chunk->data;
     
     WriteFloat3ds(data->masterscale);
     break;
      }
      case LO_SHADOW_BIAS:
      {
     LoShadowBias *data = chunk->data;
     
     WriteFloat3ds(data->bias);
     break;
      }
      case SHADOW_FILTER:
      {
     ShadowFilter *data = chunk->data;
     
     WriteFloat3ds(data->shadowfilter);
     break;
      }
      case SHADOW_RANGE:
      {
     ShadowRange *data = chunk->data;
     
     WriteLong3ds(data->shadowrange);
     break;
      }
      case HI_SHADOW_BIAS:
      {
     HiShadowBias *data = chunk->data;
     
     WriteFloat3ds(data->bias);
     break;
      }
      case RAY_BIAS:
      {
     RayBias *data = chunk->data;
     
     WriteFloat3ds(data->bias);
     break;
      }
      case SHADOW_MAP_SIZE:
      {
     ShadowMapSize *data = chunk->data;
     
     WriteShort3ds(data->shadowmapsize);
     break;
      }
      case SHADOW_SAMPLES:
      {
     ShadowSamples *data = chunk->data;
     
     WriteShort3ds(data->shadowsamples);
     break;
      }
      case O_CONSTS:
      {
     OConsts *data = chunk->data;
     
     WritePoint3ds(&(data->oconsts));
     break;
      }
      case BIT_MAP:
      {
     BitMap *data = chunk->data;
     
     WriteString3ds(data->bitmap);
     break;
      }
      case V_GRADIENT:
      {
     VGradient *data = chunk->data;
     
     WriteFloat3ds(data->gradpercent);
     break;
      }
      case FOG:
      {
     Fog *data = chunk->data;
     
     WriteFloat3ds(data->nearplanedist);
     WriteFloat3ds(data->nearplanedensity);
     WriteFloat3ds(data->farplanedist);
     WriteFloat3ds(data->farplanedensity);
     break;
      }
      case LAYER_FOG:
      {
     LayerFog *data = chunk->data;
     
     WriteFloat3ds(data->zmin);
     WriteFloat3ds(data->zmax);
     WriteFloat3ds(data->density);
     WriteULong3ds(data->type);
     break;
      } 
      case DISTANCE_CUE:
      {
     DistanceCue *data = chunk->data;
     
     WriteFloat3ds(data->nearplanedist);
     WriteFloat3ds(data->nearplanedimming);
     WriteFloat3ds(data->farplanedist);
     WriteFloat3ds(data->farplanedimming);
     break;
      }
      case VIEW_TOP:
      case VIEW_BOTTOM:
      case VIEW_LEFT:
      case VIEW_RIGHT:
      case VIEW_FRONT:
      case VIEW_BACK:
      {
     ViewStandard *data = chunk->data;
     
     WriteFloat3ds(data->viewwidth);
     WritePoint3ds(&(data->viewtargetcoord));
     break;
      }
      case VIEW_USER:
      {
     ViewUser *data = chunk->data;
     
     WriteFloat3ds(data->viewwidth);
     WriteFloat3ds(data->xyviewangle);
     WriteFloat3ds(data->yzviewangle);
     WriteFloat3ds(data->bankangle);
     WritePoint3ds(&(data->viewtargetcoord));
     break;
      }
      case VIEW_CAMERA:
      {
     ViewCamera *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case MAT_NAME:
      {
     MatName *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case MAT_SHADING:
      {
     MatShading *data = chunk->data;
     
     WriteShort3ds(data->matshading);
     break;
      }
      case MAT_ACUBIC:
      {
     MatAcubic *data = chunk->data;
     
     WriteByte3ds(data->shadelevel);
     WriteByte3ds(data->antialias);
     WriteShort3ds(data->flags);
     WriteULong3ds(data->mapsize);
     WriteULong3ds(data->frameinterval);
     break;
      }
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
      {
     IpasData *data = chunk->data;
     
     WriteVoid3ds(data->size, data->data);
     break;
      }
      case MAT_WIRESIZE:
      {
     MatWireSize *data = chunk->data;
     
     WriteFloat3ds(data->wiresize);
     break;
      }
      case MAT_MAP_TILING:
      {
     MatMapTiling *data = chunk->data;
     
     WriteUShort3ds(data ->tiling);
     break;
      }
      case MAT_MAP_TEXBLUR:
      {
     MatMapTexblur *data = chunk->data;
     
     WriteFloat3ds(data->percent);
     break;
      }
      case MAT_MAP_USCALE:
      {
     MatMapUScale *data = chunk->data;
     
     WriteFloat3ds(data->scale);
     break;
      }
      case MAT_MAP_VSCALE:
      {
     MatMapVScale *data = chunk->data;
     
     WriteFloat3ds(data->scale);
     break;
      }
      case MAT_MAP_UOFFSET:
      {
     MatMapUOffset *data = chunk->data;
     
     WriteFloat3ds(data->offset);
     break;
      }
      case MAT_MAP_VOFFSET:
      {
     MatMapVOffset *data = chunk->data;
     
     WriteFloat3ds(data->offset);
     break;
      }
      case MAT_MAP_ANG:
      {
     MatMapAng *data = chunk->data;
     
     WriteFloat3ds(data->angle);
     break;
      }
      case MAT_MAP_COL1:
      {
     MatMapCol1 *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case MAT_MAP_COL2:
      {
     MatMapCol2 *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case MAT_MAP_RCOL:
      {
     MatMapRCol *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case MAT_MAP_GCOL:
      {
     MatMapGCol *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case MAT_MAP_BCOL:
      {
     MatMapBCol *data = chunk->data;
     
     WriteUByte3ds(data->red);
     WriteUByte3ds(data->green);
     WriteUByte3ds(data->blue);
     break;
      }
      case MAT_BUMP_PERCENT:
      {
     MatBumpPercent *data = chunk->data;
     
     WriteShort3ds(data->intpercentage);
     break;
      }
      case NAMED_OBJECT:
      {
     NamedObject *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case POINT_ARRAY:
      {
     PointArray *data = chunk->data;
     ushort3ds        x;
     
     WriteUShort3ds(data->vertices);

     for (x=0; x < data->vertices; x++)
     {
        WritePoint3ds(data->pointlist + x);
     }
     break;
      }
      case POINT_FLAG_ARRAY:
      {
     PointFlagArray *data = chunk->data;
     ushort3ds x;

     WriteUShort3ds(data->flags);
     for (x=0; x < data->flags; x++)
     {
        WriteShort3ds(data->flaglist[x]);
     }
     break;
      }
      case FACE_ARRAY:
      {
     FaceArray *data = chunk->data;
     ushort3ds x;

     WriteUShort3ds(data->faces);
     for (x=0; x < data->faces; x++)
     {
        WriteFace3ds(data->facelist + x);
     }
     break;
      }
      case MSH_MAT_GROUP:
      {
     MshMatGroup *data = chunk->data;
     ushort3ds x;
     
     WriteString3ds(data->matname);
     WriteUShort3ds(data->faces);

     if (data->faces != 0)
     {
        for (x=0; x < data->faces; x++)
        {
           WriteUShort3ds(data->facelist[x]);
        }
     }
     break;
      }
      case MSH_BOXMAP:
      {
     MshBoxmap *data = chunk->data;
     ushort3ds i;

     for (i = 0; i < 6; i++)
     {
        WriteString3ds(data->matnames[i]);
     }
     break;
      }
      case SMOOTH_GROUP:
      {
     SmoothGroup *data = chunk->data;
     ushort3ds x;

     for (x=0; x < data->groups; x++)
     {
        WriteULong3ds(data->grouplist[x]);
     }
     break;
      }
      case TEX_VERTS:
      {
     TexVerts *data = chunk->data;
     
     ushort3ds      x;

     WriteUShort3ds(data->numcoords);

     for (x=0; x < data->numcoords; x++)
     {
        WriteTextVert3ds(data->textvertlist + x);
     }
     break;
      }
      case MESH_MATRIX:
      {
     MeshMatrix *data = chunk->data;
     ushort3ds        x;
     
     for (x=0; x<=11; x++)
     {
        WriteFloat3ds(data->xmatrix[x]);
     }
     break;
      }
      case MESH_COLOR:
      {
     MeshColor *data = chunk->data;
     
     WriteUByte3ds(data->color);
     break;
      }
      case MESH_TEXTURE_INFO:
      {
     MeshTextureInfo *data = chunk->data;
     ushort3ds              x;

     WriteUShort3ds(data->maptype);
     WriteFloat3ds(data->xtiling);
     WriteFloat3ds(data->ytiling);
     WritePoint3ds(&(data->iconpos));
     WriteFloat3ds(data->iconscaling);
     for (x=0; x<=11; x++)
     {
        WriteFloat3ds(data->xmatrix[x]);
     }
     WriteFloat3ds(data->iconwidth);
     WriteFloat3ds(data->iconheight);
     WriteFloat3ds(data->cyliconheight);
     break;
      }
      case PROC_NAME:
      {
     ProcName *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case N_DIRECT_LIGHT:
      {
     NDirectLight *data = chunk->data;
     
     WritePoint3ds(&(data->lightpos));
     break;
      }
      case DL_EXCLUDE:
      {
     DlExclude *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case DL_INNER_RANGE:
      {
     DlInnerRange *data = chunk->data;
     
     WriteFloat3ds(data->range);
     break;
      }
      case DL_OUTER_RANGE:
      {
     DlOuterRange *data = chunk->data;
     
     WriteFloat3ds(data->range);
     break;
      }
      case DL_MULTIPLIER:
      {
     DlMultiplier *data = chunk->data;
     
     WriteFloat3ds(data->multiple);
     break;
      }
      case DL_SPOTLIGHT:
      {
     DlSpotlight *data = chunk->data;
     
     WritePoint3ds(&(data->spotlighttarg));
     WriteFloat3ds(data->hotspotangle);
     WriteFloat3ds(data->falloffangle);
     break;
      }
      case DL_LOCAL_SHADOW2:
      {
     DlLocalShadow2 *data = chunk->data;
     
     WriteFloat3ds(data->localshadowbias);
     WriteFloat3ds(data->localshadowfilter);
     WriteShort3ds(data->localshadowmapsize);
     break;
      }
      case DL_SPOT_ROLL:
      {
     DlSpotRoll *data = chunk->data;
     
     WriteFloat3ds(data->angle);
     break;
      }
    case DL_SPOT_ASPECT:
      {
     DlSpotAspect *data = chunk->data;
     
     WriteFloat3ds(data->aspect);
     break;
      }
      case DL_SPOT_PROJECTOR:
      {
     DlSpotProjector *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case DL_RAY_BIAS:
      {
     DlRayBias *data = chunk->data;
     
     WriteFloat3ds(data->bias);
     break;
      }
      case N_CAMERA:
      {
     NCamera *data = chunk->data;
     
     WritePoint3ds(&(data->camerapos));
     WritePoint3ds(&(data->targetpos));
     WriteFloat3ds(data->camerabank);
     WriteFloat3ds(data->camerafocallength);
     break;
      }
      case CAM_RANGES:
      {
     CamRanges *data = chunk->data;
     
     WriteFloat3ds(data->nearplane);
     WriteFloat3ds(data->farplane);
     break;
      }
      case VIEWPORT_LAYOUT:
      {
     ViewportLayout *data = chunk->data;
     
     WriteShort3ds(data->form);
     WriteShort3ds(data->top);
     WriteShort3ds(data->ready);
     WriteShort3ds(data->wstate);
     WriteShort3ds(data->swapws);
     WriteShort3ds(data->swapport);
     WriteShort3ds(data->swapcur);
     break;
      }
      case VIEWPORT_SIZE:
      {
     ViewportSize *data = chunk->data;
     
     WriteUShort3ds(data->xpos);
     WriteUShort3ds(data->ypos);
     WriteUShort3ds(data->width);
     WriteUShort3ds(data->height);
     break;
      }
      case VIEWPORT_DATA_3:
      case VIEWPORT_DATA:
      {
     ViewportData *data = chunk->data;
     
     WriteShort3ds(data->flags);
     WriteShort3ds(data->axislockout);
     WriteShort3ds(data->winxpos);
     WriteShort3ds(data->winypos);
     WriteShort3ds(data->winwidth);
     WriteShort3ds(data->winheight);
     WriteShort3ds(data->view);
     WriteFloat3ds(data->zoomfactor);
     WritePoint3ds(&(data->center));
     WriteFloat3ds(data->horizang);
     WriteFloat3ds(data->vertang);
     WriteFixedString3ds(data->camname, NamedObjectStrMax3ds+1);
     break;
      }
      case XDATA_APPNAME:
      {
     XDataAppName *data = chunk->data;
     WriteString3ds(data->name);
     break;
      }
      case XDATA_STRING:
      {
     XDataString *data = chunk->data;
     WriteString3ds(data->string);
     break;
      }
      case KFHDR:
      {
     KFHdr *data = chunk->data;
     
     WriteShort3ds(data->revision);
     WriteString3ds(data->filename);
     WriteLong3ds(data->animlength);
     break;
      }
      case KFSEG:
      {
     KFSeg *data = chunk->data;
     
     WriteLong3ds(data->first);
     WriteLong3ds(data->last);
     break;
      }
      case KFCURTIME:
      {
     KFCurtime *data = chunk->data;
     
     WriteLong3ds(data->currframenum);
     break;
      }
    case NODE_ID:
      {
    KFId  *data = chunk->data;

    WriteShort3ds(data->id);
    break;
      }
      case NODE_HDR:
      {
     NodeHdr *data = chunk->data;
     
     WriteString3ds(data->objname);
     WriteUShort3ds(data->flags1);
     WriteUShort3ds(data->flags2);
     WriteShort3ds(data->parentindex);
     break;
      }
      case PIVOT:
      {
     Pivot *data = chunk->data;
     
     WritePoint3ds(&(data->offset));
     break;
      }
      case INSTANCE_NAME:
      {
     InstanceName *data = chunk->data;
     
     WriteString3ds(data->name);
     break;
      }
      case PARENT_NAME:
      {
     break;
      }
      case MORPH_SMOOTH:
      {
     MorphSmooth *data = chunk->data;
     
     WriteFloat3ds(data->smoothgroupangle);
     break;
      }
      case BOUNDBOX:
      {
     BoundBox *data = chunk->data;
     
     WritePoint3ds(&(data->min));
     WritePoint3ds(&(data->max));
     break;
      }
      case POS_TRACK_TAG:
      {
     PosTrackTag *data = chunk->data;
     ulong3ds x;
     
     WriteTrackHeader3ds(&(data->trackhdr));
     
     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WritePoint3ds(data->positionlist + x);
     }
     break;
      }
      case COL_TRACK_TAG:
      {
     ColTrackTag *data = chunk->data;
     ulong3ds          x;

     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->colorlist[x].r);
        WriteFloat3ds(data->colorlist[x].g);
        WriteFloat3ds(data->colorlist[x].b);
     }
     break;
      }
      case ROT_TRACK_TAG:
      {
     RotTrackTag *data = chunk->data;
     ulong3ds          x;

     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->rotationlist[x].angle);
        WriteFloat3ds(data->rotationlist[x].x);
        WriteFloat3ds(data->rotationlist[x].y);
        WriteFloat3ds(data->rotationlist[x].z);
     }
     break;
      }
      case SCL_TRACK_TAG:
      {
     ScaleTrackTag *data = chunk->data;
     ulong3ds          x;
     
     WriteTrackHeader3ds(&(data->trackhdr));
     
     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->scalelist[x].x);
        WriteFloat3ds(data->scalelist[x].y);
        WriteFloat3ds(data->scalelist[x].z);
     }
     break;
      }
      case MORPH_TRACK_TAG:
      {
     MorphTrackTag *data = chunk->data;
     ulong3ds          x;
         
     WriteTrackHeader3ds(&(data->trackhdr));
     
     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteString3ds(data->morphlist[x].name);
     }
     break;
      }
      case FOV_TRACK_TAG:
      {
     FovTrackTag *data = chunk->data;
     ulong3ds          x;
     
     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->fovanglelist[x]);
     }
     break;
      }
      case ROLL_TRACK_TAG:
      {
     RollTrackTag *data = chunk->data;
     ulong3ds          x;
     
     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->rollanglelist[x]);
     }
     break;
      }
      case HOT_TRACK_TAG:
      {
     HotTrackTag *data = chunk->data;
     ulong3ds          x;
     
     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->hotspotanglelist[x]);
     }
     break;
      }
      case FALL_TRACK_TAG:
      {
     FallTrackTag *data = chunk->data;
     ulong3ds          x;

     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
        WriteFloat3ds(data->falloffanglelist[x]);
     }
     break;
      }
      case HIDE_TRACK_TAG:
      {
     HideTrackTag *data = chunk->data;
     ulong3ds          x;

     WriteTrackHeader3ds(&(data->trackhdr));

     for(x = 0; x < data->trackhdr.keycount; x++)
     {
        WriteKeyHeader3ds(data->keyhdrlist + x);
     }
     break;
      }
      default: /* A truely hideous thing to do but it helps in prototyping */
      {
     if (chunk->size > 6)
     {
        WriteVoid3ds(chunk->size - 6, chunk->data);
        break;
     }
      }
      } /* End of Case */
   }
   
}
   
void CopyChunkData3ds(chunk3ds *ichunk, chunk3ds *ochunk)
{
   sm_tag(ChunkTagToString3ds(ichunk->tag));
   
   /* Don't try to copy if data field is NULL */
   if ((ichunk->data != NULL) && (ochunk->data == NULL))
   {
      switch (ichunk->tag)
      {
      case COLOR_F:
      {
     ochunk->data = malloc(sizeof(ColorF));
     memcpy(ochunk->data, ichunk->data, sizeof(ColorF));
     break;
      }
      case LIN_COLOR_F:
      {
     ochunk->data = malloc(sizeof(LinColorF));
     memcpy(ochunk->data, ichunk->data, sizeof(LinColorF));
     break;
      }
      case COLOR_24:
      {
     ochunk->data = malloc(sizeof(Color24));
     memcpy(ochunk->data, ichunk->data, sizeof(Color24));
     break;
      }
      case LIN_COLOR_24:
      {
     ochunk->data = malloc(sizeof(LinColor24));
     memcpy(ochunk->data, ichunk->data, sizeof(LinColor24));
     break;
      }
      case INT_PERCENTAGE:
      {
     ochunk->data = malloc(sizeof(IntPercentage));
     memcpy(ochunk->data, ichunk->data, sizeof(IntPercentage));
     break;
      }
      case FLOAT_PERCENTAGE:
      {
     ochunk->data = malloc(sizeof(FloatPercentage));
     memcpy(ochunk->data, ichunk->data, sizeof(FloatPercentage));
     break;
      }
      case MAT_MAPNAME:
      {
     MatMapname *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(MatMapname));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapname));
     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case M3D_VERSION:
      {
     ochunk->data = malloc(sizeof(M3dVersion));
     memcpy(ochunk->data, ichunk->data, sizeof(M3dVersion));
     break;
      }
      case MESH_VERSION:
      {
     ochunk->data = malloc(sizeof(MeshVersion));
     memcpy(ochunk->data, ichunk->data, sizeof(MeshVersion));
     break;
      }
      case MASTER_SCALE:
      {
     ochunk->data = malloc(sizeof(MasterScale));
     memcpy(ochunk->data, ichunk->data, sizeof(MasterScale));
     break;
      }
      case LO_SHADOW_BIAS:
      {
     ochunk->data = malloc(sizeof(LoShadowBias));
     memcpy(ochunk->data, ichunk->data, sizeof(LoShadowBias));
     break;
      }
      case SHADOW_FILTER:
      {
     ochunk->data = malloc(sizeof(ShadowFilter));
     memcpy(ochunk->data, ichunk->data, sizeof(ShadowFilter));
     break;
      }
      case SHADOW_RANGE:
      {
     ochunk->data = malloc(sizeof(ShadowRange));
     memcpy(ochunk->data, ichunk->data, sizeof(ShadowRange));
     break;
      }
      case HI_SHADOW_BIAS:
      {
     ochunk->data = malloc(sizeof(HiShadowBias));
     memcpy(ochunk->data, ichunk->data, sizeof(HiShadowBias));
     break;
      }
      case RAY_BIAS:
      {
     ochunk->data = malloc(sizeof(RayBias));
     memcpy(ochunk->data, ichunk->data, sizeof(RayBias));
     break;
      }
      case SHADOW_MAP_SIZE:
      {
     ochunk->data = malloc(sizeof(ShadowMapSize));
     memcpy(ochunk->data, ichunk->data, sizeof(ShadowMapSize));
     break;
      }
      case SHADOW_SAMPLES:
      {
     ochunk->data = malloc(sizeof(ShadowSamples));
     memcpy(ochunk->data, ichunk->data, sizeof(ShadowSamples));
     break;
      }
      case O_CONSTS:
      {
     ochunk->data = malloc(sizeof(OConsts));
     memcpy(ochunk->data, ichunk->data, sizeof(OConsts));
     break;
      }
      case BIT_MAP:
      {
     BitMap *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(BitMap));
     odata = ochunk->data;
     memcpy(ochunk->data, ichunk->data, sizeof(BitMap));
     odata->bitmap = malloc(strlen(idata->bitmap)+1);
     
     strcpy(odata->bitmap, idata->bitmap);
     
     break;
      }
      case V_GRADIENT:
      {
     ochunk->data = malloc(sizeof(VGradient));
     memcpy(ochunk->data, ichunk->data, sizeof(VGradient));
     break;
      }
      case FOG:
      {
     ochunk->data = malloc(sizeof(Fog));
     memcpy(ochunk->data, ichunk->data, sizeof(Fog));
     break;
      }
      case LAYER_FOG:
      {
     ochunk->data = malloc(sizeof(LayerFog));
     memcpy(ochunk->data, ichunk->data, sizeof(LayerFog));
     break;
      } 
      case DISTANCE_CUE:
      {
     ochunk->data = malloc(sizeof(DistanceCue));
     memcpy(ochunk->data, ichunk->data, sizeof(DistanceCue));
     break;
      }
      case VIEW_TOP:
      case VIEW_BOTTOM:
      case VIEW_LEFT:
      case VIEW_RIGHT:
      case VIEW_FRONT:
      case VIEW_BACK:
      {
     ochunk->data = malloc(sizeof(ViewStandard));
     memcpy(ochunk->data, ichunk->data, sizeof(ViewStandard));
     break;
      }
      case VIEW_USER:
      {
     ochunk->data = malloc(sizeof(ViewUser));
     memcpy(ochunk->data, ichunk->data, sizeof(ViewUser));
     break;
      }
      case VIEW_CAMERA:
      {
     ViewCamera *idata, *odata;
     
     ochunk->data = malloc(sizeof(ViewCamera));
     idata = ichunk->data;
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(ViewCamera));

     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case MAT_NAME:
      {
     MatName *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(MatName));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(MatName));

     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case MAT_SHADING:
      {
     ochunk->data = malloc(sizeof(MatShading));
     memcpy(ochunk->data, ichunk->data, sizeof(MatShading));
     break;
      }
      case MAT_ACUBIC:
      {
     ochunk->data = malloc(sizeof(MatAcubic));
     memcpy(ochunk->data, ichunk->data, sizeof(MatAcubic));
     break;
      }
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
      {
     IpasData *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(IpasData));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(IpasData));
     odata->data = malloc(idata->size);
     memcpy(odata->data, idata->data, (size_t)(idata->size));
     
     break;
      }
      case MAT_WIRESIZE:
      {
     ochunk->data = malloc(sizeof(MatWireSize));
     memcpy(ochunk->data, ichunk->data, sizeof(MatWireSize));
     break;
      }
      case MAT_MAP_TILING:
      {
     ochunk->data = malloc(sizeof(MatMapTiling));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapTiling));
     break;
      }
      case MAT_MAP_TEXBLUR:
      {
     ochunk->data = malloc(sizeof(MatMapTexblur));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapTexblur));
     break;
      }
      case MAT_MAP_USCALE:
      {
     ochunk->data = malloc(sizeof(MatMapUScale));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapUScale));
     break;
      }
      case MAT_MAP_VSCALE:
      {
     ochunk->data = malloc(sizeof(MatMapVScale));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapVScale));
     break;
      }
      case MAT_MAP_UOFFSET:
      {
     ochunk->data = malloc(sizeof(MatMapUOffset));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapUOffset));
     break;
      }
      case MAT_MAP_VOFFSET:
      {
     ochunk->data = malloc(sizeof(MatMapVOffset));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapVOffset));
     break;
      }
      case MAT_MAP_ANG:
      {
     ochunk->data = malloc(sizeof(MatMapAng));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapAng));
     break;
      }
      case MAT_MAP_COL1:
      {
     ochunk->data = malloc(sizeof(MatMapCol1));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapCol1));
     break;
      }
      case MAT_MAP_COL2:
      {
     ochunk->data = malloc(sizeof(MatMapCol2));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapCol2));
     break;
      }
      case MAT_MAP_RCOL:
      {
     ochunk->data = malloc(sizeof(MatMapRCol));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapRCol));
     break;
      }
      case MAT_MAP_GCOL:
      {
     ochunk->data = malloc(sizeof(MatMapGCol));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapGCol));
     break;
      }
      case MAT_MAP_BCOL:
      {
     ochunk->data = malloc(sizeof(MatMapBCol));
     memcpy(ochunk->data, ichunk->data, sizeof(MatMapBCol));
     break;
      }
      case MAT_BUMP_PERCENT:
      {
     ochunk->data = malloc(sizeof(MatBumpPercent));
     memcpy(ochunk->data, ichunk->data, sizeof(MatBumpPercent));
     break;
      }
      case NAMED_OBJECT:
      {
	NamedObject *idata = ichunk->data, *odata;
     
	odata = ochunk->data = malloc(sizeof(NamedObject));
	odata->name = strdup3ds(idata->name);
	break;
      }
      case POINT_ARRAY:
      {
     PointArray *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(PointArray));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(PointArray));

     odata->pointlist = calloc(idata->vertices, sizeof(point3ds));
     memcpy(odata->pointlist, idata->pointlist, sizeof(point3ds)*idata->vertices);
     break;
      }
      case POINT_FLAG_ARRAY:
      {
     PointFlagArray *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(PointFlagArray));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(PointFlagArray));
     odata->flaglist = calloc(idata->flags, sizeof(short3ds));

     memcpy(odata->flaglist, idata->flaglist, sizeof(short3ds)*idata->flags);
     
     break;
      }
      case FACE_ARRAY:
      {
     FaceArray *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(FaceArray));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(FaceArray));

     odata->facelist = calloc(idata->faces, sizeof(face3ds));
     memcpy(odata->facelist, idata->facelist, idata->faces*sizeof(face3ds));
     
     break;
      }
      case MSH_MAT_GROUP:
      {
     MshMatGroup *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(MshMatGroup));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(MshMatGroup));

     odata->facelist = malloc(idata->faces*sizeof(ushort3ds));
     memcpy(odata->facelist, idata->facelist, idata->faces*sizeof(ushort3ds));

     odata->matname = malloc(strlen(idata->matname)+1);
     strcpy(odata->matname, idata->matname);
     
     break;
      }
      case MSH_BOXMAP:
      {
     MshBoxmap *idata = ichunk->data, *odata;
     ushort3ds i;

     ochunk->data = malloc(sizeof(MshBoxmap));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(MshBoxmap));

     for (i = 0; i < 6; i++)
     {
        odata->matnames[i] = malloc(strlen(idata->matnames[i])+1);
        strcpy(odata->matnames[i], idata->matnames[i]);
     }
     break;
      }
      case SMOOTH_GROUP:
      {
     SmoothGroup *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(SmoothGroup));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(SmoothGroup));

     odata->grouplist = malloc(idata->groups*sizeof(ulong3ds));
     memcpy(odata->grouplist, idata->grouplist, idata->groups*sizeof(ulong3ds));
     
     break;
      }
      case TEX_VERTS:
      {
     TexVerts *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(TexVerts));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(TexVerts));

     odata->textvertlist = malloc(idata->numcoords*sizeof(textvert3ds));
     memcpy(odata->textvertlist, idata->textvertlist, idata->numcoords*sizeof(textvert3ds));
     
     break;
      }
      case MESH_MATRIX:
      {
     ochunk->data = malloc(sizeof(MeshMatrix));
     memcpy(ochunk->data, ichunk->data, sizeof(MeshMatrix));
     break;
      }
      case MESH_COLOR:
      {
     ochunk->data = malloc(sizeof(MeshColor));
     memcpy(ochunk->data, ichunk->data, sizeof(MeshColor));
     break;
      }
      case MESH_TEXTURE_INFO:
      {
     ochunk->data = malloc(sizeof(MeshTextureInfo));
     memcpy(ochunk->data, ichunk->data, sizeof(MeshTextureInfo));
     break;
      }
      case PROC_NAME:
      {
     ProcName *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(ProcName));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(ProcName));
     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case N_DIRECT_LIGHT:
      {
     ochunk->data = malloc(sizeof(NDirectLight));
     memcpy(ochunk->data, ichunk->data, sizeof(NDirectLight));
     break;
      }
      case DL_EXCLUDE:
      {
	DlExclude *idata = ichunk->data, *odata;
	
	odata = ochunk->data = malloc(sizeof(DlExclude));
	odata->name = strdup3ds(idata->name);
	break;
      }
      case DL_INNER_RANGE:
      {
     ochunk->data = malloc(sizeof(DlInnerRange));
     memcpy(ochunk->data, ichunk->data, sizeof(DlInnerRange));
     break;
      }
      case DL_OUTER_RANGE:
      {
     ochunk->data = malloc(sizeof(DlOuterRange));
     memcpy(ochunk->data, ichunk->data, sizeof(DlOuterRange));
     break;
      }
      case DL_MULTIPLIER:
      {
     ochunk->data = malloc(sizeof(DlMultiplier));
     memcpy(ochunk->data, ichunk->data, sizeof(DlMultiplier));
     break;
      }
      case DL_SPOTLIGHT:
      {
     ochunk->data = malloc(sizeof(DlSpotlight));
     memcpy(ochunk->data, ichunk->data, sizeof(DlSpotlight));
     break;
      }
      case DL_LOCAL_SHADOW2:
      {
     ochunk->data = malloc(sizeof(DlLocalShadow2));
     memcpy(ochunk->data, ichunk->data, sizeof(DlLocalShadow2));
     break;
      }
      case DL_SPOT_ROLL:
      {
     ochunk->data = malloc(sizeof(DlSpotRoll));
     memcpy(ochunk->data, ichunk->data, sizeof(DlSpotRoll));
     break;
      }
      case DL_SPOT_ASPECT:
      {
     ochunk->data = malloc(sizeof(DlSpotAspect));
     memcpy(ochunk->data, ichunk->data, sizeof(DlSpotAspect));
     break;
      }
      case DL_SPOT_PROJECTOR:
      {
     DlSpotProjector *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(DlSpotProjector));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(DlSpotProjector));

     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case DL_RAY_BIAS:
      {
     ochunk->data = malloc(sizeof(DlRayBias));
     memcpy(ochunk->data, ichunk->data, sizeof(DlRayBias));
     break;
      }
      case N_CAMERA:
      {
     ochunk->data = malloc(sizeof(NCamera));
     memcpy(ochunk->data, ichunk->data, sizeof(NCamera));
     break;
      }
      case CAM_RANGES:
      {
     ochunk->data = malloc(sizeof(CamRanges));
     memcpy(ochunk->data, ichunk->data, sizeof(CamRanges));
     break;
      }
      case VIEWPORT_LAYOUT:
      {
     ochunk->data = malloc(sizeof(ViewportLayout));
     memcpy(ochunk->data, ichunk->data, sizeof(ViewportLayout));
     break;
      }
      case VIEWPORT_SIZE:
      {
     ochunk->data = malloc(sizeof(ViewportSize));
     memcpy(ochunk->data, ichunk->data, sizeof(ViewportSize));
     break;
      }
      case VIEWPORT_DATA_3:
      case VIEWPORT_DATA:
      {
     ViewportData *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(ViewportData));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(ViewportData));
     odata->camname = malloc(strlen(idata->camname)+1);
     strcpy(odata->camname, idata->camname);
     
     break;
      }
      case KFHDR:
      {
     KFHdr *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(KFHdr));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(KFHdr));
     odata->filename = malloc(strlen(idata->filename)+1);
     strcpy(odata->filename, idata->filename);
     
     break;
      }
      case KFSEG:
      {
     ochunk->data = malloc(sizeof(KFSeg));
     memcpy(ochunk->data, ichunk->data, sizeof(KFSeg));
     break;
      }
      case KFCURTIME:
      {
     ochunk->data = malloc(sizeof(KFCurtime));
     memcpy(ochunk->data, ichunk->data, sizeof(KFCurtime));
     break;
      }
    case NODE_ID:
      {
	ochunk->data = malloc(sizeof(KFId));
	memcpy(ochunk->data, ichunk->data, sizeof(KFId));
	break;
      }
      case NODE_HDR:
      {
     NodeHdr *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(NodeHdr));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(NodeHdr));

     odata->objname = malloc(strlen(idata->objname)+1);
     strcpy(odata->objname, idata->objname);
     
     break;
      }
      case PIVOT:
      {
     ochunk->data = malloc(sizeof(Pivot));
     memcpy(ochunk->data, ichunk->data, sizeof(Pivot));
     break;
      }
      case PARENT_NAME:
      {
     InstanceName *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(InstanceName));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(InstanceName));

     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
   }
      case INSTANCE_NAME:
      {
     InstanceName *idata = ichunk->data, *odata;
     
     ochunk->data = malloc(sizeof(InstanceName));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(InstanceName));

     odata->name = malloc(strlen(idata->name)+1);
     strcpy(odata->name, idata->name);
     
     break;
      }
      case MORPH_SMOOTH:
      {
     ochunk->data = malloc(sizeof(MorphSmooth));
     memcpy(ochunk->data, ichunk->data, sizeof(MorphSmooth));
     break;
      }
      case BOUNDBOX:
      {
     ochunk->data = malloc(sizeof(BoundBox));
     memcpy(ochunk->data, ichunk->data, sizeof(BoundBox));
     break;
      }
      case POS_TRACK_TAG:
      {
     PosTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(PosTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(PosTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->positionlist = malloc(idata->trackhdr.keycount*sizeof(point3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->positionlist, idata->positionlist, idata->trackhdr.keycount*sizeof(point3ds));
     
     break;
      }
      case COL_TRACK_TAG:
      {
     ColTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(ColTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(ColTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->colorlist = malloc(idata->trackhdr.keycount*sizeof(fcolor3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->colorlist, idata->colorlist, idata->trackhdr.keycount*sizeof(fcolor3ds));
     
     break;
      }
      case ROT_TRACK_TAG:
      {
     RotTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(RotTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(RotTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->rotationlist = malloc(idata->trackhdr.keycount*sizeof(kfrotkey3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->rotationlist, idata->rotationlist, idata->trackhdr.keycount*sizeof(kfrotkey3ds));
     
     break;
      }
      case SCL_TRACK_TAG:
      {
     ScaleTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(ScaleTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(ScaleTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->scalelist = malloc(idata->trackhdr.keycount*sizeof(point3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->scalelist, idata->scalelist, idata->trackhdr.keycount*sizeof(point3ds));
     
     break;
      }
      case MORPH_TRACK_TAG:
      {
     MorphTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(MorphTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(MorphTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->morphlist = malloc(idata->trackhdr.keycount*sizeof(kfmorphkey3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->morphlist, idata->morphlist, idata->trackhdr.keycount*sizeof(kfmorphkey3ds));
     
     break;
      }
      case FOV_TRACK_TAG:
      {
     FovTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(FovTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(FovTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->fovanglelist = malloc(idata->trackhdr.keycount*sizeof(float3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->fovanglelist, idata->fovanglelist, idata->trackhdr.keycount*sizeof(float3ds));
     
     break;
      }
      case ROLL_TRACK_TAG:
      {
     RollTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(RollTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(RollTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->rollanglelist = malloc(idata->trackhdr.keycount*sizeof(float3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->rollanglelist, idata->rollanglelist, idata->trackhdr.keycount*sizeof(float3ds));
     
     break;
      }
      case HOT_TRACK_TAG:
      {
     HotTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(HotTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(HotTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->hotspotanglelist = malloc(idata->trackhdr.keycount*sizeof(float3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->hotspotanglelist, idata->hotspotanglelist, idata->trackhdr.keycount*sizeof(float3ds));
     
     break;
      }
      case FALL_TRACK_TAG:
      {
     FallTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(FallTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(FallTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));
     odata->falloffanglelist = malloc(idata->trackhdr.keycount*sizeof(float3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     memcpy(odata->falloffanglelist, idata->falloffanglelist, idata->trackhdr.keycount*sizeof(float3ds));
     
     break;
      }
      case HIDE_TRACK_TAG:
      {
     HideTrackTag *idata = ichunk->data, *odata;

     ochunk->data = malloc(sizeof(HideTrackTag));
     odata = ochunk->data;
     
     memcpy(ochunk->data, ichunk->data, sizeof(HideTrackTag));

     odata->keyhdrlist = malloc(idata->trackhdr.keycount*sizeof(keyheader3ds));

     memcpy(odata->keyhdrlist, idata->keyhdrlist, idata->trackhdr.keycount*sizeof(keyheader3ds));
     break;
      }
      default:  /* complements the default in the reader, will work on any pointer free chunk */
      {
     if (ichunk->size > 6)
     {
        ochunk->data = malloc(ichunk->size - 6);
        memcpy(ochunk->data, ichunk->data, ichunk->size - 6);
     }
      }
     
      
      } /* End of Case */
   }
   sm_untag();
}

/* Function only frees the chunk data if it has been written to a file */
void FreeFileChunkData3ds(chunk3ds *chunk)
{
   myassert(chunk != NULL, "FreeFileChunkData3ds: arg *chunk cannot be NULL");

   if (IsValidFileIndex3ds(chunk->readindex)) FreeChunkData3ds(chunk);
}

void FreeChunkData3ds(chunk3ds *chunk)
{
   myassert(chunk != NULL, "FreeChunkData3ds: arge *chunk cannot be a NULL pointer.");

   if (chunk->data != NULL)
   {
      /* Switch only cares about chunk->data fields that contain other pointers that
     need to be free */
     
      switch(chunk->tag)
      {
      case MAT_MAPNAME:
      {
     MatMapname *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case BIT_MAP: 
      {
     BitMap *data;
     data = chunk->data;
     free(data->bitmap);
     break;
      }
      case VIEWPORT_DATA_3:
      case VIEWPORT_DATA:
      {
     ViewportData *data;
     data = chunk->data;
     free(data->camname);
     break;
      }
      case VIEW_CAMERA:
      {
     ViewCamera *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case MAT_NAME:
      {
     MatName *data;
     data = chunk->data;
     free(data->name);
     break;
      }
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
      {
     IpasData *data;
     data = chunk->data;
     free(data->data);
     break;
      }
      case NAMED_OBJECT:
      {
     NamedObject *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case DL_EXCLUDE:
      {
     DlExclude *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case POINT_ARRAY:
      {
     PointArray *data;
     data = chunk->data;
     free(data->pointlist);
     break;
      }
      case POINT_FLAG_ARRAY:
      {
     PointFlagArray *data;
     data = chunk->data;
     free(data->flaglist);
     break;
      }
      case FACE_ARRAY:
      {
     FaceArray *data;
     data = chunk->data;
     free(data->facelist);
     break;
      }
      case MSH_MAT_GROUP:
      {
     MshMatGroup *data;
     data = chunk->data;
     free(data->facelist);
     free(data->matname);
     break;
      }
      case MSH_BOXMAP:
      {
     MshBoxmap *data;
     ushort3ds i;
    
     data = chunk->data;
     for (i = 0; i < 6; i++)
     {
        free(data->matnames[i]);
     }
     break;
      }
      case SMOOTH_GROUP:
      {
     SmoothGroup *data;
     data = chunk->data;
     free(data->grouplist);
     break;
      }
      case TEX_VERTS:
      {
     TexVerts *data;
     data = chunk->data;
     free(data->textvertlist);
     break;
      }
      case PROC_NAME:
      {
     ProcName *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case DL_SPOT_PROJECTOR:
      {
     DlSpotProjector *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case XDATA_ENTRY:
      {
         XDataEntry *data = chunk->data;
         free(data->data);
         break;
      }
      case XDATA_APPNAME:
      {
     XDataAppName *data = chunk->data;
     free(data->name);
     break;
      }
      case XDATA_STRING:
      {
     XDataString *data = chunk->data;
     free(data->string);
     break;
      }
      case KFHDR:
      {
     KFHdr *data;
     data = chunk->data;
     free(data->filename);
     break;
      }
      case NODE_HDR: 
      {
     NodeHdr *data;
     data = chunk->data;
     free(data->objname);
     break;
      }
      case PARENT_NAME:
      {
     InstanceName *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case INSTANCE_NAME:
      {
     InstanceName *data;
     data = chunk->data;
     free(data->name);
     break;
      }
      case POS_TRACK_TAG:
      {
     PosTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->positionlist);
     break;
      }
      case COL_TRACK_TAG:
      {
     ColTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->colorlist);
     break;
      }
      case ROT_TRACK_TAG:
      {
     RotTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->rotationlist);
     break;
      }
      case SCL_TRACK_TAG:
      {
     ScaleTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->scalelist);
     break;
      }
      case MORPH_TRACK_TAG:
      {
     MorphTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->morphlist);
     break;
      }
      case FOV_TRACK_TAG:
      {
     FovTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->fovanglelist);
     break;
      }
      case ROLL_TRACK_TAG:
      {
     RollTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->rollanglelist);
     break;
      }
      case HOT_TRACK_TAG:
      {
     HotTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->hotspotanglelist);
     break;
      }
      case FALL_TRACK_TAG:
      {
     FallTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     free(data->falloffanglelist);
     break;
      }
      case HIDE_TRACK_TAG:
      {
     HideTrackTag *data;
     data = chunk->data;
     free(data->keyhdrlist);
     break;
      }
      default:
      {
     break;
      }
      } /* End case */
      
      free(chunk->data);
      chunk->data = NULL;
   }
   
}

/* Computes a chunk weighting used to determine proper chunk order,
   higher values appear earlier in the parent than lower values */
long3ds GetChunkValue3ds(chunktag3ds tag)
{
   long3ds value = 0;

   /* Only chunks where an explicit order matters are in the case */
   
   switch(tag)
   {
   case NULL_CHUNK: value++;  /* These should just be ignored */   
   case SMAGIC: value++;
   case LMAGIC: value++;
   case M3DMAGIC: value++;
   case M3D_VERSION: value++;
   case MDATA: value++;
   case KFDATA: value++;
   case COLOR_24: value++;
   case LIN_COLOR_24: value++;
   case MESH_VERSION: value++;
   case MAT_ENTRY: value++; 
   case KFHDR: value++;
   case MASTER_SCALE: value++;
   case VIEWPORT_LAYOUT: value++;
   case LO_SHADOW_BIAS: value++;
   case SHADOW_MAP_SIZE: value++;
   case SHADOW_FILTER: value++;
   case RAY_BIAS: value++;
   case O_CONSTS: value++;
   case AMBIENT_LIGHT: value++;
   case SOLID_BGND: value++;
   case BIT_MAP: value++;
   case V_GRADIENT: value++;
   case USE_BIT_MAP:
   case USE_SOLID_BGND:
   case USE_V_GRADIENT: value++;
   case FOG: value++;
   case LAYER_FOG: value++;
   case DISTANCE_CUE: value++;
   case DEFAULT_VIEW: value++;
   case NAMED_OBJECT: value++;
   case KFSEG: value++;
   case KFCURTIME: value++;
   case TARGET_NODE_TAG:
   case L_TARGET_NODE_TAG:
   case OBJECT_NODE_TAG:
   case CAMERA_NODE_TAG:
   case SPOTLIGHT_NODE_TAG: value++;
   case AMBIENT_NODE_TAG: value++;
   case N_TRI_OBJECT:
   case N_CAMERA:
   case N_DIRECT_LIGHT: value++;
   case OBJ_HIDDEN: value++;
      break;
   default: value = 0;
   }
   return value;
}

