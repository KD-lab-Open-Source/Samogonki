/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include <string.h>

#include "wininet_api.h"
#include "online_game.h"

#include "Xreal_utl.h"

#include "iText.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

//#define _ONLINE_LOG_

#ifdef _ONLINE_LOG_
char* og_logName = "LOGS\\online.log";
int og_Log = 0;
XStream og_logFh;
#endif

ogPacket og_inP;
ogPacket og_inP2;
ogPacket og_outP;

char ogStatusStr[256];

int ogRefreshTimer = 0;

const char* ogErrorStr = NULL;
const char* ogErrorStr0 = "ERROR:";

const int OG_PACKET_VERSION = 104;

void ogQuant(void)
{
	int st;
	char* status_str = ogStatusStr;

#ifdef _ONLINE_LOG_
	char* log_name = og_logName;
#endif

	st = wi_D.get_status();
	wi_D.quant();
	CONTROL_FP();

	wiCriticalSection sec(wiCritical);

	static int cnt = 0;

	switch(wi_D.get_status()){
		case WI_ERROR:
			sprintf(status_str,"Error");
			break;
		case WI_IDLE:
			sprintf(status_str,"Idle");
			break;
		case WI_FINISHED:
			CONTROL_FP();
			if(st == WI_GET || st == WI_LOAD_FILE){
#ifdef _ONLINE_LOG_
				if(!og_Log){
					og_logFh.open(log_name,XS_OUT);
					og_Log = 1;
				}
				else
					og_logFh.open(log_name,XS_OUT | XS_APPEND | XS_NOREPLACE);

				og_logFh < "[in]  ";
				og_logFh < wi_D.input_buffer() < "\r\n";
				og_logFh.close();
#endif
				if(og_inP.decode(wi_D.input_buffer(),strlen(wi_D.input_buffer()))){
					og_inP.dump();
					hsOnlineSetup();
				}
				else 
					wi_D.set_error();
			}
			if(st == WI_POST){
#ifdef _ONLINE_LOG_
				if(!og_Log){
					og_logFh.open(log_name,XS_OUT);
					og_Log = 1;
				}
				else
					og_logFh.open(log_name,XS_OUT | XS_APPEND | XS_NOREPLACE);

				og_logFh < "[out] ";
				og_logFh < wi_D.output_buffer() < "\r\n";

				og_logFh < "[in]  ";
				og_logFh < wi_D.input_buffer() < "\r\n";
				og_logFh.close();
#endif
				ogSetRefreshTime();
			}
			break;
		case WI_GET:
			if(wi_D.get_request_status())
				sprintf(status_str,"Downloading...");
			else
				sprintf(status_str,"Status: %s",wi_D.get_request_status_str());
			break;
		case WI_POST:
			if(wi_D.get_request_status())
				sprintf(status_str,"Sending data...");
			else
				sprintf(status_str,"Status: %s",wi_D.get_request_status_str());
			break;
	}
}

void ogSetRefreshTime(void)
{
	ogRefreshTimer = clocki();
}

int ogCheckRefreshTime(void)
{
	if(clocki() - ogRefreshTimer >= 30000)
		return 1;

	return 0;
}

void ogLogError(const char* text,int uid,int turn,int prm0,int prm1)
{
#ifdef _ONLINE_LOG_
	char* log_name = og_logName;
	char str[256];

	if(!og_Log){
		og_logFh.open(log_name,XS_OUT);
		og_Log = 1;
	}
	else
		og_logFh.open(log_name,XS_OUT | XS_APPEND | XS_NOREPLACE);

	sprintf(str,text,turn,uid,prm0,prm1);

	og_logFh < "[err] ";
	og_logFh < str < "\r\n";
	og_logFh.close();
#endif
}

