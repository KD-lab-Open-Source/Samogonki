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

/*-----------------------------------------------------------
 |  kfutils.h
 |  Header file for kfutils.c
 |  sjw 4/94
 *-----------------------------------------------------------*/

#ifndef KFUTILS_3DS_H
#define KFUTILS_3DS_H

#include "3dstype.h"
#include "3dsprim.h"
#include "dbase3ds.h"
#include "xdata.h"
#include "chunk3ds.h"

/* Public Consts */
/* Flag settings for kfmesh3ds, kfomni3ds, kfcamera3ds, kfambient, and kfspot3ds */

/* For the flags field */
static const ushort3ds KfNodeOff3ds = 1<<3;
static const ushort3ds KfHideNode3ds = 1<<11;
static const ushort3ds KfFastNode3ds = 1<<12;

/* For the flags2 field */
static const ushort3ds KfNodeHasPath3ds = 1;
static const ushort3ds KfNodeAutoSmooth3ds = 1<<1;
static const ushort3ds KfNodeFrozen3ds = 1<<2;
static const ushort3ds KfMotionBlur3ds = 1<<4;
static const ushort3ds KfBlurBranch3ds = 1<<5;
static const ushort3ds KfMorphMtl3ds   = 1<<6;
static const ushort3ds KfMorphOb       = 1<<7;

typedef enum
{
  LightTarget,
  CameraTarget
} targettype3ds;

/* End Public */

/*--- Values for flags */
#define NODE_RENDOB_HIDE  (1<<2)
#define NODE_OFF 	  (1<<3)
#define ATKEY1		  (1<<4)
#define ATKEY2		  (1<<5)
#define ATKEY3		  (1<<6)
#define ATKEY4		  (1<<7)
#define ATKEY5		  (1<<8)
#define ATKEYFLAGS        (ATKEY1|ATKEY2|ATKEY3|ATKEY4|ATKEY5)
#define MARK_NODE	  (1<<9)
#define DISABLE_NODE      (1<<10)
#define HIDE_NODE 	  (1<<11)
#define FAST_NODE 	  (1<<12) /* draw node quikdraw style */
#define PRIMARY_NODE      (1<<14)  /* corresponds to mesh */
#define NODE_CALC_PATH    (1<<15)  

/*--- Values for flags2 */
#define NODE_HAS_PATH     (1)
#define NODE_AUTO_SMOOTH  (1<<1)
#define NODE_FROZEN       (1<<2)
#define NODE_ANI_HIDDEN   (1<<3)
#define NODE_MOTION_BLUR  (1<<4)
#define NODE_BLUR_BRANCH  (1<<5)
#define NODE_MORPH_MTL    (1<<6)
#define NODE_MORPH_OB     (1<<7)

/*--- Routines for setting and clearing flags fields */

/*--- flags --------------------------------------------*/
#define CHECK_NODE_RENDOB_HIDE(flag) (flag&NODE_RENDOB_HIDE)
#define CHECK_NODE_OFF(flag)         (flag&NODE_OFF)
#define CHECK_ATKEY1(flag)           (flag&ATKEY1)
#define CHECK_ATKEY2(flag)           (flag&ATKEY2)
#define CHECK_ATKEY3(flag)           (flag&ATKEY3)
#define CHECK_ATKEY4(flag)           (flag&ATKEY4)
#define CHECK_ATKEY5(flag)           (flag&ATKEY5)
#define CHECK_ATKEYFLAGS(flag)       (flag&ATKEYFLAGS)
#define CHECK_MARK_NODE(flag)        (flag&MARK_NODE)
#define CHECK_DISABLE_NODE(flag)     (flag&DISABLE_NODE)
#define CHECK_HIDE_NODE(flag)        (flag&HIDE_NODE)
#define CHECK_FAST_NODE(flag)        (flag&FAST_NODE)
#define CHECK_PRIMARY_NODE(flag)     (flag&PRIMARY_NODE)
#define CHECK_NODE_CALC_PATH(flag)   (flag&NODE_CALC_PATH)

#define SET_NODE_RENDOB_HIDE(flag) (flag|=NODE_RENDOB_HIDE)
#define SET_NODE_OFF(flag)         (flag|=NODE_OFF)
#define SET_ATKEY1(flag)           (flag|=ATKEY1)
#define SET_ATKEY2(flag)           (flag|=ATKEY2)
#define SET_ATKEY3(flag)           (flag|=ATKEY3)
#define SET_ATKEY4(flag)           (flag|=ATKEY4)
#define SET_ATKEY5(flag)           (flag|=ATKEY5)
#define SET_ATKEYFLAGS(flag)       (flag|=ATKEYFLAGS)
#define SET_MARK_NODE(flag)        (flag|=MARK_NODE)
#define SET_DISABLE_NODE(flag)     (flag|=DISABLE_NODE)
#define SET_HIDE_NODE(flag)        (flag|=HIDE_NODE)
#define SET_FAST_NODE(flag)        (flag|=FAST_NODE)
#define SET_PRIMARY_NODE(flag)     (flag|=PRIMARY_NODE)
#define SET_NODE_CALC_PATH(flag)   (flag|=NODE_CALC_PATH)

