#include "StdAfx.h"
#include "FirePoint.h"
#include "Mesh3ds.h"
#include "HashStringGroup.h"

//////////////////////////////////////////////////////////////////////////
// Конструктора
//////////////////////////////////////////////////////////////////////////
typedef FirePoint* (*FirePointConstructor)(const Vect3f& dummyA, const Vect3f& dummyB);

FirePoint* baseFirePointConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new FirePoint(dummyA, dummyB);
}

FirePoint* CannonConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new CannonPoint(dummyA, dummyB);
}

FirePoint* FireConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new DragonPoint(dummyA, dummyB,FXLAB_FIRE_POINT_INTERFACE_FIRE);
}

FirePoint* WaterConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new DragonPoint(dummyA, dummyB,FXLAB_FIRE_POINT_INTERFACE_PUMP);
}

FirePoint* ClawConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new DragonPoint(dummyA,dummyB,FXLAB_FIRE_POINT_INTERFACE_CLAW);
}

FirePoint* FountainConstructor(const Vect3f& dummyA, const Vect3f& dummyB)
{
	return new DragonPoint(dummyA,dummyB,FXLAB_FIRE_POINT_INTERFACE_BOSS_WATER);
}

//////////////////////////////////////////////////////////////////////////
// Hash-table типов
//////////////////////////////////////////////////////////////////////////
struct FirePointType
{
	string dummyB;
	FirePointConstructor constructor;
	
	FirePointType(){}
	FirePointType(char* str, FirePointConstructor constr) : dummyB(str), constructor(constr) {}
};

struct FirePointTypeList :  HashStringGroup<FirePointType>
{
	FirePointTypeList()
	{
		add("vectora", FirePointType("vectorb", &FireConstructor));
		add("cannona", FirePointType("cannonb", &CannonConstructor));
		add("firea", FirePointType("fireb", &FireConstructor));
		add("watera", FirePointType("waterb", &WaterConstructor));
		add("clawa", FirePointType("clawb", &ClawConstructor));
		add("fountaina", FirePointType("fountainb", &FountainConstructor));
	}

} fire_point_types;


//////////////////////////////////////////////////////////////////////////
// FirePointList
//////////////////////////////////////////////////////////////////////////
int FirePointList::parseDummy(DummyList& dummies)
{
	DummyList::iterator ia;
	int i = 0;
	FOR_EACH(dummies, ia){
		FirePointType* t = fire_point_types.look(ia -> name);
		if(t){
			string nb(ia -> name);
			nb.replace(0, t -> dummyB.size(), t -> dummyB);
			push_back( t -> constructor(*ia, dummies.Find((char*)nb.c_str())) );
			i++;
			}
		}
	return i;
}	

int FirePointList::parseDummy(cMesh* mesh)
{
	return parseDummy(mesh -> Dummies);
}
	
void FirePointList::fire(const MatXf& bodyPose)
{
	iterator i;
	FOR_EACH(*this, i)
		(**i).Fire(bodyPose);
}

void FirePointList::fire_with_rotate(const MatXf& bodyPose)
{
	iterator i;
	FOR_EACH(*this, i)
		(**i).FireWithRotate(bodyPose);
}

DragonPoint::DragonPoint(const Vect3f& dummyA, const Vect3f& dummyB,const int fxID) : FirePoint(dummyA,dummyB)
{
	fxlabFirePoint.Open(fxID);
};


