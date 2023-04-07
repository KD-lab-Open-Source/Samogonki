#ifndef __BASEDEFINE_H__
#define __BASEDEFINE_H__

#include "assert.h"
#include "stdlib.h"
#include "string.h"

#include "BaseClass.h"
#include "cString.h"

/*
namespace KDWIN
{
#include <windows.h>
};
using namespace KDWIN;
*/

#include "xtool.h"

inline void ErrAbort(const char *message)
{
	ErrH.Abort(message);
}
inline void ErrAbort(const char *message1,const char *message2,const char *message3)
{
	XBuffer buf;
	buf<message1<message2<message3;
	ErrH.Abort(buf.address());
}

#endif //__BASEDEFINE_H__