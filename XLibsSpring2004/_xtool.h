/*
******************************************************************************
**									    **
**		     XTOOL	Windows 32 API version 3.0nt		    **
**			   for Windows-NT, Windows 95			    **
**									    **
**	  (C)1993-97 by K-D Lab::KranK, K-D Lab::Steeler, Dr.Tronick	    **
**									    **
******************************************************************************
*/


#ifndef __XTCORE_H
#define __XTCORE_H

#ifndef NULL
#define NULL	0L
#endif

// XSysObject IDs...
#define XGRAPH_SYSOBJ_ID	0x01
#define XSOUND_SYSOBJ_ID	0x02
#define XAVI_SYSOBJ_ID		0x03
#define XD3D_SYSOBJ_ID		0x04

struct XListElement
{
	XListElement* next;
	XListElement* prev;

	XListElement(void){ next = prev = NULL; }
};

// fPtr -> prev == lPtr
// lPtr -> next == NULL
struct XList
{
	int ListSize;
	XListElement* fPtr;
	XListElement* lPtr;

	void ClearList(void){ ListSize = 0; fPtr = lPtr = NULL; }

	void AddElement(XListElement* p);
	void RemoveElement(XListElement* p);

	XList(void);
	~XList(void);
};

#define XT_TERMINATE_ID 	0xFFFFFFFF

struct XRuntimeObject
{
	int ID;
	int Timer;

	XRuntimeObject* next;

	virtual void Init(int pID){ }
	virtual void Finit(void){ }
	virtual int Quant(void){ if(next) return next -> ID; return XT_TERMINATE_ID; }

	void SetTimer(int t){ Timer = t; }

	XRuntimeObject(void){ ID = 1; next = NULL; Timer = 0; }
	XRuntimeObject(int id){ ID = id; next = NULL; Timer = 0; }
	XRuntimeObject(int id,int tm){ ID = id; next = NULL; Timer = tm; }
	~XRuntimeObject(void){ }
};

// XSysObject::flags...
#define XSYS_OBJ_INACTIVE	0x01

struct XSysObject : XListElement
{
	int ID;
	int flags;

	void* QuantPtr;

	XSysObject(void){ ID = 0; flags = 0; QuantPtr = NULL; }
};

/*
	"Critical Section" sinchronisation method
	for "protected" data in multi-processing application

struct XCriticalSection
{
	KDWIN::LPCRITICAL_SECTION csection;
	XCriticalSection(void *section) { csection = (KDWIN::LPCRITICAL_SECTION )section; KDWIN::EnterCriticalSection(csection); }
	~XCriticalSection(void) { KDWIN::LeaveCriticalSection(csection); }
};

*/

void xtCreateRuntimeObjectTable(int len = 0);
void xtRegisterRuntimeObject(XRuntimeObject* p);
XRuntimeObject* xtGetRuntimeObject(int id);

void xtRegisterSysQuant(void (*qPtr)(void),int id);
void xtUnRegisterSysQuant(int id);

int xtIsActive(void);
int xtNeedExit(void);

void xtClearMessageQueue(void);

int xtInitApplication(void);
void xtDoneApplication(void);

void* xtGet_hInstance(void);
void* xtGet_hWnd(void);
void xtSet_hWnd(void* hWnd);
extern void* XGR_hWnd;

void xtSysQuantDisable(int v = 1);
void xtSysQuant(void);

void xtCreateDummyWindow(int x = 0,int y = 0,int sx = 100,int sy = 100);
void xtCloseDummyWindow(void);

#endif /* __XTCORE_H */


/*
		   XConsole (Windows 32 API version)
	 By K-D Lab::KranK, Dr.Tronick, K-D Lab::Steeler (C) 1993-97 V3.0nt
*/

#ifndef __XCONSOLE_H
#define __XCONSOLE_H

#undef	NULL
#ifndef NULL
#define NULL	0L
#endif

struct XConsole
{
	int radix;
	int digits;

	int tab;

	void UpdateBuffer(const char *);

	XConsole(void);
	~XConsole(void);

