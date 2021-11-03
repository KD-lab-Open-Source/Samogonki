#include "aci_parser.h"

#include <cstring>

int** scrStringData = NULL;

char* scrOutputDIR = "";
char* scrOutputFILE = NULL;
char* scrInputDIR = "";

scrDataBlock* scrRoot;
int scrVerboseFlag = 1;
int scrSavePacked = 1;

int scrVersion = 1;

scrDataBlock::scrDataBlock(int tp)
{
	ID = 0;
	flags = 0;
	dataSize = 1;
	dataType = tp;

	name = NULL;
	owner = NULL;
}

scrDataBlock::scrDataBlock(void)
{
	ID = 0;
	flags = 0;
	dataSize = 0;
	dataType = 0;

	dataSize0 = -1;
	name = NULL;

	owner = NULL;
	nextLevel = NULL;
}

scrDataBlock::~scrDataBlock(void)
{
	freeData();

	if(nextLevel)
		delete nextLevel;

	if(name)
		delete name;
}

void scrDataBlock::saveInfo(XStream& fh)
{
	int sz = 0;

	fh < ID < dataSize < dataType < dataSize0;
	if(name) sz = strlen(name);

	fh < sz;
	if(name) fh < name;

	if(nextLevel){
		fh < (int)1;
		scrDataBlock* p = nextLevel -> first();
		while(p){
			p -> saveInfo(fh);
			p = p -> next;
		}
	}
	else {
		fh < (int)0;
	}
}

void scrDataBlock::saveData(XStream& fh)
{
	if(dataSize){
		switch(dataType){
			case SCR_INT_DATA:
				fh.write((char*)i_dataPtr,dataSize * sizeof(int));
				break;
			case SCR_DOUBLE_DATA:
				fh.write((char*)d_dataPtr,dataSize * sizeof(double));
				break;
			case SCR_CHAR_DATA:
				fh.write(c_dataPtr,dataSize);
				break;
		}
	}
	if(nextLevel){
		scrDataBlock* p = nextLevel -> first();
		while(p){
			p -> saveInfo(fh);
			p = p -> next;
		}
	}
}

void scrDataBlock::loadInfo(XStream& fh)
{
	int i,sz = 0;
	scrDataBlock* p;

	fh > ID > dataSize > dataType > sz;

	if(scrVersion >= 2)
		fh > dataSize0;

	if(sz){
		name = new char[sz + 1];
		fh.read(name,sz);
		name[sz] = 0;
	}
	fh > sz;

	allocData();

	if(sz){
		for(i = 0; i < sz; i ++){
			p = new scrDataBlock;
			p -> loadInfo(fh);
			add(p);
		}
	}
}

void scrDataBlock::loadData(XStream& fh)
{
	if(dataSize){
		switch(dataType){
			case SCR_INT_DATA:
				fh.read((char*)i_dataPtr,dataSize * sizeof(int));
				break;
			case SCR_DOUBLE_DATA:
				fh.read((char*)d_dataPtr,dataSize * sizeof(double));
				break;
			case SCR_CHAR_DATA:
				fh.read(c_dataPtr,dataSize);
				break;
		}
	}
	if(nextLevel){
		scrDataBlock* p = nextLevel -> first();
		while(p){
			p -> loadData(fh);
			p = p -> next;
		}
	}
}

void scrDataBlock::loadInfo(XBuffer& xb)
{
	int i,sz = 0;
	scrDataBlock* p;

	xb > ID > dataSize > dataType;

	if(scrVersion >= 2)
		xb > dataSize0;

	xb > sz;

	if(sz){
		name = new char[sz + 1];
		xb.read(name,sz);
		name[sz] = 0;
	}
	xb > sz;

	allocData();

	if(sz){
		for(i = 0; i < sz; i ++){
			p = new scrDataBlock;
			p -> loadInfo(xb);
			add(p);
		}
	}
}

