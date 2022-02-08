/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include <cstdlib>
#include <cstring>

#include "online_game.h"

#ifndef _WIN32
#include "port.h"
#endif

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

static char wiConvertBuffer[64];

void ogBuffer::alloc(unsigned int sz)
{
	buf = new char[size = sz];
	offset = 0L;
	make_free = 1;
	*buf = 0;
	radix = 10;
	digits = 8;
}

void ogBuffer::realloc(unsigned int sz)
{
	char* p;

	if(sz < size) return;

	p = new char[sz];
	memcpy(p,buf,size);
	size = sz;

	delete buf;
	buf = p;
}

ogBuffer::ogBuffer(void* p,int sz)
{
	buf = (char*)p;
	size = sz;
	offset = 0;
	make_free = 0;
	radix = 10;
	digits = 8;
	separator = ';';
}

void ogBuffer::free(void)
{
	if(make_free && buf){
		delete buf;
		buf = 0;
	}
}

void ogBuffer::fill(char fc)
{ 
	memset(buf,fc,size); 
}

void ogBuffer::set(int off)
{
	offset = off;
}

unsigned int ogBuffer::read(void* s,unsigned int len)
{
	memcpy(s,buf + offset,len);
	offset+=len;
	return len;
}

unsigned int ogBuffer::write(const void* s,unsigned int len)
{
	if(len + offset + 1 > size)
		realloc(offset + len + 1);

	memcpy(buf + offset,s,len);
	offset += len;

	buf[offset++] = separator;

	return len;
}

ogBuffer& ogBuffer::operator< (char var)
{
	write(&var,(unsigned int)sizeof(char));
	return *this;
}

ogBuffer& ogBuffer::operator< (unsigned char var)
{
	write(&var,(unsigned int)sizeof(char));
	return *this;
}

ogBuffer& ogBuffer::operator< (short var)
{
	char* s = port_itoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (unsigned short var)
{
	char* s = port_ltoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (int var)
{
	char* s = port_itoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (unsigned var)
{
	char* s = port_ultoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (long var)
{
	char* s = port_ltoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (unsigned long var)
{
	char* s = port_ultoa(var,wiConvertBuffer,radix);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (float var)
{
	char* s = gcvt(var,digits,wiConvertBuffer);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (double var)
{
	char* s = gcvt(var,digits,wiConvertBuffer);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (long double var)
{
	char* s = gcvt(var,digits,wiConvertBuffer);
	write(s,strlen(s));
	return *this;
}

ogBuffer& ogBuffer::operator< (const char* v)
{
	if(v) write(v,(unsigned int)strlen(v));
	return *this;
}

ogBuffer& ogBuffer::operator> (char& var)
{
	var = buf[offset];
	offset ++;
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (unsigned char& var)
{
	var = buf[offset];
	offset ++;
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (short& var)
{
	char* p = buf + offset;
	var = (short)strtol(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (unsigned short& var)
{
	char* p = buf + offset;
	var = (unsigned short)strtoul(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (int& var)
{
	char* p = buf + offset;
	var = strtol(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (unsigned& var)
{
	char* p = buf + offset;
	var = strtoul(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (long& var)
{
	char* p = buf + offset;
	var = strtol(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (unsigned long& var)
{
	char* p = buf + offset;
	var = strtoul(p,&p,0);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (double& var)
{
	char* p = buf + offset;
	var = strtod(p,&p);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (long double& var)
{
	char* p = buf + offset;
	var = strtod(p,&p);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (float& var)
{
	char* p = buf + offset;
	var = (float)strtod(p,&p);
	offset += p - (buf + offset);
	if(buf[offset] == separator) offset ++;
	return *this;
}

ogBuffer& ogBuffer::operator> (char* v)
{
	unsigned int idx = token_length();

	memcpy(v,buf + offset,idx);
	v[idx] = 0;

	offset += idx + 1;
	return *this;
}

unsigned int ogBuffer::token_length(void)
{
	unsigned int idx = offset;

	while(buf[idx++] != separator){
		if(!buf[idx]){ 
			idx ++;
			break;
		}

		if(idx >= size){ 
			idx ++;
			break;
		}
	}

	return idx - offset - 1;
}

void ogBuffer::write_skip(void)
{
	buf[offset++] = separator;
}

void ogBuffer::read_skip(void)
{
	offset ++;
	if(buf[offset] == separator)
		offset ++;
}
