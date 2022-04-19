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

/*---------------------------------------------------------------------------
 | kfPutNodeHeader
 |   Add a NODE_HDR chunk to the parent chunk
 |
 |  Needs: parent stuff implemented
 |
 |    flags1
 |       NODE_UNUSED0     (1)
 |       NODE_UNUSED1     (1<<1)
 |       NODE_RENDOB_HIDE (1<<2)
 |       NODE_OFF         (1<<3)
 |       ATKEY1           (1<<4)
 |       ATKEY2           (1<<5)
 |       ATKEY3           (1<<6)
 |       ATKEY4           (1<<7)
 |       ATKEY5           (1<<8)
 |       ATKEYFLAGS       (ATKEY1|ATKEY2|ATKEY3|ATKEY4|ATKEY5)
 |       MARK_NODE        (1<<9)
 |       DISABLE_NODE     (1<<10)
 |       HIDE_NODE        (1<<11)
 |       FAST_NODE        (1<<12)    draw node quikdraw style 
 |       NODE_UNUSED13    (1<<13)
 |       PRIMARY_NODE     (1<<14)    corresponds to mesh 
 |       NODE_CALC_PATH   (1<<15)  
 |
 |    flags2:
 |      NODE_HAS_PATH     (1)
 |      NODE_AUTO_SMOOTH  (1<<1)
 |      NODE_FROZEN       (1<<2)
 |      NODE_ANI_HIDDEN   (1<<3)
 |      NODE_MOTION_BLUR  (1<<4)
 |      NODE_BLUR_BRANCH  (1<<5)
 |      NODE_MORPH_MTL    (1<<6)
 |      NODE_MORPH_OB     (1<<7)
 |
 +----------------------------------------------------------------------------*/
void kfPutNodeHeader(char3ds *objname, 
             short3ds flags1, 
             short3ds flags2, 
	     char3ds *parentname,
             chunk3ds *pParentChunk)

{
  chunk3ds *pHdrChunk;
  NodeHdr *pNodeHdr;

  InitChunk3ds(&pHdrChunk);
  pHdrChunk->tag = NODE_HDR;
  pNodeHdr = InitChunkData3ds(pHdrChunk);
  pNodeHdr->objname = strdup(objname);
  pNodeHdr->flags1 = flags1;
  pNodeHdr->flags2 = flags2;
  pNodeHdr->parentindex = -1;
  AddChildOrdered3ds(pParentChunk, pHdrChunk);
  kfAddParentName(pHdrChunk, parentname);
}     


/*----------------------------------------------------------------------------
 | kfPutNodeId
 |  put a NodeId into database as a child of parentChunk
 |
 +---------------------------------------------------------------------------*/
void kfPutNodeId(chunk3ds *pParentChunk)
{
  chunk3ds *pIdChunk;
  KFId *pId;

  InitChunk3ds(&pIdChunk);
  pIdChunk->tag = NODE_ID;
  pId = InitChunkData3ds(pIdChunk);
  /*--- This is validated later */
  pId->id = -1;
  AddChildOrdered3ds(pParentChunk, pIdChunk);
}

/*----------------------------------------------------------------------------
 | kfPutGenericNode
 |
 | Put a tag into database as a child of parentChunk 
 |
 +---------------------------------------------------------------------------*/
chunk3ds * kfPutGenericNode(chunktag3ds tagId, chunk3ds *pParentChunk)
{
  chunk3ds *pGenericChunk;

  InitChunk3ds(&pGenericChunk);
  ON_ERROR_RETURNR(NULL);
  
  pGenericChunk->tag = tagId;
  AddChildOrdered3ds(pParentChunk, pGenericChunk);
  return pGenericChunk;
}

/*-----------------------------------------------------------------------
  | kfPutPosTrack
  | 
  | Put a position track into database
  |
  +----------------------------------------------------------------------*/
