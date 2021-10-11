/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"
#include "xINI_File.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

#define _xINI_BINARY_ONLY_

void xINI_FileKey::putValue(const char* p)
{
	int i,sz = strlen(p) - 1;

	if(p[0] != '\"' || p[sz] != '\"'){
		sz = strlen(p) + 1;
		if(size < sz){
			if(string) delete string;
			string = new char[sz];
			size = sz;
		}
		strcpy(string,p);
	}
	else {
		sz = strlen(p) - 1;
		if(size < sz){
			if(string) delete string;
			string = new char[sz];
			size = sz;
		}
		for(i = 0; i < sz; i ++)
			string[i] = p[i + 1];

		string[sz] = 0;
	}
}

xINI_FileSection::~xINI_FileSection(void)
{
	xINI_FileKey* p;
	if(name) free(name);

	p = keyList.first();
	while(p){
		keyList.remove(p);
		delete p;

		p = keyList.first();
	}
}

char* xINI_FileSection::getKey(const char* key_name)
{
	static char* empty_str = "";
	xINI_FileKey* p = keyList.first();
	while(p){
		if(!stricmp(p -> getName(),key_name))
			return p -> getValue();
		p = p -> next;
	}
	return empty_str;
}

void xINI_FileSection::putKey(const char* key_name,const char* key_value)
{
	xINI_FileKey* p = keyList.first();
	while(p){
		if(!stricmp(p -> getName(),key_name))
			break;
		p = p -> next;
	}
	if(!p){ 
		p = new xINI_FileKey(key_name);
		keyList.append(p);
	}

	p -> putValue(key_value);
}

xINI_File::xINI_File(const char* nm)
{
	char path[_MAX_PATH];

	if(_fullpath(path,nm,_MAX_PATH) == NULL) ErrH.Abort("Invalid INI path...");

	name = strdup(path);

	list = NULL;
}

xINI_File::~xINI_File(void)
{
	xINI_FileSection* p;

	free(name);

	p = sectionList.first();
	while(p){
		sectionList.remove(p);
		delete p;

		p = sectionList.first();
	}
}

const char* xINI_Sign = "xINI1.00\x0B0\x0B1\x0B2";
int xINI_File::is_binary(void) const 
{
	char str[256];
	XStream fh(name,XS_IN);
	fh.read(str,strlen(xINI_Sign));
	fh.close();

	str[strlen(xINI_Sign)] = 0;

	return !strcmp(xINI_Sign,str);
}

void xINI_File::save_binary(char* fname)
{
	xINI_FileSection* sp;

	XStream fh(fname,XS_OUT);
	fh < (char*)xINI_Sign < int(0) < sectionList.size();

	sp = sectionList.first();
	while(sp){
		sp -> save(fh,1);
		sp = sp -> next;
	}

	fh.close();
}

void xINI_File::save_text(char* fname)
{
	xINI_FileSection* sp;

	XStream fh(fname,XS_OUT);
	sp = sectionList.first();

	while(sp){
		sp -> save(fh,0);
		sp = sp -> next;
	}

	fh.close();
}

void xINI_File::load(void)
{
	if(is_binary())
		load_binary();
#ifndef _xINI_BINARY_ONLY_
	else
		load_text();
#endif
}

void xINI_File::load_binary(void)
{
	int i,sz;
	xINI_FileSection* p;

	XStream fh(name,XS_IN);
	fh.seek(strlen(xINI_Sign),XS_BEG);

	fh > sz;

	if(sz){
		fh.close();
		unpack();
		fh.open(name,XS_IN);
		fh.seek(strlen(xINI_Sign) + sizeof(int),XS_BEG);
	}
		
	fh > sz;

	for(i = 0; i < sz; i ++){
		p = new xINI_FileSection;
		p -> load(fh);
		sectionList.append(p);
	}

	pack();
}

void xINI_File::load_text(void)
{
	int sz;
	char* buf,*vbuf,*p;

	xINI_FileSection* sp;

	XStream fh(name,XS_IN);
	sz = fh.size() * 2;
	fh.close();

	buf = new char[sz];
	vbuf = new char[256];

	GetPrivateProfileString(NULL,NULL,NULL,buf,sz,name);
	p = buf;

	while(1){
		sp = new xINI_FileSection(p);
		sectionList.append(sp);
		
		while(*p++);

		if(!p[0]) break;
	}

	sp = sectionList.first();
	while(sp){
		GetPrivateProfileString(sp -> getName(),NULL,NULL,buf,sz,name);
		p = buf;

		if(p[0]){
			while(1){
				GetPrivateProfileString(sp -> getName(),p,NULL,vbuf,256,name);
				sp -> putKey(p,vbuf);
				
				while(*p++);

				if(!p[0]) break;
			}
		}

		sp = sp -> next;
	}

	delete buf;
	delete vbuf;
}

