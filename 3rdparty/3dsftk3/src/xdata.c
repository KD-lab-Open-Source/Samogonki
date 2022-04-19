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


#include <stdlib.h>

#include <string.h>
#include "3dstype.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "3dserr.h"
#include "kfutils.h"
#include "xdata.h"


/* GetXDataCount3ds counts the number of occurances of XDATA_ENTRIES
   contained in the chunk. */

ulong3ds GetXDataCount3ds(chunk3ds *chunk)
{
   chunk3ds *xdatasection, *xdataentry;
   ulong3ds xdatacount = 0;
      
   if (chunk == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_ARG, 0);

   if (chunk->tag != XDATA_SECTION)
      FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);
   else
      xdatasection = chunk;

   if (xdatasection == NULL) return 0;

   for (FindChunk3ds(xdatasection, XDATA_ENTRY, &xdataentry);
           xdataentry != NULL;
           FindNextChunk3ds(xdataentry->sibling, XDATA_ENTRY, &xdataentry)
        ) xdatacount++;
   
   return(xdatacount);
}

void GetXDataChunkList3ds(chunk3ds *chunk, chunklist3ds **list)
{
   chunk3ds *xdatasection, *xdataentry, *xdataname;
   long3ds xdatacount = 0, i;
   XDataAppName *data;
   
   if (chunk == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   xdatacount = GetXDataCount3ds(chunk);

   InitChunkList3ds(list, xdatacount);

   if (xdatacount <= 0) return;

   if (chunk->tag != XDATA_SECTION)
      FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);
   else
      xdatasection = chunk;

   for (i = 0, FindChunk3ds(xdatasection, XDATA_ENTRY, &xdataentry);
           xdataentry != NULL;
           i++, FindNextChunk3ds(xdataentry->sibling, XDATA_ENTRY, &xdataentry)
        )
   {
      FindChunk3ds(xdataentry, XDATA_APPNAME, &xdataname);
      if (xdataname != NULL) /* If there is an appname then put it in the list */
      {
              ReadChunkData3ds(xdataname);
              data = xdataname->data;
         
              (*list)->list[i].name = strdup(data->name);
              (*list)->list[i].chunk = xdataentry;
         FreeChunkData3ds(xdataname);    
              
      } else /* Put a null string in the list entry */
      {
              (*list)->list[i].name = malloc(sizeof(char3ds));
              (*list)->list[i].name[0] = 0;
         (*list)->list[i].chunk = xdataentry;
      }
   }

  
}

void GetXDataNameList3ds(chunk3ds *chunk, namelist3ds **list)
{
   chunk3ds *xdatasection, *xdataentry, *xdataname;
   long3ds xdatacount = 0, i;
   XDataAppName *data;
   
   if (chunk == NULL || list == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   xdatacount = GetXDataCount3ds(chunk);

   InitNameList3ds(list, xdatacount);

   if (xdatacount <= 0) return;

   if (chunk->tag != XDATA_SECTION)
      FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);
   else
      xdatasection = chunk;

   for (i = 0, FindChunk3ds(xdatasection, XDATA_ENTRY, &xdataentry);
           xdataentry != NULL;
           i++, FindNextChunk3ds(xdataentry->sibling, XDATA_ENTRY, &xdataentry)
        )
   {
      FindChunk3ds(xdataentry, XDATA_APPNAME, &xdataname);
      if (xdataname != NULL) /* If there is an appname then put it in the list */
      {
              ReadChunkData3ds(xdataname);
              data = xdataname->data;
         
              (*list)->list[i].name = strdup(data->name);
              
      } else /* Put a null string in the list entry */
      {
              (*list)->list[i].name = malloc(sizeof(char3ds));
              (*list)->list[i].name[0] = 0;
         
      }
   }
  
}


