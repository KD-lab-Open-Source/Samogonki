
#ifndef __RESOURCE_DISPATCHER_H__
#define __RESOURCE_DISPATCHER_H__

#include "SynchroTimer.h"


/////////////////////////////////////////////////////////////////////////////////////////
//	General Time Resourcing
/////////////////////////////////////////////////////////////////////////////////////////
class ResourceUser
{				
	int ID;
	static int IDs;
public:
	explicit ResourceUser(time_type period) { dtime = period; time = 0; ID = ++IDs; }
        virtual ~ResourceUser() = default;
	virtual int quant() { return 1; } 

protected:
	time_type time;
	time_type dtime;	

	virtual void init_time(time_type time_) { time = time_ + time_step(); } 
	virtual time_type time_step() { return dtime; } 

	friend class ResourceDispatcher;
	friend class SystemResourceDispatcher;
};

class ResourceDispatcher : public list<PtrHandle<ResourceUser> >
{
public:
	void attach(ResourceUser* user) { push_back(0); back() = user; user -> init_time(global_time()); }
	void detach(ResourceUser* user) { PtrHandle<ResourceUser> p(user); remove(p); p.set(0); }
	void quant();
};


/////////////////////////////////////////////////////////////////////////////////////////
//	System Time Resourcing
/////////////////////////////////////////////////////////////////////////////////////////

class VoidFunctionCallResourceUser : public ResourceUser
{
	typedef void (*type)();
	type func;

public:
	VoidFunctionCallResourceUser(type func_, time_type dtime) :
		ResourceUser(dtime), func(func_) {}
	int quant() { func(); return 1; }
};


class SystemResourceDispatcher : public ResourceDispatcher
{
	SyncroTimer syncro_timer;
	time_type max_time_interval;
	int start_log;
	int step_mode;
	int make_step;
	int last_press;
	int break_evolve_log;

	void step_quant();
	void do_start();

public:
	SystemResourceDispatcher() { start_log = 0; step_mode = 0; make_step = 0; last_press = 0; break_evolve_log = 0; }
	void setTimer(int syncro_by_clock, time_type time_per_frame, time_type max_time_interval_) { syncro_timer.set(syncro_by_clock, time_per_frame, max_time_interval = max_time_interval_); }
	void attach(void (*func)(), time_type dtime) 
		{ ResourceDispatcher::attach(new VoidFunctionCallResourceUser(func, dtime)); }
	void start() { start_log = 1; }
	void reset();
	void skip_time(){ syncro_timer.skip(); }
	void quant();
	void evolve(time_type interval, int do_log = 0);
	void break_evolve() { break_evolve_log = 1; }
	void set_speed(float speed) { syncro_timer.setSpeed(speed); }
};


#endif // __RESOURCE_DISPATCHER_H__
	
