
#ifndef __DEMO_DISPATCHER_H__
#define __DEMO_DISPATCHER_H__

enum mchDemoDispatcherModes 
{
	DEMO_IDLE = 0,
	DEMO_ROTATING_CAMERA,	// 1
	DEMO_MOVING_CAMERA,	// 2
	DEMO_PLANET_VIEW,	// 3
	DEMO_CHANGE_PLAYER,	// 4

	DEMO_MODE_MAX
};

class mchDemoDispatcher
{
	int cur_mode;
	int cur_mode_timer_start;
	int cur_mode_timer_max;

	int enabled;

	int get_timer(int tm,int rnd) const;

public:

	int mode(void) const { return cur_mode; }

	void set_mode(int mode,int timer);
	void quant(void);

	void enable(int m = 1){ enabled = m; }
	int is_enabled(void) const { return enabled; }

	mchDemoDispatcher(void);
	~mchDemoDispatcher(void);
};

extern mchDemoDispatcher mch_demoD;

void mchInitDemo(void);
void mchFinitDemo(void);

#endif /* __DEMO_DISPATCHER_H__ */

