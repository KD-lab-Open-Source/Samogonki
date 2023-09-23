#ifndef __BASECLASS_H__
#define __BASECLASS_H__

#include "string.h"
#include "assert.h"

#ifndef __BASE_LIST__
#define __BASE_LIST__

template <class cBase> class cBaseList
{	// двусвязанный список
//	friend class cBase;
public:
	cBase		*Base;
	cBaseList	*next;
	cBaseList	*prev;
	
	cBaseList(cBase *BaseInit=0)								{ Base=BaseInit; next=prev=0; }
	~cBaseList()												{ Detach(); Base=0; }

	inline cBaseList* Attach(cBaseList *ListBase)
	{
		cBaseList *Next=next;
		ListBase->next=Next;
		ListBase->prev=this;
		next=ListBase;
		if(Next) Next->prev=ListBase;
		return ListBase;
	}
	inline cBaseList* Detach()
	{
		if(prev) prev->next=next;
		if(next) next->prev=prev;
		next=prev=0;
		return this;
	}
	inline cBaseList* operator ++ ()							{ return next; }
};

#endif //__BASE_LIST__

#ifndef __BASE_LIBRARY__
#define __BASE_LIBRARY__

template <class cBase,class cBaseList> class cBaseLibrary
{
public:
	cBaseList		*BaseList;				

	cBaseLibrary()												{ BaseList=0; }
	~cBaseLibrary()												{ }
	
	inline cBaseList* Attach(cBase *Base)	
	{ // присоединие объекта к списку 
		if(BaseList==0) return BaseList=new cBaseList(Base);
		return BaseList->Attach(new cBaseList(Base));
	}
	inline cBaseList* AttachEnd(cBase *Base)	
	{ // присоединие объекта к списку 
		if(BaseList==0) return BaseList=new cBaseList(Base);
		cBaseList *start = nullptr;
		for(start=BaseList; start->next; start=start->next);
		cBaseList *List=new cBaseList(Base);
		return List->Attach(start);
	}
	inline void Detach(cBase *Base)		
	{ // отсоединение объекта от списка
		cBaseList *start=BaseList;
		for(start; start; start=start->next)
			if(start->Base==Base) break;
		if(start) 
		{
			if(start==BaseList) BaseList=start->next; 
			delete start; 
		}
	}
	void Detach(cBaseList *List)
	{
		assert(List);
		if(List==BaseList) BaseList=BaseList->next; 
		delete List; 
	}
	inline void Delete(cBase *Base)		// удаление объекта в списке
	{
		Detach(Base);
		if(Base) delete Base; 
	}
	inline cBaseList* First()										{ return BaseList; }
};

#endif //__BASE_LIBRARY__

#ifndef __BASE_DISPATCHER__
#define __BASE_DISPATCHER__

template <class cBase,class cBaseList> class cBaseDispatcher : public cBaseLibrary <cBase,cBaseList>
{
public:
	unsigned int	NumberID;			// ID последнего объекта = NumberID

	cBaseDispatcher()											{ NumberID=0; }
	~cBaseDispatcher()											{ assert(this->BaseList==0); }
	inline cBase* FindID(unsigned int ID)
	{
		cBaseList *start = nullptr;
		for(start=this->BaseList; start; start=start->next)
			if(start->Base->ID==ID) return start->Base;
		return 0;
	}
};

#endif //__BASE_DISPATCHER__

#ifndef __BASE_STACK__
#define __BASE_STACK__

template <class cBase,class cBaseList> class cBaseStack
{
public:
	cBaseList		*BaseList;			

	cBaseStack()												{ BaseList=0; }
	~cBaseStack()												{ assert(BaseList==0); }
	
	inline cBaseList* Push(cBase *Base)	// присоединие объекта к списку 
	{
		if(BaseList==0) return BaseList=new cBaseList(Base);
		return BaseList->Attach(new cBaseList(Base));
	}
	inline void Pop()					// удаление объекта в списке
	{
		cBaseList *tmp=BaseList;
		if(tmp) 
		{
			BaseList=tmp->next; 
			delete tmp; 
		}
	}
};

#endif // __BASE_STACK__

#ifndef __BASE_QUEUE__
#define __BASE_QUEUE__

