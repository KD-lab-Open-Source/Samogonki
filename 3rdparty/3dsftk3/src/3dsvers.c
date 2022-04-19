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


/* Module Overview:
   3dsvers.c, 3dsvers.h - File Release related functions.
   
*/

#include <stdio.h>
#include <string.h>
#include "3dstype.h"
#include "dbase3ds.h"
#include "3dsprim.h"
#include "chunkinf.h"
#include "chunk3ds.h"
#include "3dsftkst.h"
#include "3dsvers.h"
#include "3dserr.h"

/* Scans the database for M3D_VERSION chunk and returnes its release */
releaselevel3ds GetM3dMagicRelease3ds(database3ds *db)
{
   chunk3ds *c = NULL;

   if (db == NULL)
      ADD_ERROR_RETURNR(ERR_INVALID_ARG, ReleaseNotKnown3ds);

   if (db->topchunk == NULL)
      ADD_ERROR_RETURNR(ERR_INVALID_DATABASE, ReleaseNotKnown3ds);
   
   /* If the database is a 3DS file */
   if (db->topchunk->tag == M3DMAGIC)
   {
      FindChunk3ds(db->topchunk, M3D_VERSION, &c);

      if (c != NULL)
      {
	 M3dVersion *d;
	 d = ReadChunkData3ds(c);

	 if (d->version == 1) return Release13ds;
	 else
	    if (d->version == 2) return Release23ds;
	    else
	       if (d->version == 3) return Release33ds;
	       else
		  return ReleaseNotKnown3ds;
      }
      else
	 return ReleaseNotKnown3ds;
   }
   else
      return ReleaseNotKnown3ds;
}

/* Sets the M3dMagicRelease level */
void PutM3dMagicRelease3ds(database3ds *db, releaselevel3ds release)
{

   if (db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_DATA);

   if (db->topchunk->tag == M3DMAGIC)
   {
      chunk3ds *c;
      M3dVersion *d;

      ReplaceOrAddChild3ds(db->topchunk, M3D_VERSION, &c);
      ON_ERROR_RETURN;
      d = InitChunkData3ds(c);
      ON_ERROR_RETURN;

      switch(release)
      {
      case Release13ds:
	 d->version = 1;
	 break;
      case Release23ds:
	 d->version = 2;
	 break;
      case Release33ds:
	 d->version = 3;
	 break;
      case ReleaseNotKnown3ds:
	 d->version = 4;
      }
   }

}

