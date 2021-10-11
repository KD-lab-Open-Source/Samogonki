#include "StdAfx.h"
#include "Body.h"
#include "GrowingBody.h"


GrowingBody::GrowingBody(float initial_size_, int time_to_grow_, int time_to_dwindle_, int time_to_live)
{
	initial_size = initial_size_;
	time_to_grow = time_to_grow_;
	time_to_dwindle = time_to_dwindle_;
	normal_size_time = time_to_live - time_to_grow - time_to_dwindle;

	timer.start();	
	if(time_to_grow){
		setScale(size = initial_size);
		growing_mode_ = Growing;
		}
	else{
		size = 1;
		growing_mode_ = NormalSize;
		normal_size_timer.start(normal_size_time);
		}
	prev_size = size;
}

void GrowingBody::setTimeToLive(int time_to_live)
{
	normal_size_time = time_to_live - time_to_grow - time_to_dwindle;
	if(growing_mode() != Growing){
		growing_mode_ = NormalSize;
		normal_size_timer.start(normal_size_time);
		}
}

void GrowingBody::post_quant()
{
	Body::post_quant();
	switch(growing_mode_){
		case Growing:
			if((size += (float)timer()/(float)time_to_grow) > 1){
				size = 1;
				growing_mode_ = NormalSize;
				normal_size_timer.start(normal_size_time);
				}
			break;

		case NormalSize:
			if(!normal_size_timer){
				growing_mode_ = Dwindling;
				if(!time_to_dwindle){
					growing_mode_ = Dead;
					kill();
					}
				}
			break;

		case Dwindling:
			if((size -= (float)timer()/(float)time_to_dwindle) < initial_size){
				size = initial_size;
				kill();
				}
			break;

		case Dead:
			return;
		}

	timer.start();	
	if(size != prev_size){
		setScale(size);
		prev_size = size;
		}
}

void GrowingBody::decrSize(float ds)
{
	if(growing_mode_ != Growing && (size -= ds) < initial_size){
		size = initial_size;
		kill();
		}
}		    
