/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "fcontainer.h"

#define MAX_PATH 1024

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

#ifndef _fC_READONLY_
void CSP_Rnd(unsigned short& v);
void xDlgShowProgress(int v,int max_v);
#endif

/* --------------------------- DEFINITION SECTION --------------------------- */

const int fC_BLOCK_SIZE = 64;

fContainer::fContainer(void)
{
	fname = NULL;
	fpath = NULL;

	indexSize = 0;
	index = NULL;
}

fContainer::~fContainer(void)
{
	if(fname) free(fname);
	if(fpath) free(fpath);

	if(index) delete index;
}

void fContainer::load_idx(char* idx_name)
{
	XStream fh(idx_name,XS_IN);
	load_idx(fh);
}

void fContainer::save_idx(char* idx_name)
{
	int buf_sz,buf_sz0;
	char* buf;

	XBuffer* buf0;

	if(!index) return;

	buf_sz0 = indexSize * sizeof(int) + sizeof(int) + strlen(fname) + 1;

	buf = new char[buf_sz0 + sizeof(int)];

	buf0 = new XBuffer(buf_sz0);
	*buf0 < indexSize;
	buf0 -> write((char*)index,indexSize * sizeof(int));
	*buf0 < fname;

	buf_sz = ZIP_compress(buf,buf_sz0 + sizeof(int),buf0 -> address(),buf_sz0);

	XStream fh(idx_name,XS_OUT);
	fh.write(buf,buf_sz);
	fh.close();

	delete buf;
	delete buf0;
}

void fContainer::load_idx(XStream& fh)
{
	int buf_sz,buf_sz0;
	char* buf;
	XBuffer* buf0;

	if(index) delete index;

	buf_sz = fh.size();
	buf = new char[buf_sz];

	fh.read(buf,buf_sz);
	fh.close();

	buf_sz0 = ZIP_GetExpandedSize(buf);
	buf0 = new XBuffer(buf_sz0);

	ZIP_expand(buf0 -> address(),buf_sz0,buf,buf_sz);

	*buf0 > indexSize;
	index = new int[indexSize];
	buf0 -> read((char*)index,indexSize * sizeof(int));

	set_file(buf0 -> address() + buf0 -> tell());

	delete buf;
	delete buf0;
}

#ifndef _fC_READONLY_
void fContainer::write(char* p,int len)
{
	int i,idx,sz,offs;

	unsigned offs_quant;
	unsigned short rnd_val;

	if(index) delete index;

	indexSize = (len / fC_BLOCK_SIZE + 1) * 2;
	index = new int[indexSize];

	for(i = 0; i < indexSize; i ++)
		index[i] = -1;

	XStream fh(fname,XS_OUT | XS_APPEND | XS_NOREPLACE);

	i = idx = offs = 0;

	offs_quant = fh.size() / (indexSize/2 + 2);

	while(idx < len){
		CSP_Rnd(rnd_val);

		sz = fC_BLOCK_SIZE;

		if(sz >= len - idx)
			sz = len - idx;

//		sz = (idx <= len - fC_BLOCK_SIZE) ? fC_BLOCK_SIZE : (len - idx);

		CSP_Rnd(rnd_val);
		offs = offs_quant * i + rnd_val * offs_quant / 0xFFFF; 

		fh.seek(offs,XS_BEG);
		fh.write(p + idx,sz);

		index[i * 2] = offs;
		index[i * 2 + 1] = sz;

		i ++;
		idx += sz;

		xDlgShowProgress(idx,len);
	}

	fh.close();
}
#endif

void fContainer::read(char*& p,int& len)
{
	int i,idx;
	static char str[MAX_PATH];

	if(!fpath)
		sprintf(str,"%s",fname);
	else
		sprintf(str,"%s\\%s",fpath,fname);


	XStream fh(str,XS_IN);

	len = 0;
	for(i = 1; i < indexSize; i += 2){
		if(index[i] != -1)
			len += index[i];
	}

	p = new char[len];

	idx = 0;
	for(i = 0; i < indexSize; i += 2){
		if(index[i] != -1){
			fh.seek(index[i],XS_BEG);
			fh.read(p + idx,index[i + 1]);
			idx += index[i + 1];
		}
	}

	fh.close();
}
