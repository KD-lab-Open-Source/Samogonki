/******************************************************************
		XTList  - V 1.1
		Copyright 1999 K-D Lab
		Author: Alexandre Kotliar
		

					Шаблонные списки

  Топология списка:
  вперед - разорванный: first() -> next -> ... -> next -> 0
  назад - закольцованный: last() -> (prev == first()) -> prev ->... -> (prev == last()) ...

  При _DEBUG'е на каждую операцию добавления/освобождения включена проверка

  Необходимые элементы:
  struct Type {
	int ID; // не обязателен, если не используешь search()
	Type* next;		
	Type* prev;
	Type(){ next = prev = 0; } // необходимо обнулять
	};

  Использование:
   XTList<Type> list;	
   list.append(new Type);

  Для помещения одного элемента в несколько списков используйте шаблоны
   XTList1<Type> list1;	
   XTList2<Type> list2;	
  и т.д. (пока, правда, до 2). 
  Элемент в этом случае должен содержать проинициализированные
  Type *next1, *prev1, *next2, *prev2 и т.д.

******************************************************************/
#ifndef _XTLIST_
#define _XTLIST_

//#ifdef _DEBUG
//#define test_of_list(str)	test(str)
//#else
#define test_of_list(str)	
//#endif


/******************************************************************
						XTList
******************************************************************/

template <class Type> class XTList {
	int num_elements;
	Type* first_element;
	void test(char* message);
public:

		XTList();
		~XTList();   // clear(), т.е. не удаляет элементы.
	
	int size();		// число элементов
	Type* first();		// первый
	Type* last();		// последний

	void clear();		// удаляет все элементы только из списка
	void delete_all();   // удаляет все элементы из списка и памяти

	void insert(Type* p);		// добавление в начало списка
	void append(Type* p);		// добавление в конец списка
	void insert(Type* pointer,Type* p);	// добавление перед pointer (используется при сортировке)
	void remove(Type* p);		// удаление элемента из списка
	Type* search(int ID);		// поиск (должен быть Type::ID)
};


template <class Type> 
inline void XTList<Type>::test(char* message)
{
	Type* p = first();
	int cnt = 0;
	while(p){
		cnt++;
		p = p -> next;
		}
	if(cnt != num_elements)
		ErrH.Abort(message);
}

template <class Type> 
inline XTList<Type>::XTList()
{
	num_elements = 0; first_element = 0;
}

template <class Type> 
inline XTList<Type>::~XTList()
{
	clear();
}

template <class Type> 
inline void XTList<Type>::clear()
{
	while(first())
		remove(first());
}

template <class Type> 
inline void XTList<Type>::delete_all()
{
	Type* p;
	while((p = first()) != 0){
		remove(p);
		delete p;
		}
}

template <class Type> 
inline int XTList<Type>::size()
{
	return num_elements;
}

template <class Type> 
inline Type* XTList<Type>::first()
{
	return first_element;
}

template <class Type> 
inline Type* XTList<Type>::last()
{
	return first_element ? first_element -> prev : 0;
}

template <class Type> 
inline void XTList<Type>::insert(Type* p)
{
	if(p -> prev)
		ErrH.Abort("XTList: element is already in list");
	num_elements++;
	if(first_element){
		p -> next = first_element;
		p -> prev = first_element -> prev;
		first_element -> prev = p;
		}
	else{
		p -> prev = p;
		p -> next = 0;
		}
	first_element = p;
	test_of_list("XTList: incorrect insert(*)");
}

template <class Type> 
inline void XTList<Type>::insert(Type* pointer,Type* p)
{
	if(!first_element || first_element == pointer){
		insert(p);
		return;
		}
	if(!pointer){
		append(p);
		return;
		}

	if(p -> prev)
		ErrH.Abort("XTList: element is already in list");
	num_elements++;
	p -> next = pointer;
	p -> prev = pointer -> prev;
	pointer -> prev -> next = p;
	pointer -> prev = p;
	test_of_list("XTList: incorrect insert(*,*)");
}


template <class Type> 
inline void XTList<Type>::append(Type* p)
{
	if(p -> prev)
		ErrH.Abort("XTList: element is already in list");
	num_elements++;
	if(first_element){
		p -> next = 0;
		p -> prev = first_element -> prev;
		first_element -> prev -> next = p;
		first_element -> prev = p;
		}
	else{
		p -> next = 0;
		p -> prev = first_element = p;
		}
	test_of_list("XTList: incorrect append(*)");
}

