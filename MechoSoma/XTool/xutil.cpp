#include "xtool.h"

#include <chrono>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "filesystem.h"

static const std::unordered_map<SDL_Keycode, int> keycodeMap{
    { SDLK_UNKNOWN          , VK_UNKNOWN         },
    { SDLK_SPACE            , VK_SPACE           },
    { SDLK_SLASH            , VK_SLASH           },

    { SDLK_0                , VK_0               },
    { SDLK_1                , VK_1               },
    { SDLK_2                , VK_2               },
    { SDLK_3                , VK_3               },
    { SDLK_4                , VK_4               },
    { SDLK_5                , VK_5               },
    { SDLK_6                , VK_6               },
    { SDLK_7                , VK_7               },
    { SDLK_8                , VK_8               },
    { SDLK_9                , VK_9               },

    { SDLK_a                , VK_A               },
    { SDLK_b                , VK_B               },
    { SDLK_c                , VK_C               },
    { SDLK_d                , VK_D               },
    { SDLK_e                , VK_E               },
    { SDLK_f                , VK_F               },
    { SDLK_g                , VK_G               },
    { SDLK_h                , VK_H               },
    { SDLK_i                , VK_I               },
    { SDLK_j                , VK_J               },
    { SDLK_k                , VK_K               },
    { SDLK_l                , VK_L               },
    { SDLK_m                , VK_M               },
    { SDLK_n                , VK_N               },
    { SDLK_o                , VK_O               },
    { SDLK_p                , VK_P               },
    { SDLK_q                , VK_Q               },
    { SDLK_r                , VK_R               },
    { SDLK_s                , VK_S               },
    { SDLK_t                , VK_T               },
    { SDLK_u                , VK_U               },
    { SDLK_v                , VK_V               },
    { SDLK_w                , VK_W               },
    { SDLK_x                , VK_X               },
    { SDLK_y                , VK_Y               },
    { SDLK_z                , VK_Z               },

    { SDLK_ESCAPE           , VK_ESCAPE          },
    { SDLK_RETURN           , VK_RETURN          },
    { SDLK_TAB              , VK_TAB             },
    { SDLK_BACKSPACE        , VK_BACK            },
    { SDLK_INSERT           , VK_INSERT          },
    { SDLK_DELETE           , VK_DELETE          },
    { SDLK_RIGHT            , VK_RIGHT           },
    { SDLK_LEFT             , VK_LEFT            },
    { SDLK_DOWN             , VK_DOWN            },
    { SDLK_UP               , VK_UP              },
    { SDLK_HOME             , VK_HOME            },
    { SDLK_END              , VK_END             },

    { SDLK_PAUSE            , VK_PAUSE           },
    { SDLK_F1               , VK_F1              },
    { SDLK_F2               , VK_F2              },
    { SDLK_F3               , VK_F3              },
    { SDLK_F4               , VK_F4              },
    { SDLK_F5               , VK_F5              },
    { SDLK_F6               , VK_F6              },
    { SDLK_F7               , VK_F7              },
    { SDLK_F8               , VK_F8              },
    { SDLK_F9               , VK_F9              },
    { SDLK_F10              , VK_F10             },
    { SDLK_F11              , VK_F11             },
    { SDLK_F12              , VK_F12             },
    { SDLK_F13              , VK_F13             },
    { SDLK_F14              , VK_F14             },
    { SDLK_F15              , VK_F15             },
    { SDLK_F16              , VK_F16             },
    { SDLK_F17              , VK_F17             },
    { SDLK_F18              , VK_F18             },
    { SDLK_F19              , VK_F19             },
    { SDLK_F20              , VK_F20             },
    { SDLK_F21              , VK_F21             },
    { SDLK_F22              , VK_F22             },
    { SDLK_F23              , VK_F23             },
    { SDLK_F24              , VK_F24             },

    { SDLK_KP_0             , VK_NUMPAD0         },
    { SDLK_KP_1             , VK_NUMPAD1         },
    { SDLK_KP_2             , VK_NUMPAD2         },
    { SDLK_KP_3             , VK_NUMPAD3         },
    { SDLK_KP_4             , VK_NUMPAD4         },
    { SDLK_KP_5             , VK_NUMPAD5         },
    { SDLK_KP_6             , VK_NUMPAD6         },
    { SDLK_KP_7             , VK_NUMPAD7         },
    { SDLK_KP_8             , VK_NUMPAD8         },
    { SDLK_KP_9             , VK_NUMPAD9         },

    { SDLK_LSHIFT           , VK_LSHIFT          },
    { SDLK_LCTRL            , VK_LCONTROL        },
    { SDLK_RSHIFT           , VK_RSHIFT          },
    { SDLK_RCTRL            , VK_RCONTROL        },

    { SDLK_MENU             , VK_MENU            },
    { SDLK_PERIOD           , VK_OEM_PERIOD      },
};