	XConsole& operator< (const char*);
	XConsole& operator< (char);
	XConsole& operator< (unsigned char);

	XConsole& operator<= (short);
	XConsole& operator<= (unsigned short);
	XConsole& operator<= (int);
	XConsole& operator<= (unsigned int);
	XConsole& operator<= (long);
	XConsole& operator<= (unsigned long);
	XConsole& operator<= (float);
	XConsole& operator<= (double);
	XConsole& operator<= (long double);

	void SetRadix(int _radix){ radix = _radix; }
	void SetDigits(int _digits){ digits = _digits; }
	void SetTab(int size){ tab = size; }
	void clear();

	void setpos(int _x,int _y);
	void getpos(int &x,int &y);
	void initialize(int mode);
};

#define _CONV_BUFFER_LEN	63
extern char _ConvertBuffer[_CONV_BUFFER_LEN + 1];
extern XConsole XCon;

#endif /* __XCONSOLE_H */

#ifndef __XBUFFER_H
#define __XBUFFER_H

#ifndef NULL
#define NULL	0L
#endif

#define XB_DEFSIZE	256

#define XB_BEG		0
#define XB_CUR		1
#define XB_END		2

#define XB_FORWARD	0
#define XB_BACKWARD	1
#define XB_GLOBAL	2

#define XB_CASEON	0
#define XB_CASEOFF	1

#define XB_DEFRADIX	10
#define XB_DEFDIGITS	8

struct XBuffer
{
	char* buf;
	unsigned int size;
	unsigned int offset;
	int radix;
	int digits;
	int MakeFree;
	int automatic_realloc;

	XBuffer(unsigned int sz = XB_DEFSIZE, int automatic_realloc_ = 0);
	XBuffer(void* p,int sz);
	~XBuffer(void){ free(); }

	void SetRadix(int r) { radix = r; }
	void SetDigits(int d) { digits = d; }

	void alloc(unsigned int sz);
	void free(void);
	void fill(char fc = '\0');
	void set(int off,int mode = XB_BEG);
	void init(void){ offset = 0; *buf = 0; }
	int search(char* what,int mode = XB_FORWARD,int cs = XB_CASEON);
	int end() const { return (offset > size); }

	unsigned int tell(void) const { return offset; }
	unsigned int length(void) const { return size; }
	char* address(){ return buf; }

	unsigned int read(void* s, unsigned int len);
	unsigned int write(const void* s, unsigned int len, int bin_flag = 1);
	void handleOutOfSize();
	
	XBuffer& operator< (const char* v);
	XBuffer& operator< (char* v) { return this->operator<((const char*) v); };
	XBuffer& operator< (char v) { return write(v); }
	XBuffer& operator< (unsigned char v) { return write(v); }
	XBuffer& operator< (short v) { return write(v); }
	XBuffer& operator< (unsigned short v) { return write(v); }
	XBuffer& operator< (int v ) { return write(v); }
	XBuffer& operator< (unsigned int v) { return write(v); }
	XBuffer& operator< (long v) { return write(v); }
	XBuffer& operator< (unsigned long v) { return write(v); }
	XBuffer& operator< (float v) { return write(v); }
	XBuffer& operator< (double v) { return write(v); }
	XBuffer& operator< (long double v) { return write(v); }

	XBuffer& operator> (char* v);
	XBuffer& operator> (char& v) { return read(v); }
	XBuffer& operator> (unsigned char& v) { return read(v); }
	XBuffer& operator> (short& v) { return read(v); }
	XBuffer& operator> (unsigned short& v) { return read(v); }
	XBuffer& operator> (int& v) { return read(v); }
	XBuffer& operator> (unsigned int& v) { return read(v); }
	XBuffer& operator> (long& v) { return read(v); }
	XBuffer& operator> (unsigned long& v) { return read(v); }
	XBuffer& operator> (float& v) { return read(v); }
	XBuffer& operator> (double& v) { return read(v); }
	XBuffer& operator> (long double& v) { return read(v); }

