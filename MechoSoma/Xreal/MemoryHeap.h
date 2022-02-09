/*/////////////////////////////////////////////////////////////////////////////////////////////////////
	Многоблочная куча пямяти
 
  Using:
	
	MemoryHeap mh( [начальный размер], [максимальное кол-во блоков], [название - для вывода статистики] );

	mh.init(); - освобождает все блоки кроме первого, в котором обнуляет смещение

	p = mh.malloc(size);
	p = mh.calloc(number, size_of_type);
	p = mh.realloc(p, old_size); - оптимален только для последнего блока

	mh.store();  - запоминание для неполной инициализации кучи, вложенность неограничена
	mh.restore(); - неполный откат кучи.

	1. Каждый новый блок в два раза больше предыдущего.
	2. Использование статистики (_MEMORY_HEAP_STATISTICS_) позволяет выбрать оптимальный начальный размер.
	3. При превышении максимального количества блоков генерируется: trow "MemoryHeap - out of memory";
	4. Для отладки можно перейти к обычному выделению: _MEMORY_HEAP_STD_MALLOC_

/////////////////////////////////////////////////////////////////////////////////////////////////////*/

#ifndef __MEMORY_HEAP_H__
#define __MEMORY_HEAP_H__

//#define _MEMORY_HEAP_STATISTICS_
//#define _MEMORY_HEAP_STD_MALLOC_

#ifdef _MEMORY_HEAP_STATISTICS_
#include "StatisticalAnalizator.h"
#endif


class MemoryHeapBlock 
{
public:

	MemoryHeapBlock(int sz = 0) { allocBlock(sz); }
	virtual ~MemoryHeapBlock(){ freeBlock(); }
	
	MemoryHeapBlock(const MemoryHeapBlock& block){
		*this = block;
		}
	MemoryHeapBlock& operator = (const MemoryHeapBlock& block){
		ID = block.ID;
		offset = block.offset;
		size = block.size;
		heap = block.heap;
		last_sz = block.last_sz;
		last_p = block.last_p;
		const_cast<MemoryHeapBlock&>(block).size = 0;
		return *this;
		}

#ifndef _MEMORY_HEAP_STD_MALLOC_
	void* malloc(size_t sz) { 
			last_p = heap + offset;
			if((offset += last_sz = sz) > size){
				offset -= sz;
				out_of_block();
				return malloc(sz);
				}
			return last_p;
			}

	void* calloc(size_t num, size_t sz) {
			last_p = heap + offset;
			sz *= num;
			if((offset += last_sz = sz) > size){
				offset -= sz;
				out_of_block();
				return calloc(num, sz);
				}
			memset(last_p, 0, sz);
			return last_p;
			}

	void* realloc(void* old_p, size_t sz) {
			if(old_p != last_p){
				last_p = heap + offset;
				if((offset += last_sz = sz) > size){
					offset -= sz;
					out_of_block();
					return realloc(old_p, sz);
					}
				memcpy(last_p, old_p, sz);
				return last_p;
				}
			else{
				if((offset += sz - last_sz) > size){
					offset -= sz - last_sz;
					out_of_block();
					return realloc(old_p, sz);
					}
				last_sz = sz;
				return last_p;
				}
			}

#else // _MEMORY_HEAP_STD_MALLOC_
	void* malloc(size_t sz) { return ::malloc(sz); }
	void* calloc(size_t num, size_t sz) { return ::calloc(num, sz); }
	void* realloc(void* old_p, size_t sz) { return ::realloc(old_p, sz); }
#endif 

protected:

	int ID;
	int offset, size;
	char* heap;
	int last_sz;
	char* last_p;
	static int IDs;

	void allocBlock(int sz) { ID = IDs++; freeBlock(); heap = sz ? new char[sz] : 0; size = sz; initBlock(); }
	void freeBlock(){ if(size){delete heap; size = 0; } }

	void initBlock(){ last_sz = offset = 0; last_p = heap; }
	int storeBlock() const { return offset; }
	void restoreBlock(int stored_offset){ offset = stored_offset; last_sz = 0; last_p = 0;}
	
	virtual void out_of_block(){}
};

/////////////////////////////////////////////////////////////////////////////////
//	Многоблочная куча памяти
/////////////////////////////////////////////////////////////////////////////////
class MemoryHeap : public MemoryHeapBlock 
{
public:
	MemoryHeap(int initial_block_size, int max_blocks_ = 5, char* title_ = "Memory Heap") { allocBlock(initial_block_size); max_blocks = max_blocks_; title = title_; used_memory = allocated_memory = 0; }
	~MemoryHeap();

	void init(); // Освобождает все дополнительные блоки и сбрасывает указатель текущего блока
	void store(); // Устанавливает метку для неполной очистки (модель стека)
	void restore();  // Неполная очистка по установленной ранее метке

	int allocated() const { return allocated_memory + size; } // Выделенная память
	int used() const { return used_memory + offset; } // Реально используемая

	void SaveStatistics();
private:
	void push_block();
	void pop_block();
	void out_of_block();

	struct MemoryHeapMark
	{
		int blockID;
		int offset;
		MemoryHeapMark(int blockID_ = 0, int offset_ = 0) { blockID = blockID_; offset = offset_; }
	};

	char* title;
	int used_memory, allocated_memory;
	int max_blocks;
	vector<MemoryHeapBlock> blocks;
	vector<MemoryHeapMark> stack;

#ifdef _MEMORY_HEAP_STATISTICS_
	StatisticalAnalizator allocated_memory_stat;
	StatisticalAnalizator used_memory_stat;
	StatisticalAnalizator used_blocks_stat;
#endif
};					    


/////////////////////////////////////////////////////////////////////////////////
//	Адаптация оператора new
/////////////////////////////////////////////////////////////////////////////////
inline void* operator new(size_t size, MemoryHeap& heap)
{
	return heap.malloc(size); 
}

inline void operator delete(void* pMem, MemoryHeap& heap){}

#endif // __MEMORY_HEAP_H__
