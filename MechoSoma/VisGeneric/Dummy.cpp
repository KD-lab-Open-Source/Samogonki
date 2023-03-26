#include "xtool.h"
#include "Base.h"
#include "Dummy.h"
#include "HashStringGroup.h"
#include "cString.h"

#include "port.h"

//////////////////////////////////////////////////////////////////////////
// Hash-table для определения типов дамми
//////////////////////////////////////////////////////////////////////////
struct DummyNameMap :  HashStringGroup<int>
{
	DummyNameMap()
	{
		add("engine", M3D_ENGINE);
		add("back", M3D_ENGINE); // чтобы не переименовывать задние колеса
		add("front", M3D_FRONT);
		
		add("lf_wheel", M3D_LF_WHEEL);
		add("rf_wheel", M3D_RF_WHEEL);
		add("lb_wheel", M3D_LB_WHEEL);
		add("rb_wheel", M3D_RB_WHEEL);

		add("bound", M3D_BOUND);

		add("vectora", 0);
		add("vectorb", 0);
		
		add("cannona", 0);
		add("cannonb", 0);

		add("firea", 0);
		add("fireb", 0);

		add("watera", 0);
		add("waterb", 0);

		add("clawa", 0);
		add("clawb", 0);

		add("fountaina", 0);
		add("fountainb", 0);

		add("float", 0);
	}

} dummy_name_map;


unsigned int CalcType(const char* NameMesh)
{
	int* type = dummy_name_map.look(NameMesh);
	if(type)
		 return *type;
	return 0;
}

////////////////////////////////////////////////////////////////
//	Dummy
////////////////////////////////////////////////////////////////
Dummy::Dummy(const Vect3f& v, const char* n, int t)
: Vect3f(v)
{
	name = new char[strlen(n) + 1];
	strcpy(name, n);
	Type = t;
}

Dummy::Dummy(const Dummy& d)
: Vect3f(d)
{
	name = new char[strlen(d.name) + 1];
	strcpy(name, d.name);
	Type = d.Type;
}

Dummy& Dummy::operator = (const Dummy& d)
{
	if(name)
		delete name;

	(Vect3f&)*this = d;
	name = new char[strlen(d.name) + 1];
	strcpy(name, d.name);
	Type = d.Type;
	return *this;
}

Dummy::~Dummy()
{
	if(name)
		delete[] name;
}
	
//////////////////////////////////////////////////
//	Dummy List
//////////////////////////////////////////////////
int DummyList::Add(const Vect3f& v, const char* n)
{
        cString t = n;
        t.ToLower();
	int* type = dummy_name_map.look(t);
	if(type){
		push_back( Dummy(v, t, *type) );
		return 1;
		}
	return 0;
}

void DummyList::Print()
{
//	iterator i;
//	FOR_EACH(*this, i)
//		XCon <= i -> Type < "\t" < i -> name < "\t" <= *i;
}


void DummyList::Scale(float dSx,float dSy,float dSz)
{
	iterator i;
	FOR_EACH(*this, i)
		i -> Scale(dSx, dSy, dSz);
}
  
void DummyList::Translate(float dx,float dy,float dz)
{
	iterator i;
	FOR_EACH(*this, i)
		i -> Translate(dx, dy, dz);
}

Dummy* DummyList::Search(unsigned int Type)
{
	iterator i;
	FOR_EACH(*this, i)
		if(i -> Type == Type)
			return &*i;
	return 0;
}

Dummy* DummyList::Search(const char* name)
{
	iterator i;
	FOR_EACH(*this, i)
		if(strstr(i -> name, name) == i -> name)
			return &*i;
	return 0;
}

Dummy& DummyList::Find(unsigned int Type)
{
	Dummy* p = Search(Type);
	if(!p){
		XBuffer buf;
		buf.SetRadix(16);
		buf < "Dummy not found: " <= Type < "\n";
		ErrH.Abort(buf);
		}
	return *p;
}
		
Dummy& DummyList::Find(const char* name)
{
	Dummy* p = Search(name);
	if(!p){
		XBuffer buf;
		buf < "Dummy not found: " < name < "\n";
		ErrH.Abort(buf);
		}
	return *p;
}

struct dummy_comp
{																	       
	bool operator()(const Dummy& d1, const Dummy& d2) const 
	{ 
		return strcmp(d1.name, d2.name) <= 0; 
	}
};

void DummyList::SortByName()
{
	// TODO: @caiiiycuk: not implemeted
	// sort(begin(), end(), dummy_comp());
	abort();
}

DummyList::iterator DummyList::Iterator(const char* name_mask)
{
	iterator i;
	FOR_EACH(*this, i)
		if(strstr(i -> name, name_mask))
			break;
	return i;
}


	
