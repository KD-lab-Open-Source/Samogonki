#ifndef _XJOYSTICK_H
#define _XJOYSTICK_H

#include <cstring>

//---------------------------------------------------------------------------

enum eXJoystickState {
	XJ_PressButton = 1,
	XJ_UnPressButton,
	XJ_ChangeXAxis,
	XJ_ChangeYAxis
};

typedef void (*XJoystickHandler)(eXJoystickState state, int button_or_axis_value, int joystick_ID);

// prototypes
int XJoystickInit(int ErrHUsed = 1); // return the number of joysticks avaible
void XJoystickQuant();
int XJoystickIsButtonPressed(int button, int joystick_ID = 0);
int XJoystickGetXAxis(int joystick_ID = 0);
int XJoystickGetYAxis(int joystick_ID = 0);
void XJoystickCleanup();
XJoystickHandler XJoystickSetHandler(XJoystickHandler handler);

// constants used for scaling the input device
const int DEADZONE  =    2500;            // 25% of the axis range
const int RANGE_MAX =     256;            // maximum positive axis value
const int RANGE_MIN  =     -256;           // minimum negative axis value

enum {
	VK_BUTTON_1 = 512,
	VK_BUTTON_2,
	VK_BUTTON_3,
	VK_BUTTON_4,
	VK_BUTTON_5,
	VK_BUTTON_6,
	VK_BUTTON_7,
	VK_BUTTON_8,
	VK_BUTTON_9,
	VK_BUTTON_10,
	VK_BUTTON_11,
	VK_BUTTON_12,
	VK_BUTTON_13,
	VK_BUTTON_14,
	VK_BUTTON_15,
	VK_BUTTON_16,
	VK_BUTTON_17,
	VK_BUTTON_18,
	VK_BUTTON_19,
	VK_BUTTON_20,
	VK_BUTTON_21,
	VK_BUTTON_22,
	VK_BUTTON_23,
	VK_BUTTON_24,
	VK_BUTTON_25,
	VK_BUTTON_26,
	VK_BUTTON_27,
	VK_BUTTON_28,
	VK_BUTTON_29,
	VK_BUTTON_30,
	VK_BUTTON_31,
	VK_BUTTON_32,
};

//---------------------------------------------------------------------------

#endif // _XJOYSTICK_H









