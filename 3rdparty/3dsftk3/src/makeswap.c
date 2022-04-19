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



/* a routine that generates platform specific byte order swapping functions
   by testing for the native processors byte order, and then writing c
   source for conversion routines.  This code is meant to be used during
   the build process prior to compiling the core code that is using the
   byte swapping routines.

   This code makes the assumption that longs are 4 byte integers, shorts
   are 2 byte integers, floats are 4 byte floating point values, doubles
   are 8 byte floating point values, and arrays of char3ds are one byte per
   char3ds and byte alligned.  This code also makes the assumption that
   members of a union overlap each other.

*/

#include <stdlib.h>
#include <stdio.h>
#include "3dstype.h"

typedef union {
   long3ds l;
   char3ds c[4];
   } TestLong;

typedef union {
   short3ds s;
   char3ds c[2];
   } TestShort;

typedef union {
   float3ds f;
   char3ds c[4];
   } TestFloat;

typedef union {
   double3ds d;
   char3ds c[8];
} TestDouble;

#define SRCNAME "swapbyte.c"  /* the name of the generated source file */

#define SRCPREAMBLE "/* this source was algorythmically generated by makeswap.c */\n\n#include \"3dstype.h\"\n#include \"swapbyte.h\"\n"

#define LongValue 0x11223344 /* the compiler defined order */
#define LByte0 0x44 /* the same value in intel order */
#define LByte1 0x33
#define LByte2 0x22
#define LByte3 0x11
#define LongSize3ds 8

#define ShortValue 0x1122 /* the compiler defined order */
#define SByte0 0x22 /* the same value in intel order */
#define SByte1 0x11
#define ShortSize3ds 2

#define CharSize3ds 1

/* warning, I picked the floating point constant arbitrarily */
#define FloatValue 1.12233E31F /* the compiler defined order */
#define FByte0 0x70 /* the same value in intel order */
#define FByte1 0xA8
#define FByte2 0x0D
#define FByte3 0x73
#define FloatSize3ds 4

#define DoubleValue 1.1223344556677E156
#define DByte0 0x6A
#define DByte1 0x5A
#define DByte2 0xE1
#define DByte3 0x22
#define DByte4 0x48
#define DByte5 0xED
#define DByte6 0x54
#define DByte7 0x60

#define DoubleSize3ds 8

TestLong tlong; /* test variables */
TestShort tshort;
TestFloat tfloat;
TestDouble tdouble;

int longtab[4]; /* translation tables */
int shorttab[2];
int floattab[4];
int doubletab[8];

FILE *newsource;

