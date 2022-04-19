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

/* a list of constants that define various value ranges for specific chunks */

#ifndef RANGE_3DS_H
#define RANGE_3DS_H


/* 3DS filename */
#define FileNameStrMax3ds 512

/* 3DS file attributes */
#define FileAttrStrMax3ds 128

/* MASTER_SCALE chunk */
#define MasterScaleMin3ds 0.0  /* Noninclusive minimum value for master scale */

/* LO_SHADOW_BIAS chunk */
#define LoShadowBiasMin3ds 0.0 /* Noninclusive minimum value for low shadow bias setting */

/* HI_SHADOW_BIAS chunk */
#define HiShadowBiasMin3ds 0.0 /* Noninclusive minimum value for high shadow bias setting */

/* SHADOW_MAP_SIZE chunk */
#define ShadowMapSizeMin3ds 0 /* Noninclusive minimum value for shadow map size */

/* SHADOW_SAMPLES chunk */
#define ShadowSamplesMin3ds 0 /* Noninclusive minimum value for shadow samples */

/* SHADOW_RANGE chunk */
#define ShadowRangeMin3ds 0 /* Noninclusive minimum value for shadow range */

/* SHADOW_FILTER chunk */
#define ShadowFilterMin3ds 1.0 /* Inclusive minimum value for shadow filter */
#define ShadowFilterMax3ds 10.0 /* Inclusive maximum value for shadow filter */

/* BITMAP chunk */
#define BitMapStrMax3ds 12      /* maximum string length for filename */

/* V_GRADIENT chunk */
#define VGradientMin3ds 0.0    /* minimum value for gradient midpoint */
#define VGradientMax3ds 1.0    /* maximum value for gradient midpoint */

/* FOG chunk */
#define FogMin3ds 0.0 /* minimum value for fogging plane density */
#define FogMax3ds 1.0 /* maximum value for fogging plane density */

/* DISTANCE_CUE */
#define DistanceCueMin3ds 0.0    /* minimum value for dimming factor */
#define DistanceCueMax3ds 1.0    /* maximum value for dimming factor */

/* VIEW_CAMERA */
#define ViewCameraStrMax3ds 10  /* maximum string length for filename */

/* MAT_NAME */
#define MatNameStrMax3ds 16 /* maximum string length for material name */

/* MAT_SHADING */
#define MatShadingMin3ds 0 /* minimum shading value */
#define MatShadingMax3ds 3 /* maximum shading value */

/* MAT_ACUBIC_FMIN */
#define MatACubicFMin3ds 1 /* minimum frame skip count */
#define MatACubicAMin3ds 0 /* minimum reflection map aliasing */
#define MatACubicAMax3ds 3 /* maximum reflection map aliasing */

/* POINT_ARRAY */
#define PointArrayMin3ds 3 /* minimum number of vertices */

/* FACE_ARRAY */
#define FaceArrayMin3ds 1 /* minimum number of faces */

/* MshMatGroup3ds */
#define MshMatGroupMin3ds 1 /* minimum number of faces per material */
#define MshMatGroupStrMax3ds 16 /* maximim string length for MshMatGroup */

/* PROC_NAME */
#define ProcNameStrMax3ds 12 /* maximum string length for axp process */

/* DL_SPOTLIGHT */
#define DLSpotlightMin3ds 0.0 /* minimum for hotspot and falloff cones */
#define DLSpotlightMax3ds 160.0 /* maximum for hotspot and falloff cones */

/* DL_LOCAL_SHADOW2 */
#define DLLocalShadow2SMin3ds 10 /* minimum shadow map size */
#define DLLocalShadow2SMax3ds 4096 /* maximum shadow map size */
#define DLLocalShadow2FMin3ds 1.0 /* minimum shadow map size */
#define DLLocalShadow2FMax3ds 10.0 /* maximum shadow map size */

/* COLOR_F */
#define ColorFMin3ds 0.0 /* minimum color value in a channel */
#define ColorFMax3ds 1.0 /* maximum color value in a channel */

/* INT_PERCENTAGE */
#define IntPercentageMax3ds 100 /* Maximum integer percentage */

/* FLOAT_PERCENTAGE */
#define FloatPercentageMax3ds 1.0 /* Maximum floating point percentage */

/* MAT_MAPNAME */
#define MatMapNameStrMax3ds 12 /* Maximum map name string size */

/* NAMED_OBJECT */
#define NamedObjectStrMax3ds 10 /* Maximum named object string size */

/* N_CAMERA */
#define NCameraFOVMin3ds 0.00025  /* Minimum field of view for camera */
#define NCameraFOVMax3ds 160.0    /* Maximum field of view for camera */
#define NCameraFocMin3ds 10.7813  /* Minimum lens size for camera */
#define NCameraFocMax3ds 10000000 /* Maximum lens size for camera */

/* KFHDR */
#define KFHdrStrMax3ds 12 /* Maximum keyframe header name string size */

/* NODE_HDR */
#define NodeHdrStrMax3ds 10 /* Maximum node name string size */

/* INSTANCE_NAME */
#define InstanceNameStrMax3ds 10 /* Maximum instance name string size */

/* MORPH_TRACK */
#define MorphTrackStrMax3ds 10 /* Maximum morph object name string size */

/* MORPH_SMOOTH */
#define MorphSmoothMin3ds 0.0 /* Minimum morph smoothing angle */
#define MorphSmoothMax3ds 360.0 /* Maximum morph smoothing angle */

/* Keyframe Spline Limits */
#define KFTensionMin3ds -1.0 /* Minimum key spline tension */
#define KFTensionMax3ds 1.0 /* Maximum key spline tension */
#define KFContinuityMin3ds -1.0 /* Minimum key spline continuity */
#define KFContinuityMax3ds 1.0 /* Maximum key spline continuity */
#define KFBiasMin3ds -1.0 /* Minimum key spline bias */
#define KFBiasMax3ds 1.0 /* Maximum key spline bias */
#define KFEaseToMin3ds 0.0 /* Minimum key spline ease to */
#define KFEaseToMax3ds 1.0 /* Maximum key spline ease to */
#define KFEaseFromMin3ds 0.0 /* Minimum key spline ease from */
#define KFEaseFromMax3ds 1.0 /* Maximum key spline ease from */

/* Track header Limits */
#define TrackHeaderKeysMin3ds 1 /* Minimum number of keys in a track */

/* COL_TRACK_TAG_KEY */
#define ColTrackTagMin3ds 0.0 /* Minimum color value */
#define ColTrackTagMax3ds 1.0 /* Maximum color value */

/* FOV_TRACK_TAG_KEY */
#define FOVTrackTagMin3ds NCameraFOVMin3ds /* Minimum camera FOV */
#define FOVTrackTagMax3ds NCameraFOVMax3ds /* Maximum camera FOV */

/* HOT_TRACK_TAG_KEY */
#define HotTrackTagMin3ds 0.0 /* Minimum hot spot angle */
#define HotTrackTagMax3ds 160.0 /* Maximum hot spot angle */

/* FALL_TRACK_TAG_KEY */
#define FallTrackTagMin3ds 0.0 /* Minimum fall off angle */
#define FallTrackTagMax3ds 160.0 /* Maximum fall off angle */

/* End Public */

#endif
