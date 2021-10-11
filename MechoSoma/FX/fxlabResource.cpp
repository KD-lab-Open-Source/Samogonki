#include "StdAfx.h"

#include "aci_parser.h"

#include "fxlabID.h"
#include "fxlabResource.h"
#include "fxlabKey.h"
#include "fxlabSprite.h"

extern char* getIniKey(char* fname,char* section,char* key);
extern char* mch_mainINI;

int* fxlabCosTableType::FXLAB_COS_DATA = NULL;

void fxlabCreateSpriteTable(void);

void fxlabResourceDispatcher::Open(void)
{
	char* t;
	int i,j,id,type;
	scrDataBlock* p;
	scrDataBlock* pp;
	scrDataBlock* script;
	fxlabResourceObject* n;
	XBuffer msg(256);

	fxlabCosTableType::Open();
	fxlabUniverseDataType::Open();
	fxlabCreateSpriteTable();

	fxlabCompileScript = atoi(getIniKey(mch_mainINI,"FXLAB","make_script"));

	t = getIniKey(mch_mainINI,"FXLAB","ScriptTmpPath");
	fxlabScriptTmpPath = new char[strlen(t) + 1];
	strcpy(fxlabScriptTmpPath,t);

	t = getIniKey(mch_mainINI,"FXLAB","ScriptSource");
	fxlabScriptSource = new char[strlen(t) + 1];
	strcpy(fxlabScriptSource,t);

	t = getIniKey(mch_mainINI,"FXLAB","ScriptDefinition");
	fxlabScriptDefinition = new char[strlen(t) + 1];
	strcpy(fxlabScriptDefinition,t);

	t = getIniKey(mch_mainINI,"FXLAB","ScriptBody");
	fxlabScriptBody = new char[strlen(t) + 1];
	strcpy(fxlabScriptBody,t);

	if(fxlabCompileScript){
		scrSetVerbose();
//		scrSetTempDir(fxlabScriptTmpPath);
		script = parseScript(fxlabScriptSource,fxlabScriptDefinition);
		saveScript(fxlabScriptBody,script);
	}else script = loadScript(fxlabScriptBody);

	Index = new fxlabResourceObject**[FXLAB_SCRIPT_SECTION_MAX];
	for(i = 0;i < FXLAB_SCRIPT_SECTION_MAX;i++){
		Index[i] = NULL;
		Size[i] = 0;
	};

	p = script->nextLevel->first();
	while(p){
		type = p->ID;
		if(type < 0 || type > FXLAB_SCRIPT_SECTION_MAX){
			msg < "fxlabSectionResource " <= type < " out of Range!";
			ErrH.Abort(msg);
		};

		if(Index[type]){
			msg < "fxlabSectionResource " <= type < " Duplicate!";
			ErrH.Abort(msg);
		};

		Size[type] = p->nextLevel->size();
		Index[type] = new fxlabResourceObject*[Size[type]];
		for(i = 0;i < Size[type];i++)
			Index[type][i] = NULL;

		pp = p->nextLevel->first();
		while(pp){
			id = *(pp->i_dataPtr);

			if(id >= Size[type] || id < 0){
				msg < "fxSectionResource " <= type < "Element " <= id < " out of Range!";
				ErrH.Abort(msg);
			};

			if(Index[type][id]){
				msg < "fxSectionResource " <= type < "Element " <= id < " Duplicate!";
				ErrH.Abort(msg);
			};

			n = CreateResource(type);
			n->Type = type;
			n->ID = id;
			n->Open(pp->nextLevel->first());
			Index[type][id] = n;
			
			pp = pp->next;
		};
		p = p->next;
	};

	for(i = 0;i < FXLAB_SCRIPT_SECTION_MAX;i++){
		for(j = 0;j < Size[i];j++){
			Index[i][j]->Link();
		};
	};

	delete script;
};

void fxlabResourceDispatcher::Close(void)
{
	int i,j;
	for(i = 0;i < FXLAB_SCRIPT_SECTION_MAX;i++){
		for(j = 0;j < Size[i];j++){
			Index[i][j]->Close();
			delete Index[i][j];
		};
		delete Index[i];
	};
	delete Index;

	delete fxlabScriptTmpPath;
	delete fxlabScriptSource;
	delete fxlabScriptDefinition;
	delete fxlabScriptBody;

	fxlabUniverseDataType::Close();
	fxlabCosTableType::Close();
};

