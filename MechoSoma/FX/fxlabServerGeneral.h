struct fxlabProtectionType
{
	int ProtectionID;
	int ProtectionTime;
	DurationTimer ProtectionTimer;

	fxlabProtectionType(void);
	virtual void ProtectionQuant(void);
	void SetProtection(int id,int time)
	{ 
		ProtectionID = id;
		ProtectionTime = time;
		ProtectionTimer.start(ProtectionTime);
	};
	int CheckProtection(struct mchRacer* p);
};

const float FXLAB_TRACK_AI_SCALE = 4.0f;
#define FXLAB_TRACK_AI_UPDATE(x,y,radius,type) mch_trackMap->SetFlag(round(x),round(y),round(radius/* * FXLAB_TRACK_AI_SCALE*/),round(radius/* * FXLAB_TRACK_AI_SCALE*/),type)

/*struct fxlabTrackDataType
{
	int TrackType;
	int TrackRadius;

	fxlabTrackDataType(void);
	virtual void SetTrackInfo(void);
	virtual void UpdateTrack(int x,int y);
};
*/

//--------------------------------------------

struct fxlabServerEvolutionType : fxlabApplicationObjectType , fxlabProtectionType , public ArcaneStatisticsInfo
{
	void Start(void);
	void Quant(void);
	void SetProtection(int id,int time);
	void SetStatistic(int owner_id,int arcane_id,fxlabGeneralObjectType* p,int object_id);
	int GetStatisticOwnerID(void){ return GetOwnerID(); };
	class ArcaneStatisticsInfo* GetStatistic(void){ return (dynamic_cast<ArcaneStatisticsInfo*>(this)); };
};

//------------------------------------------------


struct fxlabServerSpaceType : fxlabServerEvolutionType
{
	Vect3f Position;
	void Open(void)
	{
		fxlabServerEvolutionType::Open();
		Position = Vect3f::ZERO;
	};
	void SetPosition(Vect3f& v){ Position = v; };
};

//----------------------------------------------------

struct fxlabServerKeyObjectType : fxlabServerSpaceType
{
	float* KeyData;
	int KeyID;
	fxlabResourceObject* KeyPoint;
	float KeyTime;

	void Open(void)
	{
		fxlabServerSpaceType::Open();
		KeyData = NULL;
		KeyID = -1;
		KeyPoint = NULL;
		KeyTime = 0;
	};

	void Close(void);
	void Start(void);
	void Quant(void);

	void SetKeyID(int id){ KeyID = id; };

	virtual void KeyCheck(void){};
	virtual void KeyUpdate(void);
};

//-------------------------------------------------
