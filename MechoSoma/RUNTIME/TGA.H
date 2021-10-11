
#ifndef __TGA_H__
#define __TGA_H__

/*
	TGA file header info

	Byte 0 - Length of Image ID field
	Byte 1 - Color map type (0 is no color map)
	Byte 2 - TGA file type:
		2 - Uncompressed True-Color Image
		10 - Run-length compressed True-Color Image

	Byte 3 - Index of first color map entry LSB
	Byte 4 - Index of first color map entry MSB
	Byte 5 - Color map length LSB
	Byte 6 - Color map legth MSB
	Byte 7 - Color map size
	Bytes 8,9 - x origin
	Bytes 10,11 - y origin
	Bytes 12 - 15 - width and height
	Byte 16 - bits/pixel
	Byte 17 - flags
		0x28 - Data starts at top left, 8 bits Alpha
		0x20 - Data starts at top left, 0 bits Alpha

	[up to 255 chars] - TGA file Image ID field
*/

struct tgaFile
{
	int SizeX;
	int SizeY;

	int bitsPerPixel;

	char ID[256];

	int imageSize;
	char* image;

	void read(char* fname);
	void write(char* fname);

	tgaFile(int sx,int sy,int bpp,void* buf,void* id);
	tgaFile(void);
	~tgaFile(void);
};

void mchLoadTGA(XStream& fh,void** p,int& sx,int& sy,int& colors);

#endif	// __TGA_H__