void kfPutPosTrack(ulong3ds nKeys,    /* Number of position keys */
           ushort3ds flag,/* Loop control flag for position keys */
           keyheader3ds  *headers, /* Spline vals for keys */
           point3ds *pos,          /* Position keys  */
           chunk3ds *pParentChunk) 
{     
  chunk3ds *pPosChunk;
  PosTrackTag *pPosTrack;

  if (nKeys<1 || !headers || !pos) 
    return;
           
  /*--- Create the POS_TRACK_TAG chunk */
  InitChunk3ds(&pPosChunk);
  pPosChunk->tag = POS_TRACK_TAG;
  pPosTrack = InitChunkData3ds(pPosChunk);

  /*--- pos tag header */
  pPosTrack->trackhdr.flags = flag;
  pPosTrack->trackhdr.keycount = nKeys;
  pPosTrack->trackhdr.nu1 = pPosTrack->trackhdr.nu2 = 0; /*--- Not Used */

  /*--- Copy node headers */
  pPosTrack->keyhdrlist = malloc(nKeys * sizeof(keyheader3ds));
  /*--- Check for null pointer here later */
  memcpy(pPosTrack->keyhdrlist, headers, nKeys * sizeof(keyheader3ds));

  /*--- POINTS */
  pPosTrack->positionlist = malloc(nKeys * sizeof(point3ds));
  /*--- Check for null pointer here later */
  memcpy(pPosTrack->positionlist, pos, nKeys * sizeof(point3ds));

  AddChildOrdered3ds(pParentChunk, pPosChunk);
}

/*-----------------------------------------------------------------------
 | kfPutColorTrack
 | 
 | Put a color track into database
 |
 | Needs: index of parent stuff
 |
 +----------------------------------------------------------------------*/
void kfPutColorTrack(ulong3ds      nKeys,         /* Number of color keys */
           ushort3ds     flag,          /* Loop ctrl flag for color keys */
           keyheader3ds  *headers,      /* Spline values for color keys */
           fcolor3ds     *colors,      /* colors */
           chunk3ds      *pParentChunk) 
{     
  chunk3ds *pColorChunk;
  ColTrackTag *pColorTrack;

  if (nKeys<1 || !headers || !colors) 
    return;
           
  /*--- Create the COLOR_TRACK_TAG chunk */
  InitChunk3ds(&pColorChunk);
  pColorChunk->tag = COL_TRACK_TAG;
  pColorTrack = InitChunkData3ds(pColorChunk);

  /*--- Color tag header */
  pColorTrack->trackhdr.flags = flag;
  pColorTrack->trackhdr.keycount = nKeys;
  pColorTrack->trackhdr.nu1 = pColorTrack->trackhdr.nu2 = 0; /*--- Not Used */

  /*--- Copy node headers */
  pColorTrack->keyhdrlist = malloc(nKeys * sizeof(keyheader3ds));
  /*--- Check for null pointer here later */
  memcpy(pColorTrack->keyhdrlist, headers, nKeys * sizeof(keyheader3ds));

  /*--- COLORS */
  pColorTrack->colorlist = malloc(nKeys * sizeof(fcolor3ds));
  /*--- Check for null pointer here later */
  memcpy(pColorTrack->colorlist, colors, nKeys * sizeof(fcolor3ds));

  /*--- Make if a child of the kframe node */
  AddChildOrdered3ds(pParentChunk, pColorChunk);
}


/*---------------------------------------------------------
 | kfPutTargetTrack
 |  Put a target track into database, this goes
 |  before the [CAMERA, LIGHT etc]_TRACK_TAG
 |
 *--------------------------------------------------------*/
chunk3ds *kfPutTargetTrack(
  char3ds       *tparent,          /* Name of parent object for target */
  ulong3ds      nKeys,          /* Number of target position keys */
  ushort3ds     tflag,          /* Loop ctrl flag for target position keys */
  keyheader3ds  *headers,       /* Spline values for target position keys */
  point3ds      *pos,           /* Target position keys  */
  char3ds       *pName,
  short3ds      flags1,         /* flags 1 and 2 for header info */
  short3ds      flags2,
  targettype3ds target,         /* LightTarget or CameraTarget */
  chunk3ds      *pParentChunk)
{
  chunk3ds *pTargetChunk;

  
  if (target == LightTarget)
    pTargetChunk = kfPutGenericNode(L_TARGET_NODE_TAG, pParentChunk);
  else /* (target == CameraTarget) */
    pTargetChunk = kfPutGenericNode(TARGET_NODE_TAG, pParentChunk);

  kfPutNodeId(pTargetChunk);
  kfPutNodeHeader(pName, flags1, flags2, tparent, pTargetChunk);
  kfPutPosTrack(nKeys, tflag, headers, pos, pTargetChunk); 

  return pTargetChunk;
}


