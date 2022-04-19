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
#include "3dssptm.h"
#include "3dsomnm.h"
#include "kfutils.h"
#include "3dserr.h"

/*----------------------------------------------------------
 |
 |InitOmnilightMotion3ds
 |
 +----------------------------------------------------------*/
void InitOmnilightMotion3ds(kfomni3ds **light, 
                ulong3ds npkeys, 
                ulong3ds nckeys)
{
   ulong3ds i;

   if (*light == NULL)
   {
      (*light) = malloc(sizeof(kfomni3ds));
      if(*light == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      (*light)->name[0] = 0;
      (*light)->parent[0] = 0;
      (*light)->flags1 = (*light)->flags2 = (ushort3ds)0;

      (*light)->npkeys  = npkeys;
      (*light)->nckeys  = nckeys;
      (*light)->pkeys = NULL;
      (*light)->pos = NULL;
      (*light)->ckeys = NULL;
      (*light)->color = NULL;
   }

   if (npkeys != 0)
   {
      (*light)->npflag = TrackSingle3ds;

      (*light)->npkeys = npkeys;
      if ((*light)->pkeys != NULL) {
      free((*light)->pkeys); 
      (*light)->pkeys = NULL;
      }
      
      if ((*light)->pos   != NULL) {
      free((*light)->pos);   
      (*light)->pos = NULL;
      }

      (*light)->pkeys = calloc((*light)->npkeys, sizeof(keyheader3ds));
      if((*light)->pkeys == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*light)->npkeys; i++)
         memcpy(&(((*light)->pkeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*light)->pos   = calloc((*light)->npkeys, sizeof(point3ds));
      if((*light)->pos == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*light)->npkeys; i++)
         memcpy(&(((*light)->pos)[i]), 
        &DefPoint3ds, 
        sizeof(DefPoint3ds));
  }

   if (nckeys != 0)
   {
      (*light)->ncflag = TrackSingle3ds;

      (*light)->nckeys = nckeys;
      if ((*light)->ckeys != NULL) {
      free((*light)->ckeys); 
      (*light)->ckeys = NULL;
      }
      
      if ((*light)->color != NULL) {
      free((*light)->color); 
      (*light)->color = NULL;
      }

      (*light)->ckeys = calloc((*light)->nckeys, sizeof(keyheader3ds));
      if((*light)->ckeys == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*light)->nckeys; i++)
         memcpy(&(((*light)->ckeys)[i]), 
        &DefKeyHeader3ds, 
        sizeof(DefKeyHeader3ds));

      (*light)->color   = calloc((*light)->nckeys, sizeof(fcolor3ds));
      if((*light)->color == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);

      for (i = 0; i < (*light)->nckeys; i++)
         ((*light)->color)[i].r = 
         ((*light)->color)[i].g = 
         ((*light)->color)[i].b = (float3ds)1.0;
  }
}

/*----------------------------------------------------------
 |
 |ReleaseOmnilightMotion3ds
 |
 +----------------------------------------------------------*/
void ReleaseOmnilightMotion3ds(kfomni3ds **light)
{
   if (*light != NULL) {
      if((*light)->pkeys != NULL) free((*light)->pkeys);
      if((*light)->pos != NULL) free((*light)->pos);
      if((*light)->ckeys != NULL) free((*light)->ckeys);
      if((*light)->color != NULL) free((*light)->color);
      free(*light);
      *light = NULL;
   }
}

/*--------------------------------------------------------------------
 |
 | GetOmnilightNodeCount3ds
 |
 +--------------------------------------------------------------------*/ 
ulong3ds GetOmnilightNodeCount3ds(database3ds *db)
{
    return kfGetGenericNodeCount(db, LIGHT_NODE_TAG);
}

/*--------------------------------------------------------------------
 |
 | GetOmnilightNodeNameList3ds
 |
 +--------------------------------------------------------------------*/ 
void GetOmnilightNodeNameList3ds(database3ds *db, namelist3ds **list)
{
    kfGetGenericNodeNameList(db, LIGHT_NODE_TAG, list);
}


