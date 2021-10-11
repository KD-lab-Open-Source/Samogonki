/*==========================================================================
 *
 * Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 * File:       input.c
 * Content:    DirectInput functionality for FFDonuts sample
 *
 *
 ***************************************************************************/
#include "StdAfx.h"
#include "XJoystick.h"
#include "DebugPrm.h"

const int MOUSE_BUFFER_SIZE = 16;

BOOL CALLBACK   inputEnumDeviceProc(LPDIDEVICEINSTANCE pdidi, LPVOID pv);
BOOL CALLBACK inputEnumMouseProc(LPDIDEVICEINSTANCE pdidi, LPVOID pv);
void readMouseState();

char* XJoystickLastErrorString = 0;
int XJoystickLastErrorCode = 0;
static int XJoystickErrHUsed = 1;
inline void XJOYSTICK_ABORT(char* str, int code) { if(XJoystickErrHUsed) ErrH.Abort(str,XERR_USER,code); else{ XJoystickLastErrorString = str; XJoystickLastErrorCode = code; }}
#define XTRY(cmd) { HRESULT hRes = cmd; if(FAILED(hRes)) if(XJoystickErrHUsed) ErrH.Abort(#cmd, XERR_USER, hRes); else{ XJoystickLastErrorString = #cmd; XJoystickLastErrorCode = hRes; } }


typedef list<XJoystick> JoystickList;
JoystickList xJoysticks; 
XJoystickHandler xJoystickHandler;
static LPDIRECTINPUT      gpdi      = NULL;    // base DirectInput object
static LPDIRECTINPUTDEVICE directInputMouse = NULL;

DIMOUSESTATE2 directInputMouseState;
int mouseGranularityZ;

//===========================================================================
//	XJoystickInit
//===========================================================================
int XJoystickInit(int ErrHUsed)
{
	XJoystickErrHUsed = ErrHUsed;
#ifndef _FINAL_VERSION_
	XJoystickErrHUsed = 1;
#endif

	xJoysticks.clear();
	xJoystickHandler = 0;

	// create the base DirectInput object
	HRESULT hRes = DirectInputCreate((HINSTANCE)xtGet_hInstance(), DIRECTINPUT_VERSION, &gpdi, NULL);
	if(FAILED(hRes)){
		XJOYSTICK_ABORT("DirectInputCreate() failed\n""DirectX 7 or later required.", hRes);
		return FALSE;
		}

	// enumerate for joystick devices
	hRes = gpdi -> EnumDevices(DIDEVTYPE_JOYSTICK,
                                    (LPDIENUMDEVICESCALLBACK)inputEnumDeviceProc,
                                    0,
                                    DIEDFL_ATTACHEDONLY);

	if(FAILED(hRes) || XJoystickLastErrorCode){
		XJOYSTICK_ABORT("EnumDevices() failed", hRes);
		return FALSE;
		}
/*
	// enumerate for mouse
	//hRes = gpdi -> EnumDevices(DIDEVTYPE_MOUSE,(LPDIENUMDEVICESCALLBACK)inputEnumMouseProc, 0, DIEDFL_ATTACHEDONLY);
	XTRY( gpdi -> CreateDevice( GUID_SysMouse, &directInputMouse, NULL ));
	XTRY( directInputMouse -> SetDataFormat( &c_dfDIMouse2 ));
	XTRY( directInputMouse -> SetCooperativeLevel( (HWND)xtGet_hWnd(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) );

	// Get Z-Granularity
	DIPROPDWORD dipdw;  // DIPROPDWORD contains a DIPROPHEADER structure. 
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj        = DIMOFS_Z;
	dipdw.diph.dwHow        = DIPH_BYOFFSET; 
	if(SUCCEEDED( directInputMouse -> GetProperty(DIPROP_GRANULARITY , &dipdw.diph) ) ){
		mouseGranularityZ = dipdw.dwData;
		if(!mouseGranularityZ)
			mouseGranularityZ = 1;
		}

	// Set buffered mode
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = MOUSE_BUFFER_SIZE; 
	XTRY( directInputMouse -> SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ); 

	XTRY( directInputMouse -> Acquire() );
*/

	if(XJoystickLastErrorCode){
		XJOYSTICK_ABORT("EnumDevices() failed", hRes);
		return FALSE;
		}


	// if we get here, we succeeded
	return xJoysticks.size();
} 

void XJoystickCleanup(void)
{
	xJoysticks.clear();

//	if(directInputMouse){
//		directInputMouse -> Release();
//		directInputMouse = NULL;
//		}

	// Release() base object
	if(gpdi){
		gpdi -> Release();
		gpdi = NULL;
		}
} 

void XJoystickQuant()
{
//	if(directInputMouse){
//		HRESULT res = directInputMouse -> GetDeviceState(sizeof(DIMOUSESTATE2), &directInputMouseState);
//		if(res == DIERR_INPUTLOST) 
//			directInputMouse -> Acquire();
//		}
//	readMouseState();
	

	JoystickList::iterator ji;
	FOR_EACH(xJoysticks, ji)
		ji -> quant();
}

