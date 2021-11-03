/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "Dispatcher.h"

#include "mechosoma.h"
#include "parts_pool.h"

#include "mch_common.h" // For far target

//#define _POOL_DEBUG2_

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int mchPartID[];
extern unsigned char mch_partFlags[];
extern unsigned char mch_excludeFlags[];

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

cMesh* mchGetPart(int p_id,int id);

int mchGetPartID(int tp){
	for(int i = 0; i < NUM_ACTIVE_PARTS; i ++)
		if(mchPartID[i] == tp) return i;
	return -1;
}

/* --------------------------- DEFINITION SECTION --------------------------- */

xtList<mchMechosPartsPool> mch_partsPoolLst;

mchMechosPartInfo::mchMechosPartInfo(void)
{
	ID = partID = -1;
	data = NULL;
	list = NULL;
	flags = 0;
}

mchMechosPartInfo::mchMechosPartInfo(cMesh* p)
{
	ID = M3D_KIND(p -> Type);
	partID = mchGetPartID(M3D_TYPE(p -> Type));

	flags = 0;

	data = p;
	list = NULL;
}

mchMechosPartInfo::mchMechosPartInfo(int p_id,int id)
{
	ID = id;
	partID = p_id;

	flags = 0;

	data = mchGetPart(p_id,id);
	list = NULL;
}

cMesh* mchGetPart(int p_id,int id)
{
	int tp;
	cMesh* p = MeshLibrary -> First();

	tp = M3D_TOTAL_TYPE(id,mchPartID[p_id]);
	while(p){
		if(p -> Type == tp) return p;
		p = MeshLibrary -> Next();
	}

	ErrH.Abort("mchGetPart() failed...");
	return NULL;
}

mchMechosPartsPool::mchMechosPartsPool(void)
{
	int i;
	pool = new xtList<mchMechosPartInfo>*[NUM_ACTIVE_PARTS];
	poolCur = new mchMechosPartInfo*[NUM_ACTIVE_PARTS];
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		pool[i] = new xtList<mchMechosPartInfo>;

	list = NULL;
}

mchMechosPartsPool::~mchMechosPartsPool(void)
{
	int i;
	mchMechosPartInfo* p;
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		p = pool[i] -> first();
		while(p){
			pool[i] -> remove(p);
			delete p;
			p = pool[i] -> first();
		}
		delete pool[i];
	}
	delete pool;
	delete poolCur;
}

void mchMechosPartsPool::Create(int sz)
{
	int i;
	cMesh* p = MeshLibrary -> First();

	while(p){
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			if(p -> Type & mchPartID[i] && M3D_KIND(p -> Type) < 64)
				PutPart(p);
		}
		p = MeshLibrary -> Next();
	}
	resize(sz);
}

void mchMechosPartsPool::resize(int sz)
{
	int i,id;
	mchMechosPartInfo* ip;

	if(sz < 0) return;

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		while(pool[i] -> size() > sz){
			ip = pool[i] -> first();
			id = XRnd(pool[i] -> size());
			while(id){
				if(ip) ip = ip -> next;
				id --;
			}
			if(ip){
				pool[i] -> remove(ip);
				delete ip;
			}
		}
	}
}

void mchMechosPartsPool::PutPart(cMesh* p)
{
	int p_id = mchGetPartID(M3D_TYPE(p -> Type));
	mchMechosPartInfo* ip;

	if(p_id != -1){
		ip = new mchMechosPartInfo(p);
		PutPart(ip);
	}
}

void mchMechosPartsPool::PutPart(int pID,int ID)
{
	mchMechosPartInfo* ip = new mchMechosPartInfo(pID,ID);
	PutPart(ip);
}

void mchMechosPartsPool::PutMechos(char* cfg)
{
	int i,id;
	XBuffer XBuf(cfg,strlen(cfg));

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		XBuf >= id;
		EnablePart(i,id);
	}
}

void mchMechosPartsPool::GetMechos(char* cfg,int sz,unsigned char* part_flags,int mode)
{
	int i,id,cnt = 0;
	mchMechosPartInfo* p;
	XBuffer XBuf(cfg,sz);

	if(!mode){
		XBuf < "1 ";
		for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
			p = GetPart(i);
			while(p -> flags & MCP_BUSY || (p -> data -> arcaneData.ID != -1 && !part_flags[p -> data -> arcaneData.ID])){
				cnt ++;
				if(cnt > 10000){ 
					mchDumpPartsPool(ID);
//					ErrH.Abort("GetMechos() failed");
					break;
				}
				p = GetPart(i);
			}
			p -> flags |= MCP_BUSY;
			XBuf <= p -> ID < " ";
		}
	}
	else {
		XBuf >= id;
		for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
			XBuf >= id;
			p = GetPart(i,id);
			if(p)
				p -> flags |= MCP_BUSY;
		}
	}
}

