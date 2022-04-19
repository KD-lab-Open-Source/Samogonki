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
#include "3dserr.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "3dshier.h"
#include "kfutils.h"

static void s_assignParentNames(database3ds *db); 
static void s_computeNodeIds(database3ds *db);

/*------------------------------------------------
 | 
 |  nodeTags: Those tags that are in hierarchy
 | 
 +-----------------------------------------------*/
static chunktag3ds nodeTags[] = {
  TARGET_NODE_TAG,
  OBJECT_NODE_TAG,
  CAMERA_NODE_TAG,
  LIGHT_NODE_TAG,
  L_TARGET_NODE_TAG,
  SPOTLIGHT_NODE_TAG,
  (chunktag3ds)NULL,
};

static IdNodePtr nodeList = NULL;

/*----------------------------------------------------------
 | AssignParentNames
 |  convert parent ID's to parent names.
 |
 +----------------------------------------------------------*/
void AssignParentNames(database3ds *db)
{
  if (db == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

  InitNodeList();
  ParseDb(db);
  s_assignParentNames(db);
  ReleaseNodeList();
}

/*----------------------------------------------------------
 | ComputeNodeIds
 |  Assign NODE_ID's and parentId's
 |
 +----------------------------------------------------------*/
void ComputeNodeIds(database3ds *db)
{
  if (db == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
 
  InitNodeList();
  s_computeNodeIds(db);
  ReleaseNodeList();
 
}

/*---------------------------------------------------
 |
 | IsNode3ds: return True3ds if id is node id
 | 
 +---------------------------------------------------*/
short3ds IsNode3ds(chunktag3ds tagID)
{
  long3ds i = 0;

  while(nodeTags[i] != (chunktag3ds)NULL){
    if (tagID == nodeTags[i])
      return True3ds;
    i++;
  }
  return False3ds;
}

/*---------------------------------------------------
 |
 | InitNodeList
 | 
 +---------------------------------------------------*/
void InitNodeList()
{
  if (nodeList != NULL)
    ReleaseNodeList();
}

/*-------------------------------------------------------
 |
 | makeNode
 |  add node to linked list (uninitialized)
 |
 +------------------------------------------------------*/
static void makeNode(IdNodePtr *pPNode)
{
  IdNodePtr pNode, pId = nodeList;
  
  pNode = malloc(sizeof(IdNode));
  if (pNode != NULL){
    if (!pId){ /*--- First Node */
      nodeList = pNode;
    }
    else{  /*--- Add to list */
      while(pId->next)
    pId = pId->next;
      pId->next = pNode;
    }
  }
  /* else push error here */
    
  *pPNode = pNode;
}

static short3ds getChunkNodeId(chunk3ds *pChunk)
{
  chunk3ds *pIdChunk;

  short3ds retVal = kNoId;

  FindChunk3ds(pChunk, NODE_ID, &pIdChunk);
  if (pIdChunk != NULL){
    KFId *pKfId = ReadChunkData3ds(pIdChunk);
    if (pKfId != NULL) retVal = pKfId->id;
    FreeFileChunkData3ds(pIdChunk);    
  }

  return retVal;
}


/*-------------------------------------------------
 |
 | addToNodeList
 |   create a node, put node in list and fill-in
 |   structure.
 |
 +------------------------------------------------*/
static void addToNodeList(chunk3ds *pChunk)
{
  IdNodePtr pNewNode;
  chunk3ds *pHdrChunk, *pInstChunk;
  NodeHdr *pNodeHdr;

  makeNode(&pNewNode);
  if (pNewNode == NULL)
    return;

  FindChunk3ds(pChunk, NODE_HDR, &pHdrChunk);
  if (pHdrChunk == NULL)
    return;

  pNodeHdr = ReadChunkData3ds(pHdrChunk);
  if (pNodeHdr == NULL)
    return;
  
  /*--- FILL IN NODE DATA */
  pNewNode->name = strdup(pNodeHdr->objname);
  pNewNode->Id = getChunkNodeId(pChunk);  
  pNewNode->tag = pChunk->tag;
  pNewNode->parentId = pNodeHdr->parentindex;
  pNewNode->next = NULL;
  pNewNode->inst = NULL;
  
  /*--- Check for INSTANCE */
  if (pChunk->tag == OBJECT_NODE_TAG){
    FindChunk3ds(pChunk, INSTANCE_NAME, &pInstChunk);
    if (pInstChunk != NULL){
      InstanceName *data = ReadChunkData3ds(pInstChunk);
      if (data != NULL && data->name != NULL){
	pNewNode->inst = strdup(data->name);
	FreeFileChunkData3ds(pInstChunk);
      }
    }
  }
  FreeFileChunkData3ds(pHdrChunk); 
}

/*------------------------------------------------------------------
 |
 | PrintNodeList
 |   Simple array style print-out of what is in current nodeList.
 |
 +------------------------------------------------------------------*/
void PrintNodeList(FILE *outfile)
{
  IdNodePtr pId = nodeList;

  while(pId){
    if (pId->inst == NULL)
      fprintf(outfile, "ID:%d\tName:%s\tParent:%d\n", 
          (int)pId->Id,
          pId->name,
          (int)pId->parentId);
    else
      fprintf(outfile, "ID:%d\tName:%s.%s\tParent:%d\n", 
          (int)pId->Id,
          pId->name,
          pId->inst,
          (int)pId->parentId);
    pId = pId->next;
  }
}


static void printNode(FILE *outfile, IdNodePtr pId, int spaces)
{
  int i = 0;
  IdNodePtr nList = nodeList;

  while(i<spaces){
    fprintf(outfile, "-");
    i++;
  }
  fprintf(outfile, "%s\n", pId->name);
  
  while(nList){
    if (nList->parentId == pId->Id)
      printNode(outfile, nList, spaces+1);
    nList = nList->next;
  }
}

/*------------------------------------------------------------------
 |
 | PrintHierarchy
 |   Hierarchy print-out of parents and thier children
 |
 +------------------------------------------------------------------*/
void PrintHierarchy(FILE *outfile)
{
  IdNodePtr pId = nodeList;

  while(pId){
    /*--- Only do those w/o parents, rest do themselves */
    if (pId->parentId == -1)
      printNode(outfile, pId, 0);
    pId = pId->next;
  }
}



/*-------------------------------------------------------------------
 |
 |  ReleaseNodeList
 |
 +------------------------------------------------------------------*/
void ReleaseNodeList()
{
  IdNodePtr pId = nodeList, pNext;

  while (pId != NULL)
    {
      pNext = pId;
      if (pId->name)
    free(pId->name);
      if (pId->inst)
    free(pId->inst);
      pNext = pId->next;
      free(pId);
      pId = pNext;
    }
  nodeList = NULL;
}

/*----------------------------------------------------------------
 |
 | checkListNodeIds
 |   earlier versions (pre 3) of 3dstudio had no node ids, it
 |   simple used the order in which they came along, if so
 |   put in NODE IDs. Assuming that if one node has no ID the
 |   whole list get renumbered.
 |
 +---------------------------------------------------------------*/
static void checkListNodeIds()
{
  IdNodePtr pId = nodeList;

  while(pId){
    if (pId->Id == kNoId){ /*--- If somebody has no ID renumber list */
      short3ds index = 0;

      pId = nodeList;
      while(pId){
    pId->Id = index++;
    pId = pId->next;
      }
      break;
    }
    pId = pId->next;
  }
}


/*-------------------------------------------------------------------
 |
 | ParseDb
 |   Traverse the database and fill in the nodeList.
 |   InitNodeList first.
 |
 +-------------------------------------------------------------------*/
void ParseDb(database3ds *db)
{
  chunk3ds *pChunk, *pKfData;

  FindChunk3ds(db->topchunk, KFDATA, &pKfData);
  if (pKfData == NULL) 
    return;

  for (pChunk=pKfData->children; pChunk!=NULL; pChunk=pChunk->sibling){
    if (IsNode3ds(pChunk->tag) == True3ds)
      addToNodeList(pChunk);
  }
  
  checkListNodeIds();
}

static IdNodePtr findNodeById(short3ds id)
{
  IdNodePtr pId = nodeList;

  while (pId != NULL){
    if (pId->Id == id)
      break;
    pId = pId->next;
  }

  return pId;
}

#ifdef NOT_USED_IN_THIS_MODULE
static IdNodePtr findNodeByNameAndId(char3ds *name, chunktag3ds id)
{
  IdNodePtr pId = nodeList;

  while (pId != NULL){
    if (strcmp(pId->name, name) == 0 &&
    pId->tag == id)
      break;
    pId = pId->next;
  }

  return pId;
}
#endif

static IdNodePtr findObjNodeByName(char3ds *name)
{
  IdNodePtr pId = nodeList;
  
  /*--- extract names */
  if (strchr(name, (int)'.') != NULL){
    char3ds *pC, *pName, *pInst;
    
    /*--- Copy name and cutoff instance name */
    pName = strdup(name);
    pC = strchr(pName, (int)'.');
    *pC = (char3ds)0;
    
    /*--- point to instance part of orignal name string */
    pInst = strchr(name, (int)'.') + 1;
    
    while (pId != NULL){
      if (strcmp(pId->name, pName) == 0 && 
      strcmp(pId->inst, pInst) == 0){
    break;
      }
      pId = pId->next;
    }
    free(pName);
  }
  else {  /*--- No Instance name */
     while (pId != NULL){
	if (strcmp(pId->name, name) == 0)
	{
	   if (pId->inst == NULL) break;
	   if (pId->inst[0] == 0) break;
	}
	pId = pId->next;
     }
  }    
  return pId;
}


/*-----------------------------------------------------
 |
 | s_AssignParentNames
 |   Traverse keyframe data and assign parent
 |   names to its own chunk PARENT_NAME which is
 |   is a child of NODE_HDR.
 | 
 |  NODE_HDR
 |    PARENT_NAME
 +----------------------------------------------------*/
static void s_assignParentNames(database3ds *db)
{
  chunk3ds *pChunk, *pKfData, *pHdrChunk, *pNameChunk, *pIdChunk;
  int i = 0;
  KFId *pKfId;
  IdNodePtr pIdNode, pIdParentNode;
  char3ds *pName = NULL, *pInst = NULL;
  
  FindChunk3ds(db->topchunk, KFDATA, &pKfData);
  if (pKfData == NULL) 
    return;
  
  /*--- Find chunks in KFRAMER */
  while(nodeTags[i] != (chunktag3ds)NULL){
    FindChunk3ds(pKfData, nodeTags[i], &pChunk);
    if (pChunk){
      
      while(pChunk){
    FindChunk3ds(pChunk, NODE_HDR, &pHdrChunk);
    if (pHdrChunk == NULL)
      goto NEXT;
    
    FindChunk3ds(pChunk, NODE_ID, &pIdChunk);
    if (pIdChunk == NULL)
      goto NEXT;

    pKfId = ReadChunkData3ds(pIdChunk);
    if (pKfId == NULL)
      goto NEXT;
    
    /*--- Find table entry for node of interest */
    pIdNode = findNodeById(pKfId->id);
    /*--- No ID (bad) or No Parent (ok) */
    if (pIdNode == NULL || pIdNode->parentId == -1)
      goto NEXT;
    
    /*--- Find table entry for PARENT */
    pIdParentNode = findNodeById(pIdNode->parentId);
    if (pIdParentNode != NULL){
      pName = pIdParentNode->name;
      pInst = pIdParentNode->inst;
    }
    
    if (pName == NULL)
      goto NEXT;
    
    /*--- Concatenate names if there is an inst name  */
    if (pInst != NULL)
      {
        char3ds *pC;
        
        pC = malloc(strlen(pName) + strlen(pInst) + 2);
        sprintf(pC, "%s.%s", pName, pInst);
        pName = pC;
      }
    
    /*--- If PARENT chunk exists, copy into it */
    FindChunk3ds(pHdrChunk, PARENT_NAME, &pNameChunk);
    if (pNameChunk != NULL){
      InstanceName *pInstName;
      
      pInstName = ReadChunkData3ds(pNameChunk);
      if (pInstName == NULL){
        if (pInst) free(pName);
        goto NEXT;
      }
      strcpy(pInstName->name, pName);
    }
    else /*--- Create PARENT_NAME chunk */
      kfAddParentName(pHdrChunk, pName);
    
    if (pInst) free(pName);    
    
      NEXT:FindNextChunk3ds(pChunk->sibling, nodeTags[i], &pChunk);
      }
    }
    i++;
  }
}

static void setNodeId(chunk3ds *pChunk, short3ds id)
{
  chunk3ds *pIdChunk;
  KFId *pKfId;

  FindChunk3ds(pChunk, NODE_ID, &pIdChunk);
  if (pIdChunk == NULL)
    return;
  pKfId = ReadChunkData3ds(pIdChunk);
  if (pKfId == NULL)
    return;

  pKfId->id = id;
}
/*-----------------------------------------------------
 |
 | s_computeNodeIds
 |   Traverse keyframe data and assign NODE_ID's
 |   and fill in parentId's of named parents.
 |
 | Note: naming scheme
 |
 |  NODE                         PARENT
 |  parentname:foo.bar  ---->    name:foo
 |                               instance:bar
 |
 |  parentname:foo      ---->    name:foo
 |                               instance:(NULL)
 |
 +----------------------------------------------------*/
static void s_computeNodeIds(database3ds *db)
{
  chunk3ds *pChunk, *pKfData;
  short3ds i = 0;
  IdNodePtr pId;

  InitNodeList();

  FindChunk3ds(db->topchunk, KFDATA, &pKfData);
  if (pKfData == NULL) 
    return;

  /*--- Set Node ID's and build list */
  for (pChunk=pKfData->children; pChunk!=NULL; pChunk=pChunk->sibling){
    if (pChunk->tag == AMBIENT_NODE_TAG){
      setNodeId(pChunk, -1);
      continue;
    }

    if (IsNode3ds(pChunk->tag) == True3ds){
      setNodeId(pChunk, i);
      addToNodeList(pChunk);
      i++;
    }    
  }

  /*--- Assign parent IDs base on parent names */
  for (pChunk=pKfData->children; pChunk!=NULL; pChunk=pChunk->sibling){
    chunk3ds *pHdrChunk, *pNameChunk;
    NodeHdr *pNodeHdr;
    InstanceName *pInstName;

    if (IsNode3ds(pChunk->tag) == False3ds)
      continue;

    /*--- Get Header and Data */
    FindChunk3ds(pChunk, NODE_HDR, &pHdrChunk);
    if (pHdrChunk == NULL)
      continue;
    pNodeHdr = ReadChunkData3ds(pHdrChunk);
    if (pNodeHdr == NULL)
      continue;

    /*--- Get PARENT_NAME and Data */
    FindChunk3ds(pHdrChunk, PARENT_NAME, &pNameChunk);
    /*--- No parent chunk ---> no parent */
    if (pNameChunk == NULL){
      pNodeHdr->parentindex = -1;
      continue;
    }
    pInstName = ReadChunkData3ds(pNameChunk);
    if (pInstName == NULL)
      continue;

    /*--- Find the table entry of the parent */
    pId = findObjNodeByName(pInstName->name);

    /*--- If there is an Id use it, else no parent */
    pNodeHdr->parentindex = (short3ds)((pId)? pId->Id : -1);
    FreeFileChunkData3ds(pNameChunk);
  }
}

