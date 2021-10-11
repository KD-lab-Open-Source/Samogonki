//** 1999 Creator - Balmer **//
#pragma once

#include <stdlib.h> 
#include <string.h>

template<class T>
class array
{
protected:
	T* x;
	int size;
	int maxsize;
	int addsize;
public:
	explicit array(int _addsize=5);
	~array();
	void SetAddSize(int add){addsize=add;}

	inline T& operator [](int i);
	inline T& top()
	{
		ASSERT(size>0);
		return x[size-1];
	}

	inline void add(T t);
	inline void del();
	inline void delAt(int at);
	inline void addAt(T t,int at);
	virtual void removeAll();
	inline int GetSize() const {return size;}

	inline void push(T& t){add(t);}
	inline void pop(T& t){t=top();del();}
	inline bool empty(){return size<=0;}

	inline void SetSize(int n);
};

template<class T>
class parray:public array<T*>
{
protected:
	bool destroy;
public:
	explicit parray(int _addsize=50,bool _destroy=false);
	~parray();
	void setDestroy(bool d){destroy=d;}
	inline void removeAll();
	inline void delAt(int at);
	template<class Y> inline int find(Y& y)
	{
		for(int i=0;i<size;i++)
		{
			if(*x[i]==y)
				return i;
		}
		return -1;
	}

	inline bool remove(T* y)
	{
		for(int i=0;i<size;i++)
		{
			if(x[i]==y)
			{
				delAt(i);
				return true;
			}
		}
		return false;
	}
};


template<class T>
array<T>::array(int _addsize)
{
	x=NULL;
	size=maxsize=0;
	addsize=_addsize;
	ASSERT(addsize>0);
}

template<class T>
inline void array<T>::removeAll()
{
	if(x)
	{
		free(x);
	}
	x=NULL;
	size=maxsize=0;
}

template<class T>
array<T>::~array()
{
	removeAll();	
}
template<class T>
inline T& array<T>::operator [](int i)
{
	ASSERT(i<size && i>=0);
	return x[i];
}

template<class T>
inline void array<T>::add(T t)
{
	size++;
	if(size>maxsize)
	{
		maxsize+=addsize;
		int sz=maxsize*sizeof(T);
		x=(T*)(x?realloc(x,sz):malloc(sz));
	}

	x[size-1]=t;
}

template<class T>
inline void array<T>::SetSize(int n)
{
	maxsize=size=n;
	int sz=maxsize*sizeof(T);
	x=(T*)(x?realloc(x,sz):malloc(sz));
}

template<class T>
inline void array<T>::del()
{
	ASSERT(size>0);
	size--;
}

template<class T>
inline void array<T>::delAt(int at)
{
	ASSERT(size>0);
	ASSERT(0<=at && at<size);
	size--;
	for(int i=at;i<size;i++)
	{
		x[i]=x[i+1];
	}
}

template<class T>
inline void array<T>::addAt(T t,int at)
{
	ASSERT(0<=at && at<=size);
	size++;
	if(size>maxsize)
	{
		maxsize+=addsize;
		int sz=maxsize*sizeof(T);
		x=(T*)(x?realloc(x,sz):malloc(sz));
	}

	for(int i=size-1;i>at;i--)
		x[i]=x[i-1];

	x[at]=t;
}

////////////////////////////////
template<class T> parray<T>::
parray(int _addsize,bool _destroy)
:array<T*>(_addsize)
{
	destroy=_destroy;
}

template<class T> parray<T>::
~parray()
{
	removeAll();
}

template<class T> 
inline void parray<T>::removeAll()
{
	if(destroy)
	{
		for(int i=0;i<size;i++)
		{
			delete x[i];
		}
	}

	array<T*>::removeAll();
}

template<class T> 
inline void parray<T>::delAt(int at)
{
	ASSERT(0<=at && at<size);

	T* p=x[at];

	array<T*>::delAt(at);
	if(destroy)
		delete p;
}

typedef const char* LPCSTR;
struct string
{
protected:
	char* name;
public:
	inline string():name(NULL){}
	inline string(int n){name=new char[n+1];};
	inline string(LPCSTR p):name(NULL){*this=p;}
	inline string(const string& p):name(NULL){*this=p.name;}

	inline string(array<char>& in);

	inline ~string(){delete(name);}
	inline operator LPSTR() const {return name;}
	inline bool IsEmpty() const { return name==NULL || name[0]==0;}

	inline bool operator ==(LPCSTR pc) const;
	inline void operator =(const char* p);
	inline void operator =(const string& p){*this=p.name;}
	inline void operator =(array<char>& in);
	inline void operator +=(LPCSTR ps);
};


inline string::string(array<char>& in)
{
	name=new char[in.GetSize()+1];
	if(in.GetSize()>0)
		memcpy(name,&in[0],in.GetSize());
	name[in.GetSize()]=0;
}

inline void string::operator =(array<char>& in)
{
	delete name;
	name=NULL;
	if(in.GetSize()>0)
	{
		name=new char[in.GetSize()+1];
		if(in.GetSize()>0)
			memcpy(name,&in[0],in.GetSize());
		name[in.GetSize()]=0;
	}
}

inline bool string::operator ==(LPCSTR pc) const
{
	char* p=name;
	
	do{
		if(*p!=*pc)
			return false;
		p++;
		pc++;
	}while(*p && *pc);

	return *p==0 && *pc==0;
}

inline void string::operator =(const char* p)
{
	delete name;
	name=NULL;
	if(p)
	{
		int len=strlen(p);
		name=new char[len+1];
		strcpy(name,p);
	}
}

inline void string::operator +=(const char* ps)
{
	if(ps==NULL)return;
	if(name==NULL)
	{
		*this=ps;
		return;
	}

	int l=strlen(name),ls=strlen(ps);

	char* p=new char[l+ls+1];
	strcpy(p,name);
	strcpy(p+l,ps);

	delete name;
	name=p;
}
