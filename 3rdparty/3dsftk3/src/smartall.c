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


/*

    Smart memory allocator

    Evolved over several years, starting with the initial SMARTALLOC
    code  for  AutoSketch in 1986, guided by the Blind Watchbreaker,
    John Walker.  Isolated in this general-purpose form in September
    of 1989.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NDEBUG
void assertfunc(int ex, char *msg, char *file, int line);
#define _myassert(ex, msg) assertfunc((ex), msg, __FILE__, __LINE__)
#define myassert(ex, msg) _myassert(ex, msg)
#else
#define _myassert(ex, msg)
#define myassert(ex,msg)
#endif

/*LINTLIBRARY*/
#ifdef lint
#include <memory.h>
extern char *sprintf();
#endif

#ifdef SMARTALLOC

typedef enum {False = 0, True = 1} Boolean;
typedef unsigned short ushort;

void sm_free(void *fp);
void *sm_malloc(const char *fname, int lineno, unsigned int nbytes);
void *sm_calloc(const char *fname, int lineno, unsigned int nelem, unsigned int elsize);
void *sm_realloc(const char *fname, int lineno, void *ptr, unsigned int size);
void *actuallymalloc(unsigned int size);
void *actuallycalloc(unsigned int nelem, unsigned int elsize);
void *actuallyrealloc(void *ptr, unsigned int size);
void actuallyfree(void *cp);
void sm_tag(char *tag);
void sm_untag();
void sm_dump(Boolean bufdump);
void sm_static(int mode);


#define EOS      '\0'              /* End of string sentinel */
#define V    (void)

#ifdef min 
#undef min
#endif

#define min(a,b) ((a)<(b) ? (a) : (b))

/*  Queue data structures  */

/*  General purpose queue  */

typedef struct queue {
    struct queue *qnext,       /* Next item in queue */
             *qprev;       /* Previous item in queue */
} queue;

/*  Memory allocation control structures and storage.  */

typedef struct {
    queue abq;             /* Links on allocated queue */
    unsigned ablen;        /* Buffer length in bytes */
    const char *abfname;         /* File name pointer */
    char *abtag;           /* Comment tag */
    ushort ablineno;       /* Line number of allocation */ 
} abufhead;

static queue abqueue = {    /* Allocated buffer queue */
    &abqueue, &abqueue
};

static Boolean bufimode = False;   /* Buffers not tracked when True */

/*  Queue functions  */

static void qinsert();
static queue *qdchain();


static const abtagstacksize = 40;
static int   abtagstackpos = 0;
static char* abtagstack[40] = {NULL};


/*  SMALLOC  --  Allocate buffer, enqueing on the orphaned buffer
         tracking list.  */

static void *smalloc(const char *fname, int lineno, unsigned nbytes)
{
    char *buf;

    /* Note:  Unix  MALLOC  actually  permits  a zero length to be
       passed and allocates a valid block with  zero  user  bytes.
       Such  a  block  can  later  be expanded with realloc().  We
           disallow this based on the belief that it's better to  make
       a  special case and allocate one byte in the rare case this
       is desired than to miss all the erroneous occurrences where
       buffer length calculation code results in a zero.  */

    nbytes += sizeof(abufhead) + 1;
    if ((buf = malloc(nbytes)) != NULL) {
       /* Enqueue buffer on allocated list */
       qinsert(&abqueue, (queue *) buf);
       ((abufhead *) buf)->ablen = nbytes;
       ((abufhead *) buf)->abtag = abtagstack[abtagstackpos];
       ((abufhead *) buf)->abfname = bufimode ? NULL : fname;
       ((abufhead *) buf)->ablineno = lineno;
       /* Emplace end-clobber detector at end of buffer */
       buf[nbytes - 1] = (((long) buf) & 0xFF) ^ 0xC5;
       buf += sizeof(abufhead);  /* Increment to user data start */
    }
    return buf;
}

/*  SM_FREE  --  Update free pool availability.  FREE is never called
         except  through  this interface or by actuallyfree().
         free(x)  is  defined  to  generate  a  call  to  this
         routine.  */

