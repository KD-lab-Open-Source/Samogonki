struct fxlabClientImpulseSound : fxlabClientEvolutionType
{
	int SoundID;

	void Open() override;
	void Start() override;
	int GetAlive() override {return 0; };
	void SetSoundID(int id) override { SoundID = id; };
};

struct fxlabClientImpulseSpaceSound : fxlabClientSpaceType
{
	int SoundID;

	void Open() override;
	void Start() override;
	int GetAlive() override {return 0; };
	void SetSoundID(int id) override { SoundID = id; };
};

struct fxlabClientLongSound : fxlabClientEvolutionType
{
	int SoundID;

	void Open() override;
	void Start() override;
	void Close() override;
	void SetSoundID(int id) override { SoundID = id; };
};

struct fxlabClientLongSpaceSound : fxlabClientSpaceType
{
	int SoundID;

	void Open() override;
	void Start() override;
	void Quant() override;
	void Close() override;
	void SetSoundID(int id) override { SoundID = id; };
};
