#ifndef __BODY_FACTORY__
#define __BODY_FACTORY__

class Body;

class GeneralBodyCreator
{
	int size;
public:
	int sizeOf(){ return size; }
	virtual Body* create() = 0;

	GeneralBodyCreator(int size_) : size(size_) {}
	virtual ~GeneralBodyCreator() {}
};


struct eqstr
{																	       
	bool operator()(const char* s1, const char* s2) const { return strcmp(s1, s2) == 0; }
};

class BodyFactory : std::unordered_map<const char*, PtrHandle<GeneralBodyCreator>, hash<const char*>, eqstr>
{
	typedef std::unordered_map<const char*, PtrHandle<GeneralBodyCreator>, hash<const char*>, eqstr> HashMap;
public:
	BodyFactory();
	void add(const char* type_name, GeneralBodyCreator* creator);
	Body* operator()(const char* type_name);
	int sizeOf(const char* type_name);
};


#endif  // __BODY_FACTORY__
