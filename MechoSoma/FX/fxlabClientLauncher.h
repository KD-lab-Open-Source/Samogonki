enum fxlabClientSetAtomDataField
{
	FXLAB_CLIENT_SET_ATOM_DATA_TYPE = 0,
	FXLAB_CLIENT_SET_ATOM_DATA_KEY_ID,
	FXLAB_CLIENT_SET_ATOM_DATA_MAX
};

struct fxlabClientSetAtomType
{
	int ObjectType;
	int KeyID;
	fxlabProcessInterface ObjectPoint;
	int UseFlag;

	fxlabClientSetAtomType(void){ ObjectType = -1;KeyID = -1;UseFlag = 0; };
};

enum fxlabClientSetObjectDataField
{
	FXLAB_CLIENT_SET_OBJECT_MODE = 0,	
	FXLAB_CLIENT_SET_OBJECT_VELOCITYX,
	FXLAB_CLIENT_SET_OBJECT_VELOCITYY,
	FXLAB_CLIENT_SET_OBJECT_VELOCITYZ,
	FXLAB_CLIENT_SET_OBJECT_NUM,
	FXLAB_CLINET_SET_OBJECT_MAX
};

struct fxlabClientSetObjectType : fxlabClientKeyObjectType
{
	int Mode;
	int NumAtom;
	int ActiveCnt;
	fxlabClientSetAtomType* AtomData;
	Vect3f Velocity;

	void Open(void);
	void Close(void);
	void Start(void);
	void Quant(void);
	void KeyUpdate(void);
	void KeyCheck(void);
	void SetVelocity(const Vect3f& v) override;
	int GetAlive(void);

	void ObjectGenerate(void);
};

//-----------------------------------------

enum fxlabClientBodyLinkDataField
{
	FXLAB_CLIENT_BODY_LINK_DATA_TAIL_TYPE = 0,
	FXLAB_CLIENT_BODY_LINK_DATA_TAIL_ID,
	FXLAB_CLIENT_BODY_LINK_DATA_EXPLODE_TYPE,
	FXLAB_CLIENT_BODY_LINK_DATA_EXPLODE_ID,
	FXLAB_CLIENT_BODY_LINK_DATA_BORDER_SPEED,
	FXLAB_CLIENT_BODY_LINK_DATA_MAX
};

struct fxlabClientBaseBodyType : fxlabClientKeyObjectType
{	
	int TailType,TailID;
	int ExplodeType,ExplodeID;

	ShareHandle<Body> Core;

	Vect3f Velocity;
	fxlabProcessInterface TailPoint;

	void Open(void);
	void Close(void);
	void Quant(void);

	void SetBody(class Body* p);
	void KeyCheck(void);
	void KeyUpdate(void);

	virtual void CalcVelocity(void){};
	virtual void Destroy(void);
};

struct fxlabClientBodyLinkType : fxlabClientBaseBodyType
{
	Vect3f LastPosition;

	void Open(void);
	void CalcVelocity(void);
	void SetBody(class Body* p);
};

//---------------------------------------

struct fxlabClientMechosLinkType : fxlabClientBodyLinkType
{
	class Mechos* MechosOwner;
	void Open(void);
	void SetBody(class Body* p);
	void Quant(void);
};

struct fxlabClientFrontLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};

struct fxlabClinetBackLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};

struct fxlabClinetLeftForwardWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};

struct fxlabClientRightForwardWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};

struct fxlabClientLeftBackWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};

struct fxlabClientRightBackWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity(void);
};
												
//----------------------------------------------

enum fxlabClientObjectLauncherDataField
{
	FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_TYPE = 0,
	FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_ID,
	FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_START_TIME,
	FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_DISABLE_TIME,
	FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_MAX
};

struct fxlabClientObjectLauncher : fxlabClientKeyObjectType
{
	DurationTimer ObjectTimer;
	Vect3f Velocity;

	void Open(void);
	void Start(void);
	void Quant(void);

	void KeyCheck(void);
	void SetVelocity(const Vect3f& v) override { Velocity = v; };

	virtual void CreateObject(void);
};

enum fxlabClientObjectSwitcherDataField
{
	FXLAB_CLIENT_OBJECT_SWITCHER_DATA_ENABLE_TIME = FXLAB_CLIENT_OBJECT_LAUNCHER_DATA_MAX,
	FXLAB_CLIENT_OBJECT_SWITCHER_DATA_MAX
};

