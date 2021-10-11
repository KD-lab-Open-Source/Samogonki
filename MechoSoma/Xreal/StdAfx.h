// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>

// Standart includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include <iomanip>
#include <fstream>
#include <typeinfo.h>
#include <float.h>


// STL
#include <vector> 
#include <list>
//#include "slist.h"
#include "hash_map.h"
//#include <map.h>
#include <string>

#include "my_STL.h"

// XTool
#include "Handle.h"
#include "_xtool.h"
#include "xgr.h"
#include "xmath.h"
#include "cycledMath.h"
#include "Timers.h"
#include "Mesh3ds.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
