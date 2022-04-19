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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "3dsfile.h"
#include "3dsftkst.h"
#include "chunk3ds.h"
#include "3dsutil.h"
#include "seekchil.h"
#include "dbase3ds.h"
#include "3dshier.h"
#include "3dsvers.h"
#include "3dserr.h"

void InitDatabase3ds(database3ds **db)
{

   if (*db == NULL) {
      *db = (database3ds *)malloc(sizeof(database3ds));
      if ((*db) == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
   }

   (*db)->topchunk = NULL;

/*   (*db)->readfile = 0;
   (*db)->writefile = 0; */
   (*db)->objlistdirty = True3ds;
   (*db)->matlistdirty = True3ds;
   (*db)->nodelistdirty = True3ds;
   (*db)->objlist = NULL;
   (*db)->matlist = NULL;
   (*db)->nodelist = NULL;
      
}

void ReleaseDatabase3ds(database3ds **db)
{
   if (db != NULL)
   {
      if (*db != NULL)
      {
     if ((*db)->topchunk != NULL) 
     {
        ReleaseChunk3ds(&((*db)->topchunk));
     }
     if((*db)->objlist){
     ReleaseChunkList3ds(&((*db)->objlist));
       ON_ERROR_RETURN;
     }

     if((*db)->matlist){
     ReleaseChunkList3ds(&((*db)->matlist));
       ON_ERROR_RETURN;
     }

     if((*db)->nodelist){
     ReleaseChunkList3ds(&((*db)->nodelist));
       ON_ERROR_RETURN;
     }
                  
     free (*db);
     *db = NULL;
      }
   }
}

void UpdateNamedObjectList3ds(database3ds *db)
{
   chunk3ds *mdata = NULL, *nobj = NULL;
   long3ds objcount = 0, i;
   NamedObject *data;
   
   if (db == NULL) 
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (db->objlistdirty)
   {
      db->objlistdirty = False3ds;
      
      ReleaseChunkList3ds(&(db->objlist));
      ON_ERROR_RETURN;

      FindChunk3ds(db->topchunk, MDATA, &mdata);

      if (mdata != NULL)
      {
	 for (FindChunk3ds(mdata, NAMED_OBJECT, &nobj);
	      nobj != NULL;
	      FindNextChunk3ds(nobj->sibling, NAMED_OBJECT, &nobj)
	      )
	 {
	    ON_ERROR_RETURN;
	    objcount++;
	 }
      }

      InitChunkList3ds(&(db->objlist), objcount);
      ON_ERROR_RETURN;

      if (mdata == NULL) return;
      
      for (FindChunk3ds(mdata, NAMED_OBJECT, &nobj), i = 0;
	   nobj != NULL;
	   FindNextChunk3ds(nobj->sibling, NAMED_OBJECT, &nobj), i++
	   )
      {
	 ReadChunkData3ds(nobj);
	 ON_ERROR_RETURN;

	 data = nobj->data;

	 db->objlist->list[i].chunk = nobj;
	 db->objlist->list[i].name = strdup(data->name);
      }
   }
}

void FindNamedObject3ds(database3ds *db, char3ds *name, chunk3ds **found)
{
   chunk3ds *current = NULL;
   ulong3ds i;

   if (db == NULL || name == NULL || found == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   UpdateNamedObjectList3ds(db);
   ON_ERROR_RETURN;

   for(i = 0; i < db->objlist->count; i++)
   {
      if (strcmp(name, db->objlist->list[i].name) == 0)
      {
     current = db->objlist->list[i].chunk;
     break;
      }
   }

   *found = current;
}

/* FindNamedObjectByIndex: Searches the database for a named object by index position and
   object type. Returns the NAMED_OBJECT chunk if found, NULL otherwise */
chunk3ds *FindNamedObjectByIndex3ds(database3ds *db, chunktag3ds type, ulong3ds index)
{
   chunk3ds *chunk;
   ulong3ds i, count;

   UpdateNamedObjectList3ds(db);

   for (i = 0, count = 0; i <db->objlist->count; i++)
   {
      if (type == DL_SPOTLIGHT)
      {
	 FindChunk3ds(db->objlist->list[i].chunk, N_DIRECT_LIGHT, &chunk);
	 if (chunk != NULL)
	    FindChunk3ds(chunk, type, &chunk);
      } else
      {
	 FindChunk3ds(db->objlist->list[i].chunk, type, &chunk);
      }

      if (chunk != NULL)
      {
	 if (count == index)
	    return db->objlist->list[i].chunk;
	 count++;
      }
   }

   return NULL;
}
      
/*-----------------------------------------------------------
 | DeleteChunkTypeByIndex3ds
 |   
 | Deletes an indexed chunk by type 
 |  type : N_CAMERA, etc.
 |
 +----------------------------------------------------------*/
void DeleteChunkTypeByIndex3ds(database3ds *db, 
			       chunktag3ds type, 
			       ulong3ds index)
{
  chunk3ds *pChunk;

  if (db == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

  pChunk = FindNamedObjectByIndex3ds(db, type, index);
  if (pChunk != NULL)
  {
     DeleteChunk3ds(pChunk);
     MakeNamedObjectListDirty3ds(db);
  }
  
}

void DeleteNamedObjectByName3ds(database3ds *db, char3ds *name)
{
   chunk3ds *pChunk;
  
   if (db == NULL || name == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);
  
   FindNamedObject3ds(db, name, &pChunk);

   if (pChunk != NULL)
      DeleteChunk3ds(pChunk);

   MakeNamedObjectListDirty3ds(db);
}


void CopyNamedObjectByIndex3ds(database3ds *destdb, database3ds *srcdb, chunktag3ds nobjtype, ulong3ds index)
{
   chunk3ds *mdata, *nobj, *newnobj;

   if ((destdb == NULL) || (srcdb == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Check to see if the destination database is a 3DS or PRJ */
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find the named object in the source database */
   nobj = FindNamedObjectByIndex3ds(srcdb, nobjtype, index);

   if (nobj != NULL)
   {
      NamedObject *nobjname;
      
      FindChunk3ds(destdb->topchunk, MDATA, &mdata);

      /* If MDATA chunk not present then create one */
      if (mdata == NULL)
      {
	 InitChunkAs3ds(&mdata, MDATA);
	 ON_ERROR_RETURN;
	 AddChildOrdered3ds(destdb->topchunk, mdata);
      }

      /* Get the source object's name */
      nobjname = ReadChunkData3ds(nobj);

      /* Delete any object by that name in the destination */
      DeleteNamedObjectByName3ds(destdb, nobjname->name);
      
      /* Copy the source mesh */
      CopyChunk3ds(nobj, &newnobj);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(mdata, newnobj);

      MakeNamedObjectListDirty3ds(destdb);
   }
}

void CopyNamedObjectByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   chunk3ds *mdata, *nobj, *newnobj;

   if ((destdb == NULL) || (srcdb == NULL) || (name == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);


   /* Find the object in the source database */
   FindNamedObject3ds(srcdb, name, &nobj);
   ON_ERROR_RETURN;
   
   if (nobj != NULL)
   {
      FindChunk3ds(destdb->topchunk, MDATA, &mdata);

      /* If MDATA chunk not present then create one */
      if (mdata == NULL)
      {
	 InitChunkAs3ds(&mdata, MDATA);
	 ON_ERROR_RETURN;
	 AddChildOrdered3ds(destdb->topchunk, mdata);
      }

      /* Delete any object by that name in the destination */
      DeleteNamedObjectByName3ds(destdb, name);
      
      /* Copy the source object */
      CopyChunk3ds(nobj, &newnobj);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(mdata, newnobj);

      MakeNamedObjectListDirty3ds(destdb);
   }
}


#ifdef SPEEDS_OLD_FUNCTION
/*-----------------------------------------------------------
 | DeleteChunkTypeByIndex3ds
 |   
 | Deletes an indexed chunk by type 
 |  type : N_CAMERA, etc.
 |
 +----------------------------------------------------------*/
void DeleteChunkTypeByIndex3ds(database3ds *db, 
			       chunktag3ds type, 
			       ulong3ds index)
{
  chunk3ds *pChunk;
  ulong3ds i, count;

  UpdateNamedObjectList3ds(db);
   
  for (i = 0, count = 0; i < db->objlist->count; i++){

    FindChunk3ds(db->objlist->list[i].chunk, type, &pChunk);

    if(pChunk != NULL){
      if (count == index){
	DeleteChunk3ds(pChunk);
	return;
      }
      count++;
    }
  }
}
#endif


void MakeNamedObjectListDirty3ds(database3ds *db)
{

  if (db == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);

   db->objlistdirty = True3ds;
}

void UpdateMatEntryList3ds(database3ds *db)
{
   chunk3ds *parent = NULL, *current = NULL, *matentry = NULL, *matname = NULL;
   long3ds matcount = 0, i;
   MatName *data;
   
   if (db == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (db->matlistdirty)
   {
      db->matlistdirty = False3ds;
      
      ReleaseChunkList3ds(&(db->matlist));

      FindChunk3ds(db->topchunk, MDATA, &parent);
      if (parent == NULL) FindChunk3ds(db->topchunk, MLIBMAGIC, &parent);

      if (parent != NULL)
      {
     for (FindChunk3ds(parent, MAT_ENTRY, &matentry);
          matentry != NULL;
          FindNextChunk3ds(matentry->sibling, MAT_ENTRY, &matentry)
          )
        matcount++;
      }

      InitChunkList3ds(&(db->matlist), matcount);

      if (parent == NULL) return;
      
      for (FindChunk3ds(parent, MAT_ENTRY, &matentry), i = 0;
       matentry != NULL;
       FindNextChunk3ds(matentry->sibling, MAT_ENTRY, &matentry), i++
       )
      {
     FindChunk3ds(matentry, MAT_NAME, &matname);
     ReadChunkData3ds(matname);
     data = matname->data;

     db->matlist->list[i].chunk = matentry;
     db->matlist->list[i].name = strdup(data->name);
      }
   }
}

void FindMatEntry3ds(database3ds *db, char3ds *name, chunk3ds **found)
{
   chunk3ds *current = NULL;
   ulong3ds i;

   if (db == NULL || name == NULL || found == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((db->topchunk->tag != MLIBMAGIC) && (db->topchunk->tag != M3DMAGIC) &&
       (db->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   UpdateMatEntryList3ds(db);
   ON_ERROR_RETURN;

   for(i = 0; i < db->matlist->count; i++)
   {
      if (strcmp(name, db->matlist->list[i].name) == 0)
      {
     current = db->matlist->list[i].chunk;
     break;
      }
   }

   *found = current;
}

chunk3ds *FindMatEntryByIndex3ds(database3ds *db, ulong3ds index)
{
   if (db == NULL) SET_ERROR_RETURNR(ERR_INVALID_ARG, NULL);
   if (db->topchunk == NULL) SET_ERROR_RETURNR(ERR_INVALID_DATABASE, NULL);
   if ((db->topchunk->tag != MLIBMAGIC) && (db->topchunk->tag != M3DMAGIC) &&
       (db->topchunk->tag != CMAGIC)) SET_ERROR_RETURNR(ERR_WRONG_DATABASE, NULL);

   UpdateMatEntryList3ds(db);
   ON_ERROR_RETURNR(NULL);

   if (index < db->matlist->count)
   {
      return db->matlist->list[index].chunk;
   }
   return NULL;
}
      
void MakeMatEntryListDirty3ds(database3ds *db)
{
  if (db == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);

   db->matlistdirty = True3ds;
}

void UpdateNodeTagList3ds(database3ds *db)
{
   chunk3ds *kfdata = NULL, *nodetag = NULL;
   chunk3ds *ic;
   long3ds nodecount = 0, i;

   myassert(db != NULL, "UpdateNodeTagList3ds: arg *db cannot be NULL");
   myassert(db->topchunk != NULL, "UpdateNodeTagList3ds: arg *db->topchunk cannot be NULL");

   if (db->nodelistdirty)
   {
      
      ReleaseChunkList3ds(&(db->nodelist));

      FindChunk3ds(db->topchunk, KFDATA, &kfdata);

      /* If there is a keyframe section, then cound the number of nodetags around */
      if (kfdata != NULL)
      {
	 for(ic = kfdata->children; ic != NULL; ic = ic->sibling)
	 {
	    switch(ic->tag)
	    {
	    case AMBIENT_NODE_TAG:
	    case OBJECT_NODE_TAG:
	    case CAMERA_NODE_TAG:
	    case TARGET_NODE_TAG:
	    case LIGHT_NODE_TAG:
	    case L_TARGET_NODE_TAG:
	    case SPOTLIGHT_NODE_TAG:
	       nodecount++;
	       break;
	    default:
	       break;
	    }
	    
	 }
      }

      InitChunkList3ds(&(db->nodelist), nodecount);
      ON_ERROR_RETURN;

      if (nodecount == 0) return;

      i = 0;
      
      for (nodetag = kfdata->children; nodetag != NULL; nodetag = nodetag->sibling)
      {
	 switch(nodetag->tag)
	 {
	 case AMBIENT_NODE_TAG:
	 case OBJECT_NODE_TAG:
	 case CAMERA_NODE_TAG:
	 case TARGET_NODE_TAG:
	 case LIGHT_NODE_TAG:
	 case L_TARGET_NODE_TAG:
	 case SPOTLIGHT_NODE_TAG:
	 {
	    chunk3ds *c;
	    NodeHdr *d = NULL;

	    FindNextChunk3ds(nodetag->children, NODE_HDR, &c);
	    if (c != NULL)
	    {
	       d = ReadChunkData3ds(c);
	       ON_ERROR_RETURN;
	       
	       db->nodelist->list[i].chunk = nodetag;
	       db->nodelist->list[i].name = strdup(d->objname);
	       if (db->nodelist->list[i].name == NULL)
		  SET_ERROR_RETURN(ERR_NO_MEM);
	       
	    }

	    /* Object tags may have an instance name as well, which gets appended to
	       the object name with a "." seperator */
	    if (nodetag->tag == OBJECT_NODE_TAG)
	    {
	       FindNextChunk3ds(nodetag->children, INSTANCE_NAME, &c);
	       if (c != NULL)
	       {
		  InstanceName *insd;
		  insd = ReadChunkData3ds(c);
		  ON_ERROR_RETURN;

		  db->nodelist->list[i].name = realloc(db->nodelist->list[i].name, strlen(d->objname)+strlen(insd->name)+2);
		  if (db->nodelist->list[i].name == NULL)
		     SET_ERROR_RETURN(ERR_NO_MEM);
		  strcat(db->nodelist->list[i].name, ".");
		  strcat(db->nodelist->list[i].name, insd->name);
	       }
	    }
	    i++; /* Increment index counter */
	    break;
	 }
	 default:
	    break;
	 }
      }

      myassert(i == nodecount, "UpdateNodeTagList3ds: Difference in node count and node list elements.");
      
      db->nodelistdirty = False3ds;
   }
}

void MakeNodeTagListDirty3ds(database3ds *db)
{
   myassert(db != NULL, "MakeNodeTagListDirt3ds: arg *db cannot be NULL.");
   db->nodelistdirty = True3ds;
}

chunk3ds *FindNodeTagByNameAndType3ds(database3ds *db, char3ds *name, chunktag3ds type)
{
   chunk3ds *current = NULL;
   ulong3ds i;

   myassert(db != NULL, "FindNodeTagByNameAndType3ds: arg *db cannot be NULL.");
   myassert(db != NULL, "FindNodeTagByNameAndType3ds: arg *name cannot be NULL.");
   
   UpdateNodeTagList3ds(db);
   ON_ERROR_RETURNR(NULL);

   for(i = 0; i < db->nodelist->count; i++)
   {
      if ((db->nodelist->list[i].chunk->tag == type) && (strcmp(name, db->nodelist->list[i].name) == 0))
      {
	 return db->nodelist->list[i].chunk;
      }
   }

   return NULL;
}

chunk3ds *FindNodeTagByIndexAndType3ds(database3ds *db, ulong3ds index, chunktag3ds type)
{
   chunk3ds *current = NULL;
   ulong3ds i, icount = 0;

   myassert(db != NULL, "FindNodeTagByNameAndType3ds: arg *db cannot be NULL.");
   myassert(db != NULL, "FindNodeTagByNameAndType3ds: arg *name cannot be NULL.");
   
   UpdateNodeTagList3ds(db);
   ON_ERROR_RETURNR(NULL);

   for(i = 0; i < db->nodelist->count; i++)
   {
      if (db->nodelist->list[i].chunk->tag == type)
      {
	 if (icount == index) return db->nodelist->list[i].chunk;
	 icount++;
      }
   }

   return NULL;
}

/* Deletes a node based on index, if a camera or spotlight, target gets aced as well */
void DeleteNodeTagByIndexAndType3ds(database3ds *db, ulong3ds index, chunktag3ds type)
{
  chunk3ds *nodetag;

  if (db == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  nodetag = FindNodeTagByIndexAndType3ds(db, index, type);
  ON_ERROR_RETURN;
  
  if (nodetag != NULL)
  {
     if ((type == CAMERA_NODE_TAG) || (type == SPOTLIGHT_NODE_TAG))
     {
	chunk3ds *nodehdr, *target;
	NodeHdr *d;

	FindNextChunk3ds(nodetag->children, NODE_HDR, &nodehdr);
	if (nodehdr != NULL)
	{
	   d = ReadChunkData3ds(nodehdr);
	   ON_ERROR_RETURN;
	   if (type == CAMERA_NODE_TAG)
	      target = FindNodeTagByNameAndType3ds(db, d->objname, TARGET_NODE_TAG);
	   else
	      target = FindNodeTagByNameAndType3ds(db, d->objname, L_TARGET_NODE_TAG);
	   ON_ERROR_RETURN;
	   
	   if (target != NULL)
	      DeleteChunk3ds(target);
	}
     }
     DeleteChunk3ds(nodetag);
     MakeNodeTagListDirty3ds(db);
  }
}

/* Deletes a node based on name, if a camera or spotlight, target gets aced as well */
void DeleteNodeTagByNameAndType3ds(database3ds *db, char3ds *name, chunktag3ds type)
{
  chunk3ds *nodetag;

  if ((db == NULL) || (name == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  nodetag = FindNodeTagByNameAndType3ds(db, name, type);
  ON_ERROR_RETURN;
  
  if (nodetag != NULL)
  {

     DeleteChunk3ds(nodetag);
     MakeNodeTagListDirty3ds(db);
  }

  if (type == CAMERA_NODE_TAG)
     DeleteNodeTagByNameAndType3ds(db, name, TARGET_NODE_TAG);
  if (type == SPOTLIGHT_NODE_TAG)
     DeleteNodeTagByNameAndType3ds(db, name, L_TARGET_NODE_TAG);
  ON_ERROR_RETURN;
  
}

void CopyNodeTagByIndexAndType3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index, chunktag3ds type)
{
   chunk3ds *kfdata, *nodetag, *newnode, *nodehdr;
   NodeHdr *nodename;
   

   if ((destdb == NULL) || (srcdb == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);


   /* Find the node to be copied */
   nodetag = FindNodeTagByIndexAndType3ds(srcdb, index, type);
   ON_ERROR_RETURN;
  
   if (nodetag != NULL)
   {
      /* Find or make the destination keyframer section */
      FindNextChunk3ds(destdb->topchunk->children, KFDATA, &kfdata);
      if (kfdata == NULL)
      {
	 InitChunkAs3ds(&kfdata, KFDATA);
	 AddChildOrdered3ds(destdb->topchunk, kfdata);
      }

      /* Get the source node's name */
      FindNextChunk3ds(nodetag->children, NODE_HDR, &nodehdr);
      nodename = ReadChunkData3ds(nodehdr);
      ON_ERROR_RETURN;

      /* Delete any node of the same name in the destination */
      if (type == OBJECT_NODE_TAG){  /* 'cat' the instance name on */
	chunk3ds *pInstChunk;
	char3ds buf[50];
	
	sprintf(buf, "%s", nodename->objname);

	FindNextChunk3ds(nodetag->children, INSTANCE_NAME, &pInstChunk);
	if (pInstChunk){
	  InstanceName *pInstName = ReadChunkData3ds(pInstChunk);
	  if (pInstName)
	    sprintf(buf, "%s.%s", nodename->objname, pInstName->name);
	}
	DeleteNodeTagByNameAndType3ds(destdb, buf, type);
	ON_ERROR_RETURN;
      }
      else{  /*--- Regular non-instance type of node object */
	DeleteNodeTagByNameAndType3ds(destdb, nodename->objname, type);
	ON_ERROR_RETURN;
      }
      
      /* If a camera or a spot, then copy their targets first */
      if (type == CAMERA_NODE_TAG)
	 CopyNodeTagByNameAndType3ds(destdb, srcdb, nodename->objname, TARGET_NODE_TAG);
      if (type == SPOTLIGHT_NODE_TAG)
	 CopyNodeTagByNameAndType3ds(destdb, srcdb, nodename->objname, L_TARGET_NODE_TAG);
      ON_ERROR_RETURN;

      /* Copy the node */
      CopyChunk3ds(nodetag, &newnode);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(kfdata, newnode);

      MakeNodeTagListDirty3ds(destdb);
   }
}

void CopyNodeTagByNameAndType3ds(database3ds *destdb, database3ds *srcdb, char3ds *name, chunktag3ds type)
{
   chunk3ds *kfdata, *nodetag, *newnode;

   if ((destdb == NULL) || (srcdb == NULL) || (name == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);


   /* Find the node to be copied */
   nodetag = FindNodeTagByNameAndType3ds(srcdb, name, type);
   ON_ERROR_RETURN;
  
   if (nodetag != NULL)
   {
      /* Find or make the destination keyframer section */
      FindNextChunk3ds(destdb->topchunk->children, KFDATA, &kfdata);
      if (kfdata == NULL)
      {
	 InitChunkAs3ds(&kfdata, KFDATA);
	 AddChildOrdered3ds(destdb->topchunk, kfdata);
      }

      /* If a camera or a spot, then copy their targets too */
      if (type == CAMERA_NODE_TAG)
	 CopyNodeTagByNameAndType3ds(destdb, srcdb, name, TARGET_NODE_TAG);
      if (type == SPOTLIGHT_NODE_TAG)
	 CopyNodeTagByNameAndType3ds(destdb, srcdb, name, L_TARGET_NODE_TAG);
      

      /* Delete any node of the same name in the destination */
      DeleteNodeTagByNameAndType3ds(destdb, name, type);
      
      /* Copy the node */
      CopyChunk3ds(nodetag, &newnode);
      ON_ERROR_RETURN;
      
      /* Add the copy to the destination */
      AddChildOrdered3ds(kfdata, newnode);

      MakeNodeTagListDirty3ds(destdb);
   }
}

/* Special function to handle child reads when an IPAS routine incorrectly creates and XDATA_ENTRY chunk.  This
   function could be used to recover chunks from any badly formed chunk structure as well. */
void ReadXDataEntryChildren3ds(chunk3ds *parent)
{
   ulong3ds  parentBody=0, childPosition=0, cpos=0, fpos=0;
   chunk3ds *child;

   SetContextByIndex3ds(parent->readindex);

   SeekChild3ds(parent); 

   parentBody = parent->position + parent->size;

   while (GetFilePosition3ds() < parentBody)
   {
      child = NULL;
      InitChunk3ds(&child);
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;
   
      child->position = GetFilePosition3ds();
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;

      child->readindex = GetContextIndex3ds();
      ReadHeader3ds(&(child->tag),&(child->size));
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;

      /* Validate the child chunk... */

      /* First, is it a valid header? */
      switch (child->tag)
      {
         case XDATA_APPNAME:
         case XDATA_STRING:
         case XDATA_FLOAT:
         case XDATA_DOUBLE:
         case XDATA_SHORT:
         case XDATA_LONG:
         case XDATA_VOID:
         case XDATA_GROUP:
         case XDATA_RFU6:
         case XDATA_RFU5:
         case XDATA_RFU4:
         case XDATA_RFU3:
         case XDATA_RFU2:
         case XDATA_RFU1:
         {
            /* Second, does the size fit inside the XDATA_ENTRY chunk? */
            if ((child->position + child->size) <= parentBody)
            {
               /* Chances are, its a good subchunk, so add it in */
               AddChild3ds(parent, child);
               ADD_ERROR(ERR_READING_DATABASE); 
               ON_ERROR_RETURN;
      
               ReadXDataEntryChildren3ds(child);
               ADD_ERROR(ERR_READING_DATABASE); 
               ON_ERROR_RETURN;
            }
			else {
				ReleaseChunk3ds(&child); /* CCJ */
			}
            break;
         }
         default: /* must not be a valid chunk, seek to the end of the parent then */
         {
			ReleaseChunk3ds(&child);	/* CCJ */
            FileSeek3ds(parentBody);
            break;
         }         
      }
   }
}
      

void ReadChildren3ds(chunk3ds *parent)
{
   ulong3ds  parentBody=0, childPosition=0, cpos=0, fpos=0;
   chunk3ds              *child;

   SetContextByIndex3ds(parent->readindex);
   
   SeekChild3ds(parent);
   
   parentBody = parent->position + parent->size;
   
   while (GetFilePosition3ds() < parentBody) {
      child = NULL;
      InitChunk3ds(&child);
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;
   
      child->position = GetFilePosition3ds();
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;

      child->readindex = GetContextIndex3ds();
      ReadHeader3ds(&(child->tag),&(child->size));
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;


#ifdef NOT     
      printf("%s%s %s%d %s%d\n", "Tag: ",
    ChunkTagToString3ds(child->tag), 
         "Size: ",child->size, "Pos: ",child->position); 
#endif

      AddChild3ds(parent, child);
      ADD_ERROR(ERR_READING_DATABASE); 
      ON_ERROR_RETURN;

      if (child->tag == XDATA_ENTRY) 
      {
         ReadXDataEntryChildren3ds(child);
         ADD_ERROR(ERR_READING_DATABASE); 
         ON_ERROR_RETURN;
      } else
      {
         ReadChildren3ds(child);
         ADD_ERROR(ERR_READING_DATABASE); 
         ON_ERROR_RETURN;
      }
   }
}

void WriteChunk3ds(chunk3ds *chunk, ubyte3ds fileindex)
{
   long3ds startpos, endpos;
   chunk3ds *chilindex;
   
   if (chunk == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   myassert(GetContextByIndex3ds(fileindex) != NULL, "WriteChunk3ds: tried to write to nonexistant file3ds.");

   if (chunk != NULL && chunk->tag != PARENT_NAME && chunk->tag != NULL_CHUNK)
   {

      SetContextByIndex3ds(fileindex); /* Select the file to be written too */
      chunk->writeindex = fileindex; /* Store the index of the chunk */
      startpos = GetFilePosition3ds(); /* Get the current location of file */
      ReadChunkData3ds(chunk);  /* Read chunk data if needed */
      ON_ERROR_RETURN;

      /* Reselect the file to written to in case the read occured from another */
      SetContextByIndex3ds(fileindex);

      /* Seek the same position in case the data was read from the same file */
      FileSeek3ds(startpos);
      ON_ERROR_RETURN;

      /* Write the header information, the size may not be correct */
      WriteHeader3ds(chunk->tag, chunk->size);  
      ON_ERROR_RETURN;

      WriteChunkData3ds(chunk);  /* Write the chunk data if needed */
      ON_ERROR_RETURN;

      /* If the chunk has children, then write the children out  */
      for (chilindex = chunk->children; chilindex != NULL; chilindex = chilindex->sibling)
      {
	 WriteChunk3ds(chilindex, fileindex);
	 ON_ERROR_RETURN;
      }

      /* Reselect the file to written to in case the read occured from another file */
      SetContextByIndex3ds(fileindex);  
      endpos = GetFilePosition3ds();  /* Get the end position of the chunk */
   
      /* If the real size of the chunk is not the same as chunk->size fix it up
         the size in the header and *chunk */
      if ((ulong3ds)(endpos - startpos) != chunk->size)
      {
	 FinishHeader3ds(startpos, endpos);  /* FinishHeader3ds returns to endpos after the write */
	 chunk->size = endpos - startpos;
      }

      /* If the write file is available for read change the read index to the write file */
      if (GetContextByIndex3ds(fileindex)->state == ReadWriteFile) chunk->readindex = fileindex;
      ON_ERROR_RETURN;

      /* If there is a valid source for chunk->data then free it up */
      if (GetContextByIndex3ds(chunk->readindex) != NULL) FreeChunkData3ds(chunk);

   }
   

}

void CreateDatabase3ds(file3ds *file, database3ds *db)
{
   chunk3ds *chunk=NULL;

  if (db == NULL || file == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);
   
   if ((file->state == ReadFromFile) || (file->state == ReadWriteFile))
   {
      InitChunk3ds(&chunk);
      ADD_ERROR_RETURN(ERR_CREATING_DATABASE);

      db->topchunk = chunk;
      
      chunk->readindex = file->index;

      SetContext3ds(file);
      ADD_ERROR_RETURN(ERR_CREATING_DATABASE);

      FileSeek3ds(0);
      ADD_ERROR_RETURN(ERR_CREATING_DATABASE);

      ReadHeader3ds(&(chunk->tag), &(chunk->size));
      ADD_ERROR(ERR_CREATING_DATABASE);
      ON_ERROR_RETURN;

      /* Test Chunk Header to determine if current chunk is 
	 Top Level chunk type */
      if ((chunk->tag == M3DMAGIC) || (chunk->tag == CMAGIC) || (chunk->tag == MLIBMAGIC))
      {
	 ReadChildren3ds(db->topchunk);
	 ADD_ERROR_RETURN(ERR_CREATING_DATABASE);

	 AssignParentNames(db);
	 ADD_ERROR_RETURN(ERR_CREATING_DATABASE);
	 
      }
      
   }
}


void CreateNewDatabase3ds(database3ds *db, dbtype3ds type)
{
   chunk3ds *chunk=NULL;

   if (db == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   InitChunk3ds(&chunk);
   ADD_ERROR(ERR_CREATING_DATABASE);   
   ON_ERROR_RETURN;
   
   switch (type)
   {
   case MeshFile:
      chunk->tag = M3DMAGIC;
      break;
   case ProjectFile:
      chunk->tag = CMAGIC;
      break;
   case MaterialFile:
      chunk->tag = MLIBMAGIC;
      break;
   }
   db->topchunk = chunk;

   /* All new databases are created with R3 version */
   PutDatabaseRelease3ds(db, Release33ds);
   
}

dbtype3ds GetDatabaseType3ds(database3ds *db)
{
   if (db == NULL) return Unknown;
   if (db->topchunk == NULL) return Unknown;
      
   switch (db->topchunk->tag)
   {
   case M3DMAGIC:
      return MeshFile;
   case CMAGIC:
      return ProjectFile;
   case MLIBMAGIC:
      return MaterialFile;
   default:
      return Unknown;
   }

}
     

void CopyDatabase3ds(database3ds *idb, database3ds *odb)
{
  if (idb == NULL || odb == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);
   
   CopyChunk3ds(idb->topchunk, &(odb->topchunk));
}

/* DisconnectDatabase3ds sucks up all the data in a file internally, and removes
   references to the file data */
void DisconnectDatabase3ds(database3ds *db)
{
   if (db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);

   DisconnectChunk3ds(db->topchunk);
   ON_ERROR_RETURN;
}

void WriteDatabase3ds(file3ds *file, database3ds *db)
{
   if (file == NULL || db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Since the toolkit can only write out R3 stuff, force the version chunks to R3 before
      writting the database */
   PutDatabaseRelease3ds(db, Release33ds); 

   if (file->state == WriteToFile)
   {
/*      db->writefile = file->index; */

      ComputeNodeIds(db);
      ADD_ERROR(ERR_WRITING_DATABASE);
      ON_ERROR_RETURN;
   
      SetContext3ds(file);
      ADD_ERROR(ERR_WRITING_DATABASE);
      ON_ERROR_RETURN;

      FileSeek3ds(0);  /* Write file rewrites the entire file from the beginning */
      ADD_ERROR(ERR_WRITING_DATABASE);
      ON_ERROR_RETURN;

      db->topchunk->writeindex = file->index;
      
      WriteChunk3ds(db->topchunk, file->index);
      ADD_ERROR(ERR_WRITING_DATABASE);
      ON_ERROR_RETURN;
   }
}

