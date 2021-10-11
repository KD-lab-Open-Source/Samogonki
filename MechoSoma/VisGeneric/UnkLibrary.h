#ifndef __UNKLIBRARY_H__
#define __UNKLIBRARY_H__

#include "unknown.h"
#include "BaseDefine.h"

typedef cBaseList <cUnknownClass> cUnknownClassList;
typedef cBaseDynArrayPointer <cUnknownClass> cUnknownDynArrayPointer;
typedef cBaseLibrary <cUnknownClass,cUnknownClassList> cUnknownClassLibrary;

class cUnknownLibrary : public cUnknownClass, public cUnknownClassLibrary
{	// удалять бибилиотеку можно только посло вызова Release()
public:
	cUnknownLibrary(int kind,int type=TYPE_NULL):cUnknownClass(kind,type)	{ }
	~cUnknownLibrary()											
	{ 
		cUnknownClass::Release(); 
		UNKNOWN_ASSERT(BaseList==0); 
	}
	inline void Attach(cUnknownClass *UnkClass)
	{
		UnkClass->IncRef();	// указатель хранится и в библиотеке
		cUnknownClassLibrary::Attach(UnkClass);
	}
/*
	inline void Release()
	{
		for(cUnknownClassList *tmp=BaseList,*start=BaseList;tmp;tmp=start)
		{
			start=start->next;
			if(tmp->Base->GetRef()==1) // на данный объект имеется единственная ссылка tmp->Base
			{
				DeleteBase(tmp->Base);
				tmp->Base=0;
				Detach(tmp);
			}
		}
	}
private:
	virtual void DeleteBase(cUnknownClass *UnknownClass)		{ UNKNOWN_ASSERT(UnknownClass==0); }
*/
};

class cUnkClassDynArrayPointer : public cUnknownClass, public cBaseDynArrayPointer <cUnknownClass>
{ // динамический массив указателей на cUnknownClass
public:
	cUnkClassDynArrayPointer(int kind,int type=TYPE_NULL):cUnknownClass(kind,type)		{ }
	~cUnkClassDynArrayPointer()										{ UNKNOWN_DESTRUCTOR; }
};

#endif //__UNKLIBRARY_H__