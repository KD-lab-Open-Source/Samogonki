//-------------------------------------

enum fxlabColorModelDataField
{
	FXLAB_COLOR_MODEL_DATA_MODEL = 0,
	FXLAB_COLOR_MODEL_DATA_RED,
	FXLAB_COLOR_MODEL_DATA_GREEN,
	FXLAB_COLOR_MODEL_DATA_BLUE,
	FXLAB_COLOR_MODEL_DATA_ALPHA,
	FXLAB_COLOR_MODEL_DATA_MAX
};

struct fxlabColorModel : fxlabClientKeyObjectType
{
	class cMesh* ModelPoint;
	MatXf Matrix;

	void Open() override;
	void Start() override;
	void Close() override;
	void KeyCheck() override;
	void Quant() override;
	void SetMatrix(const class MatXf& m) override;
	void SetPosition(const class Vect3f& v) override;

	virtual int CreateObjectID();

	virtual void CalcColor(sColor4f& color);
	virtual int CalcAttribute();
};

//---------------------------------------------

enum fxlabControlModelDataField
{	
	FXLAB_CONTROL_MODEL_DATA_RADIUS = FXLAB_COLOR_MODEL_DATA_MAX,
	FXLAB_CONTROL_MODEL_DATA_ANGLEX,
	FXLAB_CONTROL_MODEL_DATA_ANGLEY,
	FXLAB_CONTROL_MODEL_DATA_ANGLEZ,
	FXLAB_CONTROL_MODEL_DATA_MAX
};

struct fxlabControlModel : fxlabColorModel
{
	Vect3f ModelSize;
	Vect3f ModelAngle;
	float ModelRadius;

	void Open() override;
	void Start() override;
	void Quant() override;
	void KeyCheck() override;
	virtual void CalcSize(Vect3f& size);
	virtual void CalcMatrix();
};

//----------------------------------------------

enum fxlabAnimationModelDataField
{	
	FXLAB_ANIMATION_MODEL_DATA_DELTA_PHASE = FXLAB_CONTROL_MODEL_DATA_MAX,
	FXLAB_ANIMATION_MODEL_DATA_MAX
};

struct fxlabAnimationModel : fxlabControlModel
{
	void Quant() override;
	void KeyCheck() override;
	virtual void CalcDeltaPhase(float& delta);
};

//----------------------------------------------

enum fxlabControlOmniDataField
{
	FXLAB_CONTROL_OMNI_DATA_RED = 0,
	FXLAB_CONTROL_OMNI_DATA_GREEN,
	FXLAB_CONTROL_OMNI_DATA_BLUE,
	FXLAB_CONTROL_OMNI_DATA_RADIUS,
	FXLAB_CONTROL_OMNI_DATA_MAX
};

struct fxlabControlOmni : fxlabClientKeyObjectType
{
	class cOmni* OmniPoint;

	void Open() override;
	void Start() override;
	void Close() override;
	void Quant() override;
	void KeyCheck() override;
	void SetVisible(int v) override;
};

//----------------------------------------------

struct fxlabBonusModel : fxlabControlModel
{
	void Start() override;
};

struct fxlabBonusPart : fxlabBonusModel
{
	int BonusPartID;

	void Open() override;
	int CreateObjectID() override { return BonusPartID; };
	void SetBonusPartID(int id) override { BonusPartID = id; };

};

//-----------------------------------------------

struct fxlabSlimeModel : fxlabAnimationModel
{
	float Phase;

	void Start() override;
	void Quant() override;
	void CalcColor(sColor4f& color) override;
	void CalcSize(Vect3f& size) override;
	int GetAlive() override;
	void CalcMatrix() override {};
};

//---------------------------------------

struct fxlabLashModel : fxlabAnimationModel
{
	float AnimationRate;
	float Phase;
	float CurrentScale;
	float ScaleDelta;
	int ScalingFlag;
	DurationTimer ScaleTimer;
	float DeltaPhase;

	void Open() override;
	void Start() override;
	void Quant() override;
	void CalcColor(sColor4f& color) override;
	void CalcDeltaPhase(float& delta) override;
	int GetAlive() override;
	void CalcSize(Vect3f& size) override;
	void SetScaleAnimation(float start_scale,float end_scale,int time) override;
};

//---------------------------------------

struct fxlabVoodooHeadModel : fxlabControlModel
{
	float Phase;

	void Start() override;
	void Quant() override;
	void CalcColor(sColor4f& color) override;
	void CalcSize(Vect3f& size) override;
	void CalcMatrix() override {};
	int GetAlive() override;
};

//-----------------------------------------

enum fxlabPressureModelDataField
{	
	FXLAB_PRESSURE_MODEL_DATA_DELTA_PHASE = FXLAB_CONTROL_MODEL_DATA_MAX,
	FXLAB_PRESSURE_MODEL_DATA_DELTA_RADIUS,
	FXLAB_PRESSURE_MODEL_DATA_MAX
};

struct fxlabPressureModel : fxlabControlModel
{
	Vect3f PressureSize;

	void Open() override;
	void CalcSize(Vect3f& size) override;
	void KeyCheck() override;
	void SetVelocity(const Vect3f& v) override;
	void CalcMatrix() override {};
};

struct fxlabRevoltSpaceModel : fxlabPressureModel
{
	float Phase;

	void Start() override;
	void Quant() override;
	void CalcColor(sColor4f& color) override;
	void CalcSize(Vect3f& size) override;
	int GetAlive() override;
};

//----------------------------------------------

struct fxlabMassShifterModel : fxlabControlModel
{
	void Quant() override;
	void CalcMatrix() override {};
	void CalcSize(Vect3f& size) override;
};

//----------------------------------------------

struct fxlabTrapGroundModel : fxlabControlModel
{
	float Phase;

	void Start() override;
	void Quant() override;
	void CalcColor(sColor4f& color) override;
	void CalcSize(Vect3f& size) override;
	int GetAlive() override;
	void CalcMatrix() override;
};

//----------------------------------------------

struct fxlabBulletModel : fxlabControlModel
{
	Vect3f Velocity;
	Vect3f NormVelocity;
	Vect3f SourcePosition;

	void Open() override;
	void Start() override;
	void Quant() override;
	void SetVelocity(const Vect3f& v) override;
	void CalcMatrix() override;
	int CalcAttribute() override;
};

struct fxlabPetardModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabSnowBulletModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start() override;
	void Quant() override;
	void Close() override;
};

struct fxlabFireBulletModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start() override;
	void Quant() override;
	void Close() override;
};

//---------------------------------------------

struct fxlabClientJumpActionModel : fxlabControlModel
{
	Vect3f Velocity;
	Vect3f LocalPosition;

	void Open() override;
	void Quant() override;
	void CalcMatrix() override {};
	void SetVelocity(const Vect3f& v) override { Velocity = v; };
};


//-----------------------------------------

struct fxlabClientWaterWaveModel : fxlabControlModel
{
	float Red,Green,Blue,Alpha;

	void Open() override;
	void CalcColor(sColor4f& color) override;
	void SetColorVector(const Vect3f& v) override { Red = v.x; Green = v.y; Blue = v.z; };
	void SetTransparency(float a) override { Alpha = a; };
	void SetPosition(const class Vect3f& v) override;
};


