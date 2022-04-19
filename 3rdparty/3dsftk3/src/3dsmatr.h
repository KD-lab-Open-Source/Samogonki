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

/* 3dsmatr.c, 3dsmatr.h: performs Material chunk related operations
   using the material3ds structure */

#ifndef MATR_3DS_H
#define MATR_3DS_H

#include "3dstype.h"
#include "3dsprim.h"
#include "dbase3ds.h"
#include "xdata.h"

/* Public Typedefs */
/* Used for shading field in material3ds structure */
typedef enum
{
   Wire = 0,
   Flat = 1,
   Gouraud = 2,
   Phong = 3,
   Metal = 4
} shadetype3ds;

/* Used for tiling field in bitmap3ds structure */
typedef enum 
{
   Tile = 1,
   Decal = 2,
   Both = 3
} tiletype3ds;

typedef enum
{
   Pyramidal,
   SummedArea
} filtertype3ds;

typedef enum
{
   RGB,
   Alpha,
   RGBLumaTint,
   AlphaTint,
   RGBTint
} tinttype3ds;

/* End Public */



/* For the shading field */
static const ushort3ds WireShading3ds    = 0;
static const ushort3ds FlatShading3ds    = 1;
static const ushort3ds GouraudShading3ds = 2;
static const ushort3ds PhongShading3ds   = 3;
static const ushort3ds MetalShading3ds   = 4;

/* For the MAT_TILING chunk */
static const ushort3ds TEX_DECAL  = 1;
static const ushort3ds TEX_MIRROR  = 1<<1;
static const ushort3ds TEX_UNUSED1  = 1<<2;
static const ushort3ds TEX_INVERT  = 1<<3;
static const ushort3ds TEX_NOWRAP  = 1<<4;
static const ushort3ds TEX_SAT  = 1<<5;	 /* summed area table */
static const ushort3ds TEX_ALPHA_SOURCE  = 1<<6; /* use ALPHA instead of RGB of map */
static const ushort3ds TEX_TINT  = 1<<7;         /* tint for color */
static const ushort3ds TEX_DONT_USE_ALPHA  = 1<<8;    /* don't use map alpha */
static const ushort3ds TEX_RGB_TINT  = 1<<9;    /* Do RGB color transform */

/* Used with acubic3ds structure */

static const ushort3ds ACubicFirst3ds = 0x09;
static const ushort3ds ACubicFlat3ds  = 0x11;

/* Public Typedefs */

/* Used By AddMaterial3ds */

typedef struct {
   ubyte3ds firstframe; /* True for First Frame Only */
   ubyte3ds flat; /* True for flat mirror reflection */
   ulong3ds size;     /* Map resolution */
   ulong3ds nthframe; /* Map update period */
} acubic3ds;          /* Cubic reflection map defintion */

typedef struct {
   char3ds  name[13];  /* Bitmap file name */
   float3ds percent;  /* Strength percentage */
   tiletype3ds tiling;  /* Tile/Decal/Both */
   byte3ds ignorealpha;
   filtertype3ds filter; /* Pyramidal/Summed Area */
   float3ds blur;
   byte3ds mirror;
   byte3ds negative;
   float3ds uscale, vscale;
   float3ds uoffset, voffset;
   float3ds rotation;
   tinttype3ds source; /* RGB/RGB Luma Tint/Alpha Tint/RGB Tint */
   fcolor3ds tint1;
   fcolor3ds tint2;
   fcolor3ds redtint;
   fcolor3ds greentint;
   fcolor3ds bluetint;
   ulong3ds datasize; /* Size of procedural data */
   void3ds *data;     /* Procedural data */
} bitmap3ds;          /* Bit map definition */

/* Structure to all map settings */
typedef struct {
   bitmap3ds map; /* The map settings */
   bitmap3ds mask; /* The mask settings */
} mapset3ds;

typedef struct {
   bitmap3ds map; /* The map settings */
   byte3ds useauto; /* True3ds if automatic reflections are being used */
   acubic3ds automap; /* Automatic reflection definitions */
   bitmap3ds mask; /* The mask settings */
} rmapset3ds;
   
typedef struct {
   char3ds name[17];      /* Name */
   fcolor3ds ambient;     /* Ambient light color */
   fcolor3ds diffuse;     /* Diffuse light color */
   fcolor3ds specular;    /* Specular light color */
   float3ds shininess;    /* Shininess factor */
   float3ds shinstrength; /* Shininess strength */
   float3ds blur;         /* Blur factor */
   float3ds transparency; /* Trasparency factor */
   float3ds transfalloff; /* Fallof factor */
   float3ds selfillumpct;     /* Self illumination percentage */
   float3ds wiresize;     /* Width of wireframe */
   shadetype3ds shading;      /* Shading type */
   byte3ds useblur;       /* Blurring flag */
   byte3ds usefall;       /* Transparency falloff flag */
   byte3ds twosided;      /* Two sided material flag */
   byte3ds selfillum;     /* Self illumination flag */
   byte3ds additive;      /* Additive transparency flag */
   byte3ds usewire;       /* Use wireframe rendering */
   byte3ds usewireabs;    /* Wire size is in units, not pixels. */
   byte3ds facemap;       /* Face mapping switch */
   byte3ds soften;        /* Soften switch */
   mapset3ds texture;     /* Texture map settings */
   mapset3ds texture2;    /* Second texture map settings */
   mapset3ds opacity;     /* Opacity map settings */
   mapset3ds bump;       /* Bump map settings */
   mapset3ds specmap;     /* Specularity map settings */
   mapset3ds shinmap;     /* Shininess map settings */
   mapset3ds illummap;    /* Self illumination map settings */
   rmapset3ds reflect;    /* Reflection map settings */
} material3ds;            /* Material definition */
/* End Public */

/* Public Prototypes */
short3ds AddMaterial3ds(const material3ds *mat);
void InitBitmap3ds(bitmap3ds *map);
short3ds InitMaterial3ds(material3ds **mat);
void ReleaseMaterial3ds(material3ds **mat);
ulong3ds GetMaterialCount3ds(database3ds *db);
void GetMaterialNameList3ds(database3ds *db, namelist3ds **list);
void GetMaterialByIndex3ds(database3ds *db, ulong3ds index, material3ds **mat);
void GetMaterialByName3ds(database3ds *db, char3ds *name, material3ds **mat);
void PutMaterial3ds(database3ds *db, material3ds *mat);
void CopyMaterialByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyMaterialByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteMaterialByName3ds(database3ds *db, char3ds *name);
void DeleteMaterialByIndex3ds(database3ds *db, ulong3ds index);
/* End Public */
void ReadMatEntryChunk3ds(chunk3ds *chunk, material3ds **mat);

#endif

