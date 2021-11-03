#include "StdAfx.h"
#include "PlayWSC.h"
#include "DefineWSC.h"
#include "StreamBuffer.h"
#include "IVisGeneric.h"
#include "IGraph3d.h"
#include "CameraDispatcher.h"
#include "AnimChannelNode.h"
#include "Maths.h"
#include "sound.h"
#include "sound_api.h"
#include "PolyMgr.h"
#include "RenderDevice.h"
#include "VisGeneric.h"

// ��� ������ �� ���������
#include "WorldScript.h"
#include "BaseDefine.h"
#include "aci_parser.h"

extern void fxlabMovieUpdateProcess(const char* name,const Vect3f& position,const Vect3f& velocity,int status);
extern void fxlabMovieDestroy(void);

cWorldScriptPlay TestWorldScriptPlay;

inline void CyclMatrix(MatXf &Matrix)
{
	Matrix.trans().x=2048-fmod(Matrix.trans().x+2048,2048);
	Matrix.trans().y=fmod(Matrix.trans().y+2048,2048);
}

class cElementWorldScript : public sAttribute
{
public:
	cElementWorldScript()													{ FinishTime=StartTime=CurrentTime=0; }
	virtual ~cElementWorldScript()											{}
	virtual void Draw()														{}
	virtual void SetAnimation(float period,float phase=0,float finish=1)	{ StartTime=0; CurrentTime=phase*period; FinishTime=period; }
	virtual int Animate(float dt)											
	{
		CurrentTime+=dt;
		if(CurrentTime<0) return 0;
		if(CurrentTime>=FinishTime) return 0;
//		if(CurrentTime>=FinishTime) CurrentTime=fmodf(CurrentTime,FinishTime);
		return 1;
	}
	virtual void SetOrientation(cAnimChainNode *AnimChainNode)				{ }
	virtual void GetAnimation(MatXf &ConnectMatrix,int &visible)			{ }
	virtual cString& GetConnectName()										{ return name; }
protected:
	float					FinishTime,StartTime,CurrentTime;
	cString					name;
};

class cConnectBaseWSC : public cElementWorldScript, public cAnimChainNode
{ // ������� ������� ��� �������� � ������������
public:
	cConnectBaseWSC()														{}
	virtual ~cConnectBaseWSC()												{ cAnimChainNode::Release(); }
	virtual void SetAnimation(float period,float phase=0,float finish=1)
	{
		cAnimChainNode::GetTime()=period;
		cElementWorldScript::SetAnimation(period,phase,finish);
	}
	virtual void GetAnimation(MatXf &Matrix,int &visible)
	{ 
		if(CurrentTime>=FinishTime||CurrentTime<StartTime) 
		{ visible=0; return; }
		cAnimChainNode::GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),visible,Matrix);
		CyclMatrix(Matrix);
	}
};
class cConnectObjectWSC : public cConnectBaseWSC
{
public:
	cConnectObjectWSC()
	{
		UObject=0; 
		FinishTime=StartTime=CurrentTime=0;
	}
	virtual ~cConnectObjectWSC()											
	{ 
		if(IVisGeneric&&UObject) IVisGeneric->ReleaseObject(UObject);
		UObject=0; IVisGeneric=0;
	}
	virtual int Animate(float dt)
	{
		// matrix sign
		float gbm00=-1;
		float gbm01=1;
		float gbm02=-1;
		float gbm10=1;
		float gbm11=-1;
		float gbm12=1;
		float gbm20=1;
		float gbm21=1;
		float gbm22=1;

		if(cElementWorldScript::Animate(dt)==0) return 0;
		int visible;
		MatXf ConnectMatrix;
		cAnimChainNode::GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),visible,ConnectMatrix);
		CyclMatrix(ConnectMatrix);
		ConnectMatrix.rot()[0][0]*=gbm00;
		ConnectMatrix.rot()[0][1]*=gbm01;
		ConnectMatrix.rot()[0][2]*=gbm02;
		ConnectMatrix.rot()[1][0]*=gbm10;
		ConnectMatrix.rot()[1][1]*=gbm11;
		ConnectMatrix.rot()[1][2]*=gbm12;
		ConnectMatrix.rot()[2][0]*=gbm10;
		ConnectMatrix.rot()[2][1]*=gbm11;
		ConnectMatrix.rot()[2][2]*=gbm12;
		IVisGeneric->SetObjectVisible(UObject,visible);
		IVisGeneric->SetObjectPosition(UObject,&ConnectMatrix);
		IVisGeneric->SetObjectAnimation(UObject,0,(CurrentTime-StartTime)/(FinishTime-StartTime));
		return 1;
	}
	inline void SetObject(cUnknownClass *pUObject)					{ UObject=pUObject; }
	inline void SetVisGeneric(cInterfaceVisGeneric *pIVisGeneric)	{ IVisGeneric=pIVisGeneric; }
