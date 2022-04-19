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

/* 3dsaset.c, 3dsaset.h

   Inits, frees, puts and gets the atmosphere3ds structure which provides
   and interface to the 3ds atmospheric effects depth cue, fog, and
   layer fog (DISTANCE_CUE, FOG, and LAYER_FOG chunks.
*/

#ifndef ASET_3DS_H
#define ASET_3DS_H

#include "3dstype.h"
#include "3dsprim.h"

/* Public Consts */
/* Fog flags */
static const ulong3ds LayerFogBgnd = 0x100000;
static const ulong3ds NoFalloff = 0x0;
static const ulong3ds TopFalloff = 0x2;
static const ulong3ds BottomFalloff = 0x1;

/* End Public */

/* Public Typedefs */
typedef enum
{
   NoAtmo, /* no active astmospherics */
   UseFog, /* fog astmostpheric */
   UseLayerFog, /* layer fog astmostpheric */
   UseDistanceCue /* distance cue astmostpheric */
} atmospheretype3ds;

typedef enum
{
   NoFall, /* no falloff */
   TopFall, /* falloff to the top */
   BottomFall /* falloff to the bottom */
} layerfogfalloff3ds;

typedef struct /* fog atmosphere parameters */
{
   float3ds nearplane; /* near radius of fog effect */
   float3ds neardensity; /* near fog density */
   float3ds farplane; /* far radius of fog effect */
   float3ds fardensity; /* far fog density */
   fcolor3ds     fogcolor; /* color of fog effect */
   byte3ds    fogbgnd; /* True3ds / False3ds "fog background" flag */
} fogsettings3ds;

typedef struct /* layered fog atmosphere parameters */
{
   float3ds zmin; /* lower bounds of fog */
   float3ds zmax; /* upper bounds of fog */
   float3ds density; /* fog density */
   fcolor3ds fogcolor; /* fog color */
   layerfogfalloff3ds falloff; /* falloff style */
   byte3ds fogbgnd; /* True3ds / False3ds "fog background" flag */
} layerfogsettings3ds;

typedef struct /* distance cue atmosphere parameters */
{
   float3ds nearplane; /* near radius of effect */
   float3ds neardim; /* near dimming factor */
   float3ds farplane; /* far radius of effect */
   float3ds fardim; /* far dimming factor */
   byte3ds dcuebgnd; /* True3ds / False3ds effect the background flag */
} dcuesettings3ds;

typedef struct {
   fogsettings3ds fog; /* fog atmostphere settings */
   layerfogsettings3ds layerfog;  /* layered fog atmosphere parameters */
   dcuesettings3ds dcue;  /* distance cue atmosphere parameters */
   atmospheretype3ds activeatmo; /* The active astmospheric */
} atmosphere3ds;

/* End Public */

/* Public Prototypes */
void InitAtmosphere3ds(atmosphere3ds **atmo);
void ReleaseAtmosphere3ds(atmosphere3ds **atmo);
void GetAtmosphere3ds(database3ds *db, atmosphere3ds **atmo);
void PutAtmosphere3ds(database3ds *db, atmosphere3ds *atmo);
void CopyAtmosphere3ds(database3ds *destdb, database3ds *srcdb);

/* End Public */




#endif


