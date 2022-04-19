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
#include "3dscamm.h"
#include "kfutils.h"
#include "3dskey.h"

/*------------------------------------------------------------
 | 
 | InitKeyInfo3ds
 | 
 +------------------------------------------------------------*/
void InitKeyInfo3ds(kfkeyinfo3ds *key)
{
  key->length = 30;
  key->curframe = 0;
}

void InitKfSegment3ds(kfsegment3ds *key)
{
  key->use = False3ds;
  key->begin = 0;
  key->end = 30;
}

void InitKfSets3ds(kfsets3ds **key)
{
  if (key == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

  if (*key == NULL)
  {
    *key = malloc(sizeof(kfsets3ds));
    if (*key == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
  }

  InitKeyInfo3ds(&((*key)->anim));
  InitKfSegment3ds(&((*key)->seg));
}

void ReleaseKfSets3ds(kfsets3ds **key)
{
  if (key != NULL)
  {
    free(*key);
    *key = NULL;
  }
}

void GetKeyInfo3ds(database3ds *db, kfkeyinfo3ds *key)
{
   chunk3ds *kfdata, *kfhdr, *kfcurtime;
   myassert(key != NULL, "GetKeyInfo3ds: arg *key cannot be NULL");

   kfGetKfSeg(db->topchunk, &kfdata);
   ON_ERROR_RETURN;

   FindNextChunk3ds(kfdata->children, KFHDR, &kfhdr);

   if (kfhdr != NULL)
   {
      KFHdr *d;
      
      d = ReadChunkData3ds(kfhdr);
      ON_ERROR_RETURN;

      key->length = d->animlength;

      FreeFileChunkData3ds(kfhdr);
   }

   FindNextChunk3ds(kfdata->children, KFCURTIME, &kfcurtime);

   if (kfcurtime != NULL)
   {
      KFCurtime *d;

      d = ReadChunkData3ds(kfcurtime);
      ON_ERROR_RETURN;

      key->curframe = d->currframenum;
   }
}

   
/*---------------------------------------------------------
 | PutKeyInfo
 |  Puts keyframe header information. Note:
 |   KeyframeHeader filename not handled in this
 |   version.         -sjw
 |
 +--------------------------------------------------------*/
void PutKeyInfo3ds(database3ds *db, kfkeyinfo3ds *key)
{
  chunk3ds *pChunk = NULL, *pKfChunk = NULL;
  KFHdr *hdata;
/*  KFSeg *fdata;  */
  KFCurtime *cdata;

  kfGetKfSeg(db->topchunk, &pKfChunk);
  ON_ERROR_RETURN;

  /*--- KEYFRAME HEADER */
  FindChunk3ds(pKfChunk, KFHDR, &pChunk);
  if (pChunk == NULL){ 
    InitChunk3ds(&pChunk); ON_ERROR_RETURN;
    pChunk->tag = KFHDR;
    hdata = InitChunkData3ds(pChunk);
    hdata->revision = 5; /*--- Default value */
    hdata->animlength = key->length;
    /*--- Filename Not Handled for now */
    hdata->filename = strdup("");
    AddChildOrdered3ds(pKfChunk, pChunk);ON_ERROR_RETURN;
  }
  else{
    hdata = ReadChunkData3ds(pChunk); ON_ERROR_RETURN;
    hdata->animlength = key->length;
  }

#ifdef DONT_DO
  /*--- KEYFRAME SEGMENT */
  pChunk = NULL;
  FindChunk3ds(pKfChunk, KFSEG, &pChunk);
  if (pChunk == NULL){ 
    InitChunk3ds(&pChunk); ON_ERROR_RETURN;
    pChunk->tag = KFSEG;
    fdata = InitChunkData3ds(pChunk);
    fdata->first = key->firstframe;
    fdata->last =  key->lastframe;
    AddChildOrdered3ds(pKfChunk, pChunk); ON_ERROR_RETURN;
  }
  else{
    fdata = ReadChunkData3ds(pChunk); ON_ERROR_RETURN;
    fdata->first = key->firstframe;
    fdata->last =  key->lastframe;
  }
#endif
  
  /*--- KEYFRAME CURTIME */ 
  pChunk = NULL;
  FindChunk3ds(pKfChunk, KFCURTIME, &pChunk);
  if (pChunk == NULL){ 
    InitChunk3ds(&pChunk); ON_ERROR_RETURN;
    pChunk->tag = KFCURTIME;
    cdata = InitChunkData3ds(pChunk);
    cdata->currframenum = key->curframe;
    AddChildOrdered3ds(pKfChunk, pChunk);
  }
  else {
    cdata = ReadChunkData3ds(pChunk); ON_ERROR_RETURN;
    cdata->currframenum = key->curframe;
  }
}

void GetKfSegment3ds(database3ds *db, kfsegment3ds *key)
{
   chunk3ds *kfdata, *kfseg;
   KFSeg *data;

   kfGetKfSeg(db->topchunk, &kfdata);

   FindNextChunk3ds(kfdata->children, KFSEG, &kfseg);
   if (kfseg != NULL)
   {
      data = ReadChunkData3ds(kfseg);
      ON_ERROR_RETURN;

      key->use = True3ds;
      key->begin = data->first;
      key->end = data->last;

      FreeFileChunkData3ds(kfseg);
   }
}
   

void PutKfSegment3ds(database3ds *db, kfsegment3ds *key)
{
  chunk3ds *kfdata, *kfseg;
  KFSeg *data;

  kfGetKfSeg(db->topchunk, &kfdata);

  if (key->use)
  {
    ReplaceOrAddChild3ds(kfdata, KFSEG, &kfseg);
    ON_ERROR_RETURN;
    
    data = InitChunkData3ds(kfseg);
    ON_ERROR_RETURN;
    
    data->first = key->begin;
    data->last = key->end;
  } else
  {
    FindNextChunk3ds(kfdata->children, KFSEG, &kfseg);
    DeleteChunk3ds(kfseg);
  }
}

void GetKfSets3ds(database3ds *db, kfsets3ds **key)
{
   kfsets3ds *k;
   
   if(db == NULL || key == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   InitKfSets3ds(key);
   ON_ERROR_RETURN;

   k = *key;
   
   GetKeyInfo3ds(db, &(k->anim));
   ON_ERROR_RETURN;

   GetKfSegment3ds(db, &(k->seg));
   ON_ERROR_RETURN;
}


void PutKfSets3ds(database3ds *db, kfsets3ds *key)
{
  if(db == NULL || key == NULL) 
    SET_ERROR_RETURN(ERR_INVALID_ARG);
  if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
  if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC))
     SET_ERROR_RETURN(ERR_WRONG_DATABASE);

  PutKeyInfo3ds(db, &(key->anim));
  ON_ERROR_RETURN;
  
  PutKfSegment3ds(db, &(key->seg));
  ON_ERROR_RETURN;
}

  
void CopyKfSets3ds(database3ds *destdb, database3ds *srcdb)
{
   chunk3ds *srckdata, *destkdata, *srcchunk, *destchunk;

   if ((destdb == NULL) || (srcdb == NULL))
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL))
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);


   FindNextChunk3ds(srcdb->topchunk->children, KFDATA, &srckdata);

   if (srckdata != NULL)
   {
      
      kfGetKfSeg(destdb->topchunk, &destkdata);
      ON_ERROR_RETURN;

      FindNextChunk3ds(srckdata, KFHDR, &srcchunk);

      if (srcchunk != NULL)
      {
	 FindNextChunk3ds(destkdata->children, srcchunk->tag, &destchunk);

	 if (destchunk != NULL) DeleteChunk3ds(destchunk);

	 CopyChunk3ds(srcchunk, &destchunk);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(destkdata, destchunk);

      }

      FindNextChunk3ds(srckdata, KFCURTIME, &srcchunk);

      if (srcchunk != NULL)
      {
	 FindNextChunk3ds(destkdata->children, srcchunk->tag, &destchunk);

	 if (destchunk != NULL) DeleteChunk3ds(destchunk);

	 CopyChunk3ds(srcchunk, &destchunk);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(destkdata, destchunk);

      }

      FindNextChunk3ds(srckdata, KFSEG, &srcchunk);

      if (srcchunk != NULL)
      {
	 FindNextChunk3ds(destkdata->children, srcchunk->tag, &destchunk);

	 if (destchunk != NULL) DeleteChunk3ds(destchunk);

	 CopyChunk3ds(srcchunk, &destchunk);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(destkdata, destchunk);

      }

   }
}

	 