void ogLogStr(const char* text)
{
#ifdef _ONLINE_LOG_
	char* log_name = og_logName;

	if(!og_Log){
		og_logFh.open(log_name,XS_OUT);
		og_Log = 1;
	}
	else
		og_logFh.open(log_name,XS_OUT | XS_APPEND | XS_NOREPLACE);

	og_logFh < text;
	og_logFh.close();
#endif
}

char* ogStatus(void)
{
	return ogStatusStr;
}

ogPacket::ogPacket(void)
{
	Version = OG_PACKET_VERSION;
	TType = 0;
	ID = 0;
	*LID = 0;
	OWNER_UID = 0;
	*PASSWORD = 0;

	KD_WORLD_ID = KD_ROUTE_ID = 0;
	GAME_RND = 0;
	GTYPE = 'A';
	LAPS = SEEDS = DURATION = MOVE_CNT = STEPS_CNT = PLAYERS_CNT = 0;

	strcpy(START_STRING,"KDLAB");
	strcpy(STOP_STRING,"BITRIX");

	URL_POST_SZ = URL_POST_PATH_SZ = URL_RETURN_SZ = 0;
	URL_POST = URL_POST_PATH = URL_RETURN = NULL;

	turns = 0;
}

ogPacket::~ogPacket(void)
{
	if(URL_POST) delete URL_POST;
	if(URL_POST_PATH) delete URL_POST_PATH;
	if(URL_RETURN) delete URL_RETURN;
	free_turns();
}

void ogPacket::alloc_post_url(int sz)
{
	if(sz > URL_POST_SZ){
		if(URL_POST) delete URL_POST;
		URL_POST = new char[sz];
		URL_POST_SZ = sz;
	}
}

void ogPacket::alloc_post_path_url(int sz)
{
	if(sz > URL_POST_PATH_SZ){
		if(URL_POST_PATH) delete URL_POST_PATH;
		URL_POST_PATH = new char[sz];
		URL_POST_PATH_SZ = sz;
	}
}

void ogPacket::alloc_return_url(int sz)
{
	if(sz > URL_RETURN_SZ){
		if(URL_RETURN) delete URL_RETURN;
		URL_RETURN = new char[sz];
		URL_RETURN_SZ = sz;
	}
}

ogPlayerTurnInfo* ogPacket::get_player(int turn_num,int pl_num)
{
	int i,uID;
	ogTurnInfo* tp = get_turn_by_num(turn_num);
	if(!tp) return NULL;

	uID = players[pl_num].UID;

	for(i = 0; i < tp -> USERS_CNT; tp ++){
		if(tp -> turns[i] -> UID == uID)
			return tp -> turns[i];
	}

	return NULL;
}

void ogPacket::copy_header(ogPacket& p)
{
	p.Version = Version;
	p.TType = TType;
	p.ID = ID;

	strcpy(p.LID,LID);
	p.GAME_OWNER_UID = GAME_OWNER_UID;
	p.OWNER_UID = OWNER_UID;
	strcpy(p.PASSWORD,PASSWORD);

	p.KD_WORLD_ID = KD_WORLD_ID;
	p.KD_ROUTE_ID = KD_ROUTE_ID;

	p.GAME_RND = GAME_RND;
	p.GTYPE = GTYPE;

	p.LAPS = LAPS;
	p.SEEDS = SEEDS;

	p.DURATION = DURATION;
	p.MOVE_CNT = MOVE_CNT;
	p.PLAYERS_CNT = PLAYERS_CNT;
	p.STEPS_CNT = 0;

	p.EXPRESS = EXPRESS;
/*
	p.alloc_post_url(URL_POST_SZ);
	strcpy(p.URL_POST,URL_POST);

	p.alloc_return_url(URL_RETURN_SZ);
	strcpy(p.URL_RETURN,URL_RETURN);
*/
	memcpy(p.players,players,sizeof(ogPlayerInfo) * 5);
}

