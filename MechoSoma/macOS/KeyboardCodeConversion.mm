#include "KeyboardCodeConversion.h"

#include "xtool.h"

// Translates from character code to keyboard code.
int KeyboardCodeFromCharCode(unichar charCode)
{
	switch (charCode)
	{
		case 8: case 0x7F: return VK_BACK;
		case 9: return VK_TAB;
		case 0xD: case 3: return VK_RETURN;
		case 0x1B: return VK_ESCAPE;
		case ' ': return VK_SPACE;
		case NSHomeFunctionKey: return VK_HOME;
		case NSEndFunctionKey: return VK_END;
		case NSPageUpFunctionKey: return VK_PRIOR;
		case NSPageDownFunctionKey: return VK_NEXT;
		case NSUpArrowFunctionKey: return VK_UP;
		case NSDownArrowFunctionKey: return VK_DOWN;
		case NSLeftArrowFunctionKey: return VK_LEFT;
		case NSRightArrowFunctionKey: return VK_RIGHT;
		case NSDeleteFunctionKey: return VK_DELETE;

		case '0': case ')': return VK_0;
		case '1': case '!': return VK_1;
		case '2': case '@': return VK_2;
		case '3': case '#': return VK_3;
		case '4': case '$': return VK_4;
		case '5': case '%': return VK_5;
		case '6': case '^': return VK_6;
		case '7': case '&': return VK_7;
		case '8': case '*': return VK_8;
		case '9': case '(': return VK_9;

		case 'a': case 'A': return VK_A;
		case 'b': case 'B': return VK_B;
		case 'c': case 'C': return VK_C;
		case 'd': case 'D': return VK_D;
		case 'e': case 'E': return VK_E;
		case 'f': case 'F': return VK_F;
		case 'g': case 'G': return VK_G;
		case 'h': case 'H': return VK_H;
		case 'i': case 'I': return VK_I;
		case 'j': case 'J': return VK_J;
		case 'k': case 'K': return VK_K;
		case 'l': case 'L': return VK_L;
		case 'm': case 'M': return VK_M;
		case 'n': case 'N': return VK_N;
		case 'o': case 'O': return VK_O;
		case 'p': case 'P': return VK_P;
		case 'q': case 'Q': return VK_Q;
		case 'r': case 'R': return VK_R;
		case 's': case 'S': return VK_S;
		case 't': case 'T': return VK_T;
		case 'u': case 'U': return VK_U;
		case 'v': case 'V': return VK_V;
		case 'w': case 'W': return VK_W;
		case 'x': case 'X': return VK_X;
		case 'y': case 'Y': return VK_Y;
		case 'z': case 'Z': return VK_Z;

		case NSPauseFunctionKey: return VK_PAUSE;
		case NSSelectFunctionKey: return VK_SELECT;
		case NSPrintFunctionKey: return VK_PRINT;
		case NSExecuteFunctionKey: return VK_EXECUTE;
		case NSPrintScreenFunctionKey: return VK_SNAPSHOT;
		case NSInsertFunctionKey: return VK_INSERT;
		case NSHelpFunctionKey: return VK_INSERT;

		case NSF1FunctionKey: return VK_F1;
		case NSF2FunctionKey: return VK_F2;
		case NSF3FunctionKey: return VK_F3;
		case NSF4FunctionKey: return VK_F4;
		case NSF5FunctionKey: return VK_F5;
		case NSF6FunctionKey: return VK_F6;
		case NSF7FunctionKey: return VK_F7;
		case NSF8FunctionKey: return VK_F8;
		case NSF9FunctionKey: return VK_F9;
		case NSF10FunctionKey: return VK_F10;
		case NSF11FunctionKey: return VK_F11;
		case NSF12FunctionKey: return VK_F12;
		case NSF13FunctionKey: return VK_F13;
		case NSF14FunctionKey: return VK_F14;
		case NSF15FunctionKey: return VK_F15;
		case NSF16FunctionKey: return VK_F16;
		case NSF17FunctionKey: return VK_F17;
		case NSF18FunctionKey: return VK_F18;
		case NSF19FunctionKey: return VK_F19;
		case NSF20FunctionKey: return VK_F20;

		case NSF21FunctionKey: return VK_F21;
		case NSF22FunctionKey: return VK_F22;
		case NSF23FunctionKey: return VK_F23;
		case NSF24FunctionKey: return VK_F24;
		case NSScrollLockFunctionKey: return VK_SCROLL;

		// U.S. Specific mappings. Mileage may vary.
		case ';': case ':': return VK_OEM_1;
		case '=': case '+': return VK_OEM_PLUS;
		case ',': case '<': return VK_OEM_COMMA;
		case '-': case '_': return VK_OEM_MINUS;
		case '.': case '>': return VK_OEM_PERIOD;
		case '/': case '?': return VK_OEM_2;
		case '`': case '~': return VK_OEM_3;
		case '[': case '{': return VK_OEM_4;
		case '\\': case '|': return VK_OEM_5;
		case ']': case '}': return VK_OEM_6;
		case '\'': case '"': return VK_OEM_7;
	}

	return VK_UNKNOWN;
}

