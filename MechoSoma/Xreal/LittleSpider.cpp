#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "ControlledObject.h"
#include "CrawlingObject.h"
#include "LittleSpider.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

LittleSpider::LittleSpider()
: Body(myM3DType(12))
{
	type_ = LITTLE_SPIDER;
	traction = 1;
	k_traction = 5;
	k_rudder = 0.8f;
	mesh_offset.z = 3;
	//setInverseTarget(Mdisp -> searchBody(R(), MECHOS));
	//target_timer.start(3000);
	time_to_live.start(little_spider_time_to_live);
	search_closest_triangle_delay = 4;
}

void LittleSpider::quant()
{
	float t = little_spider_flash_amplitude*sin(2*M_PI*clocki()/little_spider_flash_period) - little_spider_flash_amplitude + 1.;
	if(t < 0)
		t = 0;
	setColor(t, 0, 0);

//	if(!target_timer){
//		setTarget(Mdisp -> searchBody(R(), MECHOS));
//		target_timer.start(1000);
//		}
	if(!time_to_live)
		kill();
	HydrophobicObject::quant();
}

void LittleSpider::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && owner() != body.ID){
		kill();
		body.damage(10);
		startSound(EFF_SPIDERS_ADD);
		}
}
