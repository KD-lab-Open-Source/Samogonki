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
#include "3dserr.h"
#include "3dsftkst.h"
#include "chunk3ds.h"
#include "dbase3ds.h"
#include "3dsvprt.h"

void InitViewport3ds(viewport3ds **vp)
{
   viewport3ds *v;

   if (vp == NULL) SET_ERROR_RETURN(ERR_INVALID_ARG);

   v = *vp;
   
   if (v == NULL)
   {
      v = malloc(sizeof(viewport3ds));
      if (v == NULL) SET_ERROR_RETURN(ERR_NO_MEM);
      *vp = v;
   }

   v->type = TopView3ds;
   v->ortho.center.x = 0.0f;
   v->ortho.center.y = v->ortho.center.z = 0.0f;
   v->ortho.zoom = 0.7395f;
   v->user.center.x = v->user.center.y = v->user.center.z = 0.0f;
   v->user.zoom = 0.7395f;
   v->user.horang = 20.0f;
   v->user.verang = 30.0f;
   v->camera.name[0] = 0;
   v->size.xpos = 0;
   v->size.ypos = 0;
   v->size.width = 1000;
   v->size.height = 1000;
   
}

void ReleaseViewport3ds(viewport3ds **vp)
{
   if (vp != NULL)
   {
      free(*vp);
      *vp = NULL;
   }
}

void GetViewportEntry3ds(chunk3ds *section, viewport3ds **vp)
{
   chunk3ds *vlay, *c;
   ViewportLayout *dvlay;
   ViewportSize *dvsize;
   ViewportData *dvdata;
   long3ds portindex = 0;
   byte3ds foundv3 = False3ds;
   viewport3ds *v;

   FindNextChunk3ds(section->children, VIEWPORT_LAYOUT, &vlay);

   if (vlay != NULL)
   {
      InitViewport3ds(vp);
      ADD_ERROR_RETURN(ERR_GET_FAIL);

      v = *vp;
   
      dvlay = ReadChunkData3ds(vlay);
      ADD_ERROR_RETURN(ERR_GET_FAIL);
      
      for (c = vlay->children; c != NULL; c = c->sibling)
      {
	 switch(c->tag)
	 {
	 case VIEWPORT_SIZE:
	    dvsize = ReadChunkData3ds(c);
	    ADD_ERROR_RETURN(ERR_GET_FAIL);
	    v->size.xpos = dvsize->xpos;
	    v->size.ypos = dvsize->ypos;
	    v->size.width = dvsize->width;
	    v->size.height = dvsize->height;
	    break;
	 case VIEWPORT_DATA_3:
	    foundv3 = True3ds;
	    if (portindex == dvlay->top)
	    {
	       dvdata = ReadChunkData3ds(c);
	       ADD_ERROR_RETURN(ERR_GET_FAIL);

	       switch(dvdata->view)
	       {
	       case 0:
		  v->type = NoView3ds;
		  break;
	       case 1:
		  v->type = TopView3ds;
		  break;
	       case 2:
		  v->type = BottomView3ds;
		  break;
	       case 3:
		  v->type = LeftView3ds;
		  break;
	       case 4:
		  v->type = RightView3ds;
		  break;
	       case 5:
		  v->type = FrontView3ds;
		  break;
	       case 6:
		  v->type = BackView3ds;
		  break;
	       case 7:
		  v->type = UserView3ds;
		  break;
	       case 0xFFFF:
		  v->type = CameraView3ds;
		  break;
	       case 18:
		  v->type = SpotlightView3ds;
		  break;
	       default:
		  v->type = NoView3ds;
	       }

	       v->ortho.zoom = v->user.zoom = dvdata->zoomfactor;
	       v->ortho.center.x = v->user.center.x = dvdata->center.x;
	       v->ortho.center.y = v->user.center.y = dvdata->center.y;
	       v->ortho.center.z = v->user.center.z = dvdata->center.z;
	       v->user.horang = dvdata->horizang;
	       v->user.verang = dvdata->vertang;
	       strcpy(v->camera.name, dvdata->camname);

	    }
	    portindex++;
	    break;
	 case VIEWPORT_DATA:
	    if (foundv3) break;
	    if (portindex == dvlay->top)
	    {
	       dvdata = ReadChunkData3ds(c);
	       ADD_ERROR_RETURN(ERR_GET_FAIL);

	       switch(dvdata->view)
	       {
	       case 0:
		  v->type = NoView3ds;
		  break;
	       case 1:
		  v->type = TopView3ds;
		  break;
	       case 2:
		  v->type = BottomView3ds;
		  break;
	       case 3:
		  v->type = LeftView3ds;
		  break;
	       case 4:
		  v->type = RightView3ds;
		  break;
	       case 5:
		  v->type = FrontView3ds;
		  break;
	       case 6:
		  v->type = BackView3ds;
		  break;
	       case 7:
		  v->type = UserView3ds;
		  break;
	       case 0xFFFF:
		  v->type = CameraView3ds;
		  break;
	       case 18:
		  v->type = SpotlightView3ds;
		  break;
	       default:
		  v->type = NoView3ds;
	       }

	       v->ortho.zoom = v->user.zoom = dvdata->zoomfactor;
	       v->ortho.center.x = v->user.center.x = dvdata->center.x;
	       v->ortho.center.y = v->user.center.y = dvdata->center.y;
	       v->ortho.center.z = v->user.center.z = dvdata->center.z;
	       v->user.horang = dvdata->horizang;
	       v->user.verang = dvdata->vertang;
	       strcpy(v->camera.name, dvdata->camname);

	    }
	    portindex++;
	    break;
	 }
      }
   } else
	{
		/* Not Viewport Layout in file */
		*vp = NULL;
	}
}

