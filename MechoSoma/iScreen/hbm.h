
#ifndef __HBM_H__
#define __HBM_H__

struct hbm_Image;

#define HBM_FRAME_ZIP_COMPRESSED	0x01

struct hbm_Frame 
{
	int flags;

	int X;
	int Y;

	int SizeX;
	int SizeY;

	int dataSize;
	char* dataPtr;

	hbm_Image* ownerPtr;

	void load(XStream& fh);
	void save(XStream& fh);

	void free(void);
	void redraw(int mode = -1);

	void crop(void);
	void compress(int mode);

	hbm_Frame(void);
	~hbm_Frame(void);
};

// flags...
#define HBM_PALETTE	0x01

struct hbm_Image
{
	int X;
	int Y;

	int SizeX;
	int SizeY;

	int flags;
	int colors;

	int numFrames;
	hbm_Frame** frameTable;
	hbm_Frame** scr_frameTable;

	int paletteSize;
	char* palettePtr;

	unsigned palette16[256];

	void load(const char* fname,int conv = 0);
	void load(XStream& fh,int conv = 0);
	void save(const char* fname);
	void free(void);

	void convert_frameRGBA(int x,int y,int frame,void* buf,int pitch,int shift_r,int shift_g,int shift_b,int alpha_shift);

	void build_frames(void);
	void free_frames(void);

	void compress(int mode,int scr_mode = 1);

	hbm_Image(void);
	~hbm_Image(void);
};

#endif // __HBM_H__
