
#ifndef __HANDLE_H__
#define __HANDLE_H__

////////////////////////////////////////////////////////////////////
// Автоматически удаляемый указатель
////////////////////////////////////////////////////////////////////
template<class T>
class PtrHandle {
  T *ptr;
public:
  void set(T* p) { ptr = p; }
  PtrHandle(T* p = 0) { set(p); }
  PtrHandle(const PtrHandle& orig) { set(orig.ptr); const_cast<PtrHandle&>(orig).ptr = 0; }
  ~PtrHandle() { if(ptr) delete ptr;}

  PtrHandle& operator=(const PtrHandle &orig) 
  {
	if(ptr && ptr != orig.ptr) 
		delete ptr;
	set(orig.ptr);
	const_cast<PtrHandle&>(orig).ptr = 0;
	return *this;
  }
  PtrHandle& operator=(T* p) 
  {
	if(ptr && ptr != p) 
		delete ptr;
	set(p);
	return *this;
  }
  
  T* operator->() {return ptr;}
  T& operator*() {return *ptr;}
  const T* operator->() const {return ptr;}
  const T& operator*() const {return *ptr;}
  T* operator() () {return ptr;}
  const T* operator() () const {return ptr;}
  operator T* () const { return ptr; }
};

////////////////////////////////////////////////////////////////////
// Автоматически удаляемый указатель
// с отслеживанием владельцев.
// Обекты должны иметь handleCount
////////////////////////////////////////////////////////////////////
template<class T>
class ShareHandle {
  T *ptr;
public:
  void set(T *p) {ptr = p; if(p) p->handleCount++;}
  ShareHandle() {ptr = 0;}
  ShareHandle(T *p) {set(p);}
  ShareHandle(const ShareHandle &orig) {set(orig.ptr);}
  ~ShareHandle() {if (ptr && --ptr->handleCount == 0) delete ptr;}
  
  ShareHandle &operator=(const ShareHandle &orig) {
    if (ptr && --ptr->handleCount == 0 && ptr != orig.ptr) delete ptr;
    set(orig.ptr);
    return *this;
  };
  ShareHandle& operator=(T* p) {
    if (ptr && --ptr->handleCount == 0 && ptr != p) delete ptr;
    set(p);
    return *this;
  };
  
  T *operator->() {return ptr;}
  T &operator*() {return *ptr;}
  const T* operator->() const {return ptr;}
  const T& operator*() const {return *ptr;}
  T* operator() () {return ptr;}
  const T* operator() () const {return ptr;}
  operator T* () const { return ptr; }
};

#endif // __HANDLE_H__
