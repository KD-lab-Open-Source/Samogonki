
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "SST_Reader.h"

#include "mch_rto.h"

#include "mechosoma.h"
#include "race.h"
#include "mesh3ds.h"
#include "xreal.h"
#include "terra.h"
#include "arcane.h"
#include "SpriteDispatcher.h"
#include "arcane_menu.h"
#include "sound.h"

#include "mesh3ds.h"

#include "iText.h"
#include "aci_ids.h"
#include "aci_evnt.h"
#include "aci_scr.h"
#include "hfont.h"
#include "keys.h"

#include "LocalVersion.h"

#include "controls.h"
#include "I-World.h"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "CameraDispatcher.h"
#include "TrackDispatcher.h"

#include "CustomFieldSources.h"

#include "parts_pool.h"

#include "aci_parser.h"
#include "scr_defs.h"

#include "PlayWSC.h"

#include "MechosPrm.h"

#include "cdcheck.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern char acsJoyUse[];

extern int acsY0;
extern int acsMaxEssenceID;
extern int mchFreeResourcesFlag;
extern HFont** acsFntTable;
extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];

extern int mchSplitScreenMode;

extern int mchA_PartID[];
extern int mchPartID[];
extern unsigned char mch_excludeFlags[];
extern xtList<mchRacerStats> mch_racerLst;

extern cInterfaceVisGeneric	*gb_IVisGeneric;

extern XBuffer mchA_XBuf;
extern char mchA_String[];
extern char mch_playerNames[HS_PLAYERS_MAX][HS_NAME_LEN + 2];

extern char* mchIscreenINI;

extern int mchOpenNewChar;

extern GameQuantRTO* quantRTO;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchInitOutro(void);
int mchGetKey(int id,int cfg);

void fxlabPartWave(int x,int y); 
void fxlabPartWaveQuant(void);
void fxlabBigBossStatus(int status);
void fxlabBigBossWorld(int world,int status);

int mchA_ConvertCoords(int x,int y,int z,int& sx,int& sy,int viewport = 0);
void mchA_DrawTextWindow(int x,int y,int sx,int sy,int wnd_alpha,int border_col,int border_alpha);
void mchA_ShowEnergy(int x,int y,float scale,float phase,float phase_scale,int alpha);

void acsSetTurnBased(int mode,int value);
void mchSelectKeysConfig(int cfg);

void iwCreateTeleports(void);
void iwInitWorld(void);

char* GetTargetName(char*);
void mchSetNextQuantRTO(void);
void mchA_DrawMouse(int mode = 0);
int acsGetKeyName(int vkey,int shift,int lng_flag);
aciScreenObject* acsGetObject(int scr_id,int obj_id);
void mchA_DrawM3D(cMesh* p);
void mchA_DrawStar(int x,int y,float sz = 1.0f,float alpha = 1.0f);

void acsSaveGame(void);

void fxlabCreateTeleport(int world_id,int track_id,class Vect3f& p,int status);
void fxlabChangeTeleport(int world_id,int track_id,int status);

int acsCheckSaveName(char* p);

int acsStrLen(int fnt,unsigned char* str,int space);
int acsTextStrLenMax(int fnt,unsigned char* str,int space);
int acsStrHeight(int fnt,unsigned char* str);
int acsTextHeight(int fnt,unsigned char* str,int space);

void iwInitDoggy(void);

void mchA_ShowLayoutID(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

int iwInitFlag = 0;
iWorldDispatcher* iworld_D = NULL;
iWorldDispatcher* iworld_D2 = NULL;
iWorldDispatcher* iworld_Cur = NULL;

float iwElect_Z0;
Vect3f iwElect_R;

float iwAssembly_Z0;
float iwAssembly_MaxZ;
float iwAssembly_Z1;
float iwAssembly_G;
float iwAssembly_RC;
float iwAssembly_A;
float iwAssembly_Diss;
float iwAssembly_dT;

float iwAssembly_CurPartAngle = 0.0f;

Vect3f iwAssembly_R;

int iwElectionaryFlag = 0;

char* iwSpellStr;
char* iwWaitStr;
char* iwTurnBasedStr;
char* iwRealTimeStr;

cMesh* iwElectionary_Arrow = NULL;
cMesh* iwAssembly_Arrow = NULL;
acsTimer* iwArrowRot = NULL;
acsTimer* iwArrowPos = NULL;

float iwArrowSize = 0.4f;

cMesh** iwFigures = NULL;

cMesh** iwParts[4] = { NULL, NULL, NULL, NULL };
iwBody* iwPartsB[4] = { NULL, NULL, NULL, NULL };

int iwPartSize[4];

xtList<iwDoggyPhrase> iwDoggyPhrases;

char* iwBeginMovie = "RESOURCE\\WorldScript\\InterfaceWS.scb";
char* iwEndMovie = "RESOURCE\\WorldScript\\GameVictoryWS.scb";
char* iwOutroMovie = "Outro";

const int IW_NUM_OBJECTS_TO_HIDE	= 8;
char* iwBoss3DS_Names[IW_NUM_OBJECTS_TO_HIDE] = {
	"Puzirks_Frog.m3d",
	"Puzirks_Kit.m3d",
	"Puzirks_Pauk.m3d",
	"Puzirks_Stone.m3d",
	"Puzirks_Dragon.m3d",

	"SuperBoss.m3d",
	"water_doska.3ds",
	"forst_st_logo.3ds"
};

int iwBossSounds[3] = { EFF_SPANKING_STREAM_1, EFF_SPANKING_STREAM_2, EFF_SPANKING_STREAM_3 };

int iwEnableMovies = 1;
int iwEnableIntroMovie = 1;

int iwTimer = 0;
int iwHideArrows = 0;

int iwForceEndMovie = 0;

void iwInit(void)
{
	if(mchCurrentWorld == 7)
		iWorldMode = 1;
	else
		iWorldMode = 0;

	cdCheck_Silent(-1);

	if(iwInitFlag){
		if(iWorldMode){
			iworld_D -> CreateTeleports();
			if(!iworld_D -> NumPlaces())
				iworld_D -> CreatePlaces();
			if(!iworld_D2 -> NumPlaces())
				iworld_D2 -> CreatePlaces();
		}
		return;
	}

	iwSpellStr = iGetText(iTXT_SPELL_NA);
	iwWaitStr = iGetText(iTXT_WAIT);
	iwTurnBasedStr = iGetText(iTXT_TB_MODE);
	iwRealTimeStr = iGetText(iTXT_RT_MODE);

	iwInitDoggy();

	iworld_D = new iWorldDispatcher(0);
	iworld_D2 = new iWorldDispatcher(1);

	iwSelect(0);

	if(iWorldMode)
		iworld_D -> CreateTeleports();

	iwInitFlag = 1;
}

void iWorldDispatcher::CreateTeleports(void)
{
	iwTeleport* tp;

	if(tpLst.size()) return;

	SST_Reader<iwTeleport>::iterator it;
	SST_Reader<iwTeleport> teleport_list(GetTargetName("iwTeleports.sst"));
	FOR_EACH(teleport_list,it){
		tp = new iwTeleport;

		tp -> World = it -> World;
		tp -> Track = it -> Track;
		tp -> PosX = it -> PosX;
		tp -> PosY = it -> PosY;

		tpLst.append(tp);

		fxlabCreateTeleport(it -> World,it -> Track,Vect3f(it -> PosX,it -> PosY,0),1);
	}
}

void iWorldDispatcher::CreatePlaces(void)
{
	iwPlace* p;

	const float dr = 30.0f;
	Vect3f r0,r1;

	p = plLst.first();
	while(p){
		plLst.remove(p);
		delete p;
		p = plLst.first();
	}

	SST_Reader<iwPlace>::iterator pl;
	SST_Reader<iwPlace> place_list(GetTargetName("iwPlaces.sst"));
	FOR_EACH(place_list,pl){
		p = new iwPlace;

		p -> ID = pl -> ID;
		p -> X = pl -> X;
		p -> Y = pl -> Y;
		p -> angle = pl -> angle;

		if(mchSplitScreenGame){
			r0 = Vect3f(cos(float(p -> angle) * M_PI / 180.0f),sin(float(p -> angle) * M_PI / 180.0f),0);
			r1 = r0.postcross(Vect3f::K);

			r1.normalize(dr);

			p -> X += (ID) ? -r1.xi() : r1.xi();
			p -> Y += (ID) ? -r1.yi() : r1.yi();
		}

		plLst.append(p);
	}
}

iWorldDispatcher::iWorldDispatcher(int id)
{
	int i;

	XBuffer buf;
	char* ini_str;

	ID = id;

	owner = NULL;
	viewport = NULL;

	buf.init();
	ini_str = getIniKey(mchIscreenINI,"iWorld","assembly_r");
	buf.write(ini_str,strlen(ini_str));
	buf.set(0,XB_BEG);
	buf >= iwAssembly_R.x >= iwAssembly_R.y >= iwAssembly_R.z;

	iwAssembly_Z0 = atof(getIniKey(mchIscreenINI,"iWorld","assembly_z"));
	iwAssembly_Z1 = atof(getIniKey(mchIscreenINI,"iWorld","assembly_z1"));
	iwAssembly_G = atof(getIniKey(mchIscreenINI,"iWorld","assembly_g"));
	iwAssembly_RC = atof(getIniKey(mchIscreenINI,"iWorld","assembly_rc"));
	iwAssembly_A = atof(getIniKey(mchIscreenINI,"iWorld","assembly_a"));
	iwAssembly_Diss = atof(getIniKey(mchIscreenINI,"iWorld","assembly_diss"));
	iwAssembly_MaxZ = iwAssembly_Z1;

	iwAssembly_dT = atof(getIniKey(mchIscreenINI,"iWorld","assembly_dt"));

	buf.init();
	ini_str = getIniKey(mchIscreenINI,"iWorld","electionary_r");
	buf.write(ini_str,strlen(ini_str));
	buf.set(0,XB_BEG);
	buf >= iwElect_R.x >= iwElect_R.y >= iwElect_R.z;

	iwElect_Z0 = atof(getIniKey(mchIscreenINI,"iWorld","electionary_z"));

	curMovie = curMovieSize = 0;

	keyBuf = new KeyBuffer;

	for(i = 0; i < 4; i ++){
		partSize[i] = 0;
		parts[i] = NULL;
	}

	mode = IW_FREE;
	flags = 0;

	figures = NULL;

	curScreen = NULL;
	activeInput = NULL;

	curPartType = 0;
	curPart = 0;

	lastPlace = 100;

	curScreenPhase = destScreenPhase = 1.0f;

	CreatePlaces();
}

iWorldDispatcher::~iWorldDispatcher(void)
{
	iwTeleport* p;
	p = tpLst.first();

	while(p){
		tpLst.remove(p);
		delete p;
		p = tpLst.first();
	}
}

void iWorldDispatcher::Redraw(void)
{
	acsY0 = GetY0();

	if(curMovieSize && TestWorldScriptPlay.isPlay())
		return;

	if(ElectionaryMode())
		ElectionaryQuant();
/*
	if(!mchSplitScreenGame && RenderMode == DIRECT3D_HICOLOR && mch_arcScrD -> Visible){
		if(flags & IW_TURNBASED_FLAG)
			mchA_d3dOutString(520,460,mchA_FontScaleX[2],mchA_FontScaleY[2],iwTurnBasedStr,mchA_ColorF[2],230,2,0);
		else
			mchA_d3dOutString(530,460,mchA_FontScaleX[2],mchA_FontScaleY[2],iwRealTimeStr,mchA_ColorF[2],230,2,0);
	}
*/
	if(curScreen){
		draw_iScreen(curScreen);
		switch(curScreen -> ID){
			case IW_ELECTIONARY_SCR:
			case IW_ELECTIONARY2_SCR:
				draw_ElectionaryScreen();
				break;
			case IW_ASSEMBLY_SCR:
				draw_AssemblyScreen();
				break;
			case IW_TELEPORT_SCR:
				draw_TeleportScreen();
				break;
			case IW_TELEPORT2_SCR:
				draw_Teleport2Screen();
				break;
			case IW_DOGGY_SCR:
				draw_AdvisorScreen();
				break;
		}

		if(destScreenPhase)
			mchA_DrawMouse(1);

		if(activeInput)
			mchA_ShowLayoutID();

	}

	int x,y,alpha;

	alpha = round((0.5f - curScreenPhase) * 200.0f * 2.0f);
	if(alpha < 0) alpha = 0;

	if(!curScreen) alpha = 200;

	x = viewport -> PosX + 10;
	y = viewport -> PosY + viewport -> SizeY - 30;

	mchA_ShowEnergy(x,y,1.0f,owner -> EnergyValue(),owner -> EnergyValueMax(),alpha);

	ShowNewPhrase();

	if(Disabled())
		draw_DisabledScreen();

	iwTimer = mch_clock();
}

//aciScreenObject* goObj;
void iWorldDispatcher::draw_iScreen(aciScreen* scr)
{
	int y0,mx,my;
	aciScreenObject* p;

	ScaleScreen(scr);

/*	if(scr -> ID == IW_TELEPORT_SCR){
		p = scr -> GetObject(1);
		p -> PosX = 83;
		goObj = p;
	}*/

	p = (aciScreenObject*)scr -> objList -> fPtr;

	mx = round(float(XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2) / mchA_d3dResX);
	my = round(float(XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2) / mchA_d3dResY);

	while(p){
		p -> Quant(mx,my);
		p = (aciScreenObject*)p -> next;
	}

	if(RenderMode == DIRECT3D_HICOLOR){
		if(mchSplitScreenGame && mchSplitScreenMode && !ID)
			y0 = acsY0 - 2;
		else
			y0 = viewport -> PosY + viewport -> SizeY - IW_DY0 + acsY0;

		mchA_d3dOutSprite(viewport -> PosX - 1,y0,float(viewport -> SizeX + 2) / 16.0f,(IW_DY0 + 2.0f) / 16.0f,AE_D3DSPR_DUMMY,0,255);
		mchA_d3dOutSprite(viewport -> PosX - 1,y0,66.0f / 16.0f,(IW_DY0 + 2.0f) / 16.0f,AE_D3DSPR_DUMMY,0,255);
		mchA_d3dOutSprite(viewport -> PosX + viewport -> SizeX - 64,y0,64.0f / 16.0f,(IW_DY0 + 2.0f) / 16.0f,AE_D3DSPR_DUMMY,0,255);
	}

	scr -> redraw(1,1);
}

int iWorldDispatcher::PostMovieQuant(void)
{
	if(curMovieSize){
		if(iwEnableMovies && !mchSplitScreenGame){
			if(!TestWorldScriptPlay.isPlay()){
				if(curMovieSize > curMovie){ 
					if(!strcmp(movies[curMovie],iwOutroMovie))
						return 1;
				}
				else {
					if(curMovieSize == curMovie && !strcmp(movies[curMovie - 1],iwOutroMovie))
						return 1;
				}
			}
		}
	}
	return 0;
}

void iWorldDispatcher::Quant(void)
{
	int mx,my,st;
	float d,dt;
	const float dp = 0.25f;
	Vect3f R;
	iwTeleport* tp;

	iwSelect(ID);

	char* m;

	if(!mchSplitScreenGame){
		nameScrID = IW_NAME_SCR;
	}
	else {
		if(mchSplitScreenMode){
			nameScrID = (ID) ? IW_NAME_HSPLIT_SCR : IW_NAME_SCR;
		}
		else {
			nameScrID = (ID) ? IW_NAME_VSPLIT1_SCR : IW_NAME_VSPLIT2_SCR;
		}
	}

	if(mchSplitScreenGame && !mchSplitScreenMode)
		electionaryScrID = IW_ELECTIONARY2_SCR;
	else
		electionaryScrID = IW_ELECTIONARY_SCR;

	if(curMovieSize){
		if(iwEnableMovies && !mchSplitScreenGame){
			if(!TestWorldScriptPlay.isPlay()){
				m = GetMovie();
				if(m){
					if(!strcmp(m,iwOutroMovie)){
						mchInitOutro();
						return;
					}

					if(curMovie == 1){
						mch_arcScrD -> ToggleVisible(0);
						owner -> kill();
					}

					if(flags & IW_INIT_ELECTIONARY_FLAG){
						HideAssembly();
						HideElectionary();
						InitTeleports(0);
					}

					TestWorldScriptPlay.SetCameraDispatcher(camera_dispatcher);
					TestWorldScriptPlay.SetVisGeneric(gb_IVisGeneric);
					TestWorldScriptPlay.OpenWorldScript(m);
					return;
				}
				else {
					curMovie = curMovieSize = 0;
					mch_arcScrD -> ToggleVisible(1);
					Mdisp -> attachBody(owner);
					owner -> stop_movement();

					if(flags & IW_FIRST_QUANT_FLAG){
						ShowAssembly();
						ShowElectionary();
						InitTeleports(1);
					}

					if(mchTurnBasedGame){
						owner -> RemoveSeeds();
						mchSetTimeMode(MCH_TIME_STOPPED);
					}
					else {
						if(!(flags & IW_INIT_ELECTIONARY_FLAG)){
							if(ID)
								camera_dispatcher2 -> setObserver(owner,1);
							else
								camera_dispatcher -> setObserver(owner,1);
						}
					}
				}
			}
/*
			else {
				if(flags & IW_INIT_ELECTIONARY_FLAG){
					m = TestWorldScriptPlay.GetName();
					if(m && stricmp(m,"cut15")){
						ShowAssembly();
						ShowElectionary();
					}
				}
				return;
			}
*/
		}
		else 
			curMovie = curMovieSize = 0;
	}
	else
		iwHideArrows = 0;

	if(!ID){
		dt = (float)(mch_clock() - iwTimer) / 1000.0f;

		iwArrowRot -> Quant(dt);
		iwArrowPos -> Quant(dt);

		R = iwAssembly_R;
		R.z = iwAssembly_Z0 + 30.0f + 20.0f * iwArrowPos -> Value(); 

		gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)iwAssembly_Arrow,&R,&Vect3f(90.0f,iwArrowRot -> Phase()*180.0f/M_PI,0.0f));

		if(iwHideArrows || mode == IW_ASSEMBLY || (mchSplitScreenGame && iworld_D2 -> AssemblyMode()))
			iwAssembly_Arrow -> SetVisible(0);
		else
			iwAssembly_Arrow -> SetVisible(1);


		R = iwElect_R;
		R.z = iwElect_Z0 + 30.0f + 20.0f * iwArrowPos -> Value(); 

		gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)iwElectionary_Arrow,&R,&Vect3f(90.0f,iwArrowRot -> Phase()*180.0f/M_PI,0.0f));

		if(iwHideArrows || mode == IW_ELECTIONARY || (mchSplitScreenGame && iworld_D2 -> ElectionaryMode()))
			iwElectionary_Arrow -> SetVisible(0);
		else
			iwElectionary_Arrow -> SetVisible(1);

		tp = tpLst.first();
		while(tp){
			if(tp -> arrow){
				d = ZV(XCYCL(tp -> PosX),YCYCL(tp -> PosY)) + 50.0f + 20.0f * iwArrowPos -> Value();
				R = Vect3f(tp -> PosX,tp -> PosY,d);
				gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)tp -> arrow,&R,&Vect3f(90.0f,iwArrowRot -> Phase()*180.0f/M_PI,0.0f));

				if(iwHideArrows || (mode == IW_TELEPORT && nextWorld == tp -> World && nextTrack == tp -> Track) || (mchSplitScreenGame && iworld_D2 -> TeleportMode() && iworld_D2 -> NextWorld() == tp -> World && iworld_D2 -> NextTrack() == tp -> Track))
					tp -> arrow -> SetVisible(0);
				else {
					st = mch_trackD -> TrackMode(tp -> World,tp -> Track);
					if(st == MCH_TRACK_ACCESSIBLE || (st == MCH_TRACK_NEXT_LEVEL && mch_trackD -> TrackPrice(tp -> World,tp -> Track) <= owner -> stPtr -> NStar))
						tp -> arrow -> SetVisible(1);
					else
						tp -> arrow -> SetVisible(0);
				}
			}
			tp = tp -> next;
		}
	}

	d = destScreenPhase - curScreenPhase;
	if(fabs(d) <= 0.05f){
		curScreenPhase = destScreenPhase;
		if(!destScreenPhase && curScreen)
			curScreen = NULL;
	}
	else
		curScreenPhase += d * dp;

	if(flags & IW_INIT_ELECTIONARY_FLAG){
		flags ^= IW_INIT_ELECTIONARY_FLAG;
		InitElectionary();
	}

	acsY0 = round((480.0f - float(IW_Y0)) * (1.0f - curScreenPhase));

	if(!mchSplitScreenGame && flags & IW_FIRST_QUANT_FLAG && !ElectionaryMode())
		figures[owner -> stPtr -> essenceID] -> SetVisible(0);

	AssemblyCoordsQuant();
	KeyQuant();

	if(AssemblyMode())
		AssemblyQuant();

	if(TeleportMode())
		TeleportQuant();

	if(activeInput)
		ActiveInputQuant();

	if(curScreen && acsScrD -> curScr){
		acsScrD -> EventQuant();
		acsScrD -> FlushEvents();
		if(curScreen && acsScrD -> curScr){
			mx = round(float(XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2) / mchA_d3dResX);
			my = round(float(XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2) / mchA_d3dResY);

			acsScrD -> ObjectQuant(mx,my);
			acsScrD -> ActiveInputQuant();
		}
	}

	if(flags & IW_FIRST_QUANT_FLAG)
		ShowObjects(-1);

	flags &= ~IW_FIRST_QUANT_FLAG;
}

