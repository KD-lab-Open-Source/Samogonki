#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "XrealWorlds.h"
#include "Mechos.h"
#include "SimpleClip.h"
//#include "PrmEdit.h"
#include "Params.h"
#include "TERRA.H"
#include "IVisGeneric.h"
#include "Dispatcher.h"
#include "mechosoma.h"
#include "Xreal_utl.h"
#include "FieldGrid.h"
#include "XGR.h"
#include "PolyhedronLibrary.h"
#include "CameraDispatcher.h"
#include "IGraph3d.h"
#include "AllocationTracking.h"
#include "Statistics.h"
#include "DebugPrm.h"

int interface_world_now;
cInterfaceVisGeneric* xrealIVG;
cInterfaceGraph3d* xrealIGR;
cM3D* xrealM3D;
WorldDispatcher* Mdisp;
TriangleMap* tri_map;
PolyhedronLibrary* polyhedron_lib;

#ifndef _FINAL_VERSION_
AllocationAccumulator xreal_mem;
int xreal_log = 0;
int time_to_restart = 0;
int repeate_times = 1;
XStream recorder_logging;
//SmartBuffer fout("lst");
XStream fout("lst", XS_OUT);
XBuffer bout(200000);
XBuffer msg_buf(10000);
int mechos_default_config_counter = 0;
#endif

int Xreal_key_active = 0;

// TODO: @caiiiycuk investigate this
#ifdef WTF
int controlfp_flags = _PC_24;
int controlfp_mask = _MCW_PC;
#endif

void Xreal_init()
{
	allocation_tracking("before Xreal_init");
	CONTROL_FP();

#ifndef _FINAL_VERSION_
	mechos_default_config_counter = 0;
	xreal_log = check_command_line("xreal_log") || check_command_line("second_verify") ? 1 : 0;
	if(check_command_line("verify"))
		xreal_log = 2;
	const char* dir_str = check_command_line("time_to_restart:");
	if(dir_str)
		time_to_restart = atoi(dir_str)*1000;

	static int first_call = 1;
	if(first_call){
		first_call = 0;

		dir_str = check_command_line("repeate_times:");
		if(dir_str)
			repeate_times = atoi(dir_str) + 1;

		XStream test(0);
		if(test.open("shura") || check_command_line("hide_evolve:") || check_command_line("shura")){
			Xreal_key_active = 1;
			if(!check_command_line("disable_fp_exceptions")){
				_clearfp();
				controlfp_flags |= _controlfp( 0, 0 ) & ~(EM_OVERFLOW | EM_ZERODIVIDE | EM_DENORMAL |  EM_INVALID);
				controlfp_mask |= MCW_EM; 
				CONTROL_FP();
				}

			// Все потоки - в файлы
			//cerr = *(new ofstream("cerr"));		// cerr
			//_iob[1] = *fopen( "stdout", "w" );	// stdout
			//_iob[2] = *fopen( "stderr", "w" );	// stderr
			}

		fout.SetDigits(15);
		msg_buf.SetDigits(4);
		}
	else
		if(check_command_line("second_verify"))
			xreal_log = 2;

	bout.init();
	bout.SetDigits(15);
	recorder_logging.close();
	if(xreal_log == 2)
		recorder_logging.open("lst_", XS_IN);
#endif

	xrealIVG = CreateIVisGeneric();
	xrealIGR = (cInterfaceGraph3d*)xrealIVG -> GetIGraph3d();
	xrealM3D = (cM3D*)xrealIVG -> GetM3D();

	char* wname = vMap -> wTable[vMap -> cWorld].name;

//	allocation_tracking("before TriMap");
	tri_map = new TriangleMap;
	tri_map -> loadSpots(GetTargetName("Spots.sst"));
	//if(check_if_file_exist(GetTargetName("CollisionMap.obj")))
	//	tri_map -> load(GetTargetName("CollisionMap.obj"));
	//else
	tri_map -> load(xrealIVG, strstr(wname, "Interface") ? 1 : 0);
	tri_map -> load(*xrealM3D);
	allocation_tracking("Collision Map");

	polyhedron_lib = new PolyhedronLibrary(*(cMeshLibrary*)xrealIVG -> GetMeshLibrary());
//	allocation_tracking("PolyhedronLibrary");


	interface_world_now = 0;
	if(!environment_disabled){
		if(strstr(wname, "Water"))
			Mdisp = new WaterWorld(mchCurrentTrack);
		if(strstr(wname, "Lava"))
			Mdisp = new LavaWorld(mchCurrentTrack);
		if(strstr(wname, "Forest"))
			Mdisp = new ForestWorld(mchCurrentTrack);
		if(strstr(wname, "Watch"))
			Mdisp = new WatchWorld(mchCurrentTrack);
		if(strstr(wname, "Mountain"))
			Mdisp = new MountainWorld(mchCurrentTrack);
		if(strstr(wname, "Town"))
			Mdisp = new TownWorld(mchCurrentTrack);
		if(strstr(wname, "Interface")){
			interface_world_now = 1;
			Mdisp = new InterfaceWorld(mchCurrentTrack);
			}
		Mdisp -> loadGears(*xrealM3D);
		}
	if(!Mdisp)
		Mdisp = new WorldDispatcher(0);


#ifndef _FINAL_VERSION_
	if(check_command_line("build_hull"))
		ErrH.Exit();
#endif

	allocation_tracking("Xreal_init");
}
    
