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
#include "dbase3ds.h"
#include "3dsobjm.h"
#include "kfutils.h"

const kfrotkey3ds 
DefKfRotKey3ds = {0.0F, 0.0F, 0.0F, 1.0F}; /* Zero degree rotation about the Z axis */

/*-----------------------------------------------------------------------------
  | InitObjectMotion3ds
  |  
  +---------------------------------------------------------------------------*/
void InitObjectMotion3ds(kfmesh3ds **obj, 
             ulong3ds npkeys, /* Number of position keys     */
             ulong3ds nrkeys, /* Number of rot keys         */
             ulong3ds nskeys, /* Number of scale keys        */
             ulong3ds nmkeys, /* Number of morph keys        */
             ulong3ds nhkeys) /* Number of hide keys         */ 
{
    ulong3ds i;
    
    if ((*obj) == NULL)
    {
      (*obj) = malloc(sizeof(kfmesh3ds));
      if (*obj == NULL)
	SET_ERROR_RETURN(ERR_NO_MEM);

    
    (*obj)->name[0] = 0;
    (*obj)->parent[0] = 0;
    (*obj)->flags1 = (*obj)->flags2 = (ushort3ds)0;
    
    memcpy(&((*obj)->pivot), &DefPoint3ds, sizeof(DefPoint3ds));
    
    (*obj)->instance[0] = 0;
    
    memcpy(&((*obj)->boundmin), &DefPoint3ds, sizeof(DefPoint3ds));
    memcpy(&((*obj)->boundmax), &DefPoint3ds, sizeof(DefPoint3ds));
    
    (*obj)->npkeys = npkeys;
    (*obj)->nrkeys = nrkeys;
    (*obj)->nskeys = nskeys;
    (*obj)->nmkeys = nmkeys;
    (*obj)->nhkeys = nhkeys;
    
    (*obj)->pkeys = NULL;
    (*obj)->rkeys = NULL;
    (*obj)->skeys = NULL;
    (*obj)->mkeys = NULL;
    (*obj)->hkeys = NULL;
    
    
    (*obj)->pos = NULL;
    (*obj)->rot = NULL;
    (*obj)->scale = NULL;
    (*obj)->morph = NULL;
    
    (*obj)->msangle = 24.0F;
    }
    
    if (npkeys != 0)
    {
    (*obj)->npflag = TrackSingle3ds;
    
    (*obj)->npkeys = npkeys;
    if ((*obj)->pkeys != NULL) {free((*obj)->pkeys); (*obj)->pkeys = NULL;}
    if ((*obj)->pos != NULL) {free((*obj)->pos); (*obj)->pos = NULL;}
    
    (*obj)->pkeys = calloc((*obj)->npkeys, sizeof(keyheader3ds));
    if((*obj)->pkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->npkeys; i++)
        memcpy(&(((*obj)->pkeys)[i]), &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));
    
    (*obj)->pos = calloc((*obj)->npkeys, sizeof(point3ds));
    if((*obj)->pos == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->npkeys; i++)
        memcpy(&(((*obj)->pos)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
    }
    
    if (nrkeys != 0)
    {
    (*obj)->nrflag = TrackSingle3ds;
    
    (*obj)->nrkeys = nrkeys;
    if ((*obj)->rkeys != NULL) {free((*obj)->rkeys); (*obj)->rkeys = NULL;}
    if ((*obj)->rot != NULL) {free((*obj)->rot); (*obj)->rot = NULL;}
    
    (*obj)->rkeys = calloc((*obj)->nrkeys, sizeof(keyheader3ds));
    if((*obj)->rkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nrkeys; i++)
        memcpy(&(((*obj)->rkeys)[i]), &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));
    
    (*obj)->rot = calloc((*obj)->nrkeys, sizeof(kfrotkey3ds));
    if((*obj)->rot == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nrkeys; i++)
        memcpy(&(((*obj)->rot)[i]), &DefKfRotKey3ds, sizeof(DefKfRotKey3ds));
    }
    
    if (nskeys != 0)
    {
    (*obj)->nsflag = TrackSingle3ds;
    
    (*obj)->nskeys = nskeys;
    if ((*obj)->skeys != NULL) {free((*obj)->skeys); (*obj)->skeys = NULL;}
    if ((*obj)->scale != NULL) {free((*obj)->scale); (*obj)->scale = NULL;}
    
    (*obj)->skeys = calloc((*obj)->nskeys, sizeof(keyheader3ds));
    if((*obj)->skeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nskeys; i++)
        memcpy(&(((*obj)->skeys)[i]), &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));
    
    (*obj)->scale = calloc((*obj)->nskeys, sizeof(point3ds));
    if((*obj)->scale == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nskeys; i++)
        ((*obj)->scale)[i].x = ((*obj)->scale)[i].y = ((*obj)->scale)[i].z = 1.0F;
    }
    
    if (nmkeys != 0)
    {
    (*obj)->nmflag = TrackSingle3ds;
    
    (*obj)->nmkeys = nmkeys;
    if ((*obj)->mkeys != NULL) {free((*obj)->mkeys); (*obj)->mkeys = NULL;}
    if ((*obj)->morph != NULL) {free((*obj)->morph); (*obj)->morph = NULL;}
    
    (*obj)->mkeys = calloc((*obj)->nmkeys, sizeof(keyheader3ds));
    if((*obj)->mkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nmkeys; i++)
        memcpy(&(((*obj)->mkeys)[i]), &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));
    
    (*obj)->morph = calloc((*obj)->nmkeys, sizeof(kfmorphkey3ds));
    if((*obj)->morph == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nmkeys; i++)
        ((*obj)->morph)[i].name[0] = ' ';
    }

    if (nhkeys != 0)
    {
    (*obj)->nhflag = TrackSingle3ds;
    
    (*obj)->nhkeys = nhkeys;
    if ((*obj)->hkeys != NULL) {free((*obj)->hkeys); (*obj)->hkeys = NULL;}
    
    (*obj)->hkeys = calloc((*obj)->nhkeys, sizeof(keyheader3ds));
    if((*obj)->mkeys == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    for (i = 0; i < (*obj)->nmkeys; i++)
        memcpy(&(((*obj)->mkeys)[i]), &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));
    }
}

