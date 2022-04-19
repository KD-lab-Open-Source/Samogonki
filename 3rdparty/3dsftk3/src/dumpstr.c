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


/* Source file for Dump<struct>3ds functions for ascii printout of file structures.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsmobj.h"
#include "3dsmatr.h"
#include "dumpstr.h"

void DumpMapInfo3ds(FILE *out, mapinfo3ds *map, long3ds dumplevel)
{
   ushort3ds i;

   i = (ushort3ds)dumplevel; /* This routine may use this arg one day, this is just to kill
		     warnings about unused args */

   fprintf(out, "Mapping info:\n");
   
   fprintf(out, "Map Type of %i\n",  map->maptype);
   fprintf(out, "X Tiling of %f\n",  map->tilex);
   fprintf(out, "Y Tiling of %f\n",  map->tiley);
   fprintf(out, "Icon position of %f, %f, %f\n",  map->cenx, map->ceny, map->cenz);

   for (i = 0; i < 12; i+=3)
   {
      fprintf(out, "[%i] %f [%i] %f [%i] %f\n",  i, map->matrix[i], i+1, map->matrix[i+1], i+2, map->matrix[i+2]);
   }

   fprintf(out, "Scaling Value of %f\n",  map->scale);
   fprintf(out, "Planar Icon Width of %f\n",  map->pw);
   fprintf(out, "Planar Icon Height of %f\n",  map->ph);
   fprintf(out, "Cylinder Icon Height of %f\n",  map->ch);
}

void DumpMesh3ds(FILE *out, mesh3ds *mesh, long3ds dumplevel)
{
   ushort3ds i;
   
   if (out == NULL || mesh == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG); 

   fprintf(out, "Mesh3ds structure contents:\n\n");
   fprintf(out, "Name: %s\n", mesh->name);
   fprintf(out, "Object Flags: ");
   if (mesh->ishidden) fprintf(out, "ishidden ");
   if (mesh->isvislofter) fprintf(out, "vislofter ");
   if (mesh->ismatte) fprintf(out, "ismatte ");
   if (mesh->isnocast) fprintf(out, "isnocast ");
   if (mesh->isfast) fprintf(out, "isfast ");
   if (mesh->isnorcvshad) fprintf(out, "isnorcvshad ");
   if (mesh->isfrozen) fprintf(out, "isfrozen");
   fprintf(out, "\n");
   fprintf(out, "Number of vertices: %i\n", mesh->nvertices);
   fprintf(out, "Number of texture coords: %i\n", mesh->ntextverts);
   if (mesh->usemapinfo) DumpMapInfo3ds(out, &(mesh->map), dumplevel);
   fprintf(out, "Mesh Matrix:\n");
   for (i = 0; i < 12; i+=3)
   {
      fprintf(out, "[%i] %f [%i] %f [%i] %f\n",  i, mesh->locmatrix[i], i+1, mesh->locmatrix[i+1], i+2, mesh->locmatrix[i+2]);
   }
   fprintf(out, "Number of faces: %i\n", mesh->nfaces);
   fprintf(out, "Mesh Color: %i\n", mesh->meshcolor);
   if (mesh->nmats > 0)
   {
      fprintf(out, "Materials assigned to mesh:\n");
      for(i = 0; i < mesh->nmats; i++)
      {
	 fprintf(out, "%s\n", mesh->matarray[i].name);
      }
   } else 
   {
      fprintf(out, "No materials assigned to mesh.\n");
   }
   if (mesh->smootharray != NULL)
   {
      ushort3ds index;
      fprintf(out, "Smooth groups assigned to mesh: ");

      for (i = 0; i < 32; i++)
      {
	 for (index = 0; index < mesh->nfaces; index++)
	 {
	    if (mesh->smootharray[index] == (ulong3ds)(1L << i))
	    {
	       fprintf(out, "%i ", i);
	       break;
	    }
	 }
      }
      fprintf(out, "\n");
   }
   if (mesh->useboxmap)
   {
      fprintf(out, "Box mapping assignment:\n");
      for (i = 0; i < 6; i++) fprintf(out, "%s\n", mesh->boxmap[i]);
   }
   
   if (mesh->procname[0] != 0) fprintf(out, "Stand-in procedure: %s\n", mesh->procname);
}