int KeyboardCodeFromKeyCode(unsigned short keyCode)
{
	static const int kKeyboardCodes[] = {
		/* 0 */ VK_A,
		/* 1 */ VK_S,
		/* 2 */ VK_D,
		/* 3 */ VK_F,
		/* 4 */ VK_H,
		/* 5 */ VK_G,
		/* 6 */ VK_Z,
		/* 7 */ VK_X,
		/* 8 */ VK_C,
		/* 9 */ VK_V,
		/* 0x0A */ VK_OEM_3, // Section key.
		/* 0x0B */ VK_B,
		/* 0x0C */ VK_Q,
		/* 0x0D */ VK_W,
		/* 0x0E */ VK_E,
		/* 0x0F */ VK_R,
		/* 0x10 */ VK_Y,
		/* 0x11 */ VK_T,
		/* 0x12 */ VK_1,
		/* 0x13 */ VK_2,
		/* 0x14 */ VK_3,
		/* 0x15 */ VK_4,
		/* 0x16 */ VK_6,
		/* 0x17 */ VK_5,
		/* 0x18 */ VK_OEM_PLUS, // =+
		/* 0x19 */ VK_9,
		/* 0x1A */ VK_7,
		/* 0x1B */ VK_OEM_MINUS, // -_
		/* 0x1C */ VK_8,
		/* 0x1D */ VK_0,
		/* 0x1E */ VK_OEM_6, // ]}
		/* 0x1F */ VK_O,
		/* 0x20 */ VK_U,
		/* 0x21 */ VK_OEM_4, // {[
		/* 0x22 */ VK_I,
		/* 0x23 */ VK_P,
		/* 0x24 */ VK_RETURN, // Return
		/* 0x25 */ VK_L,
		/* 0x26 */ VK_J,
		/* 0x27 */ VK_OEM_7, // '"
		/* 0x28 */ VK_K,
		/* 0x29 */ VK_OEM_1, // ;:
		/* 0x2A */ VK_OEM_5, // \|
		/* 0x2B */ VK_OEM_COMMA, // ,<
		/* 0x2C */ VK_OEM_2, // /?
		/* 0x2D */ VK_N,
		/* 0x2E */ VK_M,
		/* 0x2F */ VK_OEM_PERIOD, // .>
		/* 0x30 */ VK_TAB,
		/* 0x31 */ VK_SPACE,
		/* 0x32 */ VK_OEM_3, // `~
		/* 0x33 */ VK_BACK, // Backspace
		/* 0x34 */ VK_UNKNOWN, // n/a
		/* 0x35 */ VK_ESCAPE,
		/* 0x36 */ VK_APPS, // Right Command
		/* 0x37 */ VK_LWIN, // Left Command
		/* 0x38 */ VK_SHIFT, // Left Shift
		/* 0x39 */ VK_CAPITAL, // Caps Lock
		/* 0x3A */ VK_MENU, // Left Option
		/* 0x3B */ VK_CONTROL, // Left Ctrl
		/* 0x3C */ VK_SHIFT, // Right Shift
		/* 0x3D */ VK_MENU, // Right Option
		/* 0x3E */ VK_CONTROL, // Right Ctrl
		/* 0x3F */ VK_UNKNOWN, // fn
		/* 0x40 */ VK_F17,
		/* 0x41 */ VK_DECIMAL, // Num Pad .
		/* 0x42 */ VK_UNKNOWN, // n/a
		/* 0x43 */ VK_MULTIPLY, // Num Pad *
		/* 0x44 */ VK_UNKNOWN, // n/a
		/* 0x45 */ VK_ADD, // Num Pad +
		/* 0x46 */ VK_UNKNOWN, // n/a
		/* 0x47 */ VK_CLEAR, // Num Pad Clear
		/* 0x48 */ VK_VOLUME_UP,
		/* 0x49 */ VK_VOLUME_DOWN,
		/* 0x4A */ VK_VOLUME_MUTE,
		/* 0x4B */ VK_DIVIDE, // Num Pad /
		/* 0x4C */ VK_RETURN, // Num Pad Enter
		/* 0x4D */ VK_UNKNOWN, // n/a
		/* 0x4E */ VK_SUBTRACT, // Num Pad -
		/* 0x4F */ VK_F18,
		/* 0x50 */ VK_F19,
		/* 0x51 */ VK_OEM_PLUS, // Num Pad =.
		/* 0x52 */ VK_NUMPAD0,
		/* 0x53 */ VK_NUMPAD1,
		/* 0x54 */ VK_NUMPAD2,
		/* 0x55 */ VK_NUMPAD3,
		/* 0x56 */ VK_NUMPAD4,
		/* 0x57 */ VK_NUMPAD5,
		/* 0x58 */ VK_NUMPAD6,
		/* 0x59 */ VK_NUMPAD7,
		/* 0x5A */ VK_F20,
		/* 0x5B */ VK_NUMPAD8,
		/* 0x5C */ VK_NUMPAD9,
		/* 0x5D */ VK_UNKNOWN, // Yen (JIS Keyboard Only)
		/* 0x5E */ VK_UNKNOWN, // Underscore (JIS Keyboard Only)
		/* 0x5F */ VK_UNKNOWN, // KeypadComma (JIS Keyboard Only)
		/* 0x60 */ VK_F5,
		/* 0x61 */ VK_F6,
		/* 0x62 */ VK_F7,
		/* 0x63 */ VK_F3,
		/* 0x64 */ VK_F8,
		/* 0x65 */ VK_F9,
		/* 0x66 */ VK_UNKNOWN, // Eisu (JIS Keyboard Only)
		/* 0x67 */ VK_F11,
		/* 0x68 */ VK_UNKNOWN, // Kana (JIS Keyboard Only)
		/* 0x69 */ VK_F13,
		/* 0x6A */ VK_F16,
		/* 0x6B */ VK_F14,
		/* 0x6C */ VK_UNKNOWN, // n/a
		/* 0x6D */ VK_F10,
		/* 0x6E */ VK_UNKNOWN, // n/a (Windows95 key?)
		/* 0x6F */ VK_F12,
		/* 0x70 */ VK_UNKNOWN, // n/a
		/* 0x71 */ VK_F15,
		/* 0x72 */ VK_INSERT, // Help
		/* 0x73 */ VK_HOME, // Home
		/* 0x74 */ VK_PRIOR, // Page Up
		/* 0x75 */ VK_DELETE, // Forward Delete
		/* 0x76 */ VK_F4,
		/* 0x77 */ VK_END, // End
		/* 0x78 */ VK_F2,
		/* 0x79 */ VK_NEXT, // Page Down
		/* 0x7A */ VK_F1,
		/* 0x7B */ VK_LEFT, // Left Arrow
		/* 0x7C */ VK_RIGHT, // Right Arrow
		/* 0x7D */ VK_DOWN, // Down Arrow
		/* 0x7E */ VK_UP, // Up Arrow
		/* 0x7F */ VK_UNKNOWN // n/a
	};

	if (keyCode >= 0x80)
	{
		return VK_UNKNOWN;
	}

	return kKeyboardCodes[keyCode];
}

int platform::KeyboardCodeFromNSEvent(NSEvent* event)
{
	int code = VK_UNKNOWN;

	if (event.type == NSKeyDown || event.type == NSKeyUp)
	{
		NSString* characters = event.characters;
		if (characters.length > 0)
		{
			code = KeyboardCodeFromCharCode([characters characterAtIndex:0]);
		}
		if (code)
		{
			return code;
		}

		characters = [event charactersIgnoringModifiers];
		if (characters.length > 0)
		{
			code = KeyboardCodeFromCharCode([characters characterAtIndex:0]);
		}
		if (code)
		{
			return code;
		}
	}

	return KeyboardCodeFromKeyCode(event.keyCode);
}