int XJoystickIsButtonPressed(int button, int joystick_ID)
{
	JoystickList::iterator ji;
	FOR_EACH(xJoysticks, ji)
		if(ji -> ID == joystick_ID)
			return ji -> isButtonPressed(button);
	return 0;
}
	
XJoystick* XJoystickGetJoystick(int joystick_ID)
{
	JoystickList::iterator ji;
	FOR_EACH(xJoysticks, ji)
		if(ji -> ID == joystick_ID)
			return &*ji;
	return 0;
}

XJoystickHandler XJoystickSetHandler(XJoystickHandler handler)
{
	XJoystickHandler tmp = xJoystickHandler;
	xJoystickHandler = handler;
	return tmp;
}


//===========================================================================
// inputEnumDeviceProc
//
// Enumerates DirectInput devices of type specified in call to
//  IDirectInput::EnumDevices()
//===========================================================================
BOOL CALLBACK inputEnumDeviceProc(LPDIDEVICEINSTANCE pdidi, LPVOID pv)
{
	// create a temporary "Device 1" object
	GUID guidDevice = pdidi->guidInstance;
	LPDIRECTINPUTDEVICE pdiTempDevice = NULL;
	HRESULT hRes = gpdi -> CreateDevice(guidDevice, &pdiTempDevice, NULL);
	if(FAILED(hRes))
		XJOYSTICK_ABORT("CreateDevice() failed.\n JOYSTICK device is required.", hRes);

	// get a "Device 2" object
	// this is needed for access to the ForceFeedback functionality
	LPDIRECTINPUTDEVICE2 gpdiJoystick_;    // DirectInputDevice2 objects
	hRes = pdiTempDevice -> QueryInterface(IID_IDirectInputDevice2,(void**)&gpdiJoystick_);
	if(FAILED(hRes))
		XJOYSTICK_ABORT("QueryInterface(IID_IDirectInputDevice2) failed", hRes);

	// we no longer need the temporary device, go ahead and release it.
	if(pdiTempDevice){
		pdiTempDevice -> Release();
		pdiTempDevice = NULL;
		}

	// prepare joystick 
	xJoysticks.push_back(XJoystick());
	xJoysticks.back().ID = xJoysticks.size() - 1;
	if(xJoysticks.back().prepare(gpdiJoystick_))
		return DIENUM_CONTINUE;

	return DIENUM_STOP;
} 

//===========================================================================
//	XJoystick's functions
//===========================================================================
int XJoystick::prepare(IDirectInputDevice2* gpdiJoystick_)
{
	gpdiJoystick = gpdiJoystick_;

	// quick check to make sure that the object pointer is non-NULL
	if(!gpdiJoystick)
		return FALSE;

	// set the device's data format
	// This tells the device object to act like a specific device --
	// in our case, like a joystick
	HRESULT hRes = gpdiJoystick -> SetDataFormat(&c_dfDIJoystick);
	if(FAILED(hRes))
		XJOYSTICK_ABORT("SetDataFormat(Joystick) failed", hRes);

	// set the device's cooperative level
	// ForceFeedback requires Exclusive access to the device.
	hRes = gpdiJoystick -> SetCooperativeLevel((HWND)xtGet_hWnd(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hRes))
	        XJOYSTICK_ABORT("SetCooperativeLevel(Exclusive | Foreground) failed", hRes);

	// call Unacquire() on the device
	//
	// SetParameter() will fail if a device is currently acquired, we are
	// doing this here in case we get careless and forget to call this
	// function either before we call Acquire() or after we call Unacquire().
	gpdiJoystick -> Unacquire();

	// set the axis ranges for the device
	//
	// We will use the same range for the X and Y axes.  We are setting them
	// fairly low since we are not concerned with anything other than
	// "left", "right", "forward", "backward" and "centered"
	//* prepare DIPROPRANGE structure
	DIPROPRANGE   dipr;
	dipr.diph.dwSize        = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize  = sizeof(dipr.diph);
	dipr.diph.dwHow         = DIPH_BYOFFSET;
	dipr.lMin               = RANGE_MIN;  // negative to the left/top
	dipr.lMax               = RANGE_MAX;  // positive to the right/bottom
	//* x-axis
	dipr.diph.dwObj         = DIJOFS_X;
	//* set the x-axis range property
	hRes = gpdiJoystick -> SetProperty(DIPROP_RANGE, &dipr.diph);
	if(FAILED(hRes)){
		XJOYSTICK_ABORT("SetProperty(RANGE, X-Axis) failed.", hRes);
		return FALSE;
		}
	//* y-axis
	dipr.diph.dwObj         = DIJOFS_Y;
	hRes = gpdiJoystick -> SetProperty(DIPROP_RANGE, &dipr.diph);
	if(FAILED(hRes)){
		XJOYSTICK_ABORT("SetProperty(RANGE, Y-Axis) failed.", hRes);
		return FALSE;
		}

	// set the deadzone for the device
	//
	// We will use the same deadzone percentage for the X and Y axes.
	// This call uses a symbolic constant for the deadzone percentage so that
	// it is easy to change if we decide we don't like it.
	//* prepare DIPROPDWORD structure
	DIPROPDWORD   dipdw;
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow        = DIPH_BYOFFSET;
	dipdw.dwData            = DEADZONE;

	//* set the x-axis range property
	dipdw.diph.dwObj         = DIJOFS_X;
	hRes = gpdiJoystick -> SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	if(FAILED(hRes)){
		XJOYSTICK_ABORT("SetProperty(DEADZONE, X-Axis) failed.", hRes);
		return FALSE;
		}

	//* y-axis
	dipdw.diph.dwObj         = DIJOFS_Y;
	hRes = gpdiJoystick -> SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	if(FAILED(hRes)){
		XJOYSTICK_ABORT("SetProperty(DEADZONE, Y-Axis) failed.", hRes);
		return FALSE;
		}								 

    // Acquire the device(s)
    //
    // This is being done as a convenience since we unacquired earlier in
    // this function.  This does not guarantee that the device will be
    // acquired at the time we return from the function (in other words, we
    // are not going to spin here until we get a succeessful acquisition).
    acquire();

    // we've actually done somthing here
    next_input  = 0; 
    return TRUE;
} 

