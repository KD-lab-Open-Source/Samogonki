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

	fxlabClientSetAtomType(){ ObjectType = -1;KeyID = -1;UseFlag = 0; };
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

	void Open() override;
	void Close() override;
	void Start() override;
	void Quant() override;
	void KeyUpdate() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
	int GetAlive() override;

	void ObjectGenerate();
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

	void Open() override;
	void Close() override;
	void Quant() override;

	void SetBody(class Body* p) override;
	void KeyCheck() override;
	void KeyUpdate() override;

	virtual void CalcVelocity() {};
	virtual void Destroy();
};

struct fxlabClientBodyLinkType : fxlabClientBaseBodyType
{
	Vect3f LastPosition;

	void Open() override;
	void CalcVelocity() override;
	void SetBody(class Body* p) override;
};

//---------------------------------------

struct fxlabClientMechosLinkType : fxlabClientBodyLinkType
{
	class Mechos* MechosOwner;
	void Open() override;
	void SetBody(class Body* p) override;
	void Quant() override;
};

struct fxlabClientFrontLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
};

struct fxlabClinetBackLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
};

struct fxlabClinetLeftForwardWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
};

struct fxlabClientRightForwardWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
};

struct fxlabClientLeftBackWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
};

struct fxlabClientRightBackWheelLink : fxlabClientMechosLinkType
{
	void CalcVelocity() override;
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

	void Open() override;
	void Start() override;
	void Quant() override;

	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override { Velocity = v; };

	virtual void CreateObject();
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

	void Open() override;
	void Close() override;
	void Quant() override;

	void KeyCheck() override;

	void CreateObject() override;
	virtual void UpdateObject();
	virtual void DestroyObject();
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

	void Open() override;
	void Close() override;
	void KeyCheck() override;
	void SetRemoteID(int id) override;
	void SetVelocity(const Vect3f& v) override;
	void SetPosition(const Vect3f& v) override;

	virtual void CreateObject();
	virtual void DestroyObject();
};

//----------------------------------------

struct fxlabClientRemoteAim : fxlabClientRemoteCotrol
{
	int TargetID;
	struct mchRacer* Target;

	void Open() override;
	void SetRemoteID(int id) override;
};

//--------------------------------------

struct fxlabClientBaseBindingType : fxlabClientSpaceType
{	
	ShareHandle<Body> Core;
	Vect3f Velocity;

	void Open() override;
	void Quant() override;

	void SetBody(class Body* p) override;

	virtual void CalcVelocity() {};
	virtual void Update() {};
	virtual void Destroy();
};

struct fxlabClientCompareBindingType : fxlabClientBaseBindingType
{
	Vect3f LastPosition;

	void Open() override;
	void CalcVelocity() override;
	void SetBody(class Body* p) override;
};

//-----------------------------------------

struct fxlabClientSuicideLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void CalcVelocity() override;
	void Update() override;
};

//------------------------------

struct fxlabClientBolideLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
//	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
	void Update() override;
};

//----------------------------

struct fxlabClientVolcanicStoneLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};

//----------------------------

struct fxlabClientBombExplodeLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};

//------------------------

struct fxlabClientFireBallLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
	fxlabProcessInterface OmniPoint;

	void Close() override;
	void Update() override;
};

//-------------------------

struct fxlabClientLightningBoltLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};

//---------------------------

struct fxlabClientMortarBodyLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;
	MeasurementTimer Timer;

	void Start() override;
	void Quant() override;
	void Close() override;
	void Update() override;
};

//-------------------

struct fxlabClientBonusBodyLink : fxlabClientMortarBodyLink
{
	void Update() override;
};

//----------------------------

struct fxlabClientSnowFallLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};

struct fxlabClientMechosBurnLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};

struct fxlabClientBombSplinterLink : fxlabClientCompareBindingType
{
	fxlabProcessInterface TailPoint;

	void Close() override;
	void Update() override;
};


//-------------------------------------------------


struct fxlabClientMechosBindingType : fxlabClientSpaceType
{
	class Mechos* MechosOwner;

	void Open() override;
	void Quant() override;
	void SetMechosPoint(class mchMechosRacer* p) override;

	virtual void CalcVelocity(){};
	virtual void Update(){};
	virtual void Destroy();
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

	void Open() override;
	void Close() override;
	void CalcVelocity() override;
	void Update() override;
	void SetAngle(float angle) override { StartAngle = Angle = angle; };
	void SetDeltaAngle(float angle) override { DeltaAngle = angle; };
	void SetRadius(float r) override { Radius = r; };

	void SetTarget(class mchMechosRacer* p) override;
};