/*----------------------------------------------------------------------
 |
 | GetOmnilightMotionByName3ds
 |
 |  db: database to be searched
 |  name: name of Omnilight
 |  kfomni: Ptr to the addr of kfomni3ds structure, if (*kfomni) 
 |         is null, then memory will be allocated for the new
 |         structure, otherwise, the existing structure will be 
 |         filled in with the new values.  If no match is found, then
 |         kfomni remains unchanged.
 | 
 +----------------------------------------------------------------------*/
void GetOmnilightMotionByName3ds(database3ds *db, 
                  char3ds *name, 
                  kfomni3ds **kfomni)
{
    chunk3ds *pChunk;
  
    if(db == NULL || name == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

    kfFindNamedAndTaggedChunk(db, name, LIGHT_NODE_TAG, &pChunk);

  if (pChunk)
    GetOmnilightMotion3ds(pChunk, kfomni);
}

/*----------------------------------------------------------------------
 |
 | GetOmnilightMotionByIndex3ds
 |
 |  db: database to be searched
 |  index: index of Omnilight in name list
 |  kfomni: Ptr to the addr of kfomni3ds structure, if (*kfomni) 
 |         is null, then memory will be allocated for the new
 |         structure, otherwise, the existing structure will be 
 |         filled in with the new values.  If no match is found, then
 |         kfomni remains unchanged.
 | 
 +----------------------------------------------------------------------*/
void GetOmnilightMotionByIndex3ds(database3ds *db, 
                  ulong3ds index,
                  kfomni3ds **kfomni)
{
    chunk3ds *pOmniChunk;
    namelist3ds *list = NULL;
    char3ds *name;
  
    if (db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    GetOmnilightNodeNameList3ds(db, &list);

    if (index < list->count){
      name = list->list[index].name;
      kfFindNamedAndTaggedChunk(db, name, LIGHT_NODE_TAG, &pOmniChunk);

      if (pOmniChunk)
    GetOmnilightMotion3ds(pOmniChunk, kfomni);
    }  
    /*--- release list when done using name */
    ReleaseNameList3ds(&list);
  }



/*--------------------------------------------------------------------------
 | GetOmnilightMotion3ds
 | 
 | pOmniChunk:SPOTLIGHT_NODE_TAG chunk to extract data from
 | pTargetChunk: L_TARGET_NODE_TAG chunk to extract target data from
 | kfspot: Structure to fill in with chunk data
 |
 |  chunk----->kfOmni3ds
 |
 | Gets Omnilight keyframe information from chunk
 | 
 |  L_TARGET
 |  NODE_ID
 |  NODE_HDR
 |  APP_DATA
 |  POS_TRACK
 |  COL_TRACK
 |  HOT_TRACK
 |  FALL_TRACK
 |  ROLL_TRACK 
 | 
 +--------------------------------------------------------------------------*/
void GetOmnilightMotion3ds(chunk3ds *pOmniChunk,
               kfomni3ds **kfomni)
{
    kfomni3ds *pKfOmni;
    chunk3ds *pNodeHdrChunk, *pPosChunk, *pColChunk;
    ulong3ds i, nPosKeys = 0, nColKeys = 0;
    NodeHdr *pNodeHdr;
    PosTrackTag *pPosData = NULL;
    ColTrackTag *pColData = NULL;
    

    if (pOmniChunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 
    
    /*-------------------------------
      |  Get information from chunks 
      +-------------------------------*/
    /*--- Search children of OmniLight chunk */
    FindChunk3ds(pOmniChunk, NODE_HDR, &pNodeHdrChunk);
    FindChunk3ds(pOmniChunk, POS_TRACK_TAG, &pPosChunk);
    FindChunk3ds(pOmniChunk, COL_TRACK_TAG, &pColChunk);
    
    ReadChunkData3ds(pNodeHdrChunk);
    pNodeHdr = pNodeHdrChunk->data;
    
    if(pPosChunk){
    ReadChunkData3ds(pPosChunk);
    pPosData = pPosChunk->data;
    nPosKeys = pPosData->trackhdr.keycount;
    }
    
    if(pColChunk){
    ReadChunkData3ds(pColChunk);
    pColData = pColChunk->data;
    nColKeys = pColData->trackhdr.keycount;
    }
    
    /*--------------------------------------------
      | Set-up and fill-in the kfomni3ds structure 
      +--------------------------------------------*/
    InitOmnilightMotion3ds(kfomni, nPosKeys, nColKeys); 
    
    pKfOmni = *kfomni;
    
    /*--- Header Information */
    strcpy(pKfOmni->name, pNodeHdr->objname);
    pKfOmni->flags1 = pNodeHdr->flags1;
    pKfOmni->flags2 = pNodeHdr->flags2;
    kfGetParentName(pNodeHdrChunk, pKfOmni->parent);

    /*--- Position Information */
    if (nPosKeys){
    pKfOmni->npflag = pPosData->trackhdr.flags;
    for (i=0; i<nPosKeys; i++){
        memcpy(&((pKfOmni->pkeys)[i]), &((pPosData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfOmni->pos)[i]), &((pPosData->positionlist)[i]),
           sizeof(point3ds));
    }
    }
    
    /*--- Color Information */
    if (nColKeys){
    pKfOmni->ncflag = pColData->trackhdr.flags;
    for (i=0; i<nColKeys; i++){
        memcpy(&((pKfOmni->ckeys)[i]), &((pColData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfOmni->color)[i]), &((pColData->colorlist)[i]),
           sizeof(fcolor3ds));
    }
    }

    /*--- Free Chunk Data */
    if (pNodeHdrChunk)
    FreeFileChunkData3ds(pNodeHdrChunk);
    if (pPosChunk)
    FreeFileChunkData3ds(pPosChunk);
    if (pColChunk)
    FreeFileChunkData3ds(pColChunk);
}

/*--------------------------------------------------------------------------
 | PutOmnilightMotion3ds
 | Puts Omnilight keyframe information into database. 
 | 
 |  kfOmnilight3ds--->db
 |
 | KFDATA
 |  ...
 |  LIGHT_NODE
 |   NODE_ID
 |   NODE_HDR
 |   APP_DATA 
 |   POS_TRACK
 |   COL_TRACK 
 |
 | Needs Work:
 |   1. NodeId: what value?
 |
 +--------------------------------------------------------------------------*/
void PutOmnilightMotion3ds(database3ds *db, kfomni3ds *kfomni)
{
    chunk3ds 
    *pKfChunk, *pOmniChunk, *pOmniXdata = NULL;
    
    if(db == NULL || kfomni == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    if(db->topchunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    
    if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

    MakeNodeTagListDirty3ds(db);

    /*--- Find KFSEG chunk to put keyframe data into */
    kfGetKfSeg(db->topchunk, &pKfChunk);

    /*--- Destroy old chunks, Copy any XData and reattach it later */
    kfExtractXdata(db, kfomni->name, LIGHT_NODE_TAG, &pOmniXdata);

    
    pOmniChunk = kfPutGenericNode(LIGHT_NODE_TAG, pKfChunk);
    kfPutNodeId(pOmniChunk);
    kfPutNodeHeader(kfomni->name, 
            kfomni->flags1,
            kfomni->flags2,
            kfomni->parent,
            pOmniChunk);

    MakeNodeTagListDirty3ds(db);
    
    
    if (kfomni->npkeys>0 && kfomni->pkeys && kfomni->pos)
    kfPutPosTrack(kfomni->npkeys,
              kfomni->npflag,
              kfomni->pkeys,
              kfomni->pos,
              pOmniChunk);
    
    if (kfomni->nckeys>0 && kfomni->ckeys && kfomni->color)
    kfPutColorTrack(kfomni->nckeys,
            kfomni->ncflag,
            kfomni->ckeys,
            kfomni->color,
            pOmniChunk);
    
    /*--- Replace XData */
    if (pOmniXdata)
    AddChildOrdered3ds(pOmniChunk, pOmniXdata);
}

void DeleteOmnilightMotionByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteNodeTagByIndexAndType3ds(db, index, LIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void DeleteOmnilightMotionByName3ds(database3ds *db, char3ds *name)
{
   DeleteNodeTagByNameAndType3ds(db, name, LIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyOmnilightMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{
   CopyNodeTagByIndexAndType3ds(destdb, srcdb, index, LIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyOmnilightMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNodeTagByNameAndType3ds(destdb, srcdb, name, LIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