private:
	cInterfaceVisGeneric	*IVisGeneric;
	cUnknownClass			*UObject;
};
class cConnectEffectWSC : public cConnectBaseWSC
{ 
public:
	cConnectEffectWSC()												{ Orientation=0; }
	virtual ~cConnectEffectWSC()									
	{ 
		fxlabMovieUpdateProcess("LightningTest",Vect3f(0,0,0),Vect3f(0,0,0),0);
		if(Orientation) delete Orientation; 
		Orientation=0; 
	}
	virtual int Animate(float dt)
	{
		if(cElementWorldScript::Animate(dt)==0) return 0;
		int visible;
		MatXf ConnectMatrix;
		cAnimChainNode::GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),visible,ConnectMatrix);
		CyclMatrix(ConnectMatrix);
		if(Orientation)
		{
			int OrientationVisible;
			MatXf OrientationMatrix;
			Orientation->GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),OrientationVisible,OrientationMatrix);
			CyclMatrix(OrientationMatrix);
			OrientationMatrix.trans()-=ConnectMatrix.trans();
			fxlabMovieUpdateProcess(GetConnectName(),ConnectMatrix.trans(),OrientationMatrix.trans(),visible);
		}
		else
			fxlabMovieUpdateProcess(GetConnectName(),ConnectMatrix.trans(),Vect3f(0,0,1),visible);
		return 1;
	}
	virtual void SetOrientation(cAnimChainNode *AnimChainNode)		{ if(Orientation) delete Orientation; Orientation=AnimChainNode; if(Orientation) Orientation->GetTime()=cAnimChainNode::GetTime(); }
private:
	cAnimChainNode			*Orientation;
};
class cConnectCameraWSC : public cConnectBaseWSC
{ 
public:
	cConnectCameraWSC()												{ Orientation=0; CameraDisp=0; }
	virtual ~cConnectCameraWSC()									{ if(Orientation) delete Orientation; Orientation=0; }
	virtual int Animate(float dt)
	{
		if(cElementWorldScript::Animate(dt)==0) return 0;
		int visible;
		MatXf ConnectMatrix;
		cAnimChainNode::GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),visible,ConnectMatrix);
		if(!visible) return 0;
		int OrientationVisible;
		MatXf OrientationMatrix;
		Orientation->GetMatrix((CurrentTime-StartTime)/(FinishTime-StartTime),OrientationVisible,OrientationMatrix);
		CyclMatrix(OrientationMatrix);
		CyclMatrix(ConnectMatrix);
		Vect3f ofs(GetShortDistX(ConnectMatrix.trans().x,OrientationMatrix.trans().x,2048.f),
			GetShortDistY(ConnectMatrix.trans().y,OrientationMatrix.trans().y,2048.f),
			ConnectMatrix.trans().z-OrientationMatrix.trans().z);
		CameraDisp->setCoords(OrientationMatrix.trans()+ofs,OrientationMatrix.trans());
		return 1;
	}
	virtual void SetOrientation(cAnimChainNode *AnimChainNode)		{ if(Orientation) delete Orientation; Orientation=AnimChainNode; if(Orientation) Orientation->GetTime()=cAnimChainNode::GetTime(); }
	inline void SetCameraDispatcher(CameraDispatcher *pCameraDisp)	{ CameraDisp=pCameraDisp; }
