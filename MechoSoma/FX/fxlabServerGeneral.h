struct fxlabProtectionType
{
	int ProtectionID;
	int ProtectionTime;
	DurationTimer ProtectionTimer;

	fxlabProtectionType();
	virtual void ProtectionQuant();
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

	fxlabTrackDataType();
	virtual void SetTrackInfo();
	virtual void UpdateTrack(int x,int y);
};
*/

//--------------------------------------------

struct fxlabServerEvolutionType : fxlabApplicationObjectType , fxlabProtectionType , public ArcaneStatisticsInfo
{
	void Start() override;
	void Quant() override;
	void SetProtection(int id,int time) override;
	void SetStatistic(int owner_id,int arcane_id,fxlabGeneralObjectType* p,int object_id) override;
	int GetStatisticOwnerID() override { return GetOwnerID(); };
	class ArcaneStatisticsInfo* GetStatistic() override { return (dynamic_cast<ArcaneStatisticsInfo*>(this)); };
};

//------------------------------------------------


struct fxlabServerSpaceType : fxlabServerEvolutionType
{
	Vect3f Position;
	void Open() override
	{
		fxlabServerEvolutionType::Open();
		Position = Vect3f::ZERO;
	};
	void SetPosition(const Vect3f& v) override { Position = v; };
};

//----------------------------------------------------

struct fxlabServerKeyObjectType : fxlabServerSpaceType
{
	float* KeyData;
	int KeyID;
	fxlabResourceObject* KeyPoint;
	float KeyTime;

	void Open() override
	{
		fxlabServerSpaceType::Open();
		KeyData = NULL;
		KeyID = -1;
		KeyPoint = NULL;
		KeyTime = 0;
	};

	void Close() override;
	void Start() override;
	void Quant() override;

	void SetKeyID(int id) override { KeyID = id; };

	virtual void KeyCheck(){};
	virtual void KeyUpdate();
};

//-------------------------------------------------
