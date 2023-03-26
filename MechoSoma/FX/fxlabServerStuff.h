enum fxlabDamageSensorDataField
{
	FXLAB_DAMAGE_SENSOR_DATA_POWER = 0,
	FXLAB_DAMAGE_SENSOR_DATA_RADIUS,
	FXLAB_DAMAGE_SENSOR_DATA_TYPE,
	FXLAB_DAMAGE_SENSOR_DATA_MAX
};

struct fxlabDamageSensorType : fxlabServerKeyObjectType
{
	int SensorType;
	float Power;
	float Radius;

	void Open() override;
	void Quant() override;
	void KeyUpdate() override;
	void KeyCheck() override;
	virtual void Action();
};

//--------------------------------------------------

enum fxlabDamageLineDataField
{
	FXLAB_DAMAGE_LINE_DATA_LENGTH = FXLAB_DAMAGE_SENSOR_DATA_MAX,
	FXLAB_DAMAGE_LINE_DATA_MAX
};

struct fxlabDamageLineType : fxlabDamageSensorType
{
	Vect3f Velocity;

	void Open() override;
	void Action() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override { Velocity = v; };
};

//--------------------------------------------------

enum fxlabDamageCircleDataField
{
	FXLAB_DAMAGE_CIRCLE_DATA_MIN_RADIUS = FXLAB_DAMAGE_SENSOR_DATA_MAX,
	FXLAB_DAMAGE_CIRCLE_DATA_HEIGHT,
	FXLAB_DAMAGE_CIRCLE_DATA_MAX
};

struct fxlabDamageCircleType : fxlabDamageSensorType
{
	float MinRadius;
	float Height;

	void KeyUpdate() override;
	void KeyCheck() override;
	void Action() override;
};

//--------------------------------------------------

enum fxlabServerBodyLinkDataField
{
	FXLAB_SERVER_BODY_LINK_DATA_TYPE = 0,
	FXLAB_SERVER_BODY_LINK_DATA_ID,
	FXLAB_SERVER_BODY_LINK_DATA_MAX
};

struct fxlabServerBodyLinkType : fxlabServerKeyObjectType
{
	ShareHandle<Body> Core;

	void Quant() override;
	void KeyCheck() override;
	void SetBody(class Body* p) override;

	virtual void Destroy();
};

//-----------------------------------------------------

/*struct fxlabServerBonusType : fxlabServerSpaceType
{
	fxlabProcessInterface ModelPoint;
	fxlabProcessInterface* LinkPoint;

	int BonusModelType;
	int BonusModelID;
	int BonusActionID;
	int BonusPartID;

	void Open();
	void Start();
	void Close();
	void SetAlive(int alive);

	void SetBonusModelType(int type){ BonusModelType = type; };
	void SetBonusModelID(int id){ BonusModelID = id; };
	void SetBonusActionID(int id){ BonusActionID = id; };
	void SetBonusPartID(int id){ BonusPartID = id; };
};

struct fxlabServerBonusSingle : fxlabServerSpaceType
{
	int BonusActionID;
	int BonusPartID;
	fxlabProcessInterface BonusPoint;

	void Open();
	void Start();
	void Close();

	void Generate();

	void SetBonusActionID(int id);
};

struct fxlabServerBonusMarket : fxlabServerBonusSingle
{
	int RespawnTime;
	int WaitFlag;
	DurationTimer RespawnTimer;

	void Open();
	void Start();
	void Quant();

	void SetBonusRespawnTime(int time){ RespawnTime = time; };
};
*/
//-----------------------------------------------------------

enum fxlabServerDragonFireDataField
{
	FXLAB_SERVER_DRAGON_FIRE_DATA_START_TIME = FXLAB_DAMAGE_LINE_DATA_MAX,
	FXLAB_SERVER_DRAGON_FIRE_DATA_ENABLE_TIME,
	FXLAB_SERVER_DRAGON_FIRE_DATA_DISABLE_TIME,
	FXLAB_SERVER_DRAGON_FIRE_DATA_MAX
};

struct fxlabServerDragonFire : fxlabDamageLineType
{
	int EnableFlag;
	DurationTimer FireTimer;
	fxlabProcessInterface RemotePoint;

	void Open() override;
	void Close() override;
	void Start() override;
	void Action() override;

	void KeyCheck() override;
	void SetRemoteObject(fxlabGeneralObjectType* p) override;
};

struct fxlabServerDragonHeadFire : fxlabServerDragonFire
{
	void Action() override;
};

//----------------------------------------------------------------

enum fxlabServerSuckDamageField
{
	FXLAB_SERVER_SUCK_DAMAGE_POWER = 0,
	FXLAB_SERVER_SUCK_DAMAGE_TYPE,
	FXLAB_SERVER_SUCK_DAMAGE_ACTIVE_RADIUS,
	FXLAB_SERVER_SUCK_DAMAGE_PASSIVE_RADIUS,
	FXLAB_SERVER_SUCK_DAMAGE_DATA_MAX
};

struct fxlabServerSuckDamage : fxlabServerKeyObjectType
{
	int DamageType;
	float Power;
	int ActiveRadius2;
	int PassiveRadius2;
	struct mchRacer* Target;
	fxlabProcessInterface RemotePoint;