fxlabResourceObject* fxlabResourceDispatcher::CreateResource(int type)
{
	switch(type){
		case FXLAB_SCRIPT_SECTION_KEY:
			return (new fxlabKeyType);
		case FXLAB_SCRIPT_SECTION_SPRITE:
			return (new fxlabSpriteType);
	};
	ErrH.Abort("Bad ResourceID in fxlabresource");
	return NULL;
};

//-----------------------------------------

void fxlabCosTableType::Open(void)
{
	int i;
	FXLAB_COS_DATA = new int[FXLAB_2PI];
	for(i = 0;i < FXLAB_2PI;i++)
		FXLAB_COS_DATA[i] = round((float)(FXLAB_WAVEGROUND_RANGE) * cos(2.0f * M_PI * (float)(i) / (float)(FXLAB_2PI)));
};

void fxlabCosTableType::Close(void)
{
	delete FXLAB_COS_DATA;
	FXLAB_COS_DATA = NULL;
};

//---------------------------------------

void fxlabUniverseDataType::Open(void)
{
	scrDataBlock* p;
	scrDataBlock* s;
	int i;
	char* t;

	WorldCompileScript = atoi(getIniKey(mch_mainINI,"FXLAB","make_world_script"));

	t = getIniKey(mch_mainINI,"FXLAB","WorldScriptTmpPath");
	WorldScriptTmpPath = new char[strlen(t) + 1];
	strcpy(WorldScriptTmpPath,t);

	t = getIniKey(mch_mainINI,"FXLAB","WavePath");	
	WavePath = new char[strlen(t) + 1];
	strcpy(WavePath,t);

	t = getIniKey(mch_mainINI,"FXLAB","WorldSource");
	WorldSource = new char[strlen(t) + 1];
	strcpy(WorldSource,t);

	t = getIniKey(mch_mainINI,"FXLAB","WorldDefinition");
	WorldDefinition = new char[strlen(t) + 1];
	strcpy(WorldDefinition,t);

	t = getIniKey(mch_mainINI,"FXLAB","WorldBody");
	WorldBody = new char[strlen(t) + 1];
	strcpy(WorldBody,t);

	if(WorldCompileScript){
		scrSetVerbose();
//		scrSetTempDir(WorldScriptTmpPath);
		s = parseScript(WorldSource,WorldDefinition);
		saveScript(WorldBody,s);
	}else s = loadScript(WorldBody);

	WorldNum = s->nextLevel->size();
	WorldData = new fxlabWorldDataType*[WorldNum];
	for(i = 0;i < WorldNum;i++)
		WorldData[i] = NULL;

	p = s->nextLevel->first();
	while(p){
		if(WorldData[*(p->i_dataPtr)])
			ErrH.Abort("Bad fxlabWorld Resource ID");
		WorldData[*(p->i_dataPtr)] = new fxlabWorldDataType;
		if(p->nextLevel)
			WorldData[*(p->i_dataPtr)]->Open(p->nextLevel->first());
		p = p->next;
	};

	delete s;
};

void fxlabUniverseDataType::Close(void)
{
	int i;

	for(i = 0;i < WorldNum;i++){
		WorldData[i]->Close();
		delete WorldData[i];
	};
	delete WorldData;

	delete WorldScriptTmpPath;
	delete WavePath;
	delete WorldSource;
	delete WorldDefinition;
	delete WorldBody;
};

//---------------------------------

fxlabWorldDataType::fxlabWorldDataType(void)
{
};

void fxlabWorldDataType::Open(scrDataBlock* p)
{
	int i;
	scrDataBlock* pp;

	while(p){
		switch(p->ID){
			case FXLAB_SCRIPT_WORLD_TRACK_INFO:
				if(p->nextLevel->size() == FXLAB_WORLD_TRACK_NUM){
					i = 0;
					pp = p->nextLevel->first();
					while(pp){
						TrackData[i].Open(pp);
						pp = pp->next;
						i++;
					};
				}else
					ErrH.Abort("Bad fxlabWorld Track Info");
				break;
		};
		p = p->next;
	};
};