struct xtMsgHandlerObject
{
	int ID;

	void (*Handler)(SDL_Event *);

	void *list;
	xtMsgHandlerObject *next;
	xtMsgHandlerObject *prev;

	xtMsgHandlerObject(void (*p)(SDL_Event *), int id);
};

void xtSysQuant(void);

void xtAddSysObj(XList* lstPtr,void (*fPtr)(void),int id);
void xtDeleteSysObj(XList* lstPtr,int id);
void xtDeactivateSysObj(XList* lstPtr,int id);

void* XGR_hWnd;

typedef void (*XFNC)();

#define XT_DEFAULT_TABLE_SIZE 32

XRuntimeObject** XRObjTable = NULL;
unsigned int XRObjTableSize = 0;

XRuntimeObject* XRObjFirst = NULL;
XRuntimeObject* XRObjLast = NULL;

XList XSysQuantLst;
XList XSysFinitLst;
xtList<xtMsgHandlerObject> XSysHandlerLst;

static bool hXActiveWndEvent = true;
static bool hXNeedExitEvent = false;

file::FileFinder fileFinder;
std::string foundFilePath;

XList::XList(void)
{
	ClearList();
}

XList::~XList(void)
{
}

void XList::AddElement(XListElement* p)
{
	if (!fPtr)
	{
		fPtr = lPtr = p;
		p->prev = p;
		p->next = NULL;
	}
	else
	{
		lPtr->next = p;
		p->prev = lPtr;
		p->next = NULL;
		lPtr = p;
		fPtr -> prev = p;
	}
	ListSize ++;
}

void XList::RemoveElement(XListElement* p)
{
	XListElement* pPtr;
	XListElement* nPtr;

	ListSize--;
	if (ListSize)
	{
		pPtr = p->prev;
		nPtr = p->next;

		pPtr->next = nPtr;
		if (nPtr)
		{
			nPtr->prev = pPtr;
		}

		if (p == fPtr)
		{
			fPtr = nPtr;
		}

		if (p == lPtr)
		{
			lPtr = pPtr;
		}

		lPtr->next = NULL;
		fPtr->prev = lPtr;
	}
	else
	{
		ClearList();	
	}
}

char* XFindFirst(const char* mask)
{
	const auto result = fileFinder.find_first(mask);
	foundFilePath = result ? result->u8string() : "";
	return foundFilePath.empty() ? nullptr : foundFilePath.data();
}

char* XFindNext(void)
{
	const auto result = fileFinder.find_next();
	foundFilePath = result ? result->u8string() : "";
	return foundFilePath.empty() ? nullptr : foundFilePath.data();
}

void xtDeleteFile(const char* fname)
{
}

void xtCreateRuntimeObjectTable(int len)
{
	int i;
	if(!len) len = XT_DEFAULT_TABLE_SIZE;
	XRObjTableSize = len;
	XRObjTable = new XRuntimeObject*[len];

	for(i = 0; i < len; i ++){
		XRObjTable[i] = NULL;
	}
}

void xtRegisterRuntimeObject(XRuntimeObject* p)
{
	if(!XRObjFirst){
		XRObjFirst = XRObjLast = p;
	}
	else {
		XRObjLast -> next = p;
		XRObjLast = p;
	}
	XRObjTable[p -> ID - 1] = p;
}

XRuntimeObject* xtGetRuntimeObject(int id)
{
	if(id == XT_TERMINATE_ID)
		return NULL;
	if(!XRObjTable || !XRObjTableSize || id < 1 || id > XRObjTableSize)
		ErrH.Abort("XTool system error...");
	return XRObjTable[id - 1];
}

int xtIsActive(void)
{
	return hXActiveWndEvent ? 1 : 0;
}

int xtNeedExit()
{
	return hXNeedExitEvent ? 1 : 0;
}

int xtCallXKey(SDL_Event *event) {
	switch (event->type) {
		case SDL_KEYDOWN:
		{
			const auto key = keycodeMap.find(event->key.keysym.sym);
			if (key != keycodeMap.end()) {
				XKey.PressFnc(key->second, 1);
			}
			break;
		}

		case SDL_KEYUP:
		{
			const auto key = keycodeMap.find(event->key.keysym.sym);
			if (key != keycodeMap.end()) {
				XKey.UnPressFnc(key->second, 1);
			}
			break;
		}

		default:
			break;
	}
	return 1;
}

void xtSetExit() {
	hXActiveWndEvent = false;
	hXNeedExitEvent = true;
}

