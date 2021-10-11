
#include "FirePoint.h"

#pragma warning (disable:4250)

class Ship : public AttackAndHomeObject, public FirePointBody 
{
public: 
	Ship();
	void setPositionInit();
	void calc_forces_and_drags();
};

#pragma warning (default:4250)
