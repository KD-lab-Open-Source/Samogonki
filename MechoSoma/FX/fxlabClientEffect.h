enum fxlabSwarmDomainData
{
	FXLAB_SWARM_DOMAIN_TYPE = 0,
	FXLAB_SWARM_DOMAIN_DATA_X,
	FXLAB_SWARM_DOMAIN_DATA_Y,
	FXLAB_SWARM_DOMAIN_DATA_Z,
	FXLAB_SWARM_DOMAIN_DATA_A1,
	FXLAB_SWARM_DOMAIN_DATA_B1,
	FXLAB_SWARM_DOMAIN_DATA_C1,
	FXLAB_SWARM_DOMAIN_DATA_A2,
	FXLAB_SWARM_DOMAIN_DATA_B2,
	FXLAB_SWARM_DOMAIN_DATA_C2,
	FXLAB_SWARM_DOMAIN_DATA_MAX
};

struct fxlabSwarmDomain
{
	int Type;
	Vect3f P1,P2;
	Vect3f U,V;
	float Radius1;
	float Radius2;
	float Num;

	void Load(fxlabResourceObject* key,float time);
	void Get(Vect3f& d);
	void Copy(fxlabSwarmDomain* p)
	{
		Type = p->Type;
		P1 = p->P1;
		P2 = p->P2;
		U = p->U;
		V = p->V;
		Radius1 = p->Radius1;
		Radius2 = p->Radius2;
	};

	void Point(float x,float y,float z);
	void Line(float x1,float y1,float z1,float x2,float y2,float z2);
	void Sphere(float x,float y,float z,float r1,float r2);
	void Circle(float x1,float y1,float z1,float r1,float r2);
	void Cone(float x1,float y1,float z1,float x2,float y2,float z2,float r1,float r2);
	void Cylinder(float x1,float y1,float z1,float x2,float y2,float z2,float r1,float r2);
	void PolarSphere(float x1,float y1,float z1,float r,float a,float p,float r1,float r2);
	void IsoSphere(float x,float y,float z,float r1,float r2);
	void Box(float x1,float y1,float z1,float x2,float y2,float z2);
};

//---------------------------------------------

enum fxlabFearDataField
{
	FXLAB_FEAR_DATA_IMAGE_TYPE = FXLAB_PARTICLE_CORE_DATA_MAX,
	FXLAB_FEAR_DATA_IMAGE_ID,
	FXLAB_FEAR_DATA_IMAGE_NOISE,
	FXLAB_FEAR_DATA_MAX
};

struct fxlabParticleFear : fxlabParticleCore
{
	int ImageType;
	int ImageID;
	int ImageNoise;

//-----------
/*	class cMesh* ModelPoint;
	void Start();
	void Close();*/
//-----------

	void Show() override;
	void KeyCheck() override;
	void KeyUpdate() override;
};

//-----------------------------------------

enum fxlabFearSpringDataField
{
	FXLAB_FEAR_SPRING_DATA_RATE = FXLAB_FEAR_DATA_MAX,
	FXLAB_FEAR_SPRING_DATA_AGE,
	FXLAB_FEAR_SPRING_DATA_START_POWER,
	FXLAB_FEAR_SPRING_DATA_END_POWER,
	FXLAB_FEAR_SPRING_DATA_POSITION_KEY_ID,
	FXLAB_FEAR_SPRING_DATA_VELOCITY_KEY_ID,
	FXLAB_FEAR_SPRING_DATA_MAX
};

struct fxlabFearSpring : fxlabParticleFear
{
	float AddRateFactor;
	float Rate;
	float Age;
	float StartPower,EndPower;

	fxlabSwarmDomain EmitterPosition;
	fxlabSwarmDomain EmitterVelocity;

	void Open() override;
	void CoreGenerate() override;
	void KeyCheck() override;
	void KeyUpdate() override;
};

//-------------------------------------------