	XBuffer& operator<= (char);
	XBuffer& operator<= (unsigned char);
	XBuffer& operator<= (short);
	XBuffer& operator<= (unsigned short);
	XBuffer& operator<= (int);
	XBuffer& operator<= (unsigned int);
	XBuffer& operator<= (long);
	XBuffer& operator<= (unsigned long);
	XBuffer& operator<= (float);
	XBuffer& operator<= (double);
	XBuffer& operator<= (long double);

	XBuffer& operator>= (char&);
	XBuffer& operator>= (unsigned char&);
	XBuffer& operator>= (short&);
	XBuffer& operator>= (unsigned short&);
	XBuffer& operator>= (int&);
	XBuffer& operator>= (unsigned int&);
	XBuffer& operator>= (long&);
	XBuffer& operator>= (unsigned long&);
	XBuffer& operator>= (float&);
	XBuffer& operator>= (double&);
	XBuffer& operator>= (long double&);

	operator char* (){ return buf; }
	char* operator ()(int offs){ return buf + offs; }
	XBuffer& operator++(){ offset++; return *this; }
	XBuffer& operator--(){ offset--; return *this; }
	XBuffer& operator+= (int v){ offset+=v; return *this; }
	XBuffer& operator-= (int v){ offset-=v; return *this; }

	char operator[](int ind) const { return buf[ind]; }
	char operator()() const { return buf[offset]; }

	char& operator[](int ind){ return buf[ind]; }
	char& operator()(){ return buf[offset]; }

	template<class T> XBuffer& write(const T& v){ while(offset + sizeof(T) >= size) handleOutOfSize(); (T&)buf[offset] = v; offset += sizeof(T); return *this; }
	template<class T> XBuffer& read(T& v){ v = (T&)buf[offset]; offset += sizeof(T); return *this; }
};

#endif /* __XBUFFER_H */

#ifndef __XSTREAM_H
#define __XSTREAM_H


#define XS_IN		0x0001
#define XS_OUT		0x0002
#define XS_NOREPLACE	0x0004
#define XS_APPEND	0x0008
#define XS_NOBUFFERING	0x0010
#define XS_NOSHARING	0x0020

#define XS_SHAREREAD	0x0040
#define XS_SHAREWRITE	0x0080

#define XS_BEG		0
#define XS_CUR		1
#define XS_END		2

#define XS_DEFRADIX	10
#define XS_DEFDIGITS	8

struct XStream
{
	typedef void* XSHANDLE;

	XSHANDLE handler;
	long	pos;
	int	eofFlag;
	int	ErrHUsed;
	bool ioError_;
	const char* fname;
	int	radix, digits;
	long	extSize;
	long	extPos;

	XStream(int err = 1);
	XStream(const char* name, unsigned flags,int err = 1);
	~XStream();

	int	open(const char* name, unsigned f = XS_IN);
	int	open(XStream* owner,long start,long ext_sz = -1);
	void	close();
	unsigned long read(void* buf, unsigned long len);
	unsigned long write(const void* buf, unsigned long len);
	long	seek(long offset, int dir);
	long	tell() const { return pos; }
	char*	getline(char* buf, unsigned len);
	int	eof(){ return eofFlag || pos >= size(); }
	long	size() const;
	long	size();
	int	gethandler(){ return (int)handler; }
	void	gettime(unsigned& date,unsigned& time);
	void	flush();
	const char*	GetFileName() const { return fname; }
	void	SetRadix(int r){ radix=r; }
	void	SetDigits(int d){ digits=d; }

	XStream& operator< (const char*);
	XStream& operator< (char);
	XStream& operator< (unsigned char);
	XStream& operator< (short);
	XStream& operator< (unsigned short);
	XStream& operator< (int);
	XStream& operator< (unsigned int);
	XStream& operator< (long);
	XStream& operator< (unsigned long);
	XStream& operator< (float);
	XStream& operator< (double);
	XStream& operator< (long double);

