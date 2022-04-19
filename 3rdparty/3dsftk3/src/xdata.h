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

#ifndef XDATA_H
#define XDATA_H

#include "3dstype.h"
#include "dbase3ds.h"

/* Public Typedefs */
typedef struct {
   long3ds size;
   void3ds *data;
} xdataraw3ds;

/* End Public */

typedef enum
{
   XDataEntry3ds = 0x8001,
   XDataAppName3ds = 0x8002,
   XDataString3ds = 0x8003,
   XDataFloat3ds = 0x8004,
   XDataDouble3ds = 0x8005, 
   XDataShort3ds = 0x8006,
   XDataLong3ds = 0x8007, 
   XDataVoid3ds = 0x8008,
   XDataGroup3ds = 0x8009
   
} xdatatag3ds;

typedef struct xdata3ds
{
   xdatatag3ds xtag;
   union {
      struct { /* Used when the entry contains recognizable sub chunks */
         long3ds items;
         struct xdata3ds *list;
      } entry;
      char3ds *name; /* Appname field */
      char3ds *string; /* XDATA_STRING chunk */
      float3ds f; /* XDATA_FLOAT chunk */
      double3ds d; /* XDATA_DOUBLE chunk */
      short3ds s; /* XDATA_SHORT chunk */
      long3ds l;  /* XDATA_LONG chunk */
      struct { /* XDATA_VOID chunk */
         long3ds size;
         void3ds *v;
      } v;
      struct { /* XDATA_GROUP chunk */
         long3ds items;
         struct xdata3ds *list;
      } group;
      struct { /* XDATA_ENTRY with no recognizable sub chunks */
         long3ds size;
         void3ds *data;
      } rawentry;
   } data;
} xdata3ds;

/* Public Prototypes */
void GetMeshXDataByName3ds(database3ds *db, char3ds *meshname, char3ds *appname, xdataraw3ds **data);
ulong3ds GetMeshXDataCount3ds(database3ds *db, char3ds *meshname);
/* End Public */
ulong3ds GetXDataCount3ds(chunk3ds *chunk);
void GetXDataNameList3ds(chunk3ds *chunk, namelist3ds **list);
void FindXDataEntryByName3ds(chunk3ds *chunk, char3ds *appname, chunk3ds **xentry);
void AddXData3ds(chunk3ds *chunk, xdata3ds *data);
void AddXDataEntry3ds(chunk3ds *chunk, char3ds *appname, xdata3ds *data);
void AddAmbientLightNodeXData3ds(database3ds *db, char3ds *appname, xdata3ds *data);
void AddCameraNodeXData3ds(database3ds *db, char3ds *camname, char3ds *appname, xdata3ds *data);
void PutCameraXData3ds(database3ds *db, char3ds *camname, char3ds *appname, xdata3ds *data);
void PutMaterialXData3ds(database3ds *db, char3ds *matname, char3ds *appname, xdata3ds *data);
void PutMeshXData3ds(database3ds *db, char3ds *meshname, char3ds *appname, xdata3ds *data);
void AddObjectNodeXData3ds(database3ds *db, char3ds *name, char3ds *appname, xdata3ds *data);
void AddOmnilightNodeXData3ds(database3ds *db, char3ds *name, char3ds *appname, xdata3ds *data);
void AddSpotlightNodeXData3ds(database3ds *db, char3ds *name, char3ds *appname, xdata3ds *data);
void AddKfSectionXData3ds(database3ds *db, char3ds *appname, xdata3ds *data);
void PutMeshSectionXData3ds(database3ds *db, char3ds *appname, xdata3ds *data);



#endif