	void Open() override;
	void Close() override;
	void Quant() override;

	void KeyCheck() override;
	void KeyUpdate() override;

	void SetRemoteObject(fxlabGeneralObjectType* p) override;

	virtual void Action();
	virtual void CheckTarget();
	virtual void CheckAlive();
	virtual void TargetDestruction();
	virtual void TargetLost();
	virtual void AssignTarget(struct mchRacer* Target);
};

//---------------------------------------------

enum fxlabServerWildClawField
{
	FXLAB_SERVER_WILD_CLAW_ERECTION_TIME = FXLAB_SERVER_SUCK_DAMAGE_DATA_MAX,
	FXLAB_SERVER_WILD_CLAW_DATA_MAX
};

//AI-Info
struct fxlabServerWildClaw : fxlabServerSuckDamage
{
	DurationTimer ErectionTimer;

	void Action() override;
	void AssignTarget(struct mchRacer* target) override;
	void KeyCheck() override;
};

//AI-Info
struct fxlabServerPlasmaClaw : fxlabServerWildClaw
{
	class mchMechosRacer* OwnerPoint;

	void Open() override;
	void SetMechosPoint(class mchMechosRacer* p) override
	{
		OwnerPoint = p;
	};
	void Action() override;
};


//AI-Info
struct fxlabServerPerestroykaClaw : fxlabServerWildClaw
{
	void CheckTarget() override;

};

//------------------------------------------

//AI-Info
struct fxlabServerBombExplosion : fxlabDamageSensorType
{
	void Start() override;
};

//-----------------------------------------

enum fxlabServerStoneLauncherDataField
{
	FXLAB_SERVER_STONE_LAUNCHER_DATA_RATE = 0,
	FXLAB_SERVER_STONE_LAUNCHER_DATA_MAX
};

struct fxlabServerStoneLauncher : fxlabServerKeyObjectType
{
	void Quant() override;
	void KeyCheck() override;

	virtual void Action();
	virtual void Generate(){};
};

//-----------------------------------------

enum fxlabServerVolcanoLauncherDataField
{
	FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_SPEED = FXLAB_SERVER_STONE_LAUNCHER_DATA_MAX,
	FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_DELTA_SPEED,
	FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_THETTA,
	FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_DELTA_THETTA,
	FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_MAX
};

struct fxlabServerVolcanoLauncher : fxlabServerStoneLauncher
{
	void Generate() override;
	void KeyCheck() override;
};

//--------------------------------------------

enum fxlabServerDragonRoseDataField
{
	FXLAB_SERVER_DRAGON_ROSE_DATA_START_TIME = FXLAB_DAMAGE_SENSOR_DATA_MAX,
	FXLAB_SERVER_DRAGON_ROSE_DATA_ENABLE_TIME,
	FXLAB_SERVER_DRAGON_ROSE_DATA_DISABLE_TIME,
	FXLAB_SERVER_DRAGON_ROSE_DATA_MAX
};

//Ai-Info
struct fxlabServerDragonRose : fxlabDamageSensorType
{
	int EnableFlag;
	DurationTimer FireTimer;
	fxlabProcessInterface RemotePoint;

	void Open() override;
	void Close() override;
	void Start() override;
	void Action() override;

	void KeyCheck() override;
	void SetRemoteObject(fxlabGeneralObjectType* p) override;
};

//------------------------------------------------

enum fxlabFieldLinkDataField
{
	FXLAB_FIELD_LINK_DATA_POWER = 0,
	FXLAB_FIELD_LINK_DATA_RADIUS,
	FXLAB_FIELD_LINK_DATA_MAX
};

//AI-Info
struct fxlabFieldLinkType : fxlabServerKeyObjectType
{
	class FieldSource* FieldPoint;

	void Open() override;
	void Close() override;
	void Start() override;
	void Quant() override;
	void KeyCheck() override;
	virtual void Generate(){};
	void SetPosition(const Vect3f& v) override;
};

struct fxlabVortexField : fxlabFieldLinkType
{
	void Generate() override;
};

struct fxlabMagneticField : fxlabFieldLinkType
{
	void Generate() override;
};

struct fxlabStreamField : fxlabFieldLinkType
{
	Vect3f Velocity;

	void Open() override;
	void Quant() override;
	void SetVelocity(const Vect3f& v) override { Velocity = v; };
};

struct fxlabFluxField : fxlabStreamField
{
	void Generate() override;
	void Quant() override;
};

struct fxlabWindField : fxlabStreamField
{
	void Generate() override;
	void Quant() override;
};

//--------------------------------------

enum fxlabCircleLinkDataField
{
	FXLAB_CIRCLE_LINK_DATA_MIN_Z = FXLAB_FIELD_LINK_DATA_MAX,
	FXLAB_CIRCLE_LINK_DATA_MAX_Z,
	FXLAB_CIRCLE_LINK_DATA_MAX
};

struct fxlabCircleLinkType : fxlabFieldLinkType
{
	void KeyCheck() override;
};

