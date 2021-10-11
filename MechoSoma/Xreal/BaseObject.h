#ifndef __BASE_OBJECT__
#define __BASE_OBJECT__

#include "Timers.h"

class BaseObject 
{
public:
	BaseObject(){ alive_ = 0; handleCount = 0; }
	virtual ~BaseObject(){}

	int alive() const { return  alive_; }
	virtual void kill() { alive_ = 0; }
	virtual void quant(){}

	// Debug
	virtual void show() const {}
	
	mutable int handleCount; 

protected:
	unsigned int alive_ : 1;
	friend class BodyDispatcher;
};


class BaseObjectTTL : virtual public BaseObject 
{
	DurationTimer time_to_live;
public:
	BaseObjectTTL(int time_to_live_) { time_to_live.start(time_to_live_); }
	void quant() { if(!time_to_live) kill(); }
};

#endif  // __BASE_OBJECT__
