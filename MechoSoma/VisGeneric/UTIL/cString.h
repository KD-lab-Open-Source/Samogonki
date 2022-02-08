#ifndef __CSTRING_H__
#define __CSTRING_H__

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <strings.h>
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

//#define _STRING_DEBUG_
#define STRING_ASSERT(a) assert(a)

class cString
{
	char *buf;
public:
	
	cString()
	{
		buf=0; SetSize();
	}

	cString(int size)
	{
		if(size)
		{
			buf=(char*)malloc(size);
			SetSize(size);
		}
		else
		{
			buf=0;
			SetSize();
		}
	}

	cString(char *string)
	{
		if(string)
		{
			buf=strdup(string);
			SetSize();
		}
		else
		{
			buf=0;
			SetSize();
		}
	}

	cString(cString &string)
	{
		if(string.buf)
		{
			buf=strdup(string);
			SetSize();
		}
		else
		{
			buf=0;
			SetSize();
		} 
	}

	~cString()
	{
		if(buf) free(buf);
	}

	inline int operator == (const char *string)
	{
		if((string)&&(buf)) return !stricmp(string,buf); 
		return string==buf;
	}

	inline int operator != (const char *string)
	{
		if((string)&&(buf)) return stricmp(string,buf);
		return string!=buf;
	}	

	inline char& operator [](int l)
	{
		AssertValid(l);
		return buf[l];
	}

	inline operator char*()
	{
		return buf;
	}

	inline char* ptr()
	{
		return buf;
	}

	inline int length()
	{
		if(buf) return strlen(buf);
		return 0;
	}

	inline cString& operator << (cString &string)
	{
		if(string.buf)
		{
			if(buf)
			{
				char *tmp=buf;
				buf=(char*)malloc(strlen(tmp)+strlen(string.buf)+1);
				SetSize();
				strcpy(buf,tmp);
				strcat(buf,string.buf);
				free(tmp);
			}
			else
			{
				buf=strdup(string.buf);
				SetSize();
			}
		}
		return *this; 
	}

	inline cString& operator << (const char *string)
	{
		if(string)
		{
			if(buf)
			{
				char *tmp=buf;
				buf=(char*)malloc(strlen(tmp)+strlen(string)+1);
				SetSize();
				strcpy(buf,tmp);
				strcat(buf,string);
				free(tmp);
			}
			else
			{
				buf=strdup(string);
				SetSize();
			}
		}
		return *this;
	}

	inline cString& operator = (cString &string)
	{
		if(buf) free(buf);
		if(string.buf)
		{
			buf=strdup(string.buf);
			SetSize();
		}
		else
		{
			buf=0;
			SetSize();
		}
		return *this;
	}

	inline cString& operator = (const char *string)
	{
		if(buf) free(buf);
		if(string) buf=strdup(string);
		else buf=0;
		SetSize();
		return *this;
	}

	inline void New(const char *string)
	{
		if(buf) free(buf);
		if(string) buf=strdup(string);
		else buf=0;
		SetSize();
	}

	inline void New(int size)
	{
		if(buf) free(buf);
		if(size)
		{
			buf=(char*)malloc(size);
			SetSize(size);
		}
		else
		{
			buf=0;
			SetSize();
		}
	}

	inline void Delete()
	{
		if(buf)
		{
			free(buf);
			buf=0;
			SetSize();
		}
	}

	inline void Release()
	{
		if(buf)
		{
			free(buf);
			buf=0;
			SetSize();
		}
	}

	inline void Set(const char *string,int pos=0);				

private:
#ifdef _STRING_DEBUG_
	int size;
	inline int AssertValid(int l)
	{
		STRING_ASSERT(l<size);
		return 1;
	}

	inline void SetSize(int l=-1)
	{
		if(l>=0) size=l;
		else if(buf) size=strlen(buf)+1;
		else size=0;
	}
#else
	inline int AssertValid(int l)
	{
		return 1;
	}

	inline void SetSize(int l=-1)
	{
	}
#endif //_STRING_DEBUG_
};

inline void cString::Set(const char *string,int pos)	
{ 
	if(string) 
	{ 
		int lString=strlen(string),lBuf=strlen(buf); 
		if(buf) 
			if(lBuf>=(lString+pos)) 
				memcpy(&buf[pos],string,lString); 
			else 
			{ 
				char *tmp=buf; 
				buf=(char*)malloc(lString+pos+1);
				memcpy(buf,tmp,lBuf);
				memcpy(&buf[pos],string,lString);
				buf[lString+pos]=0;
				free(tmp); 
			} 
		else 
		{ 
			buf=(char*)malloc(lString+pos+1); 
			strcpy(&buf[pos],string); 
		} 
	} 
	else 
		if(buf) 
		{ 
			free(buf); 
			buf=0; 
		} 
	SetSize(); 
}

#endif // __CSTRING_H__
