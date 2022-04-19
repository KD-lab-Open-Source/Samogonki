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

/* Module Overview:

   3dslite.h and 3dslite.c concern themselves with the handling of spotlights
   and omnilights inside the 3ds mesh section.
   
*/      

#include "3dstype.h"
#include "3dsprim.h"
#include "3dsmset.h"

/* Public Typedefs */

/* conestyle3ds - Spolight projection cone shape. */
typedef enum
{
   Circular,
   Rectangular
} conestyle3ds;

/* spotshadow - Spotlight shadow settings */
typedef struct {
   byte3ds cast; /* True if spotlight casts shadows */
   shadowstyle3ds type; /* UseShadow or UseRayTrace */
   byte3ds local; /* True if local shadow settings are being used */
   float3ds bias; /* Shadow bias */
   float3ds filter; /* Shadow filter */
   ushort3ds mapsize; /* Shadow map size */
   float3ds raybias; /* Ray tracing shadow bias */
} spotshadow3ds; 

/* spotcond3ds - cone visability settings */
typedef struct {
   conestyle3ds type; /* Circular or rectangular light cone */
   byte3ds show; /* True if cone is visable */
   byte3ds overshoot;  /* True if cone overshoot is on */
} spotcone3ds;

/* spotproject3ds - spotlight projectio bitmap */
typedef struct {
   byte3ds use; /* True if using projector */
   char3ds *bitmap; /* Name of projector bitmap */
} spotprojector3ds;

/* spotlight3ds - spotlight settings */
typedef struct
{
   point3ds target;  /* Spotlight target */
   float3ds hotspot; /* Hotspot angle */
   float3ds falloff; /* Hotspot falloff */
   float3ds roll;    /* Roll angle */
   float3ds aspect;  /* Aspect ratio */
   spotshadow3ds shadows; 
   spotcone3ds cone;
   spotprojector3ds projector;
} spotlight3ds;

/* liteattenuate3ds - Light attenuation settings */
typedef struct {
   byte3ds on; /* True if light attenuation is on */
   float3ds inner; /* Inner range of attenuation */
   float3ds outer; /* Outer range of attenuation */ 
} liteattenuate3ds;

/* light3ds - omni and spotlight settings */
typedef struct {
   char3ds name[11];   /* Light name */
   point3ds pos; /* Light position */
   fcolor3ds color; /* Light color */
   float3ds multiplier;/* Light intensity multiplier */
   byte3ds dloff; /* True if light is off */
   liteattenuate3ds attenuation;
   namelist3ds *exclude;
   spotlight3ds *spot; /* If not null, then struct is a spotlight, else omni. */
} light3ds;
/* End Public */

/* Public Prototypes */

/* Light related functions */
void InitLight3ds(light3ds **light);
void InitSpotlight3ds(light3ds **spotlight);
void ReleaseLight3ds(light3ds **light);
ulong3ds GetOmnilightCount3ds(database3ds *db);
ulong3ds GetSpotlightCount3ds(database3ds *db);
void GetOmnilightNameList3ds(database3ds *db, namelist3ds **list);
void GetSpotlightNameList3ds(database3ds *db, namelist3ds **list);
void GetSpotlightByIndex3ds(database3ds *db, long3ds index, light3ds **light);
void GetOmnilightByIndex3ds(database3ds *db, long3ds index, light3ds **light);
void GetSpotlightByName3ds(database3ds *db, char3ds *name, light3ds **light);
void GetOmnilightByName3ds(database3ds *db, char3ds *name, light3ds **light);
void PutSpotlight3ds(database3ds *db, light3ds *light);
void PutOmnilight3ds(database3ds *db, light3ds *light);
void CopySpotlightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopySpotlightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void CopyOmnilightByName3ds(database3ds *destdb, database3ds *srcdb, char3ds *name);
void CopyOmnilightByIndex3ds(database3ds *destdb, database3ds *srcdb, ulong3ds index);
void DeleteSpotlightByIndex3ds(database3ds *db, ulong3ds index);
void DeleteOmnilightByIndex3ds(database3ds *db, ulong3ds index);
void DeleteSpotlightByName3ds(database3ds *db, char3ds *name);
void DeleteOmnilightByName3ds(database3ds *db, char3ds *name);
/* End Public */

void GetLightEntryChunk3ds(chunk3ds *chunk, light3ds **light);

void PutLight3ds(database3ds *db, light3ds *light);

