#include "_xtool.h"

struct TGA_Header
{
	long mode;		// = 0x20000UL; ...
	long unknown[2];	// = 0; ...

	unsigned short SizeX;
	unsigned short SizeY;

	char colors;		// 24;		32; ...
	char flag;		// 0x20;	0x08; ...

	void Save(XStream& fh);
	void Load(XStream& fh);

	TGA_Header(void){ mode = 0x20000UL; unknown[0] = unknown[1] = 0; colors = 32; flag = 0x08; }
};

struct TGA_Image
{
	TGA_Header* header;

	int imageSize;
	unsigned char* imageData;	// BGRA data...

	int Load(char* fname);
	void Save(char* fname);
	void Save(char* fname,int x,int y,int bpp,void *buf);

	TGA_Image(void);
	~TGA_Image(void);
};

/*struct TGA_Header
{
	long mode;		// = 0x20000UL; ...
	long unknown[2];	// = 0; ...

	unsigned short SizeX;
	unsigned short SizeY;

	char colors;		// 24;		32; ...
	char flag;		// 0x20;	0x08; ...

	void Save(XStream& fh);
	void Load(XStream& fh);

	TGA_Header(void){ mode = 0x20000UL; unknown[0] = unknown[1] = 0; colors = 32; flag = 0x08; }
};

struct TGA_Image
{
	TGA_Header* header;

	int imageSize;
	unsigned char* imageData;	// BGRA data...

	void Load(char* fname);
	void Save(char* fname);

	TGA_Image(void);
};*/