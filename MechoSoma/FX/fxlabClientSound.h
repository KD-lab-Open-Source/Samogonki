struct fxlabClientImpulseSound : fxlabClientEvolutionType
{
	int SoundID;

	void Open(void);
	void Start(void);
	int GetAlive(void){return 0; };
	void SetSoundID(int id){ SoundID = id; };
};

struct fxlabClientImpulseSpaceSound : fxlabClientSpaceType
{
	int SoundID;

	void Open(void);
	void Start(void);
	int GetAlive(void){return 0; };
	void SetSoundID(int id){ SoundID = id; };
};

struct fxlabClientLongSound : fxlabClientEvolutionType
{
	int SoundID;

	void Open(void);
	void Start(void);
	void Close(void);
	void SetSoundID(int id){ SoundID = id; };
};

struct fxlabClientLongSpaceSound : fxlabClientSpaceType
{
	int SoundID;

	void Open(void);
	void Start(void);
	void Quant(void);
	void Close(void);
	void SetSoundID(int id){ SoundID = id; };
};