void scrDataBlock::loadData(XBuffer& xb)
{
	if(dataSize){
		switch(dataType){
			case SCR_INT_DATA:
				xb.read((char*)i_dataPtr,dataSize * sizeof(int));
				break;
			case SCR_DOUBLE_DATA:
				xb.read((char*)d_dataPtr,dataSize * sizeof(double));
				break;
			case SCR_CHAR_DATA:
				xb.read(c_dataPtr,dataSize);
				break;
		}
	}
	if(nextLevel){
		scrDataBlock* p = nextLevel -> first();
		while(p){
			p -> loadData(xb);
			p = p -> next;
		}
	}
}

void scrDataBlock::alloc(int tp,int sz)
{
	dataType = tp;
	dataSize = sz;

	allocData();
}

void scrDataBlock::allocData(void)
{
	freeData();
	switch(dataType){
		case SCR_INT_DATA:
			i_dataPtr = new int[dataSize];
			break;
		case SCR_DOUBLE_DATA:
			d_dataPtr = new double[dataSize];
			break;
		case SCR_CHAR_DATA:
			c_dataPtr = new char[dataSize];
			break;
	}
	flags |= SCR_ALLOC_DATA;
}

void scrDataBlock::freeData(void)
{
	if(flags & SCR_ALLOC_DATA){
		switch(dataType){
			case SCR_INT_DATA:
				delete i_dataPtr;
				break;
			case SCR_DOUBLE_DATA:
				delete d_dataPtr;
				break;
			case SCR_CHAR_DATA:
				delete c_dataPtr;
				break;
		}
		flags ^= SCR_ALLOC_DATA;
	}
}

void scrDataBlock::add(scrDataBlock* p)
{
	nextLevel->append(p);
	p -> owner = this;
}

void scrSetVerbose(int v)
{
	scrVerboseFlag = v;
}

void scrSetInputDir(char* p)
{
	scrInputDIR = strdup(p);
}

void scrSetOutputDir(char* p)
{
	scrOutputDIR = strdup(p);
}

void scrSetOutputFile(char* p)
{
	scrOutputFILE = strdup(p);
}

scrDataBlock* loadScript(const char* fname)
{
	XStream fh(fname,XS_IN);
	return loadScript(fh);
}

scrDataBlock* loadScript(XStream& fh)
{
	int sz0,sz1,type;
	char* p0,*p1;

	scrDataBlock* p = new scrDataBlock;

	fh > type;

	scrVersion = type;

	if(!type){
		fh.seek(0,XS_BEG);
		sz1 = fh.size();
		p1 = new char[sz1];
		fh.read(p1,sz1);
	}
	else {
		sz0 = fh.size() - sizeof(int);
		p0 = new char[sz0];
		fh.read(p0,sz0);

		sz1 = ZIP_GetExpandedSize(p0);
		p1 = new char[sz1];
		ZIP_expand(p1,sz1,p0,sz0);

		delete p0;
	}

	XBuffer buf(p1,sz1);

	p -> loadInfo(buf);
	p -> loadData(buf);

	fh.close();
	delete p1;

	return p;
}

scrDataBlock* parseScript(char* fname,char *set_name)
{
	return NULL;
}

void saveScript(char* fname,scrDataBlock* p)
{
	int sz0,sz1,type = 2;
	char* p0,*p1;

	XStream fh(fname,XS_OUT);
	p -> saveInfo(fh);
	p -> saveData(fh);
	fh.close();

	if(scrSavePacked){
		fh.open(fname,XS_IN);
		sz0 = fh.size();
		p0 = new char[sz0];
		fh.read(p0,sz0);
		fh.close();

		p1 = new char[sz0];
		sz1 = ZIP_compress(p1,sz0,p0,sz0);
		fh.open(fname,XS_OUT);
		fh < type;
		fh.write(p1,sz1);
		fh.close();

		delete p0;
		delete p1;
	}
}