private:
	cAnimChainNode			*Orientation;
	CameraDispatcher		*CameraDisp;
};
class cTextWSC : public cElementWorldScript
{
	cString					Text;
	Vect2f					Pos;
	Vect2f					FontSize;
	sColor4f				Color;
	int						nFont;
	int						centered;
public:
	cTextWSC()														{ Pos.set(0.1f,0.9f); FontSize.set(1.f,1.f); Color.set(1,1,1,1); nFont=2; centered=0; }
	virtual ~cTextWSC()												{ }
	virtual void Draw()
	{
		if(StartTime>CurrentTime||CurrentTime>FinishTime) return;
		extern void mchA_d3dOutString(int x,int y,float sx,float sy,void* str,int col,int alpha,int fnt,int space = 0,float mul = 1.0f,int pr_flag = 1,int vspace = 0);
		int xPos=Pos.x*640,yPos=Pos.y*480,color=(Color.GetR()<<16)|(Color.GetG()<<8)|(Color.GetB()<<0);
		if(RenderMode == XGRAPH_HICOLOR)
			XGR_OutText(xPos,yPos,0xFFFF,(char*)Text,1,0,0,0);
		else 
		{
			if(centered) xPos-=strlen(Text)*7/2;
			mchA_d3dOutString(xPos,yPos,FontSize.x,FontSize.y,(char*)Text,color,Color.GetA(),nFont);
		}
	}
	inline void SetText(char *pText)								{ Text=pText; }
	inline void SetPos(const Vect2f &ScrPos)						{ Pos=ScrPos; }
	inline void SetNumberFont(int NumberFont)						{ nFont=NumberFont; }
	inline void SetFontSize(const Vect2f &fSize)					{ FontSize=fSize; }
	inline void SetCentered(int n)									{ centered=n; }
	inline sColor4f& GetColor()										{ return Color; }
};
class cSoundWSC : public cElementWorldScript
{
	int						play;
	int						Sound;
	Vect2f					Pos;
	cElementWorldScript		*Parent;
public:
	cSoundWSC()														{ Pos.set(0.1f,0.9f); Sound=0; play=-1; Parent=0; }
	virtual ~cSoundWSC()											
	{ 
		if(play) { mchStopSoundEFF(Sound); play=0; }
	}
	virtual int Animate(float dt)
	{
		if(cElementWorldScript::Animate(dt)==0) 
		{ // stop play sound
			if(play<=0) return 0;
			mchStopSoundEFF(Sound);
			return play=0;
		}
		// play sound
		if(Parent)
		{
			int visible;
			MatXf Matrix;
			Parent->GetAnimation(Matrix,visible);
			if(play<0) 
				mchStartSoundEFF(Sound,round(Matrix.trans().x),round(Matrix.trans().y));
			else 
				mchSetSoundCoords(Sound,round(Matrix.trans().x),round(Matrix.trans().y));
		}
		else
			if(play<0)
				mchStartSoundEFF(Sound);
		play=1;
		return 1;
	}
	inline void SetSound(int SoundID)								{ Sound=SoundID; }
	inline void SetPos(const Vect2f &WorldPos)						{ Pos=WorldPos; }
	inline void SetParent(cElementWorldScript *ParentWS)			{ Parent=ParentWS; }
};
class cLighingWSC : public cElementWorldScript
{
	sColor4f				Start,Finish;
	cInterfaceGraph3d		*IGraph3d;
public:
	cLighingWSC()													{ Start.set(1.f,1.f,1.f,0.f); Finish.set(1.f,1.f,1.f,0.f); IGraph3d=0; }
	virtual ~cLighingWSC()											{ }
	virtual void Draw()
	{
		if(CurrentTime<StartTime||FinishTime<CurrentTime) return;
		float t=(CurrentTime-StartTime)/(FinishTime-StartTime);
		sColor4f color=Start*(1-t)+Finish*t;
		if(IGraph3d) IGraph3d->SetViewColor(color.GetR(),color.GetG(),color.GetB(),color.GetA());
	}
	inline void SetIGraph3d(cInterfaceGraph3d *IGraph3d)			{ cLighingWSC::IGraph3d=IGraph3d; }
	inline sColor4f& GetColorFinish()								{ return Finish; }
	inline sColor4f& GetColorStart()								{ return Start; }
};
class cSkyWSC : public cElementWorldScript
{
	sColor4f				Start,Finish;
	cWorldPolyGrid			*Sky;
	cWorldPolyGrid			*Clouds;
public:
	cSkyWSC()														{ Start.set(0.f,0.f,0.f,0.f); Finish.set(1.f,1.f,1.f,0.f); Sky=0; Clouds=0; }
	virtual ~cSkyWSC()												{ if(Sky) Sky->MaterialArray[0].GetSpecular().set(0,0,0,1); if(Clouds) Clouds->MaterialArray[0].GetSpecular().set(0,0,0,1); }
	virtual void Draw()
	{
		if(CurrentTime<StartTime||FinishTime<CurrentTime) return;
		float t=(CurrentTime-StartTime)/(FinishTime-StartTime);
		if(Sky) 
			Sky->MaterialArray[0].GetSpecular()=Start*(1-t)+Finish*t;
		if(Clouds)
			Clouds->MaterialArray[0].GetSpecular()=Start*(1-t)+Finish*t;
	}
	inline void Set(cWorldPolyGrid *pSky,cWorldPolyGrid *pClouds)	{ Sky=pSky; Clouds=pClouds; }
	inline sColor4f& GetColorFinish()								{ return Finish; }
	inline sColor4f& GetColorStart()								{ return Start; }
};

