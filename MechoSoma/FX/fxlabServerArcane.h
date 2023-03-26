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

	void Open() override;
	void Start() override;
	void Quant() override;
	void Close() override;

	void ProtectionQuant() override {};

	virtual void PowerUse();
	virtual void ArcaneFatality();

	virtual void Condition(){};
	virtual void LinkPowerZero();
	virtual void PowerZero(){ };
	virtual void ActionZero(){ };

	virtual void InterruptAction(){};
	virtual void InterruptUse();

	void SetArcaneLink(struct mchArcaneData* p) override { LinkPoint = p; };
	void SetArcaneOwner(class mchMechosRacer* p) override { Owner = p; };
	void SetArcaneInterruptNum(int num) override { InterruptNum = num; };
	void SetArcaneActionPoint(int num) override { ActionPoint = num; };
	void SetArcaneActionDelay(int time) override { ActionDelay = time; };
	void SetArcaneLifeTime(float time) override { LifeTime = time; };
	void SetArcaneAlive(int arcane_id,int owner_id) override;

	virtual void ArcaneAction() override;
};

//----------------------------------------------------

//InternalArcane

struct fxlabInternalArcane : fxlabGeneralArcaneType
{
	void Start() override;
	void Quant() override;
	void Close() override;

	void Condition() override;
	void LinkPowerZero() override;
	void ActionZero() override;
};

struct fxlabArcaneDragonWind : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcaneDirigibleWind : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcaneButterflyWind : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcaneFrogWind : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcaneHelicopterWind : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcanePlasmaClaw : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint;

	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabArcaneBowMachine : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint;

	void Start() override;
	void Quant() override;
	void Close() override;
	void ArcaneAction() override {};
};

struct fxlabArcaneShaftMachine : fxlabInternalArcane
{
	fxlabProcessInterface ClientPoint;
	fxlabProcessInterface ServerPoint;
	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabArcaneIronSide : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

struct fxlabArcaneBallWind : fxlabInternalArcane
{
	fxlabProcessInterface LeftPoint;
	fxlabProcessInterface RightPoint;
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabRevoltSpace : fxlabInternalArcane
{
	fxlabProcessInterface ClientPoint;

	void Start() override;
	void Close() override;
};

struct fxlabArcaneFireBall : fxlabInternalArcane
{
	void InterruptAction() override;
};

struct fxlabArcaneLightningBolt : fxlabInternalArcane
{
	void InterruptAction() override;
};

/*struct fxlabArcaneJumpBall : fxlabInternalArcane
{
	void InterruptAction();
};*/

struct fxlabArcaneSpiderSpawn : fxlabInternalArcane
{
	void InterruptAction() override;
};

struct fxlabArcaneCaterpillarSpawn : fxlabInternalArcane
{
	void InterruptAction() override;
};

struct fxlabArcaneBubbleRush : fxlabInternalArcane
{
	void InterruptAction() override;
};

const int FXLAB_VOOODOO_MASTER_OBJECT_NUM = 5;

struct fxlabArcaneVoodooMaster : fxlabInternalArcane
{
	fxlabProcessInterface ServerPoint[FXLAB_VOOODOO_MASTER_OBJECT_NUM];

	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabArcaneRedTrack : fxlabInternalArcane
{
	void Start() override;
	void Close() override;
};

//ImpulseArcane

struct fxlabImpulseArcane : fxlabGeneralArcaneType
{
	void Quant() override;
	void Close() override;
};

struct fxlabArcaneFormicaryWind : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneSuicideWarrior : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneClownWarrior : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneEyeWarrior : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneTomatoWarrior : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneKineticHit : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneMassShifter : fxlabImpulseArcane
{
	void Start() override;
};

struct fxlabArcaneJumpBall : fxlabImpulseArcane
{
	void Start() override;
};

//-----------------------

struct fxlabExternalPowerArcane : fxlabGeneralArcaneType
{
	void Start() override;
	void PowerUse() override;
	void LinkPowerZero() override;
	void ActionZero() override;
	void PowerZero() override;
};

struct fxlabArcaneFireStone : fxlabExternalPowerArcane
{
	Vect3f Velocity;
	DurationTimer BulletTimer;
	fxlabProcessInterface BulletPoint;
	fxlabProcessInterface CloudPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
//	void InterruptAction();
};

struct fxlabArcaneSnowFall : fxlabExternalPowerArcane
{
	Vect3f Velocity;
	DurationTimer BulletTimer;
	fxlabProcessInterface BulletPoint;
	fxlabProcessInterface CloudPoint;
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
};

struct fxlabArcaneElecricStorm : fxlabExternalPowerArcane
{
	fxlabProcessInterface CloudPoint;
	fxlabProcessInterface MarkerPoint;

	void Start() override;
	void Close() override;
	void InterruptAction() override;
};

const int FXLAB_FIRE_GARDEN_ROSE_NUM = 5;

struct fxlabArcaneFireGarden : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint[FXLAB_FIRE_GARDEN_ROSE_NUM];
	fxlabProcessInterface ClientPoint[FXLAB_FIRE_GARDEN_ROSE_NUM];
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
};

struct fxlabArcaneRageSlime : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint;
	fxlabProcessInterface ClientPoint;

	fxlabProcessInterface SoundPoint;
	DurationTimer WaitTimer;

	void Start() override;
	void Close() override;
	void Quant() override;
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

	void Open() override;
	void Start() override;
	void Close() override;
	void ArcaneAction() override;
	void Quant() override;
};

struct fxlabArcaneShiftTrap : fxlabExternalPowerArcane
{
	fxlabProcessInterface ServerPoint;
	fxlabProcessInterface ClientPoint;
	fxlabProcessInterface SoundPoint;

	void Start() override;
	void Close() override;
};

//---------------------------------------

struct fxlabExternalMindArcane : fxlabGeneralArcaneType
{
	int InterruptCount;
	int InterruptEnable;
	int ExternalTime;

	void Open() override;
	void Close() override;
	void Start() override;
	void Condition() override;
	void InterruptUse() override;
	void PowerUse() override;
	void LinkPowerZero() override;
	void ActionZero() override;
	void PowerZero() override;
	void SetArcaneTime(int time) override { ExternalTime = time; };
};

struct fxlabArcaneLightningSeed : fxlabExternalMindArcane
{
	fxlabProcessInterface* ServerPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
	void Condition() override;
	void InterruptAction() override;
};

struct fxlabArcaneCrystalSpike : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > SpikePoint;

	void Start() override;
	void Close() override;
	void Quant() override;
	void Condition() override;
	void InterruptAction() override;
};

struct fxlabArcaneMortarMine : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > MortarPoint;
	fxlabProcessInterface* ClientPoint;

	void Start() override;
	void Close() override;
	void Quant() override;
	void Condition() override;
	void InterruptAction() override;
};

const int FXLAB_PETARD_BONUS_NUM = 5;

struct fxlabArcanePetard : fxlabExternalMindArcane
{
	vector<ShareHandle<Body> > BonusPoint;
	fxlabProcessInterface ClientPoint[FXLAB_PETARD_BONUS_NUM];
	fxlabProcessInterface PetardPoint;
	Vect3f ExplodePosition;
	Vect3f Velocity;

	void Start() override;
	void Close() override;
	void Quant() override;
	void Condition() override;
	void InterruptAction() override;
};

struct fxlabArcaneScarletTrack : fxlabExternalMindArcane
{
	fxlabProcessInterface* ServerPoint;
	fxlabProcessInterface* ClientPoint;
	Vect3f LastPosition;

	void Start() override;
	void Close() override;
	void Quant() override;
	void InterruptUse() override;
};

