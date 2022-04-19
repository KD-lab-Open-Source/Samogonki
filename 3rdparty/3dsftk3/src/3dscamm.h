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

#ifndef CAMM_3DS_H
#define CAMM_3DS_H

#include "3dstype.h"
#include "3dsprim.h"
#include "dbase3ds.h"
#include "chunk3ds.h"

#define NoParent3ds -1

/* Public Typedefs */

typedef struct {
  char3ds name[11];    /* Name of camera object */
  char3ds parent[22];  /* Name of parent object */
  
  ushort3ds flags1;    /* flags field from node header -fixup later */
  ushort3ds flags2;    /* flags2 field from node header -fixup later */
  
  ulong3ds npkeys;     /* Number of camera position keys */
  ushort3ds npflag;    /* Loop control flag for camera position keys */
  keyheader3ds *pkeys; /* Spline values for camera position keys */
  point3ds *pos;       /* Camera position keys */
  
  ulong3ds nfkeys;     /* Number of camera FOV keys */
  ushort3ds nfflag;    /* Loop control flag for camera FOV keys */
  keyheader3ds *fkeys; /* Spline values for camera FOV keys */
  float3ds *fov;       /* Camera FOV keys */
  
  ulong3ds nrkeys;     /* Number of camera roll keys */
  ushort3ds nrflag;    /* Loop control flag for camera roll keys */
  keyheader3ds *rkeys; /* Spline values for camera roll keys */
  float3ds *roll;      /* Camera roll keys */
  
  char3ds tparent[22]; /* Index of parent object for target */
  ulong3ds ntkeys;     /* Number of target position keys */
  ushort3ds ntflag;    /* Loop control flag for target position keys */
  keyheader3ds *tkeys; /* Spline values for target position keys */
  point3ds *tpos;      /* Target position keys */
  ushort3ds tflags1;   /* flags field from target node header */
  ushort3ds tflags2;   /* flags field from target node header */
} kfcamera3ds;
/* End Public */

/* Public Prototypes */

void InitCameraMotion3ds(kfcamera3ds **cam, ulong3ds npkeys, ulong3ds nfkeys,
                             ulong3ds nrkeys, ulong3ds ntkeys);

void GetCameraMotionByName3ds(database3ds *db, 
			      const char3ds *name, 
			      kfcamera3ds **kfcam);

void GetCameraMotionByIndex3ds(database3ds *db, 
			       ulong3ds index,
			       kfcamera3ds **kfcam);

void PutCameraMotion3ds(database3ds *db, kfcamera3ds *kfcam);


void ReleaseCameraMotion3ds(kfcamera3ds **cam);

ulong3ds GetCameraNodeCount3ds(database3ds *db);

void GetCameraNodeNameList3ds(database3ds *db, namelist3ds **list);

void DeleteCameraMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteCameraMotionByName3ds(database3ds *db, char3ds *name);
void CopyCameraMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyCameraMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);

/* End Public */

void GetCameraMotion3ds(chunk3ds *pCamChunk, chunk3ds *pTargetChunk, 
			kfcamera3ds **kfcam);



#endif