int XJoystick::acquire()
{
	if(!gpdiJoystick)
		return FALSE;

	// reacquire the device
	if(SUCCEEDED(gpdiJoystick -> Acquire())){
		// DirectInput automatically resets the device whenever
		// ownership changes, so we can assume we've got a device
		// unsullied by its previous owner.
		//      inputCreateEffect(EF_BOUNCE | EF_FIRE | EF_EXPLODE);
		return TRUE;
		}

	// if we get here, we did >not< acquire the device
	return FALSE;
} 

void XJoystick::release()
{
	// Unacquire() and Release() device objects
	//
	// It is perfectly safe to call Unacquire() on a device that is not
	// currently acquired.  In fact, it is good practice to call
	// Unacquire() just prior to Release().
	if(gpdiJoystick){
		gpdiJoystick -> Unacquire();
		gpdiJoystick -> Release();
		gpdiJoystick = NULL;
		}
}

int XJoystick::input()
{
	if(!gpdiJoystick)
		return 0;

	if(clocki() < next_input )
		return 0;
	next_input  = clocki() + 20;

	// poll the joystick to read the current state
	HRESULT hRes = gpdiJoystick -> Poll();

	// read the device state
	hRes = gpdiJoystick -> GetDeviceState(sizeof(XJOYSTATE), (XJOYSTATE*)this);
	if(FAILED(hRes)){
		if(hRes == DIERR_INPUTLOST) 
			acquire();

		// we did not read anything, return no motion
		return 0;
		}

	return 1;
} 

int XJoystick::whatsPressedNow()
{
	if(!input())
		return 0;
	for(int i = 0;i < 32;i++)
		if(rgbButtons[i])
			return VK_BUTTON_1 + i;
	return 0;
}

int XJoystick::isButtonPressed(int vk_code)
{
	return rgbButtons[vk_code - VK_BUTTON_1] ? 1 : 0;
}

void XJoystick::quant()
{
	if(!input() || !xJoystickHandler)
		return;

	for(int i = 0; i < 32; i++)
		if(rgbButtons[i] != prev_state.rgbButtons[i])
			xJoystickHandler(rgbButtons[i] ? XJ_PressButton : XJ_UnPressButton, VK_BUTTON_1 + i, ID);

	if(lX != prev_state.lX)
		xJoystickHandler(XJ_ChangeXAxis, lX, ID);
		
	if(lY != prev_state.lY)
		xJoystickHandler(XJ_ChangeYAxis, lY, ID);
			
	prev_state = *this;
}

//////////////////////////////////////////////////////////////////////////////////
//           Read Mouse Data
//////////////////////////////////////////////////////////////////////////////////
void readMouseState()
{
	directInputMouseState.lX = 0;
	directInputMouseState.lY = 0;
	directInputMouseState.lZ = 0;

	if(!directInputMouse) 
		return;
    
	DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ];  

	DWORD dwElements = MOUSE_BUFFER_SIZE;
	HRESULT hr = directInputMouse -> GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if( hr != DI_OK ){
		if( hr == DIERR_INPUTLOST ) 
			hr = directInputMouse -> Acquire();
		return; 
		}

	for( int i = 0; i < dwElements; i++ ){
		switch( didod[ i ].dwOfs ){
			case DIMOFS_BUTTON0:
			case DIMOFS_BUTTON1:
			case DIMOFS_BUTTON2:
			case DIMOFS_BUTTON3:
				directInputMouseState.rgbButtons[didod[ i ].dwOfs - DIMOFS_BUTTON0] = didod[ i ].dwData;
				break;

			case DIMOFS_X:
				directInputMouseState.lX += (int)didod[ i ].dwData;
				break;
			case DIMOFS_Y:
				directInputMouseState.lY += (int)didod[ i ].dwData;
				break;
			case DIMOFS_Z:
				directInputMouseState.lZ += (int)didod[ i ].dwData;
				break;
			}
		}
}




