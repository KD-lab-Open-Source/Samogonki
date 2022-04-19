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

#ifndef PRIM_3DS_H
#define PRIM_3DS_H

#include "3dstype.h" 
#include "chunkinf.h" 

/* Public Defines */
/* 3DS File Toolkit revision level.  Future revisions will increment this
   integer number */
#define FTKRevisionLevel3ds 2

/* End Public */


/* Public Typedefs */

/*----------------------------------------
  Commonly used structures and constants
  --------------------------------------*/

/* Constants to define True and False in a compiler independant way. */
#define TRUE3DS (1==1)
#define FALSE3DS (1!=1)
static const byte3ds True3ds = (1==1);
static const byte3ds False3ds = (1!=1);

/* point3ds - 3D point structure */
typedef struct 
{
   float3ds x, y, z;
} point3ds;

/* fcolor3ds - Floating point color definition */
typedef struct {
   float3ds r;   /* RGB Color components */
   float3ds g;
   float3ds b;
} fcolor3ds;       

/* face3ds - Face list element */
typedef struct
{
   ushort3ds v1, v2, v3, flag;
} face3ds;

/* textvert3ds - Texture assignment coordinate */
typedef struct
{
   float3ds u, v;
} textvert3ds;
/* End Public */

/* Private Typedefs */
/* trackheader3ds - Global track settings */
typedef struct
{
   ushort3ds flags;
   ulong3ds nu1, nu2;
   ulong3ds keycount; /* Number of keys in the track */
} trackheader3ds;
/* End Private */

/* Public Typedefs */

/* keyheader3ds - Animation key settings */
typedef struct
{
   ulong3ds time;       /* Key's frame position */
   ushort3ds rflags;    /* Spline terms used flag */
   float3ds tension;    /* Flagged with 0x01 */
   float3ds continuity; /* Flagged with 0x02 */
   float3ds bias;       /* Flagged with 0x04 */
   float3ds easeto;     /* Flagged with 0x08 */
   float3ds easefrom;   /* Flagged with 0x10 */
} keyheader3ds;

/* kfrotkey3ds - Rotation key */
typedef struct {
   float3ds angle; /* angle of rotation */
   float3ds x;     /* rotation axis vector */
   float3ds y;
   float3ds z;
} kfrotkey3ds;

/* kfmorphkey3ds - Object morph key */
typedef struct {
   char3ds name[13]; /* name of target morph object */
} kfmorphkey3ds;

/* End Public */

/* Public Consts */

/*----------------------------------------
  Constants for commonly used structures
  --------------------------------------*/

/* 3DS File Toolkit revision level.  Future revisions will increment this
   integer number */
static const long3ds __FTKRevisionLevel3ds = FTKRevisionLevel3ds; 

/* Flags used by the flag field of the mesh3ds structure */
static const ushort3ds FaceCAVisable3ds = 0x0001;  /* Flags the CA edge as visable */
static const ushort3ds FaceBCVisable3ds = 0x0002;  /* Flags the BC edge as visable */
static const ushort3ds FaceABVisable3ds = 0x0004;  /* Flags the AB edge as visable */
static const ushort3ds FaceUWrap3ds     = 0x0008;  /* Flags the face as being at
						      a texture coord u wrap
						      seam */
static const ushort3ds FaceVWrap3ds     = 0x0010;  /* Flags the face as being at
						      a texture coord v wrap
						      seam */

/* Flags used by the rflags field of the keyheader3ds structure */
static const ushort3ds KeyUsesTension3ds = 0x01;
static const ushort3ds KeyUsesCont3ds    = 0x02;
static const ushort3ds KeyUsesBias3ds    = 0x04;
static const ushort3ds KeyUsesEaseTo3ds  = 0x08;
static const ushort3ds KeyUsesEaseFrom3ds= 0x10;

/* Flags used by the track flags field */
static const ushort3ds TrackSingle3ds  = 0x0000;
static const ushort3ds TrackLoops3ds   = 0x0003;
static const ushort3ds TrackRepeats3ds = 0x0002;
static const ushort3ds TrackLockX3ds   = 0x0008;
static const ushort3ds TrackLockY3ds   = 0x0010;
static const ushort3ds TrackLockZ3ds   = 0x0020;
static const ushort3ds TrackNoLinkX3ds = 0x0100;
static const ushort3ds TrackNoLinkY3ds = 0x0200;
static const ushort3ds TrackNoLinkZ3ds = 0x0400;

/* Basic structure default contents for ease of initialization */
static const point3ds DefPoint3ds = {0.0F, 0.0F, 0.0F};
static const textvert3ds DefTextVert3ds = {0.0F, 0.0F};
static const face3ds DefFace3ds = {0, 1, 2, 0};
static const trackheader3ds DefTrackHeader3ds = {0, 0, 0, 1};
static const keyheader3ds DefKeyHeader3ds = {0, 0, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
/* End Public */

static const int ByteSize3ds = 1;
static const int ShortSize3ds = 2;
static const int LongSize3ds = 4;
static const int FloatSize3ds = 4;
static const int DoubleSize3ds = 8;
static const int PointSize3ds = 12;
static const int TextVertSize3ds = 8;
static const int FaceSize3ds = 8;
static const int HeaderSize3ds = 6;
static const int TrackHeaderSize3ds = 14;

void WriteByte3ds(byte3ds x);
byte3ds ReadByte3ds();
void WriteUByte3ds(ubyte3ds x);
ubyte3ds ReadUByte3ds();
void WriteShort3ds(short3ds x);
short3ds ReadShort3ds();
void WriteUShort3ds(ushort3ds x);
ushort3ds ReadUShort3ds();
void WriteLong3ds(long3ds x);
long3ds ReadLong3ds();
void WriteULong3ds(ulong3ds x);
ulong3ds ReadULong3ds();
void WriteFloat3ds(float3ds x);
float3ds ReadFloat3ds();
void WriteDouble3ds(double3ds x);
double3ds ReadDouble3ds();
void WriteVoid3ds(ulong3ds size, const void *data);
void ReadVoid3ds(ulong3ds size, void *data);
void SkipVoid3ds(ulong3ds size);
void WriteString3ds(const char3ds *s);
void WriteFixedString3ds(const char3ds *s, ulong3ds length);
void ReadString3ds(char3ds *s, ulong3ds length);
void ReadAndAllocString3ds(char3ds **s, ulong3ds length);
void ReadAndAllocFixedString3ds(char3ds **s, ulong3ds length);
void ReadFixedString3ds(char3ds *s, ulong3ds length);
void WriteHeader3ds(chunktag3ds chunktype, ulong3ds size);
void ReadHeader3ds(chunktag3ds *chunktype, ulong3ds *size);
void FinishHeader3ds(ulong3ds startpos, ulong3ds endpos);
void WritePoint3ds(const point3ds *p);
void ReadPoint3ds(point3ds *p);
void WriteTextVert3ds(const textvert3ds *t);
void ReadTextVert3ds(textvert3ds *t);
void WriteFace3ds(const face3ds *f);
void ReadFace3ds(face3ds *f);
void WriteTrackHeader3ds(const trackheader3ds *t);
void ReadTrackHeader3ds(trackheader3ds *t);
void WriteKeyHeader3ds(const keyheader3ds *k);
void ReadKeyHeader3ds(keyheader3ds *k);


#endif

