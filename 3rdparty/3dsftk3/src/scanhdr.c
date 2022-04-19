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
#include <stdlib.h>
#include <string.h>
#include "3dstype.h"
#include "3dsprim.h"
#include "smartall.h"

static void banner (char3ds *proc_name)
{
   char3ds *cp;
   long3ds i;
   
   static const long3ds banner_size = 10;
   static char3ds *banner_help[10] = 
   {
      "headtype:",
      "   -pub     = Only declarations marked public",
      "   -priv    = Only declarations marked private",
      "",
      "dectype:",
      "   -typedef = Only type definitions",
      "   -consts  = Only constants",
      "   -protos  = Only prototypes",
      "   -define  = Only #defines",
      "   -all     = All of the above"
   };

   cp = strrchr(proc_name, '\\');
   fprintf(stdout, "Usage: %s -[headtype] -[dectype] @<headerlist> <header> <header> ...\n", (cp) ? ++cp : proc_name);
   for(i = 0; i < banner_size; i++)

   {
      fprintf(stdout, "%s\n", banner_help[i]);
   }
}


/* Flags for things to scan */
ubyte3ds gettypedef;
ubyte3ds getconsts;
ubyte3ds getprots;
ubyte3ds getdefs;
ubyte3ds getpriv;
ubyte3ds getpub;

void scanfile(char3ds *fn)
{
   FILE *infile = NULL;
   byte3ds emitlines = False3ds;
   byte3ds emitplines = False3ds;
   char3ds *buffer;
   long3ds i;

   infile = fopen(fn, "r");
   if (infile == NULL) return;

   buffer = malloc(16000);
   if (buffer == NULL) return;
      
   while (fgets(buffer, 16000, infile))
   {

      for (i = 0; i < 1; i++)
      {
	 if (getpub)
	 {
	    if (gettypedef)
	       if (strncmp("/* Public Typedefs */", buffer, 16 ) == 0)
	       {emitlines = True3ds; break;}
	    if (getconsts)
	       if (strncmp("/* Public Consts */", buffer, 16) == 0)
	       {emitlines = True3ds; break;}
	    if (getprots)
	       if (strncmp("/* Public Prototypes */", buffer, 16) == 0)
	       {emitlines = True3ds; break;}
	    if (getdefs)
	       if (strncmp("/* Public Defines */", buffer, 16) == 0)
	       {emitlines = True3ds; break;}
	    if (strncmp("/* End Public */", buffer, 12) == 0)
	    {emitlines = False3ds; break;}
	 }
	 if (getpriv)
	 {
	    if (gettypedef)
	       if (strncmp("/* Private Typedefs */", buffer, 16) == 0)
	       {emitplines = True3ds; break;}
	    if (getconsts)
	       if (strncmp("/* Private Consts */", buffer, 16) == 0)
	       {emitplines = True3ds; break;}
	    if (getprots)
	       if (strncmp("/* Private Prototypes */", buffer, 16) == 0)
	       {emitplines = True3ds; break;}
	    if (getdefs)
	       if (strncmp("/* Private Defines */", buffer, 16) == 0)
	       {emitplines = True3ds; break;}
	    if (strncmp("/* End Private */", buffer, 12) == 0)
	    {emitplines = False3ds; break;}
	 }

	 if (emitlines)
	    fprintf(stdout, "%s", buffer);
	 if (emitplines)
	 {
	    if (buffer[0] == '\n' || buffer[0] == '\r')
	       fprintf(stdout, "%s", buffer);
	    else
	       fprintf(stdout, "/* Priv */ %s", buffer);
	 }
	 
      }
   }

   fclose(infile);

   free(buffer);
}

void main(int argc, char3ds *argv[])
{

   gettypedef = False3ds;
   getconsts = False3ds;
   getprots = False3ds;
   getdefs = False3ds;
   getpriv = False3ds;
   getpub = False3ds;

   if (argc > 1)
   {
      long3ds i;
            
      /* Scan for options and filenames */
      for (i = 1; i < argc; i++)
      {
	 switch(argv[i][0])
	 {
	 case '-': /* "-*" */
	    switch(argv[i][1])
	    {
	    case 'p': /* "-p*" */
	       switch(argv[i][2])
	       {
	       case 'u': /* "-pub" */
		  getpub = True3ds;
		  break;
	       case 'r': /* "-pr*" */
		  switch(argv[i][3])
		  {
		  case 'i':
		     getpriv = True3ds;
		     break;
		  case 'o':
		     getprots = True3ds;
		  }
		  break;
	       }
	       break;
	    case 't': /* '-t*' */
	       gettypedef = True3ds;
	       break;
	    case 'c':
	       getconsts = True3ds;
	       break;
	    case 'd':
	       getdefs = True3ds;
	       break;
	    case 'a':
	       gettypedef = getconsts = getprots = getdefs = True3ds;
	       break;
	    }
	    break;
	 case '@': /* list of files */
         {
	    char3ds *c;
	    char3ds *filename;
	    const char3ds *delimiters = " \r\n\t";
	    	    
	    FILE *f;
	    
	    c = argv[i]+1;
	    f = fopen(c, "r");
	    if (f == NULL) break;
	    filename = malloc(16000);
	    if (filename == NULL) break;

	    while(fgets(filename, 16000, f))
	    {
	       char *tok;
	       for(tok = strtok(filename, delimiters); tok != NULL; tok = strtok(NULL, delimiters))
	       {
		  /* scan the file */
		  scanfile(tok);
	       }
	    }

	    fclose(f);
	    break;
	 }
	    
	 default:
	    scanfile(argv[i]);
	    break;
	 }
      }

   } else
      banner(argv[0]);

   exit(0);
}

      
	      
	       
		   
		  
	 
	
     
   
   
