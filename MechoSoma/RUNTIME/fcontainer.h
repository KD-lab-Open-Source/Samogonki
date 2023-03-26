#ifndef __FCONTAINER_H__
#define __FCONTAINER_H__

#define _fC_READONLY_

class fContainer
{
	char* fname;
	char* fpath;

	int indexSize;
	int* index;

public:

	void load_idx(const char* idx_name);
	void save_idx(const char* idx_name);

	void load_idx(XStream& fh);

	void read(char*& p,int& len);

#ifndef _fC_READONLY_
	void write(char* p,int len);
#endif

	void set_file(const char* p){ if(fname) free(fname); fname = strdup(p); }
	void set_path(const char* p){ if(fpath) free(fpath); fpath = strdup(p); }

	fContainer(void);
	~fContainer(void);
};

#endif /* __FCONTAINER_H__ */