template <class Type> 
inline void XTList<Type>::remove(Type* p)
{
	num_elements--;
	if(p -> next)
		p -> next -> prev = p -> prev;
	else 
		first_element -> prev = p -> prev;

	if(p != first_element)
		p -> prev -> next = p -> next;
	else{
		first_element = p -> next;
		if(first_element)
			first_element -> prev = p -> prev;
		}

	p -> next = p -> prev = 0;
	test_of_list("XTList: incorrect remove");
}

template <class Type> 
inline Type* XTList<Type>::search(int ID)
{
	Type* p = first();
	while(p){
		if(p -> ID == ID)
			return p;
		p = p -> next;
		}
	return 0;
}

/******************************************************************
						XTList1
******************************************************************/
template <class Type> class XTList1 {
	int num_elements;
	Type* first_element;
	void test(char* message);
public:

		XTList1();
		~XTList1();   // clear(), т.е. не удаляет элементы.
	
	int size();		// число элементов
	Type* first();		// первый
	Type* last();		// последний

	void clear();		// удаляет все элементы только из списка
	void delete_all();   // удаляет все элементы из списка и памяти

	void insert(Type* p);		// добавление в начало списка
	void append(Type* p);		// добавление в конец списка
	void insert(Type* pointer,Type* p);	// добавление перед pointer (используется при сортировке)
	void remove(Type* p);		// удаление элемента из списка
	Type* search(int ID);		// поиск (должен быть Type::ID)
};


template <class Type> 
inline void XTList1<Type>::test(char* message)
{
	Type* p = first();
	int cnt = 0;
	while(p){
		cnt++;
		p = p -> next1;
		}
	if(cnt != num_elements)
		ErrH.Abort(message);
}

template <class Type> 
inline XTList1<Type>::XTList1()
{
	num_elements = 0; first_element = 0;
}

template <class Type> 
inline XTList1<Type>::~XTList1()
{
	clear();
}

template <class Type> 
inline void XTList1<Type>::clear()
{
	while(first())
		remove(first());
}

template <class Type> 
inline void XTList1<Type>::delete_all()
{
	Type* p;
	while((p = first()) != 0){
		remove(p);
		delete p;
		}
}

template <class Type> 
inline int XTList1<Type>::size()
{
	return num_elements;
}

template <class Type> 
inline Type* XTList1<Type>::first()
{
	return first_element;
}

template <class Type> 
inline Type* XTList1<Type>::last()
{
	return first_element ? first_element -> prev1 : 0;
}

template <class Type> 
inline void XTList1<Type>::insert(Type* p)
{
	if(p -> prev1)
		ErrH.Abort("XTList1: element is already in list");
	num_elements++;
	if(first_element){
		p -> next1 = first_element;
		p -> prev1 = first_element -> prev1;
		first_element -> prev1 = p;
		}
	else{
		p -> prev1 = p;
		p -> next1 = 0;
		}
	first_element = p;
	test_of_list("XTList1: incorrect insert(*)");
}

template <class Type> 
inline void XTList1<Type>::insert(Type* pointer,Type* p)
{
	if(!first_element || first_element == pointer){
		insert(p);
		return;
		}
	if(!pointer){
		append(p);
		return;
		}

	if(p -> prev1)
		ErrH.Abort("XTList1: element is already in list");
	num_elements++;
	p -> next1 = pointer;
	p -> prev1 = pointer -> prev1;
	pointer -> prev1 -> next1 = p;
	pointer -> prev1 = p;
	test_of_list("XTList1: incorrect insert(*,*)");
}


template <class Type> 
inline void XTList1<Type>::append(Type* p)
{
	if(p -> prev1)
		ErrH.Abort("XTList1: element is already in list");
	num_elements++;
	if(first_element){
		p -> next1 = 0;
		p -> prev1 = first_element -> prev1;
		first_element -> prev1 -> next1 = p;
		first_element -> prev1 = p;
		}
	else{
		p -> next1 = 0;
		p -> prev1 = first_element = p;
		}
	test_of_list("XTList1: incorrect append(*)");
}

template <class Type> 
inline void XTList1<Type>::remove(Type* p)
{
	num_elements--;
	if(p -> next1)
		p -> next1 -> prev1 = p -> prev1;
	else 
		first_element -> prev1 = p -> prev1;

	if(p != first_element)
		p -> prev1 -> next1 = p -> next1;
	else{
		first_element = p -> next1;
		if(first_element)
			first_element -> prev1 = p -> prev1;
		}

	p -> next1 = p -> prev1 = 0;
	test_of_list("XTList1: incorrect remove");
}

