
#include "StdAfx.h"
#include "Body.h"
#include "HydrophobicObject.h"
#include "AnimalArcansPrm.h"

void HydrophobicObject::quant()
{
	calc_archimedean_force();
	if(!dying && submersion() > 0.1){
		timer.start(death_by_water_duration);
		dying = 1;
		}
	if(timer())
		setAlpha(timer()/(float)death_by_water_duration);
	else
		if(dying)
			kill();
}