/*--------------------------------------------------------
  | 
  | ReleaseObjectMotion3ds
  |
  +--------------------------------------------------------*/
void ReleaseObjectMotion3ds(kfmesh3ds **obj)
{
    if (*obj != NULL)
    {
    if ((*obj)->pkeys != NULL) 
        free((*obj)->pkeys);
    if ((*obj)->pos != NULL) 
        free((*obj)->pos);
    if ((*obj)->rkeys != NULL) 
        free((*obj)->rkeys);
    if ((*obj)->rot != NULL) 
        free((*obj)->rot);
    if ((*obj)->skeys != NULL) 
        free((*obj)->skeys);
    if ((*obj)->scale != NULL) 
        free((*obj)->scale);
    if ((*obj)->mkeys != NULL) 
        free((*obj)->mkeys);
    if ((*obj)->morph != NULL) 
        free((*obj)->morph);
    if ((*obj)->hkeys != NULL) 
        free((*obj)->hkeys);
    free(*obj);
    *obj = NULL;
    }
    
}

/*--------------------------------------------------------------------
  |
  | GetObjectNodeCount3ds
  |
  +--------------------------------------------------------------------*/ 
ulong3ds GetObjectNodeCount3ds(database3ds *db)
{
    return kfGetGenericNodeCount(db, OBJECT_NODE_TAG);
}

/*--------------------------------------------------------------------
  |
  | GetObjectNodeNameList3ds
  |
  +--------------------------------------------------------------------*/ 
