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

#ifndef OBJM_3DS_H
#define OBJM_3DS_H

#include "3dstype.h"
#include "3dsprim.h"
#include "dbase3ds.h"
#include "xdata.h"
#include "chunk3ds.h"

/* Used By ObjectMotion3ds */

#define NoMorphSmooth3ds 0
#define MorphSmooth3ds 0xb015

/* Used in the visable field of the kfmesh3ds structure */
#define ShowObjMotion3ds   0
#define HideObjMotion3ds   (1<<11)
#define FastObjMotion3ds   (1<<12)
#define FreezeObjMotion3ds (1<<2)

/* Public Type Definitions */
#define NoParent3ds -1

/* Public Consts */
static const char3ds *DummyName3ds = "$$$DUMMY";
/* End Public */

/* Public Typedefs */
/* Used By ObjectMotion3ds */

typedef struct {
   char3ds name[11];     /* Name of mesh */
   char3ds parent[22];   /* Name of parent object */
   ushort3ds flags1;     /* flags field from node header  */
   ushort3ds flags2;     /* flags2 field from node header  */

   point3ds pivot;       /* Object pivot point */
   char3ds instance[11]; /* Object instance name */
   point3ds boundmin;    /* Minimum bounding box point for dummy objects */
   point3ds boundmax;    /* Maximum bounding box point for dummy objects */

   ulong3ds npkeys;      /* Number of position keys */
   short3ds npflag;      /* Loop control flag for position keys */
   keyheader3ds *pkeys;  /* Spline values for position keys */
   point3ds *pos;        /* Mesh position keys */

   ulong3ds nrkeys;      /* Number of rotation keys */
   short3ds nrflag;      /* Loop control flag for rotation keys */
   keyheader3ds *rkeys;  /* Spline values for rotation keys */
   kfrotkey3ds *rot;     /* Rotation keys */

   ulong3ds nskeys;      /* Number of scaling keys */
   short3ds nsflag;      /* Loop control flag for scaling keys */
   keyheader3ds *skeys;  /* Spline values for scaling */
   point3ds *scale;      /* Mesh scaling keys */

   ulong3ds nmkeys;      /* Number of morph keys */
   short3ds nmflag;      /* Loop control flag for morph keys */
   keyheader3ds *mkeys;  /* Spline values for morph keys */
   kfmorphkey3ds *morph; /* Morph keys */

   ulong3ds nhkeys;      /* Number of hide keys */
   short3ds nhflag;      /* Loop control flag for hide keys */
   keyheader3ds *hkeys;  /* Spline values for hide keys */
   float3ds msangle;     /* Morph smoothing group angle */
} kfmesh3ds;
/* End Public */

/* Public Prototypes */
void InitObjectMotion3ds(kfmesh3ds **obj, 
			 ulong3ds npkeys, 
			 ulong3ds nrkeys, 
			 ulong3ds nskeys, 
			 ulong3ds nmkeys,
			 ulong3ds nhkeys);

void ReleaseObjectMotion3ds(kfmesh3ds **obj);

void GetObjectNodeNameList3ds(database3ds *db, 
			      namelist3ds **list);

ulong3ds GetObjectNodeCount3ds(database3ds *db);

void GetObjectMotionByName3ds(database3ds *db, 
			      char3ds *name, 
			      kfmesh3ds **kfmesh);

void GetObjectMotionByIndex3ds(database3ds *db, 
			       ulong3ds index,
			       kfmesh3ds **kfmesh);

void PutObjectMotion3ds(database3ds *db,
			kfmesh3ds *kfmesh);

void DeleteObjectMotionByIndex3ds(database3ds *db, ulong3ds index);
void DeleteObjectMotionByName3ds(database3ds *db, char3ds *name);
void CopyObjectMotionByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyObjectMotionByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
/* End Public */

void GetObjectMotion3ds(chunk3ds *MeshChunk, 
			kfmesh3ds **kfmesh);



#endif