void sm_free(void *fp)
{
    queue *qp;
    char *cp = fp;

    if (cp == NULL) return; /* this is acceptable behavior for now */
    
    myassert(cp != NULL, "sm_free: cannot release a NULL buffer");    /* Better not release a null buffer, guy! */

    cp -= sizeof(abufhead);
    qp = (queue *) cp;

    /* The following assertions will catch virtually every release
           of an address which isn't an allocated buffer. */

    myassert(qp->qnext->qprev == qp, "sm_free: cannot release an invalid pointer");   /* Validate queue links */
    myassert(qp->qprev->qnext == qp, "sm_free: cannot release an invalid poitner");

    /* The following assertion detects storing off the  end  of  the
       allocated  space in the buffer by comparing the end of buffer
       checksum with the address of the buffer.  */

    myassert(((unsigned char *) cp)[((abufhead *) cp)->ablen - 1] ==
       ((((long) cp) & 0xFF) ^ 0xC5), "sm_free: freed memory buffer overwrote allocated boundries");

    V qdchain(qp);

    /* Now we wipe the contents of  the  just-released  buffer  with
           "designer  garbage"  (Duff  Kurland's  phrase) of alternating
       bits.  This is intended to ruin the day for any miscreant who
           attempts to access data through a pointer into storage that's
       been previously released. */

    V memset(cp, 0xAA, (int) ((abufhead *) cp)->ablen);

    free(cp);
}

/*  SM_MALLOC  --  Allocate buffer.  NULL is returned if no memory
           was available.  */

void *sm_malloc(const char *fname, int lineno, unsigned int nbytes)
{
    char *buf;

    if ((buf = smalloc(fname, lineno, nbytes)) != NULL) {

       /* To catch sloppy code that assumes  buffers  obtained  from
          malloc()  are  zeroed,  we  preset  the buffer contents to
              "designer garbage" consisting of alternating bits.  */

       V memset(buf, 0x55, (int) nbytes);
    }
    return buf;
}

/*  SM_CALLOC  --  Allocate an array and clear it to zero.  */

void *sm_calloc(const char *fname, int lineno, unsigned int nelem, unsigned int elsize)
{
    char *buf;

    if ((buf = smalloc(fname, lineno, nelem * elsize)) != NULL) {
       V memset(buf, 0, (int) (nelem * elsize));
    }
    return buf;
}

/*  SM_REALLOC  --  Adjust the size of a  previously  allocated  buffer.
                    Note  that  the trick of "resurrecting" a previously
            freed buffer with realloc() is NOT supported by this
            function.   Further, because of the need to maintain
            our control storage, SM_REALLOC must always allocate
            a  new  block  and  copy  the data in the old block.
            This may result in programs which make heavy use  of
            realloc() running much slower than normally.  */

void *sm_realloc(const char *fname, int lineno, void *ptr, unsigned int size)
{
    unsigned osize;
    char *buf;

    myassert(size > 0, "sm_realloc: must realloc more than 0 bytes");

    /*  If  the  old  block  pointer  is  NULL, treat realloc() as a
       malloc().  SVID is silent  on  this,  but  many  C  libraries
       permit this.  */

    if (ptr == NULL)
       return sm_malloc(fname, lineno, size);

    /* If the old and new sizes are the same, be a nice guy and just
       return the buffer passed in.  */

    osize = ((abufhead *) ptr)->ablen -
        (sizeof(abufhead) + 1);
    if (size == osize) {
       return ptr;
    }

    /* Sizes differ.  Allocate a new buffer of the  requested  size.
           If  we  can't  obtain  such a buffer, act as defined in SVID:
       return NULL from  realloc()  and  leave  the  buffer  in  PTR
       intact.  */

    if ((buf = smalloc(fname, lineno, size)) != NULL) {
       V memcpy(buf, ptr, (int) min(size, osize));
       /* If the new buffer is larger than the old, fill the balance
              of it with "designer garbage". */
       if (size > osize) {
          V memset(buf + osize, 0x55, (int) (size - osize));
       }

       /* All done.  Free and dechain the original buffer. */

       sm_free(ptr);
    }
    return buf;
}

/*  ACTUALLYMALLOC  --  Call the system malloc() function to obtain
            storage which will eventually be released
            by system or library routines not compiled
            using SMARTALLOC.  */

void *actuallymalloc(unsigned int size)
{
    return malloc(size);
}

/*  ACTUALLYCALLOC  --  Call the system calloc() function to obtain
            storage which will eventually be released
            by system or library routines not compiled
            using SMARTALLOC.  */

void *actuallycalloc(unsigned int nelem, unsigned int elsize)
{
    return calloc(nelem, elsize);
}

/*  ACTUALLYREALLOC  --  Call the system realloc() function to obtain
             storage which will eventually be released
             by system or library routines not compiled
             using SMARTALLOC.  */

void *actuallyrealloc(void *ptr, unsigned int size)
{
    return realloc(ptr, size);
}

