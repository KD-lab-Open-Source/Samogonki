
#ifndef __XINI_FILE_H__
#define __XINI_FILE_H__

#include <cstring>

class xINI_FileKey 
{
	int size;
	char* string;
	char* name;

	void load(XStream& fh);
	void save(XStream& fh,int binary = 0);
public:
	xINI_FileKey* next;
	xINI_FileKey* prev;
	void* list;

	char* getValue(void) const { return string; }
	char* getName(void) const { return name; }
	
	void putValue(const char* p);
	void setName(const char* p){ if(name) free(name); name = strdup(p); }

	xINI_FileKey(void){ size = 0; string = NULL; name = NULL; list = NULL; }
	xINI_FileKey(const char* nm){ size = 0; string = NULL; name = NULL; list = NULL; setName(nm); }
	xINI_FileKey(const char* nm,const char* v){ size = 0; string = NULL; name = NULL; list = NULL; putValue(v); setName(nm); }

	~xINI_FileKey(void){
          if(string) delete[] string;
          if(name) free(name);
        }

	friend class xINI_FileSection;
};

class xINI_FileSection
{
	char* name;
	xtList<xINI_FileKey> keyList;

	void load(XStream& fh);
	void save(XStream& fh,int binary = 0);
public:
	xINI_FileSection* next;
	xINI_FileSection* prev;
	void* list;

	char* getName(void) const { return name; }

	char* getKey(const char* key_name);
	void putKey(const char* key_name,const char* key_value);

	void setName(const char* p){ if(name) free(name); name = strdup(p); }

	xINI_FileSection(void){ name = NULL; list = NULL; }
	xINI_FileSection(const char* nm){ name = NULL; list = NULL; setName(nm); }
	~xINI_FileSection(void);

	friend class xINI_File;
};

class xINI_File 
{
	char* name;
	bool isBinary;
	xtList<xINI_FileSection> sectionList;

	void load_text(void);
	void load_binary(void);
	int is_binary(void) const;
public:
	xINI_File* next;
	xINI_File* prev;
	void* list;

	void setName(const char* fname){ free(name); name = strdup(fname); }
	char* getName(void) const { return name; }

	void load(void);
	void save(void);

	void save_text(char* fname);
	void save_binary(char* fname);

	void pack(void);
	void unpack(void);

	void put_key(const char* section,const char* key,const char* value);
	char* get_key(const char* section,const char* key);

	xINI_File(const char* nm);
	~xINI_File(void);
};

#endif /* __XINI_FILE_H__ */
