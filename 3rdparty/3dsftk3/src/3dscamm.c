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
#include "3dsprim.h"
#include "3dserr.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "xdata.h"
#include "3dscamm.h"
#include "kfutils.h"

/*----------------------------------------------------------------------
 |
 | InitCameraMotion3ds
 |
 +---------------------------------------------------------------------*/
void InitCameraMotion3ds(kfcamera3ds **cam, 
             ulong3ds npkeys, 
             ulong3ds nfkeys,
             ulong3ds nrkeys, 
             ulong3ds ntkeys)
{
   ulong3ds i;

   if (*cam == NULL)
   {
      (*cam) = malloc(sizeof(kfcamera3ds));
      if(*cam == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);

      (*cam)->name[0] = 0;
      (*cam)->parent[0] = 0;
      (*cam)->flags1 = (*cam)->flags2 = (ushort3ds)0;
      (*cam)->npkeys  = npkeys;
      (*cam)->nfkeys  = nfkeys;
      (*cam)->nrkeys  = nrkeys;
      (*cam)->tparent[0] = 0;
      (*cam)->ntkeys  = ntkeys;
      (*cam)->pkeys = NULL;
      (*cam)->pos = NULL;
      (*cam)->fkeys = NULL;
      (*cam)->fov = NULL;
      (*cam)->rkeys = NULL;
      (*cam)->roll = NULL;
      (*cam)->tkeys = NULL;
      (*cam)->tpos = NULL;
   }

   if (npkeys != 0)
   {
      (*cam)->npflag = TrackSingle3ds;

      (*cam)->npkeys = npkeys;
      if ((*cam)->pkeys != NULL) {free((*cam)->pkeys); (*cam)->pkeys = NULL;}
      if ((*cam)->pos   != NULL) {free((*cam)->pos);   (*cam)->pos = NULL;}

      (*cam)->pkeys = calloc((*cam)->npkeys, sizeof(keyheader3ds));
      if ((*cam)->pkeys == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*cam)->npkeys; i++)
         memcpy(&(((*cam)->pkeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*cam)->pos   = calloc((*cam)->npkeys, sizeof(point3ds));
      if ((*cam)->pos == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*cam)->npkeys; i++)
         memcpy(&(((*cam)->pos)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
   }

   if (nfkeys != 0)
   {
      (*cam)->nfflag = TrackSingle3ds;

      (*cam)->nfkeys = nfkeys;
      if ((*cam)->fkeys != NULL) {free((*cam)->fkeys); (*cam)->fkeys = NULL;}
      if ((*cam)->fov   != NULL) {free((*cam)->fov);   (*cam)->fov = NULL;}

      (*cam)->fkeys = calloc((*cam)->nfkeys, sizeof(keyheader3ds));
      if((*cam)->fkeys == NULL)  SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*cam)->nfkeys; i++)
         memcpy(&(((*cam)->fkeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*cam)->fov   = calloc((*cam)->nfkeys, sizeof(float3ds));
      if((*cam)->fov == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      for (i = 0; i < (*cam)->nfkeys; i++)
         ((*cam)->fov)[i] = 60.0F;
   }

   if (nrkeys != 0)
   {
      (*cam)->nrflag = TrackSingle3ds;

      (*cam)->nrkeys = nrkeys;
      if ((*cam)->rkeys != NULL) {free((*cam)->rkeys); (*cam)->rkeys = NULL;}
      if ((*cam)->roll  != NULL) {free((*cam)->roll);  (*cam)->roll = NULL;}

      (*cam)->rkeys = calloc((*cam)->nrkeys, sizeof(keyheader3ds));
      if((*cam)->rkeys == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      for (i = 0; i < (*cam)->nrkeys; i++)
         memcpy(&(((*cam)->rkeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*cam)->roll  = calloc((*cam)->nrkeys, sizeof(float3ds));
      if((*cam)->roll == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      for (i = 0; i < (*cam)->nrkeys; i++)
         ((*cam)->roll)[i] = 0.0F;
   }

   if (ntkeys != 0)
   {
      (*cam)->ntflag = TrackSingle3ds;
      (*cam)->tflags1 = (*cam)->tflags2 = (short3ds)0;

      (*cam)->ntkeys = ntkeys;
      if ((*cam)->tkeys != NULL) {free((*cam)->tkeys); (*cam)->tkeys = NULL;}
      if ((*cam)->tpos  != NULL) {free((*cam)->tpos);  (*cam)->tpos = NULL;}

      (*cam)->tkeys = calloc((*cam)->ntkeys, sizeof(keyheader3ds));
      if((*cam)->tkeys == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      for (i = 0; i < (*cam)->ntkeys; i++)
         memcpy(&(((*cam)->tkeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*cam)->tpos  = calloc((*cam)->ntkeys, sizeof(point3ds));
      if((*cam)->tpos == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      for (i = 0; i < (*cam)->ntkeys; i++)
         memcpy(&(((*cam)->tpos)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
   }

}

/*-------------------------------------------------------------
 | 
 | ReleaseCameraMotion3ds
 |
 +-------------------------------------------------------------*/
void ReleaseCameraMotion3ds(kfcamera3ds **cam)
{
   if (*cam != NULL) {
      if((*cam)->pkeys != NULL) free((*cam)->pkeys);
      if((*cam)->pos != NULL) free((*cam)->pos);
      if((*cam)->fkeys != NULL) free((*cam)->fkeys);
      if((*cam)->fov != NULL) free((*cam)->fov);
      if((*cam)->rkeys != NULL) free((*cam)->rkeys);
      if((*cam)->roll != NULL) free((*cam)->roll);
      if((*cam)->tkeys != NULL) free((*cam)->tkeys);
      if((*cam)->tpos != NULL) free((*cam)->tpos);
      free(*cam);
      *cam = NULL;
   }
}

/*-----------------------------------------------------------------
 |
 | GetCameraNodCount3ds
 |
 +-----------------------------------------------------------------*/
ulong3ds GetCameraNodeCount3ds(database3ds *db)
{
    return kfGetGenericNodeCount(db, CAMERA_NODE_TAG);
}

/*------------------------------------------------------------------
 |
 | GetCameraNodNameList3ds
 |
 +-----------------------------------------------------------------*/
void GetCameraNodeNameList3ds(database3ds *db, namelist3ds **list)
{
    kfGetGenericNodeNameList(db, CAMERA_NODE_TAG, list);
}

/*----------------------------------------------------------------------
 |
 | GetCameraMotionByName3ds
 |
 |  db: database to be searched
 |  name: name of camera
 |  kfcam: Ptr to the addr of kfcamera3ds structure, if (*kfcam) 
 |         is null, then memory will be allocated for the new
 |         structure, otherwise, the existing structure will be 
 |         filled in with the new values.  If no match is found, then
 |         kfcam remains unchanged.
 | 
 +----------------------------------------------------------------------*/
void GetCameraMotionByName3ds(database3ds *db, 
                  const char3ds *name, 
                  kfcamera3ds **kfcam)
{
  chunk3ds *pCameraChunk, *pTargetChunk;

  if ((db == NULL) || (name == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);
  
  kfFindNamedAndTaggedChunk(db, name, CAMERA_NODE_TAG, &pCameraChunk);

  if (pCameraChunk)
      kfFindNamedAndTaggedChunk(db, name, TARGET_NODE_TAG, &pTargetChunk);

  if (pCameraChunk)
    GetCameraMotion3ds(pCameraChunk, pTargetChunk, kfcam);

}

/*----------------------------------------------------------------------
 |
 | GetCameraMotionByIndex3ds
 |
 |  db: database to be searched
 |  index: index of camera in name list
 |  kfcam: Ptr to the addr of kfcamera3ds structure, if (*kfcam) 
 |         is null, then memory will be allocated for the new
 |         structure, otherwise, the existing structure will be 
 |         filled in with the new values.  If no match is found, then
 |         kfcam remains unchanged.
 | 
 +----------------------------------------------------------------------*/
void GetCameraMotionByIndex3ds(database3ds *db, 
                   ulong3ds index,
                   kfcamera3ds **kfcam)
{
  chunk3ds *pCameraChunk, *pTargetChunk;
  namelist3ds *list = NULL;
  char3ds *name;
  
  if(db == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  GetCameraNodeNameList3ds(db, &list);

  if(index < list->count){

    name = list->list[index].name;
    kfFindNamedAndTaggedChunk(db, name, CAMERA_NODE_TAG, &pCameraChunk);

    if (pCameraChunk){
      kfFindNamedAndTaggedChunk(db, name, TARGET_NODE_TAG, &pTargetChunk);
      GetCameraMotion3ds(pCameraChunk, pTargetChunk, kfcam);
    }
  }
  /*--- release list when done using name */
  ReleaseNameList3ds(&list);
}



/*--------------------------------------------------------------------------
 | GetCameraMotion3ds
 | 
 | pCamChunk:CAMERA_NODE_TAG chunk to extract data from
 | pTargetChunk: TARGET_NODE_TAG chunk to extract target data from
 | kfcam: Structure to fill in with chunk data
 |
 |  chunk----->kfcamera3ds
 |
 | Gets camera keyframe information from chunk
 | 
 +--------------------------------------------------------------------------*/
void GetCameraMotion3ds(chunk3ds *pCamChunk,
            chunk3ds *pTargetChunk,
            kfcamera3ds **kfcam)
{
    kfcamera3ds 
    *pKfCam;
    chunk3ds 
    *pNodeHdrChunk, *pPosChunk, *pFovChunk, 
    *pRollChunk, *pTargetPosChunk = NULL, *pTargetHdrChunk = NULL;
    ulong3ds 
    i, nPosKeys = 0, nFovKeys = 0, nRollKeys = 0, nTargetKeys = 0;
    NodeHdr
    *pNodeHdr, *pTargetHdr = NULL;
    PosTrackTag 
    *pPosData = NULL, *pTargetData = NULL;
    FovTrackTag
    *pFovData = NULL;
    RollTrackTag 
    *pRollData = NULL;
    
    if(pCamChunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);

    
    /*-------------------------------
      |  Get information from chunks 
      +-------------------------------*/
    /*--- Search children of camera chunk */
    FindChunk3ds(pCamChunk, NODE_HDR, &pNodeHdrChunk);
    FindChunk3ds(pCamChunk, POS_TRACK_TAG, &pPosChunk);
    FindChunk3ds(pCamChunk, FOV_TRACK_TAG, &pFovChunk);
    FindChunk3ds(pCamChunk, ROLL_TRACK_TAG, &pRollChunk);
    
    ReadChunkData3ds(pNodeHdrChunk);
    pNodeHdr = pNodeHdrChunk->data;
    
    if(pPosChunk){
    ReadChunkData3ds(pPosChunk);
    pPosData = pPosChunk->data;
    nPosKeys = pPosData->trackhdr.keycount;
    }
    
    if(pFovChunk){
    ReadChunkData3ds(pFovChunk);
    pFovData = pFovChunk->data;
    nFovKeys = pFovData->trackhdr.keycount;
    }
    
    if(pRollChunk){
    ReadChunkData3ds(pRollChunk);
    pRollData = pRollChunk->data;
    nRollKeys = pRollData->trackhdr.keycount;
    }
    
    if (pTargetChunk){
      FindChunk3ds(pTargetChunk, NODE_HDR, &pTargetHdrChunk);
      if (pTargetHdrChunk){
    ReadChunkData3ds(pTargetHdrChunk);
    pTargetHdr = pTargetHdrChunk->data;
      }

      FindChunk3ds(pTargetChunk, POS_TRACK_TAG, &pTargetPosChunk);
      if (pTargetPosChunk){
    ReadChunkData3ds(pTargetPosChunk);
    pTargetData = pTargetPosChunk->data;
    nTargetKeys = pTargetData->trackhdr.keycount;
      }
    }
    
    /*--------------------------------------------
      | Set-up and fill-in the kfcamera structure 
      +--------------------------------------------*/
    InitCameraMotion3ds(kfcam, 
            nPosKeys, 
            nFovKeys, 
            nRollKeys, 
            nTargetKeys);
    
    pKfCam = *kfcam;
    
    /*--- Header Information */
    strcpy(pKfCam->name, pNodeHdr->objname);
    pKfCam->flags1 = pNodeHdr->flags1;
    pKfCam->flags2 = pNodeHdr->flags2;

    /*--- Parents */
    kfGetParentName(pNodeHdrChunk, pKfCam->parent);
    kfGetParentName(pTargetHdrChunk, pKfCam->tparent);

    /*--- Target Information */
    if (nTargetKeys){
    pKfCam->ntflag = pTargetData->trackhdr.flags;
    
    for(i=0; i<nTargetKeys; i++){
        memcpy(&((pKfCam->tkeys)[i]), 
           &((pTargetData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfCam->tpos)[i]),
           &((pTargetData->positionlist)[i]),
           sizeof(point3ds));
    }
    }
    if (pTargetHdr) {
        pKfCam->tflags1 = pTargetHdr->flags1;
        pKfCam->tflags2 = pTargetHdr->flags2;
	}
    
    /*--- Position Information */
    if (nPosKeys){
    pKfCam->npflag = pPosData->trackhdr.flags;
    for (i=0; i<nPosKeys; i++){
        memcpy(&((pKfCam->pkeys)[i]),
           &((pPosData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfCam->pos)[i]),
           &((pPosData->positionlist)[i]),
           sizeof(point3ds));
    }
    }
    
    /*--- Field of view Information */
    if (nFovKeys){
    pKfCam->nfflag = pFovData->trackhdr.flags;
    for (i=0; i<nFovKeys; i++){
        memcpy(&((pKfCam->fkeys)[i]), 
           &((pFovData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfCam->fov)[i]),
           &((pFovData->fovanglelist)[i]),
           sizeof(float3ds));
    }
    }
    
    /*--- Roll Track Information */
    if (nRollKeys){
    pKfCam->nrflag = pRollData->trackhdr.flags;
    for (i=0; i<nRollKeys; i++){
        memcpy(&((pKfCam->rkeys)[i]), 
           &((pRollData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfCam->roll)[i]),
           &((pRollData->rollanglelist)[i]),
           sizeof(float3ds));
    }
    }
    
    /*--- Free Chunk Data */
    FreeFileChunkData3ds(pPosChunk);
    FreeFileChunkData3ds(pFovChunk);
    FreeFileChunkData3ds(pRollChunk);
    FreeFileChunkData3ds(pNodeHdrChunk);
    if (pTargetPosChunk)
      FreeFileChunkData3ds(pTargetPosChunk);
    if (pTargetHdrChunk)
      FreeFileChunkData3ds(pTargetHdrChunk);
}

/*--------------------------------------------------------------------------
 | PutCameraMotion3ds
 | Puts camera keyframe information into database. 
 | 
 |  kfcamera3ds--->db
 |
 | KFDATA
 |  ...
 |  TARGET
 |  CAMERA_NODE
 |   NODE_ID
 |   NODE_HDR
 |   APP_DATA 
 |   POS_TRACK
 |   FOV_TRACK
 |   ROLL_TRACK
 |
 | Needs Work:
 |   1. NodeId: what value?
 |
 +--------------------------------------------------------------------------*/
void PutCameraMotion3ds(database3ds *db, kfcamera3ds *kfcam)
{
    chunk3ds 
    *pKfChunk, *pCamChunk, *pTargetChunk = NULL, 
    *pCameraXdata = NULL, *pTargetXdata = NULL;
    
    if(db == NULL || kfcam == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);

    if (db->topchunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_DATA);

    if (!(db->topchunk->tag == M3DMAGIC) || (db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_INVALID_DATA);
    
    /*--- Find KFSEG chunk to put keyframe data into */
    kfGetKfSeg(db->topchunk, &pKfChunk);
    ON_ERROR_RETURN;

    /*--- Destroy old chunks, Copy any XData and reattach it later */
    kfExtractXdata(db, kfcam->name, TARGET_NODE_TAG, &pTargetXdata);
    kfExtractXdata(db, kfcam->name, CAMERA_NODE_TAG, &pCameraXdata);

    MakeNodeTagListDirty3ds(db);
    
    /*--- TARGET TRACK */
    if (kfcam->ntkeys>0 && kfcam->tkeys && kfcam->tpos)
    pTargetChunk = kfPutTargetTrack(kfcam->tparent,
                    kfcam->ntkeys,
                    kfcam->ntflag,
                    kfcam->tkeys,
                    kfcam->tpos,
                    kfcam->name,
                    kfcam->tflags1,
                    kfcam->tflags2,
		    CameraTarget,		    
                    pKfChunk);
    
    pCamChunk = kfPutGenericNode(CAMERA_NODE_TAG, pKfChunk);
    kfPutNodeId(pCamChunk);
    kfPutNodeHeader(kfcam->name, 
            kfcam->flags1,
            kfcam->flags2,
            kfcam->parent,
            pCamChunk);
    
    if (kfcam->npkeys>0 && kfcam->pkeys && kfcam->pos)
    kfPutPosTrack(kfcam->npkeys,
              kfcam->npflag,
              kfcam->pkeys,
              kfcam->pos,
              pCamChunk);
    
    /*-- FIELD OF VIEW */
    if (kfcam->nfkeys>0 && kfcam->fkeys && kfcam->fov){
    FovTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys;
    
    /*--- Create the FOV_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = FOV_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = kfcam->nfflag;
    nKeys = data->trackhdr.keycount = kfcam->nfkeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = 0;  /* Not Used */
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, kfcam->fkeys, nKeys * sizeof(keyheader3ds));
    
    /*--- Copy Points */
    data->fovanglelist = malloc(nKeys * sizeof(float3ds));
    memcpy(data->fovanglelist, kfcam->fov, nKeys * sizeof(float3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pCamChunk, temp);
    }
    
    if (kfcam->nrkeys>0 && kfcam->rkeys && kfcam->roll)
    kfPutRollTrack(kfcam->nrkeys,
               kfcam->nrflag,
               kfcam->rkeys,
               kfcam->roll,
               pCamChunk);
    
    /*--- Replace XData */
    if (pCameraXdata)
    AddChildOrdered3ds(pCamChunk, pCameraXdata);
    if (pTargetXdata)
    AddChildOrdered3ds(pTargetChunk, pTargetXdata);
}

void DeleteCameraMotionByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteNodeTagByIndexAndType3ds(db, index, CAMERA_NODE_TAG);
   ON_ERROR_RETURN;
}

void DeleteCameraMotionByName3ds(database3ds *db, char3ds *name)
{
   DeleteNodeTagByNameAndType3ds(db, name, CAMERA_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyCameraMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{
   CopyNodeTagByIndexAndType3ds(destdb, srcdb, index, CAMERA_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyCameraMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNodeTagByNameAndType3ds(destdb, srcdb, name, CAMERA_NODE_TAG);
   ON_ERROR_RETURN;
}
