
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "aci_parser.h"
#include "math_exp.h"

#include "script_err.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */

struct scrKeyword
{
	int ID;
	char* name;

	int dataType;
	int dataSize;

	void init(int id,const char* p,int tp,int sz);
	void init(const char* p,int sz);

	void initName(const char* p);
	void initID(const char* p);
	void initData(const char* p);

	void dump(XStream& fh);

	void* list;
	scrKeyword* prev;
	scrKeyword* next;

	scrKeyword(void);
	~scrKeyword(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int numStrings;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int preprocessScript(const char* fname,int& outsz,char** outbuf);
void setupScript(const char* fname,int& outsz,char** outbuf);
void finitScript(void);

void prepareBuffer(char* ptr,int sz);

void initStringData(char* ptr,int sz,int mode = 1);
int getStringNumber(int index);
void freeStringData(void);

int parseBuffer(const char* fname,int sz = 0,int file_flag = 1);
scrKeyword* getKeyword(const char* ptr);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define SCR_BEG_BLOCK_ID	-1
#define SCR_END_BLOCK_ID	-2

static const char* dataID[] = { "void", "int", "double", "char" };

xtList<scrKeyword>* scrKeywordList = NULL;
int** scrStringData = NULL;

const char* scrOutputDIR = ".\\";
const char* scrOutputFILE = NULL;
const char* scrInputDIR = ".\\";

scrDataBlock* scrRoot;
int scrVerbose = 1;
int scrSavePacked = 1;

int scrVersion = 1;

scrDataBlock* parseScript(const char* fname,const char* set_name)
{
	int sz;
	char* buf;

	XBuffer name_str;

	name_str < scrInputDIR < set_name;

	setupScript((const char*)name_str,sz,&buf);

	name_str.init();
	name_str < scrInputDIR < fname;

	if(!preprocessScript((const char*)name_str,sz,&buf)) return NULL;
/*
	XStream fh("temp.txt",XS_OUT);
	fh.write(buf,sz);
	fh.close();
*/
	if(!parseBuffer(buf,sz,0)) return NULL;
	finitScript();

	return scrRoot;
}

scrDataBlock::scrDataBlock(void)
{
	ID = 0;
	dataSize = 0;
	dataSize0 = -1;
	dataType = 0;
	flags = 0;

	list = 0;

	name = NULL;
	owner = NULL;
	nextLevel = NULL;
}

scrDataBlock::scrDataBlock(int tp)
{
	ID = 0;
	dataSize = 1;
	dataType = tp;
	flags = 0;

	name = NULL;
	owner = NULL;
	nextLevel = NULL;
}

scrDataBlock::~scrDataBlock(void)
{
	scrDataBlock* p;

	freeData();
	if(nextLevel){
		p = nextLevel -> first();
		while(p){
			nextLevel -> remove(p);
			delete p;
			p = nextLevel -> first();
		}
		delete nextLevel;
	}

	if(name)
		delete name;
}

void scrDataBlock::add(scrDataBlock* p)
{
	nextLevel -> append(p);
	p -> owner = this;
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

scrKeyword::scrKeyword(void)
{
	ID = 0;
	list = NULL;
	dataType = dataSize = 0;
	name = NULL;
}

scrKeyword::~scrKeyword(void)
{
	if(name)
		delete name;
}

void scrKeyword::initID(const char* p)
{
	i_parseMathExpr(p,ID);
}

void scrKeyword::initName(const char* p)
{
	int sz = strlen(p) + 1;
	name = new char[sz];
	strcpy(name,p);
}

void scrDataBlock::initName(const char* p)
{
	int sz = strlen(p) + 1;
	name = new char[sz];
	strcpy(name,p);
}

void scrKeyword::initData(const char* p)
{
	char* p1;
	int i,sz,sz1;

	sz = strlen(p);
	for(i = 0; i < SCR_MAX_DATA_ID; i ++){
		sz1 = strlen(dataID[i]);
		if(!strncmp(p,dataID[i],sz1)){
			dataType = i;
			dataSize = 1;
			if(sz > sz1){
				if(p[sz1] == '['){
					char* expression = strdup(p + sz1 + 1);
					auto end = strchr(expression,']');
					if(end) *end = 0;
					i_parseMathExpr(expression,dataSize);
					free(expression);
				}
				else {
					if(p[sz1] == '*'){
						dataSize = -1;
					}
				}
			}
			break;
		}
	}
}

void scrKeyword::init(int id,const char* p,int tp,int sz)
{
	ID = id;
	initName(p);

	dataType = tp;
	dataSize = sz;
}

void scrKeyword::init(const char* p,int sz)
{
	int index = 0;
	while(!p[index]) if(++index >= sz) break;

	if(index >= sz) return;
	initData(p + index);

	while(p[index]) if(++index >= sz) break;
	while(!p[index]) if(++index >= sz) break;

	if(index >= sz) return;
	initName(p + index);

	while(p[index]) if(++index >= sz) break;
	while(!p[index]) if(++index >= sz) break;

	if(index >= sz) return;
	initID(p + index);
}

void setupScript(const char* fname,int& outsz,char** outbuf)
{
	int i,sz,idx;
	XStream fh;
	scrKeyword* p;
	char* line = new char[2048];

	scrKeywordList = new xtList<scrKeyword>;

	p = new scrKeyword;
	p -> init(SCR_BEG_BLOCK_ID,"{",SCR_VOID_DATA,0);
	scrKeywordList -> append(p);
	p = new scrKeyword;
	p -> init(SCR_END_BLOCK_ID,"}",SCR_VOID_DATA,0);
	scrKeywordList -> append(p);

	preprocessScript(fname,outsz,outbuf);

	idx = 0;

//	fh.open("temp.txt",XS_OUT);
//	fh.write(*outbuf,*outsz);
//	fh.close();

	while(idx < outsz){
		i = 0;
		memset(line,0,2048);
		do {
			line[i] = outbuf[0][idx];
			idx ++;
			i ++;
		} while(idx < outsz && outbuf[0][idx] != '\n');
		if(idx >= outsz) break;

		sz = strlen(line);
		prepareBuffer(line,sz);

		p = new scrKeyword;
		p -> init(line,sz);

		if(p -> name)
			scrKeywordList -> append(p);
		else
			delete p;
	}
	delete outbuf[0];

	delete[] line;
}

void finitScript(void)
{
	scrKeyword* p;
	p = scrKeywordList -> first();
	while(p){
		scrKeywordList -> remove(p);
		delete p;
		p = scrKeywordList -> first();
	}
	delete scrKeywordList;
	scrKeywordList = NULL;
}

scrKeyword* getKeyword(const char* ptr)
{
	scrKeyword* p = scrKeywordList -> first();
	while(p){
		if(!strcmp(p -> name,ptr)) return p;
		p = p -> next;
	}
	return NULL;
}

#define NEXT_WORD(ptr,i)	while(ptr[i]) if(++i >= sz) break;	\
				if(i >= sz) break;			\
				while(!ptr[i]) if(++i >= sz) break;	\
				if(i >= sz) break;

#define SKIP_SPACE(ptr,i)	while(!ptr[i]) if(++i >= sz) break;	\
				if(i >= sz) break;

int parseBuffer(const char* fname,int sz,int file_flag)
{
	int i,idx,dsz;
	scrDataBlock* p,*cp;
	scrKeyword* wrd;
	char* ptr;

	if(file_flag){
		XStream fh(fname,XS_IN);

		sz = fh.size();
		ptr = new char[sz];
		fh.read(ptr,sz);
		fh.close();
	}
	else {
		ptr = new char[sz];
		memcpy(ptr, fname, sz);
	}

	initStringData(ptr,sz,0);
	prepareBuffer(ptr,sz);

	scrRoot = new scrDataBlock(SCR_VOID_DATA);
	scrRoot -> allocList();

	cp = scrRoot;

	i = 0;

	while(!ptr[i]) if(++i >= sz) return 1;
	if(i >= sz) return 1;

	while(i < sz){
		wrd = getKeyword(ptr + i);
		if(!wrd){
			scriptError("Error: Undefined keyword\nString: %d\n %s",getStringNumber(i),ptr + i);
			return 0;
		}

		switch(wrd -> ID){
			case SCR_BEG_BLOCK_ID:
				cp = p;
				cp -> allocList();
				break;
			case SCR_END_BLOCK_ID:
				cp = cp -> owner;
				break;
			default:
				p = new scrDataBlock;
				p -> dataType = wrd -> dataType;

				dsz = wrd -> dataSize;
				p -> dataSize0 = wrd -> dataSize;
				if(wrd -> dataType == SCR_INT_DATA){
					if(dsz == -1){
						NEXT_WORD(ptr,i);
						i_parseMathExpr(ptr + i,dsz);
					}
					p -> dataSize = dsz;
					p -> allocData();
					for(idx = 0; idx < dsz; idx ++){
						NEXT_WORD(ptr,i);
						i_parseMathExpr(ptr + i,p -> i_dataPtr[idx]);
					}
				}
				if(wrd -> dataType == SCR_DOUBLE_DATA){
					if(dsz == -1){
						NEXT_WORD(ptr,i);
						i_parseMathExpr(ptr + i,dsz);
					}
					p -> dataSize = dsz;
					p -> allocData();
					for(idx = 0; idx < dsz; idx ++){
						NEXT_WORD(ptr,i);
						d_parseMathExpr(ptr + i,p -> d_dataPtr[idx]);
					}
				}
				if(wrd -> dataType == SCR_CHAR_DATA){
					if(dsz == -1){
						NEXT_WORD(ptr,i);
						p -> dataSize = strlen(ptr + i) + 1;
						p -> allocData();
						strcpy(p -> c_dataPtr,ptr + i);
					}
					else {
						p -> dataSize = dsz;
						p -> allocData();
						for(idx = 0; idx < dsz; idx ++){
							NEXT_WORD(ptr,i);
							p -> c_dataPtr[idx] = ptr[i];
						}
					}
				}
				p -> initName(wrd -> name);
				p -> ID = wrd -> ID;
				cp -> add(p);
				break;
		}
		NEXT_WORD(ptr,i);
	}

	freeStringData();
	delete ptr;

	return 1;
}

void scrDataBlock::dump(XStream& fh,int idx,int mode)
{
	int i;
	scrDataBlock* p;

	if(!mode){
		for(i = 0; i < idx; i ++){
			fh < "\t";
		}
		if(name) fh < name;
		else fh <= ID;
	}

	if(dataType != SCR_CHAR_DATA && dataSize0 == -1) fh < " " <= dataSize;

	switch(dataType){
		case SCR_INT_DATA:
			for(i = 0; i < dataSize; i ++){
				fh < " " <= i_dataPtr[i];
			}
			break;

		case SCR_DOUBLE_DATA:
			for(i = 0; i < dataSize; i ++){
				fh < " " <= d_dataPtr[i];
			}
			break;
		case SCR_CHAR_DATA:
			fh < " \"" < c_dataPtr < "\"";
			break;
	}

	if(nextLevel){
		if(!mode) fh < " {\r\n";
		p = nextLevel -> first();
		while(p){
			p -> dump(fh,idx + 1);
			p = p -> next;
		}
		for(i = 0; i < idx; i ++) fh < "\t";
		if(!mode) fh < "}\r\n";
	}

	fh < "\r\n";
}

void dumpScript(const char* fname,scrDataBlock* p)
{
	XStream fh(fname,XS_OUT);
	p -> dump(fh,-1,1);
	fh.close();
}

void scrDataBlock::get_keys(void)
{
	scrKeyword* k;
	scrDataBlock* p;

	if(name && !scrKeywordList -> search(ID)){
		k = new scrKeyword;
		k -> ID = ID;
		k -> dataSize = dataSize0;
		k -> dataType = dataType;

		k -> name = new char[strlen(name) + 1];
		strcpy(k -> name,name);

		scrKeywordList -> append(k);
	}

	if(!nextLevel) return;

	p = nextLevel -> first();
	while(p){
		p -> get_keys();
		p = p -> next;
	}
}

void dumpScriptKeywords(const char* fname,scrDataBlock* p)
{
	scrKeyword* k;

	scrKeywordList = new xtList<scrKeyword>;
	p -> get_keys();

	XStream fh(fname,XS_OUT);
	k = scrKeywordList -> first();
	while(k){
		k -> dump(fh);
		k = k -> next;
	}
	fh.close();

	k = scrKeywordList -> first();
	while(k){
		scrKeywordList -> remove(k);
		delete k;
		k = scrKeywordList -> first();
	}
	delete scrKeywordList;
}

void scrDataBlock::saveData(XStream& fh)
{
	scrDataBlock* p;

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
		p = nextLevel -> first();
		while(p){
			p -> saveData(fh);
			p = p -> next;
		}
	}
}

void scrDataBlock::loadData(XStream& fh)
{
	scrDataBlock* p;

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
		p = nextLevel -> first();
		while(p){
			p -> loadData(fh);
			p = p -> next;
		}
	}
}

void scrDataBlock::loadData(XBuffer& xb)
{
	scrDataBlock* p;

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
		p = nextLevel -> first();
		while(p){
			p -> loadData(xb);
			p = p -> next;
		}
	}
}

