#include "StdAfx.h"
#include "MemoryHeap.h"

int MemoryHeapBlock::IDs = 0;

void MemoryHeap::init()
{
#ifdef _MEMORY_HEAP_STATISTICS_
	if(used()){
		allocated_memory_stat.add(allocated());
		used_memory_stat.add(used());
		used_blocks_stat.add(blocks.size() + 1);
		}
#endif

	while(!blocks.empty())
		pop_block();
	initBlock();
	assert(!(used_memory + allocated_memory));

	stack.clear();
}
void MemoryHeap::SaveStatistics()
{
#ifdef _MEMORY_HEAP_STATISTICS_
	static XStream stat("MemoryHeapStatistics", XS_OUT);
	stat < "Statistics for " < title < "\n";
	stat < "Allocated Memory: " <= allocated_memory_stat < "\n";
	stat < "Used Memory: " <= used_memory_stat < "\n";
	stat < "Used Blocks: " <= used_blocks_stat < "\n";
#endif
}

MemoryHeap::~MemoryHeap()
{ 
	SaveStatistics();
}

void MemoryHeap::push_block()
{
	allocated_memory += size;
	used_memory += offset;
	int sz = size;
	blocks.push_back(*this);
	allocBlock(sz*2);
}
void MemoryHeap::pop_block()
{
	freeBlock();
	static_cast<MemoryHeapBlock&>(*this) = blocks.back();
	blocks.pop_back();
	allocated_memory -= size;
	used_memory -= offset;
}

void MemoryHeap::out_of_block()
{
	if(blocks.size() == max_blocks)
		throw "MemoryHeap - out of memory";
	push_block();
}

void MemoryHeap::store()
{
	stack.push_back(MemoryHeapMark(ID, storeBlock()));
}
void MemoryHeap::restore()
{
	assert(!stack.empty());
	MemoryHeapMark& mark = stack.back();

	while(ID != mark.blockID){
		assert(!blocks.empty());
		pop_block();
		}
		
	restoreBlock(mark.offset);

	stack.pop_back();
}

	

