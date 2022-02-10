
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "HFONT.H"
#include "ACI_SCR.H"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);

/* --------------------------- DEFINITION SECTION --------------------------- */

static char* HFntSign = "HFNT 1.01";

HFont::HFont(int sx,int sy,int sc,int ec)
{
	SizeX = sx;
	SizeY = sy;
	Flags = 0;
	StartChar = sc;
	EndChar = ec;
	alloc_mem();
}

HFont::~HFont(void)
{
	free_mem();
}

void HFont::alloc_mem(void)
{
	NumChars = EndChar - StartChar + 1;
	data = new HChar*[NumChars];
	data_heap = new char[dataHeapSize];
	mem_heap = new char[memHeapSize];
	if(Flags & FONT_COLORS)
		memset(mem_heap,0,memHeapSize);
	else
		memset(mem_heap,HFONT_NULL_LEVEL,memHeapSize);
}

void HFont::free_mem(void)
{
	delete data;
	delete data_heap;
	delete mem_heap;
	Flags &= ~FONT_LOADED;
}

HChar::HChar(int sx,int sy,int fl)
{
	SizeX = sx;
	SizeY = sy;
	Size = sx * sy;

	Flags = fl;
}

void HChar::init(int sx,int sy,int fl)
{
	SizeX = sx;
	SizeY = sy;
	Size = sx * sy;

	Flags = fl;
}

HChar::~HChar(void)
{
}

int HChar::null_check(void)
{
	int i;
	for(i = 0; i < Size; i ++)
		if(HeightMap[i] != HFONT_NULL_LEVEL)
			return 0;
	return 1;
}

void HChar::put(int x,int y)
{
//	  put_buf(x,y,SizeX,SizeY,HeightMap,HeightMap,0,1);
}

void HFont::put_char(int x,int y,int ch)
{
	data[ch] -> put(x,y);
}

void HFont::save(char* fname)
{
	int i;
	XStream fh(fname,XS_OUT);
	fh < HFntSign < SizeX < SizeY < StartChar < EndChar;
	for(i = 0; i < NumChars; i ++){
		fh < data[i] -> SizeX < data[i] -> SizeY < data[i] -> Flags;
		if(!(data[i] -> Flags & NULL_HCHAR))
			fh.write(data[i] -> HeightMap,data[i] -> SizeX * data[i] -> SizeY);
	}
	fh.close();
}

void HFont::load(char* fname)
{
	short sx,sy;
	int i,sz = strlen(HFntSign),fl,ch_offs = 0;
	char* str = new char[sz + 1];
	str[sz] = 0;

	if(Flags & FONT_LOADED)
		free_mem();

	scan_chars(fname);

	XStream fh;
	acsOpenResource(fname,fh);

	fh.read(str,strlen(HFntSign));
	if(strcmp(HFntSign,str)){
		ErrH.Abort("Bad HFont signature...");
	}
	delete[] str;

	fh > SizeX > SizeY > StartChar > EndChar;
	alloc_mem();

	for(i = 0; i < NumChars; i ++){
		fh > sx > sy > fl;
		if(!i) fl &= ~NULL_HCHAR;
		data[i] = (HChar*)(data_heap + i * sizeof(HChar));
		data[i] -> init(sx,sy,fl);

		if(!i){
			data[0] -> HeightMap = (unsigned char*)(mem_heap + ch_offs);
			ch_offs += sx * sy;
		}

		if(!(fl & NULL_HCHAR) && i){
			data[i] -> HeightMap = (unsigned char*)(mem_heap + ch_offs);
			fh.read(data[i] -> HeightMap,data[i] -> SizeX * data[i] -> SizeY);
			ch_offs += sx * sy;
		}
		else {
			data[i] -> HeightMap = data[0] -> HeightMap;
		}

		data[i] -> calcHLimits();
		data[i] -> calcOffsets();
	}
	fh.close();

	data[32] -> LeftOffs = data[32] -> RightOffs = (data[32] -> SizeX - data[95] -> SizeX)/2;

	Flags |= FONT_LOADED;
}

void HFont::scan_chars(char* fname)
{
	short sx,sy;
	int i,sz,fl,num_ch,mem_sz;

	XStream fh;
	acsOpenResource(fname,fh);

	sz = strlen(HFntSign);
	num_ch = mem_sz = 0;

	fh.seek(sz,XS_CUR);
	fh > SizeX > SizeY > StartChar > EndChar;
	NumChars = EndChar - StartChar + 1;

	for(i = 0; i < NumChars; i ++){
		fh > sx > sy > fl;
		if(!i) fl &= ~NULL_HCHAR;
		if(!(fl & NULL_HCHAR) && i){
			fh.seek(sx * sy,XS_CUR);
			mem_sz += sx * sy;
			num_ch ++;
		}
		else {
			if(!i){
				mem_sz += sx * sy;
				num_ch ++;
			}
		}
	}
	fh.close();

//	  dataHeapSize = num_ch * sizeof(HChar);
	dataHeapSize = NumChars * sizeof(HChar);
	memHeapSize = mem_sz;
}

void HChar::calcHLimits(void)
{
	int i,h,sz = SizeX * SizeY;
	MaxHeight = MinHeight = HeightMap[0];
	for(i = 1; i < sz; i ++){
		h = HeightMap[i];
		if(h < MinHeight)
			MinHeight = h;
		if(h > MaxHeight)
			MaxHeight = h;
	}
}

void HChar::calcOffsets(void)
{
	int i,j,offs,index = 0,null_lev;
	unsigned char* ptr;

	RightOffs = LeftOffs = SizeX/2;
	null_lev = (Flags & FONT_COLORS) ? 0 : HFONT_NULL_LEVEL;

	for(i = 0; i < SizeY; i ++){
		ptr = HeightMap + index;
		offs = SizeX/3;
		for(j = 0; j < SizeX; j ++){
			if(ptr[j] != null_lev){
				offs = j;
				break;
			}
		}
		if(offs < LeftOffs)
			LeftOffs = offs;

		offs = SizeX/3;
		for(j = SizeX - 1; j >= 0; j --){
			if(ptr[j] != null_lev){
				offs = SizeX - j - 1;
				break;
			}
		}
		if(offs < RightOffs)
			RightOffs = offs;

		index += SizeX;
	}
}

