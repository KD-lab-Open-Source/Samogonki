enum fxlabCheckPointDataField
{
	FXLAB_CHECK_POINT_DATA_RADIUS = 0,
	FXLAB_CHECK_POINT_DATA_RED,
	FXLAB_CHECK_POINT_DATA_GREEN,
	FXLAB_CHECK_POINT_DATA_BLUE,
	FXLAB_CHECK_POINT_DATA_ALPHA,
	FXLAB_CHECK_POINT_DATA_DELTA_ANGLE,
	FXLAB_CHECK_POINT_DATA_MOVE_PHASE,
	FXLAB_CHECK_POINT_DATA_PHASE_DELTA,
	FXLAB_CHECK_POINT_DATA_PHASE_STEP,
	FXLAB_CHECK_POINT_DATA_MAX
};

struct fxlabCheckPoint : fxlabClientSpaceType
{
	int Mode;	

	Vect3f AnglePosition;
	float Scale;
	float ModelRadius;

	float AddPhase;
	Vect3f Velocity;

	int ModelIndex;
	int ModelID[2];
	cMesh* ModelPoint[2];

	fxlabResourceObject* KeyPoint[FXLAB_CHECK_POINT_MODE_MAX];
	float KeyData[FXLAB_CHECK_POINT_DATA_MAX];
	int KeyStatus;

	int nCamera;

	void Open() override;
	void Close() override;
	void Start() override;
	void Quant() override;
	void StopQuant() override;

	void ModelUpdate();
	void KeyUpdate();

	void SetScale(float scale) override;
	void SetMode(int mode) override;
	void SetVelocity(const Vect3f& velocity) override;
	void SetCheckPointModels(int id0,int id1) override;
	void SetCheckPointRacer(int mode,struct mchRacer* p,int init_flag) override;
	void SetAngle(float angle) override;
	void SetCheckPointCamera(int camera) override;
};


//-----------------------

enum fxlabPolarModelDataField
{
	FXLAB_POLAR_MODEL_DATA_PSI = 0,
	FXLAB_POLAR_MODEL_DATA_THETTA,
	FXLAB_POLAR_MODEL_DATA_RADIUS,
	FXLAB_POLAR_MODEL_DATA_PHASE,
	FXLAB_POLAR_MODEL_DATA_ALPHA,
	FXLAB_POLAR_MODEL_DATA_SCALE,
	FXLAB_POLAR_MODEL_DATA_MAX
};

struct fxlabPolarModel : fxlabClientKeyObjectType
{
	int ModelID;
	Vect3f ModelScale;
	Vect3f ModelAngle;
	cMesh* ModelPoint;

	Vect3f Color;
	float Alpha;
	Vect3f StartPosition;

	struct mchRacer* Center;

	float OffsetAngle;

	int nCamera;

	void Open() override;
	void Close() override;
	void Quant() override;
	void Start() override;

	void KeyCheck() override;

	void SetScaleVector(const Vect3f& v) override;
	void SetAngleVector(const Vect3f& v) override;
	void SetColorVector(const Vect3f& v) override;
	void SetTransparency(float a) override;
	void SetModel(int id) override;
	void SetRacerPoint(struct mchRacer* p) override;
	void SetAngle(float angle) override;

	virtual int CalcAttribute();
	void SetCheckPointCamera(int camera) override;
};

//-----------------------------------------------

const int FXLAB_WAVEGROUND_RANGE_MIN = -FXLAB_WAVEGROUND_RANGE;
const int FXLAB_WAVEGROUND_RANGE_MAX = FXLAB_WAVEGROUND_RANGE;

const int FXLAB_WAVEGROUND_FORCE_SHIFT = 16;
const int FXLAB_WAVEGROUND_FORCE = 1 << FXLAB_WAVEGROUND_FORCE_SHIFT;

struct fxlabWaveGround : fxlabClientEvolutionType
{
	int Red,Green,Blue;
	int Alpha;

	int DeltaRed,DeltaGreen,DeltaBlue;
	int DeltaAlpha;
	int DeltaHeight;
	int DeltaUV;

	class cPolyGrid* PolyGrid;
	struct sTileMap* TilePoint;

	void Open() override;
	void Start() override;
	void Quant() override;

	void SetGridPoint(void* pTileMap,void* pPolyGrid) override;

