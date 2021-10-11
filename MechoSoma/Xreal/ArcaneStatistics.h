#ifndef __ARCANE_STATISTICS__
#define __ARCANE_STATISTICS__

class ArcaneStatisticsInfo
{
public:
	int ArcaneID;
	int OwnerID;
	int ObjectID;
	struct fxlabGeneralObjectType* ArcaneOwner;

public:
	ArcaneStatisticsInfo(void)
	{
		ArcaneID = -1;
		OwnerID = -1;
		ObjectID = -1;
		ArcaneOwner = NULL;
	};

	ArcaneStatisticsInfo(int arcane_id,int owner_id,struct fxlabGeneralObjectType* p,int object_id)
	{
		ArcaneID = arcane_id;
		OwnerID = owner_id;
		ArcaneOwner = p;
	};

	void Init(int arcane_id,int owner_id,struct fxlabGeneralObjectType* p,int object_id)
	{
		ArcaneID = arcane_id;
		OwnerID = owner_id;
		ArcaneOwner = p;
	};

	int GetOwnerID(void) const { return OwnerID; };
	int GetArcaneID(void) const { return ArcaneID; };
	struct fxlabGeneralObjectType* GetArcaneOwner(void) const { return ArcaneOwner; };
	int GetObjectID(void) const { return ObjectID; };

	int& GetOwnerID(void){ return OwnerID; };
	int& GetArcaneID(void){ return ArcaneID; };	
	struct fxlabGeneralObjectType*& GetArcaneOwner(void){ return ArcaneOwner; };
	int& GetObjectID(void){ return ObjectID; };
};
 
void fxlabArcaneAction(const ArcaneStatisticsInfo* p,int id);

class ArcaneStatisticsService : ArcaneStatisticsInfo
{
public:
	ArcaneStatisticsService() : ArcaneStatisticsInfo(current_arcane){}
	void putArcaneStatistics(int suffered_ID = 0) const { fxlabArcaneAction(this, suffered_ID); }
	static void setCurrentArcane(const ArcaneStatisticsInfo& arcane){ current_arcane = arcane; }
	static ArcaneStatisticsInfo current_arcane;
};

#endif //__ARCANE_STATISTICS__

