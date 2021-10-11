
#ifndef __PIG__
#define __PIG__

#pragma warning( disable : 4250 )  

class Pig : virtual public AttackAndHomeObject 
{
public: 
	Pig();
	void quant();
	void calc_forces_and_drags();
	int is_colliding_with(const Body& body){ return 1; }
	void setPositionInit();
};

#pragma warning( default : 4250 )  

#endif  // __PIG__
