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

/* Header file for 3DS File Toolkit: Database Structures */

#ifndef FTK_3DS_H
#define FTK_3DS_H

#include "3dstype.h"
#include "3dsprim.h"

typedef struct {
   ulong3ds version;
} M3dVersion;
   
typedef struct {
   float3ds  red;
   float3ds  green;
   float3ds  blue;
} ColorF, LinColorF;

typedef struct {
   ubyte3ds  red;
   ubyte3ds  green;
   ubyte3ds  blue;
} Color24, LinColor24, MatMapRCol, MatMapGCol, MatMapBCol, MatMapCol1, MatMapCol2;
      
typedef struct {
   short3ds  intpercentage;
} IntPercentage, MatBumpPercent;

typedef struct {
   float3ds floatpercentage;
} FloatPercentage;

typedef struct {
   char3ds  *name;
} MatMapname;

typedef struct {
   long3ds version;
} MeshVersion;

typedef struct {
   float3ds masterscale;
} MasterScale;

typedef struct {
   float3ds bias;
} LoShadowBias, HiShadowBias, RayBias;

typedef struct {
   short3ds shadowmapsize;
} ShadowMapSize;

typedef struct {
   short3ds shadowsamples;
} ShadowSamples;

typedef struct {
   long3ds shadowrange;
} ShadowRange;

typedef struct {
   float3ds shadowfilter;
} ShadowFilter;

typedef struct {
   point3ds oconsts;
} OConsts;

typedef struct {
   char3ds *bitmap;
} BitMap;

typedef struct {
   float3ds gradpercent;
} VGradient;

typedef struct {
   float3ds nearplanedist;
   float3ds nearplanedensity;
   float3ds farplanedist;
   float3ds farplanedensity;
} Fog;

typedef struct {
   float3ds zmin;
   float3ds zmax;
   float3ds density;
   ulong3ds type;
} LayerFog;

typedef struct {
   float3ds nearplanedist;
   float3ds nearplanedimming;
   float3ds farplanedist;
   float3ds farplanedimming;
} DistanceCue;

typedef struct {
   point3ds   viewtargetcoord;
   float3ds viewwidth;
} ViewStandard;

typedef struct {
   point3ds viewtargetcoord;
   float3ds viewwidth;
   float3ds xyviewangle;
   float3ds yzviewangle;
   float3ds bankangle;
} ViewUser;
typedef struct {
   char3ds *name;
} ViewCamera;

typedef struct {
   char3ds *name;
} MatName;

typedef struct {
   short3ds matshading;
} MatShading;

typedef struct {
   byte3ds shadelevel;
   byte3ds antialias;
   short3ds flags;
   ulong3ds mapsize;
   ulong3ds frameinterval;
} MatAcubic;

typedef struct {
   long3ds size;
   void *data;
} IpasData; 

typedef struct {
   float3ds wiresize;
} MatWireSize;

typedef struct {
   ushort3ds tiling;
} MatMapTiling;

typedef struct {
   float3ds percent;
} MatMapTexblur;

typedef struct {
   float3ds scale;
} MatMapUScale, MatMapVScale;

typedef struct {
   float3ds offset;
} MatMapUOffset, MatMapVOffset;

typedef struct {
   float3ds angle;
} MatMapAng;

typedef struct {
   char3ds *name;
} NamedObject;

typedef struct {
   ushort3ds vertices;
   point3ds *pointlist;
} PointArray;

typedef struct {
   ushort3ds flags;
   ushort3ds *flaglist;
} PointFlagArray;

typedef struct {
   ushort3ds faces;
   face3ds      *facelist;
} FaceArray;

typedef struct {
   char3ds *matname;
   ushort3ds faces;
   ushort3ds *facelist;
} MshMatGroup;

typedef struct 
{
   char3ds *matnames[6];
} MshBoxmap;

typedef struct {
   ushort3ds groups;
   ulong3ds *grouplist;
} SmoothGroup;

typedef struct {
   ushort3ds numcoords;
   textvert3ds   *textvertlist;
} TexVerts;

typedef struct {
   float3ds xmatrix[12];
} MeshMatrix;

typedef struct {
   ubyte3ds color;
} MeshColor;

typedef struct {
   ushort3ds  maptype;
   float3ds   xtiling;
   float3ds   ytiling;
   point3ds      iconpos;
   float3ds   iconscaling;
   float3ds   xmatrix[12];
   float3ds   iconwidth;
   float3ds   iconheight;
   float3ds   cyliconheight;
} MeshTextureInfo;

typedef struct {
   char3ds *name;
} ProcName;

typedef struct {
   point3ds lightpos;
} NDirectLight;

typedef struct {
   char3ds *name;
} DlExclude;

