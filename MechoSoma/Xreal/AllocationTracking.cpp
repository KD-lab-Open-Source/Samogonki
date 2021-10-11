#include "StdAfx.h"

#include <process.h>
#include <crtdbg.h>
#include "AllocationTracking.h"

#ifdef ALLOW_ALLOCATION_TRACKING

///////////////////////////////////////////////////////////////////////
//	Statics
///////////////////////////////////////////////////////////////////////

static ofstream out;
static int allocation_size = 0;
static int allocation_counter = 0;
static int allocation_size_stamp = 0;
static int allocation_counter_stamp = 0;
static int total_memory_stamp = 0;



///////////////////////////////////////////////////////////////////////
//	Memory Hook
///////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define nNoMansLandSize 4
typedef struct _CrtMemBlockHeader
{
        struct _CrtMemBlockHeader * pBlockHeaderNext;
        struct _CrtMemBlockHeader * pBlockHeaderPrev;
        char *                      szFileName;
        int                         nLine;
        size_t                      nDataSize;
        int                         nBlockUse;
        long                        lRequest;
        unsigned char               gap[nNoMansLandSize];
        /* followed by:
         *  unsigned char           data[nDataSize];
         *  unsigned char           anotherGap[nNoMansLandSize];
         */
} _CrtMemBlockHeader;

#define pHdr(pbData) (((_CrtMemBlockHeader *)pbData)-1)

#if _MSC_VER == 1100
int __cdecl AllocationTrackingHook(  int  nAllocType,  void   * pvData,  unsigned int   nSize,  int      nBlockUse,  long     lRequest,  const char * szFileName,  int      nLine  )
#else
int __cdecl AllocationTrackingHook(  int  nAllocType,  void   * pvData,  unsigned int   nSize,  int      nBlockUse,  long     lRequest,  const unsigned char * szFileName,  int      nLine  )
#endif
{
	switch(nAllocType){
		case _HOOK_REALLOC:
			{
				_CrtMemBlockHeader *pHead=pHdr(pvData);
				int dSize=(int)nSize-pHead->nDataSize;
				allocation_size +=dSize;
				break;
			}
		case _HOOK_ALLOC:   
			{
				allocation_size += nSize;
				allocation_counter++;
				break;
			}
		case _HOOK_FREE:   
			{
				_CrtMemBlockHeader *pHead=pHdr(pvData);
				nSize=pHead->nDataSize;
				allocation_size -= nSize;
				allocation_counter--;
				break;
			}
		default:{
			_CrtMemBlockHeader *pHead=pHdr(pvData);
			break;
			}

   }

   return( 1 );         // Allow the memory operation to proceed
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////
//	Count by pages
///////////////////////////////////////////////////////////////////////

int total_memory_used()
{
	int size = 0;
	MEMORY_BASIC_INFORMATION Buffer;
	VirtualQuery((void*)0x400000, &Buffer,  sizeof(Buffer) );
	while(Buffer.BaseAddress < (char*)0x6000000){
		if(Buffer.State == MEM_COMMIT && !(Buffer.Type & MEM_MAPPED) && Buffer.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READ) )
			size += Buffer.RegionSize;
		VirtualQuery((char*)Buffer.BaseAddress + Buffer.RegionSize, &Buffer,  sizeof(Buffer) );
		}
	return size;
}


///////////////////////////////////////////////////////////////////////
//	Tracking System
///////////////////////////////////////////////////////////////////////
static void separator()
{	
	out << "--------------------------------------------------------------------------------------------------------------------------------------\n";
}

void allocation_tracking_message(char* str)
{
	separator();
	out << str << endl;
	separator();
}

void allocation_tracking(char* title)
{
	allocation_tracking(title, 0);
}

void allocation_tracking(char* title, AllocationAccumulator* accumulator)
{
	static int opened, disabled;
	if(disabled)
		return;
	if(!opened){
		opened = 1;

		disabled = 1;
		for(int i = 1;i < __argc;i++)
			if(strstr(__argv[i], "memory"))
				disabled = 0;
		if(disabled)
			return;

		#ifdef _DEBUG
		_CrtSetAllocHook( &AllocationTrackingHook );
		#endif
		out.open("memory");
		out << "\n\tMemory allocation listing.\n\t";
		time_t aclock;
		time( &aclock );                 /* Get time in seconds */
		out << asctime( localtime( &aclock ) ) << "\n\n";
		separator();
		out << "|                           |         |       Actual Application Size        |  Total malloc (if DEBUG)  |  Title malloc (if DEBUG)  | \n";
		out << "|        Title              |   Time  |                                      |                           |                           |\n";
		out << "|                           |         |    Total       Title     Accumulated |     Size   Blocks  BSize  |     Size   Blocks  BSize  |\n";
		separator();
		}						  

	int total_memory = total_memory_used();
	int total_size = total_memory - total_memory_stamp;
	int accumulated_size = 0;
	int size = allocation_size - allocation_size_stamp;
	int cnt = allocation_counter - allocation_counter_stamp;

	if(total_memory != total_memory_stamp || size || cnt){
		if(accumulator){
			size = accumulator -> size += size;
			cnt = accumulator -> blocks += cnt;
			accumulated_size = accumulator -> total_size += total_size;
			}

		char buf[2048];
		sprintf(buf, "| %-25.25s | %7.3f | %10i | %10i | %10i | %10i  %5i  %6i | %10i  %5i  %6i |", title, clockf()/1000.,
			total_memory, total_size,  accumulated_size,
			allocation_size, allocation_counter, (allocation_counter ? allocation_size/allocation_counter : 0), 
			size, cnt, (cnt ? size/cnt : 0) );

		out << buf << "\n";

		allocation_size_stamp = allocation_size; 
		allocation_counter_stamp = allocation_counter;
		total_memory_stamp = total_memory;
		}
}



#endif // ALLOW_ALLOCATION_TRACKING
