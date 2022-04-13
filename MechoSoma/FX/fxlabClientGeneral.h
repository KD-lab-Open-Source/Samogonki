struct fxlabClientEvolutionType : fxlabApplicationObjectType
{
	float DeltaTime;

	void Start() override;
	void Quant() override;
};

struct fxlabClientSpaceType : fxlabClientEvolutionType
{
	Vect3f Position;

	void Open() override
	{
		fxlabClientEvolutionType::Open();
		Position = Vect3f::ZERO;
	};
	
	void SetPosition(const class Vect3f& v) override { Position = v; };
	void GetPosition(class Vect3f& v) override { v = Position; };
};

struct fxlabClientKeyObjectType : fxlabClientSpaceType
{
	float* KeyData;
	int KeyID;
	fxlabResourceObject* KeyPoint;
	float KeyTime;

	void Open() override
	{
		fxlabClientSpaceType::Open();
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
	virtual void CalcKeyTime();
};

//-------------------------------------------------------

struct fxlabClientRemoteCotrol : fxlabClientKeyObjectType
{
	fxlabProcessInterface* RemoteLink;

	void Open() override
	{
		fxlabClientKeyObjectType::Open();
		RemoteLink = NULL;
	};

	void Close() override
	{
		if(RemoteLink)
			RemoteLink->Init();
		fxlabClientKeyObjectType::Close();
	};

	void SetAlive(int alive) override
	{
		if(!alive && RemoteLink){
			RemoteLink->Init();
			RemoteLink = NULL;
		};
		fxlabClientKeyObjectType::SetAlive(alive);
	};

	void SetRemoteInterface(fxlabProcessInterface* link) override
	{ 
		if(!RemoteLink){
			RemoteLink = link;
			if(RemoteLink)
				RemoteLink->Set(this);
		}else{
			RemoteLink->Init();
			RemoteLink = link;
			if(RemoteLink)
				RemoteLink->Set(this);
		};
	};
};

//-----------------------------------------------------------

const int FXLAB_SWARM_PARTICLE_CLUSTER_SIZE = 20;

struct fxlabParticleType
{
	Vect3f Position;
	Vect3f Velocity;

	float Age,LifeTime;
	float Phase;
	float DeltaPhase;
	fxlabResourceObject* ImagePoint;

	fxlabParticleType* Cluster;
	fxlabParticleType* prev;
	fxlabParticleType* next;

	fxlabParticleType(){ next = prev = NULL; Cluster = NULL; };
};

struct fxlabParticleToolType
{
	XTList<fxlabParticleType> ParticleList;
	fxlabParticleType* ParticlePoint;

	void OpenParticleTool();
	void CloseParticleTool();

	fxlabParticleType* AddParticle();
	void DeleteParticle(fxlabParticleType* p);
	void DeleteAllParticle();
};

//--------------------------------------

enum fxlabParticleCoreData
{
	FXLAB_PARTICLE_CORE_DATA_REGENERATE_TIME = 0,
	FXLAB_PARTICLE_CORE_DATA_MAX
};

struct fxlabParticleCore : fxlabClientKeyObjectType , fxlabParticleToolType
{
	Vect3f VsL,VsS;

	int Visibility;
	int RecallTime,MaxRecallTime;

	int RegenerateTime;	
	float ProcessMaxX,ProcessMaxY,ProcessMaxZ;
	float ProcessMinX,ProcessMinY,ProcessMinZ;

	int Summoning;

	void Open() override;
	void Quant() override;
	void StopQuant() override;
	void Close() override;
	void Start() override;

	int GetAlive() override;

	void KeyCheck() override;
	void KeyUpdate() override;

	void ReCall(){	RecallTime = MaxRecallTime;	};

	virtual void ConvertPosition();
	virtual void CoreProcess(){};
	virtual void CoreGenerate(){};
	virtual void CheckVisibility();
	virtual void StopTimeCheckVisibility();
};

#define FXLAB_FEAR_BORDER_DEFINE	\
	int max_x,max_y,max_z;	\
	int min_x,min_y,min_z;	\
	ProcessMaxX = 0;		\
	ProcessMaxY = 0;		\
	ProcessMinX = 0;		\
	ProcessMinY = 0;		\
	ProcessMaxZ = 0;		\
	ProcessMinZ = 0;		\
	min_x = 1;				\
	min_y = 1;				\
	min_z = 1;				\
	max_x = 1;				\
	max_y = 1;				\
	max_z = 1;


#define FXLAB_FEAR_BORDER_PROCESS	\
	if(p->Position.x < ProcessMinX || min_x){	\
		ProcessMinX = p->Position.x;			\
		min_x = 0;								\
	};											\
	if(p->Position.x > ProcessMaxX || max_x){	\
		ProcessMaxX = p->Position.x;			\
		max_x = 0;								\
	};											\
	if(p->Position.y < ProcessMinY || min_y){	\
		ProcessMinY = p->Position.y;			\
		min_y = 0;								\
	};											\
	if(p->Position.y > ProcessMaxY || max_y){	\
		ProcessMaxY = p->Position.y;			\
		max_y = 0;								\
	};											\
	if(p->Position.z < ProcessMinZ || min_z){	\
		ProcessMinZ = p->Position.z;			\
		min_z = 0;								\
	};											\
	if(p->Position.z > ProcessMaxZ || max_z){	\
		ProcessMaxZ = p->Position.z;			\
		max_z = 0;								\
	};

//-------------------------------------------------

