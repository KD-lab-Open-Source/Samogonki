const int FXLAB_ARCANE_DEMO_WORLD = 7;

struct fxlabGeneralArcaneType : fxlabServerSpaceType
{
	int ArcaneID;
	int RacerID,MechosID;
	mchArcaneData* LinkPoint;
	mchMechosRacer* Owner;
	int PartNumber;

	DurationTimer ActionTimer;
	int ActionStatus;
	int ActionPoint;
	int ActionDelay;

	float PowerTime;
	float DeltaPower;

	int InterruptNum;
	float InterruptBorder,InterruptTime;

	float LifeTime;

	DurationTimer CurrentProtectionTimer;

	void Open(void);
	void Start(void);
	void Quant(void);
	void Close(void);

	void ProtectionQuant(void){};

	virtual void PowerUse(void);
	virtual void ArcaneFatality(void);

	virtual void Condition(void){};
	virtual void LinkPowerZero(void);
	virtual void PowerZero(void){ };
	virtual void ActionZero(void){ };

	virtual void InterruptAction(void){};
	virtual void InterruptUse(void);

	void SetArcaneLink(struct mchArcaneData* p){ LinkPoint = p; };
	void SetArcaneOwner(class mchMechosRacer* p){ Owner = p; };
	void SetArcaneInterruptNum(int num){ InterruptNum = num; };
	void SetArcaneActionPoint(int num){ ActionPoint = num; };
	void SetArcaneActionDelay(int time){ ActionDelay = time; };
	void SetArcaneLifeTime(float time){ LifeTime = time; };
	void SetArcaneAlive(int arcane_id,int owner_id);

	virtual void ArcaneAction(void);
};

//----------------------------------------------------

//InternalArcane

struct fxlabInternalArcane : fxlabGeneralArcaneType
{
	void Start(void);
	void Quant(void);
	void Close(void);

	void Condition(void);
	void LinkPowerZero(void);
	void ActionZero(void);
};

struct fxlabArcaneDragonWind : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcaneDirigibleWind : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcaneButterflyWind : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcaneFrogWind : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcaneHelicopterWind : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcanePlasmaClaw : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint;

	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabArcaneBowMachine : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint;

	void Start(void);
	void Quant(void);
	void Close(void);
	void ArcaneAction(void){};
};

struct fxlabArcaneShaftMachine : fxlabInternalArcane
{
	fxlabProcessInterface ClientPoint;
	fxlabProcessInterface ServerPoint;
	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabArcaneIronSide : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

struct fxlabArcaneBallWind : fxlabInternalArcane
{
	fxlabProcessInterface LeftPoint;
	fxlabProcessInterface RightPoint;
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabRevoltSpace : fxlabInternalArcane
{
	fxlabProcessInterface ClientPoint;

	void Start(void);
	void Close(void);
};

struct fxlabArcaneFireBall : fxlabInternalArcane
{
	void InterruptAction(void);
};

struct fxlabArcaneLightningBolt : fxlabInternalArcane
{
	void InterruptAction(void);
};

/*struct fxlabArcaneJumpBall : fxlabInternalArcane
{
	void InterruptAction(void);
};*/

struct fxlabArcaneSpiderSpawn : fxlabInternalArcane
{
	void InterruptAction(void);
};

struct fxlabArcaneCaterpillarSpawn : fxlabInternalArcane
{
	void InterruptAction(void);
};

struct fxlabArcaneBubbleRush : fxlabInternalArcane
{
	void InterruptAction(void);
};

const int FXLAB_VOOODOO_MASTER_OBJECT_NUM = 5;

struct fxlabArcaneVoodooMaster : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint[FXLAB_VOOODOO_MASTER_OBJECT_NUM];

	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabArcaneRedTrack : fxlabInternalArcane
{
	void Start(void);
	void Close(void);
};

//ImpulseArcane

struct fxlabImpulseArcane : fxlabGeneralArcaneType
{
	void Quant(void);
	void Close(void);
};

struct fxlabArcaneFormicaryWind : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneSuicideWarrior : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneClownWarrior : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneEyeWarrior : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneTomatoWarrior : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneKineticHit : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneMassShifter : fxlabImpulseArcane
{
	void Start(void);
};

struct fxlabArcaneJumpBall : fxlabImpulseArcane
{
	void Start(void);
};

//-----------------------

struct fxlabExternalPowerArcane : fxlabGeneralArcaneType
{
	void Start(void);
	void PowerUse(void);
	void LinkPowerZero(void);
	void ActionZero(void);
	void PowerZero(void);
};

struct fxlabArcaneFireStone : fxlabExternalPowerArcane
{
	Vect3f Velocity;
	DurationTimer BulletTimer;
	fxlabProcessInterface BulletPoint;
	fxlabProcessInterface CloudPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
//	void InterruptAction(void);
};

struct fxlabArcaneSnowFall : fxlabExternalPowerArcane
{
	Vect3f Velocity;
	DurationTimer BulletTimer;
	fxlabProcessInterface BulletPoint;
	fxlabProcessInterface CloudPoint;
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
};

struct fxlabArcaneElecricStorm : fxlabExternalPowerArcane
{
	fxlabProcessInterface CloudPoint;
	fxlabProcessInterface MarkerPoint;