	XStream& operator> (char*);
	XStream& operator> (char&);
	XStream& operator> (unsigned char&);
	XStream& operator> (short&);
	XStream& operator> (unsigned short&);
	XStream& operator> (int&);
	XStream& operator> (unsigned int&);
	XStream& operator> (long&);
	XStream& operator> (unsigned long&);
	XStream& operator> (float&);
	XStream& operator> (double&);
	XStream& operator> (long double&);

	XStream& operator<= (char);
	XStream& operator<= (unsigned char);
	XStream& operator<= (short);
	XStream& operator<= (unsigned short);
	XStream& operator<= (int);
	XStream& operator<= (unsigned int);
	XStream& operator<= (long);
	XStream& operator<= (unsigned long);
	XStream& operator<= (float);
	XStream& operator<= (double);
	XStream& operator<= (long double);

	XStream& operator>= (char&);
	XStream& operator>= (unsigned char&);
	XStream& operator>= (short&);
	XStream& operator>= (unsigned short&);
	XStream& operator>= (int&);
	XStream& operator>= (unsigned int&);
	XStream& operator>= (long&);
	XStream& operator>= (unsigned long&);
	XStream& operator>= (float&);
	XStream& operator>= (double&);
	XStream& operator>= (long double&);

	int operator! (){ if(handler != XSHANDLE(-1)) return 1; else return 0; }
	operator void* (){ if(handler != XSHANDLE(-1)) return 0; else return this; }

	bool ioError() const { return ioError_; }

	template<class T> XStream& write(const T& v){ write(&v, sizeof(T)); return *this; }
	template<class T> XStream& read(T& v){ read(&v, sizeof(T)); return *this; }
};

#endif /* __XSTREAM_H */


#ifndef _ERRH_H
#define _ERRH_H

#ifndef NULL
#define NULL	0L
#endif

#define X_WINNT 		0x0001
#define X_WIN32S		0x0002
#define X_WIN95 		0x0003

#define XERR_ALL		0xFFFF
#define XERR_NONE		0x0000

#define XERR_USER		0x0001
#define XERR_CTRLBRK		0x0004
#define XERR_MATH		0x0008
#define XERR_FLOAT		0x0010
#define XERR_CRITICAL		0x0020
#define XERR_SIGNAL		0x0040

#define XERR_ACCESSVIOLATION	0x0080
#define XERR_DEBUG		0x0100
#define XERR_UNKNOWN		0x0200

struct XErrorHandler
{
	unsigned flags;
	unsigned state;

	const char* prefix;
	const char* postfix;

	void (*restore_func)(void);

		 XErrorHandler(void);
		~XErrorHandler(void);

	void	 SetFlags(unsigned f);
	void	 ClearFlags(unsigned f);
	unsigned InfoFlags(unsigned f);

	void	 SetPrefix(const char* s);
	void	 SetPostfix(const char* s);
	void	 SetRestore(void (*rf)(void));
	void	 SetState(int s){state=s;}
	void	 Abort(const char* message, int code = XERR_USER, int addval = -1, const char* subj = NULL);
	void	 Exit(void);
	void	 RTC(const char *file,unsigned int line, const char *expr);
	void	 WriteLog(char* error, char* msg);
};

extern XErrorHandler ErrH;

// Use this macro for after any operation for errors diagnostic
#define XAssert(expr) ErrH.RTC(__FILE__,__LINE__,expr)


#if (!defined(_FINAL_VERSION_) || defined(_DEBUG)) && !defined(NASSERT)

// Use d3dFlipToGdiSurface() for D3D Fullscreen modes
void SetAssertRestoreGraphicsFunction(void(*func)());

int DiagAssert(unsigned long dwOverrideOpts, const char* szMsg, const char* szFile, unsigned long dwLine ) ;

#undef NDEBUG
#define xxassert(exp, msg) \
    do                                                              \
    {  static int ignore = 0;                                   \
        if ( !(exp) && !ignore)				\
		switch(DiagAssert ( 0 ,  msg, __FILE__  , __LINE__)){  \
			case 1: ignore = 1; break; \
			case 2: __asm { int 3 }; break; \
			}\
    } while (0)

