
#ifndef __ALLOCATION_TRACKING_H__
#define __ALLOCATION_TRACKING_H__

#ifndef _FINAL_VERSION_
#define ALLOW_ALLOCATION_TRACKING
#endif

#ifdef ALLOW_ALLOCATION_TRACKING

struct AllocationAccumulator 
{
	int size, blocks;
	int total_size;
	AllocationAccumulator() { init(); }
	void init() { total_size = size = blocks = 0; }
};

int total_memory_used();
void allocation_tracking(char* title);
void allocation_tracking(char* title, AllocationAccumulator* state);
void allocation_tracking_message(char* str);

#else // ALLOW_ALLOCATION_TRACKING

inline int total_memory_used(){ return 0;}
//inline void allocation_tracking(char* title){}
//inline void allocation_tracking(char* title, struct AllocationAccumulator* state){}
inline void allocation_tracking_message(char* str){}

#endif // ALLOW_ALLOCATION_TRACKING

#endif  // __ALLOCATION_TRACKING_H__
