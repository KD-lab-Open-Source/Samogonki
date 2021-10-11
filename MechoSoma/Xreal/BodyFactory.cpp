#include "StdAfx.h"

#include "BodyFactory.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "ControlledObject.h"
#include "PointControlledObject.h"
#include "CrawlingObject.h"

#include "Worm.h"
#include "Butterfly.h"
#include "Dragon.h"
#include "Spider.h"
#include "Pig.h"
#include "Penguin.h"
#include "Dog.h"
#include "Brevno.h"
#include "Shark.h"
#include "Ship.h"

#include "Tomato.h"
#include "Bubble.h"
#include "Crystal.h"
#include "Clown.h"
#include "EvilEye.h"
#include "Suicide.h"
#include "LittleSpider.h"
#include "SmartMissile.h"

#include "BonusMan.h"
#include "StaticBonus.h"

#include "Frog.h"
#include "SmartStone.h"
#include "Whale.h"


template <class T> 
class BodyCreator : public GeneralBodyCreator
{
public:
	BodyCreator(int si) : GeneralBodyCreator(si) {}
	Body* create() { return new T(); }
};
	
BodyFactory::BodyFactory()
: HashMap(100)
{
	#define ADD_TYPE(Type)	add(#Type, new BodyCreator<Type>(sizeof(Type)));

	//  Racers
	ADD_TYPE(WhaleRacer);		      
	ADD_TYPE(FrogRacer);		      
	ADD_TYPE(SmartStoneRacer);
	ADD_TYPE(DragonRacer);
	ADD_TYPE(SpiderRacer);

	//  Arcans
	ADD_TYPE(Tomato);
	ADD_TYPE(Bubble);
	ADD_TYPE(Crystal);
	ADD_TYPE(Clown);
	ADD_TYPE(EvilEye);
	ADD_TYPE(Suicide);
	ADD_TYPE(LittleSpider);
	ADD_TYPE(Caterpillar);
	ADD_TYPE(SmartMissile);
	ADD_TYPE(SmartFireBall);
	ADD_TYPE(SmartLightningBolt);
	ADD_TYPE(SmartVoodooFire);

	ADD_TYPE(Worm);

	ADD_TYPE(BonusMan1);
	ADD_TYPE(BonusMan2);
	ADD_TYPE(BonusMan3);
	ADD_TYPE(BonusMan4);

	//  Animals
	ADD_TYPE(Butterfly);
	ADD_TYPE(WickedButterfly);
	ADD_TYPE(Frog);
	ADD_TYPE(Dragon);
	ADD_TYPE(Spider);
	ADD_TYPE(Pig);
	ADD_TYPE(Penguin);
	ADD_TYPE(Dog);
	ADD_TYPE(Brevno);
	ADD_TYPE(Raft);
	ADD_TYPE(Shark);
	ADD_TYPE(Fish);
	ADD_TYPE(Ship);

	ADD_TYPE(StaticBonusAcceleration);
	ADD_TYPE(StaticBonusLife);
	ADD_TYPE(StaticBonusMana);
	ADD_TYPE(StaticBonusStar);

	ADD_TYPE(mchMechosRacer);

	#undef ADD_TYPE
}

void BodyFactory::add(const char* type_name, GeneralBodyCreator* creator)
{
	(*this)[type_name] = creator;
}

Body* BodyFactory::operator()(const char* type_name)
{
	iterator i = find(type_name);
	if(i == end()){
		XBuffer buf;
		buf < "Unknown Body Type: " < type_name;
		ErrH.Abort(buf);
		}
	return i -> second -> create();	
}

int BodyFactory::sizeOf(const char* type_name)
{
	iterator i = find(type_name);
	if(i == end()){
		XBuffer buf;
		buf < "Unknown Body Type: " < type_name;
		ErrH.Abort(buf);
		}
	return i -> second -> sizeOf();	
}


Body* BodyDispatcher::createBody(const char* type_name, const Vect3f& pose, int angle)
{
	Body* b = body_factory(type_name);
	if(angle == -1000)
		angle = random(360);
	b -> setPosition(pose, G2R(angle));
	attachBody(b);
	return b;
}

	