template <class cBase,class cBaseList> class cBaseQueue
{
public:
	cBaseList		*BaseList;			// первый элемент - NULL объект
	cBaseList		*Window;

	cBaseQueue()												{ BaseList=0; First(); }
	~cBaseQueue()												{ assert(BaseList==0); }
	
	inline cBaseList* Attach(cBase *Base)	// присоединие объекта к списку 
	{
		if(BaseList==0) return BaseList=new cBaseList(Base);
		return BaseList->Attach(new cBaseList(Base));
	}
	inline void Delete(cBase *Base=0)		// удаление объекта в списке
	{
		if(Base==0)
		{
			cBaseList *start = nullptr;
			for(start=BaseList; start->next; start=start->next)
			if(start==0) return;
			if(start==Window) Window=start->next;
			if(start->Base) delete start->Base;
			delete start;
		}
		Detach(Base);
		delete Base; 
	}
	inline void Detach(cBase *Base)		// отсоединение объекта от списка
	{
		cBaseList *start = nullptr;
		for(start=BaseList; start; start=start->next)
			if(start->Base==Base) break;
		if(start) { if(start==Window) Window=start->next; delete start; }
	}
	inline cBase* First()										{ if((Window=BaseList)) return Window->Base; return 0; }
	inline cBase* Prev()										{ if((Window)&&(Window=Window->prev)) return Window->Base; return 0; }
	inline cBase* Next()										{ if((Window)&&(Window=Window->next)) return Window->Base; return 0; }
};

#endif //__BASE_QUEUE__

#ifndef __BASE_BUFFER__
#define __BASE_BUFFER__

template <class cBase> class cBaseBuffer
{
public: 
	cBase	*Base;
	int		CurrentPos,CurrentSize,dSize,MaxSize,EndBuffer;

	cBaseBuffer(int MaxLength=1000,int MulSize=2)	{ Base=0; free(MaxLength,MulSize); }
	~cBaseBuffer()									{ if(Base) delete [] Base; Base=0; }
	inline void write(void *buf,int size=1)
	{
		if(CurrentSize>=MaxSize) Resize(MaxSize,MaxSize+dSize*size);
		memcpy(Base+CurrentSize,buf,size*sizeof(cBase));
		CurrentSize+=size; 
	}
	inline int read(void *buf,int size=1)
	{
		if(CurrentPos+size>MaxSize) { EndBuffer=1; return 0; }
		memcpy(buf,Base+CurrentPos,size*sizeof(cBase));
		CurrentPos+=size;
		return size;
	}
	inline int  getpos()							{ return CurrentPos; }
	inline int  setpos(int pos)						{ return (CurrentPos=pos); }
	inline int  length()							{ return CurrentSize; }
	inline int	size()								{ return length()*sizeof(cBase); }
	inline int  eof()								{ return EndBuffer; }
	inline void Pack()								{ if(CurrentSize<MaxSize) Resize(CurrentSize,CurrentSize); }
	inline cBase* find(int (*cmp)(cBase&,cBase&),cBase &base)	
	{ 
		int i=-1; 
		while(++i<CurrentSize) 
			if(cmp(Base[i],base)) 
				return &Base[i]; 
			return 0; 
	}
	void free(int MaxLength=1000,int MulSize=2)		
	{ 
		dSize=MulSize;
		if(Base) delete Base;
		Base=new cBase[MaxSize=MaxLength];
		Reset();		
	}
	inline cBaseBuffer* operator << (cBase *base)	{ write(base); return this; }
	inline cBaseBuffer* operator >> (cBase *base)	{ read(base);  return this; }
	inline cBaseBuffer* operator << (cBase &base)	{ write(&base); return this;}
	inline cBaseBuffer* operator >> (cBase &base)	{ read(&base); return this; }
	inline cBase& operator [] (int number)			{ if(number<CurrentSize) return Base[number]; return Base[CurrentSize-1]; }
private:
	inline void Resize(int OldSize,int NewSize)							
	{
		cBase *tmp=new cBase[MaxSize=NewSize];
		memcpy(tmp,Base,sizeof(cBase)*OldSize);
		delete [] Base;
		Base=tmp;
	}
	inline void Reset()						{ CurrentSize=0; CurrentPos=0; EndBuffer=0; }
};

#endif //__BASE_BUFFER__

#ifndef __BASE_ARRAY__
#define __BASE_ARRAY__

