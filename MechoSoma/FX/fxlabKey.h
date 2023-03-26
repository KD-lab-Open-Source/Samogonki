const float FXLAB_RESOURCE_KEY_DEFAULT_VALUE = -11083.3791f;

struct fxlabKeyDataType
{
	int Mode;
	float Value;
	float Delta;

	fxlabKeyDataType(){ Mode = FXLAB_KEY_MODE_LINEAR; Value = FXLAB_RESOURCE_KEY_DEFAULT_VALUE; Delta = 0; };
};

struct fxlabKeyPointType
{
	float Time;
	int Mode;
	fxlabKeyDataType* Data;
	fxlabKeyPointType(){ Time = -1.0f; Data = NULL; Mode = FXLAB_KEY_MODE_LINEAR; };
};

struct fxlabKeyType : fxlabResourceObject
{
	int KeyNum,PointNum;
	fxlabKeyPointType* PointData;
	int Attribute;

	fxlabKeyDataType* KeyStorage;
	char** Name;

	void Open(scrDataBlock* data) override;
	void Close() override;

	float GetScale() { return Scale; };
	float GetInvScale() { return InvScale; };

	int GetKeyNum() override { return KeyNum; };
	int GetKey(float* point,float time) override;
	int GetKeyAttribute() override { return Attribute; };
	int GetKeyStatus(float time) override;

	void FillLinear(float* point,float time);
	void FillStorage(float* point,float time);
};

