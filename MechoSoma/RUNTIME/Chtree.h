
#ifndef __CHTREE_H__
#define __CHTREE_H__

// MUST be disabled in final version...
#define ACT_ENABLE_ENCODING
#define ACT_ENABLE_DECODING

// Element IDs...
#define ACT_ROOT	0x00
#define ACT_DATA	0x01

struct aciCHTreeElement
{
	int ID;
	int data;

	xtList<aciCHTreeElement> nextLevel;

	aciCHTreeElement* findNext(int dt);

	void* list;
	aciCHTreeElement* prev;
	aciCHTreeElement* next;

	void save(XBuffer* fh);
	void load(XBuffer* fh);

	void decode(char* str,int level,XStream& fh);

	aciCHTreeElement(void);
	~aciCHTreeElement(void);
};

struct aciCHTree
{
	aciCHTreeElement* rootEl;
	aciCHTreeElement* currentEl;

	void add_string(char* str,int id);
	int quant(int k);

	void reset(void){ currentEl = rootEl; }

	void save(char* fname);
	void load(char* fname);
	void load(XStream& fh);

	void encode(char* fname);
	void decode(char* fname);

	aciCHTree(void);
	~aciCHTree(void);
};

#endif // __CHTREE_H__