/*  ACTUALLYFREE  --  Interface to system free() function to release
              buffers allocated by low-level routines. */

void actuallyfree(void *cp)
{
    free(cp);
}

void sm_tag(char *tag)
{
   if (abtagstackpos+1 < abtagstacksize)
   {
      abtagstackpos++;
   }
   
   abtagstack[abtagstackpos] = tag;
}

void sm_untag()
{
   abtagstack[abtagstackpos] = NULL;

   if (abtagstackpos > 0) abtagstackpos--;
}

/*  SM_DUMP  --  Print orphaned buffers (and dump them if BUFDUMP is
         True). */

void sm_dump(Boolean bufdump)
{
    abufhead *ap = (abufhead *) abqueue.qnext;

    while (ap != (abufhead *) &abqueue) {

       if ((ap == NULL) ||
           (ap->abq.qnext->qprev != (queue *) ap) || 
           (ap->abq.qprev->qnext != (queue *) ap)) {
          V fprintf(stderr,
                 "\nOrphaned buffers exist.  Dump terminated following\n");
          V fprintf(stderr,
                 "  discovery of bad links in chain of orphaned buffers.\n");
          V fprintf(stderr,
                 "  Buffer address with bad links: %lx\n", (long) ap);
          break;
       }

       if (ap->abfname != NULL) {
          unsigned memsize = ap->ablen - (sizeof(abufhead) + 1);
          char errmsg[120];

	  if (ap->abtag != NULL)
	  {
	     V sprintf(errmsg,
		       "Orphaned buffer:  %6u bytes allocated at line %d of %s with tag %s\n",
		       memsize, ap->ablineno, ap->abfname, ap->abtag
		       );
	  }
	  else
	  {
	     V sprintf(errmsg,
		       "Orphaned buffer:  %6u bytes allocated at line %d of %s\n",
		       memsize, ap->ablineno, ap->abfname
		       );
	  }

	  V fprintf(stderr, "%s", errmsg);
          if (bufdump) {
	     unsigned llen = 0;
	     char *cp = ((char *) ap) + sizeof(abufhead);

	     errmsg[0] = EOS;
	     while (memsize) {
		if (llen >= 16) {
		   V strcat(errmsg, "\n");
		   llen = 0;
		   V fprintf(stderr, "%s", errmsg);
		   errmsg[0] = EOS;
		}
		V sprintf(errmsg + strlen(errmsg), " %02X",
			  (*cp++) & 0xFF);
		llen++;
		memsize--;
	     }
	     V fprintf(stderr, "%s\n", errmsg);
          }
       }
       ap = (abufhead *) ap->abq.qnext;
    }
}

/*  SM_STATIC  --  Orphaned buffer detection can be disabled  (for  such
           items  as buffers allocated during initialisation) by
           calling   sm_static(1).    Normal   orphaned   buffer
           detection  can be re-enabled with sm_static(0).  Note
           that all the other safeguards still apply to  buffers
           allocated  when  sm_static(1)  mode is in effect.  */

void sm_static(int mode)
{
    bufimode = (Boolean) (mode != 0);
}

/*  Queue manipulation functions.  */


/*  QINSERT  --  Insert object at end of queue  */

static void qinsert(queue *qhead, queue *object)
{
  myassert(qhead->qprev->qnext == qhead || qhead->qnext->qprev == qhead,
	   "qinsert: allocation structure is bad or corrupt");
  
    /* qinsert: alloc struct is bad */
     
  object->qnext = qhead;
  object->qprev = qhead->qprev;
  qhead->qprev = object;
  object->qprev->qnext = object;
}


/*  QREMOVE  --  Remove object from queue.  Returns NULL if queue empty  */

static queue *qremove(queue *qhead)
{
    queue *object;

    myassert(qhead->qprev->qnext == qhead, "qremove: allocation structure is bad or corrupt");
    myassert(qhead->qnext->qprev == qhead, "qremove: allocation structure is bad or corrupt");

    if ((object = qhead->qnext) == qhead)
       return NULL;
    qhead->qnext = object->qnext;
    object->qnext->qprev = qhead;
    return object;
}


/*  QDCHAIN  --  Dequeue an item from the middle of a queue.  Passed
         the queue item, returns the (now dechained) queue item. */

static queue *qdchain(queue *qitem)
{
    myassert(qitem->qprev->qnext == qitem, "gdchain: allocation structure is bad or corrupt");
    myassert(qitem->qnext->qprev == qitem, "qdchain: allocation structure is bad or corrupt");

    return qremove(qitem->qprev);
}
#endif