	void SetGridRed(int red,int delta_red) override { Red = red; DeltaRed = delta_red; };
	void SetGridGreen(int green,int delta_green) override { Green = green; DeltaGreen = delta_green; };
	void SetGridBlue(int blue,int delta_blue) override{ Blue = blue; DeltaBlue = delta_blue; };
	void SetGridAlpha(int alpha,int delta_alpha) override{ Alpha = alpha; DeltaAlpha = delta_alpha; };
	void SetGridHeight(int z) override { DeltaHeight = z; };
	void SetGridUV(float uv) override;
};

//------------------------------------------------------

enum fxlabStreamClusterDataField
{
	FXLAB_STREAM_CLUSTER_DATA_NUM = 0,
	FXLAB_STREAM_CLUSTER_DATA_RATE,
	FXLAB_STREAM_CLUSTER_DATA_RADIUS,
	FXLAB_STREAM_CLUSTER_DATA_PSI,
	FXLAB_STREAM_CLUSTER_DATA_DELTA_PSI,
	FXLAB_STREAM_CLUSTER_DATA_THETTA,
	FXLAB_STREAM_CLUSTER_DATA_KEY_ID,
	FXLAB_STREAM_CLUSTER_DATA_MAX
};

struct fxlabStreamCluster : fxlabClientKeyObjectType
{
	int Num;
	fxlabProcessInterface* StreamPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void KeyCheck() override;
};

struct fxlabClientStreamFountain : fxlabStreamCluster
{
	Mat3f StreamMatrix;

	void Open() override;
	void Start() override;
	void SetDirectionMatrix(const class Mat3f& m) override { StreamMatrix = m; };
	void Quant() override;
};

//--------------------------------------------------------

enum fxlabClientWildClawDataField
{
	FXLAB_CLIENT_WILD_CLAW_DATA_RADIUS = 0,
	FXLAB_CLIENT_WILD_CLAW_DATA_ANGLE,
	FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS0,
	FXLAB_CLIENT_WILD_CLAW_DATA_POINT_RADIUS1,
	FXLAB_CLINET_WILD_CLAW_DATA_DELTA_PHASE,
	FXLAB_CLIENT_WILD_CLAW_DATA_ERECTION_TIME,
	FXLAB_CLIENT_WILD_CLAW_DATA_SPLINE_TYPE,
	FXLAB_CLIENT_WILD_CLAW_DATA_SPLINE_ID,
	FXLAB_CLIENT_WILD_CLAW_DATA_MAX
};

struct fxlabClientWildClaw : fxlabClientRemoteAim
{
	Vect3f Point[4];
	std::list<Vect3f> Spline;
	fxlabProcessInterface SplinePoint;

	DurationTimer ErectionTimer;

	int Alpha;
	Vect3f Velocity;

	Vect3f PointPhase;
	float ScaleFactor;

	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Close() override;
	void Start() override;
	void Quant() override;

	void KeyCheck() override;

	void CalcCenter();

	void SetVelocity(const Vect3f& v) override { Velocity = v; };
	void SetRemoteID(int id) override;
	int GetAlive() override;

	virtual void CalcVelocity(){};
};

struct fxlabClientWildClawFrontLink : fxlabClientWildClaw
{
	class Mechos* MechosOwner;

	void Open() override;
	void SetBody(class Body* p) override;
	void CalcVelocity() override;
};

//------------------------------------

enum fxlabClientBowMachineDataField
{
	FXLAB_CLIENT_BOW_MACHINE_DATA_ACTIVE_TYPE = 0,
	FXLAB_CLIENT_BOW_MACHINE_DATA_ACTIVE_ID,
	FXLAB_CLIENT_BOW_MACHINE_DATA_PASSIVE_TYPE,
	FXLAB_CLIENT_BOW_MACHINE_DATA_PASSIVE_ID,
	FXLAB_CLIENT_BOW_MACHINE_DATA_MAX
};

struct fxlabClientBowMachine : fxlabClientRemoteAim
{
	fxlabProcessInterface ActivePoint;
	fxlabProcessInterface PassivePoint;
	fxlabProcessInterface SparkPoint;
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
	void Quant() override;

	void KeyCheck() override;
	virtual void CalcVelocity(){};
	virtual void EnablePhase();
	virtual void DisablePhase();
};

