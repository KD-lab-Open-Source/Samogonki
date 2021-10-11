
#ifndef __PARTS_POOL_H__
#define __PARTS_POOL_H__

class cMesh;

// mchMechosPartInfo::flags...
const int MCP_BUSY		= 0x01;

struct mchMechosPartInfo
{
	int ID;
	int partID;

	int flags;

	cMesh* data;

	void* list;
	mchMechosPartInfo* prev;
	mchMechosPartInfo* next;

	mchMechosPartInfo(void);
	mchMechosPartInfo(int p_id,int id);
	mchMechosPartInfo(cMesh* p);

	~mchMechosPartInfo(void){ };
};


struct mchMechosPartsPool
{
	int ID;
	xtList<mchMechosPartInfo>** pool;
	mchMechosPartInfo** poolCur;

	void* list;
	mchMechosPartsPool* prev;
	mchMechosPartsPool* next;

	void Create(int sz = -1);
	void Create(mchMechosPartsPool* p,int sz = -1);

	void SetCurrentMechos(char* cfg,int sz);
	void GetCurrentMechos(char* cfg,int sz);

	void GetMechos(char* cfg,int sz,unsigned char* part_flags,int mode = 0);
	mchMechosPartInfo* GetPart(int tp);
	mchMechosPartInfo* GetPartByArcane(int tp);
	mchMechosPartInfo* GetPart(int tp,int id);

	void PutMechos(char* cfg);
	void PutPart(int pID,int ID);
	void PutPart(cMesh* p);
	void PutPart(mchMechosPartInfo* p);

	void EnablePart(int pID,int ID);

	void reset(void);
	void resize(int sz);

	int PartStatus(int tp);

	mchMechosPartsPool(void);
	~mchMechosPartsPool(void);
};

void mchCreatePartsPool(void);
void mchDeletePartsPool(void);

void mchGetMechos(char* cfg,int sz,unsigned char* part_flags,int pool_id = 0,int mode = 0);
void mchPutMechos(char* cfg,int pool_id = 0);

void mchPutPart(int pool_id,int part_type,int part_id);
int mchGetPart(int pool_id,int arc_id,int& tp_id);

int mchNextPart(int part_type,int part_id);
int mchPrevPart(int part_type,int part_id);

int mchGetPartArcane(int part_type,int part_id);
char* mchGetPartArcaneName(int part_type,int part_id);

extern xtList<mchMechosPartsPool> mch_partsPoolLst;

void mchDumpPartsPool(int pool_id);
void mchResetPartsPool(int pool_id);

cMesh* mchGetPartByArcane(int arc_id);
cMesh* mchGetPartByType(int type,int kind);
int mchPartStatus(int tp);

#endif // __PARTS_POOL_H__
