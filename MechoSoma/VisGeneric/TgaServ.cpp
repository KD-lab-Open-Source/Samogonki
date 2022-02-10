#include "TgaServ.h"

void TGA_Header::Save(XStream& fh)
{
	fh < mode < unknown[0] < unknown[1] < SizeX < SizeY < colors < flag;
};

void TGA_Header::Load(XStream& fh)
{
	fh > mode > unknown[0] > unknown[1] > SizeX > SizeY > colors > flag;
};

TGA_Image::TGA_Image(void)
{
	imageSize = 0;
	imageData = NULL;
	header = new TGA_Header;
};

TGA_Image::~TGA_Image(void)
{
	delete header;
	if(imageData) delete[] imageData;
};

int TGA_Image::Load(char* fname)
{
	XStream fh(0);
#ifdef _MECHOSOMA_
	extern int m3dOpenResource(char* fname,XStream& fh);
	if(!m3dOpenResource(fname,fh)) return 0;
#else
	if(!fh.open(fname,XS_IN)) return 0;
#endif
	header -> Load(fh);

	imageSize = header -> SizeX * header -> SizeY * (header -> colors / 8);
	imageData = new unsigned char[imageSize];

	fh.read(imageData,imageSize);
	fh.close();
	return 1;
};

void TGA_Image::Save(char* fname)
{
	XStream fh(fname,XS_OUT);
	header -> Save(fh);

	fh.write(imageData,imageSize);
	fh.close();
};
void TGA_Image::Save(char* fname,int x,int y,int bpp,void *buf)
{
	XStream fh(fname,XS_OUT);
	header->SizeX=x;
	header->SizeY=y;
	header->colors=bpp;
	header->Save(fh);
	fh.write(buf,x*y*bpp/8);
	fh.close();
};