	void Start(void);
	void Close(void);
	void InterruptAction(void);
};

const int FXLAB_FIRE_GARDEN_ROSE_NUM = 5;

struct fxlabArcaneFireGarden : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint[FXLAB_FIRE_GARDEN_ROSE_NUM];
	fxlabProcessInterface ClientPoint[FXLAB_FIRE_GARDEN_ROSE_NUM];
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
};

struct fxlabArcaneRageSlime : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint;
	fxlabProcessInterface ClientPoint;

	fxlabProcessInterface SoundPoint;
	DurationTimer WaitTimer;

	void Start(void);
	void Close(void);
	void Quant(void);
};

const int FXLAB_FURY_FUNGUS_NUM = 17;

struct fxlabArcaneFuryFungus : fxlabExternalPowerArcane
{
	fxlabProcessInterface LashPoint[FXLAB_FURY_FUNGUS_NUM];
	fxlabProcessInterface FogPoint;
	fxlabProcessInterface CirclePoint;
	fxlabProcessInterface SoundPoint;
	DurationTimer LifeTimer;
	int CheckLife;

	void Open(void);
	void Start(void);
	void Close(void);
	void ArcaneAction(void);
	void Quant(void);
};

struct fxlabArcaneShiftTrap : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint;
	fxlabProcessInterface ClientPoint;
	fxlabProcessInterface SoundPoint;

	void Start(void);
	void Close(void);
};

//---------------------------------------

struct fxlabExternalMindArcane : fxlabGeneralArcaneType
{
	int InterruptCount;
	int InterruptEnable;
	int ExternalTime;

	void Open(void);
	void Close(void);
	void Start(void);
	void Condition(void);
	void InterruptUse(void);
	void PowerUse(void);
	void LinkPowerZero(void);
	void ActionZero(void);
	void PowerZero(void);
	void SetArcaneTime(int time){ ExternalTime = time; };
};

struct fxlabArcaneLightningSeed : fxlabExternalMindArcane
{
	fxlabProcessInterface* ServerPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
	void Condition(void);
	void InterruptAction(void);
};

struct fxlabArcaneCrystalSpike : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > SpikePoint;

	void Start(void);
	void Close(void);
	void Quant(void);
	void Condition(void);
	void InterruptAction(void);
};

struct fxlabArcaneMortarMine : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > MortarPoint;
	fxlabProcessInterface* ClientPoint;

	void Start(void);
	void Close(void);
	void Quant(void);
	void Condition(void);
	void InterruptAction(void);
};

const int FXLAB_PETARD_BONUS_NUM = 5;

struct fxlabArcanePetard : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > BonusPoint;
	fxlabProcessInterface ClientPoint[FXLAB_PETARD_BONUS_NUM];
	fxlabProcessInterface PetardPoint;
	Vect3f ExplodePosition;
	Vect3f Velocity;

	void Start(void);
	void Close(void);
	void Quant(void);
	void Condition(void);
	void InterruptAction(void);
};

struct fxlabArcaneScarletTrack : fxlabExternalMindArcane
{
	fxlabProcessInterface* ServerPoint;
	fxlabProcessInterface* ClientPoint;
	Vect3f LastPosition;

	void Start(void);
	void Close(void);
	void Quant(void);
	void InterruptUse(void);
};

