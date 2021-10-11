#ifndef __FLYING_OBJECT__
#define __FLYING_OBJECT__

class FlyingObject : virtual public ControlledObject
{
protected:
	float relative_height;
	float lift_bias;
	void setPositionInit();
	void setHeight(float height);
	float currentHeight();
	void non_dynamic_evolve(float dt);

public: 
	FlyingObject();
};

#endif // __FLYING_OBJECT__
