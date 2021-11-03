/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Шаблон для отображения  строка -> Type.
//  Группировка производится по цифрам от начала строки:
//	aaa == aaa1 == aaa2 != Aaa 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HASH_STRING_GROUP__
#define __HASH_STRING_GROUP__

#include <unordered_map>
#include <ctype.h>

struct eqstr
{																	       
bool operator()(const char* s1, const char* s2)const{ return !strcmp(s1, s2); }
};

template <class T>
struct HashStringGroup :  std::unordered_map<char*, T, std::hash<const char*>, eqstr>
{
	void add(char* str, const T& data) 
	{ (*this)[str] = data; }

	T* look(char* n)
	{
		char name[256];
		assert(strlen(n) + 1 < 256);
		strcpy(name, n);
		for(char* i = name; *i; i++)
			if(isdigit(*i)){
				*i = 0;
				break;
				}
	
		auto it = this->find(name);
		return it == this->end() ? 0 : &(*it).second;
	}
};

#endif // __HASH_STRING_GROUP__
