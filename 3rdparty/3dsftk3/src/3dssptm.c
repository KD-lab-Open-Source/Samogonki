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
#include "kfutils.h"


/*-----------------------------------------------------------------------------
  | InitSpotlightMotion3ds
  | Needs:
  |  -check default values -esp. color   -sjw
  +---------------------------------------------------------------------------*/
void InitSpotlightMotion3ds(kfspot3ds **spot, 
                ulong3ds npkeys, /*  Number of position keys         */
                ulong3ds nckeys, /*  Number of color keys            */
                ulong3ds nhkeys, /*  Number of hot spot angle keys   */
                ulong3ds nfkeys, /*  Number of falloff angle keys    */
                ulong3ds nrkeys, /*  Number of roll keys */   
                ulong3ds ntkeys) /*  Number of target position keys  */
{
    ulong3ds i;
    
    if (*spot == NULL)
    {
    (*spot) = malloc(sizeof(kfspot3ds));
    if (*spot == NULL)
      SET_ERROR_RETURN(ERR_NO_MEM);
    
    (*spot)->name[0] = 0;
    (*spot)->parent[0] = 0;
    (*spot)->tparent[0] = 0;
    (*spot)->flags1 = (*spot)->flags2 = (ushort3ds)0;
    (*spot)->tflags1 = (*spot)->tflags2 = (ushort3ds)0;

    (*spot)->npkeys  = npkeys;
    (*spot)->nckeys  = nckeys;
    (*spot)->nfkeys  = nfkeys;
    (*spot)->ntkeys  = ntkeys;
    (*spot)->nhkeys  = nhkeys;
    (*spot)->nrkeys  = nrkeys;
    
    (*spot)->pkeys = NULL;
    (*spot)->ckeys = NULL;
    (*spot)->hkeys = NULL;
    (*spot)->fkeys = NULL;
    (*spot)->tkeys = NULL;
    (*spot)->rkeys = NULL;
    
    
    (*spot)->pos   = NULL;
    (*spot)->color = NULL;
    (*spot)->hot   = NULL;
    (*spot)->fall  = NULL;
    (*spot)->tpos  = NULL;
    (*spot)->roll  = NULL;
    }
    
    /*--- POSITION KEYS -----------------------------------------------------*/
    if (npkeys != 0)
    {
    (*spot)->npflag = TrackSingle3ds;  /* sjw: Is this valid? */
    (*spot)->npkeys = npkeys;
    
    /*--- free old keys if they exist */
    if ((*spot)->pkeys != NULL) {
        free((*spot)->pkeys); 
        (*spot)->pkeys = NULL;
    }
    
    /*--- free old positions if they exist */
    if ((*spot)->pos   != NULL) {
        free((*spot)->pos);   
        (*spot)->pos = NULL;
    }
    
    /*--- alloc new keys */
    (*spot)->pkeys = calloc((*spot)->npkeys, sizeof(keyheader3ds));
    if((*spot)->pkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    /*--- initialize with default header/spline */
    for (i = 0; i < (*spot)->npkeys; i++)
        memcpy(&(((*spot)->pkeys)[i]), 
           &DefKeyHeader3ds, 
           sizeof(DefKeyHeader3ds));
    
    /*--- alloc new positions */
    (*spot)->pos   = calloc((*spot)->npkeys, sizeof(point3ds));
    if((*spot)->pos == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    /*--- initialize with default position */
    for (i = 0; i < (*spot)->npkeys; i++)
        memcpy(&(((*spot)->pos)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
    }
    
    /*--- COLOR KEYS ----------------------------------------------------------*/
    if (nckeys != 0)
    {
    (*spot)->ncflag = TrackSingle3ds;  /* sjw: Is this valid? */
    (*spot)->nckeys = nckeys;
    
    /*--- free old color keys */
    if ((*spot)->ckeys != NULL) {
        free((*spot)->ckeys); 
        (*spot)->ckeys = NULL;
    }
    
    /*--- free old colors */
    if ((*spot)->color  != NULL) {
        free((*spot)->color);  
        (*spot)->color = NULL;
    }
    
    /*--- alloc new color keys */
    (*spot)->ckeys = calloc((*spot)->nckeys, sizeof(keyheader3ds));
    if((*spot)->ckeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /*--- initialize to default values */
    for (i = 0; i < (*spot)->nckeys; i++)
        memcpy(&(((*spot)->ckeys)[i]), 
           &DefKeyHeader3ds, 
           sizeof(DefKeyHeader3ds));
    
    /*--- alloc new colors */
    (*spot)->color  = calloc((*spot)->nckeys, sizeof(fcolor3ds));
    if((*spot)->color == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /*--- Where to get default color? -sjw*/
#ifdef LATER
    for (i = 0; i < (*spot)->nckeys; i++)
        memcpy(&(((*spot)->color)[i]), 
           &localDColor.bDefFColor3ds, 
           sizeof(DefFColor3ds));
#endif
    }
    
    /*---HOT-SPOT ANGLE KEYS---------------------------------------------------*/
    if (nhkeys != 0)
    {
    (*spot)->nhflag = TrackSingle3ds;  /* sjw: Is this valid? */
    (*spot)->nhkeys = nhkeys;
    
    /*--- free old color keys */
    if ((*spot)->hkeys != NULL) {
        free((*spot)->hkeys); 
        (*spot)->hkeys = NULL;
    }
    
    /*--- free old data */
    if ((*spot)->hot  != NULL) {
        free((*spot)->hot);  
        (*spot)->hot = NULL;
    }
    
    /*--- alloc new hot keys */
    (*spot)->hkeys = calloc((*spot)->nhkeys, sizeof(keyheader3ds));
    if((*spot)->hkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /*--- initialize to default values */
    for (i = 0; i < (*spot)->nhkeys; i++)
        memcpy(&(((*spot)->hkeys)[i]), 
           &DefKeyHeader3ds, 
           sizeof(DefKeyHeader3ds));
    
    /*--- alloc new hots */
    (*spot)->hot  = calloc((*spot)->nhkeys, sizeof(float3ds));
    if((*spot)->hot == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /* default hot spot ange 90.0 for now, get real value later (1..174.5) */
    for (i = 0; i < (*spot)->nhkeys; i++)
        ((*spot)->hot)[i] = (float3ds)90.0;
    }
    
    
    /*---FALLOFF ANGLE KEYS----------------------------------------------------*/
    if (nfkeys != 0)
    {
    (*spot)->nfflag = TrackSingle3ds;  /* sjw: Is this valid? */
    (*spot)->nfkeys = nfkeys;
    
    /*--- free old color keys */
    if ((*spot)->fkeys != NULL) {
        free((*spot)->fkeys); 
        (*spot)->fkeys = NULL;
    }
    
    /*--- free old data */
    if ((*spot)->fall  != NULL) {
        free((*spot)->fall);  
        (*spot)->fall = NULL;
    }
    
    /*--- alloc new fall keys */
    (*spot)->fkeys = calloc((*spot)->nfkeys, sizeof(keyheader3ds));
    if((*spot)->fkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /*--- initialize to default values */
    for (i = 0; i < (*spot)->nfkeys; i++)
        memcpy(&(((*spot)->fkeys)[i]), 
           &DefKeyHeader3ds, 
           sizeof(DefKeyHeader3ds));
    
    /*--- alloc new falls */
    (*spot)->fall  = calloc((*spot)->nfkeys, sizeof(float3ds));
    if((*spot)->fall == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /* default falloff ange 90.0 for now, get real value later (1..175) */
    for (i = 0; i < (*spot)->nfkeys; i++)
        ((*spot)->fall)[i] = (float3ds)90.0;
    }
    
    /*--- ROLL KEYS ----------------------------------------------------------*/
    
    if (nrkeys != 0)
    {
    (*spot)->nrflag = TrackSingle3ds;
    
    (*spot)->nrkeys = nrkeys;
    if ((*spot)->rkeys != NULL) {free((*spot)->rkeys); (*spot)->rkeys = NULL;}
    if ((*spot)->roll  != NULL) {free((*spot)->roll);  (*spot)->roll = NULL;}
    
    (*spot)->rkeys = calloc((*spot)->nrkeys, sizeof(keyheader3ds));
    if((*spot)->rkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    for (i = 0; i < (*spot)->nrkeys; i++)
      memcpy(&(((*spot)->rkeys)[i]), 
	     &DefKeyHeader3ds, 
	     sizeof(DefKeyHeader3ds));
    
    (*spot)->roll  = calloc((*spot)->nrkeys, sizeof(float3ds));
    if((*spot)->roll == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    for (i = 0; i < (*spot)->nrkeys; i++)
        ((*spot)->roll)[i] = (float3ds)0.0;
    }
    
    /*---L_TARGET POS KEYS ------------------------------------------------*/
    if (ntkeys != 0)
    {
    (*spot)->ntflag = TrackSingle3ds; 
    (*spot)->ntkeys = ntkeys;
    
    /*--- free old keys */
    if ((*spot)->tkeys != NULL) {
        free((*spot)->tkeys); 
        (*spot)->tkeys = NULL;
    }
    
    /*--- free old data */
    if ((*spot)->tpos  != NULL) {
        free((*spot)->tpos);  
        (*spot)->tpos = NULL;
    }
    
    /*--- alloc new tpos keys */
    (*spot)->tkeys = calloc((*spot)->ntkeys, sizeof(keyheader3ds));
    if((*spot)->tkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /*--- initialize to default values */
    for (i = 0; i < (*spot)->ntkeys; i++)
        memcpy(&(((*spot)->tkeys)[i]), 
           &DefKeyHeader3ds, 
           sizeof(DefKeyHeader3ds));
    
    /*--- alloc new positions */
    (*spot)->tpos  = calloc((*spot)->ntkeys, sizeof(point3ds));
    if((*spot)->tpos == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);

    
    /* default target position, 0,0,0  sjw fix later if necessary */
    for (i = 0; i < (*spot)->ntkeys; i++)
        memcpy(&(((*spot)->tpos)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
    }
}

/*--------------------------------------------------------
  | 
  | ReleaseSpotlightMotion3ds
  |
  +--------------------------------------------------------*/
void ReleaseSpotlightMotion3ds(kfspot3ds **spot)
{
    if (*spot != NULL) {
    /*--- POSITION */
    if((*spot)->pkeys != NULL) 
        free((*spot)->pkeys);
    if((*spot)->pos != NULL) 
        free((*spot)->pos);
    /*--- HOTSPOT */
    if((*spot)->hkeys != NULL) 
        free((*spot)->hkeys);
    if((*spot)->hot != NULL) 
        free((*spot)->hot);
    /*--- FALLOFF */
    if((*spot)->fkeys != NULL) 
        free((*spot)->fkeys);
    if((*spot)->fall != NULL) 
        free((*spot)->fall);
    /*---L_TARGET */
    if((*spot)->tkeys != NULL) 
        free((*spot)->tkeys);
    if((*spot)->tpos != NULL) 
        free((*spot)->tpos);
    /*---ROLL */
    if((*spot)->rkeys != NULL) 
        free((*spot)->rkeys);
    if((*spot)->roll != NULL) 
        free((*spot)->roll);
    /*---COLOR */
    if((*spot)->ckeys != NULL)
       free((*spot)->ckeys);
    if((*spot)->color != NULL)
       free((*spot)->color);
    free(*spot);
    *spot = NULL;
    }
}

/*--------------------------------------------------------------------
  |
  | GetSpotlightNodeCount3ds
  |
  +--------------------------------------------------------------------*/ 
ulong3ds GetSpotlightNodeCount3ds(database3ds *db)
{
    return kfGetGenericNodeCount(db, SPOTLIGHT_NODE_TAG);
}

/*--------------------------------------------------------------------
  |
  | GetSpotlightNodeNameList3ds
  |
  +--------------------------------------------------------------------*/ 
void GetSpotlightNodeNameList3ds(database3ds *db, namelist3ds **list)
{
    kfGetGenericNodeNameList(db, SPOTLIGHT_NODE_TAG, list);
}


/*----------------------------------------------------------------------
  |
  | GetSpotlightMotionByName3ds
  |
  |  db: database to be searched
  |  name: name of spotlight
  |  kfspot: Ptr to the addr of kfspot3ds structure, if (*kfspot) 
  |         is null, then memory will be allocated for the new
  |         structure, otherwise, the existing structure will be 
  |         filled in with the new values.  If no match is found, then
  |         kfspot remains unchanged.
  | 
  +----------------------------------------------------------------------*/
void GetSpotlightMotionByName3ds(database3ds *db, 
                 char3ds *name, 
                 kfspot3ds **kfspot)
{
    chunk3ds *pSpotlightChunk, *pTargetChunk;
    
    if ((db == NULL) || (name == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
    if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC))
       SET_ERROR_RETURN(ERR_WRONG_DATABASE);


    kfFindNamedAndTaggedChunk(db, name, SPOTLIGHT_NODE_TAG, &pSpotlightChunk);
    
    if (pSpotlightChunk)
    kfFindNamedAndTaggedChunk(db, name, L_TARGET_NODE_TAG, &pTargetChunk);
    
    if (pSpotlightChunk)
    GetSpotlightMotion3ds(pSpotlightChunk, pTargetChunk, kfspot);
    
}

/*----------------------------------------------------------------------
  |
  | GetSpotlightMotionByIndex3ds
  |
  |  db: database to be searched
  |  index: index of Spotlight in name list
  |  kfspot: Ptr to the addr of kfspot3ds structure, if (*kfspot) 
  |         is null, then memory will be allocated for the new
  |         structure, otherwise, the existing structure will be 
  |         filled in with the new values.  If no match is found, then
  |         kfspot remains unchanged.
  | 
  +----------------------------------------------------------------------*/
void GetSpotlightMotionByIndex3ds(database3ds *db, 
                  ulong3ds index,
                  kfspot3ds **kfspot)
{
    chunk3ds *pSpotChunk, *pTargetChunk;
    namelist3ds *list = NULL;
    char3ds *name;
    
    if(db == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);
    
    GetSpotlightNodeNameList3ds(db, &list);
    
    if(index < list->count){
      name = list->list[index].name;
      kfFindNamedAndTaggedChunk(db, name, SPOTLIGHT_NODE_TAG, &pSpotChunk);
    
      if (pSpotChunk){
	kfFindNamedAndTaggedChunk(db, name, L_TARGET_NODE_TAG, &pTargetChunk);
	if (pTargetChunk)
	  GetSpotlightMotion3ds(pSpotChunk, pTargetChunk, kfspot);
      }
    }
    
    /*--- release list when done using name */
    ReleaseNameList3ds(&list);
}

/*--------------------------------------------------------------------------
  | GetSpotlightMotion3ds
  | 
  | pSpotChunk:SPOTLIGHT_NODE_TAG chunk to extract data from
  | pTargetChunk: L_TARGET_NODE_TAG chunk to extract target data from
  | kfspot: Structure to fill in with chunk data
  |
  |  chunk----->kfSpot3ds
  |
  | Gets Spotlight keyframe information from chunk
  | 
  |  L_TARGET
  |   ...
  |  NODE_HDR
  |  APP_DATA
  |  POS_TRACK
  |  COL_TRACK
  |  HOT_TRACK
  |  FALL_TRACK
  |  ROLL_TRACK 
  | 
  +--------------------------------------------------------------------------*/
void GetSpotlightMotion3ds(chunk3ds *pSpotChunk,
               chunk3ds *pTargetChunk,
               kfspot3ds **kfspot)
{
    kfspot3ds 
    *pKfSpot;
    chunk3ds 
    *pNodeHdrChunk, *pPosChunk, *pColChunk, *pTargetPosChunk = NULL,
    *pHotChunk, *pFallChunk, *pRollChunk, *pTargetHdrChunk = NULL;
    ulong3ds i,
    nPosKeys = 0, 
    nColKeys = 0, 
    nHotKeys = 0,
    nFallKeys = 0,
    nRollKeys = 0,
    nTargetKeys = 0;
    NodeHdr
    *pNodeHdr = NULL, *pTargetHdr = NULL;
    PosTrackTag 
    *pPosData = NULL, *pTargetData = NULL;
    ColTrackTag
    *pColData = NULL;
    HotTrackTag
    *pHotData = NULL;
    FallTrackTag
    *pFallData = NULL;
    RollTrackTag 
    *pRollData = NULL;
    
    if(pSpotChunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);
    
    /*-------------------------------
      |  Get information from chunks 
      +-------------------------------*/
    /*--- Search children of Spotlight chunk */
    FindChunk3ds(pSpotChunk, NODE_HDR, &pNodeHdrChunk);
    FindChunk3ds(pSpotChunk, POS_TRACK_TAG, &pPosChunk);
    FindChunk3ds(pSpotChunk, COL_TRACK_TAG, &pColChunk);
    FindChunk3ds(pSpotChunk, HOT_TRACK_TAG, &pHotChunk);
    FindChunk3ds(pSpotChunk, FALL_TRACK_TAG, &pFallChunk);
    FindChunk3ds(pSpotChunk, ROLL_TRACK_TAG, &pRollChunk);
    
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
    
    if(pHotChunk){
    ReadChunkData3ds(pHotChunk);
    pHotData = pHotChunk->data;
    nHotKeys = pHotData->trackhdr.keycount;
    }
    
    if(pFallChunk){
    ReadChunkData3ds(pFallChunk);
    pFallData = pFallChunk->data;
    nFallKeys = pFallData->trackhdr.keycount;
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
      else
    pTargetData = NULL;
    }
    
    /*--------------------------------------------
      | Set-up and fill-in the kfspot3ds structure 
      +--------------------------------------------*/
    InitSpotlightMotion3ds(kfspot, 
               nPosKeys, 
               nColKeys, 
               nHotKeys,
               nFallKeys,
               nRollKeys, 
               nTargetKeys);
    
    pKfSpot = *kfspot;
    
    /*--- Header Information */
    strcpy(pKfSpot->name, pNodeHdr->objname);
    pKfSpot->flags1 = pNodeHdr->flags1;
    pKfSpot->flags2 = pNodeHdr->flags2;
    
    /*--- Get Parent Name if there is one */
    kfGetParentName(pNodeHdrChunk, pKfSpot->parent);
    kfGetParentName(pTargetHdrChunk, pKfSpot->tparent);
    
    if (pTargetHdr){
      pKfSpot->tflags1 = pTargetHdr->flags1;
      pKfSpot->tflags2 = pTargetHdr->flags2;
    }
    else 
      pKfSpot->tflags1 = pKfSpot->tflags2 = (short3ds)0;

    /*--- Target Information */
    if (nTargetKeys){
    pKfSpot->ntflag = pTargetData->trackhdr.flags;
    for(i=0; i<nTargetKeys; i++){
        memcpy(&((pKfSpot->tkeys)[i]), 
           &((pTargetData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->tpos)[i]),
           &((pTargetData->positionlist)[i]),
           sizeof(point3ds));
    }
    }
    
    /*--- Position Information */
    if (nPosKeys){
    pKfSpot->npflag = pPosData->trackhdr.flags;
    for (i=0; i<nPosKeys; i++){
        memcpy(&((pKfSpot->pkeys)[i]), 
           &((pPosData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->pos)[i]),
           &((pPosData->positionlist)[i]),
           sizeof(point3ds));
    }
    }
    
    /*--- Color Information */
    if (nColKeys){
    pKfSpot->ncflag = pColData->trackhdr.flags;
    for (i=0; i<nColKeys; i++){
        memcpy(&((pKfSpot->ckeys)[i]), 
           &((pColData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->color)[i]),
           &((pColData->colorlist)[i]),
           sizeof(fcolor3ds));
    }
    }
    
    /*--- Hot Spot Information */
    if (nHotKeys){
    pKfSpot->nhflag = pHotData->trackhdr.flags;
    for (i=0; i<nHotKeys; i++){
        memcpy(&((pKfSpot->hkeys)[i]), 
           &((pHotData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->hot)[i]),
           &((pHotData->hotspotanglelist)[i]),
           sizeof(float3ds));
    }
    }
    
    /*--- Falloff Information */
    if (nFallKeys){
    pKfSpot->nfflag = pFallData->trackhdr.flags;
    for (i=0; i<nFallKeys; i++){
        memcpy(&((pKfSpot->fkeys)[i]), 
           &((pFallData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->fall)[i]),
           &((pFallData->falloffanglelist)[i]),
           sizeof(float3ds));
    }
    }
    
    /*--- Roll Track Information */
    if (nRollKeys){
    pKfSpot->nrflag = pRollData->trackhdr.flags;
    for (i=0; i<nRollKeys; i++){
        memcpy(&((pKfSpot->rkeys)[i]), 
           &((pRollData->keyhdrlist)[i]),
           sizeof(keyheader3ds));
        memcpy(&((pKfSpot->roll)[i]),
           &((pRollData->rollanglelist)[i]),
           sizeof(float3ds));
    }
    }
    
    /*--- Free Chunk Data */
    FreeFileChunkData3ds(pNodeHdrChunk);
    FreeFileChunkData3ds(pPosChunk);
    FreeFileChunkData3ds(pColChunk);
    FreeFileChunkData3ds(pHotChunk);
    FreeFileChunkData3ds(pFallChunk);
    FreeFileChunkData3ds(pRollChunk);
    if (pTargetPosChunk)
    FreeFileChunkData3ds(pTargetPosChunk);
}



/*--------------------------------------------------------------------------
  | PutSpotlightMotion3ds
  | Puts Spotlight keyframe information into database. 
  | 
  |  kfSpotlight3ds--->db
  |
  | KFDATA
  |  ...
  |  L_TARGET
  |  SPOTLIGHT_NODE
  |   NODE_ID
  |   NODE_HDR
  |   APP_DATA 
  |   POS_TRACK
  |   COL_TRACK 
  |   HOT_TRACK
  |   FALL_TRACK
  |   ROLL_TRACK
  |
  | Needs Work:
  |   1. NodeId: what value?
  |
  +--------------------------------------------------------------------------*/
void PutSpotlightMotion3ds(database3ds *db, kfspot3ds *kfspot)
{
    chunk3ds 
    *pKfChunk, *pSpotChunk, *pTargetChunk = NULL, 
    *pSpotXdata = NULL, *pTargetXdata = NULL;
    
    if(db == NULL || kfspot == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    if(db->topchunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    
    if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

    
    /*--- Find KFSEG chunk to put keyframe data into */
    kfGetKfSeg(db->topchunk, &pKfChunk);

    /*--- Destroy old chunks, Copy any XData and reattach it later */
    kfExtractXdata(db, kfspot->name, L_TARGET_NODE_TAG, &pTargetXdata);
    kfExtractXdata(db, kfspot->name, SPOTLIGHT_NODE_TAG, &pSpotXdata);

    MakeNodeTagListDirty3ds(db);
    
    /*--- L_TARGET TRACK */
    if (kfspot->ntkeys>0 && kfspot->tkeys && kfspot->tpos)
    pTargetChunk = kfPutTargetTrack(kfspot->tparent,
                    kfspot->ntkeys,
                    kfspot->ntflag,
                    kfspot->tkeys,
                    kfspot->tpos,
                    kfspot->name,
                    kfspot->tflags1,
                    kfspot->tflags2,
		    LightTarget,		    
                    pKfChunk);
    
    pSpotChunk = kfPutGenericNode(SPOTLIGHT_NODE_TAG, pKfChunk);
    kfPutNodeId(pSpotChunk);
    kfPutNodeHeader(kfspot->name, 
            kfspot->flags1,
            kfspot->flags2,
            kfspot->parent,
            pSpotChunk);
    
    if (kfspot->npkeys>0 && kfspot->pkeys && kfspot->pos)
    kfPutPosTrack(kfspot->npkeys,
              kfspot->npflag,
              kfspot->pkeys,
              kfspot->pos,
              pSpotChunk);
    
    
    if (kfspot->nckeys>0 && kfspot->ckeys && kfspot->color)
    kfPutColorTrack(kfspot->nckeys,
            kfspot->ncflag,
            kfspot->ckeys,
            kfspot->color,
            pSpotChunk);
    
    /*-- HOTSPOT */
    if (kfspot->nhkeys>0 && kfspot->hkeys && kfspot->hot){
    HotTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfspot->nhkeys;
    keyheader3ds *pKeyHdr = kfspot->hkeys;
    ushort3ds localFlag = kfspot->nhflag;
    
    /*--- Create the HOT_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = HOT_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    data->hotspotanglelist = malloc(nKeys * sizeof(float3ds));
    memcpy(data->hotspotanglelist, kfspot->hot, nKeys * sizeof(float3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pSpotChunk, temp);
    }
    
    /*-- FALLOFF */
    if (kfspot->nfkeys>0 && kfspot->fkeys && kfspot->fall){
    FallTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfspot->nfkeys;
    keyheader3ds *pKeyHdr = kfspot->fkeys;
    ushort3ds localFlag = kfspot->nfflag;
    
    /*--- Create the FALL_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = FALL_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    data->falloffanglelist = malloc(nKeys * sizeof(float3ds));
    memcpy(data->falloffanglelist, kfspot->fall, nKeys * sizeof(float3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pSpotChunk, temp);
    }
    
    /*--- ROLL */
    if (kfspot->nrkeys>0 && kfspot->rkeys && kfspot->roll)
    kfPutRollTrack(kfspot->nrkeys,
               kfspot->nrflag,
               kfspot->rkeys,
               kfspot->roll,
               pSpotChunk);
    
    /*--- Replace XData */
    if (pSpotXdata)
    AddChildOrdered3ds(pSpotChunk, pSpotXdata);
    if (pTargetXdata)
    AddChildOrdered3ds(pTargetChunk, pTargetXdata);
}

void DeleteSpotlightMotionByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteNodeTagByIndexAndType3ds(db, index, SPOTLIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void DeleteSpotlightMotionByName3ds(database3ds *db, char3ds *name)
{
   DeleteNodeTagByNameAndType3ds(db, name, SPOTLIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopySpotlightMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{
   CopyNodeTagByIndexAndType3ds(destdb, srcdb, index, SPOTLIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopySpotlightMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNodeTagByNameAndType3ds(destdb, srcdb, name, SPOTLIGHT_NODE_TAG);
   ON_ERROR_RETURN;
}