void main () {
   int i;

   /* initialize the test variables with magic numbers*/
   tlong.l = LongValue;
   tshort.s = ShortValue;
   tfloat.f = FloatValue;
   tdouble.d = DoubleValue;

   for(i = 0; i < 8; i++) printf("tdouble.c[%i] = %x\n", i, ((unsigned int)tdouble.c[i] & 0xFF));
   
   /* test for byte size */
   if (sizeof(long) != LongSize3ds)
   {
      fprintf(stdout, "ERROR: Native longs are not the same size as Intel longs.");
      fprintf(stderr, "ERROR: Native longs are not the same size as Intel longs");
      exit(1);
   }
   if (sizeof(short) != ShortSize3ds)
   {
      fprintf(stdout, "ERROR: Native shorts are not the same size as Intel shorts.");
      fprintf(stderr, "ERROR: Native shorts are not the same size as Intel shorts");
      exit(1);
   }
   if (sizeof(float) != FloatSize3ds)
   {
      fprintf(stdout, "ERROR: Native floats are not the same size as Intel floats.");
      fprintf(stderr, "ERROR: Native floats are not the same size as Intel floats");
      exit(1);
   }
   if (sizeof(double) != DoubleSize3ds)
   {
      fprintf(stdout, "ERROR: Native doubles are not the same size as Intel doubles.");
      fprintf(stderr, "ERROR: Native doubles are not the same size as Intel doubles");
      exit(1);
   }

   

   /* build translation table for long3ds */
   for (longtab[0] = 0; longtab[0] < 4; longtab[0]++) if (tlong.c[longtab[0]] == LByte0) break;
   for (longtab[1] = 0; longtab[1] < 4; longtab[1]++) if (tlong.c[longtab[1]] == LByte1) break;
   for (longtab[2] = 0; longtab[2] < 4; longtab[2]++) if (tlong.c[longtab[2]] == LByte2) break;
   for (longtab[3] = 0; longtab[3] < 4; longtab[3]++) if (tlong.c[longtab[3]] == LByte3) break;

   /* build translation table for short3ds */
   for (shorttab[0] = 0; shorttab[0] < 2; shorttab[0]++) if (tshort.c[shorttab[0]] == SByte0) break;
   for (shorttab[1] = 0; shorttab[1] < 2; shorttab[1]++) if (tshort.c[shorttab[1]] == SByte1) break;

   /* build translation table for float3ds */
   for (floattab[0] = 0; floattab[0] < 4; floattab[0]++) if ((tfloat.c[floattab[0]] & 0xFF) == FByte0) break;
   for (floattab[1] = 0; floattab[1] < 4; floattab[1]++) if ((tfloat.c[floattab[1]] & 0xFF) == FByte1) break;
   for (floattab[2] = 0; floattab[2] < 4; floattab[2]++) if ((tfloat.c[floattab[2]] & 0xFF) == FByte2) break;
   for (floattab[3] = 0; floattab[3] < 4; floattab[3]++) if ((tfloat.c[floattab[3]] & 0xFF) == FByte3) break;

   /* build translation table for double3ds */
   for (doubletab[0] = 0; doubletab[0] < 8; doubletab[0]++) if ((tdouble.c[doubletab[0]] & 0xFF) == DByte0) break;
   for (doubletab[1] = 0; doubletab[1] < 8; doubletab[1]++) if ((tdouble.c[doubletab[1]] & 0xFF) == DByte1) break;
   for (doubletab[2] = 0; doubletab[2] < 8; doubletab[2]++) if ((tdouble.c[doubletab[2]] & 0xFF) == DByte2) break;
   for (doubletab[3] = 0; doubletab[3] < 8; doubletab[3]++) if ((tdouble.c[doubletab[3]] & 0xFF) == DByte3) break;
   for (doubletab[4] = 0; doubletab[4] < 8; doubletab[4]++) if ((tdouble.c[doubletab[4]] & 0xFF) == DByte4) break;
   for (doubletab[5] = 0; doubletab[5] < 8; doubletab[5]++) if ((tdouble.c[doubletab[5]] & 0xFF) == DByte5) break;
   for (doubletab[6] = 0; doubletab[6] < 8; doubletab[6]++) if ((tdouble.c[doubletab[6]] & 0xFF) == DByte6) break;
   for (doubletab[7] = 0; doubletab[7] < 8; doubletab[7]++) if ((tdouble.c[doubletab[7]] & 0xFF) == DByte7) break;

   newsource = fopen(SRCNAME, "wt");
   if (newsource != NULL) {
      printf("Creating %s\n", SRCNAME);
      fprintf(newsource, SRCPREAMBLE);

      fprintf(newsource, "void N2ILong3ds(void *native, void *intel)");
      fprintf(newsource, "{\n");
      /* If native is an intel byte order */
      if (longtab[0]==0 && longtab[1]==1 && longtab[2]==2 && longtab[3]==3)
      {
	 fprintf(newsource, "   *((unsigned long *)(intel)) = *((unsigned long *)(native));\n");
      } else {
	 for (i = 0; i < 4; i++) fprintf(newsource,"   ((byte3ds *)intel)[%i] = ((byte3ds *)native)[%i];\n", i, longtab[i]);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void I2NLong3ds(void *intel, void *native)");
      fprintf(newsource, "{\n");
      if (longtab[0]==0 && longtab[1]==1 && longtab[2]==2 && longtab[3]==3)
      {
	 fprintf(newsource, "   *((unsigned long *)(native)) = *((unsigned long *)(intel));\n");
      } else {
	 for (i = 0; i < 4; i++) fprintf(newsource,"   ((byte3ds *)native)[%i] = ((byte3ds *)intel)[%i];\n", longtab[i], i);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void N2IShort3ds(void *native, void *intel)");
      fprintf(newsource, "{\n");
      if (shorttab[0]==0 && shorttab[1]==1)
      {
	 fprintf(newsource, "   *((unsigned short *)(intel)) = *((unsigned short *)(native));\n");
      } else {
	 for (i = 0; i < 2; i++) fprintf(newsource,"   ((byte3ds *)intel)[%i] = ((byte3ds *)native)[%i];\n", i, shorttab[i]);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void I2NShort3ds(void *intel, void *native)");
      fprintf(newsource, "{\n");
      if (shorttab[0]==0 && shorttab[1]==1)
      {
	 fprintf(newsource, "   *((unsigned short *)(native)) = *((unsigned short *)(intel));\n");
      } else {
	 for (i = 0; i < 2; i++) fprintf(newsource,"   ((byte3ds *)native)[%i] = ((byte3ds *)intel)[%i];\n", shorttab[i], i);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void N2IFloat3ds(void *native, void *intel)");
      fprintf(newsource, "{\n");
      if (floattab[0]==0 && floattab[1]==1 && floattab[2]==2 && floattab[3]==3)
      {
	 fprintf(newsource, "   *((float *)(intel)) = *((float *)(native));\n");
      } else {
	 for (i = 0; i < 4; i++) fprintf(newsource,"   ((byte3ds *)intel)[%i] = ((byte3ds *)native)[%i];\n", i, floattab[i]);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void I2NFloat3ds(void *intel, void *native)");
      fprintf(newsource, "{\n");
      if (floattab[0]==0 && floattab[1]==1 && floattab[2]==2 && floattab[3]==3)
      {
	 fprintf(newsource, "   *((float *)(native)) = *((float *)(intel));\n");
      } else {
	 for (i = 0; i < 4; i++) fprintf(newsource,"   ((byte3ds *)native)[%i] = ((byte3ds *)intel)[%i];\n", floattab[i], i);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void N2IDouble3ds(void *native, void *intel)");
      fprintf(newsource, "{\n");
      if (doubletab[0]==0 && doubletab[1]==1 && doubletab[2]==2 && doubletab[3]==3 && doubletab[4]==4 && doubletab[5]==5 && doubletab[6]==6 && doubletab[7]==7)
      {
	 fprintf(newsource, "   *((double *)(intel)) = *((double *)(native));\n");
      } else {
	 for (i = 0; i < 8; i++) fprintf(newsource,"   ((byte3ds *)intel)[%i] = ((byte3ds *)native)[%i];\n", i, doubletab[i]);
      }
      fprintf(newsource, "}\n\n");

      fprintf(newsource, "void I2NDouble3ds(void *intel, void *native)");
      fprintf(newsource, "{\n");
      if (doubletab[0]==0 && doubletab[1]==1 && doubletab[2]==2 && doubletab[3]==3 && doubletab[4]==4 && doubletab[5]==5 && doubletab[6]==6 && doubletab[7]==7)
      {
	 fprintf(newsource, "   *((double *)(native)) = *((double *)(intel));\n");
      } else {
	 for (i = 0; i < 8; i++) fprintf(newsource,"   ((byte3ds *)native)[%i] = ((byte3ds *)intel)[%i];\n", doubletab[i], i);
      }
      fprintf(newsource, "}\n\n");

      fclose(newsource);
   }
   exit(0);
}