void iWorldDispatcher::set_iScreen(int id)
{
	aciScreen* p;

	if(activeInput)
		CancelInput();

	if(!curScreen){ 
		curScreenPhase = 0.0f;
		mchStartSoundEFF(EFF_BOTTOM_MENU);
	}

	p = acsScrD -> GetScreen(id);

	acsScrD -> curScrID = id;
	acsScrD -> curScr = p;

	if(p){
		curScreen = p;
		destScreenPhase = 1.0f;

		curScreen -> prepare();
		curScreen -> init_objects();
		curScreen -> KeyTrap(ACS_STARTUP_KEY);
	}
	else {
		destScreenPhase = 0.0f;
		mchStartSoundEFF(EFF_BOTTOM_MENU);
	}
}

void iWorldDispatcher::KeyTrap(int code)
{
//	int chr,shift = 0,cfg;

	int cfg;

	if(Disabled()) return;

	if(mchSplitScreenGame)
		cfg = ID + 1;
	else
		cfg = 0;

	mchSelectKeysConfig(cfg);

	if(code == VK_ESCAPE && (ElectionaryMode() || AssemblyMode()))
		quantRTO -> NextID = RTO_MAIN_MENU_ID;

	if(fabs(curScreenPhase - destScreenPhase) < 0.5f && destScreenPhase == 1.0f){
		if(activeInput)
			InputQuant(code,-1);
		if(curScreen) 
			curScreen -> KeyTrap(code);
	}

	mchSelectKeysConfig(-1);
}