#define xassert(exp) xxassert(exp, #exp)

#else  //  ...

#define SetAssertRestoreGraphicsFunction(func)
#define xxassert(exp, msg) 
#define xassert(exp) 

#endif  //  ...

#undef assert
#define assert(x)	xassert(x)

#endif /* _ERRH_H */

#ifndef __XRECORDER_H
#define __XRECORDER_H

// XRecorderMessage types...
enum XRecorderMessages
{
	XRC_SYSTEM_MESSAGE = 1, 	// Keyoard events...
	XRC_XMOUSE_MESSAGE,		// Mouse events...
	XRC_USER_MESSAGE,		// Custom events...

	XRC_MAX_MESSAGE_ID
};

struct XRecorderMessage
{
	int Type;
	int Frame;
	int DataSize;

	int* data;

	XRecorderMessage(void){ Type = 0; DataSize = 0; Frame = 0; data = NULL; }
};

// XRecorder flags...
#define XRC_RECORD_MODE 	0x01
#define XRC_PLAY_MODE		0x02
#define XRC_MESSAGE_READ	0x04
#define XRC_EXTERNAL_QUANT	0x08

#define XRC_CONTROL_COUNT	1024
#define XRC_BUFFER_SIZE 	5

struct XRecorder
{
	int flags;

	int frameCount;
	int controlCount;

	void* hWnd;

	XStream* hFile;

	int nextMsgDataSize;
	XRecorderMessage* nextMsg;

	void Quant(void);

	void Open(char* fname,int mode);
	void Close(void);
	void Flush(void);

	void PutMessage(int msg,int sz = 0,void* p = NULL);
	void PutSysMessage(int id,int msg,int wp,int lp);
	void GetMessage(void);
	void DispatchMessage(void);
	int CheckMessage(int code);

	XRecorder(void);
};

extern XRecorder XRec;

#endif /* __XRECORDER_H */

#ifndef __XKEY_H
#define __XKEY_H

// Flags for XKey::keyStates...
#define XKEY_REPEAT		0x0002
#define XKEY_PRESSED		0x0004
#define XKEY_WASPRESSED 	0x0008

#define XKEY_MAXCODE		256

/*
 * Virtual Keys, Standard Set
 */

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14


#define VK_ESCAPE         0x1B

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE


// Some additional virtual keycodes...
#define VK_OEM_COMMA	0xBC
#define VK_OEM_PERIOD	0xBE
#define VK_OEM_SCROLL	0x91
#define VK_OEM_MINUS	0xBD
#define VK_OEM_5_		0x0C
#define VK_OEM_PLUS		0xBB
#define VK_OEM_ALT		0x12

#define VK_SLASH	0xBF
#define VK_BKSLASH	0x5C
#define VK_TILDE	0xC0
#define VK_LBR		0x5B
#define VK_RBR		0x5D


struct XKeyStruct
{
	void* keyPressFnc[XKEY_MAXCODE];
	void* keyUnpressFnc[XKEY_MAXCODE];

	void* pressHandler;
	void* unpressHandler;

	int LastChar;
	int LastScanCode;
	char keyStates[XKEY_MAXCODE];

	XKeyStruct();
	~XKeyStruct();

	void init(void* pH,void* upH);
	void finit(void);
	void setPress(int key,void (*keyFunction)(void),int repeat);
	void setUnpress(int key,void (*keyFunction)(void));

	void PressFnc(int vkey,int key);
	void UnPressFnc(int vkey,int key);

	void clear(void);

	int Pressed(int key);
	int wasPressed(int key);
};

extern XKeyStruct XKey;

int xtGetKeyName(int vkey,int shift,int russian = 0);
char* xtGetKeyNameText(int key);

int xtGetKeyState(int vk);

#endif /* __XKEY_H */


#ifndef __XUTIL_H
#define __XUTIL_H

void* xalloc(unsigned sz);
void* xrealloc(void* p,unsigned sz);
void  xfree(void* p);

