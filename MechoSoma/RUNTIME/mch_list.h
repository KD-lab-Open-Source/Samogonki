
#ifndef __MCH_LIST_H__
#define __MCH_LIST_H__

#define _TEST_LIST_

template <class Type> class mchList
{
	int numElements;
	Type* firstElement;
	void test(int code);

public:

	mchList();
	~mchList();

	int size();
	Type* first();
	Type* last();

	void clear();
	void delete_all();

	void insert(Type* p);
	void append(Type* p);
	void insert(Type* pointer,Type* p);
	void append(Type* pointer,Type* p);
	void remove(Type* p);
	Type* search(int ID);
};

template <class Type>
inline void mchList<Type>::test(int code)
{
#ifdef _TEST_LIST_
	Type* p = first();
	int cnt = 0;
	while(p){
		cnt++;
		p = p -> next;
		}
	if(cnt != numElements)
		ErrH.Abort("List",XERR_USER,code);
#endif
}

template <class Type>
inline mchList<Type>::mchList()
{
	numElements = 0; firstElement = 0;
}

template <class Type>
inline mchList<Type>::~mchList()
{
	clear();
}

template <class Type>
inline void mchList<Type>::clear()
{
	while(first())
		remove(first());
}

template <class Type>
inline void mchList<Type>::delete_all()
{
	Type* p;
	while((p = first()) != 0){
		remove(p);
		delete p;
		}
}

template <class Type>
inline int mchList<Type>::size()
{
	return numElements;
}

template <class Type>
inline Type* mchList<Type>::first()
{
	return firstElement;
}

template <class Type>
inline Type* mchList<Type>::last()
{
	return firstElement ? firstElement -> prev : 0;
}

template <class Type>
inline void mchList<Type>::insert(Type* p)
{
	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	if(firstElement){
		p -> next = firstElement;
		p -> prev = firstElement -> prev;
		firstElement -> prev = p;
		}
	else{
		p -> prev = p;
		p -> next = 0;
		}
	firstElement = p;
	p -> list = this;
	test(0);
}

template <class Type>
inline void mchList<Type>::insert(Type* pointer,Type* p)
{
	if(!firstElement || firstElement == pointer){
		insert(p);
		return;
		}
	if(!pointer){
		append(p);
		return;
		}

	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	p -> next = pointer;
	p -> prev = pointer -> prev;
	pointer -> prev -> next = p;
	pointer -> prev = p;
	p -> list = this;
	test(5);
}


template <class Type>
inline void mchList<Type>::append(Type* p)
{
	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	if(firstElement){
		p -> next = 0;
		p -> prev = firstElement -> prev;
		firstElement -> prev -> next = p;
		firstElement -> prev = p;
		}
	else{
		p -> next = 0;
		p -> prev = firstElement = p;
		}
	p -> list = this;
	test(1);
}

template <class Type>
inline void mchList<Type>::remove(Type* p)
{
	if(p -> list != this)
		ErrH.Abort("Removed element isn't in list");
	numElements--;
	if(p -> next)
		p -> next -> prev = p -> prev;
	else
		firstElement -> prev = p -> prev;

	if(p != firstElement)
		p -> prev -> next = p -> next;
	else{
		firstElement = p -> next;
		if(firstElement)
			firstElement -> prev = p -> prev;
		}

	p -> next = p -> prev = 0;
	p -> list = 0;
	test(2);
}

template <class Type>
inline Type* mchList<Type>::search(int ID)
{
	Type* p = first();
	while(p){
		if(p -> ID == ID)
			return p;
		p = p -> next;
		}
	return 0;
}

#endif // __MCH_LIST_H__
