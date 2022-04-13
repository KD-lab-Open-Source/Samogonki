struct fxlabApplicationObjectType : fxlabGeneralObjectType
{
	fxlabProcessInterface* ProcessPoint;

	void Open() override
	{ 
		fxlabGeneralObjectType::Open();
		ProcessPoint = NULL; 
	};

	void Close() override
	{
		if(ProcessPoint)
			ProcessPoint->Init();
		fxlabGeneralObjectType::Close();
	};

	void SetAlive(int alive) override
	{
		Alive = alive; 
		if(!Alive && ProcessPoint){
			ProcessPoint->Init();
			ProcessPoint = NULL;
		};
	};

	void SetProcessInterface(struct fxlabProcessInterface* p) override
	{
		if(!ProcessPoint){
			ProcessPoint = p;
			if(ProcessPoint)
				ProcessPoint->Set(this);
		}else{
			ProcessPoint->Init();
			ProcessPoint = p;
			if(p)
				ProcessPoint->Set(this);
		};
	};
};

