
#ifndef __ACI_DEFINE_H__
#define __ACI_DEFINE_H__

// scrDefineData::flags
const unsigned DEF_TEXT	= 0x01;

struct scrDefineData
{
	int flags;

	int startIndex;
	int endIndex;

	int srcLen;
	char* srcData;

	int destLen;
	char* destData;

	void* list;
	scrDefineData* prev;
	scrDefineData* next;

	void init(char* ptr,int sz);

	scrDefineData(void);
	~scrDefineData(void);
};

#endif /* __ACI_DEFINE_H__ */