cWorldScriptPlay::cWorldScriptPlay()
{
	IVisGeneric=0; CameraDisp=0;
	root=0; current=0;
	StartTime=-1;
	CurrentTime=0; FinishTime=0; MusicTrack=0;
}
cWorldScriptPlay::~cWorldScriptPlay()
{
	if(root) delete root; root=0;
	if(IVisGeneric) 
		IVisGeneric->FreeObjectLibrary(1,M3D_ONLYSCENE); IVisGeneric=0;
//	UnhideAllObject();
}
void cWorldScriptPlay::Free()
{
	StartTime=-1; CurrentTime=FinishTime=0;
	for(int i=0;i<GetNumberConnect();i++)
	{
		delete GetConnect(i); 
		GetConnect(i)=0;
	}
	ConnectArray.Delete();
//	HideAllObject();
	fxlabMovieDestroy();
	MusicTrack=0;
}
int cWorldScriptPlay::OpenWorldScript(char *fname)
{
	if(IVisGeneric==0) return 1;
	if(root) delete root;
	root=loadScript(fname);
	// ������������� ��������� ������
	scrDataBlock *p,*p1;
	p=root->find(WORLDSCRIPT_DEFOBJECT3D_PATH);
	if(p) DefaultPathObject3d=(char*)p->c_dataPtr;
	p=root->find(WORLDSCRIPT_DEFTEXTURE_PATH);
	if(p) DefaultPathTexture=(char*)p->c_dataPtr;
	p=root->find(WORLDSCRIPT_SOUNDSCRIPT);
	if(p) SoundScriptName=(char*)p->c_dataPtr;
	p=root->find(WORLDSCRIPT_MUSICTRACK);
	if(p) GetMusicTrack()=p->i_dataPtr[0];
	if((!DefaultPathObject3d)||(!DefaultPathTexture)) 
		return 1;
	p=root->nextLevel->first();
	// ����������� �����
	CurrentPathObject3d=DefaultPathObject3d;
	while(p)
	{
		switch(p->ID)
		{
			case WORLDSCRIPT_ANIMSCRIPT:
			{
				p1=p->nextLevel->first();
				while(p1)
				{
					switch(p1->ID)
					{
						case WORLDSCRIPT_OBJECT3D_PATH:
							CurrentPathObject3d=(char*)p1->c_dataPtr;
							break;
						case WORLDSCRIPT_NAME_WSC:
						{
							cString DefaultNameWSC=(char*)p1->c_dataPtr;
							int size=0;
							void *buf=0;
							extern int ResourceFileRead(char *fname,void *&buf,int &size);
							ResourceFileRead(DefaultNameWSC,buf,size);
							cWorldScriptPlay::PreLoadWSC((char*)buf,size);
							break;
						}
					}
					p1=p1->next;
				}
				break;
			}
		}
		p=p->next;
	}
	current=root->nextLevel->first();
	mch_sndD->Free(1);
	if(SoundScriptName) mch_sndD->Load(SoundScriptName,1); // ��������� ������, ������� ����� ��� ���� ���� ����������
	return 0;
}
void cWorldScriptPlay::CloseWorldScript()
{
	Free();
	if(root) delete root; root=0; current=0;
//	UnhideAllObject();
	if(IVisGeneric) 
		IVisGeneric->FreeObjectLibrary(1,M3D_ONLYSCENE); IVisGeneric=0;
//	if(CameraDisp) CameraDisp->setObserver(0,1);
	CameraDisp=0;
	StartTime=-1; CurrentTime=FinishTime=0; MusicTrack=0; CutName=0;
	if(!mchSoundMute){
		sndMusicStop();
		sndMusicSetVolume(mchMusicVolume);
	}
	mchLoadWorldSound(); // ����� ������ ��������� �� ����� ������� �����/��������� ������ �� ������
}
int cWorldScriptPlay::LoadNextWorldScript()
{
	Free();
	CurrentPathObject3d=DefaultPathObject3d;
	scrDataBlock *p;
	assert(current);
	current=current->next;
	while(current)
		if(current->ID==WORLDSCRIPT_ANIMSCRIPT) 
			break;
		else current=current->next;
	if(current==0) return 1;
	CutName=((char*)current->c_dataPtr);
	p=current->nextLevel->first();
	while(p)
	{
		switch(p->ID)
		{
			case WORLDSCRIPT_TEXT:
			{
				cTextWSC *TextWSC=new cTextWSC;
				TextWSC->SetText((char*)p->c_dataPtr);
				ConnectArray.Attach(TextWSC);
				scrDataBlock *p2=p->nextLevel->first();
				while(p2)
				{
					switch(p2->ID)
					{
						case WORLDSCRIPT_ANIMATION:
						{
							float period=(float)(p2->d_dataPtr[1]-p2->d_dataPtr[0]);
							if(period<0.1f) period=0.1f;
							float phase=-(float)(p2->d_dataPtr[0])/period;
							TextWSC->SetAnimation(period,phase);
							break;
						}
						case WORLDSCRIPT_POSXY:
							TextWSC->SetPos(Vect2f((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1])));
							break;
						case WORLDSCRIPT_NUMBERFONT:
							TextWSC->SetNumberFont(p2->i_dataPtr[0]);
							break;
						case WORLDSCRIPT_COLORFONT:
							TextWSC->GetColor().set((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1]),(float)(p2->d_dataPtr[2]),(float)(p2->d_dataPtr[3]));
							break;
						case WORLDSCRIPT_FONTSIZE:
							TextWSC->SetFontSize(Vect2f((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[0])));
							break;
						case WORLDSCRIPT_CENTERED:
							TextWSC->SetCentered(p2->i_dataPtr[0]);
							break;
					}
					p2=p2->next;
				}
				break;
			}
			case WORLDSCRIPT_LIGHTING:
			{
				cLighingWSC *LighingWSC=new cLighingWSC;
				LighingWSC->SetIGraph3d(IVisGeneric->GetIGraph3d());
				ConnectArray.Attach(LighingWSC);
				scrDataBlock *p2=p->nextLevel->first();
				while(p2)
				{
					switch(p2->ID)
					{
						case WORLDSCRIPT_ANIMATION:
						{
							float period=(float)(p2->d_dataPtr[1]-p2->d_dataPtr[0]);
							if(period<0.1f) period=0.1f;
							float phase=-(float)(p2->d_dataPtr[0])/period;
							LighingWSC->SetAnimation(period,phase);
							break;
						}
						case WORLDSCRIPT_STARTCOLOR:
							LighingWSC->GetColorStart().set((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1]),(float)(p2->d_dataPtr[2]),(float)(p2->d_dataPtr[3]));
							break;
						case WORLDSCRIPT_FINISHCOLOR:
							LighingWSC->GetColorFinish().set((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1]),(float)(p2->d_dataPtr[2]),(float)(p2->d_dataPtr[3]));
							break;
					}
					p2=p2->next;
				}
				break;
			}
			case WORLDSCRIPT_SKY:
			{
				cSkyWSC *SkyWSC=new cSkyWSC;
				SkyWSC->Set(((cVisGeneric*)IVisGeneric)->GetActiveScene()->GetAir(),((cVisGeneric*)IVisGeneric)->GetActiveScene()->GetCloud());
				ConnectArray.Attach(SkyWSC);
				scrDataBlock *p2=p->nextLevel->first();
				while(p2)
				{
					switch(p2->ID)
					{
						case WORLDSCRIPT_ANIMATION:
						{
							float period=(float)(p2->d_dataPtr[1]-p2->d_dataPtr[0]);
							if(period<0.1f) period=0.1f;
							float phase=-(float)(p2->d_dataPtr[0])/period;
							SkyWSC->SetAnimation(period,phase);
							break;
						}
						case WORLDSCRIPT_STARTCOLOR:
							SkyWSC->GetColorStart().set((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1]),(float)(p2->d_dataPtr[2]),(float)(p2->d_dataPtr[3]));
							break;
						case WORLDSCRIPT_FINISHCOLOR:
							SkyWSC->GetColorFinish().set((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1]),(float)(p2->d_dataPtr[2]),(float)(p2->d_dataPtr[3]));
							break;
					}
					p2=p2->next;
				}
				break;
			}
			case WORLDSCRIPT_SOUND:
			{
				cSoundWSC *SoundWSC=new cSoundWSC;
				ConnectArray.Attach(SoundWSC);
				SoundWSC->SetSound(p->i_dataPtr[0]);
				scrDataBlock *p2=p->nextLevel->first();
				while(p2)
				{
					switch(p2->ID)
					{
						case WORLDSCRIPT_ANIMATION:
						{
							float period=(float)(p2->d_dataPtr[1]-p2->d_dataPtr[0]);
							if(period<0.1f) period=0.1f;
							float phase=-(float)(p2->d_dataPtr[0])/period;
							SoundWSC->SetAnimation(period,phase);
							break;
						}
						case WORLDSCRIPT_POSXY:
							SoundWSC->SetPos(Vect2f((float)(p2->d_dataPtr[0]),(float)(p2->d_dataPtr[1])));
							break;
						case WORLDSCRIPT_PARENT:
							SoundWSC->SetParent(FindElement(p2->c_dataPtr));
							break;
					}
					p2=p2->next;
				}
				break;
			}
			case WORLDSCRIPT_MUSICTRACK:
				GetMusicTrack()=p->i_dataPtr[0];
				break;
			case WORLDSCRIPT_OBJECT3D_PATH:
				CurrentPathObject3d=(char*)p->c_dataPtr;
				break;
			case WORLDSCRIPT_NAME_WSC:
			{
				cString DefaultNameWSC=(char*)p->c_dataPtr;
				int size=0;
				void *buf=0;
				extern int ResourceFileRead(char *fname,void *&buf,int &size);
				ResourceFileRead(DefaultNameWSC,buf,size);
				cWorldScriptPlay::LoadWSC((char*)buf,size);
				break;
			}
			case WORLDSCRIPT_VISIBLEOBJECT:
			{
				cUnknownClass *UObj=IVisGeneric->FindObjectByFileName((char*)p->c_dataPtr);
				if(UObj) IVisGeneric->SetObjectVisible(UObj,1);
				break;
			}
			case WORLDSCRIPT_INVISIBLEOBJECT:
			{
				cUnknownClass *UObj=IVisGeneric->FindObjectByFileName((char*)p->c_dataPtr);
				if(UObj) IVisGeneric->SetObjectVisible(UObj,0);
				break;
			}
			case WORLDSCRIPT_VISIBLEALLOBJECT:
				if(p->c_dataPtr[0])
					UnhideAllObject();
				else
					HideAllObject();
				break;
			case WORLDSCRIPT_ANIMATION:
				CurrentTime=p->d_dataPtr[0];
				FinishTime=p->d_dataPtr[1];
				break;
		}
		p=p->next;
	}
	return 0;
}
int cWorldScriptPlay::LoadWSC(void *buffer,int length)
{
	char buf[1024];
	float Version=0,StartFrame=0,FinishFrame=0,NumberFrame=0,SecPerFrame=0;
	cStream InWSC;
	InWSC.open(buffer,length,STREAM_MEM|STREAM_IN);
	InWSC>>buf;
	if(buf[0]!='W'||buf[1]!='S'||buf[2]!='C') { InWSC.close(); return 1; }
	InWSC>>Version;
	if(Version>1.0f) { InWSC.close(); return 2; }
	while(InWSC.eof()==0)
	{
		buf[0]=0;
		while(InWSC.eof()==0&&buf[0]!='$')
			InWSC>>buf;
		if(stricmp(buf,WSC_CONNECT_OBJECT)==0)
		{
			char ConnectName[1024];
			cConnectBaseWSC *Connect=0;
			InWSC>>buf;
			strcpy(ConnectName,buf);
			while(InWSC.eof()==0&&buf[0]!='{') InWSC>>buf;
			while(InWSC.eof()==0&&buf[0]!='}') 
			{ // ������ connect'�
				InWSC>>buf;
				while(InWSC.eof()==0&&buf[0]!='$'&&buf[0]!='}')
					InWSC>>buf; // ���������� ������
				if(stricmp(buf,WSC_CONNECT_POSITION)==0)
				{
					sKey3f &key=Connect->GetNewPos();
					InWSC>>key.time>>key.v.x>>key.v.y>>key.v.z;
					key.time=round(key.time-StartFrame*SecPerFrame);
				}
				else if(stricmp(buf,WSC_CONNECT_ROTATION)==0)
				{
					sKey4f &key=Connect->GetNewRot();
					InWSC>>key.time>>key.q.x()>>key.q.y()>>key.q.z()>>key.q.s();
					key.time=round(key.time-StartFrame*SecPerFrame);
					key.q.s()=-key.q.s();
				}
				else if(stricmp(buf,WSC_CONNECT_SCALE)==0)
				{
					sKey3f &key=Connect->GetNewScale();
					InWSC>>key.time>>key.v.x>>key.v.y>>key.v.z;
					key.time=round(key.time-StartFrame*SecPerFrame);
				}
				else if(stricmp(buf,WSC_CONNECT_VISIBLE)==0)
				{
					sKeyVisible &key=Connect->GetNewVisible();
					InWSC>>key.time>>key.visible;
					key.time=round(key.time-StartFrame*SecPerFrame);
					if(key.visible<0) key.visible=0;
				}
				else if(stricmp(buf,WSC_CREATE_OBJECT)==0)
				{
					cConnectObjectWSC *ConnectObject=new cConnectObjectWSC;
					ConnectObject->SetVisGeneric(IVisGeneric);
					char FullName[1024];
					strcpy(FullName,CurrentPathObject3d);
					InWSC>>&FullName[strlen(FullName)];
					cUnknownClass *UObject=IVisGeneric->CreateObject(FullName,DefaultPathTexture,1,M3D_ONLYSCENE);
					IVisGeneric->SetObjectAttribute(UObject,MESH_REFLECTION|MESH_SHADE_ALWAYS);
					MatXf CenterMatrix,LeftToRight(Mat3f(Vect3f(1,0,0),Vect3f(0,1,0),Vect3f(0,0,1)),Vect3f(0,0,0));
					ConnectObject->SetObject(UObject);
					IVisGeneric->SetObjectAnimation(UObject,(FinishFrame-StartFrame)*SecPerFrame,0);
					ConnectObject->SetAttribute(ATTRCONNECTWSC_OBJECT);
					ConnectObject->GetConnectName()=ConnectName;
					Connect=ConnectObject;
				}
				else if(stricmp(buf,WSC_CREATE_SPECIALEFFECT)==0)
				{
					cConnectEffectWSC *ConnectEffect=new cConnectEffectWSC;
					ConnectEffect->SetAttribute(ATTRCONNECTWSC_SPECIALEFFECT);
					ConnectEffect->GetConnectName()=&ConnectName[1];
					Connect=ConnectEffect;
				}
				else if(stricmp(buf,WSC_CREATE_CAMERA)==0)
				{
					cConnectCameraWSC *ConnectCamera=new cConnectCameraWSC;
					ConnectCamera->SetAttribute(ATTRCONNECTWSC_CAMERA);
					ConnectCamera->SetCameraDispatcher(CameraDisp);
					ConnectCamera->GetConnectName()=ConnectName;
					Connect=ConnectCamera;
				}
				else if(stricmp(buf,WSC_ORIENTATION)==0||stricmp(buf,WSC_TARGET)==0)
				{
					cAnimChainNode *Orientation=new cAnimChainNode;
					Connect->SetOrientation(Orientation);
					Orientation->GetTime()=(FinishFrame-StartFrame)*SecPerFrame;
					InWSC>>buf;
					Orientation->SetName(buf);
					while(InWSC.eof()==0&&buf[0]!='}') 
					{ // ������ orientation'�
						InWSC>>buf;
						while(InWSC.eof()==0&&buf[0]!='$'&&buf[0]!='}')
							InWSC>>buf; // ���������� ������
						if(stricmp(buf,WSC_CONNECT_POSITION)==0)
						{
							sKey3f &key=Orientation->GetNewPos();
							InWSC>>key.time>>key.v.x>>key.v.y>>key.v.z;
							key.time=round(key.time-StartFrame*SecPerFrame);
						}
						else if(stricmp(buf,WSC_CONNECT_ROTATION)==0)
						{
							sKey4f &key=Orientation->GetNewRot();
							InWSC>>key.time>>key.q.x()>>key.q.y()>>key.q.z()>>key.q.s();
							key.time=round(key.time-StartFrame*SecPerFrame);
							key.q.s()=-key.q.s();
						}
						else if(stricmp(buf,WSC_CONNECT_SCALE)==0)
						{
							sKey3f &key=Orientation->GetNewScale();
							InWSC>>key.time>>key.v.x>>key.v.y>>key.v.z;
							key.time=round(key.time-StartFrame*SecPerFrame);
						}
						else if(stricmp(buf,WSC_CONNECT_VISIBLE)==0)
						{
							sKeyVisible &key=Orientation->GetNewVisible();
							InWSC>>key.time>>key.visible;
							key.time=round(key.time-StartFrame*SecPerFrame);
							if(key.visible<0) key.visible=0;
						}
					}
					buf[0]=0;
				}
			}
			Connect->SetAnimation((FinishFrame-StartFrame)*SecPerFrame);
			ConnectArray.Attach(Connect);
		} 
		else if(stricmp(buf,WSC_STARTFRAME)==0)
			InWSC>>StartFrame;
		else if(stricmp(buf,WSC_FINISHFRAME)==0)
			InWSC>>FinishFrame;
		else if(stricmp(buf,WSC_NUMBERFRAME)==0)
			InWSC>>NumberFrame;
		else if(stricmp(buf,WSC_SECPERFRAME)==0)
			InWSC>>SecPerFrame;
	}
	InWSC.close();
	CurrentTime=0;
	FinishTime=(FinishFrame-StartFrame)*SecPerFrame;
	return 0;
}
int cWorldScriptPlay::PreLoadWSC(void *buffer,int length)
{
	char buf[1024];
	float Version=0,StartFrame=0,FinishFrame=0,NumberFrame=0,SecPerFrame=0;
	cStream InWSC;
	InWSC.open(buffer,length,STREAM_MEM|STREAM_IN);
	InWSC>>buf;
	if(buf[0]!='W'||buf[1]!='S'||buf[2]!='C') { InWSC.close(); return 1; }
	InWSC>>Version;
	if(Version>1.0f) { InWSC.close(); return 2; }
	while(InWSC.eof()==0)
	{
		buf[0]=0;
		while(InWSC.eof()==0&&buf[0]!='$')
			InWSC>>buf;
		if(stricmp(buf,WSC_CREATE_OBJECT)==0)
		{
			char FullName[1024];
			strcpy(FullName,CurrentPathObject3d);
			InWSC>>&FullName[strlen(FullName)];
			cUnknownClass *UObject=IVisGeneric->CreateObject(FullName,DefaultPathTexture,1,M3D_ONLYSCENE);
			IVisGeneric->ReleaseObject(UObject);
		}
	}
	InWSC.close();
	return 0;
}
void cWorldScriptPlay::Animate()
{
	int CurTime;
	if(IVisGeneric==0) return;
	if(root==0||current==0) return;
	IVisGeneric->GetTime(&CurTime);
	float dt=(float)(CurTime-(CurrentTime+StartTime));
	assert(dt>=0);
	CurrentTime+=dt;
	assert(CurrentTime>=0);
	if(CurrentTime>FinishTime) 
	{
		if(LoadNextWorldScript()) 
		{ CloseWorldScript(); return; }
		if(MusicTrack>0) 
		{
//			mchSetMusicMode(MCH_CD_PLAYING);
			if(!mchSoundMute){
				sndMusicSetVolume(mchSoundVolume);
				sndMusicPlay(GetMusicTrack());
			}
		}
		StartTime=CurTime;
		dt=0;
	}
	for(int i=0;i<GetNumberConnect();i++)
		GetConnect(i)->Animate(dt);
}
void cWorldScriptPlay::Draw()
{
	if(isPlay())
	{
		P3D->GetRenderDevice(0)->GetIGraph3d()->DrawRectangle(0,0,XGR_MAXX,round(XGR_MAXY*0.1f),0,0,0,255);
		P3D->GetRenderDevice(0)->GetIGraph3d()->DrawRectangle(0,round(XGR_MAXY*0.9f),XGR_MAXX,round(XGR_MAXY*0.1f),0,0,0,255);
	}
	for(int i=0;i<GetNumberConnect();i++)
		GetConnect(i)->Draw();
}
void cWorldScriptPlay::HideAllObject()
{
	if(IVisGeneric==0) return;
	cM3D *M3D=(cM3D*)IVisGeneric->GetM3D();
	if(M3D==0) return;
	cMesh *tmp=M3D->First();
	while(tmp)
	{
		tmp->SetVisible(0);
		tmp=M3D->Next();
	}
}
void cWorldScriptPlay::UnhideAllObject()
{
	if(IVisGeneric==0) return;
	cM3D *M3D=(cM3D*)IVisGeneric->GetM3D();
	if(M3D==0) return;
	cMesh *tmp=M3D->First();
	while(tmp)
	{
		tmp->SetVisible(1);
		tmp=M3D->Next();
	}
}
cElementWorldScript* cWorldScriptPlay::FindElement(char *ConnectName)
{
	for(int i=0;i<GetNumberConnect();i++)
	{
		char *NameWSC=GetConnect(i)->GetConnectName();
		if(NameWSC&&stricmp(NameWSC,ConnectName)==0)
			return GetConnect(i);
	}
	return 0;
}
