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
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsftkst.h"
#include "chunk3ds.h"
#include "dbase3ds.h"
#include "3dsmset.h"
#include "mdata3ds.h"

/* Initializes the mesh settings structure, if the pointer is a NULL pointer, then
   memory will be allocated for the mesh settings structure */
void InitMeshSet3ds(meshset3ds **settings)
{
   if (settings == NULL) 
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* if the structure pointer is NULL, then allocate a new one. */
   if (*settings == NULL)
   {
      *settings = malloc(sizeof(meshset3ds));
      if(*settings == NULL) 
	SET_ERROR_RETURN(ERR_NO_MEM);
   }

   (*settings)->masterscale = 1.0f;
   (*settings)->shadow.bias = 1.0f;
   (*settings)->shadow.raybias = 1.0f;
   (*settings)->shadow.mapsize = 512;
   (*settings)->shadow.filter = 3.0f;
   (*settings)->ambientlight.r = (*settings)->ambientlight.g = (*settings)->ambientlight.b = 0.39216f;
   (*settings)->oconsts.x = (*settings)->oconsts.y = (*settings)->oconsts.z = 0.0f;
}

/* Frees the memory allocated to the pointer, and set the pointer to NULL */
void ReleaseMeshSet3ds(meshset3ds **settings)
{
  if (settings == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

   if (*settings != NULL)
   {
      free(*settings);
      *settings = NULL;
   }
}


/* Retreves the mesh settings from the database */
void GetMeshSet3ds(database3ds *db, /* Database being searched */
		   meshset3ds **settings /* The mesh settings structure, if
					  *settings is NULL, then a new
					  structure will be allocated by the
					  routine */
		   )
{
   chunk3ds *mdata, *chunk;

   if (db == NULL || settings == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

   /* Find the mesh data chunk */
   FindChunk3ds(db->topchunk, MDATA, &mdata);

   /* If the mesh data section is found */
   if (mdata != NULL)
   {
      InitMeshSet3ds(settings);

      /* Search for a master_scale chunk */
      FindNextChunk3ds(mdata->children, MASTER_SCALE, &chunk);
      if (chunk != NULL)
      {
	 MasterScale *scale;

	 ReadChunkData3ds(chunk);
	 scale = chunk->data;
	 (*settings)->masterscale = scale->masterscale;
	 FreeFileChunkData3ds(chunk);
      }

      /* Search for lo_shadow_bias chunk */
      FindNextChunk3ds(mdata->children, LO_SHADOW_BIAS, &chunk);
      if (chunk != NULL)
      {
	 LoShadowBias *bias;

	 ReadChunkData3ds(chunk);
	 bias = chunk->data;
	 (*settings)->shadow.bias = bias->bias;
	 FreeFileChunkData3ds(chunk);
      }
      
      /* Search for ray_bias chunk */
      FindNextChunk3ds(mdata->children, RAY_BIAS, &chunk);
      if (chunk != NULL)
      {
	 RayBias *bias;

	 ReadChunkData3ds(chunk);
	 bias = chunk->data;
	 (*settings)->shadow.raybias = bias->bias;
	 FreeFileChunkData3ds(chunk);
      }
      
      /* Search for mapsize chunk */
      FindNextChunk3ds(mdata->children, SHADOW_MAP_SIZE, &chunk);
      if (chunk != NULL)
      {
	 ShadowMapSize *size;

	 ReadChunkData3ds(chunk);
	 size = chunk->data;
	 (*settings)->shadow.mapsize = size->shadowmapsize;
	 FreeFileChunkData3ds(chunk);
      }
      
      /* Search for shadow_filter chunk */
      FindNextChunk3ds(mdata->children, SHADOW_FILTER, &chunk);
      if (chunk != NULL)
      {
	 ShadowFilter *filter;
	 ReadChunkData3ds(chunk);
	 filter = chunk->data;
	 (*settings)->shadow.filter = filter->shadowfilter;
	 FreeFileChunkData3ds(chunk);
      }

      /* Search for ambient_light chunk */
      FindNextChunk3ds(mdata->children, AMBIENT_LIGHT, &chunk);
      if (chunk != NULL)
      {
	 chunk3ds *colorchunk;

	 /* Search for the old style color chunk inside the ambient light chunk */
	 FindChunk3ds(chunk, COLOR_F, &colorchunk);
	 if (colorchunk != NULL)
	 {
	    ColorF *color;
	    ReadChunkData3ds(colorchunk);
	    color = colorchunk->data;
	    (*settings)->ambientlight.r = color->red;
	    (*settings)->ambientlight.g = color->green;
	    (*settings)->ambientlight.b = color->blue;
	    FreeFileChunkData3ds(colorchunk);
	 } else 
	 {
	    /* Just for robust completeness, search for the COLOR_24 chunk */
	    FindChunk3ds(chunk, COLOR_24, &colorchunk);
	    if (colorchunk != NULL)
	    {
	       Color24 *color;
	       ReadChunkData3ds(colorchunk);
	       color = colorchunk->data;
	       (*settings)->ambientlight.r = (float3ds)(color->red) / 256.0f;
	       (*settings)->ambientlight.g = (float3ds)(color->green) / 256.0f;
	       (*settings)->ambientlight.b = (float3ds)(color->blue) / 256.0f;
	       FreeFileChunkData3ds(colorchunk);
	    }
	 }

	 /* Search for the newer linear color chunk inside the ambient light chunk */
	 FindChunk3ds(chunk, LIN_COLOR_F, &colorchunk);
	 if (colorchunk != NULL)
	 {
	    LinColorF *color;
	    ReadChunkData3ds(colorchunk);
	    color = colorchunk->data;
	    (*settings)->ambientlight.r = color->red;
	    (*settings)->ambientlight.g = color->green;
	    (*settings)->ambientlight.b = color->blue;
	    FreeFileChunkData3ds(colorchunk);
	 } else
	 {
	    /* Just for completeness, search for the LIN_COLOR_24 chunk */
	    FindChunk3ds(chunk, LIN_COLOR_24, &colorchunk);
	    if (colorchunk != NULL)
	    {
	       LinColor24 *color;
	       ReadChunkData3ds(colorchunk);
	       color = colorchunk->data;
	       (*settings)->ambientlight.r = (float3ds)(color->red) / 256.0f;
	       (*settings)->ambientlight.g = (float3ds)(color->green) / 256.0f;
	       (*settings)->ambientlight.b = (float3ds)(color->blue) / 256.0f;
	       FreeFileChunkData3ds(colorchunk);
	    }
	 }
      }
      
      /* Search for the oconst chunk */
      FindNextChunk3ds(mdata->children, O_CONSTS, &chunk);
      if (chunk != NULL)
      {
	 OConsts *d;
	 d = ReadChunkData3ds(chunk);
	 (*settings)->oconsts.x = d->oconsts.x;
	 (*settings)->oconsts.y = d->oconsts.y;
	 (*settings)->oconsts.z = d->oconsts.z;
	 FreeFileChunkData3ds(chunk);
      }
   }
}   

/* Adds/replaces mesh settings to/in the 3ds chunk database */
void PutMeshSet3ds(database3ds *db, /* chunk database to put the settings in */
		   meshset3ds *settings /* the settings to ad to the database */
		   )
{
   chunk3ds *mdata, *color, *chunk;

    if(db == NULL || settings == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    if(db->topchunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    
    if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Search for a mesh data chunk */
   FindChunk3ds(db->topchunk, MDATA, &mdata);

   /* If one isn't found, add it to the list */
   if (mdata == NULL)
   {
      InitChunkAs3ds(&mdata, MDATA);
      AddChildOrdered3ds(db->topchunk, mdata);
   }

   /* Search for a MESH_VERSION chunk, and replace the existing one or add a new one */
   ReplaceOrAddChild3ds(mdata, MESH_VERSION, &chunk);

   /* Create a new data structure */
   InitChunkData3ds(chunk);

   /* Add the master scale chunk */
   ReplaceOrAddChild3ds(mdata, MASTER_SCALE, &chunk);
   InitChunkData3ds(chunk);
   ((MasterScale *)(chunk->data))->masterscale = settings->masterscale;

   /* Add the lo_shadow_bias chunk */
   ReplaceOrAddChild3ds(mdata, LO_SHADOW_BIAS, &chunk);
   InitChunkData3ds(chunk);
   ((LoShadowBias *)(chunk->data))->bias = settings->shadow.bias;

   /* Add the shadow_map_size chunk */
   ReplaceOrAddChild3ds(mdata, SHADOW_MAP_SIZE, &chunk);
   InitChunkData3ds(chunk);
   ((ShadowMapSize *)(chunk->data))->shadowmapsize = settings->shadow.mapsize;
	    
   /* Add the shadow_filter chunk */
   ReplaceOrAddChild3ds(mdata, SHADOW_FILTER, &chunk);
   InitChunkData3ds(chunk);
   ((ShadowFilter *)(chunk->data))->shadowfilter = settings->shadow.filter;

   /* Add the ray_bias chunk */
   ReplaceOrAddChild3ds(mdata, RAY_BIAS, &chunk);
   InitChunkData3ds(chunk);
   ((RayBias *)(chunk->data))->bias = settings->shadow.raybias;

   /* Add the ambient_light chunk */
   ReplaceOrAddChild3ds(mdata, AMBIENT_LIGHT, &chunk);

   InitChunkAndData3ds(&color, COLOR_F);
   AddChildOrdered3ds(chunk, color);
   ((ColorF *)(color->data))->red = settings->ambientlight.r;
   ((ColorF *)(color->data))->green = settings->ambientlight.g;
   ((ColorF *)(color->data))->blue = settings->ambientlight.b;

   InitChunkAndData3ds(&color, LIN_COLOR_F);
   AddChildOrdered3ds(chunk, color);
   ((LinColorF *)(color->data))->red = settings->ambientlight.r;
   ((LinColorF *)(color->data))->green = settings->ambientlight.g;
   ((LinColorF *)(color->data))->blue = settings->ambientlight.b;

   /* Add the oconst chunk */
   ReplaceOrAddChild3ds(mdata, O_CONSTS, &chunk);
   InitChunkData3ds(chunk);
   ((OConsts *)(chunk->data))->oconsts.x = settings->oconsts.x;
   ((OConsts *)(chunk->data))->oconsts.y = settings->oconsts.y;
   ((OConsts *)(chunk->data))->oconsts.z = settings->oconsts.z;


}

void CopyMeshSet3ds(database3ds *destdb, database3ds *srcdb)
{
   chunk3ds *srcmdata, *destmdata, *srcchunk, *destchunk;
   
   if ((destdb == NULL) || (srcdb == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find source mesh section */
   FindNextChunk3ds(srcdb->topchunk->children, MDATA, &srcmdata);

   /* If found */
   if (srcmdata != NULL)
   {
      /* Find or create destination mesh section */
      FindChunk3ds(destdb->topchunk, MDATA, &destmdata);
      if (destmdata == NULL)
      {
	 InitChunkAs3ds(&destmdata, MDATA);
	 AddChildOrdered3ds(destdb->topchunk, destmdata);
      }

      /* Scan the source mdata section looking for mesh setting chunks */
      for (srcchunk = srcmdata->children; srcchunk != NULL; srcchunk = srcchunk->sibling)
      {
	 switch(srcchunk->tag)
	 {
	 case MESH_VERSION:
	 case MASTER_SCALE:
	 case LO_SHADOW_BIAS:
	 case SHADOW_MAP_SIZE:
	 case SHADOW_FILTER:
	 case RAY_BIAS:
	 case O_CONSTS:
	    /* if the same chunk exists in the destination, then delete it */
	    FindNextChunk3ds(destmdata->children, srcchunk->tag, &destchunk);
	    if (destchunk != NULL)
	       DeleteChunk3ds(destchunk);

	    /* Copy and add the chunk */
	    CopyChunk3ds(srcchunk, &destchunk);
	    ON_ERROR_RETURN;
	    
	    AddChildOrdered3ds(destmdata, destchunk);
	    break;
	 default:
	    break;
	 }
      }
   }
}

	    