void FindXDataEntryByName3ds(chunk3ds *chunk, char3ds *appname, chunk3ds **xentry)
{
   chunk3ds *xdatasection, *xdataentry, *xdataname;
        XDataAppName *data;
   
   if (chunk == NULL || appname == NULL || xentry == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   *xentry = NULL;

   FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);

   if (xdatasection == NULL) return;

   for (FindChunk3ds(xdatasection, XDATA_ENTRY, &xdataentry);
      xdataentry != NULL;
           FindNextChunk3ds(xdataentry->sibling, XDATA_ENTRY, &xdataentry))
        {
           FindChunk3ds(xdataentry, XDATA_APPNAME, &xdataname);
           
           if (xdataname != NULL)
           {
                data = ReadChunkData3ds(xdataname);
                
                if (strcmp(appname, data->name) == 0)
                {
                        *xentry = xdataentry;
                        return;
                }
           }
        }
}    

void PutXDataEntryByName3ds(chunk3ds *chunk, char3ds *appname, chunk3ds *xentry)
{
        chunk3ds *xdatasection=NULL, *xdataentry=NULL, *xdataname=NULL, *section;

        if (chunk == NULL || appname == NULL || xentry == NULL)
                SET_ERROR_RETURN(ERR_INVALID_ARG);

        FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);
        ADD_ERROR_RETURN(ERR_PUT_FAIL)
        
        if (xdatasection == NULL)
        {
                /* Search for the appropriate chunk to create the XDATA_SECTION in */
                FindChunk3ds(chunk, N_TRI_OBJECT, &section);
                if (section != NULL) 
                {
                        InitChunkAs3ds(&xdatasection, XDATA_SECTION);
                        AddChildOrdered3ds(section, xdatasection);
                }
        }


        /* If an entry with the same appname already exists, then delete it */
        FindXDataEntryByName3ds(xdatasection, appname, &xdataentry);
        if (xdataentry != NULL)
        {
                DeleteChunk3ds(xdataentry);
                ADD_ERROR_RETURN(ERR_PUT_FAIL);
        }

        CopyChunk3ds(xentry, &xdataentry);

        AddChildOrdered3ds(xdatasection, xentry);
   ADD_ERROR_RETURN(ERR_PUT_FAIL);

        
}       
                                

