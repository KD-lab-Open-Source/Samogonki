
#ifndef __ACI_PARSER_H__
#define __ACI_PARSER_H__

// scrDataBlock flags...
#define SCR_ALLOC_DATA		0x01

// Data types...
enum scrDataTypes {
	SCR_VOID_DATA = 0,
	SCR_INT_DATA,
	SCR_DOUBLE_DATA,
	SCR_CHAR_DATA,

	SCR_MAX_DATA_ID
};

struct scrDataBlock
{
	int ID;
	int flags;
	int dataSize;
	int dataType;

	int dataSize0;
	char* name;

	union {
		int* i_dataPtr;
		double* d_dataPtr;
		char* c_dataPtr;
	};

	scrDataBlock* owner;
	xtList<scrDataBlock>* nextLevel;

	void* list;
	scrDataBlock* prev;
	scrDataBlock* next;

	void saveInfo(XStream& fh);
	void saveData(XStream& fh);
	void loadInfo(XStream& fh);
	void loadData(XStream& fh);

	void loadInfo(XBuffer& fh);
	void loadData(XBuffer& fh);

	void initName(char* p);
	void allocList(void){ nextLevel = new xtList<scrDataBlock>; }

	void alloc(int tp,int sz);
	void allocData(void);
	void freeData(void);

	void add(scrDataBlock* p);
	void dump(XStream& fh,int idx,int mode = 0);
	void get_keys(void);

	scrDataBlock* find(int id){ if(nextLevel) return nextLevel -> search(id); return NULL; }

	scrDataBlock(int tp);
	scrDataBlock(void);
	~scrDataBlock(void);
};

void scrSetVerbose(int v = 1);

void scrSetInputDir(char* p);
void scrSetOutputDir(char* p);
void scrSetOutputFile(char* p);

scrDataBlock* loadScript(char* fname);
scrDataBlock* loadScript(XStream& fh);
scrDataBlock* parseScript(char* fname,char *set_name);
void saveScript(char* fname,scrDataBlock* p);
void dumpScript(char* fname,scrDataBlock* p);
void dumpScriptKeywords(char* fname,scrDataBlock* p);

#endif /* __ACI_PARSER_H__ */