void fxlabWorldDataType::Close(void)
{
	int i;
	for(i = 0;i < FXLAB_WORLD_TRACK_NUM;i++)
		TrackData[i].Close();
};

//----------------------------------------

void fxlabTrackInfoType::Open(scrDataBlock* p)
{
	int i;
	scrDataBlock* pp;

	ReflectionEnable = 0;

	WaveRed = WaterRed = 128;
	WaterDeltaRed = 127;

	WaveGreen = WaterGreen = 128;
	WaterDeltaGreen = 127;

	WaveBlue = WaterBlue = 128;
	WaterDeltaBlue = 127;

	WaveAlpha = WaterAlpha = 128;
	WaterDeltaAlpha = 127;

	WaterDeltaHeight = 0;
	WaterDeltaUV = 0;

	pp = p->nextLevel->first();
	while(pp){
		switch(pp->ID){
			case FXLAB_SCRIPT_WATER_COLOR:
				WaterRed = pp->i_dataPtr[0];
				WaterGreen = pp->i_dataPtr[1];
				WaterBlue = pp->i_dataPtr[2];
				WaterAlpha = pp->i_dataPtr[3];
				break;
			case FXLAB_SCRIPT_WATER_DELTA_COLOR:
				WaterDeltaRed = pp->i_dataPtr[0];
				WaterDeltaGreen = pp->i_dataPtr[1];
				WaterDeltaBlue = pp->i_dataPtr[2];
				WaterDeltaAlpha = pp->i_dataPtr[3];
				break;
			case FXLAB_SCRIPT_WATER_DELTA_HEIGHT:
				WaterDeltaHeight = *(pp->i_dataPtr);
				break;
			case FXLAB_SCRIPT_WATER_DELTA_UV:
				WaterDeltaUV = (float)(*(pp->d_dataPtr));
				break;
			case FXLAB_SCRIPT_WORLD_REFLECTION_ENABLE:
				ReflectionEnable = 1;
				break;
			case FXLAB_SCRIPT_TRACK_DUST_STYLE:
				for(i = 0;i < FXLAB_WORLD_TRACK_DUST_STYLE_MAX;i++)
					DustStyle[i] = pp->i_dataPtr[i];
				break;
			case FXLAB_SCRIPT_TRACK_WAVE_RED:
				WaveRed = (float)(*(pp->d_dataPtr));
				break;
			case FXLAB_SCRIPT_TRACK_WAVE_GREEN:
				WaveGreen = (float)(*(pp->d_dataPtr));
				break;
			case FXLAB_SCRIPT_TRACK_WAVE_BLUE:
				WaveBlue = (float)(*(pp->d_dataPtr));
				break;
			case FXLAB_SCRIPT_TRACK_WAVE_ALPHA:
				WaveAlpha = (float)(*(pp->d_dataPtr));
				break;
		};
		pp = pp->next;
	};
};

void fxlabTrackInfoType::Close(void)
{
};

//-------------------------------------------------------------

#include "TexMgr.h"

void fxlabCreateSpriteTable(void)
{
	TextureBuffer->BeginList(256,256);
	TextureBuffer->AttachChild("resource\\fx\\tga\\sphere0000.tga",FXLAB_ID_IMAGE_SPHERE); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star4_0000.tga",FXLAB_ID_IMAGE_STAR4); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star6_0000.tga",FXLAB_ID_IMAGE_STAR6); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star8_0000.tga",FXLAB_ID_IMAGE_STAR8); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star83_0000.tga",FXLAB_ID_IMAGE_STAR83); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star12s_0000.tga",FXLAB_ID_IMAGE_STAR12S); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star4s_0000.tga",FXLAB_ID_IMAGE_STAR4S); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\star5s_0000.tga",FXLAB_ID_IMAGE_STAR5S); 
	TextureBuffer->AttachChild("resource\\fx\\tga\\bubble0000.tga",FXLAB_ID_IMAGE_BUBBLE); 
	TextureBuffer->EndList();
};