void iWorldDispatcher::handleEvent(int code,int data1,int data2)
{
	cMesh* m;
	iwTeleport* tp;

	switch(code){
		case IW_KEYTRAP_EV:
			KeyTrap(data1);
			break;
		case IW_DOGGY_EV:
			if(phrases.NewMessages())
				curPhrase = phrases.Get();
			else
				FinitDoggy();
			break;
		case IW_SET_TURNBASED_EV:
			if(data1 == 2){
				SetFlag(IW_EXIT_TELEPORT_FLAG);
			}
			else {
				DropFlag(IW_EXIT_TELEPORT_FLAG);

				if(data1) 
					SetFlag(IW_TURNBASED_FLAG);
				else
					DropFlag(IW_TURNBASED_FLAG);
			}
			break;
		case IW_TOGGLE_TURNBASED_EV:
			if(data1){
				if(CheckFlag(IW_EXIT_TELEPORT_FLAG)){
					DropFlag(IW_EXIT_TELEPORT_FLAG);
					DropFlag(IW_TURNBASED_FLAG);
				}
				else {
					if(CheckFlag(IW_TURNBASED_FLAG))
						SetFlag(IW_EXIT_TELEPORT_FLAG);
					else
						SetFlag(IW_TURNBASED_FLAG);
				}
			}
			else {
				if(CheckFlag(IW_EXIT_TELEPORT_FLAG)){
					DropFlag(IW_EXIT_TELEPORT_FLAG);
					SetFlag(IW_TURNBASED_FLAG);
				}
				else {
					if(CheckFlag(IW_TURNBASED_FLAG))
						DropFlag(IW_TURNBASED_FLAG);
					else
						SetFlag(IW_EXIT_TELEPORT_FLAG);
				}
			}
			TeleportQuant();
			break;
		case IW_MOVE_TO_PLACE:
			lastPlace = data1;
			Move2LastPlace(data2);
			owner -> stop_movement();
			break;
		case IW_TELEPORT_CANCEL_EV:
			if(mode != IW_TELEPORT)
				break;

			set_iScreen(IW_TELEPORT_SCR);

			if(owner -> Type == MCH_RACER_TYPE_MECHOS)
				((mchMechosRacer*)owner) -> cancel_teleportation();

			if(ID)
				camera_dispatcher2 -> setObserver(owner,1);
			else
				camera_dispatcher -> setObserver(owner,1);
			break;
		case IW_TELEPORT_GO_EV:
			if(mode != IW_TELEPORT)
				break;

			if(mchSplitScreenGame)
				handleEvent(IW_TELEPORT_EV);
			else {
				set_iScreen(IW_TELEPORT2_SCR);
				prepareTeleport2Screen();

				tp = getTeleport(nextWorld,nextTrack);
				if(tp)
					teleportR = Vect3f(tp -> PosX,tp -> PosY,owner -> R().z + mechos_teleportation_z_offset);
				else
					teleportR = Vect3f(owner -> R().x,owner -> R().y,owner -> R().z + mechos_teleportation_z_offset);

				camera_dispatcher -> setStaticObserver();
//				owner -> set_brake(1);
				if(owner -> Type == MCH_RACER_TYPE_MECHOS)
					((mchMechosRacer*)owner) -> start_teleportation(teleportR);
			}
			break;
		case IW_ENTER_TELEPORT_EV:
			if(mode == IW_ADVISOR)
				SetFlag(IW_DOGGY_MODE_FLAG);

			nextWorld = data1;
			nextTrack = data2;

			if(mch_trackD -> TrackMode(nextWorld,nextTrack) == MCH_TRACK_ACCESSIBLE)
				mchStartSoundEFF(EFF_PASSAGE_ENTER);

			set_iScreen(IW_TELEPORT_SCR);
			prepareTeleportScreen();
			mode = IW_TELEPORT;

			lastPlace = nextWorld;
			break;
		case IW_EXIT_TELEPORT_EV:
			if(curScreen && curScreen -> ID == IW_TELEPORT2_SCR)
				break;

			if(nextWorld != data1 || nextTrack != data2)
				break;

			if(!CheckFlag(IW_DOGGY_MODE_FLAG)){
				set_iScreen(-1);
				mode = IW_FREE;
			}
			else 
				InitDoggy(0);

			mchStopSoundEFF(EFF_PASSAGE_ENTER);
			break;
		case IW_TELEPORT_EV:
			if(!CheckFlag(IW_EXIT_TELEPORT_FLAG)){
				mchCurrentWorld = nextWorld;
				mchCurrentTrack = nextTrack;
				set_iScreen(-1);
				mode = IW_FREE;

				if(mch_trackD -> TrackMode(nextWorld,nextTrack) == MCH_TRACK_ACCESSIBLE)
					mchStopSoundEFF(EFF_PASSAGE_ENTER);

				mchFreeResourcesFlag = 1;
				mchSetNextQuantRTO();
			}
			else {
				set_iScreen(IW_TELEPORT_SCR);

				if(owner -> Type == MCH_RACER_TYPE_MECHOS)
					((mchMechosRacer*)owner) -> cancel_teleportation();

				if(ID)
					camera_dispatcher2 -> setObserver(owner,1);
				else
					camera_dispatcher -> setObserver(owner,1);

				DropFlag(IW_EXIT_TELEPORT_FLAG);
			}
			break;
		case IW_INPUT_STRING_EV:
			PrepareInput(data1);
			break;
		case IW_SET_SCREEN_EV:
			set_iScreen(data1);
			break;
		case IW_PREV_FIGURE_EV:
			if(owner -> flags & MCH_ACTIVE_RACER2){
				if(camera_dispatcher2 -> isInterpolationCompleted())
					PrevFigure();
			}
			else {
				if(camera_dispatcher -> isInterpolationCompleted())
					PrevFigure();
			}
			break;
		case IW_NEXT_FIGURE_EV:
			if(owner -> flags & MCH_ACTIVE_RACER2){
				if(camera_dispatcher2 -> isInterpolationCompleted())
					NextFigure();
			}
			else {
				if(camera_dispatcher -> isInterpolationCompleted())
					NextFigure();
			}
			break;
		case IW_INIT_ELECTIONARY_EV:
			flags |= IW_ELECTIONARY_FLAG;
			break;
		case IW_FINIT_ELECTIONARY_EV:
			FinitElectionary();
			break;
		case IW_CANCEL_ELECTIONARY_EV:
			if(!(flags & IW_FIRST_ELECTIONARY_FLAG)){
				RestoreElectionary();
				FinitElectionary();
			}
			break;
		case IW_SELECT_FIGURE_EV:
			if(flags & IW_FIRST_ELECTIONARY_FLAG){
				InitFigureName();
				set_iScreen(nameScrID);
			}
			else {
				ChooseFigure();
				FinitElectionary();
			}
			break;
		case IW_CHOOSE_FIGURE_EV:
			ChooseFigure();
			break;
		case IW_INIT_NAME_EV:
			InitFigureName();
			break;
		case IW_INIT_ASSEMBLY_EV:
			InitAssembly();
			break;
		case IW_FINIT_ASSEMBLY_EV:
			FinitAssembly();
			break;
		case IW_CANCEL_ASSEMBLY_EV:
			RestoreAssembly();
			FinitAssembly();
			break;
		case IW_PREV_PART_EV:
			ChangePart(-1);
			break;
		case IW_NEXT_PART_EV:
			ChangePart(1);
			break;
		case IW_SELECT_PART_TYPE_EV:
			if(ID) {
				if(!camera_dispatcher2 -> isInterpolationCompleted()) break;
			}
			else {
				if(!camera_dispatcher -> isInterpolationCompleted()) break;
			}
			mchStartSoundEFF(EFF_TUB_TOGGLE);

			SetCurPartType(data1);
			break;
		case IW_BUY_TRACK_EV:
			mch_trackD -> OpenTrack(nextWorld,nextTrack);
			owner -> stPtr -> NStar -= mch_trackD -> TrackPrice(nextWorld,nextTrack);
			fxlabChangeTeleport(nextWorld,nextTrack,2);
			mchStartSoundEFF(EFF_TUB_BEEP);
			break;
		case IW_PREV_PART_TYPE_EV:
			if(ID) {
				if(!camera_dispatcher2 -> isInterpolationCompleted()) break;
			}
			else {
				if(!camera_dispatcher -> isInterpolationCompleted()) break;
			}
			mchStartSoundEFF(EFF_TUB_TOGGLE);

			if(++curPartType >= NUM_ACTIVE_PARTS - 1)
				curPartType = 0;
			SetCurPart();
			SetAssemblyCamera();
			break;
		case IW_NEXT_PART_TYPE_EV:
			if(ID) {
				if(!camera_dispatcher2 -> isInterpolationCompleted()) break;
			}
			else {
				if(!camera_dispatcher -> isInterpolationCompleted()) break;
			}
			mchStartSoundEFF(EFF_TUB_TOGGLE);

			if(--curPartType < 0)
				curPartType = NUM_ACTIVE_PARTS - 2;
			SetCurPart();
			SetAssemblyCamera();
			break;
		case IW_ARCANE_DEMO_EV:
			m = ((mchMechosRacer*)owner) -> part_by_type(mchPartID[curPartType + 1]);
			if(m){
				if(m -> arcaneData.Status){
					m -> arcaneData.ownerID = owner -> ID;
					m -> arcaneData.fxlabStart();
				}
			}
			break;
		case IW_CHECK_NAME_EV:
			if(!(flags & IW_CANCEL_INPUT_FLAG)){
				if(!CheckFigureName() || mchSplitScreenGame){
					ChooseFigure();
					FinitElectionary();
				}
				else 
					set_iScreen(IW_NAME2_SCR);
			}
			else
				set_iScreen(electionaryScrID);
			break;
	}
}

void iWorldDispatcher::prepareTeleportScreen(void)
{
/*
	char* name;
	XBuffer str;
	aciScreenInputField* p;

	if(!curScreen) return;

	name = mch_trackD -> TrackName(nextWorld,nextTrack);
	if(name){
		p = (aciScreenInputField*)curScreen -> GetObject(2);
		if(p){
			str < name < " (" <= nextTrack < ")";
			strcpy(p -> string,str);
		}

		p = (aciScreenInputField*)curScreen -> GetObject(3);
		if(p){
			str.init();
			str < " ";
			strcpy(p -> string,str);
		}
	}
*/
}

void iWorldDispatcher::ActiveInputQuant(void)
{
	int chr,sz,flag = 0;
	static int inputTimer = 0;

	if(activeInput){
		if(XKey.LastChar != -1){
			if(isprint(XKey.LastChar)){
				chr = XKey.LastChar;
				if(chr != -1)
					InputQuant(0,chr);
			}

			XKey.LastChar = -1;
		}

		inputTimer ++;
		if(inputTimer >= 3){
			inputTimer = 0;
			flag = 1;
		}
		if(flag){
			sz = strlen(activeInput -> string);
			if(activeInput -> string[sz - 1] == '_')
				activeInput -> string[sz - 1] = ' ';
			else
				activeInput -> string[sz - 1] = '_';
			flag = 0;
		}
	}
}

void iWorldDispatcher::InputQuant(int code,int chr)
{
	int sz,id;
	unsigned char* ptr = NULL;
	HFont* hfnt = NULL;

	if(!activeInput) return;

	id = mchGetKeyID_First(code);

	ptr = (unsigned char*)activeInput -> string;
	if(activeInput -> flags & ACS_ISCREEN_FONT)
		hfnt = acsFntTable[activeInput -> font];

	switch(code){
		case VK_RETURN:
			DoneInput();
			break;
		case VK_ESCAPE:
			CancelInput();
			break;
		case VK_LEFT:
		case VK_BACK:
			sz = strlen((char*)ptr);
			if(sz > 1){
				ptr[sz - 1] = 0;
				ptr[sz - 2] = '_';
				activeInput -> flags |= ACS_REDRAW_OBJECT;
			}
			break;
		default:
			if(chr && chr != -1){
				if(hfnt && (hfnt -> data[chr] -> Flags & NULL_HCHAR) && chr != ' ')
					break;
				sz = strlen((char*)ptr);
				if(sz <= activeInput -> MaxStrLen){
					ptr[sz - 1] = chr;
					ptr[sz] = '_';
					ptr[sz + 1] = 0;
					activeInput -> flags |= ACS_REDRAW_OBJECT;
				}
				mchStartSoundEFF(EFF_TYPE_KEY);
			}
			break;
	}
/*
	switch(id){
		case MCH_KEY_OK:
			DoneInput();
			break;
		case MCH_KEY_CANCEL:
			CancelInput();
			break;
		case MCH_KEY_TURN_LEFT:
			sz = strlen((char*)ptr);
			if(sz > 1){
				ptr[sz - 1] = 0;
				ptr[sz - 2] = '_';
				activeInput -> flags |= ACS_REDRAW_OBJECT;
			}
			break;
		default:
			if(chr && chr != -1){
				if(hfnt && (hfnt -> data[chr] -> Flags & NULL_HCHAR) && chr != ' ')
					break;
				sz = strlen((char*)ptr);
				if(sz <= activeInput -> MaxStrLen){
					ptr[sz - 1] = chr;
					ptr[sz] = '_';
					ptr[sz + 1] = 0;
					activeInput -> flags |= ACS_REDRAW_OBJECT;
				}
				mchStartSoundEFF(EFF_TYPE_KEY);
			}
			break;
	}

	id = mchGetKeyID_Next();

	while(id != -1){
		switch(id){
			case MCH_KEY_OK:
				DoneInput();
				break;
			case MCH_KEY_CANCEL:
				CancelInput();
				break;
			case MCH_KEY_TURN_LEFT:
				sz = strlen((char*)ptr);
				if(sz > 1){
					ptr[sz - 1] = 0;
					ptr[sz - 2] = '_';
					activeInput -> flags |= ACS_REDRAW_OBJECT;
				}
				break;
		}

		id = mchGetKeyID_Next();
	}
*/
}

void iWorldDispatcher::PrepareInput(int obj_id)
{
	int sz;
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(obj_id);
	if(p){
		if(activeInput){
			activeInput -> StopEvents();
			CancelInput();
		}
		sz = strlen(p -> string) + 1;
		strcpy(backupStr,p -> string);

		p -> string[sz - 1] = '_';
		p -> string[sz] = 0;
		p -> flags |= ACS_ACTIVE_STRING;
		activeInput = p;

		iwDisable(ID ^ 1);
	}
}

void iWorldDispatcher::CancelInput(void)
{
	strcpy(activeInput -> string,backupStr);
	activeInput -> flags &= ~ACS_ACTIVE_STRING;
	activeInput = NULL;

	flags |= IW_CANCEL_INPUT_FLAG;
	iwEnable(ID ^ 1);
}