template <class Type> 
inline Type* XTList1<Type>::search(int ID)
{
	Type* p = first();
	while(p){
		if(p -> ID == ID)
			return p;
		p = p -> next1;
		}
	return 0;
}


/******************************************************************
						XTList2
******************************************************************/
template <class Type> class XTList2 {
	int num_elements;
	Type* first_element;
	void test(char* message);
public:

		XTList2();
		~XTList2();   // clear(), т.е. не удаляет элементы.
	
	int size();		// число элементов
	Type* first();		// первый
	Type* last();		// последний

	void clear();		// удаляет все элементы только из списка
	void delete_all();   // удаляет все элементы из списка и памяти

	void insert(Type* p);		// добавление в начало списка
	void append(Type* p);		// добавление в конец списка
	void insert(Type* pointer,Type* p);	// добавление перед pointer (используется при сортировке)
	void remove(Type* p);		// удаление элемента из списка
	Type* search(int ID);		// поиск (должен быть Type::ID)
};


template <class Type> 
inline void XTList2<Type>::test(char* message)
{
	Type* p = first();
	int cnt = 0;
	while(p){
		cnt++;
		p = p -> next2;
		}
	if(cnt != num_elements)
		ErrH.Abort(message);
}

template <class Type> 
inline XTList2<Type>::XTList2()
{
	num_elements = 0; first_element = 0;
}

template <class Type> 
inline XTList2<Type>::~XTList2()
{
	clear();
}

template <class Type> 
inline void XTList2<Type>::clear()
{
	while(first())
		remove(first());
}

template <class Type> 
inline void XTList2<Type>::delete_all()
{
	Type* p;
	while((p = first()) != 0){
		remove(p);
		delete p;
		}
}

template <class Type> 
inline int XTList2<Type>::size()
{
	return num_elements;
}

template <class Type> 
inline Type* XTList2<Type>::first()
{
	return first_element;
}

template <class Type> 
inline Type* XTList2<Type>::last()
{
	return first_element ? first_element -> prev2 : 0;
}

template <class Type> 
inline void XTList2<Type>::insert(Type* p)
{
	if(p -> prev2)
		ErrH.Abort("XTList2: element is already in list");
	num_elements++;
	if(first_element){
		p -> next2 = first_element;
		p -> prev2 = first_element -> prev2;
		first_element -> prev2 = p;
		}
	else{
		p -> prev2 = p;
		p -> next2 = 0;
		}
	first_element = p;
	test_of_list("XTList2: incorrect insert(*)");
}

template <class Type> 
inline void XTList2<Type>::insert(Type* pointer,Type* p)
{
	if(!first_element || first_element == pointer){
		insert(p);
		return;
		}
	if(!pointer){
		append(p);
		return;
		}

	if(p -> prev2)
		ErrH.Abort("XTList2: element is already in list");
	num_elements++;
	p -> next2 = pointer;
	p -> prev2 = pointer -> prev2;
	pointer -> prev2 -> next2 = p;
	pointer -> prev2 = p;
	test_of_list("XTList2: incorrect insert(*,*)");
}


template <class Type> 
inline void XTList2<Type>::append(Type* p)
{
	if(p -> prev2)
		ErrH.Abort("XTList2: element is already in list");
	num_elements++;
	if(first_element){
		p -> next2 = 0;
		p -> prev2 = first_element -> prev2;
		first_element -> prev2 -> next2 = p;
		first_element -> prev2 = p;
		}
	else{
		p -> next2 = 0;
		p -> prev2 = first_element = p;
		}
	test_of_list("XTList2: incorrect append(*)");
}

template <class Type> 
inline void XTList2<Type>::remove(Type* p)
{
	num_elements--;
	if(p -> next2)
		p -> next2 -> prev2 = p -> prev2;
	else 
		first_element -> prev2 = p -> prev2;

	if(p != first_element)
		p -> prev2 -> next2 = p -> next2;
	else{
		first_element = p -> next2;
		if(first_element)
			first_element -> prev2 = p -> prev2;
		}

	p -> next2 = p -> prev2 = 0;
	test_of_list("XTList2: incorrect remove");
}

template <class Type> 
inline Type* XTList2<Type>::search(int ID)
{
	Type* p = first();
	while(p){
		if(p -> ID == ID)
			return p;
		p = p -> next2;
		}
	return 0;
}

#endif // _XTLIST_