void FindXDataEntryByIndex3ds(chunk3ds *chunk, ulong3ds index, chunk3ds **xentry)
{
   chunk3ds *xdatasection, *xdataentry;
        ulong3ds i;
   
   if (chunk == NULL || xentry == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   *xentry = NULL;

   FindChunk3ds(chunk, XDATA_SECTION, &xdatasection);

   if (xdatasection == NULL) return;

   for (FindChunk3ds(xdatasection, XDATA_ENTRY, &xdataentry), i = 0;
      xdataentry != NULL;
           FindNextChunk3ds(xdataentry->sibling, XDATA_ENTRY, &xdataentry), i++)
        {
           if (i == index) 
                {
                        *xentry = xdataentry;
                        return;
                }
        }
}



/* This function retrieves the entire XDATA_ENTRY chunk */
void GetXDataRawEntryByName3ds(chunk3ds *chunk, char3ds *appname, xdataraw3ds **data)
{
   chunk3ds *xsection = NULL, *xentry = NULL, *xname = NULL, *temp = NULL;
   XDataEntry *entrydata;

   if (chunk == NULL || appname == NULL || data == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindChunk3ds(chunk, XDATA_SECTION, &xsection);
   if (xsection == NULL)
   {
      *data = NULL;
      return;
   }

   FindXDataEntryByName3ds(xsection, appname, &xentry);
   if (xentry == NULL)
   {
      *data = NULL;
      return;
   }

   CopyChunk3ds(xentry, &temp);

   *data = malloc(sizeof(xdataraw3ds));
   
   entrydata = ReadChunkData3ds(temp);

   (*data)->size = entrydata->size;

   (*data)->data = entrydata->data;
   entrydata->data = NULL;

   ReleaseChunk3ds(&temp);
   
         
}
   

/* This function retrieves the entire XDATA_ENTRY chunk */
void GetXDataRawEntryByIndex3ds(chunk3ds *chunk, ulong3ds index, xdataraw3ds **data)
{
   chunk3ds *xsection = NULL, *xentry = NULL, *xname = NULL, *temp = NULL;
   XDataEntry *entrydata;

   if (chunk == NULL || data == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindChunk3ds(chunk, XDATA_SECTION, &xsection);
   if (xsection == NULL)
   {
      *data = NULL;
      return;
   }

   FindXDataEntryByIndex3ds(xsection, index, &xentry);
   if (xentry == NULL)
   {
      *data = NULL;
      return;
   }

   CopyChunk3ds(xentry, &temp);

   *data = malloc(sizeof(xdataraw3ds));
   
   entrydata = ReadChunkData3ds(temp);

   (*data)->size = entrydata->size;

   (*data)->data = entrydata->data;
   entrydata->data = NULL;

   ReleaseChunk3ds(&temp);
   
         
}

ulong3ds GetMeshXDataCount3ds(database3ds *db, char3ds *meshname)
{
        chunk3ds *nobj, *ntrichunk;

        if (db == NULL || meshname == NULL)
                SET_ERROR_RETURNR(ERR_INVALID_ARG, 0);

        FindNamedObject3ds(db, meshname, &nobj);

        if (nobj != NULL)
        {
                FindChunk3ds(nobj, N_TRI_OBJECT, &ntrichunk);
                if (ntrichunk != NULL)
                {
                        return GetXDataCount3ds(ntrichunk);
                } else
                {
                        return 0;
                }
        } 

        return 0;
}

void GetMeshXDataByIndex3ds(database3ds *db, char3ds *meshname, ulong3ds index, xdataraw3ds **data)
{
        chunk3ds *nobj, *ntrichunk;

        if (db == NULL || meshname == NULL || data == NULL)
        SET_ERROR_RETURN(ERR_INVALID_ARG);

        FindNamedObject3ds(db, meshname, &nobj);

        if (nobj != NULL)
        {
                FindChunk3ds(nobj, N_TRI_OBJECT, &ntrichunk);
                if (ntrichunk != NULL)
                {
                        GetXDataRawEntryByIndex3ds(ntrichunk, index, data);
                }
        }
}

void GetMeshXDataByName3ds(database3ds *db, char3ds *meshname, char3ds *appname, xdataraw3ds **data)
{
   chunk3ds *nobj, *ntrichunk;

   if (db == NULL || meshname == NULL || appname == NULL || data == NULL)
   SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindNamedObject3ds(db, meshname, &nobj);

   if (nobj != NULL)
   {
      FindChunk3ds(nobj, N_TRI_OBJECT, &ntrichunk);
      if (ntrichunk != NULL)
      {
         GetXDataRawEntryByName3ds(ntrichunk, appname, data);
      }
   }
}


#ifdef not_working

/* AddXDataEntry3ds adds/replaces external data to the children list of the
   given chunk.  If an XDATA_SECTION doesn't exist, it will create it.
   If an XDATA_APPNAME already exists, it will replace it. */   

void AddXDataEntry3ds(chunk3ds *chunk, char3ds *appname, xdata3ds *data)
{
   chunk3ds *xsection = NULL, *xentry = NULL, *xname = NULL;
   XDataAppName *namedata;

   if (chunk == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindChunk3ds(chunk, XDATA_SECTION, &xsection);

   if (xsection == NULL) /* A new XDATA_SECTION must be created */
   {
      InitChunk3ds(&xsection);
      xsection->tag = XDATA_SECTION;
      AddChild3ds(chunk, xsection);
   }

   if (xsection->children == NULL) /* A new XDATA_ENTRY must be created */
   {
      InitChunk3ds(&xentry);
      xentry->tag = XDATA_ENTRY;

      xsection->children = xentry;
      
   } else /* Look for a matching appname */
   {
      FindXDataEntryByName3ds(xsection, appname, &xentry);
      if (xentry != NULL) /* if a match was found then overwrite it */
      {
         chunk3ds *tempsib;
         
         ReleaseChunk3ds(&(xentry->children));
         tempsib = xentry->sibling; /* temp store sibling list */
         InitChunk3ds(&xentry); /* Note that InitChunk shouln't allocate more memory if there is already some there */
         xentry->tag = XDATA_ENTRY;
         xentry->sibling = tempsib;
      } else  /* Add a new entry to the end of the sibling list */
      {
         InitChunk3ds(&xentry);
         xentry->tag = XDATA_ENTRY;
         AddChild3ds(xsection, xentry);
      }
   }

   /* Lets build up the new xentry */

   InitChunk3ds(&xname);
   xname->tag = XDATA_APPNAME;
   namedata = malloc(sizeof(XDataAppName));
   xname->data = namedata;
   namedata->name = malloc((strlen(appname)+1)*sizeof(char3ds));
   strcpy(namedata->name, appname);
      
   AddChild3ds(xentry, xname);
   
   AddXData3ds(xentry, data);
   
}


/*-------------------------------------------------------------------------
 |
 |AddAmbientLightNodeXData3ds
 |
 | Note: There is only one AMBIENT_NODE_TAG.
 +------------------------------------------------------------------------*/
void AddAmbientLightNodeXData3ds(database3ds *db, 
                 char3ds *appname, 
                 xdata3ds *data)
{
  chunk3ds *pKfChunk, *pChunk;

  FindChunk3ds(db->topchunk, KFDATA, &pKfChunk);
  if (pKfChunk){
    FindChunk3ds(pKfChunk, AMBIENT_NODE_TAG, &pChunk);
    if (pChunk)
      AddXDataEntry3ds(pChunk, appname, data);
  }
}

/*-------------------------------------------------------------------------
 | 
 | AddCameraNodeXData3ds
 | 
 +------------------------------------------------------------------------*/
void AddCameraNodeXData3ds(database3ds *db, 
               char3ds *camname, 
               char3ds *appname, 
               xdata3ds *data)
{
    kfAddGenericNodeXData(db, camname, appname, CAMERA_NODE_TAG, data);
}


void PutCameraXData3ds(database3ds *db, char3ds *camname, char3ds *appname, xdata3ds *data)
{
   chunk3ds *nobj, *ncam;

   if (db == NULL || camname == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

   FindNamedObject3ds(db, camname, &nobj);
   ON_ERROR_RETURN;

   if (nobj != NULL)
   {
      FindChunk3ds(nobj, N_CAMERA, &ncam);
      if (ncam != NULL)
      {
     AddXDataEntry3ds(ncam, appname, data);
      }
   }
}


void PutMaterialXData3ds(database3ds *db, char3ds *matname, char3ds *appname, xdata3ds *data)
{
   chunk3ds *matentry = NULL;

   if (db == NULL || matname == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindMatEntry3ds(db, matname, &matentry);

   if (matentry != NULL)
      AddXDataEntry3ds(matentry, appname, data);
   
}

void PutMeshXData3ds(database3ds *db, char3ds *meshname, char3ds *appname, xdata3ds *data)
{
   chunk3ds *nobj, *ntrichunk;

   if (db == NULL || meshname == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindNamedObject3ds(db, meshname, &nobj);

   if (nobj != NULL)
   {
      FindChunk3ds(nobj, N_TRI_OBJECT, &ntrichunk);
      if (ntrichunk != NULL)
      {
              AddXDataEntry3ds(ntrichunk, appname, data);
      }
   }
}
#endif

#ifdef not_working
/*-------------------------------------------------------------------------
  |
  |AddObjectNodeXData3ds
  |
  +------------------------------------------------------------------------*/
void AddObjectNodeXData3ds(database3ds *db, 
               char3ds *name, 
               char3ds *appname, 
               xdata3ds *data)
{
    kfAddGenericNodeXData(db, name, appname, OBJECT_NODE_TAG, data);
}

/*-------------------------------------------------------------------------
 |
 |AddOmnilightNodeXData3ds
 |
 +------------------------------------------------------------------------*/
void AddOmnilightNodeXData3ds(database3ds *db, 
                  char3ds *name, 
                  char3ds *appname, 
                  xdata3ds *data)
{
    kfAddGenericNodeXData(db, name, appname, LIGHT_NODE_TAG, data);
}

/*-------------------------------------------------------------------------
  |
  |AddSpotlightNodeXData3ds
  |
  +------------------------------------------------------------------------*/
void AddSpotlightNodeXData3ds(database3ds *db, 
                  char3ds *name, 
                  char3ds *appname, 
                  xdata3ds *data)
{
    kfAddGenericNodeXData(db, name, appname, SPOTLIGHT_NODE_TAG, data);
}

void AddKfSectionXData3ds(database3ds *db, char3ds *appname, xdata3ds *data)
{
   chunk3ds *kfdata;

   if (db == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindChunk3ds(db->topchunk, KFDATA, &kfdata);

   if (kfdata != NULL)
      AddXDataEntry3ds(kfdata, appname, data);
}

void PutMeshSectionXData3ds(database3ds *db, char3ds *appname, xdata3ds *data)
{
   chunk3ds *mdata;

   if (db == NULL || appname == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   FindChunk3ds(db->topchunk, MDATA, &mdata);
   if (mdata != NULL)
      AddXDataEntry3ds(mdata, appname, data);
}
#endif

#ifdef OldWayOfDoingIt

/* Older function relies on having a list made, but since the list only exists for the life of the function, I
   thought it a bit wasteful. */
void FindXDataEntryByName3ds(chunk3ds *chunk, char3ds *appname, chunk3ds **xentry)
{
   chunklist3ds *list = NULL;
   ulong3ds x;
   
   if (chunk == NULL || appname == NULL || xentry == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   *xentry = NULL;
   GetXDataChunkList3ds(chunk, &list);

   for (x = 0; x < list->count; x ++)
   {
      if (strcmp(appname, list->list[x].name) == 0) 
      {
              *xentry = list->list[x].chunk;
              break;
      }
   }

   ReleaseChunkList3ds(&list);
   
}
#endif

#ifdef NOT_YET_COMPLETE

void GetXData3ds(chunk3ds *chunk, xdata3ds **xdata)
{
   if (chunk == NULL || data == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

   *xdata = malloc(sizeof(xdata3ds));
   
   switch (chunk->tag)
   {
   case XDATA_ENTRY:
   {
      long3ds i;
      
                                           
      /* Are there recognizable sub chunks */
      if (chunk->children != NULL)
      {
         chunk3ds *temp;
         /* Count the number of subchunks */
         for (temp = chunk->children, i = 0; temp != NULL; temp = temp->sibling, i++);

         /* Allocate enough entries in the list */
         (*xdata)->data.entry.list = calloc(i, sizeof(xdata3ds));
         xdata->data.group.items = i;

         /* Fill up the entries */
         for (temp = chunk->children; temp != NULL; temp = temp->sibling)
         {
            GetXData3ds(temp, &(((*xdata)->data.entry.list)[i]));
         }
      }

      /* Is there unidentified data? */
      
      /* Code needs to be writen here and else where to handle this
         case */
      break;
   }
   case XDATA_APPNAME:
   { 
      XDataAppName *data;
      
      data = ReadChunkData3ds(chunk);

      xdata->data.name = data->name;
      data->name = NULL;
      break;
   }
   case XDATA_STRING:
   {
      XDataString *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.string = data->string;
      data->string = NULL;
      break;
   }
   case XDATA_FLOAT:
   {
      XDataFloat *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.float = data->value;
      break;
   }
   case XDATA_DOUBLE:
   {
      XDataDouble *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.double = data->value;
      break;
   }
   case XDATA_SHORT:
   {
      XDataShort *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.short = data->value;
      break;
   }
   case XDATA_LONG:
   {
      XDataLong *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.long = data->value;
      break;
   }
   case XDATA_VOID:
   {
      XDataVoid *data;
      data = ReadChunkData3ds(chunk);

      xdata->data.void.size = (chunk->size - 6);
      xdata->data.void.void = data->void;
      data->void = NULL;
   }
   case XDATA_GROUP:
   {
      long3ds i;
      
      if (chunk->children != NULL)
      {
         chunk3ds *temp;
         /* Count the number of subchunks */
         for (temp = chunk->children, i = 0; temp != NULL; temp = temp->sibling, i++);

         /* Allocate enough entries in the list */
         (*xdata)->data.group.list = calloc(i, sizeof(xdata3ds));
         xdata->data.group.items = i;

         /* Fill up the entries */
         for (temp = chunk->children; temp != NULL; temp = temp->sibling)
         {
            GetXData3ds(temp, &(((*xdata)->data.entry.list)[i]));
         }
      }
      


void AddXData3ds(chunk3ds *chunk, xdata3ds *data)
{
   chunk3ds *newchunk;
   long3ds i;

   if (chunk == NULL || data == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);


   if(chunk->tag != XDATA_ENTRY)
     SET_ERROR_RETURN(ERR_INVALID_DATA);


   if (data->xtag != XDataEntry3ds) /* XDataEntry is alread in *chunk */
   {
      InitChunk3ds(&newchunk);
      AddChild3ds(chunk, newchunk);
   }
      
   switch(data->xtag)
   {
   case XDataEntry3ds:
   {
      xdatalist3ds *l = data->data;
      for(i = 0; i < l->items; i++) AddXData3ds(chunk, &(l->list[i]));
      break;
   }
   case XDataString3ds:
   {
      XDataString *d;
      char3ds *s = data->data;

      newchunk->tag = XDATA_STRING;
      newchunk->size = 6+strlen(s)+1; /* header+string+nul */
      
      d = malloc(sizeof(XDataString));
      newchunk->data = d;
      
      d->string = malloc((strlen(s)+1)*sizeof(char3ds));
      strcpy(d->string, s);
      break;
   }
   case XDataFloat3ds:
   {
      XDataFloat *d;
      float3ds *f = data->data;

      newchunk->tag = XDATA_FLOAT;
      newchunk->size = 6+4; /* header+float */
      
      d = malloc(sizeof(XDataFloat));
      newchunk->data = d;

      d->value = *f;
      break;
   }
   case XDataDouble3ds:
   {
      XDataDouble *d;
      double3ds *doub = data->data;

      newchunk->tag = XDATA_DOUBLE;
      newchunk->size = 6+8; /* header+double */
      
      d = malloc(sizeof(XDataDouble));
      newchunk->data = d;

      d->value = *doub;
      break;
   }
   case XDataShort3ds:
   {
      XDataShort *d;
      ushort3ds *s = data->data;

      newchunk->tag = XDATA_SHORT;
      newchunk->size = 6+2; /* header+short */
      
      d = malloc(sizeof(XDataShort));
      newchunk->data = d;

      d->value = *s;
      break;
   }
   case XDataLong3ds:
   {
      XDataLong *d;
      long3ds *l = data->data;

      newchunk->tag = XDATA_LONG;
      newchunk->size = 6+4; /* header+long */
       
      d = malloc(sizeof(XDataLong));
      newchunk->data = d;

      d->value = *l;
      break;
   }
   case XDataGroup3ds:
   {
      xdatalist3ds *l = data->data;

      newchunk->tag = XDATA_GROUP;
      newchunk->size = 0; /* Size not known */
      
      for (i = 0; i < l->items; i++) AddXData3ds(newchunk, &(l->list[i]));
      break;
   }
   default: /* note that "case XDataVoid3ds:" ends up here */
   {
      xdatavoid3ds *v = data->data;

      newchunk->tag = (chunktag3ds)(data->xtag);
      newchunk->size = 6 + v->size; /* header + data */
      
      if(v->size > 0)
      {
         newchunk->data = malloc(v->size);
         memcpy(newchunk->data, v->data, v->size);
      }
      break;
   }
   } /* end switch */
}
#endif