void iWorldDispatcher::DoneInput(void)
{
	int sz;
	sz = strlen(activeInput -> string);
	activeInput -> string[sz - 1] = 0;
	activeInput -> flags &= ~ACS_ACTIVE_STRING;
	activeInput = NULL;

	flags &= ~IW_CANCEL_INPUT_FLAG;
	iwEnable(ID ^ 1);
}

int iwEventActive(aciScreenEventCommand* p)
{
	switch(p -> data[0]){
		case IW_INPUT_STRING_EV:
			if(iworld_Cur -> Input()) 
				return 0;
			break;
		default:
			return 1;
	}
	return 1;
}

void iWorldDispatcher::InitElectionary(void)
{
	if(mode == IW_ELECTIONARY)
		return;

	mode = IW_ELECTIONARY;
	owner -> kill();
	curFigure = owner -> stPtr -> essenceID;
	
	set_iScreen(electionaryScrID);

	SetElectionaryCamera();
	figures[curFigure] -> SetVisible(1);
	SetFigureAnimation(curFigure,0);

	lastPlace = 101;

	if(flags & IW_FIRST_QUANT_FLAG){
		flags |= IW_FIRST_ELECTIONARY_FLAG;
		PutPhrase(IW_DOGGY_GREETING);
	}
	else 
		flags &= ~IW_FIRST_ELECTIONARY_FLAG;

	BackupElectionary();
	mchStartSoundEFF(EFF_INTERFACE_TUB);
}

void iWorldDispatcher::FinitElectionary(void)
{
	mode = IW_FREE;
	Mdisp -> attachBody(owner);
	owner -> stop_movement();

	set_iScreen(-1);

	if(mchTurnBasedGame){
		owner -> RemoveSeeds();
		mchSetTimeMode(MCH_TIME_STOPPED);
	}
	else {
		if(ID)
			camera_dispatcher2 -> setObserver(owner);
		else
			camera_dispatcher -> setObserver(owner);
	}

	SetFigureAnimation(curFigure,0);

	if(!mchSplitScreenGame)
		figures[curFigure] -> SetVisible(0);

	Move2LastPlace(1);
	mchStartSoundEFF(EFF_TUB_INTERFACE);

	if(owner -> stPtr -> essenceID != figureBackup && !(flags & IW_FIRST_ELECTIONARY_FLAG))
		PutPhrase(IW_DOGGY_NEW_RIDER);

	acsSaveGame();
}

void iWorldDispatcher::InitWorld(void)
{
	int i,j;

	nextWorld = nextTrack = -1;

	SensorFieldSource* p;

	keyBuf -> clear();

	curScreen = NULL;

	DropFlag(IW_EXIT_FLAG | IW_EXIT_TELEPORT_FLAG);

	if(mchTurnBasedGame){
		mchTurnBasedGame = 0;
		flags |= IW_TURNBASED_FLAG;
	}

	if(!mchSplitScreenGame){
		nameScrID = IW_NAME_SCR;
	}
	else {
		if(mchSplitScreenMode)
			nameScrID = (ID) ? IW_NAME_HSPLIT_SCR : IW_NAME_SCR;
		else
			nameScrID = (ID) ? IW_NAME_VSPLIT1_SCR : IW_NAME_VSPLIT2_SCR;
	}

	if(mchSplitScreenGame && !mchSplitScreenMode)
		electionaryScrID = IW_ELECTIONARY2_SCR;
	else
		electionaryScrID = IW_ELECTIONARY_SCR;

	flags |= IW_FIRST_QUANT_FLAG;
	mode = IW_FREE;

	p = new SensorFieldSource(Vect3f(98,944,0),20,IW_CREDITS_SENSOR);
	Mdisp -> attachFieldSource(p); 

	if(mch_trackD -> CheckLevel(mch_trackD -> CurLevel())){
		mch_trackD -> NextLevel();
	}

	// Electionary
	p = new SensorFieldSource(iwElect_R,100,IW_ELECTIONARY_SENSOR);
	Mdisp -> attachFieldSource(p); 

	if(!figures) figures = new cMesh*[acsMaxEssenceID];
	for(i = 0; i < acsMaxEssenceID; i ++)
		figures[i] = iwFigures[i];

	InitFigures();

	// Assembly
	p = new SensorFieldSource(iwAssembly_R,50,IW_ASSEMBLY_SENSOR);
	Mdisp -> attachFieldSource(p); 

	if(!parts[0]){
		for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++){
			partSize[i] = iwPartSize[i];
			parts[i] = new cMesh*[partSize[i]];
		}
	}

	for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++){
		for(j = 0; j < partSize[i]; j ++){
			parts[i][j] = iwParts[i][j];
		}
	}

	SetCurPart();
	AssemblyPartsQuant();

	// Teleports
	InitTeleports();
	CreatePlaces();

	if(!(owner -> stPtr -> flags & MCH_RACER_WAS_ON_WORLD))
		PutPhrase(IW_DOGGY_FIRST_RACE);
	if(mch_trackD -> IsBossWorldOpen())
		PutPhrase(IW_DOGGY_NEW_BOSS_WORLD);
	if(mch_trackD -> IsNewWorldOpen(owner -> stPtr -> NStar))
		PutPhrase(IW_DOGGY_NEW_WORLD);

	iwTimer = mch_clock();

	DropFlag(IW_DOGGY_MODE_FLAG);
}

void iWorldDispatcher::SetElectionaryCamera(void)
{
	Vect3f r,angles;
	int i,mode,num = 0;
	float al,z = 100.0f,d = 0.0f;

	const float rr = 60.0f;

	for(i = 0; i < curFigure; i ++){
		if(mch_trackD -> FigureEnabled(i)) num ++;
	}

	al = M_PI * 2.0f / float(numActiveFigures) * float(num);

	r.x = iwElect_R.x + rr * cos(al);
	r.y = iwElect_R.y + rr * sin(al);

	if(!mchSplitScreenGame || !mchSplitScreenMode)
		r.z = ZV(XCYCL(int(r.x)),YCYCL(int(r.y))) + 50.0f;
	else {
		if(ID){
			z = 200.0f;
			r.z = ZV(XCYCL(int(r.x)),YCYCL(int(r.y))) + 50.0f;
		}
		else {
			z = 200.0f;
			r.z = ZV(XCYCL(int(r.x)),YCYCL(int(r.y))) + 70.0f;
		}
	}

	angles.x = 60.0f;
	angles.y = 0.0f;
	angles.z = -al * 180.0f / M_PI - 90;

	if(flags & IW_FIRST_QUANT_FLAG){
		mode = 0;
/*
		if(iwEnableMovies)
			mode = 2;
		else 
			mode = 0;
*/
	}
	else 
		mode = 1;

	if(ID)
		camera_dispatcher2 -> setStaticObserver(r, angles, z, d, mode*800);
	else
		camera_dispatcher -> setStaticObserver(r, angles, z, d, mode*800);
}

void iWorldDispatcher::NextFigure(void)
{
	mchStartSoundEFF(EFF_TUB_TOGGLE);
//	SetFigureAnimation(curFigure,0);
	if(++curFigure >= acsMaxEssenceID) 
		curFigure = 0;

	while(!mch_trackD -> FigureEnabled(curFigure)){
		if(++curFigure >= acsMaxEssenceID) 
			curFigure = 0;
	}

	SetFigureAnimation(curFigure,0);
	SetElectionaryCamera();
}

void iWorldDispatcher::PrevFigure(void)
{
	mchStartSoundEFF(EFF_TUB_TOGGLE);
//	SetFigureAnimation(curFigure,0);
	if(--curFigure < 0) 
		curFigure = acsMaxEssenceID - 1;

	while(!mch_trackD -> FigureEnabled(curFigure)){
		if(--curFigure < 0) 
			curFigure = acsMaxEssenceID - 1;
	}

	SetFigureAnimation(curFigure,0);
	SetElectionaryCamera();
}

void iWorldDispatcher::ChooseFigure(void)
{
	aciScreenInputField* p = (aciScreenInputField*)acsGetObject(nameScrID,2);
	if(owner -> stPtr -> essenceID != curFigure){
		owner -> ChangePart(0,curFigure);
	}
	if(p && flags & IW_FIRST_ELECTIONARY_FLAG)
		strcpy(owner -> stPtr -> name,p -> string);
}

int iWorldDispatcher::CheckFigureName(void)
{
	aciScreenInputField* p = (aciScreenInputField*)acsGetObject(nameScrID,2);

	if(p)
		return acsCheckSaveName(p -> string);

	return 0;
}

void iWorldDispatcher::InitFigureName(void)
{
	aciScreenInputField* p = (aciScreenInputField*)acsGetObject(nameScrID,2);
	if(p)
		strcpy(p -> string,mch_playerNames[curFigure]);

	switch(curFigure){
		case 0:
			mchStartSoundEFF(EFF_ANIMATION_1_LUCKY);
			break;
		case 1:
			mchStartSoundEFF(EFF_ANIMATION_2_LUCKY);
			break;
		case 2:
			mchStartSoundEFF(EFF_ANIMATION_3_LUCKY);
			break;
		case 3:
			mchStartSoundEFF(EFF_ANIMATION_4_LUCKY);
			break;
		case 4:
			mchStartSoundEFF(EFF_ANIMATION_5_LUCKY);
			break;
	}

	SetFigureAnimation(curFigure,1);
}

void iWorldDispatcher::InitAssembly(void)
{
	if(mode == IW_ASSEMBLY)
		return;

	iwAssembly_MaxZ = iwAssembly_Z0 + 100.0f;

	mode = IW_ASSEMBLY;
	set_iScreen(IW_ASSEMBLY_SCR);

	owner -> setPosition(iwAssembly_R,0);
	owner -> set_brake(1);

	SetCurPart();

	lastPlace = 100;

	BackupAssembly();

	SetAssemblyCamera();

	mchStartSoundEFF(EFF_INTERFACE_TUB);
}

void iWorldDispatcher::FinitAssembly(void)
{
	mode = IW_FREE;

	iwAssembly_MaxZ = iwAssembly_Z1;

	set_iScreen(-1);

	if(mchTurnBasedGame){
		owner -> RemoveSeeds();
		mchSetTimeMode(MCH_TIME_STOPPED);
	}
	else {
		if(ID)
			camera_dispatcher2 -> setObserver(owner);
		else
			camera_dispatcher -> setObserver(owner);
	}

	owner -> set_brake(0);
	owner -> stop_movement();

	Move2LastPlace();
	mchStartSoundEFF(EFF_TUB_INTERFACE);

	acsSaveGame();
}

void iWorldDispatcher::draw_ElectionaryScreen(void)
{
	int x,y,sx,sy,al;
	char* text;

	aciScreenInputField* p;

	if(!curScreen) return;

	p = (aciScreenInputField*)curScreen -> GetObject(1);
	if(p){
		sx = acsStrLen(1,(unsigned char*)mch_playerNames[curFigure],0);
		sy = acsStrHeight(1,(unsigned char*)mch_playerNames[curFigure]);
		x = p -> PosX + (p -> SizeX0 - sx)/2;
		y = p -> PosY + (p -> SizeY0 - sy)/2 - 3;
		mchA_d3dOutString(x,y + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],mch_playerNames[curFigure],mchA_ColorF[2],230,1,0);
	}

	p = (aciScreenInputField*)curScreen -> GetObject(2);
	if(p){
		al = p -> AlphaCur + 127;

		x = p -> PosX + p -> SizeX0/2;
		y = p -> PosY + p -> SizeY0/2;

		mchA_d3dOutSprite(x,y + acsY0,1.0f,1.0f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,M_PI,1.0f);
	}

	p = (aciScreenInputField*)curScreen -> GetObject(3);
	if(p){
		al = p -> AlphaCur + 127;

		x = p -> PosX + p -> SizeX0/2;
		y = p -> PosY + p -> SizeY0/2;

		mchA_d3dOutSprite(x,y + acsY0,1.0f,1.0f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,0.0f,1.0f);
	}

	text = iGetText(iTXT_CHAR00 + curFigure);
	sx = acsTextStrLenMax(2,(unsigned char*)text,0);
	sy = acsTextHeight(2,(unsigned char*)text,-2);

	if(!mchSplitScreenGame){
		x = viewport -> PosX + viewport -> SizeX - sx - 20;
		y = viewport -> PosY + 20;

		mchA_DrawTextWindow(x - 10,y - 10,sx + 20,sy + 20,round(150.0f * curScreenPhase),2,round(200.0f * curScreenPhase));
		mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],text,mchA_ColorF[2],round(230.0f * curScreenPhase),2,0,1.0f,1,-2);
	}
}

