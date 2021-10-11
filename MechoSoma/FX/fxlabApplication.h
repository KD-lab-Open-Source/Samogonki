struct fxlabApplicationObjectType : fxlabGeneralObjectType
{
	fxlabProcessInterface* ProcessPoint;

	void Open(void)
	{ 
		fxlabGeneralObjectType::Open();
		ProcessPoint = NULL; 
	};

	void Close(void)
	{
		if(ProcessPoint)
			ProcessPoint->Init();
		fxlabGeneralObjectType::Close();
	};

	void SetAlive(int alive)
	{
		Alive = alive; 
		if(!Alive && ProcessPoint){
			ProcessPoint->Init();
			ProcessPoint = NULL;
		};
	};

	void SetProcessInterface(struct fxlabProcessInterface* p)
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

