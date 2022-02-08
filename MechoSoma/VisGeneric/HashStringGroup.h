/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Шаблон для отображения  строка -> Type.
//  Группировка производится по цифрам от начала строки:
//	aaa == aaa1 == aaa2 != Aaa 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HASH_STRING_GROUP__
#define __HASH_STRING_GROUP__

#include <unordered_map>
#include <cctype>
#include <cstring>
#include <cassert>

template <class T>
struct HashStringGroup :  std::unordered_map<std::string, T>
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