void GetObjectNodeNameList3ds(database3ds *db, namelist3ds **list)
{
    kfGetGenericNodeNameList(db, OBJECT_NODE_TAG, list);
}


/*----------------------------------------------------------------------
  |
  | GetObjectMotionByName3ds
  |
  |  db: database to be searched
  |  name: name of Object
  |  kfmesh: Ptr to the addr of kfmesh3ds structure, if (*kfmesh) 
  |         is null, then memory will be allocated for the new
  |         structure, otherwise, the existing structure will be 
  |         filled in with the new values.  If no match is found, then
  |         kfmesh remains unchanged.
  | 
  +----------------------------------------------------------------------*/
void GetObjectMotionByName3ds(database3ds *db, 
                  char3ds *name, 
                  kfmesh3ds **kfmesh)
{
    chunk3ds *pObjectChunk;
    
    if(db == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);

    pObjectChunk = FindNodeTagByNameAndType3ds(db, name, OBJECT_NODE_TAG);
    ON_ERROR_RETURN;

    if (pObjectChunk) GetObjectMotion3ds(pObjectChunk, kfmesh);
    ON_ERROR_RETURN;
    
}

/*----------------------------------------------------------------------
  |
  | GetObjectMotionByIndex3ds
  |
  |  db: database to be searched
  |  index: index of Object in name list
  |  kfmesh: Ptr to the addr of kfmesh3ds structure, if (*kfmesh) 
  |         is null, then memory will be allocated for the new
  |         structure, otherwise, the existing structure will be 
  |         filled in with the new values.  If no match is found, then
  |         kfmesh remains unchanged.
  | 
  +----------------------------------------------------------------------*/
