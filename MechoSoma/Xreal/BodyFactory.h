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



class BodyFactory : std::unordered_map<std::string, PtrHandle<GeneralBodyCreator>>
{
	typedef std::unordered_map<std::string, PtrHandle<GeneralBodyCreator>> HashMap;
public:
	BodyFactory();
	void add(const char* type_name, GeneralBodyCreator* creator);
	Body* operator()(const char* type_name);
	int sizeOf(const char* type_name);
};


#endif  // __BODY_FACTORY__
