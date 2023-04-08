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

#ifndef FILE_3DS_H
#define FILE_3DS_H

#include "3dstype.h"
#include "3dsrange.h"
#include "3dserr.h"

/* Private Typedefs */

/* filestate3ds - Read/write state of a file */
typedef enum 
{
   StateNotKnown = 0, /* No state determined yet, file not open */
   ReadFromFile = 1,  /* Open for read */
   WriteToFile = 2,   /* Open for write */
   ReadWriteFile = 3  /* Open for read and write, not used by toolkit */
} filestate3ds;
/* End Private */

/* Public Typedefs */

/* file3ds - Handle to 3D Studio files */
typedef struct
{
   FILE *file;         /* stdio file pointer */
   char3ds *filename;  /* name of file */
   filestate3ds state; /* file open state */
   ubyte3ds index;     /* Position of pointer in the FileContextList, if the
                          index eq InvalidFileContext3ds, then it is not in the
                          list */
   void *buffer;       /* Buffered I/O buffer */
   ulong3ds buffersize;
   ulong3ds bufferposition;
   byte3ds istempfile; /* True3ds if file is a temp file */
} file3ds;
/* End Public */

static const ubyte3ds InvalidFileContext3ds = 255; /* Signifies an invalid file pointer, or
						      a file pointer without a position */

void InitContextList3ds();
void ReleaseContextList3ds();
void AddContext3ds(file3ds *file);
void DelContext3ds(file3ds *file);
void SetContextByIndex3ds(ubyte3ds index);
byte3ds IsValidFileIndex3ds(ubyte3ds index);
file3ds *GetContextByIndex3ds(ubyte3ds index);
ubyte3ds GetContextIndex3ds();
void SetContext3ds(file3ds *file);
file3ds *GetContext3ds();
void SetFileName3ds(const char3ds *filename);
char3ds *GetFileName3ds();
void InitFile3ds(file3ds **file);
ulong3ds GetFilePosition3ds();
short3ds FileSeek3ds(long3ds newposition);
void RenameFileToTemp3ds(file3ds *file);


/* Public Prototypes */

/* File control functions */
file3ds *OpenFile3ds(const char3ds *filename, const char *attrib);
file3ds *OpenFile3ds_buf(char3ds *buf, long3ds len);
void CloseAllFiles3ds();
void CloseFile3ds(file3ds *file);

/* End Public */
#endif