#define XALLOC(a)	xalloc(a)
#define XREALLOC(a,b)	xrealloc(a,b)
#define XFREE(a)	xfree(a)

unsigned int XRnd(unsigned int m);
void XRndSet(unsigned int m);
unsigned int XRndGet();

char* XFindNext(void);
char* XFindFirst(char* mask);

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

#ifndef __ROUND__
#define __ROUND__

__forceinline int round(double x)
{
	int a;
	_asm {
		fld x
		fistp dword ptr a
	}
	return a;
}

__forceinline int round(float x)
{
	int a;
	_asm {
		fld x
		fistp dword ptr a
	}
	return a;
}

template <class T> 
__forceinline T sqr(const T& x){ return x*x; }

template <class T> 
__forceinline int SIGN(const T& x) { return x ? (x > 0 ? 1 : -1 ) : 0; }

#endif __ROUND__

__forceinline int BitSR(int x)
{
	int return_var;
	_asm {
		mov eax, x
		cdq
		xor eax,edx
		sub     eax,edx
		bsr     eax,eax
		mov [return_var],eax
	}
	return return_var;
}

void initclock(void);
int clocki(void);
double clockf(void);

void xtDeleteFile(char* fname);

#endif
#ifndef __XCPUID_H
#define __XCPUID_H

#define CPU_INTEL		0x00000001

// Intel features IDs

#define INTEL_MMX		0x00800000

// Intel Family IDs
#define CPU_INTEL_386		3
#define CPU_INTEL_486		4
#define CPU_INTEL_PENTIUM	5
#define CPU_INTEL_PENTIUM_PRO	6

//Intel Pentium model's
#define PENTIUM_P5A		0
#define PENTIUM_P5		1
#define PENTIUM_P54C		2
#define PENTIUM_P54T		3
#define PENTIUM_MMX		4
#define PENTIUM_DX4		5
#define PENTIUM_P5OVER		6
#define PENTIUM_P54CA		7

// Intel Pentium Pro model's
#define PENTIUM_PROA		0
#define PENTIUM_PRO		1
#define PENTIUM_PRO2		3
#define PENTIUM_PRO55CT 	4
#define PENTIUM_PROF		5

extern unsigned int xt_processor_id;
extern unsigned int xt_processor_type;
extern unsigned int xt_processor_family;
extern unsigned int xt_processor_model;
extern unsigned int xt_processor_revision;
extern unsigned int xt_processor_features;

unsigned int xt_get_cpuid(void);
char* xt_getMMXstatus(void);

extern int xt_mmxUse;

#endif // __XCPUID_H

#ifndef __XZIP_H__
#define __XZIP_H__

typedef unsigned long ulong;

ulong ZIP_compress(char* trg,ulong trgsize,char* src,ulong srcsize);
ulong ZIP_GetExpandedSize(char* p);
void ZIP_expand(char* trg,ulong trgsize,char* src,ulong srcsize);


#endif //__XZIP_H__

#ifndef __XT_LIST_H__
#define __XT_LIST_H__


template <class Type> class xtList
{
	int numElements;
	Type* firstElement;
	void test(int code);

public:

	xtList();
	~xtList();

	int size();
	Type* first();
	Type* last();

	void clear();
	void delete_all();

	void insert(Type* p);
	void append(Type* p);
	void insert(Type* pointer,Type* p);
	void append(Type* pointer,Type* p);
	void remove(Type* p);
	Type* search(int ID);
};

template <class Type>
inline void xtList<Type>::test(int code)
{
#ifdef _XT_TEST_LIST_
	Type* p = first();
	int cnt = 0;
	while(p){
		cnt++;
		p = p -> next;
		}
	if(cnt != numElements)
		ErrH.Abort("List",XERR_USER,code);
#endif
}

template <class Type>
inline xtList<Type>::xtList()
{
	numElements = 0; firstElement = 0;
}

template <class Type>
inline xtList<Type>::~xtList()
{
	clear();
}

template <class Type>
inline void xtList<Type>::clear()
{
	while(first())
		remove(first());
}

