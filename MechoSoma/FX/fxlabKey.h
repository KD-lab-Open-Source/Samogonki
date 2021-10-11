const float FXLAB_RESOURCE_KEY_DEFAULT_VALUE = -11083.3791f;

struct fxlabKeyDataType
{
	int Mode;
	float Value;
	float Delta;

	fxlabKeyDataType(void){ Mode = FXLAB_KEY_MODE_LINEAR; Value = FXLAB_RESOURCE_KEY_DEFAULT_VALUE; Delta = 0; };
};

struct fxlabKeyPointType
{
	float Time;
	int Mode;
	fxlabKeyDataType* Data;
	fxlabKeyPointType(void){ Time = -1.0f; Data = NULL; Mode = FXLAB_KEY_MODE_LINEAR; };
};

struct fxlabKeyType : fxlabResourceObject
{
	int KeyNum,PointNum;
	fxlabKeyPointType* PointData;
	int Attribute;

	fxlabKeyDataType* KeyStorage;
	char** Name;

	void Open(scrDataBlock* data);
	void Close(void);

	float GetScale(void){ return Scale; };
	float GetInvScale(void){ return InvScale; };

	int GetKeyNum(void){ return KeyNum; };
	int GetKey(float* point,float time);
	int GetKeyAttribute(void){ return Attribute; };
	int GetKeyStatus(float time);

	void FillLinear(float* point,float time);
	void FillStorage(float* point,float time);
};