void iWorldDispatcher::draw_AssemblyScreen(void)
{
	int x,y,al,sx,sy,fnt;
	float scale;
	cMesh* m;
	aciScreenObject* p;
	aciScreenInputField* p1;

	aciScreen* scr = acsScrD -> GetScreen(IW_ASSEMBLY_SCR);

	if(!scr) return;

	if(parts[curPartType][curPart] -> arcaneData.ID != -1 && !owner -> stPtr -> partFlags[parts[curPartType][curPart] -> arcaneData.ID]){
		fnt = (!mchSplitScreenGame || mchSplitScreenMode) ? 1 : 2;

		sx = acsStrLen(fnt,(unsigned char*)iwSpellStr,1);
		x = (viewport -> SizeX - sx)/2;
		mchA_d3dOutString(viewport -> PosX + x,viewport -> PosY + viewport -> SizeY/2 + 30,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],iwSpellStr,mchA_ColorF[4],round(230.0f * curScreenPhase),fnt,1);

		p = curScreen -> GetObject(10);
		if(p && p -> flags & ACS_HIDDEN){
 			mchA_d3dOutSprite(p -> PosX + p -> SizeX/2,GetScrY() + acsY0 + IW_DY0/2,1.5f,1.5f,AE_D3DSPR_CIRCLE,mchA_ColorF[2],230,0.0f,1);
 			mchA_d3dOutSprite(p -> PosX + p -> SizeX/2,GetScrY() + acsY0 + IW_DY0/2,40.0f / 16.0f,3.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[2],230,-M_PI/4.0f,1);
		}

		mchA_ConvertCoords(parts[curPartType][curPart] -> x(),parts[curPartType][curPart] -> y(),parts[curPartType][curPart] -> z(),x,y,ID);

		if(curScreenPhase >= 0.95f){
			mchA_d3dOutSprite(x,y,3.5f,3.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[4],160,M_PI/4.0f,1);
			mchA_d3dOutSprite(x,y,3.5f,3.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[4],160,-M_PI/4.0f,1);
		}
	}

	p = scr -> GetObject(11);
	m = parts[curPartType][curPart];

	if(p && m){
		if(!mchSplitScreenGame || mchSplitScreenMode){
			mchA_SprD -> DrawArcaneSymbol(p -> PosX,p -> PosY + acsY0,m -> arcaneData.ID,0.0f,0.0f,2,255);
			if(m -> arcaneData.name)
				mchA_d3dOutString(p -> PosX + 50,p -> PosY + 10 + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],m -> arcaneData.name,mchA_ColorF[2],230,1,0);
		}
		else {
			mchA_SprD -> DrawArcaneSymbol(p -> PosX,p -> PosY + acsY0,m -> arcaneData.ID,0.0f,0.0f,2,255,0.7f);

			if(m -> arcaneData.name)
				mchA_d3dOutString(p -> PosX + 38,p -> PosY + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],m -> arcaneData.name,mchA_ColorF[2],230,2,0);
		}

		if(m -> arcaneData.text){
			sx = acsTextStrLenMax(2,(unsigned char*)m -> arcaneData.text,0);
			sy = acsTextHeight(2,(unsigned char*)m -> arcaneData.text,-2);

			if(mchSplitScreenGame){
/*
				sx = round(float(sx) * 0.8f);
				sy = round(float(sy) * 0.8f);

				x = viewport -> PosX + viewport -> SizeX - sx - 18;
				y = viewport -> PosY + 18;

				mchA_DrawTextWindow(x - 8,y - 8,sx + 16,sy + 16,round(150.0f * curScreenPhase),2,round(200.0f * curScreenPhase));
				mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],m -> arcaneData.text,mchA_ColorF[2],round(230.0f * curScreenPhase),2,0,0.8f,1,-2);
*/
			}
			else {
				x = viewport -> PosX + viewport -> SizeX - sx - 20;
				y = viewport -> PosY + 20;

				mchA_DrawTextWindow(x - 10,y - 10,sx + 20,sy + 20,round(150.0f * curScreenPhase),2,round(200.0f * curScreenPhase));
				mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],m -> arcaneData.text,mchA_ColorF[2],round(230.0f * curScreenPhase),2,0,1.0f,1,-2);
			}
		}
	}

	m = ((mchMechosRacer*)owner) -> part_by_type(mchPartID[curPartType + 1]);

	if(m){
		if(ID)
			mch_arcScrD -> playerSet2 -> statD -> GetCoords(m -> arcaneData.ID,x,y);
		else
			mch_arcScrD -> playerSet1 -> statD -> GetCoords(m -> arcaneData.ID,x,y);

		mchA_d3dOutSprite(x + 40,y,0.7f,0.7f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],round(200.0f * curScreenPhase),M_PI,1.0f);
	}

	if(!mchSplitScreenGame || mchSplitScreenMode){
		scale = 0.8f;
		p1 = (aciScreenInputField*)curScreen -> GetObject(12);
		if(p1){
			p1 -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			al = p1 -> AlphaCur + 127;

			x = p1 -> PosX + p1 -> SizeX/2;
			y = p1 -> PosY + p1 -> SizeY/2;

			mchA_d3dOutSprite(x,y + acsY0,scale,scale,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,M_PI,1.0f);
		}

		p1 = (aciScreenInputField*)curScreen -> GetObject(13);
		if(p1){
			p1 -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			al = p1 -> AlphaCur + 127;

			x = p1 -> PosX + p1 -> SizeX/2;
			y = p1 -> PosY + p1 -> SizeY/2;

			mchA_d3dOutSprite(x,y + acsY0,scale,scale,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,0.0f,1.0f);
		}

		p1 = (aciScreenInputField*)curScreen -> GetObject(14);
		if(p1){
			p1 -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			al = p1 -> AlphaCur + 127;

			x = p1 -> PosX + p1 -> SizeX/2;
			y = p1 -> PosY + p1 -> SizeY/2;

			mchA_d3dOutSprite(x,y + acsY0,scale,scale,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,-M_PI/2.0f,1.0f);
		}

		p1 = (aciScreenInputField*)curScreen -> GetObject(15);
		if(p1){
			p1 -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			al = p1 -> AlphaCur + 127;

			x = p1 -> PosX + p1 -> SizeX/2;
			y = p1 -> PosY + p1 -> SizeY/2;

			mchA_d3dOutSprite(x,y + acsY0,scale,scale,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,M_PI/2.0f,1.0f);
		}
	}
	else {
		p1 = (aciScreenInputField*)curScreen -> GetObject(12);
		if(p1)
			p1 -> flags |= ACS_HIDDEN | ACS_BLOCKED;

		p1 = (aciScreenInputField*)curScreen -> GetObject(13);
		if(p1)
			p1 -> flags |= ACS_HIDDEN | ACS_BLOCKED;

		p1 = (aciScreenInputField*)curScreen -> GetObject(14);
		if(p1)
			p1 -> flags |= ACS_HIDDEN | ACS_BLOCKED;

		p1 = (aciScreenInputField*)curScreen -> GetObject(15);
		if(p1)
			p1 -> flags |= ACS_HIDDEN | ACS_BLOCKED;
	}
}

void iWorldDispatcher::AssemblyPartsQuant(int mode)
{
	int i,j;

	const float r = 1.0f;
	const float g = 1.0f;
	const float b = 1.0f;
	const float a = 0.4f;

	for(j = 0; j < NUM_ACTIVE_PARTS - 1; j ++){
		for(i = 0; i < partSize[j]; i ++){
			if(j == curPartType){
				parts[j][i] -> SetVisible(mode);
				if(parts[j][i] -> arcaneData.ID == -1 || owner -> stPtr -> partFlags[parts[j][i] -> arcaneData.ID])
					parts[j][i] -> SetColor(sColor4f(r,g,b,1.0f));
				else
					parts[j][i] -> SetColor(sColor4f(r,g,b,a));
			}
			else {
				parts[j][i] -> SetVisible(0);
			}
		}
	}
}

void iWorldDispatcher::AssemblyQuant(void)
{
	const float da = 3.0f;

	AssemblyPartsQuant();

	iwAssembly_CurPartAngle += da;
	if(iwAssembly_CurPartAngle > 360.0f) iwAssembly_CurPartAngle -= 360.0f;

	gb_IVisGeneric -> dSetObjectPosition((cUnknownClass*)parts[curPartType][curPart],&Vect3f(0,0,0),&Vect3f(0,0,iwAssembly_CurPartAngle));

	aciScreenObject* p;
	p = curScreen -> GetObject(10);

	if(!p) return;

	if(parts[curPartType][curPart] -> arcaneData.ID != -1 && !owner -> stPtr -> partFlags[parts[curPartType][curPart] -> arcaneData.ID])
		p -> flags |= ACS_HIDDEN;
	else
		p -> flags &= ~(ACS_HIDDEN);
}

void iWorldDispatcher::SetAssemblyCamera(void)
{
	Vect3f r,dr,angles;
	float al,z = 150.0f,d = 0.0f;

	const float radius = 150.0f;

	al = M_PI * 2.0f / float(partSize[curPartType]) * float(curPart);
	r = Vect3f(iwAssembly_R.x + radius * cos(al),iwAssembly_R.y + radius * sin(al),iwAssembly_R.z);

	dr = iwAssembly_R - r;
	al = M_PI/2.0f - dr.psi();

	if(mchSplitScreenGame && mchSplitScreenMode){ 
		z = 220.0f;
		angles.x = 70.0f;
	}
	else
		angles.x = 60.0f;

	angles.y = 0.0f;
	angles.z = al * 180.0f / M_PI + 180.0f;

	owner -> setPosition(iwAssembly_R,al + M_PI);

	if(mchSplitScreenGame && mchSplitScreenMode){ 
		r = iwAssembly_R - dr / 4.5f;

		if(ID)
			r.z = owner -> R().z + 10;
		else
			r.z = owner -> R().z + 30;
	}
	else {
		r = iwAssembly_R - dr / 3.0f;
		r.z = owner -> R().z + 20;
	}

	if(ID)
		camera_dispatcher2 -> setStaticObserver(r, angles, z, d, 500);
	else
		camera_dispatcher -> setStaticObserver(r, angles, z, d, 500);
}

void iWorldDispatcher::ChangePart(int dir)
{
	int cnt = 0;

	if(ID) {
		if(!camera_dispatcher2 -> isInterpolationCompleted()) return;
	}
	else {
		if(!camera_dispatcher -> isInterpolationCompleted()) return;
	}
	mchStartSoundEFF(EFF_TUB_TOGGLE);

	if(dir == 1){
		if(++curPart >= partSize[curPartType])
			curPart = 0;
	}
	else {
		if(--curPart < 0)
			curPart = partSize[curPartType] - 1;
	}
	if(parts[curPartType][curPart] -> arcaneData.ID == -1 || owner -> stPtr -> partFlags[parts[curPartType][curPart] -> arcaneData.ID]){
		owner -> ChangePart(curPartType + 1,M3D_KIND(parts[curPartType][curPart] -> Type) - 1);
		mch_raceD -> ArcaneQuant(owner,1000000.0f);
	}

	iwAssembly_CurPartAngle = 0.0f;

	SetAssemblyCamera();
}

void iWorldDispatcher::SetCurPart(void)
{
	int i;
	cMesh* p = ((mchMechosRacer*)owner) -> part_by_type(mchPartID[curPartType + 1]);

	for(i = 0; i < partSize[curPartType]; i ++){
		if(p -> Type == parts[curPartType][i] -> Type){
			curPart = i;
			return;
		}
	}
}

void iWorldDispatcher::SetCurPartType(int tp)
{
	curPartType = tp;
	SetCurPart();

	SetAssemblyCamera();
}

void iWorldDispatcher::TeleportQuant(void)
{
	if(!curScreen || mode != IW_TELEPORT) return;

	nextTrackStatus = mch_trackD -> TrackMode(nextWorld,nextTrack);
	nextTrackPrice = mch_trackD -> TrackPrice(nextWorld,nextTrack);

	aciScreenObject* go_obj,*buy_obj;
	
	if(curScreen -> ID == IW_TELEPORT_SCR){
		go_obj = curScreen -> GetObject(1);
		buy_obj = curScreen -> GetObject(4);

		if(go_obj) go_obj -> flags |= ACS_HIDDEN | ACS_BLOCKED;
		if(buy_obj) buy_obj -> flags |= ACS_HIDDEN | ACS_BLOCKED;

		switch(nextTrackStatus){
			case MCH_TRACK_NEXT_LEVEL:
				if(nextTrackPrice <= owner -> stPtr -> NStar)
					if(buy_obj) buy_obj -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
				break;
			case MCH_TRACK_DONE:
			case MCH_TRACK_ACCESSIBLE:
				if(go_obj) go_obj -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
				break;
		}
	}
	if(curScreen -> ID == IW_TELEPORT2_SCR)
		prepareTeleport2Screen();
}

