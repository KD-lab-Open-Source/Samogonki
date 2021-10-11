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

	void Open(void);
	void Quant(void);
	void KeyUpdate(void);
	void KeyCheck(void);
	virtual void Action(void);
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

	void Open(void);
	void Action(void);
	void KeyCheck(void);
	void SetVelocity(Vect3f& v){ Velocity = v; };
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

	void KeyUpdate(void);
	void KeyCheck(void);
	void Action(void);
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

	void Quant(void);
	void KeyCheck(void);
	void SetBody(class Body* p);

	virtual void Destroy(void);
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

	void Open(void);
	void Start(void);
	void Close(void);
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

	void Open(void);
	void Start(void);
	void Close(void);

	void Generate(void);

	void SetBonusActionID(int id);
};

struct fxlabServerBonusMarket : fxlabServerBonusSingle
{
	int RespawnTime;
	int WaitFlag;
	DurationTimer RespawnTimer;

	void Open(void);
	void Start(void);
	void Quant(void);

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

	void Open(void);
	void Close(void);
	void Start(void);
	void Action(void);

	void KeyCheck(void);
	void SetRemoteObject(fxlabGeneralObjectType* p);
};

struct fxlabServerDragonHeadFire : fxlabServerDragonFire
{
	void Action(void);
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

	void Open(void);
	void Close(void);
	void Quant(void);

	void KeyCheck(void);
	void KeyUpdate(void);

	void SetRemoteObject(fxlabGeneralObjectType* p);

	virtual void Action(void);
	virtual void CheckTarget(void);
	virtual void CheckAlive(void);
	virtual void TargetDestruction(void);
	virtual void TargetLost(void);
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

	void Action(void);
	void AssignTarget(struct mchRacer* target);
	void KeyCheck(void);
};

//AI-Info
struct fxlabServerPlasmaClaw : fxlabServerWildClaw
{
	class mchMechosRacer* OwnerPoint;

	void Open(void);
	void SetMechosPoint(class mchMechosRacer* p)
	{
		OwnerPoint = p;
	};
	void Action(void);
};


//AI-Info
struct fxlabServerPerestroykaClaw : fxlabServerWildClaw
{
	void CheckTarget(void);

};

//------------------------------------------

//AI-Info
struct fxlabServerBombExplosion : fxlabDamageSensorType
{
	void Start(void);
};

//-----------------------------------------

enum fxlabServerStoneLauncherDataField
{
	FXLAB_SERVER_STONE_LAUNCHER_DATA_RATE = 0,
	FXLAB_SERVER_STONE_LAUNCHER_DATA_MAX
};

struct fxlabServerStoneLauncher : fxlabServerKeyObjectType
{
	void Quant(void);
	void KeyCheck(void);

	virtual void Action(void);
	virtual void Generate(void){};
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
	void Generate(void);
	void KeyCheck(void);
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

	void Open(void);
	void Close(void);
	void Start(void);
	void Action(void);

	void KeyCheck(void);
	void SetRemoteObject(fxlabGeneralObjectType* p);
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

	void Open(void);
	void Close(void);
	void Start(void);
	void Quant(void);
	void KeyCheck(void);
	virtual void Generate(void){};
	void SetPosition(Vect3f& v);
};

struct fxlabVortexField : fxlabFieldLinkType
{
	void Generate(void);
};

struct fxlabMagneticField : fxlabFieldLinkType
{
	void Generate(void);
};

struct fxlabStreamField : fxlabFieldLinkType
{
	Vect3f Velocity;

	void Open(void);
	void Quant(void);
	void SetVelocity(Vect3f& v){ Velocity = v; };
};

struct fxlabFluxField : fxlabStreamField
{
	void Generate(void);
	void Quant(void);
};

struct fxlabWindField : fxlabStreamField
{
	void Generate(void);
	void Quant(void);
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
	void KeyCheck(void);
};

struct fxlabWildThicketCircle : fxlabCircleLinkType
{
	void Generate(void);
	void Quant(void);
};

struct fxlabRageSlimeCircle : fxlabCircleLinkType
{
	void Generate(void);
	void Quant(void);
};

struct fxlabRedTrackCircle : fxlabCircleLinkType
{
	void Generate(void);
};

struct fxlabGreenSlimeCircle : fxlabCircleLinkType
{
	void Generate(void);
	void Quant(void);
};

struct fxlabYellowSlimeCircle : fxlabCircleLinkType
{
	void Generate(void);
	void Quant(void);
};

struct fxlabBlueSlimeCircle : fxlabCircleLinkType
{
	void Generate(void);
	void Quant(void);
};

//-------------------------------------------

struct fxlabServerBowMachineDamage : fxlabServerSuckDamage
{
	void TargetDestruction(void);
};

struct fxlabServerLightningSeedDamage : fxlabServerBowMachineDamage
{
	int SauroneFlag;
	DurationTimer LifeTimer;

	void Action(void);
	void AssignTarget(struct mchRacer* target);
	void TargetLost(void);
};

//--------------------------------------------

struct fxlabServerMassShifter : fxlabServerSpaceType
{
	DurationTimer CountDown;
	class mchMechosRacer* Owner;
	Vect3f Target;
	Vect3f Direction;

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
	virtual void CalcBranch(void);
	virtual void Generate(void);
};

//---------------------------------------------

struct fxlabServerChargeType : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;
	float Value;
	float DeltaValue;
	float ChargeTime;

	void Open(void);
	void Start(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
	void SetChargeValue(float v){ Value = v; };
	void SetChargeTime(float t);
};

struct fxlabServerChargeEnergy : fxlabServerChargeType
{
	fxlabProcessInterface ChargePoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabServerChargeMana : fxlabServerChargeType
{
	fxlabProcessInterface ChargePoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabServerMechosFire : fxlabServerChargeType
{
	fxlabProcessInterface FirePoint;

	void Start(void);
	void Close(void);
	void Quant(void);
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

	void Quant(void);
	void KeyCheck(void);
	void KeyUpdate(void);
	virtual void Action(struct mchRacer* p);
};

struct fxlabServerTrapProcess : fxlabServerMassShifter
{
	void Start(void);
	void Generate(void);
	void CalcBranch(void);
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

	void Open(void);
	void Close(void);

	void Action(void);
	void TargetDestruction(void);
	void TargetLost(void);
	void AssignTarget(struct mchRacer* Target);
	void CheckTarget(void);

	void SetMenuTeleportStatus(int id){ TeleportStatus = id; };
	void SetMenuTeleportInfo(int world_id,int track_id){ WorldID = world_id; TrackID = track_id; };
};

//----------------------------------------------------------

struct fxlabServerSkinProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open(void);
	void Start(void);
	void Quant(void);
	void Close(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
};

struct fxlabServerIsolationProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open(void);
	void Start(void);
	void Quant(void);
	void Close(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
};

struct fxlabServerArmorProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;

	void Open(void);
	void Quant(void);
	void Start(void);
	void Close(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
};

struct fxlabServerBioProtection : fxlabServerEvolutionType
{
	class mchMechosRacer* Owner;
	fxlabProcessInterface SoundPoint;

	void Open(void);
	void Quant(void);
	void Start(void);
	void Close(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void KeyCheck(void);
	void KeyUpdate(void);
	void SetAngle(float angle){ Angle = angle; };
	void SetMechosPoint(class mchMechosRacer* p)
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void SetMechosPoint(class mchMechosRacer* p){ Owner = p; };
};

