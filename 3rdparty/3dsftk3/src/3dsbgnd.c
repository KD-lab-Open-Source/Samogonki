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


/* Inits, frees, puts and gets the background3ds structure which provides an */
/* interface to the 3D Studio scene background settings. */

#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsbgnd.h"
#include "3dsftkst.h"
#include "chunk3ds.h"
#include "dbase3ds.h"

/* Initializes the background3ds structure, if **bgnd is an address to */
/* a null pointer, then memory for the struture will be allocated at */
/* this time */
void InitBackground3ds(background3ds **bgnd)
{
  if (bgnd == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

   /* if the pointer is NULL, then allocated the structure */
   if (*bgnd == NULL)
   {
      *bgnd = malloc(sizeof(background3ds));

      if(*bgnd == NULL) 
	SET_ERROR_RETURN(ERR_NO_MEM);
   }

   (*bgnd)->bitmap.name = NULL;
   (*bgnd)->solid.color.r = (*bgnd)->solid.color.g = (*bgnd)->solid.color.b = 0.0f;
   (*bgnd)->vgradient.gradpercent = 0.5f;
   (*bgnd)->vgradient.top.r = (*bgnd)->vgradient.top.g = (*bgnd)->vgradient.top.b = 0.0f;
   (*bgnd)->vgradient.mid.r = (*bgnd)->vgradient.mid.g = (*bgnd)->vgradient.mid.b = 0.0f;
   (*bgnd)->vgradient.bottom.r = (*bgnd)->vgradient.bottom.g = (*bgnd)->vgradient.bottom.b = 0.0f;
   (*bgnd)->bgndused = NoBgnd;
}

void ReleaseBackground3ds(background3ds **bgnd)
{
   if (bgnd != NULL)
   {
      if ((*bgnd)->bitmap.name != NULL) 
      {
	 free((*bgnd)->bitmap.name);
	 (*bgnd)->bitmap.name = NULL;
      }
      
      free(*bgnd);
      *bgnd = NULL;
   }
   
}

/* Retreaves the background settings from the chunk database */
void GetBackground3ds(database3ds *db, /* the database being searched */
		      background3ds **bgnd /* the background settings
					      destination.  If *atmo is NULL,
					      then a new structure will be
					      allocated */
		      )
{
   chunk3ds *mdata, *chunk;
   
   if (db == NULL || bgnd == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

   /* Find the MDATA chunk */
   FindChunk3ds(db->topchunk, MDATA, &mdata);

   /* only continue with structure filling if an MDATA chunk is found */
   if (mdata != NULL)
   {
      InitBackground3ds(bgnd);

      /* Search for bitmap chunk */
      FindChunk3ds(mdata, BIT_MAP, &chunk);
      if (chunk != NULL)
      {
	 BitMap *bitmap;

	 /* Read the chunk information */
	 ReadChunkData3ds(chunk);

	 /* Alias the data */
	 bitmap = chunk->data;

	 /* Copy the bitmap filename to the structure */
	 if (bitmap->bitmap != NULL) (*bgnd)->bitmap.name = strdup(bitmap->bitmap);
	 else (*bgnd)->bitmap.name = strdup("");
      }

      FindChunk3ds(mdata, SOLID_BGND, &chunk);
      if (chunk != NULL)
      {
	 chunk3ds *cc;

	 FindChunk3ds(chunk, COLOR_F, &cc);
	 if (cc != NULL)
	 {
	    ColorF *cd;
	    
	    cd = ReadChunkData3ds(cc);
	    (*bgnd)->solid.color.r = cd->red;
	    (*bgnd)->solid.color.g = cd->green;
	    (*bgnd)->solid.color.b = cd->blue;
	 }

	 FindChunk3ds(chunk, LIN_COLOR_F, &cc);
	 if (cc != NULL)
	 {
	    LinColorF *cd;
	    
	    cd = ReadChunkData3ds(cc);
	    (*bgnd)->solid.color.r = cd->red;
	    (*bgnd)->solid.color.g = cd->green;
	    (*bgnd)->solid.color.b = cd->blue;
	 }
      }

      FindChunk3ds(mdata, V_GRADIENT, &chunk);
      if (chunk != NULL)
      {
	 chunk3ds *topcolor, *midcolor, *botcolor;

	 /* The COLOR_F chunks are the old, non-gamma corrected colors */
	 ReadChunkData3ds(chunk);
	 (*bgnd)->vgradient.gradpercent = ((VGradient *)(chunk->data))->gradpercent;
	 FindChunk3ds(chunk, COLOR_F, &topcolor);
	 if (topcolor != NULL)
	 {
	    ReadChunkData3ds(topcolor);
	    (*bgnd)->vgradient.top.r = ((ColorF *)(topcolor->data))->red;
	    (*bgnd)->vgradient.top.g = ((ColorF *)(topcolor->data))->green;
	    (*bgnd)->vgradient.top.b = ((ColorF *)(topcolor->data))->blue;
	    FindNextChunk3ds(topcolor->sibling, COLOR_F, &midcolor);
	    if (midcolor != NULL)
	    {
	       ReadChunkData3ds(midcolor);
	       (*bgnd)->vgradient.mid.r = ((ColorF *)(midcolor->data))->red;
	       (*bgnd)->vgradient.mid.g = ((ColorF *)(midcolor->data))->green;
	       (*bgnd)->vgradient.mid.b = ((ColorF *)(midcolor->data))->blue;
	       FindNextChunk3ds(midcolor->sibling, COLOR_F, &botcolor);
	       if (botcolor != NULL)
	       {
		  ReadChunkData3ds(botcolor);
		  (*bgnd)->vgradient.bottom.r = ((ColorF *)(botcolor->data))->red;
		  (*bgnd)->vgradient.bottom.g = ((ColorF *)(botcolor->data))->green;
		  (*bgnd)->vgradient.bottom.b = ((ColorF *)(botcolor->data))->blue;
	       }
	    }
	 }

	 /* If the newer, gamma correct colors are available, then use them instead */
	 FindChunk3ds(chunk, LIN_COLOR_F, &topcolor);
	 if (topcolor != NULL)
	 {
	    ReadChunkData3ds(topcolor);
	    (*bgnd)->vgradient.top.r = ((ColorF *)(topcolor->data))->red;
	    (*bgnd)->vgradient.top.g = ((ColorF *)(topcolor->data))->green;
	    (*bgnd)->vgradient.top.b = ((ColorF *)(topcolor->data))->blue;
	    FindNextChunk3ds(topcolor->sibling, LIN_COLOR_F, &midcolor);
	    if (midcolor != NULL)
	    {
	       ReadChunkData3ds(midcolor);
	       (*bgnd)->vgradient.mid.r = ((ColorF *)(midcolor->data))->red;
	       (*bgnd)->vgradient.mid.g = ((ColorF *)(midcolor->data))->green;
	       (*bgnd)->vgradient.mid.b = ((ColorF *)(midcolor->data))->blue;
	       FindNextChunk3ds(midcolor->sibling, LIN_COLOR_F, &botcolor);
	       if (botcolor != NULL)
	       {
		  ReadChunkData3ds(botcolor);
		  (*bgnd)->vgradient.bottom.r = ((ColorF *)(botcolor->data))->red;
		  (*bgnd)->vgradient.bottom.g = ((ColorF *)(botcolor->data))->green;
		  (*bgnd)->vgradient.bottom.b = ((ColorF *)(botcolor->data))->blue;
	       }
	    }
	 }
      }
      
      /* Search for use_bitmap, use_solid_bgnd, or use_v_gradient */
      FindChunk3ds(mdata, USE_BIT_MAP, &chunk);
      if (chunk != NULL)
      {
	 (*bgnd)->bgndused = UseBitmapBgnd;
      } else
      {
	 FindChunk3ds(mdata, USE_SOLID_BGND, &chunk);
	 if (chunk != NULL)
	 {
	    (*bgnd)->bgndused = UseSolidBgnd;
	 } else
	 {
	    FindChunk3ds(mdata, USE_V_GRADIENT, &chunk);
	    if (chunk != NULL)
	    {
	       (*bgnd)->bgndused = UseVGradientBgnd;
	    } else
	    {
	       (*bgnd)->bgndused = NoBgnd;
	    }
	 }
      }
   }
}

void PutBackground3ds(database3ds *db, background3ds *bgnd)
{
   chunk3ds *mdata;

       if(db == NULL || bgnd == NULL)
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

   /* if (1) */ /* Put bitmap background chunk. */
   {
      chunk3ds *c;
      BitMap *d;

      ReplaceOrAddChild3ds(mdata, BIT_MAP, &c);
      d = InitChunkData3ds(c);
      d->bitmap = (bgnd->bitmap.name)? strdup(bgnd->bitmap.name): strdup("");
   }

   /* if (1) */ /* Put the SOLID_BGND chunk */
   {
      chunk3ds *cs, *cc;
      ColorF *dc;

      ReplaceOrAddChild3ds(mdata, SOLID_BGND, &cs);

      dc = InitChunkAndData3ds(&cc, COLOR_F);
      
      dc->red = bgnd->solid.color.r;
      dc->green = bgnd->solid.color.g;
      dc->blue = bgnd->solid.color.b;
      
      AddChild3ds(cs, cc);

      dc = InitChunkAndData3ds(&cc, LIN_COLOR_F);
      
      dc->red = bgnd->solid.color.r;
      dc->green = bgnd->solid.color.g;
      dc->blue = bgnd->solid.color.b;
      
      AddChild3ds(cs, cc);
   }

   /* if (1) */ /* Put the V_GRADIENT chunk */
   {
      chunk3ds *cv, *cc;
      VGradient *dv;
      ColorF *dc;

      ReplaceOrAddChild3ds(mdata, V_GRADIENT, &cv);
      dv = InitChunkData3ds(cv);
      dv->gradpercent = bgnd->vgradient.gradpercent;

      dc = InitChunkAndData3ds(&cc, COLOR_F);
      dc->red = bgnd->vgradient.top.r;
      dc->green = bgnd->vgradient.top.g;
      dc->blue = bgnd->vgradient.top.b;
      
      AddChild3ds(cv, cc);

      dc = InitChunkAndData3ds(&cc, LIN_COLOR_F);
      dc->red = bgnd->vgradient.top.r;
      dc->green = bgnd->vgradient.top.g;
      dc->blue = bgnd->vgradient.top.b;
      
      AddChild3ds(cv, cc);

      dc = InitChunkAndData3ds(&cc, COLOR_F);
      dc->red = bgnd->vgradient.mid.r;
      dc->green = bgnd->vgradient.mid.g;
      dc->blue = bgnd->vgradient.mid.b;
      
      AddChild3ds(cv, cc);

      dc = InitChunkAndData3ds(&cc, LIN_COLOR_F);
      dc->red = bgnd->vgradient.mid.r;
      dc->green = bgnd->vgradient.mid.g;
      dc->blue = bgnd->vgradient.mid.b;
      
      AddChild3ds(cv, cc);

      dc = InitChunkAndData3ds(&cc, COLOR_F);
      dc->red = bgnd->vgradient.bottom.r;
      dc->green = bgnd->vgradient.bottom.g;
      dc->blue = bgnd->vgradient.bottom.b;
      
      AddChild3ds(cv, cc);

      dc = InitChunkAndData3ds(&cc, LIN_COLOR_F);
      dc->red = bgnd->vgradient.bottom.r;
      dc->green = bgnd->vgradient.bottom.g;
      dc->blue = bgnd->vgradient.bottom.b;
      
      AddChild3ds(cv, cc);

   }
   
   /* if (1) */ /* Add a use_bitmap_bgnd, use_solid_bgnd, or use_v_gradient */
   {
      chunk3ds *chunk;
   
      /* first, find any existing use_* chunks */
      FindChunk3ds(mdata, USE_BIT_MAP, &chunk);
      if (chunk == NULL)
      {
	 FindChunk3ds(mdata, USE_SOLID_BGND, &chunk);
	 if (chunk == NULL)
	 {
	    FindChunk3ds(mdata, USE_V_GRADIENT, &chunk);
	 }
      }
      
      /* If one was found, delete it */
      if (chunk != NULL)
      {
	 DeleteChunk3ds(chunk);
      }
      
      if (bgnd->bgndused != NoBgnd)
      {
	 if (chunk == NULL)
	 {
	    InitChunk3ds(&chunk);
	    chunk->tag = USE_BIT_MAP; /* just to help place in order */
	    AddChildOrdered3ds(mdata,chunk);
	 }
	 
	 /* add the new use_* chunk */
	 switch (bgnd->bgndused)
	 {
	 case UseBitmapBgnd:
	    chunk->tag = USE_BIT_MAP;
	    break;
	 case UseSolidBgnd:
	    chunk->tag = USE_SOLID_BGND;
	    break;
	 case UseVGradientBgnd:
	    chunk->tag = USE_V_GRADIENT;
	    break;
	 case NoBgnd:
	    chunk->tag = NULL_CHUNK;
	    break;
	 }
      }
   }
}

void CopyBackground3ds(database3ds *destdb, database3ds *srcdb)
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

      /* Scan the source mdata section looking for background setting chunks */
      for (srcchunk = srcmdata->children; srcchunk != NULL; srcchunk = srcchunk->sibling)
      {
	 switch(srcchunk->tag)
	 {
	 case V_GRADIENT:
	 case SOLID_BGND:
	 case BIT_MAP:
	 case USE_SOLID_BGND:
	 case USE_V_GRADIENT:
	 case USE_BIT_MAP:
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

   
