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

#ifndef CHUNK_3DS_H
#define CHUNK_3DS_H

#include "3dstype.h"
#include "3dserr.h"
#include "chunkinf.h"

/* Private Typedefs */

/* chunk3ds - Internal database representation of file information */
typedef struct chunk3ds {
   chunktag3ds tag;           /* Type of chunk */
   ulong3ds     size;         /* Number of bytes used by chunk */
   ulong3ds     position;     /* Offset in source file */
   void        *data;         /* Memory copy of file data */
   struct chunk3ds *sibling;  /* Next chunk in database */
   struct chunk3ds *children; /* Chunks contained within this chunk */
   ubyte3ds readindex;        /* Index of source file in file table */
   ubyte3ds writeindex;       /* Index of destination file in file table */
} chunk3ds;

/* chunklistentry3ds - Cross reference between name and chunk */
typedef struct {
   char3ds *name;                  /* Chunk Name list */
   chunk3ds *chunk;                /* Corresponding Pos */
} chunklistentry3ds;

/* chunklist3ds - List of cross references */
typedef struct 
{
   ulong3ds count; /* Number of entries in list */
   chunklistentry3ds *list; /* Contents of list */
} chunklist3ds;

/* End Private */

/* Public Typedefs */

/* namelistentry3ds - Name of database object */
typedef struct 
{
   char3ds *name; /* Name of object */
} namelistentry3ds;

/* namelist3ds - List of database object names */
typedef struct
{
   ulong3ds count;          /*--- How many entries are in list */
   ulong3ds spaces;         /*--- How much space for entries */
   namelistentry3ds *list;  /*--- char ** to access pointers */
} namelist3ds;

/* End Public */

/* Public Prototypes */

/* Functions to use with namelist3ds structures */
void InitNameList3ds(namelist3ds **chunk, ulong3ds count);
void ReleaseNameList3ds(namelist3ds **chunk);
void AddToNameList3ds(namelist3ds **list, char3ds *name);
/* End Public */

void InitChunk3ds(chunk3ds **chunk);
void InitChunkAs3ds(chunk3ds **chunk, chunktag3ds tag);
void *InitChunkAndData3ds(chunk3ds **chunk, chunktag3ds tag);
void ReleaseChunk3ds(chunk3ds **chunk);
void ReleaseSiblingChunk3ds(chunk3ds **chunk);
void DeleteChunk3ds(chunk3ds *chunk);
void FindChunk3ds(chunk3ds *local, chunktag3ds tag, chunk3ds **chunk);
void FindNextChunk3ds(chunk3ds *local, chunktag3ds tag, chunk3ds **chunk);
void AddChild3ds(chunk3ds *parent, chunk3ds *child);
void ReplaceOrAddChild3ds(chunk3ds *parent, chunktag3ds tag, chunk3ds **child);
void CopyChunk3ds(chunk3ds *ichunk, chunk3ds **ochunk);
void DisconnectChunk3ds(chunk3ds *chunk);
void CopyChunkData3ds(chunk3ds *ichunk, chunk3ds *ochunk);
void InitChunkList3ds(chunklist3ds **chunk, ulong3ds count);
void ReleaseChunkList3ds(chunklist3ds **chunk);
void *InitChunkData3ds(chunk3ds *chunk);
void *ReadChunkData3ds(chunk3ds *chunk);
void WriteChunkData3ds(chunk3ds *chunk);
void FreeFileChunkData3ds(chunk3ds *chunk);
void FreeChunkData3ds(chunk3ds *chunk);
long3ds CheckChunk3ds(chunk3ds *chunk);
long3ds GetChunkValue3ds(chunktag3ds tag);
void AddChildOrdered3ds(chunk3ds *parent, chunk3ds *child);


#endif