mchMechosPartInfo* mchMechosPartsPool::GetPart(int tp)
{
	int id;
	mchMechosPartInfo* p;
	p = pool[tp] -> first();

	id = XRnd(pool[tp] -> size());
	while(id){
		if(p) p = p -> next;
		if(!p) 
			p = pool[tp] -> first();
//		if(!(p -> flags & MCP_BUSY))
			id --;
	}

	return p;
}

mchMechosPartInfo* mchMechosPartsPool::GetPartByArcane(int arc_id)
{
	int tp;
	mchMechosPartInfo* p;
	for(tp = 0; tp < NUM_ACTIVE_PARTS; tp ++){
		p = pool[tp] -> first();
		while(p){
			if(p -> data -> arcaneData.ID == arc_id)
				break;
			p = p -> next;
		}
		if(p && p -> data -> arcaneData.ID == arc_id)
			break;
	}
	return p;
}

mchMechosPartInfo* mchMechosPartsPool::GetPart(int tp,int id)
{
	mchMechosPartInfo* p = NULL;
	p = pool[tp] -> search(id);
//	if(p) pool[tp] -> remove(p);
	return p;
}

void mchCreatePartsPool(void)
{
	int i,num_pools = 1;
	mchMechosPartsPool* p;

	if(mch_partsPoolLst.size()) return;
/*
	if(mchGameMode == MCH_SINGLE_GAME)
		num_pools = 1;
	else
		num_pools = HS_PLAYERS_MAX + 2;
*/
	for(i = 0; i < num_pools; i ++){
		p = new mchMechosPartsPool;
		p -> ID = i;
		p -> Create();
		mch_partsPoolLst.append(p);
//		mchDumpPartsPool(i);
	}
}

void mchDeletePartsPool(void)
{
	mchMechosPartsPool* p = mch_partsPoolLst.first();
	while(p){
		mch_partsPoolLst.remove(p);
		delete p;
		p = mch_partsPoolLst.first();
	}
}

void mchGetMechos(char* cfg,int sz,unsigned char* part_flags,int pool_id,int mode)
{
	mchMechosPartsPool* p = mch_partsPoolLst.search(pool_id);
	if(p) p -> GetMechos(cfg,sz,part_flags,mode);
}

void mchPutMechos(char* cfg,int pool_id)
{
	mchMechosPartsPool* p = mch_partsPoolLst.search(pool_id);
	if(p) p -> PutMechos(cfg);
}

void mchMechosPartsPool::SetCurrentMechos(char* cfg,int sz)
{
	int i,id;

	XBuffer XBuf(cfg,sz);

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		XBuf >= id;
		poolCur[i] = pool[i] -> search(id);
		if(!poolCur[i]) ErrH.Abort("SetCurrentMechos() failed");
	}

	PutMechos(cfg);
}

void mchMechosPartsPool::GetCurrentMechos(char* cfg,int sz)
{
	int i;

	XBuffer XBuf(cfg,sz);
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		XBuf <= poolCur[i] -> ID < " ";
		poolCur[i] -> flags |= MCP_BUSY;
//		pool[i] -> remove(poolCur[i]);
		poolCur[i] = NULL;
	}
}

int mchGetPart(int pool_id,int arc_id,int& tp_id)
{
	int id;
	mchMechosPartInfo* ip;
	mchMechosPartsPool* p = mch_partsPoolLst.search(pool_id);
	if(!p) return -1;

	ip = p -> GetPartByArcane(arc_id);
	if(ip){
		id = ip -> ID;
		tp_id = ip -> partID;
		return id;
	}
	return -1;
}

void mchPutPart(int pool_id,int part_type,int part_id)
{
	mchMechosPartsPool* p = mch_partsPoolLst.search(pool_id);
	if(p) p -> PutPart(part_type,part_id);
}

void mchMechosPartsPool::PutPart(mchMechosPartInfo* p)
{ 
	mchMechosPartInfo* p0 = pool[p -> partID] -> search(p -> ID);

	if(p0){
		delete p;
		return;
	}

	pool[p -> partID] -> append(p); 
}

#ifdef _POOL_DEBUG2_
int mch_lstFlag = 0;
#endif

