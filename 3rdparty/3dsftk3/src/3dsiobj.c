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
#include <assert.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsmobj.h"
#include "3dsrobj.h"
#include "3dsiobj.h"

static const objmat3ds DefObjMat3ds = {{0}, 0, NULL};

void InitMeshObjField3ds (mesh3ds *obj, const ushort3ds field)
{
   ulong3ds i;
   void *tpnt;
   
   if (obj == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);

   /* Test to see if vertices are being allocated */
   if ((field & InitVertexArray3ds) == InitVertexArray3ds)
   {
      if (obj->nvertices == 0) /* If the vertice count is 0 then free the array */
      {
	 RelMeshObjField3ds(obj, RelVertexArray3ds);
      } else 
      {
	 if (obj->vertexarray == NULL) /* if this is the very first allocation */
	 {
	    /* Allocate the new block of memory */
	    obj->vertexarray = calloc(obj->nvertices, sizeof(point3ds));
	    if (obj->vertexarray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    /* Initialize the new block */
	    for (i = 0; i < obj->nvertices; i++)
	       memcpy(&((obj->vertexarray)[i]), &DefPoint3ds, sizeof(DefPoint3ds));
	 } else /* else this is an existing block */
	 {
	    /* Just resize it */
	    tpnt = realloc(obj->vertexarray, sizeof(point3ds)*obj->nvertices);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    /* Dont set the new pointer unless the allocation was successful */
	    obj->vertexarray = tpnt;
	 }
      }
   }
   
   if ((field & InitTextArray3ds) == InitTextArray3ds)
   {
      if (obj->ntextverts == 0)
      {
	 RelMeshObjField3ds(obj, RelTextArray3ds);
      } else 
      {
	 if (obj->textarray == NULL)
	 {
	    obj->textarray = calloc(obj->ntextverts, sizeof(textvert3ds));
	    if (obj->textarray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	    
	    for (i = 0; i < obj->ntextverts; i++)
	       memcpy(&((obj->textarray)[i]), &DefTextVert3ds, sizeof(DefTextVert3ds));
	 } else
	 {
	    tpnt = realloc(obj->textarray, sizeof(textvert3ds)*obj->ntextverts);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    obj->textarray = tpnt;
	 }
      }
   }
   
   if ((field & InitFaceArray3ds) == InitFaceArray3ds)
   {
      if (obj->nfaces == 0)
      {
	 RelMeshObjField3ds(obj, RelFaceArray3ds);
      } else
      {
	 if (obj->facearray == NULL)
	 {
	    obj->facearray = calloc(obj->nfaces, sizeof(face3ds));
	    if (obj->facearray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    for (i = 0; i < obj->nfaces; i++)
	       memcpy(&((obj->facearray)[i]), &DefFace3ds, sizeof(DefFace3ds));
	 } else
	 {
	    tpnt = realloc(obj->facearray, sizeof(face3ds)*obj->nfaces);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    obj->facearray = tpnt;
	 }
      }
   }
   
   if ((field & InitMatArray3ds) == InitMatArray3ds)
   {
      if (obj->nmats == 0)
      {
	 RelMeshObjField3ds(obj, RelMatArray3ds);
      } else
      {
	 if (obj->matarray == NULL)
	 {
	    obj->matarray = calloc(obj->nmats, sizeof(objmat3ds));
	    if (obj->matarray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	    
	    for (i = 0; i < obj->nmats; i++)
	       memcpy(&((obj->matarray)[i]), &DefObjMat3ds, sizeof(DefObjMat3ds));
	 } else
	 {
	    tpnt = realloc(obj->matarray, sizeof(objmat3ds)*obj->nmats);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    obj->matarray = tpnt;
	 }
      }
   }
   
   if ((field & InitSmoothArray3ds) == InitSmoothArray3ds)
   {
      if (obj->nfaces == 0)
      {
	 RelMeshObjField3ds(obj, RelSmoothArray3ds);
      } else
      {
	 if (obj->smootharray == NULL)
	 {
	    obj->smootharray = calloc(obj->nfaces, sizeof(ulong3ds));
	    if (obj->smootharray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    for (i = 0; i < obj->nfaces; i++)
	       (obj->smootharray)[i] = 0;
	 } else
	 {
	    tpnt = realloc(obj->smootharray, sizeof(ulong3ds)*obj->nfaces);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    obj->smootharray = tpnt;
	 }
      }
   }
   
   if ((field & InitProcData3ds) == InitProcData3ds)
   {
      if (obj->procsize == 0)
      {
	 RelMeshObjField3ds(obj, RelProcData3ds);
      } else
      {
	 if (obj->procdata == NULL)
	 {
	    obj->procdata = calloc(obj->procsize, sizeof(ubyte3ds));
	    if (obj->procdata == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    memset(obj->procdata, 0, sizeof(ubyte3ds)*obj->procsize);
	 } else
	 {
	    tpnt = realloc(obj->procdata, sizeof(ubyte3ds)*obj->procsize);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

	    obj->procdata = tpnt;
	 }
      }
   }
   
   if ((field & InitVFlagArray3ds) == InitVFlagArray3ds)
   {
      if (obj->nvertices == 0)
      {
	 RelMeshObjField3ds(obj, RelVFlagArray3ds);
      } else
      {
	 if (obj->vflagarray == NULL)
	 {
	    obj->vflagarray = calloc(obj->nvertices, sizeof(ushort3ds));
	    if (obj->vflagarray == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	    
	    for (i = 0; i < obj->nvflags; i++) obj->vflagarray[i] = 0;
	 } else
	 {
	    tpnt = realloc(obj->vflagarray, sizeof(ushort3ds)*obj->nvertices);
	    if (tpnt == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
	    
	    obj->vflagarray = tpnt;
	 }
      }
   }
   
}

void InitMatArrayIndex3ds (mesh3ds *obj, ushort3ds mat, ushort3ds nfaces)
{
  if (obj == NULL || nfaces == 0)
    SET_ERROR_RETURN(ERR_INVALID_ARG);

  if (obj->nfaces == 0 || mat > obj->nmats)
    SET_ERROR_RETURN(ERR_INVALID_DATA);

  if (obj->matarray[mat].faceindex)
    free(obj->matarray[mat].faceindex);

  obj->matarray[mat].faceindex = calloc(nfaces, sizeof(ushort3ds));
  if (obj->matarray[mat].faceindex == NULL)
    SET_ERROR_RETURN(ERR_NO_MEM);
}

void InitMeshObj3ds(mesh3ds **obj, ushort3ds nvertices, ushort3ds nfaces, ushort3ds initflags)
{
   ulong3ds i;
   mesh3ds *mobj;

   if (obj == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

   mobj = *obj; /* Some compilers generate wasteful dereferencing code for each occurance if this isn't done */
   
   if (mobj == NULL)
   {
      mobj = malloc(sizeof(mesh3ds));
      if (mobj == NULL) SET_ERROR_RETURN(ERR_NO_MEM);

      *obj = mobj;
      
      mobj->vertexarray = NULL;
      mobj->textarray = NULL;
      mobj->vflagarray = NULL;
      mobj->facearray = NULL;
      mobj->matarray = NULL;
      mobj->smootharray = NULL;
      mobj->procdata = NULL;
   }
   
   mobj->name[0] = 0;
   mobj->ishidden = False3ds;
   mobj->isvislofter = False3ds;
   mobj->ismatte = False3ds;
   mobj->isnocast = False3ds;
   mobj->isnorcvshad = False3ds;
   mobj->isfast = False3ds;
   mobj->isfrozen = False3ds;
   mobj->nvertices = nvertices;
   mobj->ntextverts = 0;
   mobj->nvflags = 0;

   mobj->usemapinfo = False3ds;
   mobj->map.maptype = 0;
   mobj->map.tilex = 1.0F;
   mobj->map.tiley = 1.0F;
   mobj->map.cenx = 0.0F;
   mobj->map.ceny = 0.0F;
   mobj->map.cenz = 0.0F;
   mobj->map.scale = 1.0F;
   mobj->map.matrix[0] = 1.0F;
   mobj->map.matrix[1] = 0.0F;
   mobj->map.matrix[2] = 0.0F;
   mobj->map.matrix[3] = 0.0F;
   mobj->map.matrix[4] = 1.0F;
   mobj->map.matrix[5] = 0.0F;
   mobj->map.matrix[6] = 0.0F;
   mobj->map.matrix[7] = 0.0F;
   mobj->map.matrix[8] = 0.0F;
   mobj->map.matrix[9] = 0.0F;
   mobj->map.matrix[10] = 0.0F;
   mobj->map.matrix[11] = 0.0F;
   mobj->map.pw = 1.0F;
   mobj->map.ph = 1.0F;
   mobj->map.ch = 1.0F;

   for (i = 0; i < 12; i++) mobj->locmatrix[i] = 0.0F;
   mobj->nfaces = nfaces;
   mobj->nmats = 0;
   mobj->useboxmap = False3ds;
   for (i = 0; i < 6; i++) mobj->boxmap[i][0] = 0;
   mobj->meshcolor = 0;
   mobj->procsize = 0;
   mobj->procname[0] = 0;
   
   InitMeshObjField3ds(mobj, (ushort3ds)(InitVertexArray3ds | InitFaceArray3ds));
   ON_ERROR_RETURN;
   
   if ((initflags & InitTextArray3ds) == InitTextArray3ds)
   {
      mobj->ntextverts = mobj->nvertices;
      InitMeshObjField3ds(mobj, InitTextArray3ds);
      ON_ERROR_RETURN;
   }

   if ((initflags & InitVFlagArray3ds) == InitVFlagArray3ds)
   {
      mobj->nvflags = mobj->nvertices;
      InitMeshObjField3ds(mobj, InitVFlagArray3ds);
      ON_ERROR_RETURN;
   }

   if ((initflags & InitSmoothArray3ds) == InitSmoothArray3ds)
   {
      InitMeshObjField3ds(mobj, InitSmoothArray3ds);
      ON_ERROR_RETURN;
   }
   return;
   
}
