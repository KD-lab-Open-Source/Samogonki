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


/* Source file for DumpScr3ds functions for emiting source reps of structures */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsmobj.h"
#include "3dsmatr.h"
#include "dumpsrc.h"

void DumpMeshSrc3ds(FILE *out, const mesh3ds *mesh, const char3ds *varname)
{
   ushort3ds i,j;
   
   if (out == NULL || mesh == NULL)
     SET_ERROR_RETURN(ERR_INVALID_ARG);

   if (mesh->vertexarray != NULL)
   {
      fprintf(out, "point3ds %sverts[%i] = {", varname, mesh->nvertices);
      for (i = 0; i < mesh->nvertices; i++)
      {
	 if (i > 0) fprintf(out, ", ");
	 if ((i % 2) == 0) fprintf(out, "\n   ");
	 fprintf(out, "{%f, %f, %f}", mesh->vertexarray[i].x, mesh->vertexarray[i].y, mesh->vertexarray[i].z);
      }
      fprintf(out, "\n};\n\n");
   }

   if (mesh->textarray != NULL)
   {
      fprintf(out, "textvert3ds %stext[%i] = {", varname, mesh->ntextverts);
      for (i = 0; i < mesh->ntextverts; i++)
      {
	 if (i > 0) fprintf(out, ", ");
	 if ((i % 3) == 0) fprintf(out, "\n   ");
	 fprintf(out, "{%f, %f}", mesh->textarray[i].u, mesh->textarray[i].v);
      }
      fprintf(out, "\n};\n\n");
   }

   if (mesh->facearray != NULL)
   {
      fprintf(out, "face3ds %sface[%i] = {", varname, mesh->nfaces);
      for (i = 0; i < mesh->nfaces; i++)
      {
	 if (i > 0) fprintf(out, ", ");
	 if ((i % 2) == 0) fprintf(out, "\n   ");
	 fprintf(out, "{%i, %i, %i, 0x%x}", mesh->facearray[i].v1, mesh->facearray[i].v2, mesh->facearray[i].v3, mesh->facearray[i].flag);
      }
      fprintf(out, "\n};\n\n");
   }
   if (mesh->smootharray != NULL)
   {
      fprintf(out, "ulong3ds %ssmooth[%i] = {", varname, mesh->nfaces);
      for (i = 0; i < mesh->nfaces; i++)
      {
	 if (i > 0) fprintf(out, ", ");
	 if ((i % 4) == 0) fprintf(out, "\n   ");
	 fprintf(out, "0x%x", mesh->smootharray[i]);
      }
      fprintf(out, "\n};\n\n");
   }

   if (mesh->matarray != NULL)
   {
      for (i = 0; i < mesh->nmats; i++)
      {
	 if (mesh->matarray[i].faceindex != NULL)
	 {
	    fprintf(out, "ushort3ds %sfal%i[%i] = {", varname, i, mesh->matarray[i].nfaces);
	    for (j = 0; j < mesh->matarray[i].nfaces; j++)
	    {
	       if (j > 0) fprintf(out, ", ");
	       if ((j % 6) == 0) fprintf(out, "\n   ");
	       fprintf(out, "%u", mesh->matarray[i].faceindex[j]);
	    }
	    fprintf(out, "\n};\n\n");
	 }
      }

      fprintf(out, "objmat3ds %smats[%i] = {\n", varname, mesh->nmats);
      for (i = 0; i < mesh->nmats; i++)
      {
	 fprintf(out, "   {\"%s\", %i, ", mesh->matarray[i].name, mesh->matarray[i].nfaces);
	 fprintf(out, "%sfal%i}\n", varname, i);
      }
      fprintf(out, "\n};\n\n");
   }
   
   fprintf(out, "mesh3ds %s = {\n", varname);

   i = (ushort3ds)strlen(varname);
   fprintf(out, "   {");
   for (j = 0; j < i; j++)
   {
      if (j > 0) fprintf(out, ",");
      fprintf(out, "'%c'", varname[j]);
   }
   fprintf(out, "}, /* name */\n");

   fprintf(out, "   %u, /* ishidden */\n", mesh->ishidden);
   fprintf(out, "   %u, /* isvislofter */\n", mesh->isvislofter);
   fprintf(out, "   %u, /* ismatte */\n", mesh->ismatte);
   fprintf(out, "   %u, /* isnocast */\n", mesh->isnocast);
   fprintf(out, "   %u, /* isfast */\n", mesh->isfast);
   fprintf(out, "   %u, /* isnorcvshad */\n", mesh->isnorcvshad);
   fprintf(out, "   %u, /* isfrozen */\n", mesh->isfrozen);

   fprintf(out, "   %i, /* nvertices */\n", mesh->nvertices);

   if (mesh->vertexarray != NULL)
      fprintf(out, "   %sverts, /* vertexarray */\n", varname);
   else
      fprintf(out, "   NULL, /* vertexarray */\n");

   fprintf(out, "   %i, /* ntextverts */\n", mesh->ntextverts);

   if (mesh->textarray != NULL)
      fprintf(out, "   %stexts, /* textarray */\n", varname);
   else
      fprintf(out, "   NULL, /* textarray */\n");
   
   fprintf(out, "   %u, /* usemapinfo */\n", mesh->usemapinfo);
   
   fprintf(out, "   { /* map */\n");
   fprintf(out, "      %i, /* map.maptype */\n", mesh->map.maptype);
   fprintf(out, "      %f, /* map.tilex */\n", mesh->map.tilex);
   fprintf(out, "      %f, /* map.tiley */\n", mesh->map.tiley);
   fprintf(out, "      %f, /* map.cenx */\n", mesh->map.cenx);
   fprintf(out, "      %f, /* map.ceny */\n", mesh->map.ceny);
   fprintf(out, "      %f, /* map.cenz */\n", mesh->map.cenz);
   fprintf(out, "      %f, /* map.scale */\n", mesh->map.scale);
   fprintf(out, "      { /* map.matrix[12] */");
   for (i = 0; i < 12; i++)
   {
      if ((i % 4) == 0) fprintf(out, "\n         ");
      fprintf(out, "%f, ", mesh->map.matrix[i]);
   }
   fprintf(out, "\n      },\n");
   fprintf(out, "      %f, /* map.pw */\n", mesh->map.pw);   
   fprintf(out, "      %f, /* map.ph */\n", mesh->map.ph);
   fprintf(out, "      %f, /* map.ch */\n", mesh->map.ch);
   fprintf(out, "   },\n");
   fprintf(out, "   { /* locmatrix[12] */");
   for (i = 0; i < 12; i++)
   {
      if ((i % 4) == 0) fprintf(out, "\n      ");
      fprintf(out, "%f, ", mesh->locmatrix[i]);
   }
   fprintf(out, "\n   },\n");
   fprintf(out, "   %i, /* nfaces */\n", mesh->nfaces);
   if (mesh->facearray != NULL)
      fprintf(out, "   %sface, /* facearray */\n", varname);
   else
      fprintf(out, "   NULL, /* facearray */\n");
   if (mesh->smootharray != NULL)
      fprintf(out, "   %ssmooth, /* smootharray */\n", varname);
   else
      fprintf(out, "   NULL, /* smootharray */\n");

   fprintf(out, "   %u, /* useboxmap */\n", mesh->useboxmap);
   fprintf(out, "   { /* boxmap */\n");
   for (i = 0; i < 6; i++)
   {
      if (mesh->boxmap[i][0] == 0)
	 fprintf(out, "      \"\",\n");
      else
	 fprintf(out, "      \"%s\",\n", mesh->boxmap[i]);
   }
   fprintf(out, "   },\n");
   fprintf(out, "   %i, /* meshcolor */\n", mesh->meshcolor);
   fprintf(out, "   %i, /* nmats */\n", mesh->nmats);
   if (mesh->matarray != NULL)
      fprintf(out, "   %smats, /* matarray */\n", varname);
   else
      fprintf(out, "   NULL, /* matarray */\n");

   fprintf(out, "   %i, /* procsize */\n", mesh->procsize);
   fprintf(out, "   \"%s\", /* procname */\n", mesh->procname);
   if (mesh->procdata != NULL)
      fprintf(out, "   %sproc /* procdata */\n", varname);
   else
      fprintf(out, "   NULL /* procdata */\n");
   fprintf(out, "};\n");
}

