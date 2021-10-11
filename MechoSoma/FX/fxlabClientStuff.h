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

	void Open(void);
	void Close(void);
	void Start(void);
	void Quant(void);
	void StopQuant(void);

	void ModelUpdate(void);
	void KeyUpdate(void);

	void SetScale(float scale);
	void SetMode(int mode);
	void SetVelocity(Vect3f& velocity);
	void SetCheckPointModels(int id0,int id1);
	void SetCheckPointRacer(int mode,struct mchRacer* p,int init_flag);
	void SetAngle(float angle);
	void SetCheckPointCamera(int camera);
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

	void Open(void);
	void Close(void);
	void Quant(void);
	void Start(void);

	void KeyCheck(void);

	void SetScaleVector(Vect3f& v);
	void SetAngleVector(Vect3f& v);
	void SetColorVector(Vect3f& v);
	void SetTransparency(float a);
	void SetModel(int id);
	void SetRacerPoint(struct mchRacer* p);
	void SetAngle(float angle);

	virtual int CalcAttribute(void);
	void SetCheckPointCamera(int camera);
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

	void Open(void);
	void Start(void);
	void Quant(void);

	void SetGridPoint(void* pTileMap,void* pPolyGrid);

	void SetGridRed(int red,int delta_red){ Red = red; DeltaRed = delta_red; };
	void SetGridGreen(int green,int delta_green){ Green = green; DeltaGreen = delta_green; };
	void SetGridBlue(int blue,int delta_blue){ Blue = blue; DeltaBlue = delta_blue; };
	void SetGridAlpha(int alpha,int delta_alpha){ Alpha = alpha; DeltaAlpha = delta_alpha; };
	void SetGridHeight(int z){ DeltaHeight = z; };
	void SetGridUV(float uv);
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void KeyCheck(void);
};

struct fxlabClientStreamFountain : fxlabStreamCluster
{
	Mat3f StreamMatrix;

	void Open(void);
	void Start(void);
	void SetDirectionMatrix(class Mat3f& m){ StreamMatrix = m; };
	void Quant(void);
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
	list<Vect3f> Spline;
	fxlabProcessInterface SplinePoint;

	DurationTimer ErectionTimer;

	int Alpha;
	Vect3f Velocity;

	Vect3f PointPhase;
	float ScaleFactor;

	fxlabProcessInterface SoundPoint;

	void Open(void);
	void Close(void);
	void Start(void);
	void Quant(void);

	void KeyCheck(void);

	void CalcCenter(void);

	void SetVelocity(Vect3f& v){ Velocity = v; };
	void SetRemoteID(int id);
	int GetAlive(void);

	virtual void CalcVelocity(void){};
};

struct fxlabClientWildClawFrontLink : fxlabClientWildClaw
{
	class Mechos* MechosOwner;

	void Open(void);
	void SetBody(class Body* p);
	void CalcVelocity(void);
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

	void Start(void);
	void Close(void);
	void Quant(void);

	void KeyCheck(void);
	virtual void CalcVelocity(void){};
	virtual void EnablePhase(void);
	virtual void DisablePhase(void);
};

struct fxlabClientBowMachineFrontLink : fxlabClientBowMachine
{
	class Mechos* MechosOwner;
	fxlabProcessInterface BackgroundPoint;

	void Open(void);
	void Start(void);
	void Close(void);
	void SetBody(class Body* p);
	void CalcVelocity(void);
	void EnablePhase(void);
	void DisablePhase(void);
};

struct fxlabClientLightningSeed : fxlabClientBowMachine
{
	fxlabProcessInterface BackgroundPoint;

	void Start(void);
	void Close(void);
	void EnablePhase(void);
	void DisablePhase(void);
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void SetBody(class Body* p);
	void KeyCheck(void);
	virtual void CalcVelocity(void){};
	virtual void CreateActive(void);
	virtual void CreatePassive(void);
};

struct fxlabClientBallWindLinkLeft : fxlabClientBallWindLink
{
	void CalcVelocity(void);
};