struct fxlabClientBowMachineFrontLink : fxlabClientBowMachine
{
	class Mechos* MechosOwner;
	fxlabProcessInterface BackgroundPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void SetBody(class Body* p) override;
	void CalcVelocity() override;
	void EnablePhase() override;
	void DisablePhase() override;
};

struct fxlabClientLightningSeed : fxlabClientBowMachine
{
	fxlabProcessInterface BackgroundPoint;

	void Start() override;
	void Close() override;
	void EnablePhase() override;
	void DisablePhase() override;
};

//------------------------------

enum fxlabClientBallWindDataField
{
	FXLAB_CLIENT_BALL_WIND_DATA_SPEED = 0,
	FXLAB_CLIENT_BALL_WIND_DATA_MAX
};

struct fxlabClientBallWindLink : fxlabClientKeyObjectType
{
	Vect3f Velocity;
	class Mechos* MechosOwner;
	DurationTimer StartTimer;
	fxlabProcessInterface ActivePoint;
	fxlabProcessInterface PassivePoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void SetBody(class Body* p) override;
	void KeyCheck() override;
	virtual void CalcVelocity(){};
	virtual void CreateActive();
	virtual void CreatePassive();
};

struct fxlabClientBallWindLinkLeft : fxlabClientBallWindLink
{
	void CalcVelocity() override;
};

struct fxlabClientBallWindLinkRight : fxlabClientBallWindLink
{
	void CalcVelocity() override;
};

//----------------------------------

enum fxlabClientVoodooMasterField
{
	FXLAB_CLIENT_VOODOO_MASTER_DATA_RADIUS = 0,
	FXLAB_CLIENT_VOODOO_MASTER_DATA_DELTA_ANGLE,
	FXLAB_CLIENT_VOODOO_MASTER_DATA_MAX
};

const int FXLAB_CLIENT_VOODOO_HEAD_NUM = 5;

struct fxlabClientVoodooMaster : fxlabClientKeyObjectType
{
	float Angle;

	fxlabProcessInterface HeadPoint[FXLAB_CLIENT_VOODOO_HEAD_NUM];
	fxlabProcessInterface TailPoint[FXLAB_CLIENT_VOODOO_HEAD_NUM];
	fxlabProcessInterface OmniPoint[FXLAB_CLIENT_VOODOO_HEAD_NUM];

	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;

	void KeyCheck() override;
};

//---------------------------------

enum fxlabTerrainDeformatorDataField
{
	FXLAB_TERRAIN_DEFORMATOR_DATA_AMPLITUDE = 0,
	FXLAB_TERRAIN_DEFORMATOR_DATA_RADIUS,
	FXLAB_TERRAIN_DEFORMATOR_DATA_DELTA,
	FXLAB_TERRAIN_DEFORMATOR_DATA_MAX
};

struct fxlabTerrainDeformator : fxlabClientKeyObjectType
{
	void Quant() override;
	void KeyCheck() override;
};

//-----------------------------------

struct fxlabClientRevoltSpaceLink : fxlabClientMechosLinkType
{
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;

	void CalcVelocity() override;
	void Quant() override;
};

//-----------------------------------

enum fxlabClientMassShifterDataField
{
	FXLAB_CLIENT_MASS_SHIFTER_DATA_RATE = 0,
	FXLAB_CLIENT_MASS_SHIFTER_DATA_RADIUS,
	FXLAB_CLIENT_MASS_SHIFTER_DATA_MAX
};

struct fxlabClientMassShifter : fxlabClientKeyObjectType
{
	void Quant() override;
	void KeyCheck() override;
	virtual void Generate(const Mat3f& m,const Vect3f& v);
};


struct fxlabClientMassShifterNitro : fxlabClientMassShifter
{
	void Generate(const Mat3f& m,const Vect3f& v) override;
};

struct fxlabClientMassShifterCharacter : fxlabClientMassShifter
{
	void Generate(const Mat3f& m,const Vect3f& v) override;
};

struct fxlabClientMassShifterMovie : fxlabClientMassShifter
{
	void Generate(const Mat3f& m,const Vect3f& v) override;
};

//-------------------------------------