void DumpBitmap3ds(FILE *out, bitmap3ds *map, long3ds dumplevel)
{
   ulong3ds i;
   
   i = dumplevel; /* This routine may use this arg one day, this is just to kill
		     warnings about unused args */

   if (out == NULL || map == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 
   
   fprintf(out, "   Name: %s\n", map->name);
   fprintf(out, "   Strength: %f\n", map->percent);
   fprintf(out, "   Tiling: ");
   
   switch(map->tiling) {
   case Tile: fprintf(out, "Tile"); break;
   case Decal: fprintf(out, "Decal"); break;
   case Both: fprintf(out, "Both"); break;
   }
   fprintf(out, "\n");

   if (map->ignorealpha) fprintf(out, "   Ignoring Alpha Channel\n");
   if (map->filter == Pyramidal) fprintf(out, "   Pyramidal");
   else fprintf(out, "   Summed Array");
   fprintf(out, " Map Filtering\n");

   fprintf(out, "   Map Blurring of %f\n", map->blur);
   if (map->mirror) fprintf(out, "   Map is mirrored\n");
   if (map->negative) fprintf(out, "   Map is negative\n");
   fprintf(out, "   Scaling u: %f v: %f\n", map->uscale, map->vscale);
   fprintf(out, "   Offset u: %f v: %f\n", map->uoffset, map->voffset);
   fprintf(out, "   Rotation angle: %f\n", map->rotation);

   fprintf(out, "   Image source from ");
   switch(map->source)
   {
   case RGB: fprintf(out, "map RGB"); break;
   case Alpha: fprintf(out, "map alpha"); break;
   case RGBLumaTint: fprintf(out, "RGB lumanence tint"); break;
   case AlphaTint: fprintf(out, "alpha channel tint"); break;
   case RGBTint: fprintf(out, "RGB component tint"); break;
   }
   fprintf(out, "\n");

   fprintf(out, "   First tint r: %f g: %f b: %f\n", map->tint1.r, map->tint1.g, map->tint1.b);
   fprintf(out, "   Second tint r: %f g: %f b: %f\n", map->tint2.r, map->tint2.g, map->tint2.b);
   fprintf(out, "   Red tint r: %f g: %f b: %f\n", map->redtint.r, map->redtint.g, map->redtint.b);
   fprintf(out, "   Green tint r: %f g: %f b: %f\n", map->greentint.r, map->greentint.g, map->greentint.b);
   fprintf(out, "   Blue tint r: %f g: %f b: %f\n", map->bluetint.r, map->bluetint.g, map->bluetint.b);
}

void DumpMaterial3ds(FILE *out, material3ds *mat, long3ds dumplevel)
{
  if (out == NULL || mat == NULL)
    SET_ERROR_RETURN(ERR_INVALID_ARG); 

   fprintf(out, "Materials3ds structure contents:\n\n");
   fprintf(out, "Name: %s\n", mat->name);
   fprintf(out, "Ambient: r: %f, g: %f, b: %f\n", mat->ambient.r, mat->ambient.g, mat->ambient.b);
   fprintf(out, "Diffuse: r: %f, g: %f, b: %f\n", mat->diffuse.r, mat->diffuse.g, mat->diffuse.b);
   fprintf(out, "Specular: r: %f, g: %f, b: %f\n", mat->specular.r, mat->specular.g, mat->specular.b);

   fprintf(out, "Shininess: %f Shininess Strength: %f\n", mat->shininess, mat->shinstrength);
   if (mat->useblur) fprintf(out, "Using blur of %f\n", mat->blur);
   else fprintf(out, "Not using blur, but its set to %f\n", mat->blur);
   fprintf(out, "Transparency: %f\n", mat->transparency);
   if (mat->usefall) fprintf(out, "Using transparency falloff of %f\n", mat->transfalloff);
   else fprintf(out, "Not using transparency falloff, but is set to %f\n", mat->transfalloff);

   if (mat->selfillum) fprintf(out, "Self Illumination: %f\n", mat->selfillumpct);
   if (mat->usewire) fprintf(out, "Wire frame size: %f\n", mat->wiresize);
   fprintf(out, "Shading Style: ");
   switch(mat->shading)
   {
   case Wire: fprintf(out, "Wire"); break;
   case Flat: fprintf(out, "Flat"); break;
   case Gouraud: fprintf(out, "Gouraud"); break;
   case Phong: fprintf(out, "Phong"); break;
   case Metal: fprintf(out, "Metal"); break;
   }
   fprintf(out, "\n");

   if (mat->twosided) fprintf(out, "Two Sided\n");
   if (mat->additive) fprintf(out, "Additive");
   else fprintf(out, "Subtractive");
   fprintf(out, " Transparency\n");

   if (mat->facemap) fprintf(out, "Using face mapping\n");
   if (mat->soften) fprintf(out, "Using phong softening\n");

   if (mat->reflect.useauto)
   {
      fprintf(out, "Cubic Reflection Mapping Parameters: \n");
      fprintf(out, "\n");
      fprintf(out, "   Flags:");
      if (!(mat->reflect.automap.firstframe) && !(mat->reflect.automap.flat)) fprintf(out, " None");
      if (mat->reflect.automap.firstframe) fprintf(out, " [First Frame Only]");
      if (mat->reflect.automap.flat) fprintf(out, " [Flat Mirror]");
      fprintf(out, "\n");
      fprintf(out, "   Map size: %i\n", mat->reflect.automap.size);
      fprintf(out, "   Nth frame: %i\n", mat->reflect.automap.nthframe);
   }

   if (mat->texture.map.name[0] != 0)
   {
      fprintf(out, "Texture map contents:\n");
      DumpBitmap3ds(out, &mat->texture.map, dumplevel);
   }
   
   if (mat->texture.mask.name[0] != 0)
   {
      fprintf(out, "Texture mask contents:\n");
      DumpBitmap3ds(out, &mat->texture.mask, dumplevel);
   }
   
   if (mat->texture2.map.name[0] != 0)
   {
      fprintf(out, "Texture map two contents:\n");
      DumpBitmap3ds(out, &mat->texture2.map, dumplevel);
   }
   
   if (mat->texture2.mask.name[0] != 0)
   {
      fprintf(out, "Texture mask two contents:\n");
      DumpBitmap3ds(out, &mat->texture2.mask, dumplevel);
   }
   
   if (mat->opacity.map.name[0] != 0)
   {
      fprintf(out, "Opacity map contents:\n");
      DumpBitmap3ds(out, &mat->opacity.map, dumplevel);
   }
   
   if (mat->opacity.mask.name[0] != 0)
   {
      fprintf(out, "Opacity mask contents:\n");
      DumpBitmap3ds(out, &mat->opacity.mask, dumplevel);
   }
   
   if (mat->reflect.map.name[0] != 0) 
   {
      fprintf(out, "Reflection map contents:\n");
      fprintf(out, "   Name: %s\n", mat->reflect.map.name);
      fprintf(out, "   Strength: %f\n", mat->reflect.map.percent);
   }
   if (mat->reflect.mask.name[0] != 0)
   {
      fprintf(out, "Reflection mask contents:\n");
      DumpBitmap3ds(out, &mat->reflect.mask, dumplevel);
   }
   
   if (mat->bump.map.name[0] != 0)
   {
      fprintf(out, "Bump map contents:\n");
      DumpBitmap3ds(out, &mat->bump.map, dumplevel);
      fprintf(out, "   R3 Bump of %f\n", mat->bump.map.percent);
   }
   if (mat->bump.mask.name[0] != 0)
   {
      fprintf(out, "Bump mask contents:\n");
      DumpBitmap3ds(out, &mat->bump.mask, dumplevel);
   }
   
   if (mat->specmap.map.name[0] != 0)
   {
      fprintf(out, "Specular map contents:\n");
      DumpBitmap3ds(out, &mat->specmap.map, dumplevel);
   }
   
   if (mat->specmap.mask.name[0] != 0)
   {
      fprintf(out, "Specular mask contents:\n");
      DumpBitmap3ds(out, &mat->specmap.mask, dumplevel);
   }
   
   if (mat->shinmap.map.name[0] != 0)
   {
      fprintf(out, "Shininess map contents:\n");
      DumpBitmap3ds(out, &mat->shinmap.map, dumplevel);
   }
   
   if (mat->shinmap.mask.name[0] != 0)
   {
      fprintf(out, "Shininess mask contents:\n");
      DumpBitmap3ds(out, &mat->shinmap.mask, dumplevel);
   }
   
   if (mat->illummap.map.name[0] != 0)
   {
      fprintf(out, "Illumination map contents:\n");
      DumpBitmap3ds(out, &mat->illummap.map, dumplevel);
   }

   if (mat->illummap.mask.name[0] != 0)
   {
      fprintf(out, "Illummap mask contents:\n");
      DumpBitmap3ds(out, &mat->illummap.mask, dumplevel);
   }
   
}

