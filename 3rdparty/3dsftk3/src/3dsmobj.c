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
#include "3dsmobj.h"
#include "3dsftkst.h"
#include "dbase3ds.h"
#include "3dsiobj.h"
#include "3dsutil.h"



const objmat3ds DefObjMat3ds = {{0}, 0, NULL};

/* Returns a count of mesh objects referenced in the chunk list */
ulong3ds GetMeshCount3ds(database3ds *db /* The chunk listing that is being searched */
			)
{
   chunk3ds *ntri; /* The possible pointer for the mesh chunk */
   ulong3ds meshcount = 0, i;

   /* Update the index to named objects if the list has changed recently */
   UpdateNamedObjectList3ds(db);
   ON_ERROR_RETURNR(0);

   if (db->objlist == NULL) return 0;
   
   /* Scan through the list of named objects */
   for (i = 0; i < db->objlist->count; i++)
   {
      /* Search each named object for a mesh chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_TRI_OBJECT, &ntri);
      /* if a mesh chunk was found, increment the count */
      if (ntri != NULL) meshcount++;
   }

   return meshcount;
}

/* Searches *db to construct **list, a list of mesh names associated with
   their chunk definition. */
void GetMeshNameList3ds(database3ds *db, /* The chunk database being searched */
			namelist3ds **list /* The resulting namelist pairs, if
					      *list is null, then a namelist
					      structure will be allocated */
			)
{
   chunk3ds *current=NULL, *nobj=NULL, *ntri;
   ulong3ds meshcount, i, j;
   
  if (db == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Update the index to named objects if the list has changed recently */
   UpdateNamedObjectList3ds(db);

   /* Get the number of meshes contained in the named object list */
   meshcount = GetMeshCount3ds(db);

   /* Initilize the namelist array */
   InitNameList3ds(list, meshcount);

   /* Scan through the list of named objects */
   for (i = 0, j = 0; (i < db->objlist->count) && (j < meshcount); i++)
   {
      /* Search each named object for a mesh chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_TRI_OBJECT, &ntri);

      /* If it is a mesh chunk */
      if (ntri != NULL)
      {
	 /* Copy the name into the list */
	 (*list)->list[j].name = strdup(db->objlist->list[i].name);
	 j++;
      }
   }
}

/* Fills out the **mesh structure from the (index)th mesh reference found in *db */
void GetMeshByIndex3ds(database3ds *db, ulong3ds index, mesh3ds **mesh)
{
   chunk3ds *ntri;
   ulong3ds i, meshcount;

   if (db == NULL || mesh == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Update the index to named objects if the list has changed recently */
   UpdateNamedObjectList3ds(db);

   /* Scan through the list of named objects */
   for (i = 0, meshcount = 0; i < db->objlist->count; i++)
   {
      /* Search each named object for a mesh chunk */
      FindChunk3ds(db->objlist->list[i].chunk, N_TRI_OBJECT, &ntri);

      /* If a mesh chunk is found */
      if (ntri != NULL)
      {
	 /* Increment the running total */
	 meshcount++;
	 /* If this is the (index)th mesh, fill out the structure */
	 if (meshcount-1 == index)
	 {
	    GetMeshEntryChunk3ds(db->objlist->list[i].chunk, mesh);
	    break;
	 }
      }
   }
}

/* Fills out the **mesh structure contained in *db with the name listed in *name */
void GetMeshByName3ds(database3ds *db, /* Chunk listing to be searched */
		      char3ds *name, /* Name of mesh being searched for */
		      mesh3ds **mesh /* A pointer to the address of the mesh
					structure, if (*mesh) is null, then
					memory will be allocated for the new
					structure, otherwise, the existing
					structure will be filled in with the new
					values.  If no match is found, then **mesh
					remains unchanged */
		      )
{
   chunk3ds *nobj = NULL, *ntri = NULL;

   if (db == NULL || name == NULL || mesh == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   UpdateNamedObjectList3ds(db);

   /* Find the matching mesh name in the chunk list */
   FindNamedObject3ds(db, name, &nobj);

   /* if a match is found */
   if (nobj != NULL)
   {
      /* See it the named object is a mesh object */
      FindChunk3ds(nobj, N_TRI_OBJECT, &ntri);

      /* If it is a mesh object, then fill in the mesh structure */
      if (ntri != NULL)
	 GetMeshEntryChunk3ds(nobj, mesh);
   }
}

void DeleteMeshByName3ds(database3ds *db, char3ds *name)
{
   DeleteNamedObjectByName3ds(db, name);
   ON_ERROR_RETURN;

}

void DeleteMeshByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteChunkTypeByIndex3ds(db, N_TRI_OBJECT, index);
   ON_ERROR_RETURN;
}

void CopyMeshByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{

   CopyNamedObjectByIndex3ds(destdb, srcdb, N_TRI_OBJECT, index);
   ON_ERROR_RETURN;
}

void CopyMeshByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNamedObjectByName3ds(destdb, srcdb, name);
   ON_ERROR_RETURN;
}

/* Function aids the GetMeshEntryChunk3ds in determining
   how many materials are defined within the mesh object */

ulong3ds GetMeshMatCount3ds(chunk3ds *current)
{
   chunk3ds *datachunk;
   ushort3ds meshmatcount=0;

   FindChunk3ds(current, MSH_MAT_GROUP, &datachunk);
   
   while(datachunk != NULL)
   {
      meshmatcount++;
      FindNextChunk3ds(datachunk->sibling, MSH_MAT_GROUP, &datachunk);
   }
   return(meshmatcount);
}

void GetMeshEntryChunk3ds(chunk3ds *chunk, mesh3ds **mesh)
{
    chunk3ds *nobj=NULL, *datachunk=NULL, *ntrichunk, *current;
    void *data = NULL;
    ushort3ds    meshmatcount=0;

    if (chunk == NULL || mesh == NULL)
       SET_ERROR_RETURN(ERR_INVALID_ARG);
   
    FindNextChunk3ds(chunk->children, N_TRI_OBJECT, &ntrichunk);
    if (ntrichunk == NULL)
       SET_ERROR_RETURN(ERR_WRONG_OBJECT);

    InitMeshObj3ds(mesh,0,0,0);

    /* Copy the mesh chunk into a temporary chunk */
    CopyChunk3ds(chunk, &nobj);
    ON_ERROR_RETURN;

    /* Get the N_TRI_OBJECT part out of it. */
    FindNextChunk3ds(nobj->children, N_TRI_OBJECT, &ntrichunk);
    
   /* Get the mesh name. */
   {
       NamedObject *d;
       d = ReadChunkData3ds(nobj);
       strcpy((*mesh)->name, d->name);
   }
   
   for (current = ntrichunk->children; current != NULL; current = current->sibling)
   {
       switch(current->tag)
       {
      case POINT_ARRAY:
      {
	 PointArray *d;
	 d = ReadChunkData3ds(current);
	 (*mesh)->nvertices = d->vertices;
	 (*mesh)->vertexarray = d->pointlist;
	 d->pointlist = NULL;
	 break;
      }
      case POINT_FLAG_ARRAY:
      {
	 PointFlagArray *flags;

	 flags = ReadChunkData3ds(current);
	 (*mesh)->nvflags = flags->flags;
	 (*mesh)->vflagarray = flags->flaglist;
	 flags->flaglist = NULL;
	 break;
      }
	    
      case FACE_ARRAY:
      {
	 FaceArray *d;
	 
	 d = ReadChunkData3ds(current);
	 (*mesh)->nfaces = d->faces;
	 (*mesh)->facearray = d->facelist;
	 d->facelist = NULL;

	 if (current->children != NULL)
	 {
	    /* begin search for MESH_MAT_GROUP and SMOOTH_GROUP */
	    chunk3ds *facearraychunk=current;
             
	    /* create a list of all mesh mat groups */

	    FindChunk3ds(facearraychunk, MSH_MAT_GROUP, &datachunk);
	    if (datachunk != NULL)
	    {
	       ulong3ds index;
	       
	       meshmatcount = (ushort3ds)GetMeshMatCount3ds(datachunk);
	       (*mesh)->nmats = meshmatcount;
	       (*mesh)->matarray = calloc(meshmatcount, sizeof(objmat3ds));

	       for (index = 0; index < (*mesh)->nmats; index++)
	       {
		  MshMatGroup *omat = NULL;
	     
		  omat = ReadChunkData3ds(datachunk);
	   	     
		  strcpy((*mesh)->matarray[index].name, omat->matname);
		  (*mesh)->matarray[index].nfaces = omat->faces;

		  (*mesh)->matarray[index].faceindex = omat->facelist;
		  omat->facelist = NULL;
	       
		  FindNextChunk3ds(datachunk->sibling, MSH_MAT_GROUP, &datachunk);

	       }
	    }
	 
	    FindNextChunk3ds(facearraychunk->children, SMOOTH_GROUP, &datachunk);
	    if (datachunk != NULL) {
	       SmoothGroup *sm;
	       long3ds smelements;
	       
	       sm = ReadChunkData3ds(datachunk);
	       smelements = (datachunk->size - 6)/LongSize3ds;
	       
	       (*mesh)->smootharray =  sm->grouplist;
	       sm->grouplist = NULL;
	    }

	    FindNextChunk3ds(facearraychunk->children, MSH_BOXMAP, &datachunk);
	    if (datachunk != NULL)
	    {
	       MshBoxmap *bm;
	       ushort3ds i;
	 
	       ReadChunkData3ds(datachunk);
	       bm = datachunk->data;
	       for (i = 0; i < 6; i++) strcpy((*mesh)->boxmap[i], bm->matnames[i]);
	       (*mesh)->useboxmap = True3ds;
	    }
      
	 }
	 break;
      }

      case TEX_VERTS:
      {
	 TexVerts *tv;
	 
	 ReadChunkData3ds(current);
	 tv = current->data;

	 (*mesh)->ntextverts = tv->numcoords;
	 (*mesh)->textarray = tv->textvertlist;
	 tv->textvertlist = NULL;
	 break;
      }

      case MESH_MATRIX:
      {
	 ReadChunkData3ds(current);
	 data = current->data;
	 memcpy((*mesh)->locmatrix ,((MeshMatrix *)data)->xmatrix, 12*sizeof(float3ds));
	 break;
      }
      case MESH_TEXTURE_INFO:
      {
	 (*mesh)->usemapinfo = True3ds;
	 ReadChunkData3ds(current);
	 data = current->data;
	 (*mesh)->map.maptype = ((MeshTextureInfo *)data)->maptype;
	 (*mesh)->map.tilex   = ((MeshTextureInfo *)data)->xtiling;
	 (*mesh)->map.tiley   = ((MeshTextureInfo *)data)->ytiling;
	 (*mesh)->map.cenx    = ((MeshTextureInfo *)data)->iconpos.x;
	 (*mesh)->map.ceny    = ((MeshTextureInfo *)data)->iconpos.y;
	 (*mesh)->map.cenz    = ((MeshTextureInfo *)data)->iconpos.z;
	 (*mesh)->map.scale   = ((MeshTextureInfo *)data)->iconscaling;
	 memcpy((*mesh)->map.matrix , ((MeshTextureInfo *)data)->xmatrix, 12*sizeof(float3ds));
	 (*mesh)->map.pw   = ((MeshTextureInfo *)data)->iconwidth;
	 (*mesh)->map.ph   = ((MeshTextureInfo *)data)->iconheight;
	 (*mesh)->map.ch   = ((MeshTextureInfo *)data)->cyliconheight;
	 break;
      }
      case PROC_NAME:
      {
	 ReadChunkData3ds(current);
	 data = current->data;
	 strcpy((*mesh)->procname, ((ProcName *)data)->name);
	 break;
      }
      case PROC_DATA:
      {
			IpasData *d;	
			d = ReadChunkData3ds(current);
	 		(*mesh)->procsize = d->size;
	 		(*mesh)->procdata = d->data;
	 		d->data = NULL; /* CCJ */
	 		break;
      }
      case MESH_COLOR:
      {
	 ReadChunkData3ds(current);
	 (*mesh)->meshcolor = ((MeshColor *)(current->data))->color;
	 break;
      }
      }
      
   }

   FindNextChunk3ds(nobj->children, OBJ_HIDDEN, &current);
   if (current != NULL) (*mesh)->ishidden = True3ds;
   else (*mesh)->ishidden = False3ds;

   FindNextChunk3ds(nobj->children, OBJ_VIS_LOFTER, &current);
   if (current != NULL) (*mesh)->isvislofter = True3ds;
   else (*mesh)->isvislofter = False3ds;

   FindNextChunk3ds(nobj->children, OBJ_DOESNT_CAST, &current);
   if (current != NULL) (*mesh)->isnocast = True3ds;
   else (*mesh)->isnocast = False3ds;

   FindNextChunk3ds(nobj->children, OBJ_MATTE, &current);
   if (current != NULL) (*mesh)->ismatte = True3ds;
   else (*mesh)->ismatte = False3ds;
      
   FindNextChunk3ds(nobj->children, OBJ_FAST, &current);
   if (current != NULL) (*mesh)->isfast = True3ds;
   else (*mesh)->isfast = False3ds;
   
   FindNextChunk3ds(nobj->children, OBJ_FROZEN, &current);
   if (current != NULL) (*mesh)->isfrozen = True3ds;
   else (*mesh)->isfrozen = False3ds;

   FindNextChunk3ds(nobj->children, OBJ_DONT_RCVSHADOW, &current);
   if (current != NULL) (*mesh)->isnorcvshad = True3ds;
   else (*mesh)->isnorcvshad = False3ds;

	FindNextChunk3ds(nobj->children, OBJ_PROCEDURAL, &current);
	if (current != NULL) (*mesh)->useproc = True3ds;
	else (*mesh)->useproc = False3ds;

   ReleaseChunk3ds(&nobj);
}

/* Puts a mesh3ds into the chunk database.  Note that this routine will clobber
   named objects of the same name, except that it will transfer the old xdata onto
   the new object. If the developer wants to wipe out the XDATA too, he'll have
   to explicitly delete using the DeleteMesh3ds function */
void PutMesh3ds(database3ds *db, mesh3ds *mesh)
{
   chunk3ds *mdata, *nobj, *ntri, *current;
   NamedObject *nobjdata;
   chunk3ds *xdata = NULL;
   long3ds i;


   if (db == NULL || mesh == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   FindChunk3ds(db->topchunk, MDATA, &mdata);

   if (mdata == NULL) 
   {
      InitChunk3ds(&mdata);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      mdata->tag = MDATA;
      AddChildOrdered3ds(db->topchunk, mdata);
   }

   /* Discover if the named object already exists */
   FindNamedObject3ds(db, mesh->name, &nobj);
   ON_ERROR_RETURN;
   
   /* If it does, then delete it */
   if (nobj != NULL)
   {
      /* First, rescue the xdata chunk if there is one */
      FindChunk3ds(nobj, XDATA_SECTION, &current);

      /* If there is one */
      if (current != NULL)
      {
	 /* Then make a copy of it to reintroduce later */
	 CopyChunk3ds(current, &xdata);
	 ADD_ERROR_RETURN(ERR_PUT_FAIL);
      }
      
      /* Delete the chunk's children and data, leaving it in the same list order */
      DeleteChunk3ds(nobj);
      nobj->tag = NAMED_OBJECT; /* retag the chunk */

   } else /* else nobj needs to be initialized and added */
   {
      InitChunkAs3ds(&nobj, NAMED_OBJECT);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);

      AddChildOrdered3ds(mdata, nobj);
      
   }
   

   /* Copy the mesh name into the chunk */
   nobjdata = InitChunkData3ds(nobj);
   ADD_ERROR_RETURN(ERR_PUT_FAIL);
   nobjdata->name = strdup(mesh->name);
   MakeNamedObjectListDirty3ds(db);

   InitChunk3ds(&ntri);
   ADD_ERROR_RETURN(ERR_PUT_FAIL);
   ntri->tag = N_TRI_OBJECT;
   AddChildOrdered3ds(nobj, ntri);

   if ((mesh->nvertices > 0) && (mesh->vertexarray != NULL))
   { /* Add the point array */
      PointArray *d;
      chunk3ds *temp = NULL;
      
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = POINT_ARRAY;
      d = InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      d->vertices = mesh->nvertices;
      d->pointlist = malloc(sizeof(point3ds)*d->vertices);
      if (d->pointlist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      memcpy(d->pointlist, mesh->vertexarray, sizeof(point3ds)*d->vertices);
      AddChildOrdered3ds(ntri, temp);
   }

   if (mesh->ntextverts > 0)
   {
      TexVerts *d;
      chunk3ds *temp = NULL;
      
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = TEX_VERTS;
      d = InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      d->numcoords = mesh->ntextverts;
      d->textvertlist = malloc(sizeof(textvert3ds)*d->numcoords);
      if (d->textvertlist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      memcpy(d->textvertlist, mesh->textarray, sizeof(textvert3ds)*d->numcoords);
      AddChildOrdered3ds(ntri, temp);
   }

   if (mesh->usemapinfo)
   {
      MeshTextureInfo *d;
      chunk3ds *temp;

      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = MESH_TEXTURE_INFO;
      d = InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);

      d->maptype = mesh->map.maptype;
      d->xtiling = mesh->map.tilex;
      d->ytiling = mesh->map.tiley;      
      d->iconpos.x = mesh->map.cenx;
      d->iconpos.y = mesh->map.ceny;
      d->iconpos.z = mesh->map.cenz;
      d->iconscaling = mesh->map.scale;
      memcpy(d->xmatrix, mesh->map.matrix, sizeof(float3ds)*12);
      d->iconwidth = mesh->map.pw;
      d->iconheight = mesh->map.ph;
      d->cyliconheight = mesh->map.ch;
      AddChildOrdered3ds(ntri, temp);
   }
   
   if ((mesh->nvflags > 0) && (mesh->vflagarray != NULL))
   {
      PointFlagArray *d;
      chunk3ds *temp = NULL;
      
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = POINT_FLAG_ARRAY;
      d = InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      d->flags = mesh->nvflags;
      d->flaglist = malloc(sizeof(ushort3ds)*d->flags);
      if (d->flaglist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      memcpy(d->flaglist, mesh->vflagarray, sizeof(ushort3ds)*d->flags);
      AddChildOrdered3ds(ntri, temp);
   }

   
   {
      MeshMatrix *d;
      chunk3ds *temp = NULL;

      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = MESH_MATRIX;
      d = InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      memcpy(d->xmatrix, mesh->locmatrix, sizeof(float3ds)*12);
      AddChildOrdered3ds(ntri, temp);
   }
   

   {
      chunk3ds *temp = NULL;

      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = MESH_COLOR;
      InitChunkData3ds(temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      ((MeshColor *)(temp->data))->color = mesh->meshcolor;
      AddChildOrdered3ds(ntri, temp);
   }

   if (mesh->nfaces > 0)
   {
      chunk3ds *farr = NULL;
      FaceArray *d;

      InitChunk3ds(&farr);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      farr->tag = FACE_ARRAY;
      d = InitChunkData3ds(farr);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      farr->data = d;
      d->faces = mesh->nfaces;
      d->facelist = malloc(sizeof(face3ds)*d->faces);
      if (d->facelist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      memcpy(d->facelist, mesh->facearray, sizeof(face3ds)*d->faces);
      AddChildOrdered3ds(ntri, farr);

      if (mesh->nmats > 0)
      {
	 chunk3ds *matgroup;
	 ushort3ds x;
	 MshMatGroup *d;
	 
	 for (x = 0; x < mesh->nmats; x++)
	 {
	    matgroup = NULL;
	    InitChunk3ds(&matgroup);
	    ADD_ERROR_RETURN(ERR_PUT_FAIL);
	    matgroup->tag = MSH_MAT_GROUP;
	    d = InitChunkData3ds(matgroup);
	    ADD_ERROR_RETURN(ERR_PUT_FAIL);
	    d->matname = strdup(mesh->matarray[x].name);
	    d->faces = mesh->matarray[x].nfaces;
	    if (d->faces > 0)
	    {
	       d->facelist = malloc(sizeof(short3ds)*d->faces);
	       if (d->facelist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	       memcpy(d->facelist, mesh->matarray[x].faceindex, sizeof(short3ds)*d->faces);
	    } else
	    {
	       d->facelist = NULL;
	    }
	    AddChildOrdered3ds(farr, matgroup);
	 }
      }
      
      if (mesh->smootharray != NULL)
      {
	 chunk3ds *smooth = NULL;
	 SmoothGroup *d;
	 
	 InitChunk3ds(&smooth);
	 ADD_ERROR_RETURN(ERR_PUT_FAIL);
	 smooth->tag = SMOOTH_GROUP;
	 d = InitChunkData3ds(smooth);
	 ADD_ERROR_RETURN(ERR_PUT_FAIL);

	 d->groups = mesh->nfaces;
	 d->grouplist = malloc(sizeof(long3ds)*d->groups);
	 if (d->grouplist == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	 memcpy(d->grouplist, mesh->smootharray, sizeof(long3ds)*d->groups);
	 AddChildOrdered3ds(farr, smooth);
      }
      
      if (mesh->useboxmap)
      {
	 chunk3ds *boxmap = NULL;
	 MshBoxmap *d;

	 InitChunk3ds(&boxmap);
	 ADD_ERROR_RETURN(ERR_PUT_FAIL);
	 boxmap->tag = MSH_BOXMAP;
	 d = InitChunkData3ds(boxmap);
	 ADD_ERROR_RETURN(ERR_PUT_FAIL);

	 for (i = 0; i < 6; i++)
	 {
	    d->matnames[i] = strdup(mesh->boxmap[i]);
	 }
	 AddChildOrdered3ds(farr, boxmap);
      }
   }
   
   if (mesh->procdata != NULL)
   {
      chunk3ds *procname, *procdata;
      ProcName *pname;
      IpasData *pdata;

      InitChunk3ds(&procname);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      InitChunk3ds(&procdata);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      procname->tag = PROC_NAME;
      procdata->tag = PROC_DATA;
      pname = InitChunkData3ds(procname);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      pdata = InitChunkData3ds(procdata);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      
      pname->name = strdup(mesh->procname);
      pdata->size = mesh->procsize;
      pdata->data = malloc(pdata->size);
      if (pdata->data == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      memcpy(pdata->data, mesh->procdata, pdata->size);
      AddChildOrdered3ds(ntri, procname);
      AddChildOrdered3ds(ntri, procdata);
   }

   if (mesh->ishidden)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_HIDDEN;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->isvislofter)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_VIS_LOFTER;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->ismatte)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_MATTE;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->isnocast)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_DOESNT_CAST;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->isfast)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_FAST;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->isnorcvshad)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_DONT_RCVSHADOW;
      AddChildOrdered3ds(nobj, temp);
   }

   if (mesh->isfrozen)
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_FROZEN;
      AddChildOrdered3ds(nobj, temp);
   }

   if ((mesh->useproc==True3ds) && (mesh->procdata != NULL))
   {
      chunk3ds *temp;
      InitChunk3ds(&temp);
      ADD_ERROR_RETURN(ERR_PUT_FAIL);
      temp->tag = OBJ_PROCEDURAL;
      AddChildOrdered3ds(nobj, temp);
   }

   if (xdata != NULL)
   {
      AddChildOrdered3ds(ntri, xdata);
   }
    }