struct fxlabClientObjectSwitcher : fxlabClientObjectLauncher
{
	int EnableFlag;
	Vect3f Velocity;
	fxlabProcessInterface ObjectPoint;

	void Open(void);
	void Close(void);
	void Quant(void);

	void KeyCheck(void);

	void CreateObject(void);
	virtual void UpdateObject(void);
	virtual void DestroyObject(void);
};

//------------------------------------

enum fxlabClientRemoteSwitcherDataField
{
	FXLAB_CLIENT_REMOTE_SWITCHER_DATA_TYPE = 0,
	FXLAB_CLIENT_REMOTE_SWITCHER_DATA_ID,
	FXLAB_CLIENT_REMOTE_SWITCHER_MAX
};

struct fxlabClientRemoteSwitcher : fxlabClientRemoteCotrol
{
	fxlabProcessInterface ObjectPoint;
	Vect3f Velocity;

	void Open(void);
	void Close(void);
	void KeyCheck(void);
	void SetRemoteID(int id);
	void SetVelocity(const Vect3f& v) override;
	void SetPosition(const Vect3f& v) override;

	virtual void CreateObject(void);
	virtual void DestroyObject(void);
};

//----------------------------------------

struct fxlabClientRemoteAim : fxlabClientRemoteCotrol
{
	int TargetID;
	struct mchRacer* Target;

	void Open(void);
	void SetRemoteID(int id);
};

//--------------------------------------

struct fxlabClientBaseBindingType : fxlabClientSpaceType
{	
	ShareHandle<Body> Core;
	Vect3f Velocity;

	void Open(void);
	void Quant(void);

	void SetBody(class Body* p);

	virtual void CalcVelocity(void){};
	virtual void Update(void){};
	virtual void Destroy(void);
};

struct fxlabClientCompareBindingType : fxlabClientBaseBindingType
{
	Vect3f LastPosition;

	void Open(void);
	void CalcVelocity(void);
	void SetBody(class Body* p);
};

//-----------------------------------------

struct fxlabClientSuicideLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void CalcVelocity(void);
	void Update(void);
};

//------------------------------

struct fxlabClientBolideLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
//	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
	void Update(void);
};

//----------------------------

struct fxlabClientVolcanicStoneLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};

//----------------------------

struct fxlabClientBombExplodeLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};

//------------------------

struct fxlabClientFireBallLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
	fxlabProcessInterface OmniPoint;

	void Close(void);
	void Update(void);
};

//-------------------------

struct fxlabClientLightningBoltLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};

//---------------------------

struct fxlabClientMortarBodyLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
	MeasurementTimer Timer;

	void Start(void);
	void Quant(void);
	void Close(void);
	void Update(void);
};

//-------------------

struct fxlabClientBonusBodyLink : fxlabClientMortarBodyLink
{
	void Update(void);
};

//----------------------------

struct fxlabClientSnowFallLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};

struct fxlabClientMechosBurnLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};

struct fxlabClientBombSplinterLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close(void);
	void Update(void);
};


//-------------------------------------------------


struct fxlabClientMechosBindingType : fxlabClientSpaceType
{
	class Mechos* MechosOwner;

	void Open(void);
	void Quant(void);
	void SetMechosPoint(class mchMechosRacer* p);

	virtual void CalcVelocity(void){};
	virtual void Update(void){};
	virtual void Destroy(void);
};

//----------------

struct fxlabClientVoodooMasterLink : fxlabClientMechosBindingType
{
	float StartAngle;
	float Angle;
	float DeltaAngle;
	float Radius;

	fxlabProcessInterface ModelPoint;
	fxlabProcessInterface TailPoint;
	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface SoundPoint;

	int LockTarget;
	Vect3f LastPosition;
	float ThetaVelocity;
	float RadiusVelocity;
	float PsiVelocity;

	void Open(void);
	void Close(void);
	void CalcVelocity(void);
	void Update(void);
	void SetAngle(float angle){ StartAngle = Angle = angle; };
	void SetDeltaAngle(float angle){ DeltaAngle = angle; };
	void SetRadius(float r){ Radius = r; };

	void SetTarget(class mchMechosRacer* p);
};


