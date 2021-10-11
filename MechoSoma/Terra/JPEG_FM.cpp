#include "stdafx.h"

#include <setjmp.h>
#ifdef _MECHOSOMA_
#include "Mechosoma.h"
#endif

#include <stdio.h>
#include "_xtool.h"
#define XMD_H
extern "C" {
#include "jpeglib.h"
}


typedef struct {
  jpeg_source_mgr pub;	/* public fields */

  XStream * infile;
  JOCTET * buffer;		/* start of buffer */
  boolean start_of_file;	/* have we gotten any data yet? */
} my_source_mgr;


#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */


static void init_source (j_decompress_ptr cinfo)
{
  my_source_mgr * src = (my_source_mgr *) cinfo->src;

  src->start_of_file = TRUE;
}



static boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  my_source_mgr * src = (my_source_mgr *) cinfo->src;
  size_t nbytes;

//  nbytes = JFREAD(src->infile, src->buffer, INPUT_BUF_SIZE);
  nbytes = src->infile->read(src->buffer, INPUT_BUF_SIZE);


  if (nbytes <= 0) {
    if (src->start_of_file)	/* Treat empty input file as fatal error */
//      ERREXIT(cinfo, JERR_INPUT_EMPTY);
//    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_file = FALSE;

  return TRUE;
}



static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_source_mgr * src = (my_source_mgr *) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}



static void term_source (j_decompress_ptr cinfo)
{
	//my_source_mgr * src = (my_source_mgr *) cinfo->src;
	//src->infile->close();
}


void my_jpeg_stdio_src (j_decompress_ptr cinfo, XStream * Xfile)//char* fname)//FILE * infile)
{
  my_source_mgr * src;

  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));
    src = (my_source_mgr *) cinfo->src;
    src->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * sizeof(JOCTET));
  }

  src = (my_source_mgr *) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
/*#ifdef _MECHOSOMA_
  vmapOpenResource(fname,src->infile);
#else
  src->infile.open(fname,XS_IN);// = infile;
#endif*/
  src->infile=Xfile;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}



extern JSAMPLE * image_buffer;	
extern int image_height;	
extern int image_width;		

struct my_error_mgr {
  struct jpeg_error_mgr pub;	

  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
}

unsigned short* loadJPG(XStream *Xfile, int &IMGwidth, int &IMGheight)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;		
	int row_stride;		

	XStream fileExsistin(0);
/*#ifdef _MECHOSOMA_
	if(!vmapOpenResource((char*)fileName, fileExsistin) ) {
#else
	if(!fileExsistin.open(fileName, XS_IN) ) {
#endif
		fileExsistin.close();
		return 0;
	}
	fileExsistin.close();
*/
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}
	jpeg_create_decompress(&cinfo);

	//jpeg_stdio_src(&cinfo, infile);
	extern void my_jpeg_stdio_src (j_decompress_ptr cinfo, XStream * Xfile);
	my_jpeg_stdio_src(&cinfo, Xfile);

	(void) jpeg_read_header(&cinfo, TRUE);

	(void) jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	IMGwidth=cinfo.output_width;
	IMGheight=cinfo.output_height;
	unsigned short* IMG=new unsigned short[IMGwidth*IMGheight];

	int i,j=0;
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* p=buffer[0];
		for (i=0; i< cinfo.output_width; i++){
			unsigned short col;
			col=(*p++ <<8)&0x0F800;
			col+=(*p++ <<3)&0x7E0;
			col+=(*p++ >>3)&0x1F;
			IMG[j]=col;
			j++;
		}
	}
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return IMG;
}