template <class cBase> class cBaseArray
{
public: 
	cBase	*Base;
	int		CurrentSize,MaxSize,dSize;

	cBaseArray()									{ Base=0; CurrentSize=MaxSize=0; dSize=2; }
	cBaseArray(cBaseArray *Array)					{ Base=0; SetArray(Array); }
	cBaseArray(int MaxLength,int MulSize)			{ Base=0; New(MaxLength); CurrentSize=0; dSize=MulSize; }
	~cBaseArray()									{ Delete(); }

	inline void New(int NewSize)					{ Delete(); if((MaxSize=NewSize)!=0) Base=new cBase[MaxSize]; }
	inline void Delete()							{ if(Base) delete [] Base; Base=0; MaxSize=CurrentSize=0; }
	inline void Clear()								{ CurrentSize=0; }
	inline int& length()							{ return CurrentSize; }
	inline void Pack()								{ if(CurrentSize<MaxSize) Resize(CurrentSize,CurrentSize); }
	inline cBase& operator [] (int number)			{
#ifdef _DEBUG
          assert(number<MaxSize);
#endif
          return Base[number];
        }
	inline cBase& operator () (int number)			{ if(number>=MaxSize) Resize(MaxSize,number+dSize); if(number>=CurrentSize) CurrentSize=number+1; return Base[number]; }
	inline void SetArray(cBaseArray *Array)			{ Delete(); dSize=Array->dSize;	Base=new cBase[MaxSize=Array->MaxSize];	for(CurrentSize=0;CurrentSize<Array->CurrentSize;CurrentSize++) Base[CurrentSize]=Array->Base[CurrentSize]; }
	inline int AddBase(cBase *base,char test=0);

private:
	inline void Resize(int OldSize,int NewSize)							
	{
		if(NewSize==0) { Delete(); return; }
		cBase *tmp=new cBase[MaxSize=NewSize];
		for(int i=0;i<OldSize;i++) tmp[i]=Base[i];
		if(Base) delete [] Base;
		Base=tmp;
	}
};
template <class cBase> inline int cBaseArray<cBase>::AddBase(cBase *base,char test)
{
	if(test) for(int i=0;i<length();i++) if(memcmp(&Base[i],base,sizeof(cBase))==0) return i;
	if(CurrentSize>=MaxSize) Resize(MaxSize,MaxSize+dSize);
	Base[CurrentSize]=*base;
	return CurrentSize++; 
}

#endif //__BASE_ARRAY__

#ifndef __BASE_POINTER_ARRAY__
#define __BASE_POINTER_ARRAY__

template <class cBase> class cBaseArrayPointer
{
public: 
	cBase*	*Base;
	int		CurrentSize,MaxSize,dSize;

	cBaseArrayPointer()								{ Base=0; CurrentSize=MaxSize=0; dSize=1; }
	cBaseArrayPointer(cBaseArrayPointer *Array)		{ Base=0; SetArray(Array); }
	cBaseArrayPointer(int MaxLength,int AddSize=1)	{ Base=0; New(MaxLength); CurrentSize=0; dSize=AddSize; }
	~cBaseArrayPointer()							{ Delete(); }

	inline void New(int NewSize)					{ Delete(); if((MaxSize=NewSize)!=0) { Base=new cBase*[MaxSize]; memset(Base,0,MaxSize*sizeof(cBase*)); } }
	inline void Delete()							{ if(Base) { for(int i=0;i<length();i++) if(Base[i]) delete Base[i]; delete [] Base; Base=0; } MaxSize=CurrentSize=0; }
	inline void Clear()								{ CurrentSize=0; }
	inline int& length()							{ return CurrentSize; }
	inline void Pack()								{ if(CurrentSize<MaxSize) Resize(CurrentSize,CurrentSize); }
	inline cBase*& operator [] (int number)			{ /*assert(number<MaxSize);*/ return Base[number]; }
	inline cBase*& operator () (int number)			{ if(number>=MaxSize) Resize(MaxSize,number+dSize); if(number>=CurrentSize) CurrentSize=number+1; return Base[number]; }
	inline void SetArray(cBaseArrayPointer *Array)	{ Delete(); dSize=Array->dSize;	Base=new cBase*[MaxSize=Array->MaxSize]; memcpy(Base,Array->Base,sizeof(cBase*)*(CurrentSize=Array->CurrentSize)); }
	inline int AddBase(cBase *base,char test=1);
	inline void Attach(cBase *base)					{ if(CurrentSize>=MaxSize) Resize(MaxSize,MaxSize+dSize); Base[CurrentSize++]=base;	}
	inline void Detach(cBase *base)					
	{ 
		int number;
		for(number=0;number<length();number++)
			if(Base[number]==base) 
				break;
		if(number>=length()) return;
		length()--;
		memcpy(&Base[number],&Base[number+1],(length()-number)*sizeof(cBase*));
	}

private:
	inline void Resize(int OldSize,int NewSize)							
	{
		if(NewSize==0) { Delete(); return; }
		cBase* *tmp=new cBase*[MaxSize=NewSize];
		memcpy(tmp,Base,sizeof(cBase*)*OldSize);
		if(Base) delete [] Base;
		Base=tmp;
	}
};
template <class cBase> inline int cBaseArrayPointer<cBase>::AddBase(cBase *base,char test)
{
	if(test) for(int i=0;i<length();i++) if(Base[i]==base) return i;
	if(CurrentSize>=MaxSize) Resize(MaxSize,MaxSize+dSize);
	Base[CurrentSize]=base;
	return CurrentSize++; 
}

