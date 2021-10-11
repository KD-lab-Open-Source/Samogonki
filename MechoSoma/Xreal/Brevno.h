#ifndef _BREVNO_
#define _BREVNO_

class Brevno : virtual public Body
{
	Vect3f R0;
public: 
	Brevno();
	void calc_forces_and_drags();
	void setPositionInit();
};

class Raft : public Brevno
{
public: 
	Raft();
//	void quant();
};


#endif _BREVNO_
