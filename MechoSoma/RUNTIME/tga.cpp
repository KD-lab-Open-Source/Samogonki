
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "tga.h"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

tgaFile::tgaFile(int sx,int sy,int bpp,void* buf,void* id)
{
	char* img_ptr,*id_ptr;

	SizeX = sx;
	SizeY = sy;
	bitsPerPixel = bpp;

	img_ptr = (char*)buf;
	imageSize = SizeX * SizeY * (bitsPerPixel / 8);
	image = new char[imageSize];
	memcpy(image,img_ptr,imageSize);

	id_ptr = (char*)id;
	strcpy(ID,id_ptr);
}

tgaFile::tgaFile(void)
{
	SizeX = SizeY = 0;
	bitsPerPixel = 24;
	imageSize = 0;
	image = NULL;
	memset(ID,256,0);
}

tgaFile::~tgaFile(void)
{
	if(image) delete image;
}

void tgaFile::write(char* fname)
{
	unsigned char header[18];

	XStream fh(fname,XS_OUT);

	memset(header,0,18);
	header[0] = strlen(ID);
	header[2] = 2;

	header[13] = (SizeX >> 8) & 0xFF;
	header[12] = SizeX & 0xFF;

	header[15] = (SizeY >> 8) & 0xFF;
	header[14] = SizeY & 0xFF;
	header[16] = bitsPerPixel;
	header[17] = 0x20;

	fh.write(header,18);
	if(header[0]) fh.write(ID,strlen(ID));

	fh.write(image,imageSize);
	fh.close();
}

void mchLoadTGA(XStream& fh,void** p,int& sx,int& sy,int& colors)
{	
	int i,flags,idx,ssx;
	unsigned char header[18];

	char* data;

	fh.read(header,18);

	if(header[0]) // Length of Image ID field
		fh.seek(header[0],XS_CUR);

	if(header[1]) // Color map type (0 is no color map)
		ErrH.Abort("Paletted TGA not supported");

	if(header[2] != 2) // TGA file type
		ErrH.Abort("Compressed TGA not supported");

	sx = header[12] + (header[13] << 8); 
	sy = header[14] + (header[15] << 8); 

	colors = header[16];
	flags = header[17];

	ssx = sx * colors/8;

	data = new char[ssx * sy];
	if(!(flags & 0x20)){
		idx = (sy - 1) * ssx;
		for(i = 0; i < sy; i ++){
			fh.read(data + idx,ssx);
			idx -= ssx;
		}
	}
	else
		fh.read(data,ssx * sy);

	*p = data;
}