/* Searches an MDATA chunk looking for a DEFAULT_VIEW chunk. *vp should allready be
   initialized before passing it to this function */
void GetDefaultView3ds(chunk3ds *section, viewport3ds *vp)
{
   chunk3ds *defv;
   chunk3ds *vtype = NULL;

   myassert(vp != NULL, "GetDefaultView3ds: arg *vp is NULL");
   
   if (section->tag == MDATA)
   {
      FindNextChunk3ds(section->children, DEFAULT_VIEW, &defv);

      if (defv != NULL) vtype = defv->children;

      if (vtype != NULL)
      {
	 switch(vtype->tag)
	 {
	 case VIEW_TOP:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;
	    }
	 case VIEW_BOTTOM:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;
	    }
	 case VIEW_LEFT:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;

	    }
	 case VIEW_RIGHT:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;

	    }
	 case VIEW_FRONT:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;
	 
	    }
	 case VIEW_BACK:
	    {
	       ViewStandard *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = TopView3ds;
	       vp->ortho.center.x = d->viewtargetcoord.x;
	       vp->ortho.center.y = d->viewtargetcoord.y;
	       vp->ortho.center.z = d->viewtargetcoord.z;
	       vp->ortho.zoom = d->viewwidth / (float3ds)vp->size.width;
	       break;
	 
	    }
	 case VIEW_USER:
	    {
	       ViewUser *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = UserView3ds;
	       vp->user.center.x = d->viewtargetcoord.x;
	       vp->user.center.y = d->viewtargetcoord.y;
	       vp->user.center.z = d->viewtargetcoord.z;
	       vp->user.zoom = d->viewwidth / (float3ds)vp->size.width;
	       vp->user.horang = d->xyviewangle;
	       vp->user.verang = d->yzviewangle;
	       break;
	       
	    }
	 case VIEW_CAMERA:
	    {
	       ViewCamera *d;
	       d = ReadChunkData3ds(vtype);
	       vp->type = CameraView3ds;
	       strcpy(vp->camera.name, d->name);
	       break;
	       
	    }
	 }

      }
   }
}

void PutDefaultView3ds(chunk3ds *section, viewport3ds *vp)
{
   chunk3ds *defv, *vtype;

   myassert(vp != NULL, "GetDefaultView3ds: arg *vp is NULL");
   
   if (section->tag == MDATA)
   {
      ReplaceOrAddChild3ds(section, DEFAULT_VIEW, &defv);
      ON_ERROR_RETURN;

      switch(vp->type)
      {
      case TopView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_TOP);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case BottomView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_BOTTOM);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case LeftView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_LEFT);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case RightView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_RIGHT);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case FrontView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_FRONT);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case BackView3ds:
	 {
	    ViewStandard *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_BOTTOM);
	    d->viewtargetcoord.x = vp->ortho.center.x;
	    d->viewtargetcoord.y = vp->ortho.center.y;
	    d->viewtargetcoord.z = vp->ortho.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->ortho.zoom;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case UserView3ds:
	 {
	    ViewUser *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_USER);
	    d->viewtargetcoord.x = vp->user.center.x;
	    d->viewtargetcoord.y = vp->user.center.y;
	    d->viewtargetcoord.z = vp->user.center.z;
	    d->viewwidth = (float3ds)vp->size.width / vp->user.zoom;
	    d->xyviewangle = vp->user.horang;
	    d->yzviewangle = vp->user.verang;
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      case CameraView3ds:
	 {
	    ViewCamera *d;
	    d = InitChunkAndData3ds(&vtype, VIEW_CAMERA);
	    d->name = strdup(vp->camera.name);
	    AddChildOrdered3ds(defv, vtype);
	    break;
	 }
      }
   }
}

