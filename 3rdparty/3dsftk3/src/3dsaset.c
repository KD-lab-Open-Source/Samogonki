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


/* 3dsaset.c, 3dsaset.h

   Inits, frees, puts and gets the atmosphere3ds structure which provides
   and interface to the 3ds astmostpheric effects depth cue, fog, and
   layer fog (DISTANCE_CUE, FOG, and LAYER_FOG chunks.
*/

#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsftkst.h"
#include "chunk3ds.h"
#include "dbase3ds.h"
#include "3dsaset.h"

/* Initializes the atmosphere3ds structure, if **env is an address to a null pointer,
   then memory for the structure will also be allocated */
void InitAtmosphere3ds(atmosphere3ds **atmo)
{
  if (atmo == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

   /* if the pointer is a null, then allocate the structure */
   if (*atmo == NULL)
   {
      *atmo = malloc(sizeof(atmosphere3ds));
      if(*atmo == NULL) 
	SET_ERROR_RETURN(ERR_NO_MEM);
   }

   (*atmo)->fog.nearplane = 0.0f;
   (*atmo)->fog.neardensity = 0.0f;
   (*atmo)->fog.farplane = 1000.0f;
   (*atmo)->fog.fardensity = 100.0f;
   (*atmo)->fog.fogcolor.r = (*atmo)->fog.fogcolor.g = (*atmo)->fog.fogcolor.b = 0.0f;
   (*atmo)->fog.fogbgnd = True3ds;

   (*atmo)->layerfog.zmin = 0.0f;
   (*atmo)->layerfog.zmax = 100.0f;
   (*atmo)->layerfog.density = 50.0f;
   (*atmo)->layerfog.fogcolor.r = (*atmo)->layerfog.fogcolor.g = (*atmo)->layerfog.fogcolor.b = 0.0f;
   (*atmo)->layerfog.falloff = NoFall;
   (*atmo)->layerfog.fogbgnd = True3ds;

   (*atmo)->dcue.nearplane = 0.0f;
   (*atmo)->dcue.neardim = 0.0f;
   (*atmo)->dcue.farplane = 1000.0f;
   (*atmo)->dcue.fardim = 100.0f;

   (*atmo)->activeatmo = NoAtmo;
}

void ReleaseAtmosphere3ds(atmosphere3ds **atmo)
{
   if(atmo == NULL) 
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (*atmo != NULL)
   {
      free(*atmo);
      *atmo = NULL;
   }
}

/* Retreves the atmospheric settings from chunk database */
void GetAtmosphere3ds(database3ds *db, /* the database being searched */
		      atmosphere3ds **atmo  /* the atmosphere settings
      					       destination.  If *atmo is NULL,
      					       then a new structure will be
      					       allocated */
		      )
{
   chunk3ds *mdata, *chunk;
   
   if (db == NULL || atmo == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

   /* find the MDATA chunk */
   FindChunk3ds(db->topchunk, MDATA, &mdata);

   /* if the MDATA chunk was found, then search for the atmospheric chunks */
   if (mdata != NULL)
   {
      InitAtmosphere3ds(atmo);

      /* Search for fog chunk */
      FindChunk3ds(mdata, FOG, &chunk);
      if (chunk != NULL)
      {
	 Fog *fog;
	 chunk3ds *color, *bgnd;

	 /* Read the chunk information */
	 fog = ReadChunkData3ds(chunk);

	 /* Copy the fog data into the structure */
	 (*atmo)->fog.nearplane = fog->nearplanedist;
	 (*atmo)->fog.neardensity = fog->nearplanedensity;
	 (*atmo)->fog.farplane = fog->farplanedist;
	 (*atmo)->fog.fardensity = fog->farplanedensity;

	 /* Search for fog color chunk */
	 FindChunk3ds(chunk, COLOR_F, &color);
	 if (color != NULL)
	 {
	    ColorF *c;

	    c = ReadChunkData3ds(color);

	    (*atmo)->fog.fogcolor.r = c->red;
	    (*atmo)->fog.fogcolor.g = c->green;
	    (*atmo)->fog.fogcolor.b = c->blue;
	 }

	 /* Search for fog_bgnd chunk */
	 FindChunk3ds(chunk, FOG_BGND, &bgnd);
	 if (bgnd != NULL)
	 {
	    (*atmo)->fog.fogbgnd = True3ds;
	 } else
	 {
	    (*atmo)->fog.fogbgnd = False3ds;
	 }

      }

      /* search for layer_fog chunk */
      FindChunk3ds(mdata, LAYER_FOG, &chunk);
      if (chunk != NULL)
      {
	 LayerFog *fog;
	 chunk3ds *color;

	 fog = ReadChunkData3ds(chunk);

	 (*atmo)->layerfog.zmin = fog->zmin;
	 (*atmo)->layerfog.zmax = fog->zmax;
	 (*atmo)->layerfog.density = fog->density;

	 if ((fog->type & LayerFogBgnd) == LayerFogBgnd)
	    (*atmo)->layerfog.fogbgnd = True3ds;
	 else
	    (*atmo)->layerfog.fogbgnd = False3ds;

	 if ((fog->type & TopFalloff) == TopFalloff)
	    (*atmo)->layerfog.falloff = TopFall;
	 else {
	    if ((fog->type & BottomFalloff) == BottomFalloff)
	       (*atmo)->layerfog.falloff = BottomFall;
	    else
	       (*atmo)->layerfog.falloff = NoFall;
	 }

	 FindChunk3ds(chunk, COLOR_F, &color);
	 if (color != NULL)
	 {
	    ColorF *c;

	    c = ReadChunkData3ds(color);

	    (*atmo)->layerfog.fogcolor.r = c->red;
	    (*atmo)->layerfog.fogcolor.g = c->green;
	    (*atmo)->layerfog.fogcolor.b = c->blue;
	 }
      }

      /* Search for distance_cue chunk */
      FindChunk3ds(mdata, DISTANCE_CUE, &chunk);
      if (chunk != NULL)
      {
	 DistanceCue *dcue;
	 chunk3ds *bgnd;

	 dcue = ReadChunkData3ds(chunk);

	 (*atmo)->dcue.nearplane = dcue->nearplanedist;
	 (*atmo)->dcue.neardim = dcue->nearplanedimming;
	 (*atmo)->dcue.farplane = dcue->farplanedist;
	 (*atmo)->dcue.fardim = dcue->farplanedimming;

	 FindChunk3ds(chunk, DCUE_BGND, &bgnd);
	 if (bgnd != NULL)
	 {
	    (*atmo)->dcue.dcuebgnd = True3ds;
	 } else
	 {
	    (*atmo)->dcue.dcuebgnd = False3ds;
	 }
	 
      }

      /* Search for use_fog, use_layer_fog, or use_distance_cue */
      FindChunk3ds(mdata, USE_FOG, &chunk);
      if (chunk != NULL)
      {
	 (*atmo)->activeatmo = UseFog;
      } else
      {
	 FindChunk3ds(mdata, USE_LAYER_FOG, &chunk);
	 if (chunk != NULL)
	 {
	    (*atmo)->activeatmo = UseLayerFog;
	 } else
	 {
	    FindChunk3ds(mdata, USE_DISTANCE_CUE, &chunk);
	    if (chunk != NULL)
	    {
	       (*atmo)->activeatmo = UseDistanceCue;
	    } else
	    {
	       (*atmo)->activeatmo = NoAtmo;
	    }
	 }
      }
   }
}

/* Adds/replaces atmosphere settings in the chunk database */
void PutAtmosphere3ds(database3ds *db, /* Database being modified */
		      atmosphere3ds *atmo /* The atmosphere settings being added */
		      )
{
   chunk3ds *mdata, *color, *chunk, *bgnd;
   Fog *fog;
   LayerFog *layerfog;
   DistanceCue *dcue;
   ColorF *c;
   
   if (db == NULL || atmo == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

    if(db->topchunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    
    if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Search for a mesh data chunk */
   FindChunk3ds(db->topchunk, MDATA, &mdata);

   /* If one isn't found, then add a new one to the database */
   if (mdata == NULL)
   {
      InitChunk3ds(&mdata);
      mdata->tag = MDATA;
      AddChildOrdered3ds(db->topchunk, mdata);
   }

   /* Search for a FOG chunk, and replace the existing one or add a new one */
   ReplaceOrAddChild3ds(mdata, FOG, &chunk);
   
   /* Create a new data structure */
   InitChunkData3ds(chunk);

   /* Type safe the void pointer */
   fog = chunk->data;

   fog->nearplanedist = atmo->fog.nearplane;
   fog->nearplanedensity = atmo->fog.neardensity;
   fog->farplanedist = atmo->fog.farplane;
   fog->farplanedensity = atmo->fog.fardensity;

   /* Add color chunk */
   InitChunk3ds(&color);
   color->tag = COLOR_F;
   InitChunkData3ds(color);

   c = color->data;
   c->red = atmo->fog.fogcolor.r;
   c->green = atmo->fog.fogcolor.g;
   c->blue = atmo->fog.fogcolor.b;
   
   AddChildOrdered3ds(chunk, color);

   if (atmo->fog.fogbgnd)
   {
      InitChunk3ds(&bgnd);
      bgnd->tag = FOG_BGND;
      AddChildOrdered3ds(chunk, bgnd);
   }
   
   /* Add a layer_fog chunk */
   ReplaceOrAddChild3ds(mdata, LAYER_FOG, &chunk);
   InitChunkData3ds(chunk);
   layerfog = chunk->data;

   layerfog->zmin = atmo->layerfog.zmin;
   layerfog->zmax = atmo->layerfog.zmax;
   layerfog->density = atmo->layerfog.density;
   switch(atmo->layerfog.falloff)
   {
   case NoFall:
      layerfog->type = NoFalloff;
      break;
   case TopFall:
      layerfog->type = TopFalloff;
      break;
   case BottomFall:
      layerfog->type = BottomFalloff;
   }

   if (atmo->layerfog.fogbgnd)
      layerfog->type |= LayerFogBgnd;
      
   InitChunk3ds(&color);
   color->tag = COLOR_F;
   InitChunkData3ds(color);

   c = color->data;
   c->red = atmo->layerfog.fogcolor.r;
   c->green = atmo->layerfog.fogcolor.g;
   c->blue = atmo->layerfog.fogcolor.b;

   AddChildOrdered3ds(chunk, color);
   
   /* Add a distance_cue chunk */
   ReplaceOrAddChild3ds(mdata, DISTANCE_CUE, &chunk);
   InitChunkData3ds(chunk);
   dcue = chunk->data;

   dcue->nearplanedist = atmo->dcue.nearplane;
   dcue->nearplanedimming = atmo->dcue.neardim;
   dcue->farplanedist = atmo->dcue.farplane;
   dcue->farplanedimming = atmo->dcue.fardim;

   if (atmo->dcue.dcuebgnd)
   {
      InitChunk3ds(&bgnd);
      bgnd->tag = DCUE_BGND;
      AddChildOrdered3ds(chunk, bgnd);
   }

   /* Add a use_fog, use_layer_fog, or use_distance_cue */

   /* first, find any existing use_* chunks */
   FindChunk3ds(mdata, USE_FOG, &chunk);
   if (chunk == NULL)
   {
      FindChunk3ds(mdata, USE_LAYER_FOG, &chunk);
      if (chunk == NULL)
      {
	 FindChunk3ds(mdata, USE_DISTANCE_CUE, &chunk);
      }
   }

   /* If one was found, delete it */
   if (chunk != NULL)
   {
      DeleteChunk3ds(chunk);
   }

   if (atmo->activeatmo != NoAtmo)
   {
      if (chunk == NULL)
      {
	 InitChunk3ds(&chunk);
	 chunk->tag = USE_FOG; /* just to help place in order */
	 AddChildOrdered3ds(mdata,chunk);
      }

      /* add the new use_* chunk */
      switch (atmo->activeatmo)
      {
      case UseFog:
	 chunk->tag = USE_FOG;
	 break;
      case UseLayerFog:
	 chunk->tag = USE_LAYER_FOG;
	 break;
      case UseDistanceCue:
	 chunk->tag = USE_DISTANCE_CUE;
	 break;
      case NoAtmo:
	 chunk->tag = NULL_CHUNK;
	 break;
      }
   }
}

void CopyAtmosphere3ds(database3ds *destdb, database3ds *srcdb)
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
   FindChunk3ds(srcdb->topchunk, MDATA, &srcmdata);

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
	 case FOG:
	 case LAYER_FOG:
	 case DISTANCE_CUE:
	 case USE_FOG:
	 case USE_LAYER_FOG:
	 case USE_DISTANCE_CUE:
	    /* if the same chunk exists in the destination, then delete it */
	    FindNextChunk3ds(destmdata->children, srcchunk->tag, &destchunk);
	    if (destchunk != NULL)
	       DeleteChunk3ds(destchunk);

	    /* Copy and add the chunk */
	    CopyChunk3ds(srcchunk, &destchunk);
	    AddChildOrdered3ds(destmdata, destchunk);
	    break;
	 default:
	    break;
	 }
      }
   }
}

