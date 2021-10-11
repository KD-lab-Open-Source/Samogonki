/**
 *	
 *	
 *
 *	@version $Id: SharedCounter.h,v 1.1 2002/11/07 13:48:36 ash Exp $
 *
 *	@author Alexander Porechnov (scmorr)(немного модифицировал Илюха;))
 */

#pragma once

#ifndef _SHAREDPOINTER_H
#define _SHAREDPOINTER_H

//#define THREAD_SAFE_COUNTER
#ifdef THREAD_SAFE_COUNTER
#include <boost/detail/atomic_count.hpp>
#endif
class SharedCounter {
	protected:
		friend void intrusive_ptr_add_ref(SharedCounter const* p);
		friend void intrusive_ptr_release(SharedCounter const* p);

		SharedCounter() : useCount(0) {}
		//! \brief при копировании и присваивании количество intrusive_ptr, 
		//! ссылающихся на этот объект, НЕ меняется
		SharedCounter(SharedCounter const& ) : useCount(0) {}
		void operator = (SharedCounter const& ){
		}
		virtual ~SharedCounter() {}

		void referenced() const{
			++useCount;
		}

		void released() const{
			--useCount;
			if (useCount == 0) {
				delete this;
			}
		}

	private:
#ifdef THREAD_SAFE_COUNTER
		mutable boost::detail::atomic_count useCount;
#else
		mutable int useCount;
#endif
};

inline void intrusive_ptr_add_ref(SharedCounter const * p){
	p->referenced();
}

inline void intrusive_ptr_release(SharedCounter const * p){
	p->released();
}

#ifdef THREAD_SAFE_COUNTER
#undef THREAD_SAFE_COUNTER
#endif

#endif