struct fxlabWildThicketCircle : fxlabCircleLinkType
{
	void Generate() override;
	void Quant() override;
};

struct fxlabRageSlimeCircle : fxlabCircleLinkType
{
	void Generate() override;
	void Quant() override;
};

struct fxlabRedTrackCircle : fxlabCircleLinkType
{
	void Generate() override;
};

struct fxlabGreenSlimeCircle : fxlabCircleLinkType
{
	void Generate() override;
	void Quant() override;
};

struct fxlabYellowSlimeCircle : fxlabCircleLinkType
{
	void Generate() override;
	void Quant() override;
};

struct fxlabBlueSlimeCircle : fxlabCircleLinkType
{
	void Generate() override;
	void Quant() override;
};

//-------------------------------------------

struct fxlabServerBowMachineDamage : fxlabServerSuckDamage
{
	void TargetDestruction() override;
};

struct fxlabServerLightningSeedDamage : fxlabServerBowMachineDamage
{
	int SauroneFlag;
	DurationTimer LifeTimer;

	void Action() override;
	void AssignTarget(struct mchRacer* target) override;
	void TargetLost() override;
};

//--------------------------------------------

struct fxlabServerMassShifter : fxlabServerSpaceType
{
	DurationTimer CountDown;
	class mchMechosRacer* Owner;
	Vect3f Target;
	Vect3f Direction;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
	virtual void CalcBranch();
	virtual void Generate();
};

//---------------------------------------------

struct fxlabServerChargeType : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;
	float Value;
	float DeltaValue;
	float ChargeTime;

	void Open() override;
	void Start() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
	void SetChargeValue(float v) override { Value = v; };
	void SetChargeTime(float t) override;
};

struct fxlabServerChargeEnergy : fxlabServerChargeType
{
	fxlabProcessInterface ChargePoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabServerChargeMana : fxlabServerChargeType
{
	fxlabProcessInterface ChargePoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabServerMechosFire : fxlabServerChargeType
{
	fxlabProcessInterface FirePoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

//------------------------------------

enum fxlabTrapShereDataField
{
	FXLAB_TRAP_SPHERE_DATA_RADIUS = 0,
	FXLAB_TRAP_SPHERE_DATA_HEIGHT,
	FXLAB_TRAP_SPHERE_DATA_MAX
};

struct fxlabServerTrapSphere : fxlabServerKeyObjectType
{
	float Radius;
	float Height;

	void Quant() override;
	void KeyCheck() override;
	void KeyUpdate() override;
	virtual void Action(struct mchRacer* p);
};

struct fxlabServerTrapProcess : fxlabServerMassShifter
{
	void Start() override;
	void Generate() override;
	void CalcBranch() override;
};

//----------------------------------------------------------

struct fxlabServerStaticTeleport : fxlabServerSuckDamage
{
	int WorldID,TrackID;
	int TeleportStatus;

	int UseFlag;
//	DurationTimer PrepareTimer;
//	DurationTimer LockTimer;
	DurationTimer DelayTimer;
	int OutFlag;

	struct mchRacer* LastTarget;

	fxlabProcessInterface OmniPoint;
	fxlabProcessInterface ColorPoint;

	void Open() override;
	void Close() override;

	void Action() override;
	void TargetDestruction() override;
	void TargetLost() override;
	void AssignTarget(struct mchRacer* Target) override;
	void CheckTarget() override;

	void SetMenuTeleportStatus(int id) override { TeleportStatus = id; };
	void SetMenuTeleportInfo(int world_id,int track_id) override { WorldID = world_id; TrackID = track_id; };
};

//----------------------------------------------------------

struct fxlabServerSkinProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open() override;
	void Start() override;
	void Quant() override;
	void Close() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

struct fxlabServerIsolationProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open() override;
	void Start() override;
	void Quant() override;
	void Close() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

struct fxlabServerArmorProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open() override;
	void Quant() override;
	void Start() override;
	void Close() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

struct fxlabServerBioProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;
	fxlabProcessInterface SoundPoint;

	void Open() override;
	void Quant() override;
	void Start() override;
	void Close() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

//-----------------------------

enum fxlabServerVoodooHeadDataField
{
	FXLAB_SERVER_VOODOO_HEAD_DATA_POWER = 0,
	FXLAB_SERVER_VOODOO_HEAD_DATA_RADIUS,
	FXLAB_SERVER_VOODOO_HEAD_DATA_MAX
};

struct fxlabServerVoodooHead : fxlabServerKeyObjectType
{
	float Angle;
	float Power;
	float Radius;
	float Distance;
	float DeltaAngle;
	fxlabProcessInterface HeadPoint;
	class mchMechosRacer* OwnerPoint;

	DurationTimer FireTimer;
	class mchMechosRacer* TargetPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void KeyCheck() override;
	void KeyUpdate() override;
	void SetAngle(float angle) override { Angle = angle; };
	void SetMechosPoint(class mchMechosRacer* p) override
	{
		OwnerPoint = p;
	};

};

//--------------------

struct fxlabServerFrozenType : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;
	fxlabProcessInterface ColorPoint;
	DurationTimer ColorTimer;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

