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


#include <stdio.h>

#include "3dstype.h"
#include "3dsprim.h"
#include "3dserr.h"

#define ERR_STACK_ENTRIES 20

#define ERR_STACK_INIT {NO_FTK_ERRORS, NULL}

/* The extra entry is always set to NO_FTK_ERRORS so we always know were the end is */
static ErrRec3ds ErrStack[ERR_STACK_ENTRIES+1] = {ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT, ERR_STACK_INIT};

/*--- Global Error Flags */
byte3ds ftkerr3ds = FALSE3DS, ignoreftkerr3ds = FALSE3DS;

void PushErrList3ds(errorid3ds id)
{
   int i = 0;

   if (id == NO_FTK_ERRORS) return;

   ftkerr3ds = True3ds;
   
   for (i = 0; i < (ERR_STACK_ENTRIES-1); i++)
   {
      if (ErrStack[i].id == NO_FTK_ERRORS)
      {
	 ErrStack[i].id = id;
	 ErrStack[i].desc = ErrCodeToString3ds(id);
	 ErrStack[i+1].id = NO_FTK_ERRORS;
	 return;
      }
   }
   /* If loop completed, then ERR_STACK_ENTRIES of errors has occured */
   if (ErrStack[ERR_STACK_ENTRIES].id == NO_FTK_ERRORS) /* If the last slot is empty */
   {
      ErrStack[ERR_STACK_ENTRIES].id = id; /* Fill the last slot with a valid error */
      ErrStack[ERR_STACK_ENTRIES].desc = ErrCodeToString3ds(id);
   } else /* Set the last error to max error warning */
   {
      ErrStack[ERR_STACK_ENTRIES].id = N_ERRORS;
      ErrStack[ERR_STACK_ENTRIES].desc = ErrCodeToString3ds(N_ERRORS);
   }
}

void ClearErrList3ds()
{
  int i;

  ftkerr3ds = False3ds;

  for (i=0; i<ERR_STACK_ENTRIES; i++)
    ErrStack[i].id = FTK_NO_ERROR;
}

void DumpErrList3ds(FILE *outfile)
{
  int i = ERR_STACK_ENTRIES;

  while(i){
    if (ErrStack[i-1].id != FTK_NO_ERROR)
      fprintf(outfile, "%s\n", ErrStack[i-1].desc);
    i--;
  }
}     

const ErrRec3ds *ReturnErrorList3ds()
{
   return (const ErrRec3ds *)(&ErrStack);
}

const char3ds *ErrCodeToString3ds(errorid3ds err)
{
   switch(err)
   {
   case NO_FTK_ERRORS:
      return("NO_FTK_ERRORS: No errors are pending");
   case ERR_NO_MEM:
      return("ERR_NO_MEM: Not enough memory to complete operation");
   case ERR_INVALID_ARG:
      return("ERR_INVALID_ARG: The argument passed to the function is invalid.  Usually caused by a NULL pointer or an out of range numeric argument.");
   case ERR_INVALID_DATA:
      return("ERR_INVALID_DATA: The structure passed as an argument to the function has invalid or out of range data in its fields.");
   case ERR_INVALID_CHUNK:
      return("ERR_INVALID_CHUNK: An invalid chunk structure was encountered while reading the database. Usually caused by a corrupt database or file");
   case ERR_INVALID_DATABASE:
      return("ERR_INVALID_DATABASE: The database passed as an argument has not be created yet.");
   case ERR_WRONG_DATABASE:
      return("ERR_WRONG_DATABASE: The database passed as an argument is the wrong kind of database for this function.");
   case ERR_UNFOUND_CHUNK:
      return("ERR_UNFOUND_CHUNK: The database is missing important file chunks needed to fill out the requested structure.  Usually caused by a corrupt database or file");
   case ERR_WRONG_OBJECT:
      return("ERR_WRONG_OBJECT: The name passed to the functions exists, but is not the type of object asked for.  For example, asking for a mesh object with the GetCameraByName3ds function.");
   case ERR_NO_SUCH_FILE:
      return("ERR_NO_SUCH_FILE: The filename passed as an argument for reading does not exist");
   case ERR_INIT_FAILED:
      return("ERR_INIT_FAILED: Failed to initialize structure passed as an argument");
   case ERR_OPENING_FILE:
      return("ERR_OPENING_FILE: Could not open requested file");
   case ERR_CLOSING_FILE:
      return("ERR_CLOSING_FILE: Could not close requested file");
   case ERR_READING_FILE:
      return("ERR_READING_FILE: Error occured while reading file");
   case ERR_CREATING_DATABASE:
      return("ERR_CREATING_DATABASE: Error occured while creating database");
   case ERR_READING_DATABASE:
      return("ERR_READING_DATABASE: Error occured while reading database");
   case ERR_WRITING_DATABASE:
      return("ERR_WRITING_DATABASE: Error occured while writing database");
   case ERR_WRITING_FILE:
      return("ERR_WRITING_FILE: Error occured while writing file");
   case ERR_STRING_TOO_LONG:
      return("ERR_STRING_TOO_LONG: String encountered in file, structure, or as an argument was longer than expected.  Possibly caused by an uninitialed pointer, corrupt file or database");
   case ERR_TOO_MANY_FILES:
      return("ERR_TOO_MANY_FILES: The toolkit has reached its maximum open file limit of 252 files.");
   case N_ERRORS:
      return("N_ERRORS: More errors were reported then could be recorded.");
   }

   return("Unknown error code was encountered");
}




      