void scrDataBlock::saveInfo(XStream& fh)
{
	int sz = 0;
	scrDataBlock* p;

	fh < ID < dataSize < dataType < dataSize0;
	if(name) sz = strlen(name);

	fh < sz;
	if(name) fh < name;

	if(nextLevel){
		fh < nextLevel -> size();
		p = nextLevel -> first();
		while(p){
			p -> saveInfo(fh);
			p = p -> next;
		}
	}
	else {
		fh < (int)0;
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
		allocList();
		for(i = 0; i < sz; i ++){
			p = new scrDataBlock;
			p -> loadInfo(fh);
			add(p);
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
		allocList();
		for(i = 0; i < sz; i ++){
			p = new scrDataBlock;
			p -> loadInfo(xb);
			add(p);
		}
	}
}

void saveScript(const char* fname,scrDataBlock* p)
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

scrDataBlock* loadScript(const std::string &fname)
{
	XStream fh(fname.c_str(),XS_IN);
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

void scrSetVerbose(int v)
{
	scrVerbose = v;
}

void scrSetInputDir(const char* p)
{
	scrInputDIR = strdup(p);
}

void scrSetOutputDir(const char* p)
{
	scrOutputDIR = strdup(p);
}

void scrSetOutputFile(const char* p)
{
	scrOutputFILE = strdup(p);
}

void scrKeyword::dump(XStream& fh)
{
	fh < dataID[dataType];

	if(dataSize == -1)
		fh < "*";

	if(dataSize > 1)
		fh < "[" <= dataSize < "]";

	fh < " " < name < "\t\t" <= ID < "\r\n";
}
