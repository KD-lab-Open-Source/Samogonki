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


/* Source file for DumpChunk3ds function for ascii printout of file chunks. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "3dserr.h"
#include "chunkinf.h"
#include "3dsftkst.h"
#include "3dsutil.h"
#include "dumpchnk.h"
#include "chunk3ds.h"
#include "3dsmatr.h"
#include "kfutils.h"

static long3ds DumpLevel3ds = 2;

void SetDumpLevel3ds(long3ds newlevel)
{
   DumpLevel3ds = newlevel;
}

/* Retrieves the data for a chunk, formats the data into ascii text, and sends the text to outfile */
void DumpChunk3ds(FILE *outfile,  /* file formated text gets output to */
		  chunk3ds *chunk, /* chunk that is being dumped */
		  ushort3ds indentlevel /* level of indentation for the chunk */
		  )
{
   chunk3ds *child;
   
   ChunkHeaderReport3ds(outfile, chunk, indentlevel);


   if (DumpLevel3ds != TerseDump3ds)
   {
      switch(chunk->tag)
      {
      case MESH_VERSION :
      {
	 MeshVersion *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sVersion %u\n", indent(indentlevel), d->version);
	 
	 break;
      }
	 
      case M3D_VERSION :
      {
	 M3dVersion *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sVersion %u\n", indent(indentlevel), d->version);
	 
	 break;
      }
	 
      case COLOR_F :
      {
	 ColorF *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %f,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %f,", d->green);
	 fprintf(outfile, "B: %f\n", d->blue);
	 
	 break;
      }
	 
      case LIN_COLOR_F :
      {
	 LinColorF *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %f,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %f,", d->green);
	 fprintf(outfile, "B: %f\n", d->blue);
	 
	 break;
      }
	 
      case COLOR_24 :
      {
	 Color24 *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case LIN_COLOR_24 :
      {
	 LinColor24 *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }      
	 
      case INT_PERCENTAGE :
      {
	 IntPercentage *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sPercentage of %i%%\n", indent(indentlevel), d->intpercentage);
	 
	 break;
      }
	 
      case FLOAT_PERCENTAGE :
      {
	 FloatPercentage *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sPercentage of %f\n", indent(indentlevel), d->floatpercentage);
	 
	 break;
      }
	 
      case MASTER_SCALE :
      {
	 MasterScale *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMaster Scale %f\n", indent(indentlevel), d->masterscale);
	 
	 break;
      }
	 
      case BIT_MAP :
      {
	 BitMap *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBitmap name %s\n", indent(indentlevel), d->bitmap);
	 
	 break;
      }
	 
      case V_GRADIENT:
      {
	 VGradient *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 fprintf(outfile, "%sMidpoint %f\n", indent(indentlevel), d->gradpercent);
	 
	 
	 break;
      }
	 
      case LO_SHADOW_BIAS:
      {
	 LoShadowBias *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBias of %f\n", indent(indentlevel), d->bias);
	 
	 break;
      }
	 
      case HI_SHADOW_BIAS:
      {
	 HiShadowBias *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBias of %f\n", indent(indentlevel), d->bias);
	 
	 break;
      }
	 
      case RAY_BIAS:
      {
	 RayBias *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBias of %f\n", indent(indentlevel), d->bias);
	 
	 break;
      }
	 
      case SHADOW_MAP_SIZE:
      {
	 ShadowMapSize *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sSize of %i\n", indent(indentlevel), d->shadowmapsize);
	 
	 break;
      }
	 
      case SHADOW_SAMPLES:
      {
	 ShadowSamples *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sSize of %i\n", indent(indentlevel), d->shadowsamples);
	 
	 break;
      }
	 
      case SHADOW_RANGE:
      {
	 ShadowRange *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sRange of %i\n", indent(indentlevel), d->shadowrange);
	 
	 break;
      }
	 
      case SHADOW_FILTER:
      {
	 ShadowFilter *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sFilter of %f\n", indent(indentlevel), d->shadowfilter);
	 
	 break;
      }
	 
      case O_CONSTS:
      {
	 OConsts *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sPlane at %f, %f, %f\n", indent(indentlevel), d->oconsts.x, d->oconsts.y, d->oconsts.z);
	 
	 break;
      }
	 
      case FOG:
      {
	 Fog *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sNear plane at %f\n", indent(indentlevel), d->nearplanedist);
	 fprintf(outfile, "%sNear density of %f\n", indent(indentlevel), d->nearplanedensity);
	 fprintf(outfile, "%sFar plane at %f\n", indent(indentlevel), d->farplanedist);
	 fprintf(outfile, "%sFar density of %f\n", indent(indentlevel), d->farplanedensity);
	 
	 break;
      }
      case LAYER_FOG:
      {
	 LayerFog *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sFog Z range is %f to %f.\n", indent(indentlevel), d->zmin, d->zmax);
	 fprintf(outfile, "%sFog density is %f.\n", indent(indentlevel), d->density);
	 fprintf(outfile, "%sFog type of %x.\n", indent(indentlevel), d->type);
	 break;
      }
	 
      case DISTANCE_CUE:
      {
	 DistanceCue *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sNear plane at %f\n", indent(indentlevel), d->nearplanedist);
	 fprintf(outfile, "%sNear density of %f\n", indent(indentlevel), d->nearplanedimming);
	 fprintf(outfile, "%sFar plane at %f\n", indent(indentlevel), d->farplanedist);
	 fprintf(outfile, "%sFar density of %f\n", indent(indentlevel), d->farplanedimming);
	 
	 break;
      }
      case VIEW_TOP:
      case VIEW_BOTTOM:
      case VIEW_LEFT:
      case VIEW_RIGHT:
      case VIEW_FRONT:
      case VIEW_BACK:
      {
	 ViewStandard *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sTarget at %f, %f, %f\n", indent(indentlevel), d->viewtargetcoord.x, d->viewtargetcoord.y, d->viewtargetcoord.z);
	 fprintf(outfile, "%sView width of %f\n", indent(indentlevel), d->viewwidth);
	 
	 break;
      }
      case VIEW_USER:
      {
	 ViewUser *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sTarget at %f, %f, %f\n", indent(indentlevel), d->viewtargetcoord.x, d->viewtargetcoord.y, d->viewtargetcoord.z);
	 fprintf(outfile, "%sView width of %f\n", indent(indentlevel), d->viewwidth);
	 fprintf(outfile, "%sHorizontal view angle of %f\n", indent(indentlevel), d->xyviewangle);
	 fprintf(outfile, "%sVertical view angle of %f\n", indent(indentlevel), d->yzviewangle);
	 fprintf(outfile, "%sBank angle of %f\n", indent(indentlevel), d->bankangle);
	 
	 break;
      }
      case VIEW_CAMERA:
      {
	 ViewCamera *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sCamera name %s\n", indent(indentlevel), d->name);
	 
	 break;
      }
      case NAMED_OBJECT:
      {
	 NamedObject *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sName: %s\n", indent(indentlevel), d->name);
	 break;
      }
      case POINT_ARRAY:
      {
	 PointArray *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Vertices\n", indent(indentlevel), d->vertices);
	 if (DumpLevel3ds == MaximumDump3ds)
	 {
	    for(i = 0; i < d->vertices; i++)
	    {
	       fprintf(outfile, "%sVertex %i at %f, %f, %f\n", indent(indentlevel), i, d->pointlist[i].x, d->pointlist[i].y, d->pointlist[i].z);
	    }
	 }
	 
	 break;
      }

      case POINT_FLAG_ARRAY:
      {
	 PointFlagArray *d;
	 ushort3ds i;

	 d = ReadChunkData3ds(chunk);

	 fprintf(outfile, "%s%u Flags\n", indent(indentlevel), d->flags);
	 if (DumpLevel3ds == MaximumDump3ds)
	 {
	    for (i = 0; i < d->flags; i++)
	    {
	       fprintf(outfile, "%sFlag %i is %u\n", indent(indentlevel), i, d->flaglist[i]);
	    }
	 }
	 break;
      }
	 
      case FACE_ARRAY:
      {
	 FaceArray *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Faces\n", indent(indentlevel), d->faces);
	 if (DumpLevel3ds == MaximumDump3ds)
	 {
	    for(i = 0; i < d->faces; i++)
	    {
	       fprintf(outfile, "%sFace %i vertices %i, %i, %i and flag %x\n", indent(indentlevel), i, d->facelist[i].v1, d->facelist[i].v2, d->facelist[i].v3, d->facelist[i].flag);
	    }
	 }
	 
	 break;
      }
      case MSH_MAT_GROUP:
      {
	 MshMatGroup *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMaterial Name Of %s\n", indent(indentlevel), d->matname);
	 fprintf(outfile, "%sAssigned to %i faces\n", indent(indentlevel), d->faces);
	 
	 break;
      }
      case MSH_BOXMAP:
      {
	 MshBoxmap *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBoxmap consists of the following materials:\n", indent(indentlevel));
	 for(i = 0; i < 6; i++)
	    fprintf(outfile, "%s%s\n", indent(indentlevel), d->matnames[i]);
	 break;
      }
      case TEX_VERTS:
      {
	 TexVerts *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Vertices\n", indent(indentlevel), d->numcoords);
	 if (DumpLevel3ds == MaximumDump3ds)
	 {
	    for(i = 0; i < d->numcoords; i++)
	    {
	       fprintf(outfile, "%sVertex %i with tex vert of %f, %f\n", indent(indentlevel), i, d->textvertlist[i].u, d->textvertlist[i].v);
	    }
	 }
	 
	 break;
      }
	 
      case MESH_TEXTURE_INFO:
      {
	 MeshTextureInfo *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap Type of %i\n", indent(indentlevel), d->maptype);
	 fprintf(outfile, "%sX Tiling of %f\n", indent(indentlevel), d->xtiling);
	 fprintf(outfile, "%sY Tiling of %f\n", indent(indentlevel), d->ytiling);
	 fprintf(outfile, "%sIcon position of %f, %f, %f\n", indent(indentlevel), d->iconpos.x, d->iconpos.y, d->iconpos.z);
	 
	 for (i = 0; i < 12; i+=3)
	 {
	    fprintf(outfile, "%s[%i] %f [%i] %f [%i] %f\n", indent(indentlevel), i, d->xmatrix[i], i+1, d->xmatrix[i+1], i+2, d->xmatrix[i+2]);
	 }
	 
	 fprintf(outfile, "%sScaling Value of %f\n", indent(indentlevel), d->iconscaling);
	 fprintf(outfile, "%sPlanar Icon Width of %f\n", indent(indentlevel), d->iconwidth);
	 fprintf(outfile, "%sPlanar Icon Height of %f\n", indent(indentlevel), d->iconheight);
	 fprintf(outfile, "%sCylinder Icon Height of %f\n", indent(indentlevel), d->cyliconheight);
	 
	 break;
      }
      case MESH_MATRIX:
      {
	 MeshMatrix *d;
	 ushort3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 for (i = 0; i < 12; i+=3)
	 {
	    fprintf(outfile, "%s[%i] %f [%i] %f [%i] %f\n", indent(indentlevel), i, d->xmatrix[i], i+1, d->xmatrix[i+1], i+2, d->xmatrix[i+2]);
	 }
	 
	 break;
      }
		case PROC_NAME:
		{
			ProcName *d;

			d = ReadChunkData3ds(chunk);

			fprintf(outfile, "%sProcedure Name of %s\n", indent(indentlevel), d->name);

			break;
		}
      case MESH_COLOR:
      {
	 MeshColor *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor index of %i\n", indent(indentlevel), d->color);
	 break;
      }
      case N_DIRECT_LIGHT:
      {
	 NDirectLight *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sLight at %f, %f, %f\n", indent(indentlevel), d->lightpos.x, d->lightpos.y, d->lightpos.z);
	 
	 break;
      }
      case DL_EXCLUDE:
      {
	 DlExclude *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sExclude %s.\n", indent(indentlevel), d->name);
	 break;
      }
      case DL_OUTER_RANGE:
      case DL_INNER_RANGE:
      {
	 DlOuterRange *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sRange of %f.\n", indent(indentlevel), d->range);
	 break;
      }
      case DL_MULTIPLIER:
      {
	 DlMultiplier *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMultiple of %f.\n", indent(indentlevel), d->multiple);
	 break;
      }
      case DL_SPOT_ROLL:
      {
	 DlSpotRoll *d;
	 d = ReadChunkData3ds(chunk);

	 fprintf(outfile, "%sRoll angle of %f.\n", indent(indentlevel), d->angle);
	 break;
      }
      case DL_SPOT_ASPECT:
      {
	 DlSpotAspect *d;
	 d = ReadChunkData3ds(chunk);

	 fprintf(outfile, "%sSpot aspect of %f.\n", indent(indentlevel), d->aspect);
	 break;
      }
      case DL_SPOT_PROJECTOR:
      {
	 DlSpotProjector *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sFilename of %s.\n", indent(indentlevel), d->name);
	 break;
      }
      case DL_RAY_BIAS:
      {
	 DlRayBias *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sBias of %f.\n", indent(indentlevel), d->bias);
	 break;
      }
	 
      case DL_SPOTLIGHT:
      {
	 DlSpotlight *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sTarget at %f, %f, %f\n", indent(indentlevel), d->spotlighttarg.x, d->spotlighttarg.y, d->spotlighttarg.z);
	 fprintf(outfile, "%sHotspot cone of %f, ", indent(indentlevel), d->hotspotangle);
	 fprintf(outfile, "Falloff cone of %f\n",  d->falloffangle);
	 
	 break;
      }
      case DL_LOCAL_SHADOW2:
      {
	 DlLocalShadow2 *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sShadow bias of %f\n", indent(indentlevel), d->localshadowbias);
	 fprintf(outfile, "%sShadow filter of %f\n", indent(indentlevel), d->localshadowfilter);
	 fprintf(outfile, "%sShadow map size of %f\n", indent(indentlevel), d->localshadowmapsize);
	 
	 break;
      }
      case N_CAMERA:
      {
	 NCamera *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sCamera at %f, %f, %f\n", indent(indentlevel), d->camerapos.x, d->camerapos.y, d->camerapos.z);
	 fprintf(outfile, "%sTarget at %f, %f, %f\n", indent(indentlevel), d->targetpos.x, d->targetpos.y, d->targetpos.z);
	 fprintf(outfile, "%sBank angle of %f", indent(indentlevel), d->camerabank);
	 fprintf(outfile, " and a foc of %f\n", d->camerafocallength);
	 
	 break;
      }
      case CAM_RANGES:
      {
	 CamRanges *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sCamera near range is %f and far range is %f.\n", indent(indentlevel), d->nearplane, d->farplane);
	 break;
      }
      case VIEWPORT_LAYOUT:
      {
	 ViewportLayout *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sForm of %i\n", indent(indentlevel), d->form);
	 fprintf(outfile, "%sTop of %i\n", indent(indentlevel), d->top);
	 fprintf(outfile, "%sReady of %i\n", indent(indentlevel), d->ready);
	 fprintf(outfile, "%sWState of %i\n", indent(indentlevel), d->wstate);
	 fprintf(outfile, "%sSwap WS of %i\n", indent(indentlevel), d->swapws);
	 fprintf(outfile, "%sSwap Port of %i\n", indent(indentlevel), d->swapport);
	 fprintf(outfile, "%sSwap Cur of %i\n", indent(indentlevel), d->swapcur);
	 
	 break;
      }
	 
      case VIEWPORT_SIZE:
      {
	 ViewportSize *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sWork Area X: %i Y: %i W: %i H: %i\n", indent(indentlevel), d->xpos, d->ypos, d->width, d->height);
	 
	 break;
      }
      case VIEWPORT_DATA_3:
      case VIEWPORT_DATA:
      {
	 ViewportData *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sFlags of %x\n", indent(indentlevel), d->flags);
	 fprintf(outfile, "%sAxis Lockouts of %x\n", indent(indentlevel), d->axislockout);
	 fprintf(outfile, "%sWindow Position of %i, ", indent(indentlevel), d->winxpos);
	 fprintf(outfile, "%i\n", d->winypos);
	 fprintf(outfile, "%sWindow Size of %i, ", indent(indentlevel), d->winwidth);
	 fprintf(outfile, "%i\n", d->winheight);
	 fprintf(outfile, "%sWindow View of %i\n", indent(indentlevel), d->view);
	 fprintf(outfile, "%sZoom Factor of %f\n", indent(indentlevel), d->zoomfactor);
	 fprintf(outfile, "%sWorld center of %f, %f, %f\n", indent(indentlevel), d->center.x, d->center.y, d->center.z);
	 fprintf(outfile, "%sHorizontal Angle of %f\n", indent(indentlevel), d->horizang);
	 fprintf(outfile, "%sVertical Angle of %f\n", indent(indentlevel), d->vertang);
	 fprintf(outfile, "%sCamera Name of %s\n", indent(indentlevel), d->camname);
	 
	 break;
      }
      case XDATA_APPNAME:
      {
	 XDataAppName *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sApplication name %s\n", indent(indentlevel), d->name);
	 break;
      }
      case XDATA_STRING:
      {
	 XDataString *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sString value of %s\n", indent(indentlevel), d->string);
	 break;
      }
      case MAT_NAME:
      {
	 MatName *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMaterial name %s\n", indent(indentlevel), d->name);
	 
	 break;
      }
      case MAT_SHADING:
      {
	 MatShading *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sShading value of %u\n", indent(indentlevel), d->matshading);
	 
	 break;
      }
      case MAT_ACUBIC:
      {
	 MatAcubic *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sShade level of %i\n", indent(indentlevel), d->shadelevel);
	 fprintf(outfile, "%sAntialias level of %i\n", indent(indentlevel), d->antialias);
	 fprintf(outfile, "%sFlags of %u\n", indent(indentlevel), d->flags);
	 fprintf(outfile, "%sMap size of %u\n", indent(indentlevel), d->mapsize);
	 fprintf(outfile, "%sFrame skip of %u\n", indent(indentlevel), d->frameinterval);
	 
	 break;
      }
      case MAT_MAPNAME:
      {
	 MatMapname *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap name %s\n", indent(indentlevel), d->name);
	 
	 break;
      }
      case MAT_WIRESIZE:
      {
	 MatWireSize *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sWire frame size of %f\n", indent(indentlevel), d->wiresize);
	 
	 break;
      }
      case MAT_MAP_TILING:
      {
	 MatMapTiling *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap flags of", indent(indentlevel));
	 if (d->tiling & TEX_DECAL) fprintf(outfile, " TEX_DECAL");
	 if (d->tiling & TEX_MIRROR) fprintf(outfile, " TEX_MIRROR");
	 if (d->tiling & TEX_UNUSED1) fprintf(outfile, " TEX_UNUSED1");
	 if (d->tiling & TEX_INVERT) fprintf(outfile, " TEX_INVERT");
	 if (d->tiling & TEX_NOWRAP) fprintf(outfile, " TEX_NOWRAP");
	 if (d->tiling & TEX_SAT) fprintf(outfile, " TEX_SAT");
	 if (d->tiling & TEX_ALPHA_SOURCE) fprintf(outfile, " TEX_ALPHA_SOURCE");
	 if (d->tiling & TEX_TINT) fprintf(outfile, " TEX_TINT");
	 if (d->tiling & TEX_DONT_USE_ALPHA) fprintf(outfile, " TEX_DONT_USE_ALPHA");
	 if (d->tiling & TEX_RGB_TINT) fprintf(outfile, " TEX_RGB_TINT");
	 if (d->tiling == 0) fprintf(outfile, " NONE");
	 fprintf(outfile, "\n");
	 
	 break;
      }
	 
      case MAT_MAP_COL1 :
      {
	 MatMapCol1 *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case MAT_MAP_COL2 :
      {
	 MatMapCol2 *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case MAT_MAP_RCOL :
      {
	 MatMapRCol *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case MAT_MAP_GCOL :
      {
	 MatMapGCol *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case MAT_MAP_BCOL :
      {
	 MatMapBCol *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sColor R: %u,", indent(indentlevel), d->red);
	 fprintf(outfile, "G: %u,", d->green);	
	 fprintf(outfile, "B: %u\n", d->blue);
	 
	 break;
      }
	 
      case MAT_MAP_TEXBLUR:
      {
	 MatMapTexblur *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap bluring of %f\n", indent(indentlevel), d->percent);
	 
	 break;
      }
	 
      case MAT_MAP_USCALE:
      {
	 MatMapUScale *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap U scale of %f\n", indent(indentlevel), d->scale);
	 
	 break;
      }
	 
      case MAT_MAP_VSCALE:
      {
	 MatMapVScale *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap V scale of %f\n", indent(indentlevel), d->scale);
	 
	 break;
      }
	 
      case MAT_MAP_UOFFSET:
      {
	 MatMapUOffset *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap U offset of %f\n", indent(indentlevel), d->offset);
	 
	 break;
      }
	 
      case MAT_MAP_VOFFSET:
      {
	 MatMapVOffset *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap V offset of %f\n", indent(indentlevel), d->offset);
	 
	 break;
      }
	 
      case MAT_MAP_ANG:
      {
	 MatMapAng *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMap rotation angle of %f\n", indent(indentlevel), d->angle);
	 
	 break;
      }
	 
      case MAT_BUMP_PERCENT :
      {
	 MatBumpPercent *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sPercentage of %i%%\n", indent(indentlevel), d->intpercentage);
	 
	 break;
      }
	 
      case KFHDR:
      {
	 KFHdr *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sRevision level of %x\n", indent(indentlevel), d->revision);
	 fprintf(outfile, "%sFilename %s\n", indent(indentlevel), d->filename);
	 fprintf(outfile, "%sAnimation length of %i\n", indent(indentlevel), d->animlength);
	 
	 break;
      }
      case KFSEG:
      {
	 KFSeg *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sSegment starts at %i and ends at %i\n", indent(indentlevel), d->first, d->last);
	 
	 break;
      }
      case KFCURTIME:
      {
	 KFCurtime *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sCurrent frame is %i\n", indent(indentlevel), d->currframenum);
	 
	 break;
      }
      case NODE_ID:
      {
	int i;
	KFId *d;
	
	d = ReadChunkData3ds(chunk);
	i = (int)d->id;
	fprintf(outfile, "%s Node ID: %d \n", indent(indentlevel), i);
	break;
      }
      case NODE_HDR:
      {
	 NodeHdr *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, 
		 "%sObject name: %s\n", 
		 indent(indentlevel), 
		 d->objname);

	 /*--- Flags 1 */
	 fprintf(outfile, 
		 "%sFlags 1: %x\n", 
		 indent(indentlevel), 
		 d->flags1);
	 if (DumpLevel3ds == MaximumDump3ds){

	   if (d->flags1&NODE_RENDOB_HIDE)
	     fprintf(outfile, "%sNODE_RENDOB_HIDE\n", indent(indentlevel));
	   if (d->flags1&NODE_OFF)
	     fprintf(outfile, "%sNODE_OFF\n", indent(indentlevel));
	   if (d->flags1&ATKEY1)
	     fprintf(outfile, "%sATKEY1\n", indent(indentlevel));
	   if (d->flags1&ATKEY2)
	     fprintf(outfile, "%sATKEY2\n", indent(indentlevel));
	   if (d->flags1&ATKEY3)
	     fprintf(outfile, "%sATKEY3\n", indent(indentlevel));
	   if (d->flags1&ATKEY4)
	     fprintf(outfile, "%sATKEY4\n", indent(indentlevel));
	   if (d->flags1&ATKEY5)
	     fprintf(outfile, "%sATKEY5\n", indent(indentlevel));
	   if (d->flags1&ATKEYFLAGS)
	     fprintf(outfile, "%sATKEYFLAGS\n", indent(indentlevel));
	   if (d->flags1&MARK_NODE)
	     fprintf(outfile, "%sMARK_NODE\n", indent(indentlevel));
	   if (d->flags1&DISABLE_NODE)
	     fprintf(outfile, "%sDISABLE_NODE\n", indent(indentlevel));
	   if (d->flags1&HIDE_NODE)
	     fprintf(outfile, "%sHIDE_NODE\n", indent(indentlevel));
	   if (d->flags1&FAST_NODE)
	     fprintf(outfile, "%sFAST_NODE\n", indent(indentlevel));
	   if (d->flags1&PRIMARY_NODE)
	     fprintf(outfile, "%sPRIMARY_NODE\n", indent(indentlevel));
	   if (d->flags1&NODE_CALC_PATH)
	     fprintf(outfile, "%sNODE_CALC_PATH\n", indent(indentlevel));
	 }

	 /*--- Flags 2 */
	 fprintf(outfile, 
		 "%sFlags 2: %x\n", 
		 indent(indentlevel), 
		 d->flags2);
	 if (DumpLevel3ds == MaximumDump3ds){
	   if (d->flags2&NODE_HAS_PATH)
	     fprintf(outfile, "%sNODE_HAS_PATH\n",indent(indentlevel));
	   if (d->flags2&NODE_AUTO_SMOOTH)
	     fprintf(outfile, "%sNODE_AUTO_SMOOTH\n",indent(indentlevel));
	   if (d->flags2&NODE_FROZEN)
	     fprintf(outfile, "%sNODE_FROZEN\n",indent(indentlevel));
	   if (d->flags2&NODE_ANI_HIDDEN)
	     fprintf(outfile, "%sNODE_ANI_HIDDEN\n",indent(indentlevel));
	   if (d->flags2&NODE_MOTION_BLUR)
	     fprintf(outfile, "%sNODE_MOTION_BLUR\n",indent(indentlevel));
	   if (d->flags2&NODE_BLUR_BRANCH)
	     fprintf(outfile, "%sNODE_BLUR_BRANCH\n",indent(indentlevel));
	   if (d->flags2&NODE_MORPH_MTL)
	     fprintf(outfile, "%sNODE_MORPH_MTL\n",indent(indentlevel));
	   if (d->flags2&NODE_MORPH_OB)
	     fprintf(outfile, "%sNODE_MORPH_OB\n",indent(indentlevel));
	 }

	 
	 if (d->parentindex == -1) 
	   fprintf(outfile, "%sNo Parent\n", indent(indentlevel));
         else 
	   fprintf(outfile, 
		   "%sParent %i\n", indent(indentlevel), d->parentindex);
	 break;
      }
      case INSTANCE_NAME:
      {
	 InstanceName *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sInstance name: %s\n", indent(indentlevel), d->name);
	 
	 break;
      }
      case PARENT_NAME:
      {
	 InstanceName *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 if (d == NULL ||d->name[0] == 0)
	   fprintf(outfile, "%sNo Parent\n", indent(indentlevel));
	 else
	   fprintf(outfile, "%sParent name: %s\n",
		   indent(indentlevel), d->name);
	 break;
      }
      case PIVOT:
      {
	 Pivot *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sPivot at %f, %f, %f\n", indent(indentlevel), d->offset.x, d->offset.y, d->offset.z);
	 
	 break;
      }
      case BOUNDBOX:
      {
	 BoundBox *d;
	 
	 d = chunk->data;
      if (d != NULL){
	 fprintf(outfile, "%sMinimum at %f, %f, %f\n", indent(indentlevel), d->min.x, d->min.y, d->min.z);
	 fprintf(outfile, "%sMaximum at %f, %f, %f\n", indent(indentlevel), d->max.x, d->max.y, d->max.z);
     }
	 
	 break;
      }
      case MORPH_SMOOTH:
      {
	 MorphSmooth *d;
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%sMorph Smoothing Angle of %f\n", indent(indentlevel), d->smoothgroupangle);
	 
	 break;
      }
      case POS_TRACK_TAG:
      {
	 PosTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile,  &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sObject at %f, %f, %f\n", indent(indentlevel), d->positionlist[i].x, d->positionlist[i].y, d->positionlist[i].z);
	 }
	 
	 break;
      }
      case ROT_TRACK_TAG:
      {
	 RotTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) 
	 {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sRotation of %f\n", indent(indentlevel), d->rotationlist[i].angle);
	    fprintf(outfile, "%sAxis of %f, %f, %f\n", indent(indentlevel), d->rotationlist[i].x, d->rotationlist[i].y, d->rotationlist[i].z);
	 }
	 
	 break;
      }
      case SCL_TRACK_TAG:
      {
	 ScaleTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sScale of %f, %f, %f\n", indent(indentlevel), d->scalelist[i].x, d->scalelist[i].y, d->scalelist[i].z);
	 }
	 
	 break;
      }
      case FOV_TRACK_TAG:
      {
	 FovTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sCamera Fov of %f\n", indent(indentlevel), d->fovanglelist[i]);
	 }
	 
	 break;
      }
      case ROLL_TRACK_TAG:
      {
	 RollTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sCamera Roll of %f\n", indent(indentlevel), d->rollanglelist[i]);
	 }
	 
	 break;
      }
      case COL_TRACK_TAG:
      {
	 ColTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sColor R: %f,", indent(indentlevel), d->colorlist[i].r);
	    fprintf(outfile, "G: %f,", d->colorlist[i].g);
	    fprintf(outfile, "B: %f\n", d->colorlist[i].b);
	 }
	 
	 break;
      }
      case MORPH_TRACK_TAG:
      {
	 MorphTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sMorph to %s\n", indent(indentlevel), d->morphlist[i].name);
	 }
	 
	 break;
      }
      case HOT_TRACK_TAG:
      {
	 HotTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sHotspot angle of %f\n", indent(indentlevel), d->hotspotanglelist[i]);
	 }
	 
	 break;
      }
      case FALL_TRACK_TAG:
      {
	 FallTrackTag *d;
	 ulong3ds i;
	 
	 ReadChunkData3ds(chunk);
	 d = chunk->data;
	 
	 fprintf(outfile, "%s%u Keys, %x Flags.\n", indent(indentlevel), d->trackhdr.keycount, d->trackhdr.flags);
	 
	 for (i = 0; i < d->trackhdr.keycount; i++) {
	    PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	    fprintf(outfile, "%sFalloff Angle of %f\n", indent(indentlevel), d->falloffanglelist[i]);
	 }
	 
	 break;
      }
    case HIDE_TRACK_TAG:
      {
	HideTrackTag *d;
	ulong3ds i;
	
	ReadChunkData3ds(chunk);
	d = chunk->data;
	
	fprintf(outfile, "%s%u Keys, %x Flags.\n", 
		indent(indentlevel), 
		d->trackhdr.keycount, d->trackhdr.flags);
	
	for (i = 0; i < d->trackhdr.keycount; i++) {
	  PrintKeyHeader3ds(outfile, &(d->keyhdrlist[i]), indentlevel);
	}
	
	break;
      }
	 
      } /* End Switch */
   }
   
   child = chunk->children;

   while (child != NULL) 
   {
      DumpChunk3ds(outfile, child, (ushort3ds)(indentlevel+1));
      ON_ERROR_RETURN;

      child = child->sibling;
   }
   
   

}

void CheckSumChunk3ds(FILE *outfile, chunk3ds *chunk, ushort3ds indentlevel)
{
   
   ChunkHeaderReport3ds(outfile, chunk, indentlevel);
   fprintf(outfile, "%sChecksum Of %li\n", indent(indentlevel), CheckChunk3ds(chunk));
   if (chunk->children != NULL) CheckSumChunk3ds(outfile, chunk->children, (ushort3ds)(indentlevel+1));
   if (chunk->sibling != NULL) CheckSumChunk3ds(outfile, chunk->sibling, indentlevel);
}

void DumpDatabase3ds(FILE *outfile, database3ds *db)
{
/*   SetContext3ds(db->file); */
   DumpChunk3ds(outfile, db->topchunk, 0);
   
}

void CheckSumDatabase3ds(FILE *outfile, database3ds *db)
{
   CheckSumChunk3ds(outfile, db->topchunk, 0);
}