typedef struct {
   point3ds spotlighttarg;
   float3ds hotspotangle;
   float3ds falloffangle;
} DlSpotlight;

typedef struct {
   float3ds range;
} DlOuterRange, DlInnerRange;

typedef struct {
   float3ds multiple;
} DlMultiplier;

typedef struct {
   float3ds angle;
} DlSpotRoll;

typedef struct {
   float3ds aspect;
} DlSpotAspect;

typedef struct {
   char3ds *name;
} DlSpotProjector;

typedef struct {
   float3ds bias;
} DlRayBias;

typedef struct {
   float3ds localshadowbias;
   float3ds localshadowfilter;
   short3ds localshadowmapsize;
} DlLocalShadow2;

typedef struct {
   point3ds camerapos;
   point3ds targetpos;
   float3ds camerabank;
   float3ds camerafocallength;
} NCamera;

typedef struct {
   float3ds nearplane;
   float3ds farplane;
} CamRanges;

typedef struct {
   short3ds form; /* 0 = single window, 1 = 2 split verticle, 2 = 2 split horizontal, 3 = 4 equal squares,
		     4 = 2 squares left & 1 rect right, 5 = 1 rect at top & 2 sqr on bot, 6 = 1 rect left & 2 sqr right,
		     7 = 2 sqr top & 1 rect bot, 8 = 3 split vertical, 9 = 2 split horiz, 10 = 3 sqr left and 1 rect right,
		     11 = 1 rect left & 3 sqr. right.

		     form becomes 0 during swap and its preswapped value is stored in the swapport field */
   short3ds top; /* Active window index of 0 to 5 */
   short3ds ready; 
   short3ds wstate; /* 0 if no swap window, 1 if in swaped "w" state.  During a swap, the old 0 window gets stored as the 4 window */
   short3ds swapws;  
   short3ds swapport; /* The preswapped value from the form field */
   short3ds swapcur; /* The window index that was swapped */
} ViewportLayout;

typedef struct { /* Values given for 1024x768 resolution */
   ushort3ds xpos; /* 0 */
   ushort3ds ypos; /* 14 */
   ushort3ds width; /* 895 */
   ushort3ds height; /* 725 */
} ViewportSize;

typedef struct {
   ushort3ds flags;
   ushort3ds axislockout;
   ushort3ds winxpos;
   ushort3ds winypos;
   ushort3ds winwidth;
   ushort3ds winheight;
   ushort3ds view; /* 0 = No view, 1 = Top View, 2 = Bottom View, 3 = Left View, 4 = Right View, 5 = Front View, 6 = Back View, 7 = User View, 18 = Spotlight View, 65535 = Camera View */
   float3ds zoomfactor;
   point3ds center;
   float3ds horizang;
   float3ds vertang;
   char3ds *camname;
} ViewportData, ViewportData3;
   
typedef struct {
   short3ds   revision;
   char3ds    *filename;
   long3ds    animlength;
} KFHdr;

typedef struct {
  short3ds id;
} KFId;

typedef struct {
   long3ds  first;
   long3ds  last;
} KFSeg;

typedef struct {
   long3ds currframenum;
} KFCurtime;

typedef struct {
   char3ds *objname;
   ushort3ds flags1;
   ushort3ds flags2;
   short3ds  parentindex;
} NodeHdr;

typedef struct {
   point3ds   offset;
} Pivot;

typedef struct {
   char3ds *name;
} InstanceName;

typedef struct {
   float3ds   smoothgroupangle;
} MorphSmooth;

typedef struct {
   point3ds   min;
   point3ds   max;
} BoundBox;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   point3ds       *positionlist;
} PosTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   fcolor3ds      *colorlist;
} ColTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   kfrotkey3ds *rotationlist;
} RotTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   point3ds *scalelist;
} ScaleTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   kfmorphkey3ds  *morphlist;
} MorphTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
} HideTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   float3ds    *fovanglelist;
} FovTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   float3ds    *rollanglelist;
} RollTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   float3ds    *hotspotanglelist;
} HotTrackTag;

typedef struct {
   trackheader3ds trackhdr;
   keyheader3ds   *keyhdrlist;
   float3ds       *falloffanglelist;
} FallTrackTag;

typedef struct {
   long3ds size;
   void3ds *data;
} XDataEntry;

typedef struct {
   char3ds *name;
} XDataAppName;

typedef struct {
   char3ds *string;
} XDataString;

typedef struct {
   float3ds value;
} XDataFloat;

typedef struct {
   double3ds value;
} XDataDouble;

typedef struct {
   short3ds value;
} XDataShort;

typedef struct {
   long3ds value;
} XDataLong;

typedef struct {
   void *data;
} XDataVoid;

#endif