#define CLEAR_NODE_RENDOB_HIDE(flag) (flag&=~NODE_RENDOB_HIDE)
#define CLEAR_NODE_OFF(flag)         (flag&=~NODE_OFF)
#define CLEAR_ATKEY1(flag)           (flag&=~ATKEY1)
#define CLEAR_ATKEY2(flag)           (flag&=~ATKEY2)
#define CLEAR_ATKEY3(flag)           (flag&=~ATKEY3)
#define CLEAR_ATKEY4(flag)           (flag&=~ATKEY4)
#define CLEAR_ATKEY5(flag)           (flag&=~ATKEY5)
#define CLEAR_ATKEYFLAGS(flag)       (flag&=~ATKEYFLAGS)
#define CLEAR_MARK_NODE(flag)        (flag&=~MARK_NODE)
#define CLEAR_DISABLE_NODE(flag)     (flag&=~DISABLE_NODE)
#define CLEAR_HIDE_NODE(flag)        (flag&=~HIDE_NODE)
#define CLEAR_FAST_NODE(flag)        (flag&=~FAST_NODE)
#define CLEAR_PRIMARY_NODE(flag)     (flag&=~PRIMARY_NODE)
#define CLEAR_NODE_CALC_PATH(flag)   (flag&=~NODE_CALC_PATH)

/*--- flags2 ----------------------------------------------*/
#define CHECK_HAS_PATH(flag)     (flag&NODE_HAS_PATH)
#define CHECK_AUTO_SMOOTH(flag)  (flag&NODE_AUTO_SMOOTH)
#define CHECK_FROZEN(flag)       (flag&NODE_FROZEN)
#define CHECK_ANI_HIDDEN(flag)   (flag&NODE_ANI_HIDDEN)
#define CHECK_MOTION_BLUR(flag)  (flag&NODE_MOTION_BLUR)
#define CHECK_BLUR_BRANCH(flag)  (flag&NODE_BLUR_BRANCH)
#define CHECK_MORPH_MATL(flag)   (flag&NODE_MORPH_MATL)
#define CHECK_MORPH_OB(flag)     (flag&NODE_MORPH_OB)

#define SET_HAS_PATH(flag)     (flag|=NODE_HAS_PATH)
#define SET_AUTO_SMOOTH(flag)  (flag|=NODE_SMOOTH)
#define SET_FROZEN(flag)       (flag|=NODE_FROZEN)
#define SET_ANI_HIDDEN(flag)   (flag|=NODE_ANI_HIDDEN)
#define SET_MOTION_BLUR(flag)  (flag|=NODE_MOTION_BLUR)
#define SET_BLUR_BRANCH(flag)  (flag|=NODE_BLUR_BRANCH)
#define SET_MORPH_MATL(flag)   (flag|=NODE_MORPH_MATL)
#define SET_MORPH_OB(flag)     (flag|=NODE_MORPH_OB)

#define CLEAR_HAS_PATH(flag)     (flag&=~NODE_HAS_PATH)
#define CLEAR_AUTO_SMOOTH(flag)  (flag&=~NODE_AUTO_SMOOTH)
#define CLEAR_FROZEN(flag)       (flag&=~NODE_FROZEN)
#define CLEAR_ANI_HIDDEN(flag)   (flag&=~NODE_ANI_HIDDEN)
#define CLEAR_MOTION_BLUR(flag)  (flag&=~NODE_MOTION_BLUR)
#define CLEAR_BLUR_BRANCH(flag)  (flag&=~NODE_BLUR_BRANCH)
#define CLEAR_MORPH_MATL(flag)   (flag&=~NODE_MORPH_MATL)
#define CLEAR_MORPH_OB(flag)     (flag&=~NODE_MORPH_OB)

/*--- prototypes */

void kfPutNodeHeader(char3ds *objname, 
		     short3ds flags1,
		     short3ds flags2, 
		     char3ds  *parentname,
		     chunk3ds *pParentChunk);


void kfPutNodeId(chunk3ds *pParentChunk);

chunk3ds * kfPutGenericNode(chunktag3ds tagId, 
			    chunk3ds *pParentChunk);

void kfPutPosTrack(ulong3ds      nKeys,
		   ushort3ds     flag,
		   keyheader3ds  *headers,
		   point3ds      *pos,
		   chunk3ds      *pParentChunk);

chunk3ds *kfPutTargetTrack(char3ds *tparent,
		      ulong3ds      nKeys,        
		      ushort3ds      tflag,        
		      keyheader3ds  *headers,     
		      point3ds      *pos,         
		      char3ds       *pName,
		      short3ds      flags1,       
		      short3ds      flags2,
		      targettype3ds target,
		      chunk3ds      *pParentChunk);

void kfPutColorTrack(ulong3ds      nKeys, 
		     ushort3ds      flag,          
		     keyheader3ds  *headers,      
		     fcolor3ds     *colors,      
		     chunk3ds      *pParentChunk); 

void kfFindNamedAndTaggedChunk(database3ds *db,
			       const char3ds *name,
			       chunktag3ds tagId,
			       chunk3ds **pChunk);

void kfExtractXdata(database3ds *db, 
		    char3ds *name, 
		    chunktag3ds tagId, 
		    chunk3ds **pNewXdata);

void kfPutRollTrack(ulong3ds nKeys,
		    ushort3ds flag,
		    keyheader3ds *headers,
		    float3ds *roll,
		    chunk3ds *pParentChunk);

ulong3ds kfGetGenericNodeCount(database3ds *db, 
			      chunktag3ds tagID);

void kfGetGenericNodeNameList(database3ds *db, 
			      chunktag3ds tagID,
			      namelist3ds **list);

void kfAddGenericNodeXData(database3ds *db, 
			   char3ds *objname, 
			   char3ds *appname, 
			   chunktag3ds tagID,
			   xdata3ds *data);

void kfAddParentName(chunk3ds *pChunk, char3ds *pChar);
void kfGetKfSeg(chunk3ds *pTopChunk, chunk3ds **pKSeg);
void kfGetParentName(chunk3ds *pChunk, char3ds *pChar);


#endif /* KFUTILS_3DS_H */
