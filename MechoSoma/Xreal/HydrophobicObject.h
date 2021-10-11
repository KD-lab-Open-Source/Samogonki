#pragma once
#pragma warning( disable : 4250 )  

class HydrophobicObject : virtual public Body
{
	DurationTimer timer;
	int dying;
public:
	HydrophobicObject() : dying(0) { set_archimedean_force(0.001f); }
	void quant();
};