void ogPacket::alloc_turns(int num)
{
	int i;

	if(num) STEPS_CNT = num;

	if(STEPS_CNT){
		turns = new ogTurnInfo*[STEPS_CNT];
		for(i = 0; i < STEPS_CNT; i ++)
			turns[i] = new ogTurnInfo;
	}
}

void ogPacket::free_turns(void)
{
	int i;
	if(STEPS_CNT){
		for(i = 0; i < STEPS_CNT; i ++)
			delete turns[i];
		delete turns;
	}
	STEPS_CNT = 0;
	turns = 0;
}

int ogPacket::decode(void* p,int sz)
{
	ogBuffer buf(p,sz);

	return decode(buf);
}

int ogPacket::decode(ogBuffer& buf)
{
	int i,len;
	char str[WI_STRINGS_LEN];

	free_turns();

	i = buf.token_length();

	if((unsigned)i != strlen(START_STRING)) return 0;

	buf > str;
	if(strcmp(str,START_STRING)) return 0;
	
	buf > Version > TType > ID > LID > GAME_OWNER_UID > OWNER_UID > 
	PASSWORD > KD_WORLD_ID > KD_ROUTE_ID > GAME_RND > GTYPE > LAPS >
	SEEDS >	DURATION > MOVE_CNT > PLAYERS_CNT > STEPS_CNT > EXPRESS;

	if(Version != OG_PACKET_VERSION)
		ErrH.Abort(iGetText(iTXT_BAD_PACKET));

	i = buf.token_length();
	if(i){
		alloc_post_url(i + 1);
		buf > URL_POST;
	}
	else
		buf.read_skip();

	i = buf.token_length();
	if(i)
		buf > URL_POST_PORT;
	else
		buf.read_skip();

	i = buf.token_length();
	if(i){
		alloc_post_path_url(i + 1);
		buf > URL_POST_PATH;
	}
	else
		buf.read_skip();

	i = buf.token_length();
	if(i){
		alloc_return_url(i + 1);
		buf > URL_RETURN;
	}
	else
		buf.read_skip();

	if(TType != OG_SEEDS_PACKET && TType != OG_REFRESH_PACKET && TType != OG_REFRESH_ANSWER_PACKET){
		for(i = 0; i < PLAYERS_CNT; i ++){
			buf > players[i].UID > players[i].NIC > 
			players[i].PERS_CAR_COMP_ID > players[i].FRONT_CAR_COMP_ID > 
			players[i].FWHEEL_CAR_COMP_ID > players[i].BWHEEL_CAR_COMP_ID > 
			players[i].ROBOT;
		}
	}

	alloc_turns();

	for(i = 0; i < STEPS_CNT; i ++){
		len = strlen(buf.address()) - buf.tell();
		if((unsigned)len >= strlen(STOP_STRING) && !strncmp(buf.address() + buf.tell(),STOP_STRING,strlen(STOP_STRING)))
			break;

		turns[i] -> decode(buf);
	}

	i = buf.token_length();
	if((unsigned)i > strlen(STOP_STRING) + 1) return 0;

	buf > str;
	if(strcmp(str,STOP_STRING)) return 0;

	return 1;
}

#define ogLOG_STR(msg,v) { sprintf(str,msg,v); ogLogStr(str); }