void iWorldDispatcher::draw_TeleportScreen(void)
{
	int id,sz,y0,info_flag = 0;
	int x,y,sx,sy;

	char* name,*text;
	XBuffer str;

	if(RenderMode != DIRECT3D_HICOLOR) return;

	y0 = GetScrY();

	switch(nextTrackStatus){
		case MCH_TRACK_NEXT_LEVEL:
			sprintf(mchA_String,iGetText(iTXT_TO_BUY),nextTrackPrice);
			if(!mchSplitScreenGame || mchSplitScreenMode){
				mchA_d3dOutString(250,y0 + 10 + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[2],230,1,1);
				mchA_DrawStar(235,y0 + 25 + acsY0,0.7f);
			}
			else {
				mchA_d3dOutString(viewport -> PosX + 100,y0 + 16 + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_String,mchA_ColorF[2],230,2,0);
				mchA_DrawStar(viewport -> PosX + 85,y0 + 25 + acsY0,0.6f);
			}
			info_flag = 1;
			break;
		case MCH_TRACK_NOT_AVAILABLE:
			if(!mchSplitScreenGame || mchSplitScreenMode)
				mchA_d3dOutString(120,y0 + 10 + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],iGetText(iTXT_TRACK_NA1),mchA_ColorF[2],230,1,2);
			else
				mchA_d3dOutString(viewport -> PosX + 120,y0 + 17 + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],iGetText(iTXT_TRACK_NA2),mchA_ColorF[2],230,2,0);
			break;
		case MCH_TRACK_DISABLED:
			if(!mchSplitScreenGame || mchSplitScreenMode){
				sz = acsStrLen(1,(unsigned char*)iGetText(iTXT_COME_LATER),2);
				mchA_d3dOutString((640 - sz)/2,y0 + 10 + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],iGetText(iTXT_COME_LATER),mchA_ColorF[2],230,1,2);
			}
			else
				mchA_d3dOutString(viewport -> PosX + 100,y0 + 17 + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],iGetText(iTXT_COME_LATER),mchA_ColorF[2],230,2,0);
			break;
		case MCH_TRACK_DONE:
		case MCH_TRACK_ACCESSIBLE:
			id = mch_trackD -> TrackPart(nextWorld,nextTrack);
			if(id && !owner -> stPtr -> partFlags[id]){
				mchA_d3dOutString(viewport -> PosX + 5,y0 + 2 + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],iGetText(iTXT_PRIZE),mchA_ColorF[2],230,2,0);
				mchA_SprD -> DrawArcaneSymbol(viewport -> PosX + 45,y0 + acsY0,id,0.0f,0.0f,2,255);
			}
			name = mch_trackD -> TrackName(nextWorld,nextTrack);
			if(name){
				str < name;
#ifndef _FINAL_VERSION_
				str < " (" <= nextTrack < ")";
#endif
				if(!mchSplitScreenGame || mchSplitScreenMode)
					mchA_d3dOutString(170,y0 + 10 + acsY0,mchA_FontScaleX[1],mchA_FontScaleY[1],str.address(),mchA_ColorF[2],230,1,0);
				else
					mchA_d3dOutString(viewport -> PosX + 10,y0 + 10 + acsY0,mchA_FontScaleX[2],mchA_FontScaleY[2],str.address(),mchA_ColorF[2],230,2,1);
			}
			info_flag = 1;
			break;
	}
	if(!mchSplitScreenGame && info_flag){
		text = mch_trackD -> TrackText(nextWorld,nextTrack);
		if(text){
			sx = acsTextStrLenMax(2,(unsigned char*)text,0);
			sy = acsTextHeight(2,(unsigned char*)text,-2);

			x = viewport -> PosX + viewport -> SizeX - sx - 30;
			y = viewport -> PosY + 30;

			name = mch_trackD -> TrackName(nextWorld,nextTrack);
			if(name)
				sy += 20;

			mchA_DrawTextWindow(x - 10,y - 10,sx + 20,sy + 20,round(150.0f * curScreenPhase),2,round(200.0f * curScreenPhase));

			if(name){
				mchA_d3dOutString(x,y - 5,mchA_FontScaleX[2],mchA_FontScaleY[2],name,mchA_ColorF[2],round(230.0f * curScreenPhase),2,0,1.3f,1,-2);
				y += 20;
			}

			mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],text,mchA_ColorF[2],round(230.0f * curScreenPhase),2,0,1.0f,1,-2);
		}
	}
}

void iWorldDispatcher::draw_Teleport2Screen(void)
{
	int x,y,y0;
	aciScreenObject* p;

	y0 = GetScrY();

	if(CheckFlag(IW_EXIT_TELEPORT_FLAG))
		p = curScreen -> GetObject(3);
	else
		p = (CheckFlag(IW_TURNBASED_FLAG)) ? curScreen -> GetObject(2) : curScreen -> GetObject(1);

	if(p){
		x = p -> PosX - 15;
		y = p -> PosY + p -> SizeY/2 + acsY0 - 3;

		mchA_d3dOutSprite(x,y,0.7f,0.7f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],200,0.0f,1.0f);
	}
}

void iWorldDispatcher::InitTeleports(int mode)
{
	int status;
	iwTeleport* tp = tpLst.first();

	if(mode){
		while(tp){
			switch(mch_trackD -> TrackMode(tp -> World,tp -> Track)){
				case MCH_TRACK_NOT_AVAILABLE:
				case MCH_TRACK_DISABLED:
					status = 0;
					break;
				case MCH_TRACK_NEXT_LEVEL:
					status = 3;
					break;
				case MCH_TRACK_ACCESSIBLE:
					status = 2;
					break;
				case MCH_TRACK_DONE:
					status = 1;
					break;
				case MCH_TRACK_HIDDEN:
					status = 4;
					break;
			}

			fxlabChangeTeleport(tp -> World,tp -> Track,status);
			tp = tp -> next;
		}
	}
	else {
		while(tp){
			fxlabChangeTeleport(tp -> World,tp -> Track,0);
			tp = tp -> next;
		}
	}
}

void iWorldDispatcher::Move2LastPlace(int rev)
{
	iwPlace* p = plLst.search(lastPlace);
	if(!p) p = plLst.first();

	if(p)
		owner -> setPosition(Vect3f(p -> X,p -> Y,0),G2R(p -> angle + 90 + rev * 180));
}

void iWorldDispatcher::LastPosition(Vect3f& r,int& angle)
{
	iwPlace* p = plLst.search(lastPlace);
	if(!p) p = plLst.first();

	if(p){
		r = Vect3f(p -> X,p -> Y,0);
		angle = p -> angle + 90;
	}
}

void iWorldDispatcher::SetFigureAnimation(int id,int mode)
{
	cMesh* obj = figures[id];

	if(mode)
		gb_IVisGeneric -> SetObjectChannel((cUnknownClass*)obj,"exult");
	else
		gb_IVisGeneric -> SetObjectChannel((cUnknownClass*)obj,"main");

	if(mode == -1)
		gb_IVisGeneric -> SetObjectAnimation((cUnknownClass*)obj,0.0f);
	else
		gb_IVisGeneric -> SetObjectAnimation((cUnknownClass*)obj,2000.0f);
}

void iWorldDispatcher::Save(XStream& fh)
{
	int tb;
	iwDoggyPhrase* p;

	fh < lastPlace;

	tb = (flags & IW_TURNBASED_FLAG || mchTurnBasedGame) ? 1 : 0;

	fh < tb < tb;

	fh < iwDoggyPhrases.size();

	p = iwDoggyPhrases.first();
	while(p){
		fh < p -> ID < p -> Flags();
		p = p -> next;
	}
}

void iWorldDispatcher::Load(XBuffer& xb,int ver)
{
	int i,fl,v,sz;
	iwDoggyPhrase* p;

	xb > lastPlace;

	if(ver >= 104){
		xb > fl > v;

		acsSetTurnBased(0,fl);
	}
	if(ver >= 105){
		xb > sz;
		for(i = 0; i < sz; i ++){
			xb > v > fl;
			p = iwDoggyPhrases.search(v);

			if(p){ 
				p -> ClearFlags();
				p -> SetFlag(fl);
			}
		}
	}
}

void iWorldDispatcher::ElectionaryQuant(void)
{
	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(electionaryScrID);

	if(!scr) return;
/*
	p = (aciScreenInputField*)scr -> GetObject(1);
	if(p){
		if(flags & IW_FIRST_ELECTIONARY_FLAG)
			strcpy(p -> string,mch_playerNames[curFigure]);
		else
			strcpy(p -> string,owner -> name);
	}
*/
	p = (aciScreenInputField*)scr -> GetObject(2);
	if(p){
		if(owner -> flags & MCH_ACTIVE_RACER2){
			if(!camera_dispatcher2 -> isInterpolationCompleted())
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
			else
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		}
		else {
			if(!camera_dispatcher -> isInterpolationCompleted())
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
			else
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		}

	}
	p = (aciScreenInputField*)scr -> GetObject(3);
	if(p){
		if(owner -> flags & MCH_ACTIVE_RACER2){
			if(!camera_dispatcher2 -> isInterpolationCompleted())
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
			else
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		}
		else {
			if(!camera_dispatcher -> isInterpolationCompleted())
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
			else
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		}

	}
}

void iWorldDispatcher::BackupElectionary(void)
{
	figureBackup = owner -> stPtr -> essenceID;
}

void iWorldDispatcher::RestoreElectionary(void)
{
	if(owner -> stPtr -> essenceID != figureBackup)
		owner -> ChangePart(0,figureBackup);
}

void iWorldDispatcher::BackupAssembly(void)
{
	int i;
	XBuffer buf(owner -> stPtr -> mechos_config,30);

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		buf >= configBackup[i];
}

void iWorldDispatcher::RestoreAssembly(void)
{
	int i;
	for(i = 1; i < NUM_ACTIVE_PARTS; i ++)
		owner -> ChangePart(i,configBackup[i] - 1);
}

void iWorldDispatcher::SetOwner(mchRacer* p)
{ 
	owner = p;
	if(mchSplitScreenGame)
		viewport = (ID) ? iWnd1 : iWnd0;
	else
		viewport = iWnd;
}

void iwInitWorld(void)
{
	int i,sz[NUM_ACTIVE_PARTS - 1];
	float x,y,z,al;
	cMesh* obj,*m;

	iwTeleport* tp;

	const float r = 120.0f;

	if(iwElectionaryFlag){
		iworld_D -> SetFlag(IW_INIT_ELECTIONARY_FLAG);
		if(mchSplitScreenGame)
			iworld_D2 -> SetFlag(IW_INIT_ELECTIONARY_FLAG);

		iwElectionaryFlag = 0;
	}

	// Electionary

	if(!iwFigures) iwFigures = new cMesh*[acsMaxEssenceID];
	for(i = 0; i < acsMaxEssenceID; i ++){
		al = M_PI * 2.0f / float(acsMaxEssenceID) * float(i);

		x = iwElect_R.x + r * cos(al);
		y = iwElect_R.y + r * sin(al);
		z = iwElect_Z0;

		if(i || !mchOpenNewChar)
			obj = (cMesh*)gb_IVisGeneric -> CreateObject(i + 1,M3D_ENGINE);
		else
			obj = (cMesh*)gb_IVisGeneric -> CreateObject(601,M3D_iSCREEN);
/*
#ifndef _LOCAL_VERSION_
		if(i)
			obj = (cMesh*)gb_IVisGeneric -> CreateObject(i + 1,M3D_ENGINE);
		else
			obj = (cMesh*)gb_IVisGeneric -> CreateObject(601,M3D_iSCREEN);
#else
		obj = (cMesh*)gb_IVisGeneric -> CreateObject(i + 1,M3D_ENGINE);
#endif
*/
//		obj = (cMesh*)gb_IVisGeneric -> CreateObject(i + 1,M3D_ENGINE);

		z += (obj -> Real.Max[2] - obj -> Real.Min[2]) / 2.0f;

		gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)obj,&Vect3f(x,y,z),&Vect3f(0,0,al * 180.0f / M_PI + 90.0f));
		iwFigures[i] = obj;
	}

	// Assembly
	iwAssembly_MaxZ = iwAssembly_Z1;

	if(!iwArrowRot)
		iwArrowRot = new acsTimer(0.0f,360.0f,500.0f);
	if(!iwArrowPos)
		iwArrowPos = new acsTimer(0.0f,20.0f,20.0f);

	iwAssembly_Arrow = (cMesh*)gb_IVisGeneric -> CreateObject(503,M3D_iSCREEN);
	iwAssembly_Arrow -> SetScale(iwArrowSize);

	iwElectionary_Arrow = (cMesh*)gb_IVisGeneric -> CreateObject(503,M3D_iSCREEN);
	iwElectionary_Arrow -> SetScale(iwArrowSize);

	tp = iworld_D -> getFirstTeleport();

	while(tp){
		if(mch_trackD -> TrackMode(tp -> World,tp -> Track) != MCH_TRACK_NOT_AVAILABLE){
			tp -> arrow = (cMesh*)gb_IVisGeneric -> CreateObject(503,M3D_iSCREEN);
			tp -> arrow -> SetScale(iwArrowSize);
			tp -> arrow -> SetVisible(0);
		}
		else
			tp -> arrow = NULL;

		tp = tp -> next;
	}

	if(!iwParts[0]){
		for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++)
			sz[i] = iwPartSize[i] = 0;

		obj = MeshLibrary -> First();
		while(obj){
			for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
				if(obj -> Type & mchPartID[i] && M3D_KIND(obj -> Type) < 64 && (obj -> arcaneData.ID == -1 || !mch_excludeFlags[obj -> arcaneData.ID]))
					iwPartSize[i - 1] ++;
			}
			obj = MeshLibrary -> Next();
		}

		for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++){
			iwParts[i] = new cMesh*[iwPartSize[i]];
			iwPartsB[i] = new iwBody[iwPartSize[i]];
		}
	}
	else {
		for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++)
			sz[i] = 0;
	}

	obj = MeshLibrary -> First();
	while(obj){
		for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
			if(obj -> Type & mchPartID[i] && M3D_KIND(obj -> Type) < 64 && (obj -> arcaneData.ID == -1 || !mch_excludeFlags[obj -> arcaneData.ID])){
				m = (cMesh*)gb_IVisGeneric -> CreateObject(M3D_KIND(obj -> Type),M3D_TYPE(obj -> Type));
				m -> SetAttribute(MESH_NOT_LIGHTING);

				iwParts[i - 1][sz[i - 1]] = m;
				iwPartsB[i - 1][sz[i - 1]].init(m -> rmaxTotal());

				m -> SetVisible(0);
				sz[i - 1] ++;
			}
		}
		obj = MeshLibrary -> Next();
	}
}

