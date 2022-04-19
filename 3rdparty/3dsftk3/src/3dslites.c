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


/*===========================================================================
 | Module Overview:
 |
 |  3dslite.h and 3dslite.c concern themselves with the handling of 
 |  spotlights and omnilights inside the 3ds mesh section.
 |  
 +===========================================================================*/



#include <stdlib.h>
#include <string.h>

#include "3dstype.h"
#include "3dsprim.h"
#include "3dserr.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "3dslites.h"
#include "kfutils.h"


static void putTagFloat(chunk3ds *pParentChunk,
            chunktag3ds tag, 
            float3ds *f) ;

/*--------------------------------------------------------------------------
 | InitLight3ds
 |
 | Initializes the light3ds structure.  If *light is null, 
 | memory is also allocated for it. Note that the *spot field is 
 | ignored when an existing structure is used.
 |
 +--------------------------------------------------------------------------*/
void InitLight3ds(light3ds **light)
{
  if (light == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
  
   /* If (*light) a null pointer, then alloc new memory for the pointer */
   if (*light == NULL){
      *light = malloc(sizeof(light3ds));
    if(*light == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    
      /* Set all fields with pointer to NULL as well */
      (*light)->spot = NULL;
      (*light)->exclude = NULL;
   }
   
  (*light)->name[0] = 0;
  (*light)->pos.x = 0.0F;
  (*light)->pos.y = 0.0F;
  (*light)->pos.z = 0.0F;
  (*light)->color.r = 0.708852F;
  (*light)->color.g = 0.708852F;
  (*light)->color.b = 0.708852F;
  (*light)->multiplier = 1.0F;
  (*light)->dloff = False3ds;
  (*light)->attenuation.on = False3ds;
  (*light)->attenuation.inner = 10.0F;
  (*light)->attenuation.outer = 100.0F;

  if ((*light)->exclude != NULL)
    ReleaseNameList3ds(&((*light)->exclude));
  
  InitNameList3ds( &((*light)->exclude), (long3ds)0); 
}

/*-----------------------------------------------------------
 | ReleaseLight3ds
 |
 +-----------------------------------------------------------*/
void ReleaseLight3ds(light3ds **light)
{
  if((*light) == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  if ((*light)->exclude != NULL)
    ReleaseNameList3ds(&((*light)->exclude));
  ON_ERROR_RETURN;

  if ((*light)->spot != NULL){
    if ((*light)->spot->projector.bitmap != NULL) 
      free((*light)->spot->projector.bitmap);
    ON_ERROR_RETURN;
    free((*light)->spot);
  }
  ON_ERROR_RETURN;

  free(*light);
  *light = NULL;
}


/*----------------------------------------------------------
 | InitSpotlight3ds
 |
 +----------------------------------------------------------*/
void InitSpotlight3ds(light3ds **spotlight)
{
  if(spotlight == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  /* Do the common light initialization */
  InitLight3ds(spotlight);

  /* free the old spotlight pointer */
  if ((*spotlight)->spot != NULL)
    {
      if ((*spotlight)->spot->projector.bitmap != NULL) 
    free((*spotlight)->spot->projector.bitmap);
      free((*spotlight)->spot);
      (*spotlight)->spot = NULL;
    }

  (*spotlight)->spot = malloc(sizeof(spotlight3ds));
  if((*spotlight)->spot == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);
   
  (*spotlight)->spot->target.x = 1.0F;
  (*spotlight)->spot->target.y = 1.0F;
  (*spotlight)->spot->target.z = 1.0F;
  (*spotlight)->spot->hotspot = 44.0F;
  (*spotlight)->spot->falloff = 45.0F;
  (*spotlight)->spot->roll = 0.0F;
  (*spotlight)->spot->aspect = 1.0F;

  (*spotlight)->spot->shadows.cast = False3ds;
  (*spotlight)->spot->shadows.type = UseShadowMap;
  (*spotlight)->spot->shadows.local = False3ds;
  (*spotlight)->spot->shadows.bias = 1.0F;
  (*spotlight)->spot->shadows.filter = 3.0F;
  (*spotlight)->spot->shadows.mapsize = 512;
  (*spotlight)->spot->shadows.raybias = 1.0F;

  (*spotlight)->spot->cone.type = Circular;
  (*spotlight)->spot->cone.show = False3ds;
  (*spotlight)->spot->cone.overshoot = False3ds;

  (*spotlight)->spot->projector.use = False3ds;
  (*spotlight)->spot->projector.bitmap = NULL;
}   

/*------------------------------------------------------------------------
 |
 | GetOmnilightCount3ds
 |
 +--------------------------------------------------------------------*/
ulong3ds GetOmnilightCount3ds(database3ds *db)
{
   chunk3ds *dlite, *spotl;
   ulong3ds omnicount = 0, i;

   if (db == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_ARG, 0); 

   /* Update the index to named objects if the list has changed recently */
   UpdateNamedObjectList3ds(db);

   if (db->objlist == NULL) return 0;
   
   /* Scan through the list of named objects looking for lights */
   for (i = 0; i < db->objlist->count; i++)
   {
      /* Search each object for a light chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &dlite);

      /* if one was found, check to see if its a spotlight */
      if (dlite != NULL)
      {
     FindChunk3ds(dlite, DL_SPOTLIGHT, &spotl);
     /* if it isn't a spotlight, then increment the count */
     if (spotl == NULL) omnicount++;
      }
   }
   return omnicount;
}

/*------------------------------------------------------------------------
 |
 | GetSpotlightCount3ds
 |
 +--------------------------------------------------------------------*/
ulong3ds GetSpotlightCount3ds(database3ds *db)
{
   chunk3ds *dlite, *spotl;
   ulong3ds spotcount = 0, i;

   if (db == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_ARG, 0); 

   /* Update the index to named objects if the list has changed recently */
   UpdateNamedObjectList3ds(db);
   
   if (db->objlist == NULL) return 0;

   /* Scan through the list of named objects looking for lights */
   for (i = 0; i < db->objlist->count; i++)
   {
      /* Search each object for a light chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &dlite);

      /* if one was found, check to see if its a spotlight */
      if (dlite != NULL)
      {
     FindChunk3ds(dlite, DL_SPOTLIGHT, &spotl);
     /* if it is a spotlight, then increment the count */
     if (spotl != NULL) 
        spotcount++;
      }
   }

   return spotcount;
}

/*--------------------------------------------------------------------------
 | 
 | GetOmnilightNameList3ds
 |  Searches *db to construct **list, a list of omnilight names within 
 |  the database 
 |
 |  db: The database bing searched.
 |  list: The resulting namelist pairs, if *list is null, then a namelist
 |        structure will be allocated.
 |
 +--------------------------------------------------------------------------*/
void GetOmnilightNameList3ds(database3ds *db, 
                 namelist3ds **list)
{
  chunk3ds *lite, *spot;
  ulong3ds omnilightcount, i, j;
  
   if (db == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 
  
  /* Update the index to named objects if the list has changed recently */
  UpdateNamedObjectList3ds(db);
  
  /* Get the number of omnilights contained in the named object list */
  omnilightcount = GetOmnilightCount3ds(db);
  
  /* Initilize the namelist array */
  InitNameList3ds(list, omnilightcount);
  
  /* Scan through the list of named objects */
  for (i = 0, j = 0; (i < db->objlist->count) && (j < omnilightcount); i++)
    {
      /* Search each named object for a light chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &lite);
      
      /* if its a light chunk, check to see if its a spotlight */
      if (lite != NULL)
    {
      FindChunk3ds(lite, DL_SPOTLIGHT, &spot);
      
      /* If its not a spot */
      if (spot == NULL)
        {
          /* Copy the name into the list */
          (*list)->list[j].name = strdup(db->objlist->list[i].name);
          j++; /* increment the omnilight counter */
        }
    }
    }
}

/*-----------------------------------------------------------------
 |
 | DeleteSpotlightByName3ds
 |
 +----------------------------------------------------------------*/
void DeleteSpotlightByName3ds(database3ds *db, char3ds *name)
{
   
   DeleteNamedObjectByName3ds(db, name);
   ON_ERROR_RETURN;
}

void DeleteSpotlightByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteChunkTypeByIndex3ds(db, DL_SPOTLIGHT, index);
   ON_ERROR_RETURN;
}

void CopySpotlightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{

   CopyNamedObjectByIndex3ds(destdb, srcdb, DL_SPOTLIGHT, index);
   ON_ERROR_RETURN;
}

void CopySpotlightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNamedObjectByName3ds(destdb, srcdb, name);
   ON_ERROR_RETURN;
}


void DeleteOmnilightByName3ds(database3ds *db, char3ds *name)
{
   
   DeleteNamedObjectByName3ds(db, name);
   ON_ERROR_RETURN;
}

void DeleteOmnilightByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteChunkTypeByIndex3ds(db, N_DIRECT_LIGHT, index);
   ON_ERROR_RETURN;
}

void CopyOmnilightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{

   CopyNamedObjectByIndex3ds(destdb, srcdb, N_DIRECT_LIGHT, index);
   ON_ERROR_RETURN;
}

void CopyOmnilightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNamedObjectByName3ds(destdb, srcdb, name);
   ON_ERROR_RETURN;
}

/*--------------------------------------------------------------------------
 | 
 | GetSpotlightNameList3ds
 |  Searches *db to construct **list, a list of spotlight names within 
 |  the database 
 |
 |  db: The database bing searched.
 |  list: The resulting namelist pairs, if *list is null, then a namelist
 |        structure will be allocated.
 |
 +--------------------------------------------------------------------------*/
void GetSpotlightNameList3ds(database3ds *db, 
                 namelist3ds **list)
{
  chunk3ds *spot;
  ulong3ds spotlightcount, i, j;
  
  if (db == NULL || list == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
  
  /* Update the index to named objects if the list has changed recently */
  UpdateNamedObjectList3ds(db);
  
  /* Get the number of spotlights contained in the named object list */
  spotlightcount = GetSpotlightCount3ds(db);
  
  /* Initilize the namelist array */
  InitNameList3ds(list, spotlightcount);
  
  /* Scan through the list of named objects */
  for (i = 0, j = 0; (i < db->objlist->count) && (j < spotlightcount); i++)
    {
       /* Search each named object for a direct light chunk */
       FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &spot);

       /* Search each direct light for a spotlight chunk */
       if (spot != NULL)
	  FindChunk3ds(spot, DL_SPOTLIGHT, &spot);
      
      /* If it is a spotlight chunk */
      if (spot != NULL)
    {
      /* Copy the name into the list */
      (*list)->list[j].name = strdup(db->objlist->list[i].name);
      j++;
    }
    }
}

/*------------------------------------------------------------------------
 |
 | GetOmnilightByIndex3ds
 | Fills out the **omni structure from the (index)th mesh reference 
 | found in *db.
 |
 +------------------------------------------------------------------------*/
void GetOmnilightByIndex3ds(database3ds *db, 
                long3ds index, 
                light3ds **omni)
{
  chunk3ds *lite, *spot;
  ulong3ds i;
  long3ds omnicount;
  
  if (db == NULL || omni == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);
  
  /* Update the list if it's changed recently */
  UpdateNamedObjectList3ds(db);
  
  /* Scan through the list */
  for (i = 0, omnicount=0; i < db->objlist->count; i++)
    {
      /* Search for a light chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &lite);
      
      /* if one was found, check to see if its a spot */
      if (lite != NULL)
    {
      FindChunk3ds(lite, DL_SPOTLIGHT, &spot);
        {
          /* if its not a spot then increment the count */
          if (spot == NULL)
        {
          omnicount++;
          /* If thisis the (index)th light, file out the structure */
          if (omnicount-1 == index)
            {
              GetLightEntryChunk3ds(db->objlist->list[i].chunk, omni);
              break;
            }
        }
        }
    }
    }
}

/*------------------------------------------------------------------------
 |
 | GetSpotlightByIndex3ds
 | Fills out the **spot structure from the (index)th spot reference 
 | found in *db.
 |
 +------------------------------------------------------------------------*/
void GetSpotlightByIndex3ds(database3ds *db, 
                long3ds index, 
                light3ds **spotlight)
{
  chunk3ds *lite, *spot;
  ulong3ds i;
  long3ds spotcount;
  
  if (db == NULL || spotlight == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  /* Update the list if it's changed recently */
  UpdateNamedObjectList3ds(db);
  
  /* Scan through the list */
  for (i = 0, spotcount=0; i < db->objlist->count; i++)
    {
      /* Search for a light chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &lite);
      
      /* if one was found, check to see if its a spot */
      if (lite != NULL)
    {
      FindChunk3ds(lite, DL_SPOTLIGHT, &spot);
        {
          /* if its a spot then increment the count */
          if (spot != NULL)
        {
          spotcount++;
          /* If thisis the (index)th light, file out the structure */
          if (spotcount-1 == index)
            {
              GetLightEntryChunk3ds(db->objlist->list[i].chunk, 
                        spotlight);
              break;
            }
        }
        }
    }
    }
}

/*-------------------------------------------------------------------------
 |
 | GetOmnilightByName3ds
 |
 |  Fills out the **omni structure contained in *db that matches the 
 |  name in *name.
 |
 +------------------------------------------------------------------------*/
void GetOmnilightByName3ds(database3ds *db, 
               char3ds *name, 
               light3ds **omnilight)
{
  chunk3ds *nobj, *lite, *spot;
  
   if (db == NULL || name == NULL || omnilight == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  /* Find the matching name */
  FindNamedObject3ds(db, name, &nobj);
  
  /* If a match was found */
  if (nobj != NULL)
    {
      /* Verify that its a light */
      FindChunk3ds(nobj, N_DIRECT_LIGHT, &lite);
      
      /* if its a light then check to see if its a spot */
      FindChunk3ds(lite, DL_SPOTLIGHT, &spot);
      
      /* if its not a spot, then retreave the structure */
      if (spot == NULL)
    {
      GetLightEntryChunk3ds(nobj, omnilight);
    }
    }
}

/*--------------------------------------------------------------------------
 |
 | GetSpotlightByName3ds
 |
 | Fills out the **spot structure contained in *db that matches the 
 | name in *name.
 |
 +--------------------------------------------------------------------------*/
void GetSpotlightByName3ds(database3ds *db, 
               char3ds *name, 
               light3ds **spotlight)
{
  chunk3ds *nobj, *lite, *spot;
  
   if (db == NULL || spotlight == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
  
  /* Find the matching name */
  FindNamedObject3ds(db, name, &nobj);
  
  /* If a match was found */
  if (nobj != NULL)
    {
      /* Verify that its a light */
      FindChunk3ds(nobj, N_DIRECT_LIGHT, &lite);
      
      /* if its a light then check to see if its a spot */
      FindChunk3ds(lite, DL_SPOTLIGHT, &spot);
      
      /* if it is a spot, then retreave the structure */
      if (spot != NULL)
    {
      GetLightEntryChunk3ds(nobj, spotlight);
    }
    }
}

/*------------------------------------------------------------------------
  |
  | GetLightEntryChunk3ds
  | Fills out the *light structure with the light pointed to 
  | by *chunk
  |
  +-----------------------------------------------------------------------*/
void GetLightEntryChunk3ds(chunk3ds *chunk, light3ds **light)
{
  chunk3ds *nobj, *dlite, *spot, *current;
  
   if (light  == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

  FindNextChunk3ds(chunk->children, N_DIRECT_LIGHT, &dlite);
  if (dlite == NULL) SET_ERROR_RETURN(ERR_WRONG_OBJECT);
		   
  CopyChunk3ds(chunk, &nobj);
      
  FindChunk3ds(nobj, N_DIRECT_LIGHT, &dlite);
      
  FindChunk3ds(nobj, DL_SPOTLIGHT, &spot);
      
  if (dlite != NULL)
  {
     NamedObject *nobjd;
     NDirectLight *ndl;
      
     /* Initilize **light properly */
     if (spot == NULL)
        InitLight3ds(light);
     else
        InitSpotlight3ds(light);
      
     /* Copy the object name */
     nobjd = ReadChunkData3ds(nobj);
     strcpy((*light)->name, nobjd->name);
      
     /* Copy the light postion */
     ndl = ReadChunkData3ds(dlite);
     (*light)->pos = ndl->lightpos;
      
     /* scan all the chunks the light contains */
     for (current = dlite->children; current != NULL; current = current->sibling)
     {
	switch(current->tag)
        {
        case COLOR_F:   /* The color of the light */
	   {
	      ColorF *d;
	      d = ReadChunkData3ds(current);
	      (*light)->color.r = d->red;
	      (*light)->color.g = d->green;
	      (*light)->color.b = d->blue;
	   }
	   break;
        case COLOR_24:          /* The color of the (*light) */
	   {
	      Color24 *d;
	      d = ReadChunkData3ds(current);
	      (*light)->color.r = (float3ds)d->red / 255.0F;
	      (*light)->color.g = (float3ds)d->green / 255.0F;
	      (*light)->color.b = (float3ds)d->blue / 255.0F;
	   }
	   break;
        case DL_MULTIPLIER:     /* The light intensity */
	   {
	      DlMultiplier *d;
	      d = ReadChunkData3ds(current);
	      (*light)->multiplier = d->multiple;
	   }
	   break;
        case DL_INNER_RANGE:
	   {
	      DlInnerRange *d;
	      
	      d = ReadChunkData3ds(current);
	      /* assuming since there is a value it is on */
	      (*light)->attenuation.inner = d->range;
	   }
	   break;
        case DL_OUTER_RANGE:
	   {
	      DlOuterRange *d;
	      
	      d = ReadChunkData3ds(current);
	      /* assuming since there is a value it is on */
	      (*light)->attenuation.outer = d->range;
	   }
	   break;
        case DL_EXCLUDE:
	   {
	      DlExclude *d;
	      char3ds *name;
	      
	      d = ReadChunkData3ds(current);
	      if ((*light)->exclude == NULL)
		 InitNameList3ds(&((*light)->exclude), 0);

	      name = d->name;
	      d->name = NULL;
	      
	      AddToNameList3ds(&((*light)->exclude), name);
	      free(name);
	   }
	   break;
        case DL_OFF:
	   (*light)->dloff = True3ds;
	   break;
        case DL_ATTENUATE:
	   (*light)->attenuation.on = True3ds;
	   break;
        }
     }
      
     /*--- DL_SPOTLIGHT chunk */
     if (spot != NULL){
        DlSpotlight *d;
        
        /*--- Read spotlight data */
        d = ReadChunkData3ds(spot);
        (*light)->spot->target = d->spotlighttarg;
        (*light)->spot->hotspot = d->hotspotangle;
        (*light)->spot->falloff = d->falloffangle;
        
        /* scan all the chunks the spotlight contains */
        for (current = spot->children; 
	     current != NULL; 
	     current = current->sibling)
	{
	   switch(current->tag)
	   {
	   case DL_SPOT_ROLL:
	      {
		 DlSpotRoll *d;
		 
		 d = ReadChunkData3ds(current);
		 (*light)->spot->roll = d->angle;
	      }
	      break;
	   case DL_LOCAL_SHADOW:
	      (*light)->spot->shadows.cast = True3ds;
	      break;
	   case DL_LOCAL_SHADOW2:
	      {
		 DlLocalShadow2 *d;
		 d = ReadChunkData3ds(current);
		 
		 (*light)->spot->shadows.bias = d->localshadowbias;
		 (*light)->spot->shadows.filter = d->localshadowfilter;
		 (*light)->spot->shadows.mapsize = d->localshadowmapsize;
		 (*light)->spot->shadows.local = True3ds;
	      }
	      break;
	   case DL_SHADOWED:
	      (*light)->spot->shadows.cast = True3ds;
	      break;
	   case DL_SPOT_RECTANGULAR:
	      (*light)->spot->cone.type = Rectangular;
	      break;
	   case DL_SEE_CONE:
	      (*light)->spot->cone.show = True3ds;
	      break;
	   case DL_SPOT_OVERSHOOT:
	      (*light)->spot->cone.overshoot = True3ds;
	      break;
	   case DL_SPOT_ASPECT:
	      {
		 DlSpotAspect *d;
		 
		 d = ReadChunkData3ds(current);
		 (*light)->spot->aspect = d->aspect;
	      }
	      break;
	   case DL_RAY_BIAS:
	      {
		 DlRayBias *d;
		 
		 d = ReadChunkData3ds(current);
		 (*light)->spot->shadows.raybias = d->bias;
	      }
	      break;
	   case DL_RAYSHAD:
	      {
		 (*light)->spot->shadows.type = UseRayTraceShadow;
		 break;
	      }
	   case DL_SPOT_PROJECTOR:
	      {
		 DlSpotProjector *d;
		 d = ReadChunkData3ds(current);
		 (*light)->spot->projector.bitmap = d->name;
		 (*light)->spot->projector.use = True3ds;
		 d->name = NULL;
	      }
	      break;
            
	   } 
	} 
     } 
  }
  ReleaseChunk3ds(&nobj);
       
}

void PutOmnilight3ds(database3ds *db, light3ds *light)
{
  if (db == NULL || light == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

  PutLight3ds(db, light);
}

void PutSpotlight3ds(database3ds *db, light3ds *light)
{
  if (db == NULL || light == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

  PutLight3ds(db,light);
}


/*----------------------------------------------------------------
 |
 | PutLight3ds
 |
 |  Put chunks in database from light3ds structure.
 |
 +-----------------------------------------------------------------*/
void PutLight3ds(database3ds *db, light3ds *light)
{
  chunk3ds *pMdata, *pNamed, *pLight, *pSpot, *current, *xdata;

  if (db == NULL || light == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  FindChunk3ds(db->topchunk, MDATA, &pMdata);
  if (pMdata == NULL) 
    {
      InitChunk3ds(&pMdata);
      pMdata->tag = MDATA;
      AddChildOrdered3ds(db->topchunk, pMdata);
    }
  
  /* Discover if the named object already exists */
  FindNamedObject3ds(db, light->name, &pNamed);
  
  /* If it does, then delete it */
  if (pNamed != NULL)
    {
      /* First, rescue the xdata chunk if there is one */
      FindChunk3ds(pNamed, XDATA_SECTION, &current);
      
      /* If there is one */
      if (current != NULL)
    {
      /* Then make a copy of it to reintroduce later */
     CopyChunk3ds(current, &xdata);
      }
      
      /* Delete the chunk's children and data, 
     leaving it in the same list order */
      DeleteChunk3ds(pNamed);
      pNamed->tag = NAMED_OBJECT; /* retag the chunk */

   } else /* else pNamed needs to be initialized and added */
   {
      InitChunkAs3ds(&pNamed, NAMED_OBJECT);
      AddChildOrdered3ds(pMdata, pNamed);
   }

  { /*--- NAME */
    NamedObject *d;

    d = InitChunkData3ds(pNamed);
    d->name = strdup(light->name);
    MakeNamedObjectListDirty3ds(db);
  }
  
  {  /*--- N_DIRECT_LIGHT & POSITION  */
    NDirectLight *d;

    pLight = kfPutGenericNode(N_DIRECT_LIGHT, pNamed);
    d = InitChunkData3ds(pLight);
    memcpy(&(d->lightpos), &(light->pos), sizeof(point3ds));
   }

  /*
   | Add children to DIRECT_LIGHT 
   */

  /*--- COLOR */
  {
    ColorF *d;
    chunk3ds *pChunk;

    pChunk = kfPutGenericNode(COLOR_F, pLight);
    d = InitChunkData3ds(pChunk);

    d->red = light->color.r;
    d->green = light->color.g;
    d->blue = light->color.b;
  }

  putTagFloat(pLight, DL_OUTER_RANGE, &light->attenuation.outer);
  ON_ERROR_RETURN;
  
  putTagFloat(pLight, DL_INNER_RANGE, &light->attenuation.inner);
  ON_ERROR_RETURN;
  
  putTagFloat(pLight, DL_MULTIPLIER, &light->multiplier);
  ON_ERROR_RETURN;

  if (light->dloff == True3ds)
    kfPutGenericNode(DL_OFF, pLight);

  if  (light->attenuation.on == True3ds)
    kfPutGenericNode(DL_ATTENUATE, pLight);

  /*--- DL_EXCLUDE */
  if (light->exclude != NULL && light->exclude->count > 0){
    chunk3ds *pChunk;
    ulong3ds i;
    DlExclude *d;

    for(i=0; i<light->exclude->count; i++){
      pChunk = kfPutGenericNode(DL_EXCLUDE, pLight);
      d = InitChunkData3ds(pChunk);
      d->name = strdup((light->exclude->list)[i].name);
    }
  }


  /*-----------------------
   |  SPOTLIGHT SECTION 
   +-----------------------*/
  if (light->spot == NULL)
    return;
  
  {
    DlSpotlight *d;

    pSpot = kfPutGenericNode(DL_SPOTLIGHT, pLight);
    d = InitChunkData3ds(pSpot);
    memcpy(&d->spotlighttarg, &light->spot->target, sizeof(point3ds));
    d->hotspotangle = light->spot->hotspot;
    d->falloffangle = light->spot->falloff;
  }
  
  putTagFloat(pSpot, DL_SPOT_ROLL, &light->spot->roll);  

  if (light->spot->aspect != 1.0F)
    putTagFloat(pSpot, DL_SPOT_ASPECT, &light->spot->aspect);  

  /*--- SHADOWS */
  if (light->spot->shadows.cast == True3ds)
    kfPutGenericNode(DL_SHADOWED, pSpot);
  
  if (light->spot->shadows.type == UseRayTraceShadow)
    kfPutGenericNode(DL_RAYSHAD, pSpot);
  
  putTagFloat(pSpot, DL_RAY_BIAS, &light->spot->shadows.raybias);

  /*--- DL_LOCAL_SHADOW2 */
  if (light->spot->shadows.local == True3ds)
  {
    DlLocalShadow2 *d;
    chunk3ds *pChunk;

    kfPutGenericNode(DL_LOCAL_SHADOW, pSpot);
    pChunk = kfPutGenericNode(DL_LOCAL_SHADOW2, pSpot);
    d = InitChunkData3ds(pChunk);

    d->localshadowbias = light->spot->shadows.bias;
    d->localshadowfilter = light->spot->shadows.filter;
    d->localshadowmapsize = light->spot->shadows.mapsize;
  }
  

  /*--- CONE */
  if (light->spot->cone.type == Rectangular)
    kfPutGenericNode(DL_SPOT_RECTANGULAR, pSpot);
  if (light->spot->cone.show == True3ds)
    kfPutGenericNode(DL_SEE_CONE, pSpot);
  if (light->spot->cone.overshoot == True3ds)
    kfPutGenericNode(DL_SPOT_OVERSHOOT, pSpot);

  /*--- PROJECTOR */
  if (light->spot->projector.use == True3ds)
  {
    DlSpotProjector *d;
    chunk3ds *pChunk;

    pChunk = kfPutGenericNode(DL_SPOT_PROJECTOR, pSpot);
    d = InitChunkData3ds(pChunk);

    d->name = strdup(light->spot->projector.bitmap);
  }
}

static void putTagFloat(chunk3ds *pParentChunk,
            chunktag3ds tag, 
            float3ds *f) 
{
  chunk3ds *pGenericChunk;
  void *pVoid;

  InitChunk3ds(&pGenericChunk);
  ON_ERROR_RETURN;
  
  pGenericChunk->tag = tag;
  AddChildOrdered3ds(pParentChunk, pGenericChunk);

  pVoid = InitChunkData3ds(pGenericChunk);
  ON_ERROR_RETURN;
  
  memcpy(pVoid, f, sizeof(float3ds));
}