void ogPacket::dump(void)
{
#ifdef _DUMP_PACKETS_
	int i;
	char str[256];

	ogLogStr("--------------------------------\r\n");

	ogLOG_STR("%s\r\n",START_STRING);
	ogLOG_STR("Version: %d\r\n",Version);

	ogLOG_STR("TType: %d\r\n",TType);

	ogLOG_STR("ID: %d\r\n",ID);

	ogLOG_STR("LID: %s\r\n",LID);
	ogLOG_STR("GAME_OWNER_UID: %d\r\n",GAME_OWNER_UID);
	ogLOG_STR("OWNER_UID: %d\r\n",OWNER_UID);

	ogLOG_STR("PASSWORD: %s\r\n",PASSWORD);

	ogLOG_STR("\r\nKD_WORLD_ID: %d\r\n",KD_WORLD_ID);
	ogLOG_STR("KD_ROUTE_ID: %d\r\n",KD_ROUTE_ID);

	ogLOG_STR("GAME_RND: %d\r\n",GAME_RND);
	ogLOG_STR("GTYPE: %c\r\n",GTYPE);

	ogLOG_STR("LAPS: %d\r\n",LAPS);
	ogLOG_STR("SEEDS: %d\r\n",SEEDS);

	ogLOG_STR("DURATION: %d\r\n",DURATION);
	ogLOG_STR("MOVE_CNT: %d\r\n",MOVE_CNT);
	ogLOG_STR("PLAYERS_CNT: %d\r\n",PLAYERS_CNT);
	ogLOG_STR("STEPS_CNT: %d\r\n",STEPS_CNT);

	ogLOG_STR("EXPRESS: %c\r\n",EXPRESS);

	ogLOG_STR("\r\nURL_POST: %s\r\n",URL_POST);
	ogLOG_STR("URL_POST_PORT: %d\r\n",URL_POST_PORT);
	ogLOG_STR("URL_POST_PATH: %s\r\n",URL_POST_PATH);

	ogLOG_STR("URL_RETURN: %s\r\n",URL_RETURN);

	ogLogStr("\r\nPlayers:\r\n");

	for(i = 0; i < PLAYERS_CNT; i ++)
		players[i].dump(str);

	ogLogStr("\r\nTurns:\r\n");

	for(i = 0; i < STEPS_CNT; i ++)
		turns[i] -> dump(str);

	ogLOG_STR("\r\n%s\r\n",STOP_STRING);

	ogLogStr("--------------------------------\r\n");
#endif
}

void ogPlayerInfo::dump(char* str)
{
	ogLOG_STR("\r\nUID: %d\r\n",UID);
	ogLOG_STR("NICK: %s\r\n",NIC);

	sprintf(str,"Mechos: %d %d %d %d\r\n",PERS_CAR_COMP_ID,FRONT_CAR_COMP_ID,FWHEEL_CAR_COMP_ID,BWHEEL_CAR_COMP_ID);
	ogLogStr(str);

	ogLOG_STR("ROBOT: %c\r\n",ROBOT);
}

void ogPlayerTurnInfo::dump(char* str)
{
	int i;
	ogLOG_STR("\r\nUID: %d\r\n",UID);
	ogLOG_STR("IS_FINISHED: %c\r\n",IS_FINISHED);
	ogLOG_STR("RANK: %d\r\n",RANK);
	ogLOG_STR("MOVE_TIME: %d\r\n",MOVE_TIME);
	ogLOG_STR("MOVE_STEPS: %d\r\n",MOVE_STEPS);
	ogLOG_STR("BOTTLES_CNT: %d\r\n",BOTTLES_CNT);
	ogLOG_STR("TOTAL_SEEDS_CNT: %d\r\n",TOTAL_SEEDS_CNT);
	ogLOG_STR("ARCANES_CNT: %d\r\n",ARCANES_CNT);
	ogLOG_STR("DESTROYS_CNT: %d\r\n",DESTROYS_CNT);
	ogLOG_STR("USER_SEEDS_CNT: %d\r\n",USER_SEEDS_CNT);

	if(USER_SEEDS_CNT){
		for(i = 0; i < USER_SEEDS_CNT; i ++){
			sprintf(str,"%d %d %d %d\r\n",seeds[i].X,seeds[i].Y,seeds[i].Z,seeds[i].ArcaneID);
			ogLogStr(str);
		}
	}
}

void ogTurnInfo::dump(char* str)
{
	int i;

	ogLOG_STR("\r\nSTEP_ID: %d\r\n",STEP_ID);
	ogLOG_STR("USERS_CNT: %d\r\n",USERS_CNT);

	if(USERS_CNT){
		ogLogStr("\r\nPlayer turns:\r\n");
		
		for(i = 0; i < USERS_CNT; i ++)
			turns[i] -> dump(str);
	}

	ogLogStr("------------\r\n");
}