enum fxlabFearTwisterDataField
{
	FXLAB_FEAR_TWISTER_DATA_NUM_BRANCH = FXLAB_FEAR_DATA_MAX,
	FXLAB_FEAR_TWISTER_DATA_NUM_WING,
	FXLAB_FEAR_TWISTER_DATA_START_HEIGHT,
	FXLAB_FEAR_TWISTER_DATA_END_HEIGHT,
	FXLAB_FEAR_TWISTER_DATA_START_RADIUS,
	FXLAB_FEAR_TWISTER_DATA_END_RADIUS,
	FXLAB_FEAR_TWISTER_DATA_START_POWER,
	FXLAB_FEAR_TWISTER_DATA_END_POWER,
	FXLAB_FEAR_TWISTER_DATA_START_BRANCH_POWER,
	FXLAB_FEAR_TWISTER_DATA_END_BRANCH_POWER,
	FXLAB_FEAR_TWISTER_DATA_AGE,
	FXLAB_FEAR_TWISTER_DATA_NOISE,
	FXLAB_FEAR_TWISTER_DATA_EMITTER_SPEED,
	FXLAB_FEAR_TWISTER_DATA_EMITTER_ANGLE,
	FXLAB_FEAR_TWISTER_DATA_MAX
};

struct fxlabCrazyTwister : fxlabParticleFear
{
	float StartHeight,EndHeight;
	float StartRadius,EndRadius;
	float Age;
	float EmitterSpeed;
	float EmitterAngle;
	float StartPowerBranch,EndPowerBranch;
	int NumBranch;
	int NumWing;
	float StartPower,EndPower;
	float Noise;

	float OffsetAngle;

	void Open() override;
	void Show() override;
	void ConvertPosition() override;

	void CoreProcess() override;
	void CoreGenerate() override;
	void KeyUpdate() override;
	void KeyCheck() override;
};

//---------------------------------------------------------------

struct fxlabRubberSnake : fxlabParticleFear
{
	float Conversion;
	list<Vect3f>* Spline;

	void Open() override;

	void CoreGenerate() override;
	void SetConversion(int conv) override;

	void SetSplineOwner(void* point) override;
	int GetAlive() override { return Alive; };
};


//------------------------------------------------------

enum fxlabFearExplodeDataField
{
	FXLAB_FEAR_EXPLODE_DATA_POINT_POWER1 = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_FEAR_EXPLODE_DATA_POINT_EPSILON1,
	FXLAB_FEAR_EXPLODE_DATA_POINT_POWER2,
	FXLAB_FEAR_EXPLODE_DATA_POINT_EPSILON2,
	FXLAB_FEAR_EXPLODE_DATA_MAX
};

struct fxlabCrazyExplode : fxlabFearSpring
{
	float PointPower1,PointEpsilon1;
	float PointPower2,PointEpsilon2;

	void CoreProcess() override;
	void KeyUpdate() override;
	void KeyCheck() override;
};

//--------------------------------------------------------

enum fxlabFearTailDataField
{
	FXLAB_FEAR_TAIL_DATA_POINT_POWER = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_FEAR_TAIL_DATA_POINT_EPSILON,
	FXLAB_FEAR_TAIL_DATA_POINT_RADIUS,
	FXLAB_FEAR_TAIL_DATA_MAX
};

struct fxlabCrazyTail : fxlabFearSpring
{
	float PointPower;
	float PointRadius;
	float PointEpsilon;

	Vect3f Velocity;	

	void Open() override;
	void CoreProcess() override;
	void KeyUpdate() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
};

//-------------------------------------------------------------

enum fxlabFearDustDataField
{
	FXLAB_FEAR_DUST_DATA_DAMP = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_FEAR_DUST_DATA_MAX
};

struct fxlabCrazyDust : fxlabFearSpring
{
	float Damp;

	Vect3f Velocity;

	void Open() override;
	void CoreProcess() override;
	void CoreGenerate() override;
	void KeyUpdate() override;
	void KeyCheck() override;

	void SetVelocity(const Vect3f& v) override;
};

//------------------------------------------

enum fxlabFearFireDataField
{
	FXLAB_FEAR_FIRE_DATA_POWER = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_FEAR_FIRE_DATA_EPSILON,
	FXLAB_FEAR_FIRE_DATA_DIRECTION_X,
	FXLAB_FEAR_FIRE_DATA_DIRECTION_Y,
	FXLAB_FEAR_FIRE_DATA_DIRECTION_Z,
	FXLAB_FEAR_FIRE_DATA_AXIS_X,
	FXLAB_FEAR_FIRE_DATA_AXIS_Y,
	FXLAB_FEAR_FIRE_DATA_AXIS_Z,
	FXLAB_FEAR_FIRE_DATA_MAX
};