template <class cBase> class cBaseArrayManager : public cBaseArrayPointer <cBase>
{
public:
	cBaseArrayManager()										{ }
	cBaseArrayManager(int MaxLength,int AddSize=1):cBaseArrayPointer<cBase>(MaxLength,AddSize)	{ }
	~cBaseArrayManager()									{ assert(this->length()==0); }
	inline int Attach(cBase *base)							
	{ 
		for(int i=0;i<this->length();i++)
			if(this->Base[i]==0) 
			{
				this->Base[i]=base;
				return i;
			}
		cBaseArrayPointer<cBase>::Attach(base);
		return this->length()-1;
	}
	inline void Detach(cBase *base)					
	{ 
		for(int i=0;i<this->length();i++)
			if(this->Base[i]==base) 
			{
				this->Base[i]=0;
				break;
			}
	}
};

#endif //__BASE_POINTER_ARRAY__

#ifndef __BASE_DYNARRAY__
#define __BASE_DYNARRAY__

#define BASEDYNARRAY_DSIZE								1

template <class cBase> class cBaseDynArray
{
public: 
	cBase	*Base;
	int		size;

	cBaseDynArray()									{ Base=0; size=0; }
	cBaseDynArray(cBaseDynArray *Array)				{ Base=0; size=0; *this=*Array; }
	cBaseDynArray(int MaxLength)					{ Base=0; size=0; New(MaxLength); }
	~cBaseDynArray()								{ Delete(); }

	inline void New(int NewSize)					
	{ 
		Delete(); 
		if((size=NewSize)!=0) Base=new cBase[size]; 
	}
	inline void Delete()							{ if(Base) delete [] Base; Base=0; size=0; }
	inline int& length()							{ return size; }
	inline void Pack(int NewSize)					{ Resize(NewSize); }
	inline cBase& operator [] (int number)			{ assert(number<size); return Base[number]; }
	inline cBase& operator () (int number)			{ if(number>=size) Resize(number+BASEDYNARRAY_DSIZE); return Base[number]; }
	inline void Clear()								{ memset(Base,0,size*sizeof(cBase)); }
	inline cBaseDynArray& operator = (const cBaseDynArray &Array)
	{
		New(Array.size);
		for(cBase *bNew=Base,*eNew=&Base[length()],*bOld=Array.Base;bNew<eNew;bNew++,bOld++)
			*bNew=*bOld;
		return *this;
	}
	inline cBase* Find(cBase *base,int (*fcmp)(cBase *,cBase *))
	{
		for(cBase *b=Base,*e=&Base[length()];b<e;b++)
			if(fcmp(base,b))
				return b;
		return 0;
	}
	inline void Resize(int NewSize)							
	{
		if(NewSize==0) { Delete(); return; }
		if(size==NewSize) return;
		int OldSize=size < NewSize ? size : NewSize;
		cBase *tmp=Base; Base=new cBase[size=NewSize];
		for(int i=0;i<OldSize;i++)
			Base[i]=tmp[i];
		if(tmp) delete [] tmp;
	}
};

