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

#ifndef MOBJ_3DS_H
#define MOBJ_3DS_H

#include "3dstype.h"
#include "3dsprim.h"
#include "dbase3ds.h"

/* Public Consts */

/* Smoothing group flags used in the smootharray field of the mesh3ds structure */
static const ulong3ds Smooth01Group3ds = 0x00000001;
static const ulong3ds Smooth02Group3ds = 0x00000002;
static const ulong3ds Smooth03Group3ds = 0x00000004;
static const ulong3ds Smooth04Group3ds = 0x00000008;
static const ulong3ds Smooth05Group3ds = 0x00000010;
static const ulong3ds Smooth06Group3ds = 0x00000020;
static const ulong3ds Smooth07Group3ds = 0x00000030;
static const ulong3ds Smooth08Group3ds = 0x00000080;
static const ulong3ds Smooth09Group3ds = 0x00000100;
static const ulong3ds Smooth10Group3ds = 0x00000200;
static const ulong3ds Smooth11Group3ds = 0x00000400;
static const ulong3ds Smooth12Group3ds = 0x00000800;
static const ulong3ds Smooth13Group3ds = 0x00001000;
static const ulong3ds Smooth14Group3ds = 0x00002000;
static const ulong3ds Smooth15Group3ds = 0x00004000;
static const ulong3ds Smooth16Group3ds = 0x00008000;
static const ulong3ds Smooth17Group3ds = 0x00010000;
static const ulong3ds Smooth18Group3ds = 0x00020000;
static const ulong3ds Smooth19Group3ds = 0x00040000;
static const ulong3ds Smooth20Group3ds = 0x00080000;
static const ulong3ds Smooth21Group3ds = 0x00100000;
static const ulong3ds Smooth22Group3ds = 0x00200000;
static const ulong3ds Smooth23Group3ds = 0x00400000;
static const ulong3ds Smooth24Group3ds = 0x00800000;
static const ulong3ds Smooth25Group3ds = 0x01000000;
static const ulong3ds Smooth26Group3ds = 0x02000000;
static const ulong3ds Smooth27Group3ds = 0x04000000;
static const ulong3ds Smooth28Group3ds = 0x08000000;
static const ulong3ds Smooth29Group3ds = 0x10000000;
static const ulong3ds Smooth30Group3ds = 0x20000000;
static const ulong3ds Smooth31Group3ds = 0x40000000;
static const ulong3ds Smooth32Group3ds = 0x80000000;
/* End Public */

/* Public Typedefs */

/* mapinfo3ds - Texture map icon placement */
typedef struct {
   ushort3ds maptype;    /* Icon type */
   float3ds tilex;       /* Tiling */
   float3ds tiley;
   float3ds cenx;        /* Position of center */
   float3ds ceny;
   float3ds cenz;
   float3ds scale;       /* Icon scaling factor */
   float3ds matrix[12];  /* Orientation matrix */
   float3ds pw;          /* Planar icon width */
   float3ds ph;          /* Planar icon height */
   float3ds ch;          /* Cylinder icon height */
} mapinfo3ds;            

/* objmat3ds - Material assignments by face */
typedef struct {
   char3ds name[17];     /* Material name */
   ushort3ds nfaces;     /* Number of faces using material */
   ushort3ds *faceindex; /* List of faces using material */
} objmat3ds;             

/* mesh3ds - Mesh object definition */
typedef struct {
   char3ds name[11];       /* Object name */
   byte3ds ishidden;       /* Hidden object flag */
   byte3ds isvislofter;    /* Lofter visibility flag */
   byte3ds ismatte;        /* Matte object flag */
   byte3ds isnocast;       /* Doesn't cast shadow's flag */
   byte3ds isfast;         /* Fast display flag */
   byte3ds isnorcvshad;    /* Doesn't recieve shadows */
   byte3ds isfrozen;       /* Frozen object flag */
   ushort3ds nvertices;    /* Vertice count */
   point3ds *vertexarray;  /* List of vertices */
   ushort3ds nvflags;      /* Number of vertex flags */
   ushort3ds *vflagarray;  /* List of vertex flags */
   ushort3ds ntextverts;   /* Number of texture vertices */   
   textvert3ds *textarray; /* List of texture coordinates */
   byte3ds usemapinfo;     /* Boolean for use of mapping icon information */
   mapinfo3ds map;         /* Mapping icon info */
   float3ds locmatrix[12]; /* Object orientation matrix */
   ushort3ds nfaces;       /* Face count */
   face3ds *facearray;     /* List of faces */
   ulong3ds *smootharray;  /* Smoothing group assignment list */
   byte3ds useboxmap;     /* Boolean used to indicate the use of box mapping */
   char3ds boxmap[6][17];  /* Material names used in boxmapping */
   ubyte3ds meshcolor;     /* UI color assigned to the mesh */
   ushort3ds nmats;        /* Assigned materials count */
   objmat3ds *matarray;    /* Material assignment list */
   byte3ds useproc;        /* Use animated stand-in flag */
   ulong3ds procsize;      /* Size of animated stand-in data */
   char3ds procname[13];   /* Name of animated stand-in procedure */
   void3ds *procdata;      /* Animated stand-in data */
} mesh3ds;                  
/* End Public */

/* Public Prototypes */

/* Mesh object functions */
ulong3ds GetMeshCount3ds(database3ds *db);
void GetMeshNameList3ds(database3ds *db, namelist3ds **list);
void GetMeshByIndex3ds(database3ds *db, ulong3ds index, mesh3ds **mesh);
void GetMeshByName3ds(database3ds *db, char3ds *name, mesh3ds **mesh);
void PutMesh3ds(database3ds *db, mesh3ds *mesh);
void CopyMeshByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyMeshByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteMeshByName3ds(database3ds *db, char3ds *name);
void DeleteMeshByIndex3ds(database3ds *db, ulong3ds index);
/* End Public */

ulong3ds GetMeshMatCount3ds(chunk3ds *current);
void GetMeshEntryChunk3ds(chunk3ds *chunk, mesh3ds **mesh);

#endif