struct fxlabClientBallWindLinkRight : fxlabClientBallWindLink
{
	void CalcVelocity(void);
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);

	void KeyCheck(void);
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
	void Quant(void);
	void KeyCheck();
};

//-----------------------------------

struct fxlabClientRevoltSpaceLink : fxlabClientMechosLinkType
{
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);

	void CalcVelocity(void);
	void Quant(void);
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
	void Quant(void);
	void KeyCheck(void);
	virtual void Generate(Mat3f& m,Vect3f& v);
};


struct fxlabClientMassShifterNitro : fxlabClientMassShifter
{
	void Generate(Mat3f& m,Vect3f& v);
};

struct fxlabClientMassShifterCharacter : fxlabClientMassShifter
{
	void Generate(Mat3f& m,Vect3f& v);
};

struct fxlabClientMassShifterMovie : fxlabClientMassShifter
{
	void Generate(Mat3f& m,Vect3f& v);
};

//-------------------------------------

enum fxlabClientActionTypeDataField
{
	FXLAB_CLIENT_ACTION_TYPE_DATA_SPEED = FXLAB_CLIENT_BODY_LINK_DATA_MAX,
	FXLAB_CLIENT_ACTION_TYPE_DATA_MAX
};

struct fxlabClientActionType : fxlabClientMechosLinkType
{
	void KeyCheck(void);
};

const int FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL = 7;

struct fxlabClientJumpActionLink : fxlabClientActionType
{
	int ModelCount;
	fxlabProcessInterface ModelData[FXLAB_CLIENT_JUMP_ACTION_NUM_MODEL];
	DurationTimer DelayTimer;
	Vect3f zVelocity;
	MatXf zMatrix;

	void Start(void);
	void Quant(void);
	void Close(void);
	void CalcVelocity(void);
	virtual void CreateModel(void);
};

struct fxlabClientNitroActionLink : fxlabClientJumpActionLink
{
	void CalcVelocity(void);
	void CreateModel(void);
};


struct fxlabClientJumpActionLeft : fxlabClientActionType
{
	void CalcVelocity(void);
};

struct fxlabClientJumpActionRight : fxlabClientActionType
{
	void CalcVelocity(void);
};

struct fxlabClientNitroActionLeft : fxlabClientActionType
{
	void CalcVelocity(void);
};

struct fxlabClientNitroActionRight : fxlabClientActionType
{
	void CalcVelocity(void);
};

struct fxlabClientMechosFire : fxlabClientActionType
{
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
	void CalcVelocity(void);
};

//-----------------------------

struct fxlabClientMechosController : fxlabClientEvolutionType
{
	class mchMechosRacer* Owner;
	DurationTimer WaveTimer;
	fxlabProcessInterface DamageWarningPoint;
	fxlabProcessInterface EnergyPoint;
	fxlabProcessInterface ManaPoint;