int xtDispatchMessage(SDL_Event *event) {
	int ret = 0;

	xtMsgHandlerObject *p = XSysHandlerLst.first();
	while (p) {
		(*p->Handler)(event);
		p = p -> next;
	}

	ret += xtCallXKey(event);

	switch (event->type) {
		case SDL_QUIT:
			xtSetExit();
			break;

		default:
			break;
	}

	return ret;
}

void xtClearMessageQueue(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (XRec.CheckMessage(event.type)) {
			xtDispatchMessage(&event);
		} else {
			// TODO XMsgBuf->put(&event);
		}
	}
}

void xtSysQuantDisable(int v)
{
}

static unsigned int XRndValue = 83838383;

unsigned int XRnd(unsigned int m)
{
	XRndValue = XRndValue * 214013L + 2531011L;
	if(!m){
		return 0;
	}
	return ((XRndValue>> 16) & 0x7fff) % m;
}

using ClockType = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<ClockType>;
TimePoint start_time;

void initclock(void)
{
	start_time = std::chrono::high_resolution_clock::now();
}

int clocki(void)
{
	const auto d = std::chrono::high_resolution_clock::now() - start_time;
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
	return static_cast<int>(ms.count());
}

double clockf(void)
{
	const auto d = std::chrono::high_resolution_clock::now() - start_time;
	return std::chrono::duration<double>(d).count();
}

void* xtGet_hInstance(void)
{
	return nullptr;
}

xtMsgHandlerObject::xtMsgHandlerObject(void (*p)(SDL_Event *), int id)
{
	list = NULL;
	ID = id;

	Handler = p;
}

void xtRegisterSysMsgFnc(void (*fPtr)(SDL_Event *), int id)
{
	xtMsgHandlerObject *p = new xtMsgHandlerObject(fPtr, id);
	XSysHandlerLst.append(p);
}

void xtRegisterSysQuant(void (*qPtr)(void),int id)
{
	xtAddSysObj(&XSysQuantLst,qPtr,id);
}

void xtUnRegisterSysQuant(int id)
{
	xtDeleteSysObj(&XSysQuantLst,id);
}

void xtRegisterSysFinitFnc(void (*fPtr)(void),int id)
{
	xtAddSysObj(&XSysFinitLst,fPtr,id);
}

void xtDeactivateSysFinitFnc(int id)
{
	xtDeactivateSysObj(&XSysFinitLst,id);
}

void xtUnRegisterSysFinitFnc(int id)
{
	xtDeleteSysObj(&XSysFinitLst,id);
}

void xtDeleteSysObj(XList* lstPtr,int id)
{
	XSysObject* p = (XSysObject*)lstPtr -> fPtr;
	while(p){
		if(p -> ID == id){
			lstPtr -> RemoveElement((XListElement*)p);
			delete p;
			return;
		}
		p = (XSysObject*)p -> next;
	}
}

void xtDeactivateSysObj(XList* lstPtr,int id)
{
	XSysObject* p = (XSysObject*)lstPtr -> fPtr;

	while(p){
		if(p -> ID == id)
			p -> flags |= XSYS_OBJ_INACTIVE;
		p = (XSysObject*)p -> next;
	}
}

void xtAddSysObj(XList* lstPtr,void (*fPtr)(void),int id)
{
	XSysObject* p = (XSysObject*)lstPtr -> fPtr;

	while(p){
		if(p -> ID == id) return;
		p = (XSysObject*)p -> next;
	}

	p = new XSysObject;
	p -> ID = id;
	p -> QuantPtr = reinterpret_cast<void*>(fPtr);

	lstPtr -> AddElement((XListElement*)p);
}

void xtSysQuant(void)
{
	XSysObject* p = (XSysObject*)XSysQuantLst.fPtr;
	while(p){
		(*(XFNC)(p -> QuantPtr))();
		p = (XSysObject*)p -> next;
	}
}

void xtSysFinit(void)
{
	int i,sz = XSysFinitLst.ListSize;
	XSysObject* p = (XSysObject*)XSysFinitLst.lPtr;
	for(i = 0; i < sz; i ++){
		if(!(p -> flags & XSYS_OBJ_INACTIVE))
			(*(XFNC)(p -> QuantPtr))();
		p = (XSysObject*)p -> prev;
	}
	XRec.Close();
}

void mchGraphicsSetup(void)
{
}

const char* win32_get_path_from_regkey(int key_id,const char* subkey_name,const char* value_name)
{
	return "";
}

void* win32_load_icon(void)
{
	return nullptr;
}

void* GetDS_Ptr(void)
{
	return nullptr;
}