/*-------------------------------------------------------------
 | kfFindNamedAndTaggedChunk
 |
 | Look through the keyframer stuff and find named chunk
 | of the tag type tagId.
 |
 | Has to be a chunk that as a Node Header
 |  -CAMERA_NODE, LIGHT_NODE,,.
 +--------------------------------------------------------------*/
void kfFindNamedAndTaggedChunk(database3ds *db,  /* dbase to be searched */
                   const char3ds *name, /* name to search for */ 
                   chunktag3ds tagId, /* tag to search for */
                   chunk3ds **pChunk) /* return chunk here */
{
  chunk3ds  *pKfChunk, *pNodeHdrChunk, *cnode;
  
  if (db == NULL || name == NULL || pChunk == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
  
  /*--- Find Keyframe chunk */
  FindChunk3ds(db->topchunk, KFDATA, &pKfChunk);
  ON_ERROR_RETURN;
  
  /*--- Look for the target tag */
  *pChunk = NULL;
  for (FindChunk3ds(pKfChunk, tagId, &cnode);
       cnode != NULL;
       FindNextChunk3ds(cnode->sibling, tagId, &cnode))
  {
      FindNextChunk3ds(cnode->children, NODE_HDR, &pNodeHdrChunk);
      if (pNodeHdrChunk != NULL){
      NodeHdr *pNodeHdr;
      
      ReadChunkData3ds(pNodeHdrChunk);
      pNodeHdr = pNodeHdrChunk->data;
      
      /*--- Match name, set pointer */
      if (!strcmp(name, pNodeHdr->objname)){
          *pChunk = cnode;
          FreeFileChunkData3ds(pNodeHdrChunk);
          break;
      }
      FreeFileChunkData3ds(pNodeHdrChunk);
      }
  }
}


/*-----------------------------------------------------------------------
 | kfPutRollTrack
 | 
 | Put a camera roll track into database
 |
 | Needs: index of parent stuff
 |
 +----------------------------------------------------------------------*/
void kfPutRollTrack(ulong3ds nKeys,
            ushort3ds flag,
            keyheader3ds *headers,
            float3ds *roll,
            chunk3ds *pParentChunk)
{    
  chunk3ds *pRollChunk;
  RollTrackTag *data;

  /*--- Create the ROLL_TRACK_TAG chunk */
  InitChunk3ds(&pRollChunk);
  pRollChunk->tag = ROLL_TRACK_TAG;
  data = InitChunkData3ds(pRollChunk);
    
  /*--- Initialize trackhdr field */
  data->trackhdr.flags = flag;
  nKeys = data->trackhdr.keycount = nKeys;
  data->trackhdr.nu1 = data->trackhdr.nu2 = (ushort3ds)0; /*--- Not used */
    
  /*--- Copy headers */
  data->keyhdrlist  = malloc(nKeys * sizeof(keyheader3ds));
  memcpy(data->keyhdrlist, 
     headers, 
     nKeys * sizeof(keyheader3ds));
    
  /*--- Copy Points */
  data->rollanglelist = malloc(nKeys * sizeof(float3ds));
  memcpy(data->rollanglelist, roll, nKeys * sizeof(float3ds));
    
  AddChildOrdered3ds(pParentChunk, pRollChunk);
}

/*----------------------------------------------------------------------
 |
 | kfExtractXdata
 |   make copy of a tags xdata, delete the tag and return the
 |   copy via pNewXdata
 |
 +-----------------------------------------------------------------------*/
void kfExtractXdata(database3ds *db, 
            char3ds *name,     /* name of node to search for */
            chunktag3ds tagId, /* tag to search for */
            chunk3ds **pNewXdata)
{
    chunk3ds *pDeadChunk, *pOldXdata;

    pDeadChunk = FindNodeTagByNameAndType3ds(db, name, tagId);

    if (pDeadChunk != NULL)
    return;

    /*--- If the chunk exists, get xdata from it */
    FindChunk3ds(pDeadChunk, XDATA_SECTION, &pOldXdata);
    if (pOldXdata)
        CopyChunk3ds(pOldXdata, pNewXdata);

    DeleteChunk3ds(pDeadChunk);
}

/*-------------------------------------------------------------------
 |
 | kfGetGenericNodeCount
 |  ID is CAMERA_NODE_TAG etc.
 |
 +-------------------------------------------------------------------*/

ulong3ds kfGetGenericNodeCount(database3ds *db, 
                  chunktag3ds tagID)
{
   chunk3ds *current=NULL, *cnode=NULL;
   ulong3ds nodecount = 0, i;

   UpdateNodeTagList3ds(db);
   ON_ERROR_RETURNR(0);

   for (i = 0; i < db->nodelist->count; i++)
   {
      if (db->nodelist->list[i].chunk->tag == tagID) nodecount++;
   }

   return nodecount;
}

/*----------------------------------------------------------------------
 |
 | kfGetGenericNodeNameList
 |  ID is CAMERA_NODE_TAG etc.
 |
 +---------------------------------------------------------------------*/
void kfGetGenericNodeNameList(database3ds *db, 
			      chunktag3ds tagID,
			      namelist3ds **list)
{
   chunk3ds *current=NULL;
   ulong3ds nodecount = 0, i, j;
    
    if (db == NULL || list == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG); 

    UpdateNodeTagList3ds(db);
    ON_ERROR_RETURN;
    
    nodecount = kfGetGenericNodeCount(db, tagID);
    
    InitNameList3ds(list, nodecount);

    for (i = 0, j = 0; (i < db->nodelist->count) && (j < nodecount); i++)
    {
       if (db->nodelist->list[i].chunk->tag == tagID)
       {
	  (*list)->list[j].name = strdup(db->nodelist->list[i].name);
	  j++;
       }
    }
}

/*----------------------------------------------------------------------
 |
 | kfAddGenericNodeXData
 |  ID is CAMERA_NODE_TAG etc.
 |
 +---------------------------------------------------------------------*/
void kfAddGenericNodeXData(database3ds *db, 
			   char3ds *objname, 
			   char3ds *appname, 
			   chunktag3ds tagID,
			   xdata3ds *data)
{
  chunk3ds *cnode;
    
  if (db == NULL || objname == NULL || appname == NULL || data == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
    
  kfFindNamedAndTaggedChunk(db, objname, tagID, &cnode);
  /* AddXDataEntry3ds(cnode, appname, data);*/
}

/*-----------------------------------------------------------------------
 | kfGetKfSeg
 |  
 | All the keyframe information has to go in the appropriate segment,
 | KFDATA
 |
 +------------------------------------------------------------------------*/
void kfGetKfSeg(chunk3ds *pTopChunk, chunk3ds **pKData)
{
  chunk3ds *pChunk = NULL;

  /*--- Look for KFDATA */
  FindNextChunk3ds(pTopChunk->children, KFDATA, &pChunk);
  if (pChunk == NULL) 
    pChunk = kfPutGenericNode(KFDATA, pTopChunk);

  *pKData = pChunk;
}
  
/*-----------------------------------------------------------------------
 |
 | kfGetParentName
 | 
 | Get Parent Name if there is one 
 |
 +-----------------------------------------------------------------------*/
void kfGetParentName(chunk3ds *pChunk, char3ds *pChar)
{
  chunk3ds *pNameChunk;
  NamedObject *nObj;

  pChar[0] = 0;
  FindChunk3ds(pChunk, PARENT_NAME, &pNameChunk);
  if (pNameChunk == NULL)
    return;

  nObj = ReadChunkData3ds(pNameChunk);
  if (nObj == NULL)
    return;

  if (nObj->name != 0 && nObj->name[0] != 0)
    strcpy(pChar, nObj->name);

  FreeFileChunkData3ds(pNameChunk);
}

void kfAddParentName(chunk3ds *pChunk, char3ds *pChar)
{
  chunk3ds *temp;
  InstanceName *data;

  if (pChar == 0)
    return;

  InitChunk3ds(&temp);
  temp->tag = PARENT_NAME;
  data = InitChunkData3ds(temp);
  
  data->name = strdup(pChar);
  AddChildOrdered3ds(pChunk, temp);
}