	void Open(void);
	void Close(void);
	void Quant(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
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

	void Open(void);
	void Quant(void);
	void KeyCheck(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
};

struct fxlabClientPartController : fxlabClientColorController
{
	int ControllerPartID;
	int ControllerArcaneID;

	void Open(void);
	void Quant(void);
//	void CalcKeyTime(void);

	void SetControllerPartID(int id){ ControllerPartID = id; };
	void SetControllerArcaneID(int id){ ControllerArcaneID = id; };
};

struct fxlabClientTeleportColorController : fxlabClientColorController
{
	float RestartTime;

	void Start(void);
	int GetAlive(void);
	void CalcKeyTime(void);
};

struct fxlabClientTeleportOmniController : fxlabControlOmni
{
	float RestartTime;

	void Start(void);
	int GetAlive(void);
	void CalcKeyTime(void);
};

//-----------------------------------------------------

struct fxlabClientHeadDustLauncher : fxlabClientObjectLauncher
{
	void CreateObject(void);
};

struct fxlabClientShaftMachineLink : fxlabClientFrontLink
{
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
	void CalcVelocity(void);
};

struct fxlabClientDragonFireSwitcher : fxlabClientRemoteSwitcher
{
	fxlabProcessInterface SoundPoint;

	void CreateObject(void);
	void DestroyObject(void);
	void Close(void);
};

struct fxlabClientDragonHeadFireSwitcher : fxlabClientRemoteSwitcher
{
	void CreateObject(void);
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

	void Open(void);
	void Start(void);
	void Quant(void);
	void Show(void);
	void Close(void);
	void KeyCheck(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void KeyCheck(void);
	void SetVelocity(Vect3f& v);
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

	void Open(void);
	void Start(void);
	void Quant(void);
	void KeyCheck(void);
	void SetVelocity(Vect3f& v);
	virtual void CreateBow(void);
};

struct fxlabClientMovieWaveRepeater : fxlabClientMovieBowRepeater
{
	int Cnt;
	void Start(void);
	void CreateBow(void);
};


struct fxlabClientMovieQuandLight : fxlabClientSpaceType
{
//	fxlabProcessInterface SoundPoint;
	fxlabProcessInterface LightPoint;
	fxlabProcessInterface BowPoint;
	fxlabProcessInterface SparkPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabClientMovieClouds : fxlabClientSpaceType
{
	fxlabProcessInterface SoundPoint;
	fxlabProcessInterface CloudPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabClientMovieShifting : fxlabClientSpaceType
{
	fxlabProcessInterface ShiftPoint;
	fxlabProcessInterface OmniPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabClientMovieTeleport : fxlabClientSpaceType
{
	fxlabProcessInterface WindPoint;

	void Start(void);
	void Close(void);
};

struct fxlabClientMovieSpot : fxlabClientSpaceType
{
	fxlabProcessInterface SpotPoint;

	void Start(void);
	void Close(void);
};

struct fxlabClientFireTreeMovie : fxlabClientSpaceType
{
	fxlabProcessInterface FirePoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabClientMovieBossChange : fxlabClientSpaceType
{
	int DeformationFlag;
	DurationTimer DeformationTimer;
	fxlabProcessInterface DeformationPoint;
	fxlabProcessInterface OmniPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabClientWindSound : fxlabClientSpaceType
{				  
	int ScreamFlag;
	DurationTimer WindTimer;
	DurationTimer ScreamTimer;

	void Start(void);
	void Quant(void);
};

struct fxlabClientMovieDragonFire : fxlabClientSpaceType
{
	Vect3f Velocity;
	fxlabProcessInterface FirePoint;
	fxlabProcessInterface SoundPoint;

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void SetVelocity(Vect3f& v);
};

//-----------------------------------

struct fxlabClientCharacterWaveGenerator : fxlabCrazyDust
{
	void CoreGenerate(void);
};

//-----------------------------------

struct fxlabClientCharacterSteamGenerator : fxlabCrazyTail
{
	void CoreGenerate(void);
};

//--------------------------

struct fxlabClientCharacterSmokeGenerator : fxlabCrazyDust
{
	void CoreGenerate(void);
};

//-------------------------

struct fxlabClientCharacterWaterfallGenerator : fxlabSparkGenerator
{
	int BigBossWorldMask;

	void Open(void);
	void CoreGenerate(void);
	void SetBigBossWorldStatus(int world);
};

//------------------------

struct fxlabClientBossWorldBurst : fxlabClientSpaceType
{
	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface BurstPoint;

	void Start(void);
	void Close(void);
};

struct fxlabClientBossWorldExplosion : fxlabClientSpaceType
{
	fxlabProcessInterface ExplosionPoint;

	void Start(void);
	void Close(void);
};

struct fxlabClientBossRocketFire : fxlabClientSpaceType
{
	Vect3f Velocity;
	Vect3f Normal;

	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface SmokePoint;
	fxlabProcessInterface FirePoint;
	fxlabProcessInterface SoundPoint;

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void SetVelocity(Vect3f& v);

};