void PutViewportEntry3ds(chunk3ds *section, viewport3ds *vp)
{
   chunk3ds *vlay, *vsize, *vdata, *c;
   ViewportLayout *dvlay;
   ViewportSize *dvsize;
   ViewportData *dvdata;
   ulong3ds i;
   
   /* Delete any existing viewport layout chunks */
   FindNextChunk3ds(section->children, VIEWPORT_LAYOUT, &vlay);
   if (vlay != NULL)
   {
      DeleteChunk3ds(vlay);
      /* Retag it so it can be used again */
      vlay->tag = VIEWPORT_LAYOUT;
   } else
   {
      InitChunkAs3ds(&vlay, VIEWPORT_LAYOUT);
      ON_ERROR_RETURN;
      AddChildOrdered3ds(section, vlay);
      ON_ERROR_RETURN;
   }

   dvlay = InitChunkData3ds(vlay);
   ON_ERROR_RETURN;

   /* init view to a single view configuration */
   dvlay->form = 0;
   dvlay->top = 0;
   dvlay->ready = 1;
   dvlay->wstate = 0;
   dvlay->swapws = 0;
   dvlay->swapport = 0;
   dvlay->swapcur = 0;

   dvsize = InitChunkAndData3ds(&vsize, VIEWPORT_SIZE);
   dvsize->xpos = vp->size.xpos;
   dvsize->ypos = vp->size.ypos;
   dvsize->width = vp->size.width;
   dvsize->height = vp->size.height;

   AddChildOrdered3ds(vlay, vsize);

   dvdata = InitChunkAndData3ds(&vdata, VIEWPORT_DATA_3);
   
   dvdata->flags = 1;
   dvdata->axislockout = 0;
   dvdata->winxpos = vp->size.xpos;
   dvdata->winypos = vp->size.ypos;
   dvdata->winwidth = vp->size.width;
   dvdata->winheight = vp->size.height;

   switch (vp->type)
   {
   case TopView3ds:
      dvdata->view = 1;
      break;
   case BottomView3ds:
      dvdata->view = 2;
      break;
   case LeftView3ds:
      dvdata->view = 3;
      break;
   case RightView3ds:
      dvdata->view = 4;
      break;
   case FrontView3ds:
      dvdata->view = 5;
      break;
   case BackView3ds:
      dvdata->view = 6;
      break;
   case UserView3ds:
      dvdata->view = 7;
      break;
   case CameraView3ds:
      dvdata->view = 0xFFFF;
      break;
   case SpotlightView3ds:
      dvdata->view = 18;
      break;
   }

   switch(vp->type)
   {
   case TopView3ds:
   case BottomView3ds:
   case LeftView3ds:
   case RightView3ds:
   case FrontView3ds:
   case BackView3ds:
      dvdata->zoomfactor = vp->ortho.zoom;
      dvdata->center.x = vp->ortho.center.x;
      dvdata->center.y = vp->ortho.center.y;
      dvdata->center.z = vp->ortho.center.z;
      dvdata->horizang = 20.0f;
      dvdata->vertang = 30.0f;
      dvdata->camname = strdup("");
      break;
   case UserView3ds:
      dvdata->zoomfactor = vp->user.zoom;
      dvdata->center.x = vp->user.center.x;
      dvdata->center.y = vp->user.center.y;
      dvdata->center.z = vp->user.center.z;
      dvdata->horizang = vp->user.horang;
      dvdata->vertang = vp->user.verang;
      dvdata->camname = strdup("");
      break;
   case CameraView3ds:
      dvdata->zoomfactor = 0.0f;
      dvdata->center.x = 0.0f;
      dvdata->center.y = 0.0f;
      dvdata->center.z = 0.0f;
      dvdata->horizang = 20.0f;
      dvdata->vertang = 30.0f;
      dvdata->camname = strdup(vp->camera.name);
      break;
   }

   /* Duplicate the same port 5 times */
   for (i = 0; i < 5; i++)
   {
      CopyChunk3ds(vdata, &c);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(vlay, c);
      ON_ERROR_RETURN;
   }

   /* Change to the old style port */
   vdata->tag = VIEWPORT_DATA;
   for (i = 0; i < 5; i++)
   {
      CopyChunk3ds(vdata, &c);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(vlay, c);
      ON_ERROR_RETURN;
   }

   ReleaseChunk3ds(&vdata);
   ON_ERROR_RETURN;

}

