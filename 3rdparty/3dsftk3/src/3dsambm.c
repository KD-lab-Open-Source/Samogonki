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
#include "3dsambm.h"
#include "kfutils.h"

static              /* const */ char3ds *kAmbientName = "$AMBIENT$";

/*----------------------------------------------------------
  |
  |InitAmbientLightMotion3ds
  |
  +----------------------------------------------------------*/
void InitAmbientLightMotion3ds(kfambient3ds **light, 
                   ulong3ds nckeys)
{
  ulong3ds i;

  if (*light == NULL)
    {
      (*light) = malloc(sizeof(kfambient3ds));
      if(*light == NULL) 
    SET_ERROR_RETURN(ERR_NO_MEM);


      (*light)->flags1 = (*light)->flags2 = (ushort3ds)0;
      (*light)->nckeys  = nckeys;
      (*light)->color = NULL;
      (*light)->ckeys = NULL;
    }

  if ((*light)->ckeys != NULL) {
    free((*light)->ckeys); 
    (*light)->ckeys = NULL;
  }
  if ((*light)->color != NULL) {
    free((*light)->color); 
    (*light)->color = NULL;
  }

  if (nckeys != 0)
    {
      (*light)->ncflag = TrackSingle3ds;
      (*light)->nckeys = nckeys;

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
 |ReleaseAmbientLightMotion3ds
 |
 +----------------------------------------------------------*/
void ReleaseAmbientLightMotion3ds(kfambient3ds **light)
{
   if (*light != NULL) {
      if((*light)->ckeys != NULL) 
      free((*light)->ckeys);
      if((*light)->color != NULL) 
      free((*light)->color);
      free(*light);
      *light = NULL;
  }
}


/*----------------------------------------------------------------------
 |
 | GetAmbientLightMotion3ds
 |
 |  db: database to be searched
 |  kfambient: Ptr to the addr of kfambient3ds structure, if (*kfambient) 
 |         is null, then memory will be allocated for the new
 |         structure, otherwise, the existing structure will be 
 |         filled in with the new values.  If no match is found, then
 |         kfambient remains unchanged.
 | 
 | Ambient light a special case: only one ambient node per keyframe data
 |    chunk.
 | 
 +----------------------------------------------------------------------*/
void GetAmbientLightMotion3ds(database3ds *db, 
                  kfambient3ds **kfambient)
{
  chunk3ds *pKfChunk, *pChunk = NULL;
  
  if(db == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  /*--- Find Keyframe chunk */
  FindChunk3ds(db->topchunk, KFDATA, &pKfChunk);
  if (pKfChunk){
    FindChunk3ds(pKfChunk, AMBIENT_NODE_TAG, &pChunk);
    if (pChunk)
      GetAmbientLightMotionChunk3ds(pChunk, kfambient);
  }
}

/*--------------------------------------------------------------------------
 | GetAmbientLightMotionChunk3ds
 | 
 | pAmbientChunk:SPOTAMBIENT_NODE_TAG chunk to extract data from
 | pTargetChunk: L_TARGET_NODE_TAG chunk to extract target data from
 | kfspot: Structure to fill in with chunk data
 |
 |  chunk----->kfAmbient3ds
 |
 | Gets AmbientLight keyframe information from chunk
 | 
 |  L_TARGET
 |   ...
 |  NODE_HDR
 |  APP_DATA
 |  COL_TRACK
 | 
 +--------------------------------------------------------------------------*/
void GetAmbientLightMotionChunk3ds(chunk3ds *pAmbientChunk,
               kfambient3ds **kfambient)
{
    kfambient3ds 
    *pKfAmbient;
    chunk3ds 
    *pNodeHdrChunk, *pColChunk;
    ulong3ds i,
    nColKeys = 0;
    NodeHdr
    *pNodeHdr = NULL;
    ColTrackTag
    *pColData = NULL;
    
    if (pAmbientChunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 
    
    /*-------------------------------
      |  Get information from chunks 
      +-------------------------------*/
    /*--- Search children of AmbientLight chunk */
    FindChunk3ds(pAmbientChunk, NODE_HDR, &pNodeHdrChunk);
    FindChunk3ds(pAmbientChunk, COL_TRACK_TAG, &pColChunk);

    if (pNodeHdrChunk){
      ReadChunkData3ds(pNodeHdrChunk);
      pNodeHdr = pNodeHdrChunk->data;
    }
    
    if(pColChunk){
    ReadChunkData3ds(pColChunk);
    pColData = pColChunk->data;
    nColKeys = pColData->trackhdr.keycount;
    }
    
    /*--------------------------------------------
      | Set-up and fill-in the kfambient3ds structure 
      +--------------------------------------------*/
    InitAmbientLightMotion3ds(kfambient, nColKeys); 
    
    pKfAmbient = *kfambient;
    
    /*--- Header Information */
    if (pNodeHdr){
      pKfAmbient->flags1 = pNodeHdr->flags1;
      pKfAmbient->flags2 = pNodeHdr->flags2;
    }

    /*--- Color Information */    
    if (pColData){
      if (nColKeys){
    pKfAmbient->ncflag = pColData->trackhdr.flags;
    for(i=0; i<nColKeys; i++){
      memcpy(&((pKfAmbient->ckeys)[i]), 
         &((pColData->keyhdrlist)[i]),
         sizeof(keyheader3ds));
      memcpy(&((pKfAmbient->color)[i]),
         &((pColData->colorlist)[i]),
         sizeof(fcolor3ds));
    }
      }
    }
    
    /*--- Free Chunk Data */
    if (pNodeHdrChunk)
    FreeFileChunkData3ds(pNodeHdrChunk);
    if (pColChunk)
    FreeFileChunkData3ds(pColChunk);
}

/*--------------------------------------------------------------------------
 | PutAmbientLightMotion3ds
 | Puts AmbientLight keyframe information into database. 
 | 
 |  kfAmbientLight3ds--->db
 |
 | KFDATA
 |  ...
 |  LIGHT_NODE
 |   NODE_ID
 |   NODE_HDR
 |   APP_DATA 
 |   COL_TRACK 
 |
 | Needs Work:
 |   1. NodeId: what value?
 |
 +--------------------------------------------------------------------------*/
void PutAmbientLightMotion3ds(database3ds *db, kfambient3ds *kfambient)
{
  chunk3ds 
    *pKfChunk, *pAmbientChunk, *pAmbientXdata = NULL;
    
  if(db == NULL || kfambient == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

  if(db->topchunk == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_DATA);
    
  if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
    SET_ERROR_RETURN(ERR_WRONG_DATABASE);
    
  /*--- Find KFSEG chunk to put keyframe data into */
  kfGetKfSeg(db->topchunk, &pKfChunk);

  /*--- Destroy old chunks, Copy any XData and reattach it later */
  kfExtractXdata(db, kAmbientName, AMBIENT_NODE_TAG, &pAmbientXdata);
    
  pAmbientChunk = kfPutGenericNode(AMBIENT_NODE_TAG, pKfChunk);
  kfPutNodeId(pAmbientChunk);
  kfPutNodeHeader(kAmbientName,
          kfambient->flags1,
          kfambient->flags2,
          (char3ds *)0,
          pAmbientChunk);
    
  if (kfambient->nckeys>0 && kfambient->ckeys && kfambient->color)
    kfPutColorTrack(kfambient->nckeys,
            kfambient->ncflag,
            kfambient->ckeys,
            kfambient->color,
            pAmbientChunk);
    
  /*--- Replace XData */
  if (pAmbientXdata)
    AddChildOrdered3ds(pAmbientChunk, pAmbientXdata);
}

void DeleteAmbientLightMotion3ds(database3ds *db)
{
   DeleteNodeTagByNameAndType3ds(db, kAmbientName, AMBIENT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyAmbientLightMotion3ds(database3ds *destdb, database3ds *srcdb)
{
   CopyNodeTagByNameAndType3ds(destdb, srcdb, kAmbientName, AMBIENT_NODE_TAG);
   ON_ERROR_RETURN;
}