enum fxlabClientActionTypeDataField
{
	FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED = FXLAB_CLIENT_BODY_LINK_DATA_MAX,
	FXLAB_CLIENT_ACTION_TYPE_DATA_MAX
};

struct fxlabClientActionType : fxlabClientMechosLinkType
{
	void KeyCheck() override;
};

const int FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL = 7;

struct fxlabClientJumpActionLink : fxlabClientActionType
{
	int ModelCount;
	fxlabProcessInterface ModelData[FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL];
	DurationTimer DelayTimer;
	Vect3f zVelocity;
	MatXf zMatrix;

	void Start() override;
	void Quant() override;
	void Close() override;
	void CalcVelocity() override;
	virtual void CreateModel();
};

struct fxlabClientNitroActionLink : fxlabClientJumpActionLink
{
	void CalcVelocity() override;
	void CreateModel() override;
};


struct fxlabClientJumpActionLeft : fxlabClientActionType
{
	void CalcVelocity() override;
};

struct fxlabClientJumpActionRight : fxlabClientActionType
{
	void CalcVelocity() override;
};

struct fxlabClientNitroActionLeft : fxlabClientActionType
{
	void CalcVelocity() override;
};

struct fxlabClientNitroActionRight : fxlabClientActionType
{
	void CalcVelocity() override;
};

struct fxlabClientMechosFire : fxlabClientActionType
{
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
	void CalcVelocity() override;
};

//-----------------------------

struct fxlabClientMechosController : fxlabClientEvolutionType
{
	class mchMechosRacer* Owner;
	DurationTimer WaveTimer;
	fxlabProcessInterface DamageWarningPoint;
	fxlabProcessInterface EnergyPoint;
	fxlabProcessInterface ManaPoint;

	void Open() override;
	void Close() override;
	void Quant() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

//---------------------------------------------------

enum fxlabClientColorControllerDataField
{
	FXLAB_CLIENT_COLOR_CONTROLLER_DATA_RED = 0,
	FXLAB_CLIENT_COLOR_CONTROLLER_DATA_GREEN,
	FXLAB_CLIENT_COLOR_CONTROLLER_DATA_BLUE,
	FXLAB_CLIENT_COLOR_CONTROLLER_DATA_ALPHA,
	FXLAB_CLIENT_COLOR_CONTROLLER_DATA_MAX
};

struct fxlabClientColorController : fxlabClientKeyObjectType
{
	class mchMechosRacer* Owner;

	void Open() override;
	void Quant() override;
	void KeyCheck() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

struct fxlabClientPartController : fxlabClientColorController
{
	int ControllerPartID;
	int ControllerArcaneID;

	void Open() override;
	void Quant() override;
//	void CalcKeyTime();

	void SetControllerPartID(int id) override { ControllerPartID = id; };
	void SetControllerArcaneID(int id) override { ControllerArcaneID = id; };
};

struct fxlabClientTeleportColorController : fxlabClientColorController
{
	float RestartTime;

	void Start() override;
	int GetAlive() override;
	void CalcKeyTime() override;
};

struct fxlabClientTeleportOmniController : fxlabControlOmni
{
	float RestartTime;

	void Start() override;
	int GetAlive() override;
	void CalcKeyTime() override;
};

//-----------------------------------------------------

struct fxlabClientHeadDustLauncher : fxlabClientObjectLauncher
{
	void CreateObject() override;
};

struct fxlabClientShaftMachineLink : fxlabClientFrontLink
{
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
	void CalcVelocity() override;
};

struct fxlabClientDragonFireSwitcher : fxlabClientRemoteSwitcher
{
	fxlabProcessInterface SoundPoint;

	void CreateObject() override;
	void DestroyObject() override;
	void Close() override;
};

struct fxlabClientDragonHeadFireSwitcher : fxlabClientRemoteSwitcher
{
	void CreateObject() override;
};

//---------------------

enum fxlabClientWorldIrradiateDataField
{
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_RADIUS = 0,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_X,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_Y,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ANGLE_Z,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_RED,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_GREEN,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_BLUE,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_ALPHA,
	FXLAB_CLIENT_WORLD_IRRADIATE_DATA_MAX
};

struct fxlabClientWorldIrradiate : fxlabClientKeyObjectType
{
	float Angle;
	int FireEnable;
	DurationTimer FireTimer;
	class mchMechosRacer* Owner;

	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface WavePoint;
	fxlabProcessInterface DeformationPoint;

