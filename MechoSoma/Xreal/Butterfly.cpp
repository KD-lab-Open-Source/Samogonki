#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "Butterfly.h"
#include "Mesh3ds.h"
#include "sound.h"
#include "AnimalsPrm.h"

Butterfly::Butterfly()
: Body(myM3DType(4)),
  FirePointBody(geometry)
{
	type_ = BUTTERFLY;
	traction = 1;
	rudder = 0;
	k_traction = 10;
	k_rudder = 0.05f;
}

void Butterfly::quant()
{
	if(findFrame().IsPhasePassed(0))
		startSound(EFF_BUTTERFLY);

	traction = 1;
	if(!rudder_latency){
		rudder_latency.start(1000 + random(2000));
		rudder = frnd(1.);
		setHeight(70 + random(50));
		}
}

WickedButterfly::WickedButterfly()
: Body(myM3DType(4)),
  AttackAndHomeObject(5000, 1)
{
	k_traction = wicked_butterfly_traction;
}

void WickedButterfly::control()
{
	AttackAndHomeObject::control();
	traction = getDist(target, R()).norm()*wicked_butterfly_distance_to_traction_factor;
	if(traction > 2)
		traction = 2;
}
