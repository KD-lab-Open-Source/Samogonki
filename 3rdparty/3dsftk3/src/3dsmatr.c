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
#include <assert.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "3dserr.h"
#include "3dsmatr.h"
#include "chunk3ds.h"
#include "3dsftkst.h"

void InitBitmap3ds(bitmap3ds *map)
{
   if (map == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

   map->name[0] = 0;
   map->percent = 0.0F;
   map->tiling = Tile;
   map->ignorealpha = False3ds;
   map->filter = Pyramidal;
   map->blur = 0.0F;
   map->mirror = False3ds;
   map->negative = False3ds;
   map->uscale = 1.0F;
   map->vscale = 1.0F;
   map->uoffset = 0.0F;
   map->voffset = 0.0F;
   map->rotation = 0.0F;
   map->source = RGB;
   map->tint1.r = map->tint1.g = map->tint1.b = 0.0F;
   map->tint2.r = map->tint2.g = map->tint2.b = 1.0F;
   map->redtint.r = 1.0F;
   map->redtint.g = map->redtint.b = 0.0F;
   map->greentint.g = 1.0F;
   map->greentint.r = map->greentint.b = 0.0F;
   map->bluetint.b = 1.0F;
   map->bluetint.g = map->bluetint.r = 0.0F;
   map->data = NULL;
   map->datasize = 0;
}

   
      

short3ds InitMaterial3ds(material3ds **mat)
{
   if (*mat == NULL)
   {
      *mat = malloc(sizeof(material3ds));
      assert(*mat != NULL);

   }

   (*mat)->name[0] = 0;
   (*mat)->ambient.r = (*mat)->ambient.g = (*mat)->ambient.b = 0.0F;
   (*mat)->diffuse.r = (*mat)->diffuse.g = (*mat)->diffuse.b = 0.0F;
   (*mat)->specular.r = (*mat)->specular.g = (*mat)->specular.b = 0.0F;
   (*mat)->shininess = 0.0F;
   (*mat)->shinstrength = 0.0F;
   (*mat)->blur = 0.0F;
   (*mat)->transparency = 0.0F;
   (*mat)->transfalloff = 0.0F;
   (*mat)->selfillumpct = 0.0F;
   (*mat)->wiresize = 1.0F;
   (*mat)->shading = Phong;
   (*mat)->useblur = False3ds;
   (*mat)->soften = False3ds;
   (*mat)->usefall = False3ds;
   (*mat)->twosided = False3ds;
   (*mat)->selfillum = False3ds;
   (*mat)->additive = False3ds;
   (*mat)->reflect.useauto = False3ds;
   (*mat)->usewire = False3ds;
   (*mat)->usewireabs = False3ds;
   (*mat)->facemap = False3ds;
   (*mat)->reflect.automap.firstframe = False3ds;
   (*mat)->reflect.automap.flat = False3ds;
   (*mat)->reflect.automap.size = 100;
   (*mat)->reflect.automap.nthframe = 1;
   InitBitmap3ds(&((*mat)->texture.map));
   InitBitmap3ds(&((*mat)->texture.mask));
   InitBitmap3ds(&((*mat)->texture2.map));
   InitBitmap3ds(&((*mat)->texture2.mask));
   InitBitmap3ds(&((*mat)->opacity.map));
   InitBitmap3ds(&((*mat)->opacity.mask));
   InitBitmap3ds(&((*mat)->reflect.map));
   InitBitmap3ds(&((*mat)->reflect.mask));
   InitBitmap3ds(&((*mat)->bump.map));
   InitBitmap3ds(&((*mat)->bump.mask));
   InitBitmap3ds(&((*mat)->specmap.map));
   InitBitmap3ds(&((*mat)->specmap.mask));
   InitBitmap3ds(&((*mat)->shinmap.map));
   InitBitmap3ds(&((*mat)->shinmap.mask));
   InitBitmap3ds(&((*mat)->illummap.map));
   InitBitmap3ds(&((*mat)->illummap.mask));

   return 0;
}

void ReleaseMaterial3ds(material3ds **mat)
{
   if (mat != NULL)
   {
      if (*mat != NULL)
      {
	      free((*mat)->texture.map.data);
	      free((*mat)->texture.mask.data);
	      free((*mat)->texture2.map.data);
	      free((*mat)->texture2.mask.data);
	      free((*mat)->opacity.map.data);
	      free((*mat)->opacity.mask.data);
	      free((*mat)->reflect.mask.data);
	      free((*mat)->bump.map.data);
	      free((*mat)->bump.mask.data);
	      free((*mat)->specmap.map.data);
	      free((*mat)->specmap.mask.data);
	      free((*mat)->shinmap.map.data);
	      free((*mat)->shinmap.mask.data);
	      free((*mat)->illummap.map.data);
	      free((*mat)->illummap.mask.data);
	      free((*mat));
      }
      *mat = NULL;
   }
   
}

ulong3ds GetMaterialCount3ds(database3ds *db)
{
   chunk3ds *current=NULL, *chunk=NULL;
   long3ds matcount=0;

   if (db == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_ARG, 0);

   UpdateMatEntryList3ds(db);

   if (db->matlist == NULL) return 0;
   return db->matlist->count;
}

void GetMaterialNameList3ds(database3ds *db, namelist3ds **list)
{
   ulong3ds  i;

   if (db == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   UpdateMatEntryList3ds(db);

   InitNameList3ds(list, GetMaterialCount3ds(db));

   for (i = 0; i < db->matlist->count; i++)
   {
      (*list)->list[i].name = strdup(db->matlist->list[i].name);
   }
}

void GetBitmapChunk3ds(chunk3ds *chunk, bitmap3ds *bitmap)
{
   chunk3ds *current, *index;
   
   if (chunk == NULL || bitmap == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);     

   for (index = chunk->children; index != NULL; index = index->sibling)
   {
      /* Make a copy of the chunk.  This is done so that the chunk can be
         cleanly junked at the end of the switch statement and its data can be
	 changed and shallow copied without fear.  */
      CopyChunk3ds(index, &current);

      switch(current->tag)
      {
      case INT_PERCENTAGE:
	 ReadChunkData3ds(current);
	 bitmap->percent = ((IntPercentage *)(current->data))->intpercentage / 100.0F;
	 break;

	 	case FLOAT_PERCENTAGE:
			ReadChunkData3ds(current);
			bitmap->percent = ((FloatPercentage *)(current->data))->floatpercentage;
			break;

      case MAT_MAPNAME:
	 ReadChunkData3ds(current);
	 strcpy(bitmap->name, ((MatMapname *)(current->data))->name);
	 break;

      case MAT_MAP_TILING:
      {
	 MatMapTiling *d;
	 ReadChunkData3ds(current);
	 d = current->data;

	 if (d->tiling & TEX_DECAL)
	 {
	    if (d->tiling & TEX_NOWRAP) bitmap->tiling = Decal;
	    else bitmap->tiling = Both;
	 }
	 else bitmap->tiling = Tile;

	 bitmap->ignorealpha = (char3ds)((d->tiling & TEX_DONT_USE_ALPHA) == TEX_DONT_USE_ALPHA);

	 if (d->tiling & TEX_SAT) bitmap->filter = SummedArea;
	 else bitmap->filter = Pyramidal;

	 bitmap->mirror = (char3ds)((d->tiling & TEX_MIRROR) == TEX_MIRROR);
	 bitmap->negative = (char3ds)((d->tiling & TEX_INVERT) == TEX_INVERT);

	 if (d->tiling & TEX_TINT)
	 {
	    if (d->tiling & TEX_ALPHA_SOURCE) bitmap->source = AlphaTint;
	    else bitmap->source = RGBLumaTint;
	 }
	 else
	 {
	    
	    if (d->tiling & TEX_RGB_TINT) bitmap->source = RGBTint;
  	    else if (d->tiling & TEX_ALPHA_SOURCE) bitmap->source = Alpha;
	    else bitmap->source = RGB;
	 }
	 break;
      }
      case MAT_MAP_USCALE:
	 ReadChunkData3ds(current);
	 bitmap->uscale = ((MatMapUScale *)(current->data))->scale;
	 break;

      case MAT_MAP_VSCALE:
	 ReadChunkData3ds(current);
	 bitmap->vscale = ((MatMapVScale *)(current->data))->scale;
	 break;

      case MAT_MAP_UOFFSET:
	 ReadChunkData3ds(current);
	 bitmap->uoffset = ((MatMapUOffset *)(current->data))->offset;
	 break;

      case MAT_MAP_VOFFSET:
	 ReadChunkData3ds(current);
	 bitmap->voffset = ((MatMapVOffset *)(current->data))->offset;
	 break;

      case MAT_MAP_ANG:
	 ReadChunkData3ds(current);
	 bitmap->rotation = ((MatMapAng *)(current->data))->angle;
	 break;

      case MAT_BUMP_PERCENT:
	 /* Value is really stored in material3ds structure */
	 break;

      case MAT_MAP_COL1:
      {
	 MatMapCol1 *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 
	 bitmap->tint1.r = (float3ds)(d->red) / 255.0F;
	 bitmap->tint1.g = (float3ds)(d->green) / 255.0F;
	 bitmap->tint1.b = (float3ds)(d->blue) / 255.0F;
	 break;
      }
	 
      case MAT_MAP_COL2:
      {
	 MatMapCol2 *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 
	 bitmap->tint2.r = (float3ds)(d->red) / 255.0F;
	 bitmap->tint2.g = (float3ds)(d->green) / 255.0F;
	 bitmap->tint2.b = (float3ds)(d->blue) / 255.0F;
	 break;
      }
	 
      case MAT_MAP_RCOL:
      {
	 MatMapRCol *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 
	 bitmap->redtint.r = (float3ds)(d->red) / 255.0F;
	 bitmap->redtint.g = (float3ds)(d->green) / 255.0F;
	 bitmap->redtint.b = (float3ds)(d->blue) / 255.0F;
	 break;
      }
	 
      case MAT_MAP_GCOL:
      {
	 MatMapGCol *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 
	 bitmap->greentint.r = (float3ds)(d->red) / 255.0F;
	 bitmap->greentint.g = (float3ds)(d->green) / 255.0F;
	 bitmap->greentint.b = (float3ds)(d->blue) / 255.0F;
	 break;
      }
	 
      case MAT_MAP_BCOL:
      {
	 MatMapBCol *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 
	 bitmap->bluetint.r = (float3ds)(d->red) / 255.0F;
	 bitmap->bluetint.g = (float3ds)(d->green) / 255.0F;
	 bitmap->bluetint.b = (float3ds)(d->blue) / 255.0F;
	 break;
      }
       case MAT_MAP_TEXBLUR:
	 {
	   MatMapTexblur *d;
	   ReadChunkData3ds(current);
	   d = current->data;
	   
	   bitmap->blur = d->percent;	   /*--- Float percents */
	   break;
	 }	   
       }
      ReleaseChunk3ds(&current);
    }
 }   

/* Converts a bitmap3ds structure into represenative chunks.  The routine expects
   arg *map to be the appropriate "map" chunk. */
void PutBitmapChunk3ds(chunk3ds *map, bitmap3ds *bitmap)
{
   
   if (map == NULL || bitmap == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);     


   /* Check to see if the chunk is used by checking for a bitmap name */
   if (bitmap->name[0] != 0)
   {

      {
	 chunk3ds *c;
	 IntPercentage *d;

	 d = InitChunkAndData3ds(&c, INT_PERCENTAGE);

	 /*--- Round to get closest fit */
	 d->intpercentage = (short3ds)(bitmap->percent * 100.0F + 0.5);

	 AddChildOrdered3ds(map, c);
      }

      {
	 chunk3ds *c;
	 MatName *d;

	 d = InitChunkAndData3ds(&c, MAT_MAPNAME);
	 d->name = strdup(bitmap->name);

	 AddChildOrdered3ds(map, c);
      }


      {
	 chunk3ds *c;
	 MatMapTiling *d;

	 d = InitChunkAndData3ds(&c, MAT_MAP_TILING);
	 
	 /*--- Clear Flag */
	 d->tiling = 0;

	 switch (bitmap->tiling)
	 {
	 case Tile:
	    break;
	 case Decal:
	    d->tiling |= TEX_DECAL | TEX_NOWRAP;
	    break;
	 case Both:
	    d->tiling |= TEX_DECAL;
	 }

	 if (bitmap->ignorealpha) d->tiling |= TEX_DONT_USE_ALPHA;
	 if (bitmap->filter == SummedArea) d->tiling |= TEX_SAT;
	 if (bitmap->mirror) d->tiling |= TEX_MIRROR;
	 if (bitmap->negative) d->tiling |= TEX_INVERT;

	 switch(bitmap->source)
	 {
	 case AlphaTint:
	    d->tiling |= TEX_ALPHA_SOURCE | TEX_TINT;
	    break;
	 case RGBLumaTint:
	    d->tiling |= TEX_TINT;
	    break;
	 case RGBTint:
	    d->tiling |= TEX_RGB_TINT;
	    break;
	 case Alpha:
	    d->tiling |= TEX_ALPHA_SOURCE;
	    break;
	 case RGB:
	    break;
	 }
	 /*--- Note: Perhaps, if d->tiling == 0) bail out from this chunk */
	 AddChildOrdered3ds(map, c);
      }

      /* Add scaling chunks if scaling is not default */
      if ((bitmap->uscale != 1.0F) || (bitmap->vscale != 1.0F))
      {
	 chunk3ds *c;
	 MatMapUScale *d;

	 d = InitChunkAndData3ds(&c, MAT_MAP_USCALE);
	 d->scale = bitmap->uscale;
	 AddChildOrdered3ds(map, c);

	 d = InitChunkAndData3ds(&c, MAT_MAP_VSCALE);
	 d->scale = bitmap->vscale;
	 AddChildOrdered3ds(map, c);
      }

      /* Add offset chunks if offset is not default of 0.0F */
      if ((bitmap->uoffset != 0.0F) || (bitmap->voffset != 0.0F))
      {
	 chunk3ds *c;
	 MatMapUOffset *d;

	 d = InitChunkAndData3ds(&c, MAT_MAP_UOFFSET);
	 d->offset = bitmap->uoffset;
	 AddChildOrdered3ds(map, c);

	 d = InitChunkAndData3ds(&c, MAT_MAP_VOFFSET);
	 d->offset = bitmap->voffset;
	 AddChildOrdered3ds(map, c);
      }

      /* Add rotation chunk if rotation is not default of 0.0F */
      if (bitmap->rotation != 0.0F)
      {
	  chunk3ds *c;
	  MatMapAng *d;
	  
	  d = InitChunkAndData3ds(&c, MAT_MAP_ANG);
	  d->angle = bitmap->rotation;
	  AddChildOrdered3ds(map, c);
	  
       }
	  
      if (bitmap->blur != 0.0F){
	chunk3ds *c;
	MatMapTexblur *d;

	d = InitChunkAndData3ds(&c, MAT_MAP_TEXBLUR);
	d->percent = bitmap->blur;
	AddChildOrdered3ds(map, c);
      }

      /* Add tinting chunks as needed */
      switch (bitmap->source)
      {
      case RGB: /* No tint chunks */
	 break;
      case RGBLumaTint: /* Add MAT_MAP_COL1 and MAT_MAP_COL2 chunks */
      case AlphaTint: 
      {
	 chunk3ds *c;
	 MatMapCol1 *d;

	 d = InitChunkAndData3ds(&c, MAT_MAP_COL1);
	 d->red = (ubyte3ds)(bitmap->tint1.r * 255.0F);
	 d->green = (ubyte3ds)(bitmap->tint1.g * 255.0F);
	 d->blue = (ubyte3ds)(bitmap->tint1.b * 255.0F);
	 
	 AddChildOrdered3ds(map, c);

	 d = InitChunkAndData3ds(&c, MAT_MAP_COL2);
	 d->red = (ubyte3ds)(bitmap->tint2.r * 255.0F);
	 d->green = (ubyte3ds)(bitmap->tint2.g * 255.0F);
	 d->blue = (ubyte3ds)(bitmap->tint2.b * 255.0F);

	 AddChildOrdered3ds(map, c);
	 
	 break;
      }
      case RGBTint: /* Add MAT_MAP_RCOL, MAT_MAP_GCOL, MAT_MAP_BCOL */
      {
	 chunk3ds *c;
	 MatMapRCol *d;

	 d = InitChunkAndData3ds(&c, MAT_MAP_RCOL);
	 d->red = (ubyte3ds)(bitmap->redtint.r * 255.0F);
	 d->green = (ubyte3ds)(bitmap->redtint.g * 255.0F);
	 d->blue = (ubyte3ds)(bitmap->redtint.b * 255.0F);
	 
	 AddChildOrdered3ds(map, c);
	 
	 d = InitChunkAndData3ds(&c, MAT_MAP_GCOL);
	 d->red = (ubyte3ds)(bitmap->greentint.r * 255.0F);
	 d->green = (ubyte3ds)(bitmap->greentint.g * 255.0F);
	 d->blue = (ubyte3ds)(bitmap->greentint.b * 255.0F);
	 
	 AddChildOrdered3ds(map, c);

	 d = InitChunkAndData3ds(&c, MAT_MAP_BCOL);
	 d->red = (ubyte3ds)(bitmap->bluetint.r * 255.0F);
	 d->green = (ubyte3ds)(bitmap->bluetint.g * 255.0F);
	 d->blue = (ubyte3ds)(bitmap->bluetint.b * 255.0F);
	 
	 AddChildOrdered3ds(map, c);
	 break;
      }
      }
   }
}

static float3ds ReadPercentageChunk3ds(chunk3ds *chunk)
{
	IntPercentage *i;
	FloatPercentage *f;
	chunk3ds *datachunk;

	FindChunk3ds(chunk, INT_PERCENTAGE, &datachunk);
	if (datachunk != NULL)
	{
		i = ReadChunkData3ds(datachunk);
		return (float3ds)(i->intpercentage) / 100.0F;
	} else
	{
		FindChunk3ds(chunk, FLOAT_PERCENTAGE, &datachunk);
		if (datachunk != NULL)
		{
			f = ReadChunkData3ds(datachunk);
			return f->floatpercentage;
		}
	}
	return 0.0f;
}
	 
void ReadMatEntryChunk3ds(chunk3ds *chunk, material3ds **mat)
{
   chunk3ds *current, *index,  *datachunk=NULL, *matentry, *color;

   if (chunk == NULL || mat == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);     

   if (chunk->tag != MAT_ENTRY)
     SET_ERROR_RETURN(ERR_INVALID_CHUNK);
   
   matentry = chunk;
   
   InitMaterial3ds(mat);

   for (index = matentry->children; index != NULL; index=index->sibling)
   {
      /* Make a copy of the chunk.  This is done so that the chunk can be
         cleanly junked at the end of the switch statement and its data can be
	 changed and shallow copied without fear.  */
      CopyChunk3ds(index, &current);

      if ((current->tag & 0xFF00) != 0x8000)        /* ignore xdata: prs */
      {                                             /* prs */
      switch (current->tag)
      {
      case MAT_NAME:
	 ReadChunkData3ds(current);
	 strcpy((*mat)->name, ((MatName *)(current->data))->name);
	 break;
      case MAT_AMBIENT:
      {
	 Color24 *d;
	 
	 FindChunk3ds(current, COLOR_24, &color);
	 d = ReadChunkData3ds(color);
	 (*mat)->ambient.r = (float3ds)d->red / 255.0F;
	 (*mat)->ambient.g = (float3ds)d->green / 255.0F;
	 (*mat)->ambient.b = (float3ds)d->blue / 255.0F;

	 FindChunk3ds(current, LIN_COLOR_24, &color);
	 if (color != NULL)
	 {
	    ReadChunkData3ds(color);
	    (*mat)->ambient.r = (float3ds)(((LinColor24 *)(color->data))->red) / 255.0F;
	    (*mat)->ambient.g = (float3ds)(((LinColor24 *)(color->data))->green) / 255.0F;
	    (*mat)->ambient.b = (float3ds)(((LinColor24 *)(color->data))->blue) / 255.0F;
	 }
	 break;
      }
      case MAT_DIFFUSE:
	 FindChunk3ds(current, COLOR_24, &color);
     if (color == NULL)
       SET_ERROR_RETURN(ERR_UNFOUND_CHUNK);
   
	 ReadChunkData3ds(color);
	 (*mat)->diffuse.r = (float3ds)(((Color24 *)(color->data))->red) / 255.0F;
	 (*mat)->diffuse.g = (float3ds)(((Color24 *)(color->data))->green) / 255.0F;
	 (*mat)->diffuse.b = (float3ds)(((Color24 *)(color->data))->blue) / 255.0F;

	 FindChunk3ds(current, LIN_COLOR_24, &color);
	 if (color != NULL)
	 {
	    ReadChunkData3ds(color);
	    (*mat)->diffuse.r = (float3ds)(((LinColor24 *)(color->data))->red) / 255.0F;
	    (*mat)->diffuse.g = (float3ds)(((LinColor24 *)(color->data))->green) / 255.0F;
	    (*mat)->diffuse.b = (float3ds)(((LinColor24 *)(color->data))->blue) / 255.0F;
	 }
	 break;
      case MAT_SPECULAR:
	 FindChunk3ds(current, COLOR_24, &color);
     if (color == NULL)
       SET_ERROR_RETURN(ERR_UNFOUND_CHUNK);
   
	 ReadChunkData3ds(color);
	 (*mat)->specular.r = (float3ds)(((Color24 *)(color->data))->red) / 255.0F;
	 (*mat)->specular.g = (float3ds)(((Color24 *)(color->data))->green) / 255.0F;
	 (*mat)->specular.b = (float3ds)(((Color24 *)(color->data))->blue) / 255.0F;

	 FindChunk3ds(current, LIN_COLOR_24, &color);
	 if (color != NULL)
	 {
	    ReadChunkData3ds(color);
	    (*mat)->specular.r = (float3ds)(((LinColor24 *)(color->data))->red) / 255.0F;
	    (*mat)->specular.g = (float3ds)(((LinColor24 *)(color->data))->green) / 255.0F;
	    (*mat)->specular.b = (float3ds)(((LinColor24 *)(color->data))->blue) / 255.0F;
	 }
	 break;

      case MAT_SHININESS:
      {
			(*mat)->shininess = ReadPercentageChunk3ds(current);
			break;
      }
	 
      case MAT_SHIN2PCT:
		
	 (*mat)->shinstrength = ReadPercentageChunk3ds(current);
	 break;

	 	case MAT_SHIN3PCT:
		/* Just skip for now */
		break;

      case MAT_REFBLUR:
	 
	 (*mat)->blur = ReadPercentageChunk3ds(current);
	 break;

      case MAT_TRANSPARENCY:
	 
	 (*mat)->transparency = ReadPercentageChunk3ds(current);
	 break;

      case MAT_XPFALL:
	 
	 (*mat)->transfalloff = ReadPercentageChunk3ds(current);
	 break;
	 
      case MAT_SELF_ILPCT:
	 
	 (*mat)->selfillumpct = ReadPercentageChunk3ds(current);
	 break;
       case MAT_WIRE:
	 (*mat)->shading = WireShading3ds;
	 break;
       case MAT_WIREABS:
	 (*mat)->usewireabs = True3ds;
	 break;
       case MAT_XPFALLIN:
     (*mat)->transparency = -((*mat)->transparency);
	 break;
       case MAT_WIRESIZE:
	 ReadChunkData3ds(current);
	 (*mat)->wiresize = ((MatWireSize *)(current->data))->wiresize;
	 break;
	 
      case MAT_USE_XPFALL:
	 (*mat)->usefall = True3ds;
	 break;

      case MAT_USE_REFBLUR:
	 (*mat)->useblur = True3ds;
	 break;

      case MAT_SELF_ILLUM:
	 (*mat)->selfillum = True3ds;
	 break;
	 
      case MAT_TWO_SIDE:
	 (*mat)->twosided = True3ds;
	 break;

      case MAT_ADDITIVE:
	 (*mat)->additive = True3ds;
	 break;

      case MAT_SHADING:
	 ReadChunkData3ds(current);
	 (*mat)->shading = ((MatShading *)(current->data))->matshading;
	 break;

      case MAT_FACEMAP:
	 (*mat)->facemap = True3ds;
	 break;

      case MAT_PHONGSOFT:
	 (*mat)->soften = True3ds;
	 break;

      case MAT_TEXMAP:
	 GetBitmapChunk3ds(current, &(*mat)->texture.map);
	 break;

      case MAT_TEXMASK:
	 GetBitmapChunk3ds(current, &(*mat)->texture.mask);
	 break;

      case MAT_TEX2MAP:
	 GetBitmapChunk3ds(current, &(*mat)->texture2.map);
	 break;

      case MAT_TEX2MASK:
	 GetBitmapChunk3ds(current, &(*mat)->texture2.mask);
	 break;

      case MAT_OPACMAP:
	 GetBitmapChunk3ds(current, &(*mat)->opacity.map);
	 break;

      case MAT_OPACMASK:
	 GetBitmapChunk3ds(current, &(*mat)->opacity.mask);
	 break;
	 
      case MAT_REFLMAP:
	 GetBitmapChunk3ds(current, &(*mat)->reflect.map);
	 break;

      case MAT_ACUBIC:
      {
	 MatAcubic *d;
	 d = ReadChunkData3ds(current);
	 (*mat)->reflect.useauto = True3ds;
	 (*mat)->reflect.automap.firstframe = (uchar3ds)((d->flags & ACubicFirst3ds) == ACubicFirst3ds);
	 (*mat)->reflect.automap.flat = (uchar3ds)((d->flags & ACubicFlat3ds) == ACubicFlat3ds);
	 
	 (*mat)->reflect.automap.size = d->mapsize;
	 (*mat)->reflect.automap.nthframe = d->frameinterval;
	 break;
      }

      case MAT_REFLMASK:
	 GetBitmapChunk3ds(current, &(*mat)->reflect.mask);
	 break;

      case MAT_BUMPMAP:
	 GetBitmapChunk3ds(current, &(*mat)->bump.map);
	 FindChunk3ds(current, MAT_BUMP_PERCENT, &datachunk);
	 if (datachunk != NULL)
	 {
	    MatBumpPercent *d;
	    d = ReadChunkData3ds(datachunk);
	    (*mat)->bump.map.percent = (float3ds)(d->intpercentage) / 100.0F;
	    
	 }
	 break;
	 
      case MAT_BUMPMASK:
	 GetBitmapChunk3ds(current, &(*mat)->bump.mask);
	 break;

      case MAT_SPECMAP:
	 GetBitmapChunk3ds(current, &(*mat)->specmap.map);
	 break;

      case MAT_SPECMASK:
	 GetBitmapChunk3ds(current, &(*mat)->specmap.mask);
	 break;

      case MAT_SHINMAP:
	 GetBitmapChunk3ds(current, &(*mat)->shinmap.map);
	 break;

      case MAT_SHINMASK:
	 GetBitmapChunk3ds(current, &(*mat)->shinmap.mask);
	 break;

      case MAT_SELFIMAP:
	 GetBitmapChunk3ds(current, &(*mat)->illummap.map);
	 break;

      case MAT_SELFIMASK:
	 GetBitmapChunk3ds(current, &(*mat)->illummap.mask);
	 break;

      case MAT_SXP_TEXT_DATA:
      {
	 IpasData *d;
	 d = ReadChunkData3ds(current);
	 (*mat)->texture.map.datasize = d->size;
	 (*mat)->texture.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_TEXT_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->texture.mask.datasize = d->size;
	 (*mat)->texture.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_TEXT2_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->texture2.map.datasize = d->size;
	 (*mat)->texture2.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_TEXT2_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->texture2.mask.datasize = d->size;
	 (*mat)->texture2.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_OPAC_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->opacity.map.datasize = d->size;
	 (*mat)->opacity.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_OPAC_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->opacity.mask.datasize = d->size;
	 (*mat)->opacity.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_REFL_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->reflect.mask.datasize = d->size;
	 (*mat)->reflect.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_BUMP_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->bump.map.datasize = d->size;
	 (*mat)->bump.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_BUMP_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->bump.mask.datasize = d->size;
	 (*mat)->bump.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SPEC_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->specmap.map.datasize = d->size;
	 (*mat)->specmap.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SPEC_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->specmap.mask.datasize = d->size;
	 (*mat)->specmap.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SHIN_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->shinmap.map.datasize = d->size;
	 (*mat)->shinmap.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SHIN_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->shinmap.mask.datasize = d->size;
	 (*mat)->shinmap.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SELFI_DATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->illummap.map.datasize = d->size;
	 (*mat)->illummap.map.data = d->data;
	 d->data = NULL;
	 break;
      }
      case MAT_SXP_SELFI_MASKDATA:
      {
	 IpasData *d;
	 ReadChunkData3ds(current);
	 d = current->data;
	 (*mat)->illummap.mask.datasize = d->size;
	 (*mat)->illummap.mask.data = d->data;
	 d->data = NULL;
	 break;
      }
      default:
	 SET_ERROR_RETURN(ERR_INVALID_CHUNK)
      }
     } /* prs */
	 ReleaseChunk3ds(&current);
   }
}   
	
void GetMaterialByIndex3ds(database3ds *db, ulong3ds index, material3ds **mat)
{
   chunk3ds *chunk;

   if (mat == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

   chunk = FindMatEntryByIndex3ds(db, index);
   ON_ERROR_RETURN;
   
   if (chunk != NULL)
   {
      ReadMatEntryChunk3ds(chunk, mat);
      ON_ERROR_RETURN;
   } 
}

void GetMaterialByName3ds(database3ds *db, char3ds *name, material3ds **mat)
{
   chunk3ds *chunk=NULL;

   if ( mat == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindMatEntry3ds(db, name, &chunk);
   ON_ERROR_RETURN;

   if (chunk != NULL)
      ReadMatEntryChunk3ds(chunk, mat);
   
}

void DeleteMaterialByIndex3ds(database3ds *db, ulong3ds index)
{
  chunk3ds *pChunk;

  pChunk = FindMatEntryByIndex3ds(db, index);
  ON_ERROR_RETURN;
  
  if (pChunk != NULL)
  {
     DeleteChunk3ds(pChunk);
     MakeMatEntryListDirty3ds(db);
  }
}

void DeleteMaterialByName3ds(database3ds *db, char3ds *name)
{
   chunk3ds *pChunk;
  
   FindMatEntry3ds(db, name, &pChunk);
   ON_ERROR_RETURN;
   
   if (pChunk != NULL)
      DeleteChunk3ds(pChunk);

   MakeMatEntryListDirty3ds(db);
}


void CopyMaterialByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{
   chunk3ds *topsect, *matr, *newmatr;

   if ((destdb == NULL) || (srcdb == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Check to see if the destination database is a 3DS, PRJ, or MLI */
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC) &&
       (srcdb->topchunk->tag != MLIBMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC) &&
       (destdb->topchunk->tag != MLIBMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find the material in the source database */
   matr = FindMatEntryByIndex3ds(srcdb, index);
   ON_ERROR_RETURN;
   
   if (matr != NULL)
   {
      chunk3ds *nc;
      MatName *matrname;

      /* Find the section in the destination the material gets copied to */
      switch(destdb->topchunk->tag)
      {
      case M3DMAGIC:
      case CMAGIC:
      {	 
	 FindNextChunk3ds(destdb->topchunk->children, MDATA, &topsect);

	 /* If MDATA chunk not present then create one */
	 if (topsect == NULL)
	 {
	    InitChunkAs3ds(&topsect, MDATA);
	    ON_ERROR_RETURN;
	    AddChildOrdered3ds(destdb->topchunk, topsect);
	 }
	 break;
      }
      case MLIBMAGIC:
	 topsect = destdb->topchunk;
      }
      
      /* Get the source materials's name */
      FindNextChunk3ds(matr->children, MAT_NAME, &nc);
      if (nc == NULL) SET_ERROR_RETURN(ERR_INVALID_CHUNK);
      
      matrname = ReadChunkData3ds(nc);

      /* Delete any material by that name in the destination */
      DeleteMaterialByName3ds(destdb, matrname->name);
      
      /* Copy the source material */
      CopyChunk3ds(matr, &newmatr);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(topsect, newmatr);

      MakeMatEntryListDirty3ds(destdb);
   }
}

void CopyMaterialByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   chunk3ds *topsect, *matr, *newmatr;

   if ((destdb == NULL) || (srcdb == NULL) || (name == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Check to see if the destination database is a 3DS, PRJ, or MLI */
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC) &&
       (srcdb->topchunk->tag != MLIBMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC) &&
       (destdb->topchunk->tag != MLIBMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find the material in the source database */
   FindMatEntry3ds(srcdb, name, &matr);
   ON_ERROR_RETURN;
   
   if (matr != NULL)
   {
   
      /* Find the section in the destination the material gets copied to */
      switch(destdb->topchunk->tag)
      {
      case M3DMAGIC:
      case CMAGIC:
      {	 
	 FindNextChunk3ds(destdb->topchunk->children, MDATA, &topsect);

	 /* If MDATA chunk not present then create one */
	 if (topsect == NULL)
	 {
	    InitChunkAs3ds(&topsect, MDATA);
	    ON_ERROR_RETURN;
	    AddChildOrdered3ds(destdb->topchunk, topsect);
	 }
	 break;
      }
      case MLIBMAGIC:
	 topsect = destdb->topchunk;
      }
      
      /* Delete any material by that name in the destination */
      DeleteMaterialByName3ds(destdb, name);
      
      /* Copy the source material */
      CopyChunk3ds(matr, &newmatr);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(topsect, newmatr);

      MakeMatEntryListDirty3ds(destdb);
   }
}


/* Puts a material3ds into the chunk database.  Not that this routine will clobber
   materials of the same name, except it will transfer the old xdata onto the new
   object.  If the developer wants to wipe out the XDATA too, he'll have to
   explicitly delete using the DeleteMaterial3ds function */
void PutMaterial3ds(database3ds *db, material3ds *material)
{
   chunk3ds *container, *matr, *current;
   chunk3ds *xdata = NULL;

   if(db == NULL || material == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if(db->topchunk == NULL)
     SET_ERROR_RETURN(ERR_INVALID_DATABASE);

   if(!(db->topchunk->tag == M3DMAGIC || 
	db->topchunk->tag == CMAGIC ||
	db->topchunk->tag == MLIBMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);     

   /* Depending on the file type, we look for different things */

   /* If it's a material library, then we use the magic chunk for the containing chunk */
   if (db->topchunk->tag == MLIBMAGIC)
   {
      container = db->topchunk;
   } else
   {
      FindChunk3ds(db->topchunk, MDATA, &container);

      /* if an MDATA chunk is not found, then add one to the file */
      if (container == NULL)
      {
	 InitChunkAs3ds(&container, MDATA);
	 AddChildOrdered3ds(db->topchunk, container);
      }
   }

   /* Discover if the material name already exists */
   FindMatEntry3ds(db, material->name, &matr);

   /* If it does, then delete it */
   if (matr != NULL)
   {
      /* First, resue the xdata chunk if there is one */
      FindChunk3ds(matr, XDATA_SECTION, &current);

      /* If there is one */
      if (current != NULL)
      {
	 /* Then make a copy of it to reintroduce later */
	 CopyChunk3ds(current, &xdata);
      }

      /* Delete the chunk's children and data, it's list position */
      DeleteChunk3ds(matr);
      matr->tag = MAT_ENTRY;
   } else /* else a MAT_ENTRY needs to be initialized and added */
   {
      InitChunkAs3ds(&matr, MAT_ENTRY);
      AddChildOrdered3ds(container, matr);
   }

   /* Make the name list dirty */
   MakeMatEntryListDirty3ds(db);
   
   /* Add the material name. */
   {
      MatName *d;
      chunk3ds *temp = NULL;
      
      /* Initialize the new chunk */
      d = InitChunkAndData3ds(&temp, MAT_NAME);
      
      d->name = strdup(material->name);
      AddChildOrdered3ds(matr, temp);
   }
   
   /* Add ambient light color */
   {
      chunk3ds *ambient, *color;
      Color24 *c;
      
      /* Initialize the new ambient chunk */
      InitChunkAs3ds(&ambient, MAT_AMBIENT);
      
      /* Initialize the color chunk */
      c = InitChunkAndData3ds(&color, COLOR_24);
      
      /* Copy over the values */
      c->red = (ubyte3ds)(material->ambient.r * 255.0F);
      c->green = (ubyte3ds)(material->ambient.g * 255.0F);
      c->blue = (ubyte3ds)(material->ambient.b * 255.0F);
      
      /* Put the color chunk into the ambient chunk */
      AddChildOrdered3ds(ambient, color);
      
      /* Initialize the color chunk again for the second linear color */
      c = InitChunkAndData3ds(&color, LIN_COLOR_24);
      
      /* Copy over the values */
      c->red = (ubyte3ds)(material->ambient.r * 255.0F);
      c->green = (ubyte3ds)(material->ambient.g * 255.0F);
      c->blue = (ubyte3ds)(material->ambient.b * 255.0F);
      
      /* Put the lcolor chunk into the ambient chunk */
      AddChildOrdered3ds(ambient, color);
      
      /* Put the ambient chunk into the material */
      AddChildOrdered3ds(matr, ambient);
   }
   
   /* Add diffuse light color */
   {
      chunk3ds *diffuse, *color;
      Color24 *c;
      
      InitChunkAs3ds(&diffuse, MAT_DIFFUSE);
      c = InitChunkAndData3ds(&color, COLOR_24);
      
      c->red = (ubyte3ds)(material->diffuse.r * 255.0F);
      c->green = (ubyte3ds)(material->diffuse.g * 255.0F);
      c->blue = (ubyte3ds)(material->diffuse.b * 255.0F);
      
      AddChildOrdered3ds(diffuse, color);
      
      c = InitChunkAndData3ds(&color, LIN_COLOR_24);
      
      c->red = (ubyte3ds)(material->diffuse.r * 255.0F);
      c->green = (ubyte3ds)(material->diffuse.g * 255.0F);
      c->blue = (ubyte3ds)(material->diffuse.b * 255.0F);
      
      AddChildOrdered3ds(diffuse, color);
      AddChildOrdered3ds(matr, diffuse);
   }
   
   /* Add specular light color */
   {
      chunk3ds *specular, *color;
      Color24 *c;
      
      InitChunkAs3ds(&specular, MAT_SPECULAR);
      c = InitChunkAndData3ds(&color, COLOR_24);
      
      c->red = (ubyte3ds)(material->specular.r * 255.0F);
      c->green = (ubyte3ds)(material->specular.g * 255.0F);
      c->blue = (ubyte3ds)(material->specular.b * 255.0F);
      
      AddChildOrdered3ds(specular, color);
      
      c = InitChunkAndData3ds(&color, LIN_COLOR_24);
      
      c->red = (ubyte3ds)(material->specular.r * 255.0F);
      c->green = (ubyte3ds)(material->specular.g * 255.0F);
      c->blue = (ubyte3ds)(material->specular.b * 255.0F);
      
      AddChildOrdered3ds(specular, color);
      AddChildOrdered3ds(matr, specular);
   }
   
   /* Add shininess factor */
   {
      chunk3ds *shininess, *ci;
      IntPercentage *i;
      
      InitChunkAs3ds(&shininess, MAT_SHININESS);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      i->intpercentage = (short3ds)(material->shininess * 100.0F);
      
      AddChildOrdered3ds(shininess, ci);
      AddChildOrdered3ds(matr, shininess);
   }
   
    /* Add shininess strength factor */
   {
      chunk3ds *shin2pct, *ci;
      IntPercentage *i;
      
      InitChunkAs3ds(&shin2pct, MAT_SHIN2PCT);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      i->intpercentage = (short3ds)(material->shinstrength * 100.0F);
      
      AddChildOrdered3ds(shin2pct, ci);
      AddChildOrdered3ds(matr, shin2pct);
   }
   
   /* Add transparency factor */
   {
      chunk3ds *transparency, *ci;
      IntPercentage *i;
      
      InitChunkAs3ds(&transparency, MAT_TRANSPARENCY);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      i->intpercentage = (short3ds)(material->transparency * 100.0F);
      
      AddChildOrdered3ds(transparency, ci);
      AddChildOrdered3ds(matr, transparency);
   }
   
   /* Add transparency falloff factor */
   {
      chunk3ds *transfalloff, *ci;
      IntPercentage *i;
      short3ds abs_percent;
      float3ds round = (material->transfalloff > 0.0F)? 0.5F: -0.5F;
      
      InitChunkAs3ds(&transfalloff, MAT_XPFALL);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      /*--- Round for accuracy */
      abs_percent = (short3ds)(material->transfalloff * 100.0F + round);
      i->intpercentage = (short3ds)abs((int)abs_percent);
      
      AddChildOrdered3ds(transfalloff, ci);
      AddChildOrdered3ds(matr, transfalloff);

      if (material->transfalloff < 0.0F){
    chunk3ds *fallin;

    InitChunkAs3ds(&fallin, MAT_XPFALLIN);  
    AddChildOrdered3ds(matr, fallin);
      }
   }
   
   /* Add refblur factor */
   {
      chunk3ds *refblur, *ci;
      IntPercentage *i;
      
      InitChunkAs3ds(&refblur, MAT_REFBLUR);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      i->intpercentage = (short3ds)(material->blur * 100.0F);
      
      AddChildOrdered3ds(refblur, ci);
      AddChildOrdered3ds(matr, refblur);
   }
   
   /* Shading type */
   {
      chunk3ds *c;
      MatShading *d;
      
      d = InitChunkAndData3ds(&c, MAT_SHADING);
      d->matshading = (short3ds)(material->shading);
      
      AddChildOrdered3ds(matr, c);
   }
   
   /* Add self illum percentage */
   {
      chunk3ds *sillum, *ci;
      IntPercentage *i;
      
      InitChunkAs3ds(&sillum, MAT_SELF_ILPCT);
      i = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      
      i->intpercentage = (short3ds)(material->selfillumpct * 100.0F);
      
      AddChildOrdered3ds(sillum, ci);
      AddChildOrdered3ds(matr, sillum);
   }
   
   if (material->twosided)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_TWO_SIDE);
      
      AddChildOrdered3ds(matr, c);
   }
   
   /* Wire size */
   {
      chunk3ds *c;
      MatWireSize *d;
      
      d = InitChunkAndData3ds(&c, MAT_WIRESIZE);
      d->wiresize = material->wiresize;
      
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->useblur)
   {
      chunk3ds *c;
      
      InitChunkAs3ds(&c, MAT_USE_REFBLUR);
      
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->usefall)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_USE_XPFALL);
      
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->selfillum)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_SELF_ILLUM);
      
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->additive)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_ADDITIVE);
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->usewire)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_WIRE);
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->usewireabs) 
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_WIREABS);
      AddChildOrdered3ds(matr, c);
   }

   if (material->facemap)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_FACEMAP);
      AddChildOrdered3ds(matr, c);
   }
   
   if (material->soften)
   {
      chunk3ds *c;
      InitChunkAs3ds(&c, MAT_PHONGSOFT);
      AddChildOrdered3ds(matr, c);
   }
   
   /* If the map structure has a filename then add it to the chunk */
   if (material->texture.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_TEXMAP);
      PutBitmapChunk3ds(m, &(material->texture.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->texture.map.data != NULL) && (material->texture.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_TEXT_DATA);
	 ds->size = material->texture.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->texture.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
      
   }
   
   if (material->texture.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_TEXMASK);
      PutBitmapChunk3ds(m, &(material->texture.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->texture.mask.data != NULL) && (material->texture.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_TEXT_MASKDATA);
	 ds->size = material->texture.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->texture.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
      
   }
   
   if (material->texture2.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_TEX2MAP);
      PutBitmapChunk3ds(m, &(material->texture2.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->texture2.map.data != NULL) && (material->texture2.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_TEXT2_DATA);
	 ds->size = material->texture2.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->texture2.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->texture2.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_TEX2MASK);
      PutBitmapChunk3ds(m, &(material->texture2.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->texture2.mask.data != NULL) && (material->texture2.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_TEXT2_MASKDATA);
	 ds->size = material->texture2.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->texture2.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->opacity.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_OPACMAP);
      PutBitmapChunk3ds(m, &(material->opacity.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->opacity.map.data != NULL) && (material->opacity.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_OPAC_DATA);
	 ds->size = material->opacity.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->opacity.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->opacity.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_OPACMASK);
      PutBitmapChunk3ds(m, &(material->opacity.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->opacity.mask.data != NULL) && (material->opacity.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_OPAC_MASKDATA);
	 ds->size = material->opacity.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->opacity.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->bump.map.name[0] != 0)
   {
      chunk3ds *m;
      chunk3ds *s;
      MatBumpPercent *d;
      
      InitChunkAs3ds(&m, MAT_BUMPMAP);
      PutBitmapChunk3ds(m, &(material->bump.map));
      
      d = InitChunkAndData3ds(&s, MAT_BUMP_PERCENT);
      d->intpercentage = (short3ds)(material->bump.map.percent * 100.0F);
      AddChildOrdered3ds(m, s);
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->bump.map.data != NULL) && (material->bump.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_BUMP_DATA);
	 ds->size = material->bump.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->bump.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
      
   }
   
   if (material->bump.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_BUMPMASK);
      PutBitmapChunk3ds(m, &(material->bump.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->bump.mask.data != NULL) && (material->bump.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_BUMP_MASKDATA);
	 ds->size = material->bump.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->bump.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->specmap.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SPECMAP);
      PutBitmapChunk3ds(m, &(material->specmap.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->specmap.map.data != NULL) && (material->specmap.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_SPEC_DATA);
	 ds->size = material->specmap.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->specmap.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->specmap.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SPECMASK);
      PutBitmapChunk3ds(m, &(material->specmap.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->specmap.mask.data != NULL) && (material->specmap.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_OPAC_MASKDATA);
	 ds->size = material->specmap.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->specmap.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->shinmap.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SHINMAP);
      PutBitmapChunk3ds(m, &(material->shinmap.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->shinmap.map.data != NULL) && (material->shinmap.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_SHIN_DATA);
	 ds->size = material->shinmap.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->shinmap.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->shinmap.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SHINMASK);
      PutBitmapChunk3ds(m, &(material->shinmap.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->shinmap.mask.data != NULL) && (material->shinmap.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_SHIN_MASKDATA);
	 ds->size = material->shinmap.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->shinmap.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->illummap.map.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SELFIMAP);
      PutBitmapChunk3ds(m, &(material->illummap.map));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->illummap.map.data != NULL) && (material->illummap.map.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_SELFI_DATA);
	 ds->size = material->illummap.map.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->illummap.map.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->illummap.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_SELFIMASK);
      PutBitmapChunk3ds(m, &(material->illummap.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->illummap.mask.data != NULL) && (material->illummap.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_SELFI_MASKDATA);
	 ds->size = material->illummap.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->illummap.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if ((material->reflect.map.name[0] != 0) || (material->reflect.useauto))
   {
      chunk3ds *cm, *cn, *ci;
      IntPercentage *di;
      MatName *dn;
      
      InitChunkAs3ds(&cm, MAT_REFLMAP);
      
      di = InitChunkAndData3ds(&ci, INT_PERCENTAGE);
      di->intpercentage = (short3ds)(material->reflect.map.percent * 100.0F);
      AddChildOrdered3ds(cm, ci);
      
      dn = InitChunkAndData3ds(&cn, MAT_MAPNAME);
      dn->name = strdup(material->reflect.map.name);
      
      AddChildOrdered3ds(cm, cn);
      
      AddChildOrdered3ds(matr, cm);
   }
   
   if (material->reflect.mask.name[0] != 0)
   {
      chunk3ds *m;
      
      InitChunkAs3ds(&m, MAT_REFLMASK);
      PutBitmapChunk3ds(m, &(material->reflect.mask));
      
      AddChildOrdered3ds(matr, m);
      
      if ((material->reflect.mask.data != NULL) && (material->reflect.mask.datasize > 0))
      {
	 chunk3ds *cs;
	 IpasData *ds;
	 
	 ds = InitChunkAndData3ds(&cs, MAT_SXP_REFL_MASKDATA);
	 ds->size = material->reflect.mask.datasize;
	 ds->data = malloc(ds->size);
	 memcpy(ds->data, material->reflect.mask.data, ds->size);
	 
	 AddChildOrdered3ds(matr, cs);
      }
   }
   
   if (material->reflect.useauto)
   {
      chunk3ds *ca;
      MatAcubic *da;
      
      da = InitChunkAndData3ds(&ca, MAT_ACUBIC);
      da->flags = 1;
      if (material->reflect.automap.firstframe)
	 da->flags = (short3ds)(da->flags | ACubicFirst3ds);
      if (material->reflect.automap.flat)
	 da->flags = (short3ds)(da->flags | ACubicFlat3ds);
      
      da->mapsize = material->reflect.automap.size;
      da->frameinterval = material->reflect.automap.nthframe;
      
      AddChildOrdered3ds(matr, ca);
   }
   
}