void GetObjectMotionByIndex3ds(database3ds *db, 
                   ulong3ds index,
                   kfmesh3ds **kfmesh)
{
    chunk3ds *pObjChunk;
    
    if(db == NULL || kfmesh == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
    if(db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    if(db->topchunk->tag != M3DMAGIC && db->topchunk->tag != CMAGIC)
       SET_ERROR_RETURN(ERR_WRONG_DATABASE);

    pObjChunk = FindNodeTagByIndexAndType3ds(db, index, OBJECT_NODE_TAG);
    ON_ERROR_RETURN;

    if (pObjChunk) GetObjectMotion3ds(pObjChunk, kfmesh);
    ON_ERROR_RETURN;
    
}




/*--------------------------------------------------------------------------
  | GetObjectMotion3ds
  | 
  | pSpotChunk:OBJECT_NODE_TAG chunk to extract data from
  | kfmesh: Structure to fill in with chunk data
  |
  |  chunk----->kfMesh3ds
  |
  | Gets mesh keyframe information from chunk
  | 
  |  NODE_ID
  |  NODE_HDR
  |  APP_DATA
  |  INSTANCE_NAME
  |  PRESCALE   a no-op in 3ds code 
  |  POS_TRACK
  |  ROT_TRACK
  |  SCL_TRACK
  |  MORPH_TRACK
  |  MORPH_SMOOTH
  |  HIDE_TRACK 
  |   
  +--------------------------------------------------------------------------*/
void GetObjectMotion3ds(chunk3ds *pMeshChunk, kfmesh3ds **kfmesh)
{
    kfmesh3ds 
    *pKfMesh;
    chunk3ds 
    *pNodeHdrChunk, 
    *pInstChunk,
    *pPivotChunk,
    *pBboxChunk,
    *pMsChunk,
    *pPosChunk, 
    *pRotChunk,
    *pScaleChunk,
    *pMorphChunk,
    *pHideChunk;
    
    ulong3ds 
    nPosKeys = 0, 
    nRotKeys = 0, 
    nScaleKeys = 0, 
    nMorphKeys = 0, 
    nHideKeys = 0; 
    
    NodeHdr
    *pNodeHdr;
    Pivot
    *pPivotData = NULL;
    InstanceName
    *pInstData = NULL;
    BoundBox
    *pBboxData = NULL;
    MorphSmooth
    *pMsData = NULL;
    PosTrackTag 
    *pPosData = NULL;
    RotTrackTag
    *pRotData = NULL;
    ScaleTrackTag
    *pScaleData = NULL;
    MorphTrackTag
    *pMorphData = NULL;
    HideTrackTag
    *pHideData = NULL;

    chunk3ds *objtag;
    
    if(pMeshChunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_ARG);
    if(pMeshChunk->tag != OBJECT_NODE_TAG) SET_ERROR_RETURN(ERR_WRONG_OBJECT);

    CopyChunk3ds(pMeshChunk, &objtag);
    ON_ERROR_RETURN;

    /*-------------------------------
      |  Get information from chunks 
      +-------------------------------*/
    /*--- Search children of MeshLight chunk */
    FindChunk3ds(objtag, NODE_HDR, &pNodeHdrChunk);
    FindChunk3ds(objtag, INSTANCE_NAME, &pInstChunk);
    FindChunk3ds(objtag, PIVOT, &pPivotChunk); 
    FindChunk3ds(objtag, BOUNDBOX, &pBboxChunk);
    FindChunk3ds(objtag, MORPH_SMOOTH, &pMsChunk);
    FindChunk3ds(objtag, POS_TRACK_TAG, &pPosChunk);
    FindChunk3ds(objtag, ROT_TRACK_TAG, &pRotChunk);
    FindChunk3ds(objtag, SCL_TRACK_TAG, &pScaleChunk);
    FindChunk3ds(objtag, MORPH_TRACK_TAG, &pMorphChunk);
    FindChunk3ds(objtag, HIDE_TRACK_TAG, &pHideChunk);
    
    ReadChunkData3ds(pNodeHdrChunk);
    pNodeHdr = pNodeHdrChunk->data;
    
    if (pInstChunk){
    ReadChunkData3ds(pInstChunk);
    pInstData = pInstChunk->data;
    pInstChunk->data = NULL;
    }
    
    if (pPivotChunk){
    ReadChunkData3ds(pPivotChunk);
    pPivotData = pPivotChunk->data;
    pPivotChunk->data = NULL;
    }
    
    if (pBboxChunk){
    ReadChunkData3ds(pBboxChunk);
    pBboxData = pBboxChunk->data;
    pBboxChunk->data = NULL;    
    }
    
    if (pMsChunk){
    pMsData = ReadChunkData3ds(pMsChunk);
	pMsChunk->data = NULL;
    }

    if(pPosChunk){
    ReadChunkData3ds(pPosChunk);
    pPosData = pPosChunk->data;
    nPosKeys = pPosData->trackhdr.keycount;
    pPosChunk->data = NULL;
    }
    
    if(pRotChunk){
    ReadChunkData3ds(pRotChunk);
    pRotData = pRotChunk->data;
    nRotKeys = pRotData->trackhdr.keycount;
    pRotChunk->data = NULL;
    }
    if(pScaleChunk){
    ReadChunkData3ds(pScaleChunk);
    pScaleData = pScaleChunk->data;
    nScaleKeys = pScaleData->trackhdr.keycount;
    pScaleChunk->data = NULL;
    }
    
    if(pMorphChunk){
    ReadChunkData3ds(pMorphChunk);
    pMorphData = pMorphChunk->data;
    nMorphKeys = pMorphData->trackhdr.keycount;
    pMorphChunk->data = NULL;
    }
    
    if(pHideChunk){
    ReadChunkData3ds(pHideChunk);
    pHideData = pHideChunk->data;
    nHideKeys = pHideData->trackhdr.keycount;
    pHideChunk->data = NULL;
    }
    
    /*--------------------------------------------
      | Set-up and fill-in the kfmesh3ds structure 
      +--------------------------------------------*/
    if ((*kfmesh) == NULL)
    {
    (*kfmesh) = malloc(sizeof(kfmesh3ds));
    if(*kfmesh == NULL) 
      SET_ERROR_RETURN(ERR_NO_MEM);
    }
    
    pKfMesh = *kfmesh;
    
    /*--- Header Information */
    strcpy(pKfMesh->name, pNodeHdr->objname);
    pKfMesh->flags1 = pNodeHdr->flags1;
    pKfMesh->flags2 = pNodeHdr->flags2;
    
    /*--- Get Parent Name if there is one */
    kfGetParentName(pNodeHdrChunk, pKfMesh->parent);

    /*--- Instance */
    if (pInstData)
	{
		strcpy(pKfMesh->instance, pInstData->name);
		free(pInstData->name);	/* CCJ */
		free( pInstData );
	}
    else
	    pKfMesh->instance[0] = 0;   
    
    /*--- Pivot */
    if (pPivotData)
	{
		memcpy(&(pKfMesh->pivot), &(pPivotData->offset), 
           sizeof(DefPoint3ds));
		free( pPivotData );
    }
    else
		memcpy(&(pKfMesh->pivot), &(DefPoint3ds), 
           sizeof(DefPoint3ds));    
    
    /*--- Bound */
    if (pBboxData)
	{
		memcpy(&(pKfMesh->boundmin), &(pBboxData->min), sizeof(point3ds));
		memcpy(&(pKfMesh->boundmax), &(pBboxData->max), sizeof(point3ds));
		free( pBboxData );
    }
    else
	{
		memcpy(&(pKfMesh->boundmin), &DefPoint3ds, sizeof(DefPoint3ds));
		memcpy(&(pKfMesh->boundmax), &DefPoint3ds, sizeof(DefPoint3ds));
    }
    
    /*--- MorphSmooth Angle */
    if (pMsData)
	{
		pKfMesh->msangle = pMsData->smoothgroupangle;
		free( pMsData );
	}
    else
	   pKfMesh->msangle = 0.0F;

    /*--- Position */
    pKfMesh->npkeys = nPosKeys;
    if (nPosKeys)
	{
		pKfMesh->pkeys = pPosData->keyhdrlist;
		pKfMesh->pos = pPosData->positionlist;
		pKfMesh->npflag = pPosData->trackhdr.flags;
		free( pPosData );
    }
    else
	{
		pKfMesh->pkeys = NULL;
		pKfMesh->pos = NULL;
		pKfMesh->npflag = 0;
    }

    /*--- Rotation */
    pKfMesh->nrkeys = nRotKeys;
    if (nRotKeys)
	{
		pKfMesh->rkeys = pRotData->keyhdrlist;
		pKfMesh->rot = pRotData->rotationlist;
		pKfMesh->nrflag = pRotData->trackhdr.flags;
		free( pRotData );
    }
    else
	{
		pKfMesh->rkeys = NULL;
		pKfMesh->rot = NULL;
		pKfMesh->nrflag = 0;
    }
    
    /*--- Scale */
    pKfMesh->nskeys = nScaleKeys;
    if (nScaleKeys)
	{
		pKfMesh->skeys = pScaleData->keyhdrlist;
		pKfMesh->scale = pScaleData->scalelist;
		pKfMesh->nsflag = pScaleData->trackhdr.flags;
		free( pScaleData );
    }
    else
	{
		pKfMesh->skeys = NULL;
		pKfMesh->scale = NULL;
		pKfMesh->nsflag = 0;
    }
    
    /*--- Morph */
    pKfMesh->nmkeys = nMorphKeys;
    if (nMorphKeys)
	{
		pKfMesh->mkeys = pMorphData->keyhdrlist;
		pKfMesh->morph = pMorphData->morphlist;
		pKfMesh->nmflag = pMorphData->trackhdr.flags;
		free( pMorphData );
    }
    else
	{
		pKfMesh->mkeys = NULL;
		pKfMesh->morph = NULL;
		pKfMesh->nmflag = 0;
    }
    
    pKfMesh->nhkeys = nHideKeys;
    if (nHideKeys)
	{
		pKfMesh->hkeys = pHideData->keyhdrlist;
		pKfMesh->nhflag = pHideData->trackhdr.flags;
		free( pHideData );
    }
    else
	{
		pKfMesh->hkeys = NULL;
		pKfMesh->nhflag = 0;
    }

    /*-- ADDITIONAL MORPH INFO HERE */
    
    /*--- Free Chunk Data: only free those that arent being copied */
    ReleaseChunk3ds(&objtag);
}


/*--------------------------------------------------------------------------
  | PutObjectMotion3ds
  | Puts Object keyframe information into database. 
  | 
  |  kfMesh3ds--->db
  |
  | KFDATA
  |  ...
  |  OBJECT_NODE
  |   NODE_ID
  |   NODE_HDR
  |   APP_DATA 
  |   INSTANCE
  |   POS_TRACK
  |   ROT_TRACK 
  |   SCL_TRACK
  |   MORPH_TRACK
  |   MORPH_SMOOTH
  |   HIDE_TRACK
  |
  | Needs Work:
  | 
  +--------------------------------------------------------------------------*/
void PutObjectMotion3ds(database3ds *db, kfmesh3ds *kfmesh)
{
    chunk3ds 
    *pKfChunk, *pMeshChunk, *pMeshXdata = NULL;
    
    if(db == NULL || kfmesh == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    if(db->topchunk == NULL) 
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
    
    if (!(db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

    /*--- Find KFSEG chunk to put keyframe data into */
    kfGetKfSeg(db->topchunk, &pKfChunk);

    /*--- Destroy old chunks, Copy any XData and reattach it later */
    kfExtractXdata(db, kfmesh->name, OBJECT_NODE_TAG, &pMeshXdata);
    
    pMeshChunk = kfPutGenericNode(OBJECT_NODE_TAG, pKfChunk);
    ON_ERROR_RETURN;

    MakeNodeTagListDirty3ds(db);
    
    kfPutNodeId(pMeshChunk);
    kfPutNodeHeader(kfmesh->name,  
            kfmesh->flags1,
            kfmesh->flags2,
            kfmesh->parent,
            pMeshChunk);
    
    if (strlen(kfmesh->instance) > 0){  /*--- Instance Name */
       chunk3ds *temp;
       InstanceName *data;
    
       InitChunk3ds(&temp);
       temp->tag = INSTANCE_NAME;
       data = InitChunkData3ds(temp);
       data->name = strdup(kfmesh->instance);
       AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    if(strcmp(kfmesh->name, DummyName3ds) == 0){ /*--- Bounding Box */
    chunk3ds *temp;
    BoundBox *data;
    
    InitChunk3ds(&temp);
    temp->tag = BOUNDBOX;
    data = InitChunkData3ds(temp);
    
    memcpy(&(data->min), &(kfmesh->boundmin), sizeof(point3ds));
    memcpy(&(data->max), &(kfmesh->boundmax), sizeof(point3ds));
    
    AddChildOrdered3ds(pMeshChunk, temp);
    }

    if((kfmesh->flags2) & KfNodeAutoSmooth3ds == KfNodeAutoSmooth3ds)
    { /*--- Morphsmooth angle */
       chunk3ds *temp;
       MorphSmooth *data;
    
       InitChunk3ds(&temp);
       temp->tag = MORPH_SMOOTH;
       data = InitChunkData3ds(temp);
    
       data->smoothgroupangle = kfmesh->msangle;
       AddChildOrdered3ds(pMeshChunk, temp);
    }

    /* if(1)*/
	{ 	/*--- Pivot */
    chunk3ds *temp;
    Pivot *data;
    
    InitChunk3ds(&temp);
    temp->tag = PIVOT;
    data = InitChunkData3ds(temp);
    
    memcpy(&(data->offset), &(kfmesh->pivot), sizeof(DefPoint3ds));    
    AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    if (kfmesh->npkeys>0 && kfmesh->pkeys && kfmesh->pos)
    kfPutPosTrack(kfmesh->npkeys,
              kfmesh->npflag,
              kfmesh->pkeys,
              kfmesh->pos,
              pMeshChunk);
    
    /*--- ROTATION */
    if (kfmesh->nrkeys>0 && kfmesh->rkeys && kfmesh->rot){
    RotTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfmesh->nrkeys;
    keyheader3ds *pKeyHdr = kfmesh->rkeys;
    ushort3ds localFlag = kfmesh->nrflag;
    
    /*--- Create the ROT_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = ROT_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    data->rotationlist = malloc(nKeys * sizeof(kfrotkey3ds));
    memcpy(data->rotationlist, kfmesh->rot, nKeys * sizeof(kfrotkey3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    /*--- SCALING */
    if (kfmesh->nskeys>0 && kfmesh->skeys && kfmesh->scale){
    ScaleTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfmesh->nskeys;
    keyheader3ds *pKeyHdr = kfmesh->skeys;
    ushort3ds localFlag = kfmesh->nsflag;
    
    /*--- Create the SCALE_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = SCL_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    data->scalelist = malloc(nKeys * sizeof(point3ds));
    memcpy(data->scalelist, kfmesh->scale, nKeys * sizeof(point3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    /*--- Morphing */
    if (kfmesh->nmkeys>0 && kfmesh->mkeys && kfmesh->morph){
    MorphTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfmesh->nmkeys;
    keyheader3ds *pKeyHdr = kfmesh->mkeys;
    ushort3ds localFlag = kfmesh->nmflag;
    
    /*--- Create the MORPH_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = MORPH_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    data->morphlist = malloc(nKeys * sizeof(kfmorphkey3ds));
    memcpy(data->morphlist, kfmesh->morph, nKeys * sizeof(kfmorphkey3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    /*--- Hide -special case: only headers */
    if (kfmesh->nhkeys>0 && kfmesh->hkeys){
    HideTrackTag *data;
    chunk3ds *temp = NULL;
    int nKeys = kfmesh->nhkeys;
    keyheader3ds *pKeyHdr = kfmesh->hkeys;
    ushort3ds localFlag = kfmesh->nhflag;
    
    /*--- Create the HIDE_TRACK_TAG chunk */
    InitChunk3ds(&temp);
    temp->tag = HIDE_TRACK_TAG;
    data = InitChunkData3ds(temp);
    
    /*--- Initialize trackhdr field */
    data->trackhdr.flags = localFlag;
    data->trackhdr.keycount = nKeys;
    data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0;
    
    /*--- Copy headers */
    data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
    memcpy(data->keyhdrlist, pKeyHdr, nKeys * sizeof(keyheader3ds));
    
    /*--- Make if a child of the kframe node */
    AddChildOrdered3ds(pMeshChunk, temp);
    }
    
    /*--- Replace XData */
    if (pMeshXdata)
    AddChildOrdered3ds(pMeshChunk, pMeshXdata);
}

void DeleteObjectMotionByIndex3ds(database3ds *db, ulong3ds index)
{
   DeleteNodeTagByIndexAndType3ds(db, index, OBJECT_NODE_TAG);
   ON_ERROR_RETURN;
}

void DeleteObjectMotionByName3ds(database3ds *db, char3ds *name)
{
   DeleteNodeTagByNameAndType3ds(db, name, OBJECT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyObjectMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index)
{
   CopyNodeTagByIndexAndType3ds(destdb, srcdb, index, OBJECT_NODE_TAG);
   ON_ERROR_RETURN;
}

void CopyObjectMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name)
{
   CopyNodeTagByNameAndType3ds(destdb, srcdb, name, OBJECT_NODE_TAG);
   ON_ERROR_RETURN;
}

