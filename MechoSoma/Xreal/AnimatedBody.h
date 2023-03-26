#ifndef __ANIMATED_BODY_H__
#define __ANIMATED_BODY_H__

#include "ResourceDispatcher.h"

struct AnimationData
{
	const char* channel;
	float period;
	float start;
	float finish;

	AnimationData(float period_ = 2000, float start_ = 0, float finish_ = -1, const char* channel_ = 0) :
		period(period_), start(start_), finish(finish_), channel(channel_){}
};

class EffectData : public AnimationData 
{
public:
	enum Mode {
		Cycled,
		Forward, 
		ForwardThenBackwardThenForwardThenBackward,
		BackwardThenForwardThenBackward,
		ForwardThenBackward,
		ThenBackward,
		Backward
		};

	EffectData(){}
	EffectData(float period, const char* channel, Mode mode_, int sound_ = 0, int delay_ = 0);
	
	int soundID() const { return sound; }

protected:
	Mode mode;
	int sound;
	int delay;
};

#endif // __ANIMATED_BODY_H__
