
#ifndef __Penguin__
#define __Penguin__

class Penguin : public Pig
{
public: 
	Penguin() : Body(myM3DType(25), CenteredBottomAlignmentedBound), AttackAndHomeObject(300, 1, 5000) { type_ = PENGUIN; }
};

#endif  // __Penguin__