template <class Type>
inline void xtList<Type>::delete_all()
{
	Type* p;
	while((p = first()) != 0){
		remove(p);
		delete p;
		}
}

template <class Type>
inline int xtList<Type>::size()
{
	return numElements;
}

template <class Type>
inline Type* xtList<Type>::first()
{
	return firstElement;
}

template <class Type>
inline Type* xtList<Type>::last()
{
	return firstElement ? firstElement -> prev : 0;
}

template <class Type>
inline void xtList<Type>::insert(Type* p)
{
	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	if(firstElement){
		p -> next = firstElement;
		p -> prev = firstElement -> prev;
		firstElement -> prev = p;
		}
	else{
		p -> prev = p;
		p -> next = 0;
		}
	firstElement = p;
	p -> list = this;
	test(0);
}

template <class Type>
inline void xtList<Type>::insert(Type* pointer,Type* p)
{
	if(!firstElement || firstElement == pointer){
		insert(p);
		return;
		}
	if(!pointer){
		append(p);
		return;
		}

	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	p -> next = pointer;
	p -> prev = pointer -> prev;
	pointer -> prev -> next = p;
	pointer -> prev = p;
	p -> list = this;
	test(5);
}


template <class Type>
inline void xtList<Type>::append(Type* p)
{
	if(p -> list)
		ErrH.Abort("Element is already in list");
	numElements++;
	if(firstElement){
		p -> next = 0;
		p -> prev = firstElement -> prev;
		firstElement -> prev -> next = p;
		firstElement -> prev = p;
		}
	else{
		p -> next = 0;
		p -> prev = firstElement = p;
		}
	p -> list = this;
	test(1);
}

template <class Type>
inline void xtList<Type>::remove(Type* p)
{
	if(p -> list != this)
		ErrH.Abort("Removed element isn't in list");
	numElements--;
	if(p -> next)
		p -> next -> prev = p -> prev;
	else
		firstElement -> prev = p -> prev;

	if(p != firstElement)
		p -> prev -> next = p -> next;
	else{
		firstElement = p -> next;
		if(firstElement)
			firstElement -> prev = p -> prev;
		}

	p -> next = p -> prev = 0;
	p -> list = 0;
	test(2);
}

template <class Type>
inline Type* xtList<Type>::search(int ID)
{
	Type* p = first();
	while(p){
		if(p -> ID == ID)
			return p;
		p = p -> next;
		}
	return 0;
}

#endif	// __XT_LIST_H__


#ifndef __ZIP_RESOURCE_H__
#define __ZIP_RESOURCE_H__

class XZIP_FileHeader 
{
	char* fileName;
	unsigned dataOffset;
	unsigned dataSize;

	int extDataSize;
	char* extData;

public:
	void* list;
	XZIP_FileHeader* prev;
	XZIP_FileHeader* next;

	void SetName(char* p);

	unsigned size(void) const { return dataSize; }
	unsigned offset(void) const { return dataOffset; }
	char* data(void) const { return extData; }
	char* name(void) const { return fileName; }

	void save(XStream& fh);

	XZIP_FileHeader(void);
	XZIP_FileHeader(char* fname,unsigned offs,unsigned size,void* ext_ptr,int ext_sz);
	XZIP_FileHeader(XStream& fh);
	~XZIP_FileHeader(void);
};

// XZIP_Resource flags
const int XZIP_ENABLE_EXTERNAL_FILES	= 0x01;
const int XZIP_ENABLE_ZIP_HEADERS		= 0x02;

class XZIP_Resource 
{
	int flags;

	char* fileName;
	char* idxName;

	xtList<XZIP_FileHeader> fileList;
	XStream file;

	XZIP_FileHeader* find(char* fname);
public:
	int open(char* fname,XStream& fh,int mode = 0);

	void LoadHeaders(void);
	void LoadIndex(void);
	void SaveIndex(void);

	void dump(char* fname);

	XZIP_Resource(char* fname,int fl);
	~XZIP_Resource(void);
};

#endif 