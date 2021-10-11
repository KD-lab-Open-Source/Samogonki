#ifndef __SYNCRO_TIMER__
#define __SYNCRO_TIMER__

typedef unsigned int time_type;
	
class SyncroTimer
{
public: 
	// Main property
	time_type operator()() const { return time; }
	
	SyncroTimer()
	{
		set(1, 15, 100);
		time = 1; time_offset = 0;
		time_speed = 1;
	}

	void set(int syncro_by_clock_ = 1, time_type time_per_frame_ = 12, time_type max_time_interval_ = 200) 
	{ 
		syncro_by_clock = syncro_by_clock_; 
		time_per_frame = time_per_frame_;
		max_time_interval = max_time_interval_;
	}

	SyncroTimer& adjust()
	{
		if(syncro_by_clock){
			time_type t = clocki();
			time_type dt = t - time - time_offset;
			if(dt > max_time_interval)
				dt = max_time_interval;
			time += round(dt*time_speed);
			time_offset = t - time;
			}
		return *this;
	}

	void next_frame()
	{
		if(syncro_by_clock)
			adjust();									 
		else
			time += round(time_per_frame*time_speed);
	}

	void skip()
	{
		if(syncro_by_clock)
			time_offset = clocki() - time;
	}

	void setTime(time_type t) { time = t; time_offset = syncro_by_clock ? clocki() - time : 0; }
	
	void setSpeed(float speed) { time_speed = speed; }

private:	
	time_type time;
	time_type time_offset;
	time_type max_time_interval;
	time_type time_per_frame;
	int syncro_by_clock;
	float time_speed;
};

#endif // __SYNCRO_TIMER__
