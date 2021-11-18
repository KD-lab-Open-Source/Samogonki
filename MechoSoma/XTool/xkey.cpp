#include "xtool.h"

#include <cstring>

XKeyStruct XKey;

XKeyStruct::XKeyStruct()
{
	memset(keyStates, 0, XKEY_MAXCODE);
	for (int i = 0; i < XKEY_MAXCODE; i++)
	{
		keyPressFnc[i] = NULL;
		keyUnpressFnc[i] = NULL;
	}

	pressHandler = NULL;
	unpressHandler = NULL;
}

XKeyStruct::~XKeyStruct()
{
}

void XKeyStruct::init(void* pH, void* upH)
{
	pressHandler = pH;
	unpressHandler = upH;
}

void XKeyStruct::setPress(int key, void (*keyFunction)(void), int repeat)
{
	keyPressFnc[key] = keyFunction;
	if (repeat)
	{
		keyStates[key] |= XKEY_REPEAT;
	}
}

void XKeyStruct::setUnpress(int key, void (*keyFunction)(void))
{
	keyUnpressFnc[key] = keyFunction;
}

void XKeyStruct::clear(void)
{
	memset(keyStates, 0, XKEY_MAXCODE);
}

int XKeyStruct::Pressed(int key)
{
	return (keyStates[key] & XKEY_PRESSED);
}

int XKeyStruct::wasPressed(int key)
{
	if (keyStates[key] & XKEY_WASPRESSED)
	{
		if (!(keyStates[key] & XKEY_PRESSED))
		{
			keyStates[key] &= ~XKEY_WASPRESSED;
		}
		return 1;
	}
	return 0;
}

void XKeyStruct::PressFnc(int vkey, int key)
{
	int rptCount = key & 0xFFFF;

	LastScanCode = key;

	keyStates[vkey] |= (XKEY_PRESSED | XKEY_WASPRESSED);
	if (keyPressFnc[vkey])
	{
		if (!(keyStates[vkey] & XKEY_REPEAT))
		{
			rptCount = 1;
		}

		for (int i = 0; i < rptCount; i++)
		{
			(*(PFK)(keyPressFnc[vkey]))();
		}
	}
	else
	{
		if (pressHandler)
		{
			for (int i = 0; i < rptCount; i++)
			{
				(*(PFN)(pressHandler))(vkey);
			}
		}
	}
}

void XKeyStruct::UnPressFnc(int vkey, int key)
{
	int rptCount = key & 0xFFFF;

	LastScanCode = key;

	keyStates[vkey] &= ~XKEY_PRESSED;

	if (keyUnpressFnc[vkey])
	{
		(*(PFK)(keyUnpressFnc[vkey]))();
	}
	else
	{
		if (unpressHandler)
		{
			for (int i = 0; i < rptCount; i++)
			{
				(*(PFN)(unpressHandler))(vkey);
			}
		}
	}
}

int xtGetKeyName(int vkey,int shift,int russian)
{
	return 0;
}

char* xtGetKeyNameText(int key)
{
	return nullptr;
}
