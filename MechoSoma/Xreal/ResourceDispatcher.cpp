#include "StdAfx.h"
#include "ResourceDispatcher.h"
#include "xreal_utl.h"
#include "statistics.h"
#include "Terra.h"

int ResourceUser::IDs;

SyncroTimer global_time;
SyncroTimer non_stop_global_time;

void ResourceDispatcher::quant()
{
	for(;;) {
		if(empty())
			return;
		time_type t_min = front() -> time;
		ResourceUser* user_min = front();
		iterator i;
		FOR_EACH(*this, i){
			ResourceUser& u = **i;
			if(t_min > u.time){
				t_min = u.time;
				user_min = &u;
				}
			}
		if(t_min < global_time()){
			if(!user_min -> quant())
				detach(user_min);
			else
				user_min -> time += user_min -> time_step();
			}
		else
			break;
		} 
}	


void SystemResourceDispatcher::do_start()
{
	if(start_log){
		start_log = 0;
		syncro_timer.setTime(1);
		iterator i;
		FOR_EACH(*this, i)
			(*i) -> time = syncro_timer();
		}
}

void SystemResourceDispatcher::reset()
{
	iterator i;
	FOR_EACH(*this, i)
		(*i) -> time = syncro_timer();
}

void SystemResourceDispatcher::quant()
{
	if(empty())
		return;

	do_start();
	
#ifndef _FINAL_VERSION_
	int auto_run = 0;
	if(xPressed(VK_TILDE))
		if(xPressed(VK_CONTROL))
			auto_run = 1;
		else
			if(xPressed(VK_SHIFT))
				step_mode = 0;
			else{
				step_mode = 1;
				if(make_step == -1){
					make_step = 1;
					last_press = clocki();
					}
				if(clocki() - last_press > 700)
					auto_run = 1;
				}
	else
		make_step = -1;

	if(step_mode && !auto_run){
		if(make_step == 1){
			step_quant();
			make_step = 0;
			}
		syncro_timer.skip(); 
		return;
		}
#endif

	syncro_timer.next_frame();

	for(;;) {
		syncro_timer.adjust();

		time_type t_min = front() -> time;
		ResourceUser* user_min = front();
		iterator i;
		FOR_EACH(*this, i){
			ResourceUser& u = **i;
			if(t_min > u.time){
				t_min = u.time;
				user_min = &u;
				}
			}
		if(t_min < syncro_timer()){
			if(!user_min -> quant())
				detach(user_min);
			else
				user_min -> time += user_min -> time_step();
			}
		else
			break;
		} 
}	

void SystemResourceDispatcher::step_quant()
{
	if(!empty()){
		time_type t_min = front() -> time;
		ResourceUser* user_min = front();
		iterator i;
		FOR_EACH(*this, i){
			ResourceUser& u = **i;
			if(t_min > u.time){
				t_min = u.time;
				user_min = &u;
				}
			}
		if(!user_min -> quant())
			detach(user_min);
		else
			user_min -> time += user_min -> time_step();
		} 
}	

void SystemResourceDispatcher::evolve(time_type interval, int do_log)
{
	if(empty())
		return;

	do_start();

	break_evolve_log = 0;

	syncro_timer.setTime(syncro_timer() + interval);

	for(;;) {
		time_type t_min = front() -> time;
		ResourceUser* user_min = front();
		iterator i;
		FOR_EACH(*this, i){
			ResourceUser& u = **i;
			if(t_min > u.time){
				t_min = u.time;
				user_min = &u;
				}
			}
		if(t_min < syncro_timer() && !break_evolve_log){
			if(!user_min -> quant())
				detach(user_min);
			else
				user_min -> time += user_min -> time_step();
			}
		else
			break;

		#ifndef _FINAL_VERSION_
			//MSG msg;
			//if(GetMessage(&msg, NULL, 0, 0) && msg.wParam)
			//if(GetAsyncKeyState(VK_MENU) & 128 && GetAsyncKeyState(VK_F4) & 128)
			//if(GetAsyncKeyState(VK_ESCAPE) & 128)
			//	ErrH.Exit();
			if(do_log){
				static int first_time = clocki();
				static int timer;
				if(timer < clocki()){
					timer = clocki() + 2000;
					XBuffer buf;
					buf < "World: " < vMap -> wTable[vMap -> cWorld].name < "\n";
					buf < (check_command_line("verify") ? "Verifying: " : "Saving: ") <= round(100.*t_min/syncro_timer()) < " %\n";
					int s = t_min/1000;
					int m = s/60;
					s -= m*60;
					buf < "Time elapsed: " <= m < " min " <= s < " sec\n";
					buf < "Speed: " <= double(t_min)/(clocki() - first_time + 1) < " ms/ms\n";
					show_debug_window((char*)buf, 230, 100);
					}
				}
		#endif		
		} 
}