void Xreal_finit()
{
	delete Mdisp;
	Mdisp = 0;
	delete polyhedron_lib;
	delete tri_map;
	xrealM3D = 0;
	xrealIVG -> Release();
}

void Xreal_key_handler(int &key)
{
#ifndef _FINAL_VERSION_

	switch(key){
		case VK_F4:
			if(XKey.Pressed(VK_MENU))
				return;
			d3dFlipToGdiSurface();
			if(!XKey.Pressed(VK_SHIFT))
				edit_parameters();
			else
				reload_parameters();
			return;
		case VK_F6:
			d3dFlipToGdiSurface();
			profiler_start_stop();
			return;
		}

	if(key == VK_F1 && XKey.Pressed(VK_SHIFT))
		Xreal_key_active = 1 - Xreal_key_active;
	if(!Xreal_key_active)
		return;

	Mdisp -> key_handler(key);

	switch(key){
		case VK_F2:
			switch((XKey.Pressed(VK_CONTROL) ? 1 : 0) + (XKey.Pressed(VK_SHIFT) ? 2 : 0) + (XKey.Pressed(VK_MENU) ? 4 : 0)){
				case 0:	
					Xreal_write_mode ^= WRITE_BODY_STATE;
					break;
				case 1:	
					break;
				case 2:	
					Xreal_write_mode ^= WRITE_CAMERA_STATE;
					break;
				case 3:	
					Xreal_write_mode ^= WRITE_BODY_DISPATCHER_STATISTICS;
					break;
				}
			break;
		case VK_F3:
			switch((XKey.Pressed(VK_CONTROL) ? 1 : 0) + (XKey.Pressed(VK_SHIFT) ? 2 : 0) + (XKey.Pressed(VK_MENU) ? 4 : 0)){
				case 0:
					Xreal_show_mode ^= SHOW_BODIES;
					break;
				case 1:
					Xreal_show_mode ^= SHOW_TERRAIN;
					break;
				case 2:
					Xreal_show_mode ^= SHOW_CAMERA;
					break;
				case 3:
					Xreal_show_mode ^= SHOW_MAP_OBJECTS;
					break;
				case 5:
					Xreal_show_mode ^= SHOW_FIELDS;
					break;
				}
			break;
		}
#endif
}

void Xreal_draw_handler()
{
#ifndef _FINAL_VERSION_
	profiler_quant();
	Mdisp -> draw_handler();
#endif
}

///////////////////////////////////////////////////////////////
//	BodyDispatcher's debug
///////////////////////////////////////////////////////////////
void BodyDispatcher::key_handler(int& key)
{
	switch(key){
		case VK_F5:
			if(!XKey.Pressed(VK_SHIFT))
				camera_dispatcher -> setTrackObserver(0, 60, 300, 0);
				//camera_dispatcher -> runStartScenario();
			else
				camera_dispatcher -> setObserver(0);
			break;
		case 'W':
			camera_dispatcher -> rotateWhenStopTime();
			break;
		case 'C':
			camera_dispatcher -> centerMechosWhenStopTime();
			break;
		case 'B':
			if(!XKey.Pressed(VK_SHIFT))
				setNextBody();
			else
				setPrevBody();
			break;
		case 'R':
			if(!XKey.Pressed(VK_SHIFT))
				active_mechos() -> set_feature(Mechos::HUMBLE);
			else				
				active_mechos() -> reset_feature(Mechos::HUMBLE);
			//active_mechos() -> start_teleportation(active_mechos() -> R() + Vect3f(0, 30, 100));
			//active_mechos() -> Mechos::startEffect(Mechos::StoneHitEffect);
			break;
		case 'Y':
			active_mechos() -> jump();
			break;
		case 'T':
			if(!XKey.Pressed(VK_SHIFT))
				active_mechos() -> disorganize();
			else
				active_mechos() -> startEffect(Mechos::StoneHitEffect);
			break;
		}
}