void ogPacket::encode(ogBuffer& buf)
{
	int i;

	buf < START_STRING < Version < TType < ID < LID < GAME_OWNER_UID < OWNER_UID < 
	PASSWORD < KD_WORLD_ID < KD_ROUTE_ID < GAME_RND < GTYPE < LAPS <
	SEEDS <	DURATION < MOVE_CNT < PLAYERS_CNT < STEPS_CNT < EXPRESS;

	if(URL_POST) buf < URL_POST;
	else buf.write_skip();

	buf < URL_POST_PORT;

	if(URL_POST_PATH) buf < URL_POST_PATH;
	else buf.write_skip();

	if(URL_RETURN) buf < URL_RETURN;
	else buf.write_skip();

	if(TType != OG_SEEDS_PACKET && TType != OG_REFRESH_PACKET && TType != OG_REFRESH_ANSWER_PACKET){
		for(i = 0; i < PLAYERS_CNT; i ++){
			buf < players[i].UID < players[i].NIC < 
			players[i].PERS_CAR_COMP_ID < players[i].FRONT_CAR_COMP_ID < 
			players[i].FWHEEL_CAR_COMP_ID < players[i].BWHEEL_CAR_COMP_ID < 
			players[i].ROBOT;
		}
	}

	for(i = 0; i < STEPS_CNT; i ++)
		turns[i] -> encode(buf);

	buf < STOP_STRING;
}

void ogPlayerTurnInfo::decode(ogBuffer& buf)
{
	int i;

	free_seeds();
	buf > UID > IS_FINISHED > RANK > MOVE_TIME > MOVE_STEPS > BOTTLES_CNT > TOTAL_SEEDS_CNT > ARCANES_CNT > DESTROYS_CNT > USER_SEEDS_CNT;
	
	alloc_seeds();

	if(USER_SEEDS_CNT){
		for(i = 0; i < USER_SEEDS_CNT; i ++)
			seeds[i].decode(buf,(i == USER_SEEDS_CNT - 1));
	}
	else
		buf.read_skip();
}

void ogPlayerTurnInfo::encode(ogBuffer& buf)
{
	int i;
	buf < UID < IS_FINISHED < RANK < MOVE_TIME < MOVE_STEPS < BOTTLES_CNT < TOTAL_SEEDS_CNT < ARCANES_CNT < DESTROYS_CNT < USER_SEEDS_CNT;

	if(USER_SEEDS_CNT){
		for(i = 0; i < USER_SEEDS_CNT; i ++)
			seeds[i].encode(buf,(i == USER_SEEDS_CNT - 1));
	}
	else 
		buf.write_skip();
}

void ogPlayerTurnInfo::alloc_seeds(int num)
{
	if(num) USER_SEEDS_CNT = num;

	if(USER_SEEDS_CNT)
		seeds = new ogSeedInfo[USER_SEEDS_CNT];
}

void ogPlayerTurnInfo::free_seeds(void)
{
	if(seeds) delete seeds;
	USER_SEEDS_CNT = 0;
	seeds = 0;
}

void ogTurnInfo::decode(ogBuffer& buf)
{
	int i;

	free_turns();
	buf > STEP_ID > USERS_CNT;
	alloc_turns();

	if(USERS_CNT){
		for(i = 0; i < USERS_CNT; i ++)
			turns[i] -> decode(buf);
	}
/*
	else {
		for(i = 0; i < 4; i ++)
			buf.read_skip();
	}
*/
}

void ogTurnInfo::encode(ogBuffer& buf)
{
	int i;
	buf < STEP_ID < USERS_CNT;

	for(i = 0; i < USERS_CNT; i ++)
		turns[i] -> encode(buf);
}