/* Scans the database for MESH_VERSION chunk and returnes its release */
releaselevel3ds GetMeshRelease3ds(database3ds *db)
{
   chunk3ds *c = NULL;

   if (db == NULL)
      ADD_ERROR_RETURNR(ERR_INVALID_ARG, ReleaseNotKnown3ds);

   if (db->topchunk == NULL)
      ADD_ERROR_RETURNR(ERR_INVALID_DATABASE, ReleaseNotKnown3ds);
   
   /* If the database is a 3DS file */
   if ((db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
   {
      FindChunk3ds(db->topchunk, MESH_VERSION, &c);

      if (c != NULL)
      {
	 MeshVersion *d;
	 d = ReadChunkData3ds(c);

	 if (d->version == 1) return Release13ds;
	 else
	    if (d->version == 2) return Release23ds;
	    else
	       if (d->version == 3) return Release33ds;
	       else
		  return ReleaseNotKnown3ds;
      }
      else
	 return ReleaseNotKnown3ds;
   }
   else
       return ReleaseNotKnown3ds;
}

/* Sets the MeshRelease level */
void PutMeshRelease3ds(database3ds *db, releaselevel3ds release)
{

   if (db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_DATA);

   if ((db->topchunk->tag == M3DMAGIC) || (db->topchunk->tag == CMAGIC))
   {
      chunk3ds *mdata;
      chunk3ds *c;
      M3dVersion *d;


      FindChunk3ds(db->topchunk, MDATA, &mdata);

      if (mdata == NULL)
      {
	 InitChunkAs3ds(&mdata, MDATA);
	 AddChildOrdered3ds(db->topchunk, mdata);
      }
      
      ReplaceOrAddChild3ds(mdata, MESH_VERSION, &c);
      d = InitChunkData3ds(c);

      switch(release)
      {
      case Release13ds:
	 d->version = 1;
	 break;
      case Release23ds:
	 d->version = 2;
	 break;
      case Release33ds:
	 d->version = 3;
	 break;
      case ReleaseNotKnown3ds:
	 d->version = 4;
      }
   }
}

/* Scans the database for KFHDR chunk and returnes its release level */
releaselevel3ds GetKfRelease3ds(database3ds *db)
{
   chunk3ds *c = NULL, *kfdata;

   if (db == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_ARG, ReleaseNotKnown3ds);
   if (db->topchunk == NULL)
      SET_ERROR_RETURNR(ERR_INVALID_DATABASE, ReleaseNotKnown3ds);

   /* If the database is a 3DS file */
   if ((db->topchunk->tag == M3DMAGIC || db->topchunk->tag == CMAGIC))
   {
      FindChunk3ds(db->topchunk, KFDATA, &kfdata);

      if (kfdata != NULL)
	 FindChunk3ds(db->topchunk, KFHDR, &c);

      if (c != NULL)
      {
	 KFHdr *d;
	 d = ReadChunkData3ds(c);

	 if (d->revision == 1) return Release13ds;
	 else
	    if (d->revision == 2) return Release23ds;
	    else
	       if (d->revision == 5) return Release33ds;
	       else
		  return ReleaseNotKnown3ds;
      }
      else
	 return ReleaseNotKnown3ds;
   }
   else
      return ReleaseNotKnown3ds;
}

/* Sets the KFDATA release level */
void PutKfRelease3ds(database3ds *db, releaselevel3ds release)
{
   KFHdr *d;

   if (db == NULL)
      SET_ERROR_RETURN(ERR_INVALID_ARG);
   if (db->topchunk == NULL)
      SET_ERROR_RETURN(ERR_INVALID_DATABASE);

   if ((db->topchunk->tag == M3DMAGIC) || (db->topchunk->tag == CMAGIC))
   {
      chunk3ds *kfdata;
      chunk3ds *kfhdr;

      /* Find the KFDATA section */
      FindChunk3ds(db->topchunk, KFDATA, &kfdata);

      /* If one doesn't exist, then create it */
      if (kfdata == NULL)
      {
	 InitChunkAs3ds(&kfdata, KFDATA);
	 AddChildOrdered3ds(db->topchunk, kfdata);
      }

      /* Find the KFHDR */
      FindChunk3ds(kfdata, KFHDR, &kfhdr);

      /* If a KFHDR doesn't exist, then create it */
      if (kfhdr == NULL)
      {

	 d = InitChunkAndData3ds(&kfhdr, KFHDR);
	 d->filename = strdup("");
	 d->animlength = 30; /* Just a default */
	 AddChildOrdered3ds(kfdata, kfhdr);
	 
      } else /* Else, just get its data */
      {
	 d = ReadChunkData3ds(kfhdr);
      }

      /* Set the revision level */
      switch(release)
      {
      case Release13ds:
	 d->revision = 1;
	 break;
      case Release23ds:
	 d->revision = 2;
	 break;
      case Release33ds:
	 d->revision = 5;
	 break;
      case ReleaseNotKnown3ds:
	 d->revision = 6;
	 break;
      }
   }
}

releaselevel3ds GetDatabaseRelease3ds(database3ds *db)
{
   dbtype3ds dtype;
   
   dtype = GetDatabaseType3ds(db);

   switch(dtype)
   {
   case MeshFile:
      return GetM3dMagicRelease3ds(db);
   case ProjectFile:
      return GetMeshRelease3ds(db);
   case MaterialFile:
      return Release33ds;
   default:
      return ReleaseNotKnown3ds;
   }
}

void PutDatabaseRelease3ds(database3ds *db, releaselevel3ds rev)
{
   dbtype3ds dtype;

   dtype = GetDatabaseType3ds(db);

   switch(dtype)
   {
   case MeshFile:
      PutM3dMagicRelease3ds(db, rev);
   case ProjectFile:
      PutMeshRelease3ds(db, rev);
      PutKfRelease3ds(db, rev);
      break;
   default:
      break;
   }
}