#endif //__BASE_DYNARRAY__

#ifndef __BASE_DYNARRAYPOINTER__
#define __BASE_DYNARRAYPOINTER__

template <class cBase> class cBaseDynArrayPointer
{
public: 
	cBase	**Base;
	int		size;

	cBaseDynArrayPointer()							{ memset(this,0,sizeof(cBaseDynArrayPointer)); }
	cBaseDynArrayPointer(cBaseDynArrayPointer *Array){ memset(this,0,sizeof(cBaseDynArrayPointer)); *this=*Array; }
	cBaseDynArrayPointer(int MaxLength)				{ memset(this,0,sizeof(cBaseDynArrayPointer)); New(MaxLength); }
	~cBaseDynArrayPointer()							{ Delete(); }

	inline void New(int NewSize)					
	{ 
		Delete(); 
		if((size=NewSize)!=0) Base=new cBase*[size]; 
	}
	inline void Delete()							
	{ 
		for(int i=0;i<length();i++)
			if(Base[i]) 
				delete Base[i];
		if(Base) delete[] Base;
		Base=0; size=0;
//		memset(this,0,sizeof(cBaseDynArrayPointer)); 
	}
	inline int& length()							{ return size; }
	inline void Pack(int NewSize)					{ Resize(NewSize); }
	inline cBase*& operator [] (int number)			{ assert(number<size); return Base[number]; }
	inline cBase*& operator () (int number)			{ if(number>=size) Resize(number+BASEDYNARRAY_DSIZE); return Base[number]; }
	inline void Clear()								{ for(int i=0;i<length();i++) Base[i]=0; }
	inline cBaseDynArrayPointer& operator = (const cBaseDynArrayPointer &Array)
	{
		New(Array.size);
		for(int i=0;i<length();i++)
			Base[i]=Array.Base[i];
		return *this;
	}
	inline cBase* Find(cBase *base,int (*fcmp)(cBase *,cBase *))
	{
		for(cBase **b=Base,**e=&Base[length()];b<e;b++)
			if(fcmp(base,*b))
				return *b;
		return 0;
	}
	inline void Attach(cBase *base)
	{ // добавляет указатель в конец списка
		Resize(length()+BASEDYNARRAY_DSIZE);
		Base[length()-1]=base;
	}
	inline void Detach(cBase *base)
	{ // отсоединение объекта от списка
		int number;
		for(number=0;number<length();number++)
			if(Base[number]==base) 
				break;
		if(number>=length()) return;
		Base[number]=0;
		memcpy(&Base[number],&Base[number+1],(length()-number-1)*sizeof(cBase*));
		Resize(length()-1);
	}
	inline void Delete(int number)
	{ // удаляет из списка элемент с номером number, и удаляет сам эелемент с номером
		if(Base[number]) { delete Base[number]; Base[number]=0; }
		memcpy(&Base[number],&Base[number+1],(length()-number-1)*sizeof(cBase*));
		Resize(length()-1);
	}
	inline void Resize(int NewSize)							
	{
		if(NewSize==0) { Delete(); return; }
		if(size==NewSize) return;
		int OldSize=size < NewSize ? size : NewSize;
		cBase **tmp=Base; Base=new cBase*[size=NewSize];
		for(int i=0;i<OldSize;i++)
			Base[i]=tmp[i];
		if(tmp) delete [] tmp;
	}
};

#endif //__BASE_DYNARRAYPOINTER__

struct sAttribute
{
protected:
	int		Attribute;	
public:
	sAttribute()													{ Attribute=0; }
	
	inline int GetAttribute(int attribute=0xFFFFFFFF)				{ return Attribute&attribute; }
	inline void ClearAttribute(int attribute=0xFFFFFFFF)			{ Attribute&=~attribute; }
	inline void SetAttribute(int attribute)							{ Attribute|=attribute; }
};

struct sPointerCount
{
protected:
	int		Ref;	
public:
	sPointerCount()													{ Ref=1; }
	~sPointerCount()												{ assert(Ref==0); }
	
	inline int IncRef()												{ return ++Ref; }
	inline int DecRef()												{ return --Ref; }
	inline int GetRef()												{ return Ref; }
};

#endif //__BASECLASS_H__
