struct fxlabResourceObject
{
	int Type,ID;
	float Scale,InvScale;

	fxlabResourceObject(void){ Scale = 0; InvScale = 0; Type = ID = -1; };
	virtual ~fxlabResourceObject(){};

	virtual void Open(scrDataBlock* data){};
	virtual void Close(void){};
	virtual void Link(void){};

	float GetScale(void){ return Scale; };
	float GetInvScale(void){ return InvScale; };

	virtual int GetKeyNum(void){ return 0; };
	virtual int GetKey(float* point,float time){ return 0; };
	virtual int GetKeyAttribute(void){ return 0; };
	virtual int GetKeyStatus(float time){ return 1; };

	virtual struct sSpriteFX* GetSpriteData(float phase){ return NULL; };
	virtual int GetSpriteID(void){ return 0; };
};

//---------------------------------------------

const int FXLAB_WORLD_TRACK_DUST_STYLE_MAX = 16;

struct fxlabTrackInfoType
{
	int DustStyle[FXLAB_WORLD_TRACK_DUST_STYLE_MAX];
	float WaveRed,WaveGreen,WaveBlue,WaveAlpha;

	int ReflectionEnable;

	int WaterRed,WaterDeltaRed;
	int WaterGreen,WaterDeltaGreen;
	int WaterBlue,WaterDeltaBlue;
	int WaterAlpha,WaterDeltaAlpha;
	int WaterDeltaHeight;
	float WaterDeltaUV;

	void Open(scrDataBlock* p);
	void Close(void);
};

const int FXLAB_WORLD_TRACK_NUM = 7;

struct fxlabWorldDataType
{
	fxlabTrackInfoType TrackData[FXLAB_WORLD_TRACK_NUM];

	fxlabWorldDataType(void);
	void Open(struct scrDataBlock* p);
	void Close(void);
};

//--------------------------------------------

struct fxlabUniverseDataType
{
	int WorldNum;
	fxlabWorldDataType** WorldData;

	int WorldCompileScript;
	char* WorldScriptTmpPath;
	char* WavePath;
	char* WorldSource;
	char* WorldDefinition;
	char* WorldBody;

	void Open(void);
	void Close(void);
};

//--------------------------------------------

const int FXLAB_PI_SHIFT = 8;
const int FXLAB_PI = 1 << FXLAB_PI_SHIFT;
const int FXLAB_2PI = 2 * FXLAB_PI;
const int FXLAB_2PI_CLIP = FXLAB_2PI - 1;
#define FXLAB_PI_CLIP(d)	(d & FXLAB_2PI_CLIP)
#define FXLAB_COS(d)		(fxlabCosTableType::FXLAB_COS_DATA[FXLAB_PI_CLIP(d)])
#define FXLAB_SIN(d)		(fxlabCosTableType::FXLAB_COS_DATA[FXLAB_PI_CLIP(d - FXLAB_PI / 2)])

const int FXLAB_WAVEGROUND_RANGE_POWER = 7;
const int FXLAB_WAVEGROUND_RANGE = 1 << FXLAB_WAVEGROUND_RANGE_POWER;

struct fxlabCosTableType
{
	static int* FXLAB_COS_DATA;

	void Open(void);
	void Close(void);
};

//-------------------------------------

struct fxlabResourceDispatcher : fxlabCosTableType , fxlabUniverseDataType
{
	int fxlabCompileScript;
	char* fxlabScriptTmpPath;
	char* fxlabScriptSource;
	char* fxlabScriptDefinition;
	char* fxlabScriptBody;

	int Size[FXLAB_SCRIPT_SECTION_MAX];
	fxlabResourceObject*** Index;	

	void Open(void);
	void Close(void);

	fxlabResourceObject* CreateResource(int type);
	fxlabResourceObject* GetResource(int type,int id)
	{
		return (Index[type][id]);
	};
};

extern fxlabResourceDispatcher* fxlabResourceD;



