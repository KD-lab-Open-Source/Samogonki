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
#include <assert.h>
#include "3dstype.h"
#include "3dserr.h"
#include "3dsfile.h"
#include "swapbyte.h"
#include "3dsprim.h"


void WriteByte3ds(byte3ds x)
{
  if (fwrite(&x, ByteSize3ds, 1, GetFile3ds()) != 1)
    SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

byte3ds ReadByte3ds()
{
   byte3ds x;
   
   if (fread(&x, ByteSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURNR(ERR_READING_FILE, 0);     

   return(x);
}

void WriteUByte3ds(ubyte3ds x)
{
   if(fwrite(&x, ByteSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

ubyte3ds ReadUByte3ds()
{
   ubyte3ds x;

   if (fread(&x, ByteSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURNR(ERR_READING_FILE, 0);
   return(x);
}

void WriteShort3ds(short3ds x)
{
   short3ds intel;

   N2IShort3ds(&x, &intel);

   if(fwrite(&intel, ShortSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

short3ds ReadShort3ds()
{
   short3ds native, intel;

   if (fread(&intel, ShortSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURNR(ERR_READING_FILE, 0);     

   I2NShort3ds(&intel, &native);

   return(native);
}

void WriteUShort3ds(ushort3ds x)
{
  ushort3ds intel;

  N2IShort3ds(&x, &intel);

  if (fwrite(&intel, ShortSize3ds, 1, GetFile3ds()) != 1)
    SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

ushort3ds ReadUShort3ds()
{
   ushort3ds native, intel;

   if (fread(&intel, ShortSize3ds, 1, GetFile3ds()) != 1)
      SET_ERROR_RETURNR(ERR_READING_FILE, 0);     
   
   I2NShort3ds(&intel, &native);

   return(native);
}

void WriteLong3ds(long3ds x)
{
   long3ds intel;

   N2ILong3ds(&x, &intel);
   
   if(fwrite(&intel, LongSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

long3ds ReadLong3ds()
{
   long3ds native, intel;

   if(fread(&intel, LongSize3ds, 1, GetFile3ds()) != 1)
      SET_ERROR_RETURNR(ERR_READING_FILE, 0);          
     
   I2NLong3ds(&intel, &native);

   return(native);
}

void WriteULong3ds(ulong3ds x)
{
   ulong3ds intel;

   N2ILong3ds(&x, &intel);

   if(fwrite(&intel, LongSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

ulong3ds ReadULong3ds()
{
   ulong3ds native, intel;

   if(fread(&intel, LongSize3ds, 1, GetFile3ds()) != 1)
      SET_ERROR_RETURNR(ERR_READING_FILE, 0);     
     
   I2NLong3ds(&intel, &native);

   return(native);
}

void WriteFloat3ds(float3ds x)
{
   float3ds intel;

   N2IFloat3ds(&x, &intel);

   if(fwrite(&intel, FloatSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     
}

float3ds ReadFloat3ds()
{
   float3ds native, intel;

   if(fread(&intel, FloatSize3ds, 1, GetFile3ds()) != 1)
      SET_ERROR_RETURNR(ERR_READING_FILE, 0.0F);     

   I2NFloat3ds(&intel, &native);

   return(native);
}

void WriteDouble3ds(double3ds x)
{
   double3ds intel;

   N2IDouble3ds(&x, &intel);

   if(fwrite(&intel, DoubleSize3ds, 1, GetFile3ds()) != 1)
     SET_ERROR_RETURN(ERR_WRITING_FILE);     

}

double3ds ReadDouble3ds()
{
   double3ds native, intel;

   if(fread(&intel, FloatSize3ds, 1, GetFile3ds()) != 1)
      SET_ERROR_RETURNR(ERR_READING_FILE, 0.0);     

   I2NDouble3ds(&intel, &native);

   return(native);
}

void WriteVoid3ds(ulong3ds size, const void *data)
{
   ulong3ds i;
   for (i = 0; i < size; i++) {
      WriteUByte3ds(((ubyte3ds *)data)[i]);
      ON_ERROR_RETURN;
   }
}

void ReadVoid3ds(ulong3ds size, void *data)
{
   ulong3ds i;
   for (i = 0; i < size; i++)
   {
      ((ubyte3ds *)data)[i] = ReadByte3ds();
      ON_ERROR_RETURN;
   }
}

void SkipVoid3ds(ulong3ds size)
{
   ulong3ds i;
   for (i = 0; i < size; i++)
   {
      ReadByte3ds();
      ON_ERROR_RETURN;
   }
}
 
void WriteString3ds(const char3ds *s)
{
   ulong3ds i,l;
   l = strlen(s);
   for (i = 0; i < l; i++) WriteByte3ds(s[i]);

   WriteByte3ds(0); /* Write a null on the end of the string */
   ON_ERROR_RETURN;
}

void WriteFixedString3ds(const char3ds *s, ulong3ds length)
{
/* length is the length of the string space including null */
   ulong3ds i,l;
   l = strlen(s);
   for (i = 0; i < l; i++)
   {
      WriteByte3ds(s[i]);
   }
   for (i = l; i < length; i++)
   {
      WriteByte3ds(0); /* fill the remaining space with nulls */
      ON_ERROR_RETURN;
   }

}

void ReadString3ds(char3ds *s, ulong3ds length)
{
/* length is the length of the string space including null */
   ulong3ds i;

   s[0] = 0;

   for (i = 0; i < length; i++)
   {
      s[i] = ReadByte3ds();
      ON_ERROR_RETURN;

      if (s[i] == 0) break;
   }

   /* check that the string wasn't too long and was null terminated */
   if((s[i] != 0) && (i == length))
     SET_ERROR_RETURN(ERR_READING_FILE);     
}

void ReadAndAllocString3ds(char3ds **s, /* the address of the pointer for the string */
               ulong3ds length /* the maximum allowable length for the string */
               )
{
   char3ds *temp = NULL;
   ulong3ds l;
   
   temp = malloc(sizeof(char3ds)*(length+1));
   if(temp == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   ReadString3ds(temp, length+1);
   ON_ERROR_RETURN;

   l = strlen(temp);
   *s = malloc(sizeof(char3ds)*(l+1));
   if (*s == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   strcpy(*s, temp);

   free(temp);
   
}
void ReadAndAllocFixedString3ds(char3ds **s, /* the address of the pointer for the string */
                ulong3ds length /* the maximum allowable length for the string */
                )
{
   char3ds *temp = NULL;
   ulong3ds l;
   
   temp = malloc(sizeof(char3ds)*(length+1));
   if (temp == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   ReadFixedString3ds(temp, length+1);
   ON_ERROR_RETURN;

   l = strlen(temp);
   *s = malloc(sizeof(char3ds)*(l+1));
   if (*s == NULL)
     SET_ERROR_RETURN(ERR_NO_MEM);

   strcpy(*s, temp);

   free(temp);
   
}

void ReadFixedString3ds(char3ds *s, ulong3ds length)
{
/* length is the length of the string space including null */
   ulong3ds i,l;

   s[0] = 0;

   for (i = 0; i < length; i++)
   {
      s[i] = ReadByte3ds();
      ON_ERROR_RETURN;

      if (s[i] == 0) break;
   }

   /* check that the string wasn't too long and was null terminated */
/*   myassert((i < length) || (s[i] == 0), "ReadFixedString3ds: Bad string read"); */

   if (s[i] != 0)
     SET_ERROR_RETURN(ERR_READING_FILE);     

   /* read the unused portion of the string */
   if (i < length)
   {
      l = strlen(s);
      for (i = l+1; i < length; i++)
      {
         s[i] = ReadByte3ds();
	 ON_ERROR_RETURN;
      }
   }
}

void WriteHeader3ds(chunktag3ds chunktype, ulong3ds size)
{
   WriteUShort3ds((ushort3ds)chunktype);
   ON_ERROR_RETURN;

   WriteULong3ds(size);
   ON_ERROR_RETURN;

}

void ReadHeader3ds(chunktag3ds *chunktype, ulong3ds *size)
{
   *chunktype = ReadUShort3ds();
   ON_ERROR_RETURN;

   *size = ReadULong3ds();
   ON_ERROR_RETURN;
}

void FinishHeader3ds(ulong3ds startpos, ulong3ds endpos)
{
   fseek(GetFile3ds(), startpos+2, SEEK_SET);
   WriteULong3ds(endpos - startpos);
   ON_ERROR_RETURN;

   fseek(GetFile3ds(), endpos, SEEK_SET);
}

void WritePoint3ds(const point3ds *p)
{
   WriteFloat3ds(p->x);
   ON_ERROR_RETURN;

   WriteFloat3ds(p->y);
   ON_ERROR_RETURN;

   WriteFloat3ds(p->z);
   ON_ERROR_RETURN;

}

void ReadPoint3ds(point3ds *p)
{
   memcpy(p, &DefPoint3ds, sizeof(DefPoint3ds));

   p->x = ReadFloat3ds();
   ON_ERROR_RETURN;
   
   p->y = ReadFloat3ds();
   ON_ERROR_RETURN;
   
   p->z = ReadFloat3ds();
   ON_ERROR_RETURN;
}

void WriteTextVert3ds(const textvert3ds *t)
{
   WriteFloat3ds(t->u);
   ON_ERROR_RETURN;

   WriteFloat3ds(t->v);
   ON_ERROR_RETURN;

}

void ReadTextVert3ds(textvert3ds *t)
{
   memcpy(t, &DefTextVert3ds, sizeof(DefTextVert3ds));

   t->u = ReadFloat3ds();
   ON_ERROR_RETURN;

   t->v = ReadFloat3ds();
   ON_ERROR_RETURN;
}

void WriteFace3ds(const face3ds *f)
{
   WriteUShort3ds(f->v1);
   ON_ERROR_RETURN;

   WriteUShort3ds(f->v2);
   ON_ERROR_RETURN;

   WriteUShort3ds(f->v3);
   ON_ERROR_RETURN;

   WriteUShort3ds(f->flag);
   ON_ERROR_RETURN;
}

void ReadFace3ds(face3ds *f)
{
   memcpy(f, &DefFace3ds, sizeof(DefFace3ds));

   f->v1 = ReadUShort3ds();
   ON_ERROR_RETURN;

   f->v2 = ReadUShort3ds();
   ON_ERROR_RETURN;

   f->v3 = ReadUShort3ds();
   ON_ERROR_RETURN;

   f->flag = ReadUShort3ds();
   ON_ERROR_RETURN;
}

void WriteTrackHeader3ds(const trackheader3ds *t)
{
   WriteUShort3ds(t->flags);
   ON_ERROR_RETURN;

   WriteULong3ds(t->nu1);
   ON_ERROR_RETURN;

   WriteULong3ds(t->nu2);
   ON_ERROR_RETURN;

   WriteULong3ds(t->keycount);
   ON_ERROR_RETURN;
}

void ReadTrackHeader3ds(trackheader3ds *t)
{
   memcpy(t, &DefTrackHeader3ds, sizeof(DefTrackHeader3ds));

   t->flags = ReadUShort3ds();
   ON_ERROR_RETURN;

   t->nu1 = ReadULong3ds();
   ON_ERROR_RETURN;

   t->nu2 = ReadULong3ds();
   ON_ERROR_RETURN;

   t->keycount = ReadULong3ds();
   ON_ERROR_RETURN;
}

void WriteKeyHeader3ds(const keyheader3ds *k)
{
   WriteULong3ds(k->time);
   ON_ERROR_RETURN;

   WriteUShort3ds(k->rflags);
   ON_ERROR_RETURN;

   if ((k->rflags & KeyUsesTension3ds) == KeyUsesTension3ds)
   {
      WriteFloat3ds(k->tension);
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesCont3ds) == KeyUsesCont3ds)
   {
      WriteFloat3ds(k->continuity);
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesBias3ds) == KeyUsesBias3ds)
   {
      WriteFloat3ds(k->bias);
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesEaseTo3ds) == KeyUsesEaseTo3ds)
   {
      WriteFloat3ds(k->easeto);
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesEaseFrom3ds) == KeyUsesEaseFrom3ds)
   {
      WriteFloat3ds(k->easefrom);
      ON_ERROR_RETURN;
   }
}

void ReadKeyHeader3ds(keyheader3ds *k)
{
   memcpy(k, &DefKeyHeader3ds, sizeof(DefKeyHeader3ds));

   k->time = ReadULong3ds();
   ON_ERROR_RETURN;

   k->rflags = ReadUShort3ds();
   ON_ERROR_RETURN;

   if ((k->rflags & KeyUsesTension3ds) == KeyUsesTension3ds)
   {
      k->tension = ReadFloat3ds();
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesCont3ds) == KeyUsesCont3ds)
   {
      k->continuity = ReadFloat3ds();
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesBias3ds) == KeyUsesBias3ds)
   {
      k->bias = ReadFloat3ds();
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesEaseTo3ds) == KeyUsesEaseTo3ds)
   {
      k->easeto = ReadFloat3ds();
      ON_ERROR_RETURN;
   }

   if ((k->rflags & KeyUsesEaseFrom3ds) == KeyUsesEaseFrom3ds)
   {
      k->easefrom = ReadFloat3ds();
      ON_ERROR_RETURN;
   }
}