void BodyDispatcher::draw_handler()
{
#ifndef _FINAL_VERSION_
	if(Xreal_show_mode & SHOW_TERRAIN)
		tri_map -> show(active_body() -> R().xi(), active_body() -> R().yi(), terrain_show_distance, terrain_show_grid_color);
	if(Xreal_show_mode & SHOW_BODIES)
		showBodies();
	if(Xreal_show_mode & SHOW_MAP_OBJECTS)
		showObjects();
	if(Xreal_show_mode & SHOW_FIELDS)
		permanent_fields_grid.show(Mdisp -> active_body() -> R().xi(), Mdisp -> active_body() -> R().yi(), terrain_show_distance, terrain_show_grid_color);
	if(Xreal_show_mode & SHOW_CAMERA)
		camera_dispatcher -> show();
	
	if(Xreal_write_mode){
		if(Xreal_write_mode & WRITE_BODY_STATE)
			active_body() -> write_state(msg_buf);
		
		if(Xreal_write_mode & WRITE_BODY_DISPATCHER_STATISTICS){
			Mdisp -> print_statistics(msg_buf);
			msg_buf < "Memory: " <= xreal_mem.size < "\n";
			msg_buf < "Blocks: " <= xreal_mem.blocks < "\n";
			}

		if(Xreal_write_mode & WRITE_CAMERA_STATE)
			camera_dispatcher -> write_state(msg_buf);

		show_debug_window(msg_buf, XGR_MAXX/3, XGR_MAXY);
		//draw_text(msg_buf);
		}
	else
		hide_debug_window();

	msg_buf.init();
#endif
}

void draw_text(char* text)
{
// TODO: @caiiiycuk invetigate this
#ifdef WTF
	HDC hdc;
	RECT rect = { gameWnd -> PosX, gameWnd -> PosY, gameWnd -> PosX + gameWnd -> SizeX , gameWnd -> PosY +  + gameWnd -> SizeY };
	GetBackBufferDC(&hdc);
	HFONT hfont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
	HGDIOBJ hold_font = SelectObject(hdc, hfont);
	DrawText(hdc, text, strlen(text), &rect, DT_EXPANDTABS);
	SelectObject(hdc, hold_font);
	ReleaseBackBufferDC(hdc);

//		if(RenderMode == XGRAPH_HICOLOR)
//			XGR_OutText(gameWnd -> CenterX - gameWnd -> SizeX2 + 2, gameWnd -> CenterY - gameWnd -> SizeY2 + 8,0xffff,msg_buf, 0, 1, 1, 0);
//		else{
//			xrealIGR -> SetViewColor(0, 0, 100, 80);
//			mchA_d3dOutString(gameWnd -> PosX + 5,gameWnd -> PosY + 5, 1.4f, 1.4f, msg_buf, RGB_MAKE(255,255,255), 255, 2, 0, 0.6f, 0);
//			}
#endif
}

const char* check_command_line(const char* switch_str)
{
// TODO: @caiiiycuk investigate this
#ifdef COMMAND_LINE
	for(int i = 1; i < __argc; i ++){
		const char* s = strstr(__argv[i], switch_str);
		if(s){
			for(const char* p = __argv[i]; p < s; p++)
				if(*p != '-' && *p != '/')
					goto cont;
			return s += strlen(switch_str);
			}
		cont:;
		}
#endif		
	return 0;
}

int check_if_file_exist(const char* name)
{
	XStream test(0);
	return vmapOpenResource((char*)name, test);
}

void append_message_to_file(const char* message, const char* fname)
{
	ofstream ff(fname, ios::app);
	ff << "\n\n=========================================================================================\n";
	time_t aclock;
	time( &aclock );                 /* Get time in seconds */
	ff << asctime( localtime( &aclock ) ) << endl;
	ff << "World: " << vMap -> wTable[vMap -> cWorld].name << endl;
	extern int mchNumRacers;
	ff << "Racers: " << mchNumRacers << endl;
	ff << message << endl;
	ff << "=========================================================================================\n";
	ff << endl;
}
