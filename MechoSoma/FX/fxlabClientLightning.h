//-------------------------------------

enum fxlabLightningSphereField
{
	FXLAB_LIGHTNING_SPHERE_DATA_STOP = 0,
	FXLAB_LIGHTNING_SPHERE_DATA_NASCENCY,
	FXLAB_LIGHTNING_SPHERE_DATA_XP,
	FXLAB_LIGHTNING_SPHERE_DATA_YP,
	FXLAB_LIGHTNING_SPHERE_DATA_XLEVEL,
	FXLAB_LIGHTNING_SPHERE_DATA_YLEVEL,

	FXLAB_LIGHTNING_SPHERE_DATA_RADIUS,

	FXLAB_LIGHTNING_SPHERE_DATA_RED,
	FXLAB_LIGHTNING_SPHERE_DATA_GREEN,
	FXLAB_LIGHTNING_SPHERE_DATA_BLUE,

	FXLAB_LIGHTNING_SPHERE_DATA_NUM,

	FXLAB_LIGHTNING_SPHERE_OMNI_ID,

	FXLAB_LIGHTNING_SPHERE_DATA_MAX
};

struct fxlabLightningSphere : fxlabClientKeyObjectType
{
	fxlabProcessInterface OmniPoint;
	cLevin Bow;

	void Start() override;
	void Close() override;
	void Quant() override;
	void Show() override;
	void KeyCheck() override;

	virtual void CalcVelocity(Vect3f& v);
};

struct fxlabLightningLine : fxlabLightningSphere
{
	Vect3f Velocity;

	void Open() override;
	void CalcVelocity(Vect3f& v) override;
	void SetVelocity(const Vect3f& v) override;
};

struct fxlabLightningRadius : fxlabLightningLine
{
	float Radius;
	void Open() override;
	void Show() override;
	void SetVelocity(const Vect3f& v) override;
};

