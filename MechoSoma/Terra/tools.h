#ifndef __TOOLS_H__
#define __TOOLS_H__
#include "TERRA.H"

extern int MosaicTypes[8];

struct BitMap {
	unsigned short sx,sy;
	int sz;
	unsigned char* data;
	unsigned char* palette;
	int force,mode,border,copt;
	int mosaic;
	int size,level,Kmod,modeC2H,alpha;
	int xoffset,yoffset;
	double A11, A12, A21, A22;
	double X, Y;

		BitMap(int _mosaic = 0);

	void load(char* name);
	void convert(void);
	void place(char* name,int x, int y,int _force,int _mode,int _border = 0,int _level = 0,int _size = 0,int Kmod=0, int _modeC2H = 0,int alpha=90, int ter_type=-1);
	inline int getDelta(int x,int y,int delta){
		x += xoffset;
		y += yoffset;
		x+=H_SIZE;
		y+=V_SIZE;
		int x1,y1;
		x1 = A11*x + A12*y + X;
		y1 = A21*x + A22*y + Y;
		while (x<0){ x+=sx;}
		while (y<0){ y+=sy;}
		if(!mode) return delta*(64 - data[(y1%sy)*sx + (x1%sx)])/64; 
		return delta + force*(64 - data[(y1%sy)*sx + (x1%sx)])/64;
		//if(!mode) return delta*(64 - data[(y%sy)*sx + (x%sx)])/64;
		//return delta + force*(64 - data[(y%sy)*sx + (x%sx)])/64;
	}
	inline int getType(int x,int y){ 
		x += xoffset;
		y += yoffset;
		x+=H_SIZE;
		y+=V_SIZE;
		int x1,y1;
		x1 = A11*x + A12*y + X;
		y1 = A21*x + A22*y + Y;
		while (x<0){ x+=sx;}
		while (y<0){ y+=sy;}
		return MosaicTypes[data[(y1%sy)*sx + (x1%sx)]%8]; 
		//return MosaicTypes[data[(y%sy)*sx + (x%sx)]%8]; 
	}
	inline int getColor(int x, int y){
		x += xoffset;
		y += yoffset;
		x+=H_SIZE;
		y+=V_SIZE;
		int x1,y1;
		x1 = A11*x + A12*y + X;
		y1 = A21*x + A22*y + Y;
		while (x<0){ x+=sx;}
		while (y<0){ y+=sy;}
		return data[(y1%sy)*sx + (x1%sx)]; 
	};

	void set_alpha(double alpha, int X0, int Y0){
		X0+=H_SIZE;
		Y0+=V_SIZE;
		alpha-=90;
		A11 = A22 = cos(alpha*3.1415926535/180);
		A21 = -(A12 = sin(alpha*3.1415926535/180));
		X = X0 - A11*X0 - A12*Y0;
		Y = Y0 - A21*X0 - A22*Y0;
	};

};

extern BitMap placeBMP;
extern BitMap mosaicBMP;
extern int curBmpIndex;

extern void PutTrackPoints(int num_track);

#endif // __TOOLS_H__