void ogTurnInfo::alloc_turns(int num)
{
	int i;

	if(num) USERS_CNT = num;

	if(USERS_CNT){
		turns = new ogPlayerTurnInfo*[USERS_CNT];
		for(i = 0; i < USERS_CNT; i ++)
			turns[i] = new ogPlayerTurnInfo;
	}
}

void ogTurnInfo::free_turns(void)
{
	int i;
	if(USERS_CNT){
		for(i = 0; i < USERS_CNT; i ++)
			delete turns[i];

		delete turns;
	}
	USERS_CNT = 0;
	turns = 0;
}

ogTurnInfo::ogTurnInfo(void)
{
	STEP_ID = -1;
	USERS_CNT = 0;
	turns = 0;
}

ogTurnInfo::~ogTurnInfo(void)
{
	free_turns();
}


ogPlayerTurnInfo::ogPlayerTurnInfo(void)
{
	UID = IS_FINISHED = RANK = MOVE_TIME = MOVE_STEPS = USER_SEEDS_CNT = 0;
	BOTTLES_CNT = TOTAL_SEEDS_CNT = ARCANES_CNT =	DESTROYS_CNT = 0;

	seeds = 0;
}

ogPlayerTurnInfo::~ogPlayerTurnInfo(void)
{
	if(seeds) delete seeds;
}

void ogSeedInfo::decode(ogBuffer& buf,int last)
{ 
	buf.set_separator('#');
	buf > X > Y > Z;

	if(last)
		buf.set_separator(';');

	buf > ArcaneID; 
}

void ogSeedInfo::encode(ogBuffer& buf,int last)
{ 
	buf.set_separator('#');
	buf < X < Y < Z;

	if(last)
		buf.set_separator(';');

	buf < ArcaneID; 
}

ogTurnInfo* ogPacket::get_turn_by_idx(int idx)
{
	if(idx >= 0 && idx < STEPS_CNT)
		return turns[idx];

	return NULL;
}

ogTurnInfo* ogPacket::get_turn_by_num(int num)
{
	int i;
	for(i = 0; i < STEPS_CNT; i ++){
		if(turns[i] -> STEP_ID == num)
			return turns[i];
	}

	return NULL;
}

void ogPlayerTurnInfo::init_seed(int idx,int x,int y,int z,int a_id)
{
	seeds[idx].X = x;
	seeds[idx].Y = y;
	seeds[idx].Z = z;
	seeds[idx].ArcaneID = a_id;
}

void ogPlayerTurnInfo::get_seed(int idx,int& x,int& y,int& z,int& a_id)
{
	x = seeds[idx].X;
	y = seeds[idx].Y;
	z = seeds[idx].Z;
	a_id = seeds[idx].ArcaneID;
}

int ogPacket::get_player_id(int uid)
{
	int i;
	for(i = 0; i < PLAYERS_CNT; i ++)
		if(players[i].UID == uid) return i;

	return -1;
}

ogPlayerTurnInfo* ogTurnInfo::get_player_turn(int uid)
{
	int i;

	for(i = 0; i < USERS_CNT; i ++){
		if(turns[i] -> UID == uid)
			return turns[i];
	}

	return NULL;
}

int ogPacket::check_return(char* p)
{
	static char* return_str = "OK:KDLAB";
	static char* next_turn_str = "NEXT_MOVE";

	if(!stricmp(p,next_turn_str)) return 2;

	if(strlen(p) < strlen(return_str) || stricmp(p,return_str)) return 0;

	return 1;
}

int ogGetErrorStr(void)
{
	if(wi_D.input_buffer() && strlen(wi_D.input_buffer()) >= strlen(ogErrorStr0)){
		if(!strncmp(wi_D.input_buffer(),ogErrorStr0,strlen(ogErrorStr0))){
			ogErrorStr = wi_D.input_buffer();
			return 1;
		}
	}

	ogErrorStr = iGetText(iTXT_ERROR);
	return 0;
}