void iwSelect(int n)
{
	if(n) 
		iworld_Cur = iworld_D2;
	else
		iworld_Cur = iworld_D;

	if(iworld_Cur -> CurScreen()){
		acsScrD -> curScrID = iworld_Cur -> CurScreen() -> ID;
		acsScrD -> curScr = iworld_Cur -> CurScreen();
	}
}

void iWorldDispatcher::PutKeys(KeyBuffer* p)
{
	if(!Disabled())
		p -> copy(keyBuf);
}

void iwKeyTrap(void)
{
	int k;
	iworld_D -> PutKeys(KeyBuf);
	if(mchSplitScreenGame)
		iworld_D2 -> PutKeys(KeyBuf);
	
	if(iworld_D -> ElectionaryMode() || iworld_D -> AssemblyMode()){
		while(KeyBuf -> size){
			k = KeyBuf -> get();
			mchA_CHQuant(k);
		}
	}

	if(mchSplitScreenGame){
		if((iworld_D2 -> ElectionaryMode() || iworld_D2 -> AssemblyMode()) && !(iworld_D -> ElectionaryMode() || iworld_D -> AssemblyMode()))
			KeyBuf -> clear();
	}
}

void iWorldDispatcher::KeyQuant(void)
{
	int k;

	ScaleScreen(curScreen);

	if(CheckFlag(IW_EXIT_FLAG))
		keyBuf -> clear();

	while(keyBuf -> size){
		k = keyBuf -> get();
		KeyTrap(k);
	}
}

int iWorldDispatcher::GetY0(void) const
{
	if(!mchSplitScreenGame || !mchSplitScreenMode || ID)
		return round((480.0f - float(IW_Y0)) * (1.0f - curScreenPhase));
	else
		return round(-float(IW_DY0) * (1.0f - curScreenPhase));
}

void iWorldDispatcher::ScaleScreen(aciScreen* scr)
{
	if(!scr)
		return;

	if(!mchSplitScreenGame)
		scr -> ScaleCoords(1.0f,1.0f);
	else {
		if(!mchSplitScreenMode){
			scr -> ScaleCoords(0.5f,1.0f);
			scr -> ChangeCoords(viewport -> PosX,0);
		}
		else {
			scr -> ScaleCoords(1.0f,1.0f);

			if(ID)
				scr -> ChangeCoords(0,viewport -> PosY - viewport -> SizeY);
			else
				scr -> ChangeCoords(0,viewport -> PosY - viewport -> SizeY * 2 + IW_DY0);
		}
	}
	
	aciScreenObject* p = (aciScreenObject*)scr -> objList -> fPtr;
	aciScreenInputField* fp;
	while(p){
		if(p -> type == ACS_INPUT_FIELD_OBJ && p -> align_x == 1){
			fp = (aciScreenInputField*)p; 
			fp -> SizeX = acsStrLen(fp -> font,(unsigned char*)fp -> string,fp -> Space);
			fp -> PosX = viewport -> PosX + viewport -> SizeX - fp -> SizeX - (640 - fp -> SizeX0 - fp -> PosX0);
		}
		p = (aciScreenObject*)p -> next;
	}
}

int iWorldDispatcher::GetScrY(void) const
{
	if(!mchSplitScreenGame || !mchSplitScreenMode || ID)
		return (viewport -> PosY + viewport -> SizeY - IW_DY0);
	else
		return (viewport -> PosY);
}

void iwDisable(int n)
{
	if(n)
		iworld_D2 -> Disable();
	else
		iworld_D -> Disable();
}

void iwEnable(int n)
{
	if(n)
		iworld_D2 -> Enable();
	else
		iworld_D -> Enable();
}

void iWorldDispatcher::draw_DisabledScreen(void)
{
	int x,y,sx,sy,fnt;
	if(mchSplitScreenGame && !mchSplitScreenMode)
		fnt = 1;
	else 
		fnt = 0;

	if(RenderMode != DIRECT3D_HICOLOR) return;

	mchA_DarkenRect(viewport -> PosX * mchA_d3dResX,viewport -> PosY * mchA_d3dResY,(viewport -> PosX + viewport -> SizeX) * mchA_d3dResX,(viewport -> PosY + viewport -> SizeY) * mchA_d3dResY,150);

	sx = acsStrLen(fnt,(unsigned char*)iwWaitStr,1);
	sy = acsStrHeight(fnt,(unsigned char*)iwWaitStr);

	x = viewport -> PosX + (viewport -> SizeX - sx) / 2;
	y = viewport -> PosY + (viewport -> SizeY - sy) / 2;

	mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],iwWaitStr,mchA_ColorF[2],230,fnt,1);
}

void iWorldDispatcher::InitFigures(void)
{
	int i,n,num = 0;
	float x,y,z,al;

	const float r = 120.0f;

	for(i = 0; i < acsMaxEssenceID; i ++){
		if(mch_trackD -> FigureEnabled(i)) num ++;
	}

	n = 0;
	for(i = 0; i < acsMaxEssenceID; i ++){
		if(mch_trackD -> FigureEnabled(i)){
			figures[i] -> SetVisible(1);
			al = M_PI * 2.0f / float(num) * float(n);

			x = iwElect_R.x + r * cos(al);
			y = iwElect_R.y + r * sin(al);
			z = iwElect_Z0;

			z += (figures[i] -> Real.Max[2] - figures[i] -> Real.Min[2]) / 2.0f;

			gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)figures[i],&Vect3f(x,y,z),&Vect3f(0,0,al * 180.0f / M_PI + 90.0f));
			n ++;

			SetFigureAnimation(i,0);
		}
		else 
			figures[i] -> SetVisible(0);
	}

	numActiveFigures = num;
}

iwDoggyPhrase::iwDoggyPhrase(void)
{
	ID = 0;
	flags = 0;
	priority = 0;

	message = NULL;
	list = NULL;
}

iwDoggyPhrase::~iwDoggyPhrase(void)
{
	if(message) 
		free(message);
}

void iwDoggyPhrase::SetMessage(char* msg)
{
	message = strdup(msg);
}

void iwDoggyPhraseQueue::Sort(void)
{
	int i,fl = 1;
	iwDoggyPhrase* p;

	while(fl){
		fl = 0;
		for(i = 0; i < length - 1; i ++){
			if(queue[i] -> Priority() > queue[i + 1] -> Priority()){
				p = queue[i];
				queue[i] = queue[i + 1];
				queue[i + 1] = p;

				fl = 1;
			}
		}
	}
}

void iwDoggyPhraseQueue::Put(iwDoggyPhrase* p)
{
	int i;
	if(length < IW_DOGGY_MAX_PHRASE){
		for(i = 0; i < length; i ++){
			if(queue[i] == p) return;
		}
		queue[length++] = p;
	}

	Sort();
}

void iwDoggyPhraseQueue::Remove(void)
{
	int i;
	if(length){
		for(i = 1; i < length; i ++)
			queue[i - 1] = queue[i];

		length --;
	}
}

int iwDoggyPhraseQueue::NewMessages(void) const
{
	int i;

	for(i = 0; i < length; i ++){
		if(!queue[i] -> CheckFlag(IW_DOGGY_READ_FLAG)) 
			return 1;
		if(!queue[i] -> CheckFlag(IW_DOGGY_ONE_TIME_FLAG)) 
			return 0;
	}

	return 0;
}

iwDoggyPhrase* iwDoggyPhraseQueue::Get(void) 
{ 
	iwDoggyPhrase* p = NULL;

	if(length){
		p = queue[0];
		if(p -> CheckFlag(IW_DOGGY_ONE_TIME_FLAG))
			Remove();
	}

	return p;
}

void iwDoggyPhrase::Init(scrDataBlock* p)
{
	scrDataBlock* p1 = p -> nextLevel -> first();

	ID = *p -> i_dataPtr;

	while(p1){
		switch(p1 -> ID){
			case ASCR_STRING:
				SetMessage(p1 -> c_dataPtr);
				break;
			case IW_DOGGY_PHRASE_PRIORITY:
				SetPriority(*p1 -> i_dataPtr);
				break;
			case ASCR_FLAG:
				SetFlag(*p1 -> i_dataPtr);
				break;
		}
		p1 = p1 -> next;
	}
}

void iwInitDoggy(void)
{
	XStream fh;

	iwDoggyPhrase* pr;
	scrDataBlock* root,*p;

	acsOpenResource("doggy.scb",fh);
	root = loadScript(fh);
	p = root -> nextLevel -> first();

	while(p){
		if(p -> ID == IW_DOGGY_PHRASE){
			pr = new iwDoggyPhrase;
			pr -> Init(p);

			iwDoggyPhrases.append(pr);
		}
		p = p -> next;
	}
}

void iWorldDispatcher::InitDoggy(int get_mode)
{
	if(mode == IW_ADVISOR) return;

	mode = IW_ADVISOR;

	if(get_mode){
		if(!mchSplitScreenGame)
			curPhrase = phrases.Get();
		else
			curPhrase = NULL;
	}

	if(!curPhrase)
		curPhrase = iwDoggyPhrases.search(IW_DOGGY_INFO0 + XRnd(5));

	set_iScreen(IW_DOGGY_SCR);
}

void iWorldDispatcher::FinitDoggy(void)
{
	if(mode != IW_ADVISOR) return;

	mode = IW_FREE;

	set_iScreen(-1);

	DropFlag(IW_DOGGY_MODE_FLAG);
}

void iWorldDispatcher::draw_AdvisorScreen(void)
{
	int i,idx,sz,x,y,al;
	char* msg;
	static char str[256];

	aciScreenInputField* p;

	const float sc = 0.9f;

	if(curPhrase){
		i = 0;
		y = acsY0 + GetScrY();
		msg = curPhrase -> Message();
		sz = strlen(msg);

		while(i < sz){
			idx = 0;
			memset(str,0,256);
			while(msg[i] != '\n'){
				if(msg[i] == '%'){
					switch(msg[i + 1]){
						case 'n':
						case 'N':
							if(owner -> name){
								strcpy(str + idx,owner -> name);
								idx += strlen(owner -> name);
							}
							break;
					}
					if(++i >= sz) break;
				}
				else
					str[idx++] = msg[i];
				if(++i >= sz) break;
			}
			x = (viewport -> SizeX - round(float(acsStrLen(2,(unsigned char*)str,0)) * sc)) / 2;
			mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],str,mchA_ColorF[2],230,2,0,sc);

			if(++i >= sz) break;
			y += 14;
		}

		curPhrase -> SetFlag(IW_DOGGY_READ_FLAG);
	}

	p = (aciScreenInputField*)curScreen -> GetObject(10);
	if(p){
		if(phrases.NewMessages()){
			al = p -> AlphaCur + 127;

			x = p -> PosX + p -> SizeX/2;
			y = p -> PosY + p -> SizeY/2 + 5;

			mchA_d3dOutSprite(x,y + acsY0,1.0f,1.0f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,0.0f,1.0f);
		}
		else {
			al = p -> AlphaCur + 127;

			x = p -> PosX;
			y = p -> PosY;

			mchA_d3dOutString(x,y + acsY0,mchA_FontScaleX[0],mchA_FontScaleY[0],iGetText(iTXT_OK),mchA_ColorF[2],al,0,0);
		}
	}
}

