// StaticBonus.h: interface for the StaticBonus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICBONUS_H__CC5F563F_D532_460D_97DE_89B41E32D8CE__INCLUDED_)
#define AFX_STATICBONUS_H__CC5F563F_D532_460D_97DE_89B41E32D8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Body.h"

class StaticBonus : virtual public Body  
{
	int bonusID;
	DelayTimer birth_timer;
	
public:
	StaticBonus(int bonusID_);
	void quant();
	void setPosition(const Vect3f& pose, float psi);
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body) { return 0; }
};

class StaticBonusAcceleration : public StaticBonus
{
public:
	StaticBonusAcceleration() : StaticBonus(1), Body(myM3DType(18), CenteredBottomAlignmentedBound) {}
};

class StaticBonusLife : public StaticBonus
{
public:
	StaticBonusLife() : StaticBonus(2), Body(myM3DType(31), CenteredBottomAlignmentedBound) {}
};

class StaticBonusMana : public StaticBonus
{
public:
	StaticBonusMana() : StaticBonus(3), Body(myM3DType(30), CenteredBottomAlignmentedBound) {}
};

class StaticBonusStar : public StaticBonus
{
public:
	StaticBonusStar() : StaticBonus(4), Body(myM3DType(21), CenteredBottomAlignmentedBound) {}
};

#endif // !defined(AFX_STATICBONUS_H__CC5F563F_D532_460D_97DE_89B41E32D8CE__INCLUDED_)