void mchDumpPartsPool(int pool_id)
{
#ifdef _POOL_DEBUG2_
	int i,j;
	mchMechosPartInfo* p;
	mchMechosPartsPool* pl = mch_partsPoolLst.search(pool_id);

	XStream fh;
	if(!mch_lstFlag){
		fh.open("shop.lst",XS_OUT);
		mch_lstFlag = 1;
	}
	else
		fh.open("shop.lst",XS_OUT | XS_NOREPLACE | XS_APPEND);

	char* lst_names[5] = { "Engine", "Front", "Rear", "F-Wheels", "R-Wheels" };

	fh < "\r\n -------------------- \r\n";

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		fh < " " <= pl -> pool[i] -> size();

	fh < "\r\n\r\n";

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		fh < lst_names[i] < ":\r\n";
		for(j = 0; j <= 50; j ++){
			p = pl -> pool[i] -> first();
			while(p){
				if(!(p -> flags & MCP_BUSY) && p -> ID == j && !mch_excludeFlags[p -> data -> arcaneData.ID]){
					fh <= p -> ID < " " <= p -> data -> arcaneData.ID < " " < p -> data -> arcaneData.name;
					if(p -> data -> arcaneData.ID != -1 && !mch_excludeFlags[p -> data -> arcaneData.ID] && !mch_partFlags[p -> data -> arcaneData.ID])
						fh < " *";
					fh < "\r\n";
				}
				p = p -> next;
			}
		}
		fh < "\r\n";
	}
	fh.close();
#endif
}

void mchResetPartsPool(int pool_id)
{
	mchMechosPartsPool* pl = mch_partsPoolLst.search(pool_id);
	if(pl) pl -> reset();
}

void mchMechosPartsPool::reset(void)
{
	int i;
	mchMechosPartInfo* p;
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		p = pool[i] -> first();
		while(p){
			p -> flags &= ~MCP_BUSY;
			p = p -> next;
		}
	}
}

void mchMechosPartsPool::EnablePart(int pID,int ID)
{
	mchMechosPartInfo* p = pool[pID] -> search(ID);
	if(p) p -> flags &= ~MCP_BUSY;
}

cMesh* mchGetPartByArcane(int arc_id)
{
	mchMechosPartInfo* p;
	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);

	p = pl -> GetPartByArcane(arc_id);
	if(p) return p -> data;

	return NULL;
}

cMesh* mchGetPartByType(int type,int kind)
{
	mchMechosPartInfo* p;
	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);

	p = pl -> GetPart(type,kind);
	if(p) return p -> data;

	return NULL;
}

int mchMechosPartsPool::PartStatus(int tp)
{
	int i;
	mchMechosPartInfo* p;
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		p = pool[i] -> first();
		while(p){
			if(p -> data -> Type == tp)
				return (p -> flags & MCP_BUSY);
			p = p -> next;
		}
	}
	return 0;
}

int mchPartStatus(int tp)
{
	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);
	return pl -> PartStatus(tp);
}

int mchGetPartArcane(int part_type,int part_id)
{
	if(!mch_partsPoolLst.size())
		mchCreatePartsPool();

	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);
	mchMechosPartInfo* p = pl -> pool[part_type] -> search(part_id);

	if(p) return p -> data -> arcaneData.ID;

	return -1;
}

char* mchGetPartArcaneName(int part_type,int part_id)
{
	if(!mch_partsPoolLst.size())
		mchCreatePartsPool();

	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);
	mchMechosPartInfo* p = pl -> pool[part_type] -> search(part_id);

	if(p) return p -> data -> arcaneData.name;

	return NULL;
}

int mchNextPart(int part_type,int part_id)
{
	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);
	mchMechosPartInfo* p = pl -> pool[part_type] -> search(part_id),*p1;

	if(p){
		p1 = p -> next;
		if(!p1) p1 = pl -> pool[part_type] -> first();

		if(p1 -> data -> arcaneData.ID == -1) return p1 -> ID;

		while(mch_excludeFlags[p1 -> data -> arcaneData.ID]){
			p1 = p -> next;
			if(!p1) p1 = pl -> pool[part_type] -> first();
		}

		return p1 -> ID;
	}

	return -1;
}

int mchPrevPart(int part_type,int part_id)
{
	mchMechosPartsPool* pl = mch_partsPoolLst.search(0);
	mchMechosPartInfo* p = pl -> pool[part_type] -> search(part_id),*p1;

	if(p){
		p1 = p -> prev;

		if(p1 -> data -> arcaneData.ID == -1) return p1 -> ID;

		while(mch_excludeFlags[p1 -> data -> arcaneData.ID])
			p1 = p -> prev;

		return p1 -> ID;
	}

	return -1;
}