void iWorldDispatcher::PutPhrase(int id)
{
	iwDoggyPhrase* p = iwDoggyPhrases.search(id);

	if(p)
		phrases.Put(p);
}

void iWorldDispatcher::ClearPhrases(void)
{
	phrases.Clear();
}

void iWorldDispatcher::ResetPhrases(void)
{
	iwDoggyPhrase* p = iwDoggyPhrases.first();
	while(p){
		p -> DropFlag(IW_DOGGY_READ_FLAG);
		p = p -> next;
	}
	ClearPhrases();
}

void iWorldDispatcher::ShowNewPhrase(void)
{
	Vect3f r,r0,dr_;
	float scale;
	int x,y,z;

	const float sc = 0.3f;
	const float dr = 5.0f;
	const float dz = 40.0f;

	Body* p = Mdisp -> searchBody(Vect3f::ZERO,Body::DOG);

	if(!p) return;

//	real pos[3] = { 2046, 1495, 220 };
//	p -> SetPosition(pos);

	if(mchSplitScreenGame || !phrases.NewMessages()) return;

	r.x = p -> R().x + dr * sin(p -> psi());
	r.y = p -> R().y + dr * cos(p -> psi());
	r.z = p -> R().z + dz;

	scale = camera_dispatcher -> getScale(r) * sc;

	z = mchA_ConvertCoords(r.xi(),r.yi(),r.zi(),x,y);

	r0 = owner -> R();

	r.z = 0.0f;
	r0.z = 0.0f;

	dr_ = getDist(r,r0);

	if(dr_.norm() < 800.0f)
		mchA_SprD -> DrawSpriteZ(x,y,z,scale,scale,300,mchA_ColorF[7],200,0.0f,1);
}

void iWorldDispatcher::Clear(void)
{
	curMovie = curMovieSize = 0;
	mode = IW_FREE;
	curScreen = NULL;
	activeInput = NULL;

	curPartType = 0;
	curPart = 0;

	lastPlace = 100;

	curScreenPhase = destScreenPhase = 1.0f;

	ResetPhrases();
}

void iWorldDispatcher::MovieQuant(void)
{
	int boss_flag = 0,snd_flags = 1 << 2;

	iwHideArrows = 0;

/*	if(iwElectionaryFlag && iwEnableIntroMovie){
		SetMovie(iwBeginMovie);
		HideObjects(-1,0);
	}*/

	mchTrackInfo* p = mch_trackD -> FirstTrack();
	while(p){
		if(p -> Boss()){
			if(p -> Access()){
				if(p -> Status()){
					if(!p -> PassStatus()){
						SetMovie(p -> CloseMovie());
						p -> SetPassStatus();
						boss_flag = 1;
					}
				}
				HideObjects(p -> BossID() - 10,1);
				if(p -> BossSoundID() != -1){
					if(snd_flags & (1 << p -> BossSoundID()))
						mch_sndD -> DisableTimer(iwBossSounds[p -> BossSoundID()]);
					else
						snd_flags |= (1 << p -> BossSoundID());
				}
				fxlabBigBossWorld(p -> World(),0);
			}
			else {
				mch_trackD -> TrackMode(p);
				if(p -> Access()){
					SetMovie(p -> OpenMovie());
					HideObjects(p -> BossID() - 10,1);

					if(p -> BossSoundID() != -1){
						if(snd_flags & (1 << p -> BossSoundID()))
							mch_sndD -> DisableTimer(iwBossSounds[p -> BossSoundID()]);
						else
							snd_flags |= (1 << p -> BossSoundID());
					}
				}
				else
					fxlabBigBossWorld(p -> World(),1);
			}
		}
		p = p -> next;
	}

	cdCheck("Resource\\Mountain0\\output.vmp","Resource\\Water2\\output.vmp");

	if(iwForceEndMovie || mch_trackD -> AllBossesDefeated()){
		if(iwForceEndMovie || boss_flag){
			SetMovie(iwEndMovie);
			SetMovie(iwOutroMovie);

			iwHideArrows = 1;
		}

		HideObjects(5,1);
		fxlabBigBossStatus(0);

		iwForceEndMovie = 0;
	}
	else
		fxlabBigBossStatus(1);
}

void iWorldDispatcher::SetMovie(char* m)
{
	if(curMovieSize < IW_MAX_MOVIE)
		movies[curMovieSize ++] = m;

	iwHideArrows = 1;
}

void iWorldDispatcher::HideObjects(int id,int del_mode)
{
	int i;
	cUnknownClass* UObject;

	if(id == -1){
		for(i = 0; i < IW_NUM_OBJECTS_TO_HIDE; i ++){
			UObject = gb_IVisGeneric -> FindObjectByFileName(iwBoss3DS_Names[i]);
			if(UObject){
				if(!del_mode)
					gb_IVisGeneric -> SetObjectVisible(UObject,0);
				else
					gb_IVisGeneric -> ReleaseObject(UObject);
			}
		}
	}
	else {
		UObject = gb_IVisGeneric -> FindObjectByFileName(iwBoss3DS_Names[id]);
		if(UObject){
			if(!del_mode)
				gb_IVisGeneric -> SetObjectVisible(UObject,0);
			else
				gb_IVisGeneric -> ReleaseObject(UObject);
		}
	}
}

void iWorldDispatcher::ShowObjects(int id)
{
	int i;
	cUnknownClass* UObject;

	if(id == -1){
		for(i = 0; i < IW_NUM_OBJECTS_TO_HIDE; i ++){
			UObject = gb_IVisGeneric -> FindObjectByFileName(iwBoss3DS_Names[i]);
			if(UObject)
				gb_IVisGeneric -> SetObjectVisible(UObject,1);
		}
	}
	else {
		UObject = gb_IVisGeneric -> FindObjectByFileName(iwBoss3DS_Names[id]);
		if(UObject)
			gb_IVisGeneric -> SetObjectVisible(UObject,1);
	}
}

void iWorldDispatcher::HideAssembly(void)
{
	AssemblyPartsQuant(0);
}

void iWorldDispatcher::ShowAssembly(void)
{
	AssemblyPartsQuant(1);
}

void iWorldDispatcher::HideElectionary(void)
{
	int i;
	for(i = 0; i < acsMaxEssenceID; i ++)
		figures[i] -> SetVisible(0);
}

void iWorldDispatcher::ShowElectionary(void)
{
	int i;
	for(i = 0; i < acsMaxEssenceID; i ++){
		if(mch_trackD -> FigureEnabled(i))
			figures[i] -> SetVisible(1);
	}
}

void iWorldDispatcher::prepareTeleport2Screen(void)
{
	aciScreenInputField* p_rt,*p_tb,*p_b;
	if(!curScreen || curScreen -> ID != IW_TELEPORT2_SCR) return;

	p_rt = (aciScreenInputField*)curScreen -> GetObject(1);
	p_tb = (aciScreenInputField*)curScreen -> GetObject(2);
	p_b = (aciScreenInputField*)curScreen -> GetObject(3);

	p_b -> Color = p_b -> Color0 = p_b -> Color1 = p_b -> Color2 = 1;
	p_tb -> Color = p_tb -> Color0 = p_tb -> Color1 = p_tb -> Color2 = 1;
	p_rt -> Color = p_rt -> Color0 = p_rt -> Color1 = p_rt -> Color2 = 1;

	if(CheckFlag(IW_EXIT_TELEPORT_FLAG)){
		p_b -> Color = p_b -> Color0 = p_b -> Color1 = p_b -> Color2 = 2;
	}
	else {
		if(CheckFlag(IW_TURNBASED_FLAG))
			p_tb -> Color = p_tb -> Color0 = p_tb -> Color1 = p_tb -> Color2 = 2;
		else
			p_rt -> Color = p_rt -> Color0 = p_rt -> Color1 = p_rt -> Color2 = 2;
	}
}

void iWorldDispatcher::AssemblyCoordsQuant(void)
{
	int i,j;

	float x,y,z,al;
	cMesh* m;

	const float r1 = 80.0f;

	for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++){
		for(j = 0; j < iwPartSize[i]; j ++){
			iwPartsB[i][j].quant(iwAssembly_dT);

			m = iwParts[i][j];

			al = M_PI * 2.0f / float(iwPartSize[i]) * float(j);

			x = iwAssembly_R.x + r1 * cos(al);
			y = iwAssembly_R.y + r1 * sin(al);
			z = iwPartsB[i][j].X();

			gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)m,&Vect3f(x,y,z),&Vect3f(0,0,al * 180.0f / M_PI - 90.0f));

			if(i == curPartType && fabs(iwPartsB[i][j].X() - iwAssembly_Z0) < 10.0f)
				fxlabPartWave(x,y);
		}
	}

	fxlabPartWaveQuant();
}

iwBody::iwBody(void)
{
	reset();
	init(1.0f);
}

iwBody::~iwBody(void)
{
}

void iwBody::quant(float dt)
{
	float f;
	f = calc_force();

	v += f * dt;
	v *= 1.0f - iwAssembly_Diss;
	x += v * dt;
	if(x > iwAssembly_MaxZ){ 
		x = iwAssembly_MaxZ;
		v = 0.0f;
	}
}

float iwBody::calc_force(void)
{
	float f;

	float xx,x_min,x_max;

	xx = iwAssembly_Z0;
	x_min = x - dx/2.0f;
	x_max = x + dx/2.0f;

	if(xx < x_min) xx = x_min;
	if(xx > x_max) xx = x_max;

	f = -iwAssembly_G + iwAssembly_A * dx * (xx - x_min) / (x_max - x_min) - iwAssembly_RC * v * v;

	return f;
}

void iwBody::init(float _dx) 
{ 
	dx = _dx; 
}

iwTeleport* iWorldDispatcher::getTeleport(int wr,int tr)
{
	iwTeleport* p = tpLst.first();
	while(p){
		if(p -> World == wr && p -> Track == tr)
			return p;

		p = p -> next;
	}

	return NULL;
}

void iwJoyKeyTrap(int k)
{
	int code,cfg;
	if(iworld_D -> isScreenSelected() && !iworld_D -> Disabled() && acsJoyUse[mchSplitScreenGame]){
		cfg = mchSplitScreenGame;

		if(iworld_D -> isInInputMode()){
			if(k == MCH_KEY_OK) code = VK_RETURN;
			if(k == MCH_KEY_CANCEL) code = VK_ESCAPE;
			if(k == MCH_KEY_TURN_LEFT) code = VK_BACK;
		}
		else 
			code = mchGetKey(k,cfg);

		KeyBuf -> put(code,KBD_CUR_KEY_PRESSED);
	}

	if(mchSplitScreenGame && iworld_D2 -> isScreenSelected() && !iworld_D2 -> Disabled() && acsJoyUse[2]){
		cfg = 2;

		if(iworld_D2 -> isInInputMode()){
			if(k == MCH_KEY_OK) code = VK_RETURN;
			if(k == MCH_KEY_CANCEL) code = VK_ESCAPE;
			if(k == MCH_KEY_TURN_LEFT) code = VK_BACK;
		}
		else 
			code = mchGetKey(k,cfg);

		KeyBuf -> put(code,KBD_CUR_KEY_PRESSED);
	}
}

void iWorldDispatcher::SetMovie(int op_mode)
{
	mchTrackInfo* p = mch_trackD -> FirstTrack();
	while(p){
		if(p -> Boss() && p -> World() == nextWorld){
			if(op_mode){
				SetMovie(p -> OpenMovie());
				HideObjects(p -> BossID() - 10,1);
			}
			else {
				SetMovie(p -> CloseMovie());
			}
		}
		p = p -> next;
	}
}

void iWorldDispatcher::SetEndMovie(void)
{
	SetMovie(iwEndMovie);
	SetMovie(iwOutroMovie);

	iwHideArrows = 1;

	HideObjects(5,1);
	fxlabBigBossStatus(0);
}
