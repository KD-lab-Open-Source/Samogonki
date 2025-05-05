/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "Chtree.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

int aciXData = 0x8383;

aciCHTreeElement::aciCHTreeElement(void)
{
	ID = ACT_ROOT;
	data = 0;

	list = NULL;
}

aciCHTreeElement::~aciCHTreeElement(void)
{
	aciCHTreeElement* p,*p1;

	p = nextLevel.first();
	while(p){
		p1 = p -> next;
		delete p;
		p = p1;
	}
}

aciCHTree::aciCHTree(void)
{
	rootEl = NULL;
	currentEl = NULL;
}

aciCHTree::~aciCHTree(void)
{
	delete rootEl;
}

void aciCHTree::add_string(char* str,int id)
{
	int i,i0,sz,v;
	aciCHTreeElement* curEl,*prevEl;

	sz = strlen(str);

	if(!rootEl){
		curEl = new aciCHTreeElement;
		curEl -> ID = ACT_ROOT;
		prevEl = rootEl = curEl;

		for(i = 0; i < sz; i ++){
			curEl = new aciCHTreeElement;
			curEl -> data = str[i];
			curEl -> data ^= aciXData;
			curEl -> ID = ACT_DATA;

			prevEl -> nextLevel.append(curEl);
			prevEl = curEl;
		}
		prevEl -> ID = id;
	}
	else {
		i0 = 0;
		curEl = prevEl = rootEl;
		while(curEl){
			prevEl = curEl;
			v = str[i0] ^ aciXData;
			curEl = curEl -> findNext(v);
			i0 ++;
		}
		for(i = i0 - 1; i < sz; i ++){
			curEl = new aciCHTreeElement;
			curEl -> data = str[i];
			curEl -> data ^= aciXData;
			curEl -> ID = ACT_DATA;

			prevEl -> nextLevel.append(curEl);
			prevEl = curEl;
		}
		prevEl -> ID = id;
	}
}

void aciCHTreeElement::save(XBuffer* fh)
{
	aciCHTreeElement* p;
	*fh < ID < data < nextLevel.size();

	p = nextLevel.first();
	while(p){
		p -> save(fh);
		p = p -> next;
	}
}

void aciCHTreeElement::load(XBuffer* fh)
{
	int i,sz;
	aciCHTreeElement* p;
	*fh > ID > data > sz;

	for(i = 0; i < sz; i ++){
		p = new aciCHTreeElement;
		p -> load(fh);
		nextLevel.append(p);
	}
}

aciCHTreeElement* aciCHTreeElement::findNext(int dt)
{
	aciCHTreeElement* p;

	p = nextLevel.first();
	while(p){
		if(p -> data == dt)
			return p;
		p = p -> next;
	}
	return NULL;
}

int aciCHTree::quant(int k)
{
	k ^= aciXData;
	if(!currentEl) currentEl = rootEl;
	currentEl = currentEl -> findNext(k);

	if(currentEl)
		return currentEl -> ID;

	return 0;
}

void aciCHTree::save(char* fname)
{
	int sz,sz1;
	XStream fh;
	char* p,*p1;
	XBuffer* XBuf = new XBuffer(64000);

	rootEl -> save(XBuf);
	p = XBuf -> address();
	sz = XBuf -> tell();
	p1 = new char[sz];

	sz1 = ZIP_compress(p1,sz,p,sz);

	fh.open(fname,XS_OUT);
	fh.write(p1,sz1);
	fh.close();

	delete XBuf;
	delete[] p1;
}

void aciCHTree::load(char* fname)
{
	XStream fh(fname,XS_IN);
	load(fh);
}

void aciCHTree::load(XStream& fh)
{
	int sz,sz1;
	char* p,*p1;
	XBuffer* XBuf;

	sz = fh.size();
	p = new char[sz];
	fh.read(p,sz);
	fh.close();

	sz1 = ZIP_GetExpandedSize(p);
	p1 = new char[sz1];
	ZIP_expand(p1,sz1,p,sz);

	XBuf = new XBuffer(p1,sz1);

	rootEl = new aciCHTreeElement;
	rootEl -> load(XBuf);

	delete XBuf;
	delete[] p;
	delete[] p1;
}

void aciCHTree::decode(char* fname)
{
#ifdef ACT_ENABLE_DECODING
	char str[256];

	XStream fh(fname,XS_OUT);
	rootEl -> decode(str,-1,fh);
	fh.close();
#endif
}

void aciCHTreeElement::decode(char* str,int level,XStream& fh)
{
#ifdef ACT_ENABLE_DECODING
	aciCHTreeElement* p;

	if(ID){ 
		str[level] = (char)(data ^ aciXData);
		str[level + 1] = 0;
	}

	if(ID > ACT_DATA)
		fh < str < "\t\t" <= ID < "\r\n";

	p = nextLevel.first();
	while(p){
		p -> decode(str,level + 1,fh);
		p = p -> next;
	}
#endif
}

void aciCHTree::encode(char* fname)
{
#ifdef ACT_ENABLE_ENCODING
	int i;
	char str[256];
	char* p,*p0,*p1;

	XStream fh(fname,XS_IN);
	
	while(!fh.eof()){
		memset(str,0,256);
		fh.getline(str,256);

		for(i = 0; i < 256; i ++){
			if(str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
				str[i] = 0;
		}

		p = str;
		for(i = 0; i < 255; i ++)
			if(!p[0]) p ++;

		if(strlen(p)){
			p0 = p;
			p1 = p;
			
			while(p1[0]){
				p1 ++;
				if((int)(p1 - p) >= 255) break;
			}
			while(!p1[0]){
				p1 ++;
				if((int)(p1 - p) >= 255) break;
			}
			
			XCon < p0 < "\t\t" < p1 < "\n";
			add_string(p0,atoi(p1));
		}
	}

	fh.close();

	XCon < "\n";
#endif
}