	void Open() override;
	void Start() override;
	void Quant() override;
	void Show() override;
	void Close() override;
	void KeyCheck() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

//-------------------- Movies -----------------

enum fxlabClientSetFaceFireDataField
{
	FXLAB_CLIENT_SET_FACE_FIRE_SPEED = 0,	
	FXLAB_CLIENT_SET_FACE_FIRE_MAX
};

struct fxlabClientSetFaceFire : fxlabClientKeyObjectType
{
	Vect3f Velocity;
	fxlabProcessInterface FirePoint;
	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
};

enum fxlabClientMovieBowRepeaterDataField
{
	FXLAB_CLIENT_MOVIE_BOW_REPEATER_SPEED = 0,	
	FXLAB_CLIENT_MOVIE_BOW_REPEATER_MAX
};

struct fxlabClientMovieBowRepeater : fxlabClientKeyObjectType
{
	Vect3f Velocity;
	MeasurementTimer Timer;

	void Open() override;
	void Start() override;
	void Quant() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
	virtual void CreateBow();
};

struct fxlabClientMovieWaveRepeater : fxlabClientMovieBowRepeater
{
	int Cnt;
	void Start() override;
	void CreateBow() override;
};


struct fxlabClientMovieQuandLight : fxlabClientSpaceType
{
//	fxlabProcessInterface SoundPoint;
	fxlabProcessInterface LightPoint;
	fxlabProcessInterface BowPoint;
	fxlabProcessInterface SparkPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabClientMovieClouds : fxlabClientSpaceType
{
	fxlabProcessInterface SoundPoint;
	fxlabProcessInterface CloudPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabClientMovieShifting : fxlabClientSpaceType
{
	fxlabProcessInterface ShiftPoint;
	fxlabProcessInterface OmniPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabClientMovieTeleport : fxlabClientSpaceType
{
	fxlabProcessInterface WindPoint;

	void Start() override;
	void Close() override;
};

struct fxlabClientMovieSpot : fxlabClientSpaceType
{
	fxlabProcessInterface SpotPoint;

	void Start() override;
	void Close() override;
};

struct fxlabClientFireTreeMovie : fxlabClientSpaceType
{
	fxlabProcessInterface FirePoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabClientMovieBossChange : fxlabClientSpaceType
{
	int DeformationFlag;
	DurationTimer DeformationTimer;
	fxlabProcessInterface DeformationPoint;
	fxlabProcessInterface OmniPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabClientWindSound : fxlabClientSpaceType
{				  
	int ScreamFlag;
	DurationTimer WindTimer;
	DurationTimer ScreamTimer;

	void Start() override;
	void Quant() override;
};

struct fxlabClientMovieDragonFire : fxlabClientSpaceType
{
	Vect3f Velocity;
	fxlabProcessInterface FirePoint;
	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void SetVelocity(const Vect3f& v) override;
};

//-----------------------------------

struct fxlabClientCharacterWaveGenerator : fxlabCrazyDust
{
	void CoreGenerate() override;
};

//-----------------------------------

struct fxlabClientCharacterSteamGenerator : fxlabCrazyTail
{
	void CoreGenerate() override;
};

//--------------------------

struct fxlabClientCharacterSmokeGenerator : fxlabCrazyDust
{
	void CoreGenerate() override;
};

//-------------------------

struct fxlabClientCharacterWaterfallGenerator : fxlabSparkGenerator
{
	int BigBossWorldMask;

	void Open() override;
	void CoreGenerate() override;
	void SetBigBossWorldStatus(int world) override;
};

//------------------------

struct fxlabClientBossWorldBurst : fxlabClientSpaceType
{
	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface BurstPoint;

	void Start() override;
	void Close() override;
};

struct fxlabClientBossWorldExplosion : fxlabClientSpaceType
{
	fxlabProcessInterface ExplosionPoint;

	void Start() override;
	void Close() override;
};

struct fxlabClientBossRocketFire : fxlabClientSpaceType
{
	Vect3f Velocity;
	Vect3f Normal;

	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface SmokePoint;
	fxlabProcessInterface FirePoint;
	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void SetVelocity(const Vect3f& v) override;

};
