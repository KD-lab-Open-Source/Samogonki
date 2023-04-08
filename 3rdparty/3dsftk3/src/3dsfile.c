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


/* The module concerns itself with the management of open 3ds files and provides
   for association of a file handle with a number. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "3dsstrlf.h"
#include "3dsrange.h"

#include "3dsftkst.h"
#include "3dsfile.h"

#include "3dserr.h"

file3ds *FileContext3ds = NULL; /* The current file being read from and/or written to at
				   any given time by the other file routines.  A trickier
				   way of doing it, but it keeps me from passing file handles
				   all around the place */
		     
file3ds **FileContextList3ds = NULL;  /* A list of open file3ds structures.  Used to
					 associate a file with an index */

static const ubyte3ds MaxFileContext = 252; /* Maximum number of open 3DS files.  Higher
					       values have other meanings, see file3ds for
					       more info on this */

static ubyte3ds NextFileContext = 0;  /* The next available *file3ds slot in FileContextList3ds */

static ubyte3ds CurrentFileContext = 0; /* The corresponding index for the file in *FileContext3ds */

long3ds NewFileBufferSize3ds = 4096*2;

/* Initializes **FileContextList3ds */
void InitContextList3ds()
{
   ubyte3ds i;

   FileContextList3ds = calloc(MaxFileContext, sizeof(file3ds *));
   if (FileContextList3ds == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   for(i = 0; i < MaxFileContext; i++) FileContextList3ds[i] = NULL;
   NextFileContext = 0;
   CurrentFileContext = 0;
}

/* Releases memory used by **FileContextList3ds */
void ReleaseContextList3ds()
{
   
   free(FileContextList3ds);
   FileContextList3ds = NULL;
   NextFileContext = 0;
   CurrentFileContext = 0;
}

/* Adds a file3ds * to **FileContextList3ds */
void AddContext3ds(file3ds *file)
{
   ubyte3ds i;

   /* If this is the first add, initialize the list */
   if (FileContextList3ds == NULL) InitContextList3ds();

   /* scan for an open slot in the list */

   for (i = 0; i < NextFileContext; i++)
   {
      if (FileContextList3ds[i] == NULL) break;
   }

   if (i>=MaxFileContext)
     SET_ERROR_RETURN(ERR_TOO_MANY_FILES);
   
   /* Install the file pointer */
   FileContextList3ds[i] = file;

   /* Associate the list position with the file */;
   file->index = i;

   /* Increment the file counter if needed */
   if (i == NextFileContext) NextFileContext++;
}

/* Removes a file3ds * from **FileContextList3ds */
void DelContext3ds(file3ds *file)
{
   myassert(file != NULL, "DelContext3ds: arg *file cannot be NULL");
   
  /* if the file's list position is accurate */
  if (file == FileContextList3ds[file->index])
  {
     /* Clear the file's entry */
     FileContextList3ds[file->index] = NULL;
     file->index = InvalidFileContext3ds; /* Make sure the file index gets invalidated */
  }
}

void SetContextByIndex3ds(ubyte3ds index)
{
   if (index <= NextFileContext)
   {
      SetContext3ds(FileContextList3ds[index]);
   }
}

file3ds *GetContextByIndex3ds(ubyte3ds index)
{
   if (index < NextFileContext)
      return(FileContextList3ds[index]);
   else
      return(NULL);
}

file3ds *GetContextByName3ds(const char3ds *name)
{
   ubyte3ds i;

   for (i = 0; i < NextFileContext; i++)
   {
      if (FileContextList3ds[i] != NULL)
      {
	 if (strcmp(FileContextList3ds[i]->filename, name) == 0)
	    return FileContextList3ds[i];
      }
   }
   return NULL;
}

ubyte3ds GetContextIndex3ds()
{
   return(FileContext3ds->index);
}

/* Returns True3ds if file index is a real file */
byte3ds IsValidFileIndex3ds(ubyte3ds index)
{
   if (index == InvalidFileContext3ds) return False3ds;
   return ((byte3ds)(GetContextByIndex3ds(index) != NULL));
}

/* Sets the file to be used for subsequent file operations */
void SetContext3ds(file3ds *file)
{
   myassert(file != NULL, "SetContext3ds: arg *file cannot be NULL");

   FileContext3ds = file;
   CurrentFileContext = file->index;
}

file3ds *GetContext3ds()
{
   return(FileContext3ds);
}

void SetFileName3ds(const char3ds *filename)
{
   long3ds length;

   assert(filename != NULL);

   length = strlenf3ds(filename, FileNameStrMax3ds);
   GetContext3ds()->filename = malloc(sizeof(char3ds)*(length+1));
   if (GetContext3ds()->filename == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   strcpy((GetContext3ds())->filename, filename);
}

char3ds *GetFileName3ds()
{
   return((GetContext3ds())->filename);
}

void InitFile3ds(file3ds **file)
{

   (*file) = malloc(sizeof(file3ds));
   
   if ((*file) == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   (*file)->file = NULL;
   (*file)->filename = NULL;
   (*file)->state = StateNotKnown;
   (*file)->index = InvalidFileContext3ds;
   (*file)->buffer = NULL;
   (*file)->buffersize = 0;
   (*file)->bufferposition = 0;
   (*file)->istempfile = False3ds;
}

file3ds *OpenFile3ds(const char3ds *filename, const char *attrib)
{
   file3ds *new = NULL;
   file3ds *exist = NULL;
   FILE *file = NULL;
   long3ds i, length;

   if (filename == NULL || attrib == NULL)
     SET_ERROR_RETURNR(ERR_INVALID_ARG, NULL);

   /* Search for duplicate filename in the list */
   exist = GetContextByName3ds(filename);

   InitFile3ds(&new);
   ADD_ERROR_RETURNR(ERR_INIT_FAILED, NULL);

   AddContext3ds(new);

   new->filename = strdup(filename);

   if (new->filename == NULL)
     ADD_ERROR_RETURNR(ERR_NO_MEM, NULL);

   strcpy(new->filename, filename);

   length = strlenf3ds(attrib, 30);
   for (i = 0; i < length; i++)
   {
      if (attrib[i] == 'r') {new->state = new->state | ReadFromFile; break;}
      
      if (attrib[i] == 'w') {new->state = new->state | WriteToFile; break;}
   }
   if (new->state == StateNotKnown)
     SET_ERROR_RETURNR(ERR_INIT_FAILED, NULL);

   /* If a file of this name is already in use */
   if (exist != NULL)
   {
      /* If the state is the same, then delete the new one and return the old */
      if (exist->state == new->state)
      {
	 free(new->filename);
	 free(new);
	 return exist;
      }

      /* If the existing is read, then the new must be write */
      if ((exist->state & ReadFromFile) != 0)
      {
	 /* Change the existing filename */
	 RenameFileToTemp3ds(exist);
	 ON_ERROR_RETURNR(NULL);
      } else { /* New file is read while existing is write */
	 /* Close it off so it can be opened for reading */
	 CloseFile3ds(exist);
      }
   }
   
   switch(new->state)
   {
   case ReadFromFile: file = fopen(new->filename, "rb"); break;
   case WriteToFile:  file = fopen(new->filename, "wb"); break;
   case ReadWriteFile: file = fopen(new->filename, "rb"); break;
   }
   
   if (file == NULL)
     SET_ERROR_RETURNR(ERR_OPENING_FILE, NULL);

   new->file = file;

   if (NewFileBufferSize3ds > 0)
   {
      void *buf;

      buf = malloc(NewFileBufferSize3ds);
      if (buf != NULL)
      {
	 new->buffer = buf;
	 new->buffersize = NewFileBufferSize3ds;
	 setbuf(file, new->buffer);
      }
   }
   
   return new;
}

file3ds *OpenFile3ds_buf(char3ds *buf, long3ds len)
{
   file3ds *new = NULL;

   InitFile3ds(&new);
   ADD_ERROR_RETURNR(ERR_INIT_FAILED, NULL);

   AddContext3ds(new);

   new->filename = 0;
   new->istempfile = 0;
   new->state = new->state | ReadFromFile;

   new->file = NULL;
   new->buffer = buf;
   new->buffersize = len;
   new->bufferposition = 0;

   return new;
}

ulong3ds GetFilePosition3ds()
{
   return GetContext3ds()->bufferposition;
}

short3ds FileSeek3ds(long3ds newposition)
{
   GetContext3ds()->bufferposition = newposition;
   return 1;
}

void CloseAllFiles3ds()
{
   ubyte3ds i;

   for(i = 0; i < NextFileContext; i++)
   {
      CloseFile3ds(FileContextList3ds[i]);
      ON_ERROR_RETURN;
   }
   free(FileContextList3ds);

   FileContextList3ds = NULL;
   NextFileContext = 0;
   CurrentFileContext = 0;
}

void CloseFile3ds(file3ds *file)
{
   long3ds closeresult = 0, removeresult = 0;

   if (file != NULL)
   {
      if (file->file != NULL)
      {
	 closeresult = fclose(file->file);
	 if (file->istempfile) removeresult = remove(file->filename);
      }
      
      DelContext3ds(file);
	 
      free(file->filename);
      free(file->buffer);
      free(file);

      if (closeresult != 0)
	 SET_ERROR_RETURN(ERR_CLOSING_FILE);
   }
}

void RenameFileToTemp3ds(file3ds *file)
{
   long3ds fileresult;
   long3ds filepos;
   char3ds *newtempname;
   char3ds *c, *t;

   filepos = ftell(file->file);
   
   fileresult = fclose(file->file);
   if (fileresult != 0) SET_ERROR_RETURN(ERR_CLOSING_FILE);

   newtempname = malloc(FileNameStrMax3ds);
   if (newtempname == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

   /* Make a copy of the old name, so we can strip off the name and keep the path */
   strcpy(newtempname, file->filename);

   for (c = &(newtempname[strlen(newtempname)-1]); c > newtempname; c--)
      if ( *c == '\\' || *c == '/' || *c == ':') 
      {
	 c++; /* Keep the seperator */
	 break;
      }

   *c = 0; /* Null terminate the path */

   /* Get the temp name */
   t = tmpnam(NULL);

   /* Since different platforms disagree on where the tempfile goes, be sure
      to strip off any extra path information */
   for (c = &(t[strlen(t)-1]); c > t; c--)
      if ( *c == '\\' || *c == '/' || *c == ':')
      {
	 t = c + 1; /* just on the other side of the slash */
	 break;
      }
   
   /* append a temp name to the path */
   strcat(newtempname, t);

   fileresult = rename(file->filename, newtempname);
   if (fileresult != 0) SET_ERROR_RETURN(ERR_OPENING_FILE);
   
   file->istempfile = True3ds;
   free(file->filename);
   file->filename = strdup(newtempname);
   if (file->filename == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
   free(newtempname);

   file->file = fopen(file->filename, "rb");
   if (file->file == NULL) SET_ERROR_RETURN(ERR_OPENING_FILE);

   if (file->buffer != NULL)
   {
      setbuf(file->file, file->buffer);
   }

   /* Return to original position */
   fseek(file->file, filepos, 0);
}