void xINI_FileSection::save(XStream& fh,int binary)
{
	xINI_FileKey* p = keyList.first();

	if(!binary){
		fh < "[" < name < "]\r\n";
		while(p){
			p -> save(fh,binary);
			p = p -> next;
		}
		fh < "\r\n";
	}
	else {
		fh < keyList.size();
		fh < strlen(name) < name;
		while(p){
			p -> save(fh,binary);
			p = p -> next;
		}
	}
}

void xINI_FileSection::load(XStream& fh)
{
	int i,sz;
	xINI_FileKey* p;

	char buf[256];

	fh > sz > i;
	fh.read(buf,i);
	buf[i] = 0;

	setName(buf);

	for(i = 0; i < sz; i ++){
		p = new xINI_FileKey;
		p -> load(fh);
		keyList.append(p);
	}
}

void xINI_FileKey::save(XStream& fh,int binary)
{
	int i,sz,sp_flag;
	if(!binary){
		sz = strlen(string);
		sp_flag = 0;
		for(i = 0; i < sz; i ++){
			if(!isdigit(string[i])){
				sp_flag = 1;
				break;
			}
		}

		if(sp_flag)
			fh < name < "=\"" < string < "\"\r\n";
		else
			fh < name < "=" < string < "\r\n";
	}
	else {
		fh < strlen(name) < name < strlen(string) < string;
	}
}

void xINI_FileKey::load(XStream& fh)
{
	int sz;
	static char buf[256];

	fh > sz;
	fh.read(buf,sz);
	buf[sz] = 0;
	setName(buf);

	fh > sz;
	fh.read(buf,sz);
	buf[sz] = 0;
	putValue(buf);
}

void xINI_File::pack(void)
{
	int sz0,sz1;
	char* buf0,*buf1;

	if(!is_binary()) return;

	XStream fh(name,XS_IN);
	fh.seek(strlen(xINI_Sign),XS_BEG);
	fh > sz0;

	if(sz0){
		fh.close();
		return;
	}

	sz0 = fh.size() - strlen(xINI_Sign) - sizeof(int);
	buf0 = new char[sz0];
	fh.read(buf0,sz0);
	fh.close();

	buf1 = new char[sz0 + sizeof(int)];

	sz1 = ZIP_compress(buf1,sz0 + sizeof(int),buf0,sz0);

	fh.open(name,XS_OUT);
	fh < (char*)xINI_Sign < int(1);
	fh.write(buf1,sz1);
	fh.close();

	delete buf0;
	delete buf1;
}

void xINI_File::unpack(void)
{
	int sz0,sz1;
	char* buf0,*buf1;

	if(!is_binary()) return;

	XStream fh(name,XS_IN);
	fh.seek(strlen(xINI_Sign),XS_BEG);
	fh > sz0;

	if(!sz0){
		fh.close();
		return;
	}

	sz0 = fh.size() - strlen(xINI_Sign) - sizeof(int);
	buf0 = new char[sz0];
	fh.read(buf0,sz0);
	fh.close();

	sz1 = ZIP_GetExpandedSize(buf0);
	buf1 = new char[sz1];

	ZIP_expand(buf1,sz1,buf0,sz0);

	fh.open(name,XS_OUT);
	fh < (char*)xINI_Sign < int(0);
	fh.write(buf1,sz1);
	fh.close();

	delete buf0;
	delete buf1;
}

void xINI_File::put_key(const char* section,const char* key,const char* value)
{
	xINI_FileSection* p = sectionList.first();
	while(p){
		if(!stricmp(p -> getName(),section)){
			p -> putKey(key,value);
			return;
		}
		p = p -> next;
	}
	p = new xINI_FileSection(section);
	p -> putKey(key,value);
	sectionList.append(p);
}

char* xINI_File::get_key(const char* section,const char* key)
{
	static char* empty_str = "";
	xINI_FileSection* p = sectionList.first();
	while(p){
		if(!stricmp(p -> getName(),section)){
			return p -> getKey(key);
		}
		p = p -> next;
	}
	return empty_str;
}