void GetViewport3ds(database3ds *db, viewport3ds **vp)
{
   chunk3ds *kfdata;

   if ((db == NULL) || (vp == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC))
      SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   FindNextChunk3ds(db->topchunk->children, KFDATA, &kfdata);
   if (kfdata != NULL)
   {
      GetViewportEntry3ds(kfdata, vp);
      ON_ERROR_RETURN;
   } else {
      FindChunk3ds(db->topchunk->children, MDATA, &kfdata);
      if (kfdata != NULL)
      {
	 GetViewportEntry3ds(kfdata, vp);
	 ON_ERROR_RETURN;
      }
   }
}


void PutViewport3ds(database3ds *db, viewport3ds *vp)
{
   chunk3ds *mdata, *kfdata;

   if ((db == NULL) || (vp == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((db->topchunk->tag != M3DMAGIC) && (db->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find or create the mdata section */
   FindNextChunk3ds(db->topchunk->children, MDATA, &mdata);
   if (mdata == NULL)
   {
      InitChunkAs3ds(&mdata, MDATA);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(db->topchunk, mdata);
   }

   PutViewportEntry3ds(mdata, vp);
   ON_ERROR_RETURN;
   
   PutDefaultView3ds(mdata, vp);
   ON_ERROR_RETURN;
   

   /* Find or create the kfdata section */
   FindNextChunk3ds(db->topchunk->children, KFDATA, &kfdata);
   if (kfdata == NULL)
   {
      InitChunkAs3ds(&kfdata, KFDATA);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(db->topchunk, kfdata);
   }

   PutViewportEntry3ds(kfdata, vp);
   ON_ERROR_RETURN;
}

void CopyViewport3ds(database3ds *destdb, database3ds *srcdb)
{
   chunk3ds *smdata, *skfdata, *dmdata, *dkfdata, *svp, *dvp;

   if ((destdb == NULL) || (srcdb == NULL)) SET_ERROR_RETURN(ERR_INVALID_ARG);
   if ((srcdb->topchunk == NULL) || (destdb->topchunk == NULL)) SET_ERROR_RETURN(ERR_INVALID_DATABASE);
   if ((srcdb->topchunk->tag != M3DMAGIC) && (srcdb->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);
   if ((destdb->topchunk->tag != M3DMAGIC) && (destdb->topchunk->tag != CMAGIC)) SET_ERROR_RETURN(ERR_WRONG_DATABASE);

   /* Find or make the mdata and kfdata sections in the destination */
   FindNextChunk3ds(destdb->topchunk->children, MDATA, &dmdata);
   if (dmdata == NULL)
   {
      InitChunkAs3ds(&dmdata, MDATA);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(destdb->topchunk, dmdata);
   }

   FindNextChunk3ds(destdb->topchunk->children, KFDATA, &dkfdata);
   if (dkfdata == NULL)
   {
      InitChunkAs3ds(&dkfdata, KFDATA);
      ON_ERROR_RETURN;

      AddChildOrdered3ds(destdb->topchunk, dkfdata);
   }

   /* Find the mdata viewport in the source file */
   FindNextChunk3ds(srcdb->topchunk->children, MDATA, &smdata);
   if (smdata != NULL)
   {
      /* Find the source viewport */
      FindNextChunk3ds(smdata->children, VIEWPORT_LAYOUT, &svp);
      if (svp != NULL)
      {
	 /* Find the viewport chunks (if any) in the destination and delete them */
	 FindNextChunk3ds(dmdata->children, VIEWPORT_LAYOUT, &dvp);
	 if (dvp != NULL) DeleteChunk3ds(dvp);

	 /* Copy the chunk */
	 CopyChunk3ds(svp, &dvp);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(dmdata, dvp);
      }

      FindNextChunk3ds(smdata->children, DEFAULT_VIEW, &svp);
      if (svp != NULL)
      {
	 FindNextChunk3ds(dmdata->children, DEFAULT_VIEW, &dvp);
	 if (dvp != NULL) DeleteChunk3ds(dvp);

	 CopyChunk3ds(svp, &dvp);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(dmdata, dvp);
      }
      
   }
   
   /* Find the kfdata viewport in the source file */
   FindNextChunk3ds(srcdb->topchunk->children, KFDATA, &skfdata);
   if (skfdata != NULL)
   {
      FindNextChunk3ds(skfdata->children, VIEWPORT_LAYOUT, &svp);
      if (svp != NULL)
      {
	 /* Find the viewport chunks (if any) in the destination and delete them */
	 FindNextChunk3ds(dkfdata->children, VIEWPORT_LAYOUT, &dvp);
	 if (dvp != NULL) DeleteChunk3ds(dvp);

	 /* Copy the chunk */
	 CopyChunk3ds(svp, &dvp);
	 ON_ERROR_RETURN;

	 AddChildOrdered3ds(dkfdata, dvp);
      }
   }
}  
	 
