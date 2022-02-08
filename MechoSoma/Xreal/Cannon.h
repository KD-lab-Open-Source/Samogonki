////////////////////////////////////////////////////////////////////
//	Пушка
////////////////////////////////////////////////////////////////////

#include "BaseObject.h"
#include "FirePoint.h"

class Cannon : public BaseObject {
	Vect3f A, B;
	cMesh* model;
public:
	Cannon();
	Cannon(class cMesh* model);
	void fire();
	void quant();
	void show()  const;
};

class Fortress : public BaseObject, public vector<Cannon> {
	DurationTimer latency;
	cMesh* model;
public:
	Fortress();
	Fortress(cMesh* model);
	void quant();
	void show()  const;
};

class WaterPump : public BaseObject, public FirePointList  {
	cMesh* model;
public:
	WaterPump();
	WaterPump(cMesh* model);
	void quant();	
};