struct fxlabFearFire : fxlabFearSpring
{
	Vect3f Direction;
	Vect3f Axis;

	float Power;
	float Epsilon;

	Vect3f Velocity;

	void Open() override;
	void CoreProcess() override;
	void KeyUpdate() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
};

//------------------------------------------------------------

struct fxlabSparkGenerator : fxlabFearSpring
{	
	void CoreProcess() override;
};

//----------------------------------

enum fxlabFearStreamDataField
{
	FXLAB_FEAR_STREAM_DATA_FORCE_X = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_FEAR_STREAM_DATA_FORCE_Y,
	FXLAB_FEAR_STREAM_DATA_FORCE_Z,
	FXLAB_FEAR_STREAM_DATA_REFLECTION_IMPULSE,
	FXLAB_FEAR_STREAM_DATA_VELOCITY_IMPULSE,
	FXLAB_FEAR_STREAM_DATA_MAX
};

struct fxlabFearStream : fxlabFearSpring
{
	Vect3f Force;
	float ReflectionImpulse;
	float ScaleRate;
	float VelocityImpulse;

	Vect3f Velocity;

	void Open() override;
	void CoreProcess() override;
	void CoreGenerate() override;
	void KeyCheck() override;
	void KeyUpdate() override;
	void SetVelocity(const Vect3f& v) override;
	void SetScale(float scale) override;
};

//---------------------------------------------------

struct fxlabCrazyTailWaterClip : fxlabCrazyTail
{
	void CoreProcess() override;
};

struct fxlabCrazyDustWaterClip : fxlabCrazyDust
{
	void CoreProcess() override;
};

struct fxlabSparkWaterClip : fxlabSparkGenerator
{	
	void CoreProcess() override;
};

//------------------------------------------------------

enum fxlabSparkWindDataField
{
	FXLAB_SPARK_WIND_DATA_SPEED = FXLAB_FEAR_SPRING_DATA_MAX,
	FXLAB_SPARK_WIND_DATA_MAX
};

struct fxlabSparkWind : fxlabSparkGenerator
{
	Vect3f Velocity;

	void Open() override;
	void CoreGenerate() override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override { Velocity = v; };
};

struct fxlabSparkWindWaterClip : fxlabSparkWind
{
	void CoreProcess() override;
};

struct fxlabSparkLine : fxlabSparkGenerator
{
	Vect3f Velocity;

	void Open() override;
	void CoreGenerate() override;
	void SetVelocity(const Vect3f& v) override { Velocity = v; };
};

//----------------------------------

struct fxlabSparkGeneratorExternal : fxlabSparkGenerator
{	
	fxlabSwarmDomain ExternalPosition;
	fxlabSwarmDomain ExternalVelocity;

	void CoreGenerate() override;
	void SetPositionDomain(fxlabSwarmDomain* p) override;
	void SetVelocityDomain(fxlabSwarmDomain* p) override;
};

//----------------------------------

/*
struct fxlabSnowLayer : fxlabSparkGenerator
{
	void CheckVisibility();
	void StopTimeCheckVisibility();

	void CoreProcess();
	void ConvertPosition();
	void Show();
};
*/

struct fxlabSnowLayer : fxlabSparkGenerator
{
	struct sTileMap* TilePoint;

	void Open() override;

	void CheckVisibility() override;
	void StopTimeCheckVisibility() override;

	void CoreProcess() override;
	void ConvertPosition() override;

	void SetTilePoint(void* pTileMap) override;
};

//-------------------------------------

struct fxlabSplineWay : fxlabClientEvolutionType
{
	class mchMechosRacer* Owner;
	fxlabResourceObject* ImagePoint;
	double Offset;
	float Phase;

	void Open() override;
	void Show() override;
	void SetMechosPoint(class mchMechosRacer* p) override { Owner = p; };
};

