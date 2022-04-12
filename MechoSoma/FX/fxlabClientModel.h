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

	void Open(void);
	void Start(void);
	void Close(void);
	void KeyCheck(void);
	void Quant(void);
	void SetMatrix(const class MatXf& m) override;
	void SetPosition(const class Vect3f& v) override;

	virtual int CreateObjectID(void);

	virtual void CalcColor(sColor4f& color);
	virtual int CalcAttribute(void);
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

	void Open(void);
	void Start(void);
	void Quant(void);
	void KeyCheck(void);
	virtual void CalcSize(Vect3f& size);
	virtual void CalcMatrix(void);
};

//----------------------------------------------

enum fxlabAnimationModelDataField
{	
	FXLAB_ANIMATION_MODEL_DATA_DELTA_PHASE = FXLAB_CONTROL_MODEL_DATA_MAX,
	FXLAB_ANIMATION_MODEL_DATA_MAX
};

struct fxlabAnimationModel : fxlabControlModel
{
	void Quant(void);
	void KeyCheck(void);
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

	void Open(void);
	void Start(void);
	void Close(void);
	void Quant(void);
	void KeyCheck(void);
	void SetVisible(int v);
};

//----------------------------------------------

struct fxlabBonusModel : fxlabControlModel
{
	void Start(void);
};

struct fxlabBonusPart : fxlabBonusModel
{
	int BonusPartID;

	void Open(void);
	int CreateObjectID(void){ return BonusPartID; };
	void SetBonusPartID(int id){ BonusPartID = id; };

};

//-----------------------------------------------

struct fxlabSlimeModel : fxlabAnimationModel
{
	float Phase;

	void Start(void);
	void Quant(void);
	void CalcColor(sColor4f& color);
	void CalcSize(Vect3f& size);
	int GetAlive(void);
	void CalcMatrix(void){};
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

	void Open(void);
	void Start(void);
	void Quant(void);
	void CalcColor(sColor4f& color);
	void CalcDeltaPhase(float& delta);
	int GetAlive(void);
	void CalcSize(Vect3f& size);
	void SetScaleAnimation(float start_scale,float end_scale,int time);
};

//---------------------------------------

struct fxlabVoodooHeadModel : fxlabControlModel
{
	float Phase;

	void Start(void);
	void Quant(void);
	void CalcColor(sColor4f& color);
	void CalcSize(Vect3f& size);
	void CalcMatrix(void){};
	int GetAlive(void);
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

	void Open(void);
	void CalcSize(Vect3f& size);
	void KeyCheck(void);
	void SetVelocity(Vect3f& v);
	void CalcMatrix(void){};
};

struct fxlabRevoltSpaceModel : fxlabPressureModel
{
	float Phase;

	void Start(void);
	void Quant(void);
	void CalcColor(sColor4f& color);
	void CalcSize(Vect3f& size);
	int GetAlive(void);
};

//----------------------------------------------

struct fxlabMassShifterModel : fxlabControlModel
{
	void Quant(void);
	void CalcMatrix(void){};
	void CalcSize(Vect3f& size);
};

//----------------------------------------------

struct fxlabTrapGroundModel : fxlabControlModel
{
	float Phase;

	void Start(void);
	void Quant(void);
	void CalcColor(sColor4f& color);
	void CalcSize(Vect3f& size);
	int GetAlive(void);
	void CalcMatrix(void);
};

//----------------------------------------------

struct fxlabBulletModel : fxlabControlModel
{
	Vect3f Velocity;
	Vect3f NormVelocity;
	Vect3f SourcePosition;

	void Open(void);
	void Start(void);
	void Quant(void);
	void SetVelocity(Vect3f& v);
	void CalcMatrix(void);
	int CalcAttribute(void);
};

struct fxlabPetardModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabSnowBulletModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start(void);
	void Quant(void);
	void Close(void);
};

struct fxlabFireBulletModel : fxlabBulletModel
{
	fxlabProcessInterface TailPoint;
	void Start(void);
	void Quant(void);
	void Close(void);
};

//---------------------------------------------

struct fxlabClientJumpActionModel : fxlabControlModel
{
	Vect3f Velocity;
	Vect3f LocalPosition;

	void Open(void);
	void Quant(void);
	void CalcMatrix(void){};
	void SetVelocity(Vect3f& v){ Velocity = v; };
};


//-----------------------------------------

struct fxlabClientWaterWaveModel : fxlabControlModel
{
	float Red,Green,Blue,Alpha;

	void Open(void);
	void CalcColor(sColor4f& color);
	void SetColorVector(Vect3f& v){ Red = v.x; Green = v.y; Blue = v.z; };
	void SetTransparency(float a){ Alpha = a; };
	void SetPosition(const class Vect3f& v);
};


