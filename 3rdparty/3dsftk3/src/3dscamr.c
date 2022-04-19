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


/* Module Overview:

   3dscamr.c and 3dscamr.h concern themselves with the handling of cameras
   inside the 3ds mesh section.

*/

#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dscamr.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "3dsutil.h"

void InitCamera3ds(camera3ds **cam)
{
  if (cam == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (*cam == NULL)
   {
      *cam = malloc(sizeof(camera3ds));
      if (*cam == NULL)
	SET_ERROR_RETURN(ERR_NO_MEM);
   }

   (*cam)->name[0] = 0;
   (*cam)->position.x = 0.0f;
   (*cam)->position.y = 0.0f;
   (*cam)->position.z = 0.0f;
   (*cam)->target.x = 1.0f;
   (*cam)->target.y = 1.0f;
   (*cam)->target.z = 1.0f;
   (*cam)->roll = 0.0f;
   (*cam)->fov = 45.0f;
   (*cam)->showcone = False3ds;
   (*cam)->ranges.cam_near = 10.0f;
   (*cam)->ranges.cam_far = 1000.0f;
}

void ReleaseCamera3ds(camera3ds **cam)
{
   if (cam != NULL)
   {
      if (*cam != NULL)
      {
     free(*cam);
      }
      *cam = NULL;
   }
}

ulong3ds GetCameraCount3ds(database3ds *db)
{
   chunk3ds *ncam;
   ulong3ds camcount = 0, i;

   UpdateNamedObjectList3ds(db);

   if (db->objlist == NULL) return 0;

   for (i = 0; i < db->objlist->count; i++)
   {
      FindChunk3ds(db->objlist->list[i].chunk, N_CAMERA, &ncam);
      if (ncam != NULL) camcount++;
   }

   return camcount;
}

void GetCameraNameList3ds(database3ds *db, namelist3ds **list)
{
   chunk3ds *current=NULL, *nobj=NULL, *ncam;
   ulong3ds camcount, i, j;
   
   if (db == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   UpdateNamedObjectList3ds(db);

   camcount = GetCameraCount3ds(db);
   
   InitNameList3ds(list, camcount);
   
   for (i = 0, j = 0; (i < db->objlist->count) && (j < camcount); i++)
   {
      FindChunk3ds(db->objlist->list[i].chunk, N_CAMERA, &ncam);
      if (ncam != NULL)
      {
     (*list)->list[j].name = strdup(db->objlist->list[i].name);
     j++;
      }
   }
}

void GetCameraByIndex3ds(database3ds *db, ulong3ds index, camera3ds **cam)
{
   chunk3ds *ncam;
   ulong3ds i, camcount;

   if (db == NULL || cam == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   UpdateNamedObjectList3ds(db);
   
   for (i = 0, camcount = 0; i < db->objlist->count; i++)
   {
      FindChunk3ds(db->objlist->list[i].chunk, N_CAMERA, &ncam);
      if (ncam != NULL)
      {
     camcount++;
     if (camcount-1 == index) GetCameraEntry3ds(db->objlist->list[i].chunk, cam);
      }
   }
}

void GetCameraByName3ds(database3ds *db, char3ds *name, camera3ds **cam)
{
   chunk3ds *nobj, *ncam;

   if (db == NULL || name == NULL || cam == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   UpdateNamedObjectList3ds(db);

   FindNamedObject3ds(db, name, &nobj);

   if (nobj != NULL)
   {
      FindChunk3ds(nobj, N_CAMERA, &ncam);
      if (ncam == NULL)
	SET_ERROR_RETURN(ERR_WRONG_OBJECT);
      GetCameraEntry3ds(nobj, cam);
   }
}

void DeleteCameraByName3ds(database3ds *db, char3ds *name)
{
   
   DeleteNamedObjectByName3ds(db, name);
   ON_ERROR_RETURN;
}

void DeleteCameraByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteChunkTypeByIndex3ds(db, N_CAMERA, index);
   ON_ERROR_RETURN;
}

void CopyCameraByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{

   CopyNamedObjectByIndex3ds(destdb, srcdb, N_CAMERA, index);
   ON_ERROR_RETURN;
}

void CopyCameraByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNamedObjectByName3ds(destdb, srcdb, name);
   ON_ERROR_RETURN;
}

void PutCamera3ds(database3ds *db, camera3ds *cam)
{
   chunk3ds *mdata, *nobj = NULL, *ncamera = NULL, *current = NULL;
   NamedObject *nobjdata;
   NCamera *ncameradata;
   chunk3ds *xdata = NULL;

   if (db == NULL || cam == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (db->topchunk == NULL)
     SET_ERROR_RETURN(ERR_INVALID_DATABASE);

   if (!(db->topchunk->tag == M3DMAGIC) || (db->topchunk->tag == CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   FindChunk3ds(db->topchunk, MDATA, &mdata);

   if (mdata == NULL) 
   {
      InitChunk3ds(&mdata);
      AddChildOrdered3ds(db->topchunk, mdata);
   }
      
   FindNamedObject3ds(db, cam->name, &nobj);

   if (nobj != NULL)
   {
      FindChunk3ds(nobj, XDATA_SECTION, &xdata);
      DeleteChunk3ds(nobj);
      nobj = NULL;
   }
	 
   nobjdata = InitChunkAndData3ds(&nobj, NAMED_OBJECT);
   ON_ERROR_RETURN;
   
   nobjdata->name = malloc(strlen(cam->name)+1);
   strcpy(nobjdata->name, cam->name);

   AddChildOrdered3ds(mdata, nobj);
   MakeNamedObjectListDirty3ds(db);

   InitChunk3ds(&ncamera);
   ncamera->tag = N_CAMERA;
   AddChild3ds(nobj, ncamera);
	  
   /* Fill in the camera chunk definition */
   ncamera->size = HeaderSize3ds + 2*PointSize3ds + 2*FloatSize3ds;

   ncameradata = malloc(sizeof(NCamera));
   ncamera->data = ncameradata;
	 
   ncameradata->camerapos.x = cam->position.x;
   ncameradata->camerapos.y = cam->position.y;
   ncameradata->camerapos.z = cam->position.z;
   ncameradata->targetpos.x = cam->target.x;
   ncameradata->targetpos.y = cam->target.y;
   ncameradata->targetpos.z = cam->target.z;
   ncameradata->camerabank = cam->roll;
   ncameradata->camerafocallength = 2400.0f / cam->fov;
      
   if (cam->showcone) 
   {
      chunk3ds *showcone = NULL;

      InitChunk3ds(&showcone);
      showcone->tag = CAM_SEE_CONE;
      showcone->size = 6;

      AddChild3ds(ncamera, showcone);
      ncamera->size += showcone->size;
   }

   /* if (1) */
   {
      chunk3ds *ranges = NULL;
      
      InitChunk3ds(&ranges);
      ranges->tag = CAM_RANGES;
      ranges->size = 2*FloatSize3ds;
      ranges->data = malloc(sizeof(CamRanges));
      ((CamRanges *)(ranges->data))->nearplane = cam->ranges.cam_near;
      ((CamRanges *)(ranges->data))->farplane = cam->ranges.cam_far;
      
      AddChild3ds(ncamera, ranges);
      ncamera->size += ranges->size;
      
   }

   if (xdata != NULL)
   {
      AddChild3ds(ncamera, xdata);
      ncamera->size += xdata->size;
   }
}
   
void GetCameraEntry3ds(chunk3ds *chunk, camera3ds **cam)
{
   chunk3ds *ncam, *current, *nobj;
   NCamera *data;
   
   if(chunk == NULL || cam == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (chunk->tag != NAMED_OBJECT)
     SET_ERROR_RETURN(ERR_WRONG_OBJECT);

   FindNextChunk3ds(chunk->children, N_CAMERA, &ncam);
   if (ncam == NULL) SET_ERROR_RETURN(ERR_WRONG_OBJECT);

   InitCamera3ds(cam);
   ON_ERROR_RETURN;

   CopyChunk3ds(chunk, &nobj);
   ON_ERROR_RETURN;

   FindNextChunk3ds(nobj->children, N_CAMERA, &ncam);

   ReadChunkData3ds(nobj);
   ON_ERROR_RETURN;

   strcpy((*cam)->name, ((NamedObject *)(nobj->data))->name);

   ReadChunkData3ds(ncam);
   ON_ERROR_RETURN;
   
   data = ncam->data;
   (*cam)->position.x = data->camerapos.x;
   (*cam)->position.y = data->camerapos.y;
   (*cam)->position.z = data->camerapos.z;
   (*cam)->target.x = data->targetpos.x;
   (*cam)->target.y = data->targetpos.y;
   (*cam)->target.z = data->targetpos.z;
   (*cam)->roll = data->camerabank;
   (*cam)->fov = 2400.0f / data->camerafocallength;

   for (current = ncam->children; current != NULL; current = current->sibling)
   {
      switch(current->tag)
      {
      case CAM_SEE_CONE:
	 (*cam)->showcone = True3ds;
	 break;
      case CAM_RANGES:
      {
	 CamRanges *data;
	 ReadChunkData3ds(current);
	 data = current->data;

	 (*cam)->ranges.cam_near = data->nearplane;
	 (*cam)->ranges.cam_far = data->farplane;
	 break;
      }
      }
   }
   /* ReleaseChunk3ds(&ncam); */
   ReleaseChunk3ds(&nobj); /* CCJ */
}


#ifdef NOTWORKING
short3ds AddCamera3ds(const char3ds *name, const point3ds *pos, const point3ds *targ, float3ds bnkang, float3ds foc, ushort3ds cone)
{
   ReturnIfError3ds();

   OpenNamedObject3ds(name);
   ReturnIfError3ds();

   if (cone == True3ds) cone = CamSeeCone3ds;

   WriteNCamera3ds(pos, targ, bnkang, foc, cone);
   ReturnIfError3ds();

   CloseNamedObject3ds();
   ReturnIfError3ds();

   ReturnSuccess3ds();
}


#endif

