
#ifndef __GROWING_BODY__
#define __GROWING_BODY__

class GrowingBody : virtual public Body
{
public:
	GrowingBody(float initial_size, int time_to_grow, int time_to_dwindle = 0, int time_to_live = 0x7fffffff);
	void setTimeToLive(int time_to_live);
	void decrSize(float ds); 
	void post_quant();

	enum GrowingMode {
		Growing,
		NormalSize,
		Dwindling, 
		Dead
		};

	GrowingMode growing_mode() const { return growing_mode_; }

protected:
	GrowingMode growing_mode_;
	float initial_size;
	float size;
	float prev_size;
	int time_to_grow;
	int time_to_dwindle;
	int normal_size_time;
	MeasurementTimer timer;
	DurationTimer normal_size_timer;
};


#endif // __GROWING_BODY__