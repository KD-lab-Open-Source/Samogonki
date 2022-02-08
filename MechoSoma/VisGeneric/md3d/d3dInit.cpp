//*******************************************************************
//	
//	Mechosoma D3D Wrapper API
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include "D3DEnum.h"
#include "D3DUtil.h"

#define OCR_NORMAL          32512


// DD/D3D libs
#pragma comment( lib, "ddraw" )
#pragma comment( lib, "d3d8" )
#pragma comment( lib, "dxguid" )


//-----------------------------------------------------------------------------
// Global variables 
//-----------------------------------------------------------------------------

BOOL		g_bInitialized = FALSE;
BOOL		g_bInScene;				// TRUE when in BeginScene/EndScene bracket

HWND		m_hRenderWnd = NULL;

BOOL		g_bIsFullscreen;    // Fullscreen vs. windowed
DWORD		g_dwRenderWidth;    // Dimensions of the render target
DWORD		g_dwRenderHeight;
RECT		g_rcScreenRect;     // Screen rect for window
RECT		g_rcViewportRect;   // Offscreen rect for VPort

LPDIRECTDRAWSURFACE7 g_pddsFrontBuffer=0;  // The primary surface
LPDIRECTDRAWSURFACE7 g_pddsBackBuffer=0;   // The backbuffer surface
LPDIRECTDRAWSURFACE7 m_pddsZBuffer=0;      // The zbuffer surface
LPDIRECT3DDEVICE7    g_pd3dDevice=0;       // The D3D device
LPDIRECTDRAW7        g_pDD=0;              // The DirectDraw object
LPDIRECT3D7          m_pD3D;             // The Direct3D object
D3DDEVICEDESC        g_ddDeviceDesc;
DWORD                m_dwDeviceMemType;
DDPIXELFORMAT		 g_ddpfPrimary;

DWORD				 g_dwSceneStamp;	 // Incremented on every EndScene and used 
										 // by texture manager

BOOL g_bActive=FALSE;//Активно ли приложение в данный момент
BOOL g_bSleepIfNonActive=TRUE;//Активно ли приложение в данный момент
HCURSOR hStandartCursor=NULL;

static HINSTANCE hInstance=0;
static BOOL g_bCanDoMultiTexture=FALSE;

extern LPDIRECTDRAWSURFACE7	m_pddsBackBufferSM;
/////Balmer Code
void InitIsMultiTextureSupport();

HRESULT RestoreAll()
{
	if(!d3dIsActive())
	while(TRUE)
	{
		MSG  msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else
		if(!d3dIsActive() && g_bSleepIfNonActive)
        {
            WaitMessage();
        }else
			break;
	}

	if(g_pDD==NULL)return MD3D_OK;
	HRESULT hr=DD_OK;
	bool MustRestore=false;
	if(FAILED(hr=g_pDD->TestCooperativeLevel()))
		switch(hr)
		{
			case DDERR_EXCLUSIVEMODEALREADYSET:
			case DDERR_NOEXCLUSIVEMODE:
				return S_OK;
			default:
//				_RPT0( _CRT_ERROR,"No cooperative level");
				return S_OK;
		}

    if( g_pddsFrontBuffer )
        if( g_pddsFrontBuffer->IsLost() )
		{
			MustRestore=true;
            hr=g_pddsFrontBuffer->Restore();
		}
	if(hr!=DD_OK)
		return hr;

    if( g_pddsBackBuffer )
        if( g_pddsBackBuffer->IsLost() )
		{
			MustRestore=true;
            hr=g_pddsBackBuffer->Restore();
		}
	if(hr!=DD_OK)
		return hr;

    if( m_pddsZBuffer )
        if( m_pddsZBuffer->IsLost() )
		{
			MustRestore=true;
            hr=m_pddsZBuffer->Restore();
		}
	if(hr!=DD_OK)
		return hr;

	return hr;
}
//-----------------------------------------------------------------------------
// Local function-prototypes
//-----------------------------------------------------------------------------
LRESULT CALLBACK d3dWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
HRESULT ConfirmDeviceCallBack( DDCAPS*, D3DDEVICEDESC* );
HRESULT RestoreSurfaces();

HRESULT Initialize3D( HWND, GUID*, GUID*, DDSURFACEDESC2*, DWORD ,BOOL InitAllManager);
//HRESULT Change3DEnvironment( HWND );
//HRESULT Render3DEnvironment();

HRESULT CreateEnvironment( GUID*, GUID*, DDSURFACEDESC2*, DWORD ,BOOL InitAllManager);
HRESULT CreateBuffers( DDSURFACEDESC2*, DWORD );
HRESULT CreateZBuffer(GUID* pDeviceGUID);

HRESULT WINAPI EnumZBufferFormatsCallback( DDPIXELFORMAT*, VOID* );

HRESULT Cleanup3D();
HRESULT DestroyObjects(BOOL bReInit=FALSE);

BOOL d3dIsActive(){return g_bActive;};
//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------
extern MD3DERROR __d3dInitTextureManager();
extern MD3DERROR __d3dInitSpriteManager();
extern MD3DERROR __d3dInitGammaSupport();
extern MD3DERROR __d3dCloseSpriteManager();
extern MD3DERROR __d3dCloseTextureManager();
extern MD3DERROR __d3dCloseGammaSupport();
extern HRESULT __TSS_InitPipeline();

void __d3dStartLogging();
void __d3dStopLogging();

#define RENDER_CLASS_NAME "RenderWindowClass"

//////////////////////////d3dEnumVideoMode/////////////////////
MD3DERROR d3dEnumVideoMode(int* numvideomode,MD3DMODE** array)
{
	*numvideomode=0;
	*array=NULL;
	D3DEnum_DriverInfo* pDriverInfo;
	D3DEnum_DeviceInfo* pDeviceInfo;
	bool bMustFree=false;

	if(D3DEnum_GetSelectedDriver(&pDriverInfo,&pDeviceInfo)!=S_OK)
	{
		HRESULT hr;
		// Enumerate available D3D devices
		if( FAILED( hr = D3DEnum_EnumerateDevices() ) )
		{
			switch( hr ) {
			case D3DENUMERR_NODIRECTDRAW:
				return MD3DERR_NODIRECTDRAW;

			case D3DENUMERR_ENUMERATIONFAILED:
				return MD3DERR_ENUMERATIONFAILED;

			default:
				return MD3DERR_GENERIC;
			}
		}
		DWORD dwSelectFlags = D3DENUM_PRIMARYHAL;

		if( FAILED( hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, 0, 0, 0, ConfirmDeviceCallBack ) ) )
		{
			if( hr == D3DENUMERR_NOTFOUND || hr == D3DENUMERR_NOCOMPATIBLEDEVICES ) {
				// Switch the primary/secondary flags
				if( dwSelectFlags == D3DENUM_PRIMARYHAL ) {
					dwSelectFlags = D3DENUM_SECONDARYHAL;
				} else {
					dwSelectFlags = D3DENUM_PRIMARYHAL;
				}
			
				// Try again
				hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, 0, 0, 0, ConfirmDeviceCallBack );

				if(FAILED(hr))
				{
					dwSelectFlags = D3DENUM_RGBEMULATION;
					hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, 0, 0, 0, ConfirmDeviceCallBack );
				}
			}
		}

		// Bail out if still no luck
		if( FAILED( hr ) ) { 
			switch( hr ) {
			case D3DENUMERR_NOTFOUND:
			case D3DENUMERR_NOCOMPATIBLEDEVICES:

				return MD3DERR_NOCOMPATIBLEDEVICES;

			default:
				return MD3DERR_GENERIC;
			}
		}

		if(D3DEnum_GetSelectedDriver(&pDriverInfo,&pDeviceInfo)!=S_OK)
			return MD3DERR_GENERIC;
		bMustFree=true;
	}

	D3DEnum_ModeInfo* pMode;
	for(pMode=pDeviceInfo->pFirstMode;pMode;pMode=pMode->pNext)
	{
		(*numvideomode)++;
	}

	*array=new MD3DMODE[*numvideomode];
	int i=0;
	for(pMode=pDeviceInfo->pFirstMode;pMode;pMode=pMode->pNext)
	{
		MD3DMODE& m=(*array)[i];
		m.dx=pMode->ddsd.dwWidth;
		m.dy=pMode->ddsd.dwHeight;
		m.bitperpixel=pMode->ddsd.ddpfPixelFormat.dwRGBBitCount;

		i++;
	}

	if(bMustFree)
		D3DEnum_FreeResources();

	return MD3D_OK;
}

//////////////////////////GetAvailableVidMem////////////////
#include <stdio.h>
MD3DERROR d3dGetAvailableVidMem(DWORD* allvideomem)
{
	*allvideomem=0;
	if(g_pDD==NULL)
		return MD3DERR_GENERIC;

	DDSCAPS2      ddsCaps2; 
	DWORD         dwTotal; 
	DWORD         dwFree;
	HRESULT       hr; 
 
	// Initialize the structure.
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
 
	ddsCaps2.dwCaps = DDSCAPS_3DDEVICE|DDSCAPS_TEXTURE|DDSCAPS_VIDEOMEMORY|
		DDSCAPS_LOCALVIDMEM; 
	hr = g_pDD->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree); 
	if (FAILED(hr))
		return hr;
	*allvideomem=dwTotal;

/*	
	DWORD         dwTotal1; 
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
 
	ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
	hr = g_pDD->GetAvailableVidMem(&ddsCaps2, &dwTotal1, &dwFree); 
	if (FAILED(hr))
		return hr;

	char s[256];
	sprintf(s,"Local video memory=%i\nAll video memety=%i",dwTotal,dwTotal1);
	MessageBox(NULL,s,"Info",MB_OK);
*/
	return MD3D_OK;
}

//-----------------------------------------------------------------------------
// Name: d3dInit
// Desc: Window creation, D3D initialization
//-----------------------------------------------------------------------------
MD3DERROR d3dInitX(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle,
				   BOOL InitAllManager)
{
 	_ASSERTE( !g_bInitialized );

	hInstance=hInst;
	hStandartCursor=LoadCursor(NULL,(LPCSTR)OCR_NORMAL);

	DWORD dwSelectFlags;
    HRESULT hr;

	// Start logging
	__d3dStartLogging();

	g_bIsFullscreen=dwInitFlags&MD3D_FULLSCREEN;
	if(!g_bIsFullscreen)
	{
		DWORD e_width,e_height,e_bpp;
		hr=d3dGetDisplayMode(e_width,e_height,e_bpp);
		if(hr==MD3D_OK)
		{
			dwBpp=e_bpp;
		}
	}

	MD3DLOG4( 1, "d3dInit: dwWidth=%d, dwHeight=%d, dwBpp=%d, dwInitFlags=0x%X\n", dwWidth, dwHeight, dwBpp, dwInitFlags );

    // Enumerate available D3D devices
    if( FAILED( hr = D3DEnum_EnumerateDevices() ) )
    {
        switch( hr ) {
		case D3DENUMERR_NODIRECTDRAW:
			return MD3DERR_NODIRECTDRAW;

		case D3DENUMERR_ENUMERATIONFAILED:
			return MD3DERR_ENUMERATIONFAILED;

		default:
			return MD3DERR_GENERIC;
		}
    }

	// Set flags for device selection
	if( dwInitFlags & MD3D_ALTDEVICE ) {
		dwSelectFlags = D3DENUM_SECONDARYHAL;
	} else {
		dwSelectFlags = D3DENUM_PRIMARYHAL;
	}

	// Select driver, device and mode, passing a callback that allows devices
    // to be accepted/rejected based on what capabilities the app requires.
    if( FAILED( hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, dwWidth, dwHeight, dwBpp, ConfirmDeviceCallBack ) ) )
    {
		if( hr == D3DENUMERR_NOTFOUND || hr == D3DENUMERR_NOCOMPATIBLEDEVICES ) {
			// Switch the primary/secondary flags
			if( dwSelectFlags == D3DENUM_PRIMARYHAL ) {
				dwSelectFlags = D3DENUM_SECONDARYHAL;
			} else {
				dwSelectFlags = D3DENUM_PRIMARYHAL;
			}
		
			// Try again
			hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, dwWidth, dwHeight, dwBpp, ConfirmDeviceCallBack );

			if(FAILED(hr))
			{
				dwSelectFlags = D3DENUM_RGBEMULATION;
				hr = D3DEnum_SelectDefaultDriver( dwSelectFlags, dwWidth, dwHeight, dwBpp, ConfirmDeviceCallBack );
			}
		}
	}

	// Bail out if still no luck
	if( FAILED( hr ) ) { 
		switch( hr ) {
		case D3DENUMERR_NOTFOUND:
		case D3DENUMERR_NOCOMPATIBLEDEVICES:

			return MD3DERR_NOCOMPATIBLEDEVICES;

		default:
			return MD3DERR_GENERIC;
		}
	}

	// Create the focus/device window 

	int NoFullscreenWidth = dwWidth+
		GetSystemMetrics(SM_CXSIZEFRAME)*2-2;
    int NoFullscreenHeight = dwHeight+
                     GetSystemMetrics(SM_CYSIZEFRAME) * 2 +
                     GetSystemMetrics(SM_CYCAPTION)-2;

	int x=(GetSystemMetrics(SM_CXSCREEN)-NoFullscreenWidth)/2;
	int y=(GetSystemMetrics(SM_CYSCREEN)-NoFullscreenHeight)/2;

	int wnd_styles;

	g_dwRenderWidth=dwWidth;
	g_dwRenderHeight=dwHeight;


	if(InitAllManager || true)
	{
		if(m_hRenderWnd)
		{
			HWND wnd=m_hRenderWnd;
			m_hRenderWnd=NULL;
			DestroyWindow(wnd);
		}

		// Register the window class
		WNDCLASS wndClass;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = d3dWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInst;
		wndClass.hIcon = hIcon;
		wndClass.hCursor = NULL;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = RENDER_CLASS_NAME;
		RegisterClass( &wndClass );

		wnd_styles = (!g_bIsFullscreen) ? WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CAPTION : 0;

		m_hRenderWnd = CreateWindow( "RenderWindowClass", szTitle,
							          WS_POPUP | WS_VISIBLE | wnd_styles,
								  g_bIsFullscreen?0:x,
								  g_bIsFullscreen?0:y, 
								  g_bIsFullscreen?1:NoFullscreenWidth, 
								  g_bIsFullscreen?1:NoFullscreenHeight, 
								  0L, 0L, hInst, 0L );

		
		ShowWindow( m_hRenderWnd, SW_SHOWNORMAL );
		UpdateWindow( m_hRenderWnd );
	}else
	{
		MoveWindow(m_hRenderWnd,
				g_bIsFullscreen?0:x,
				g_bIsFullscreen?0:y,
				g_bIsFullscreen?1:NoFullscreenWidth,
				g_bIsFullscreen?1:NoFullscreenHeight,
				TRUE);
	}


    // Finally, initialize the framework and scene.

    D3DEnum_DriverInfo* pDriverInfo;
    D3DEnum_DeviceInfo* pDeviceInfo;

    D3DEnum_GetSelectedDriver( &pDriverInfo, &pDeviceInfo );

    // Initialize the D3D framework
    if( FAILED( hr = Initialize3D( m_hRenderWnd, &pDriverInfo->guid,
                       &pDeviceInfo->guid, &pDeviceInfo->pCurrentMode->ddsd, 
                       dwInitFlags,InitAllManager ) ) )
	{
		return hr;
	}

	// Setup default TSS configuration depending on 3D chipset
	if( FAILED( hr = __TSS_InitPipeline() ) )
	{
		return hr;
	}

	// Init globals 
	g_dwSceneStamp = 0;

	g_bInitialized = TRUE;
	g_bActive = TRUE;
	InitIsMultiTextureSupport();

	return MD3D_OK;
}

MD3DERROR d3dInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle)
{
	return d3dInitX(dwWidth,dwHeight,dwBpp,dwInitFlags,hInst,hIcon,szTitle,TRUE);
}

//-----------------------------------------------------------------------------
// Name: d3dClose()
// Desc: Close the render window and cleanup 3D environment
//-----------------------------------------------------------------------------
MD3DERROR d3dClose()
{
	_ASSERTE( g_bInitialized );

	MD3DLOG( 1, "d3dClose\n" );

	HRESULT hr;
	hr = Cleanup3D();

	MD3DLOG( 1, "Cleanup3D - OK\n" );
	// Close the window
//???Balmer	DestroyWindow( m_hRenderWnd );
//	MD3DLOG( 1, "DestroyWindow - OK\n" );


	// Stop logging
	__d3dStopLogging();

	g_bInitialized = FALSE;

//???Balmer	UnregisterClass(RENDER_CLASS_NAME,hInstance);
//	MD3DLOG( 1, "UnregisterClass- OK\n" );
	return hr;
}



//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: This is the basic Windows-programming function that processes
//       Windows messages. We need to handle window movement, painting,
//       and destruction.
//-----------------------------------------------------------------------------
LRESULT CALLBACK d3dWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
//        case WM_PAINT:
//            if( g_bInitialized )
//            {
                // If we are paused, and in fullscreen mode, give the dialogs
                // a GDI surface to draw on.
//                if( !g_bReady && !g_bWindowed)
//                    ;//FlipToGDISurface( TRUE );
//                else // Simply repaint the frame's contents
//                    ;//ShowFrame();
//            }
//            break;

        case WM_ACTIVATEAPP:
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
			if( g_bActive )
				MD3DLOG(4, "Window activated\n");
			else
				MD3DLOG(4, TEXT( "Window de-activated\n" ) );
            return 0L;
        case WM_SIZE:
            // Check to see if we are losing our window...
            if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
                g_bActive = FALSE;
            else g_bActive = TRUE;

			if( g_bActive )
				MD3DLOG(4, "Window activated\n");
			else
				MD3DLOG(4, TEXT( "Window de-activated\n" ) );

            break;
		case WM_MOVE:
			GetClientRect( m_hRenderWnd, &g_rcScreenRect );
			ClientToScreen( m_hRenderWnd, (POINT*)&g_rcScreenRect.left );
			g_rcScreenRect.right=g_rcScreenRect.left+g_dwRenderWidth;
			g_rcScreenRect.bottom=g_rcScreenRect.top+g_dwRenderHeight;
			break;

        case WM_SETCURSOR:
			{
				if(g_bIsFullscreen)
				{
					SetCursor(NULL);
					return TRUE;
				}

				POINT pos;
				RECT rc;
				GetWindowRect(m_hRenderWnd,&rc);
				rc.bottom=rc.top+GetSystemMetrics(SM_CYSIZEFRAME)+
                     GetSystemMetrics(SM_CYCAPTION);
				GetCursorPos(&pos);
				if(!PtInRect(&rc,pos))
				{
					SetCursor(NULL);
					return TRUE;
				}
				
				SetCursor(hStandartCursor);
			}
 
			break;

        case WM_DESTROY:
			if(hWnd!=m_hRenderWnd)
				break;
            PostQuitMessage(0);
            return 0L;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case SC_MONITORPOWER:
                    // Prevent potential crashes when the monitor powers down
                    return 1;
            }
            break;
/*
		case WM_GETMINMAXINFO:
			if(g_bIsFullscreen)
			{
				LPMINMAXINFO lpmmi = (LPMINMAXINFO) lParam; 
				POINT max_size={1,1};
				lpmmi->ptReserved=
				lpmmi->ptMaxSize=
				lpmmi->ptMaxPosition=
				lpmmi->ptMinTrackSize=
				lpmmi->ptMaxTrackSize=max_size;

			}
			break;
*/
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: d3dGetWindowHandle()
// Desc: Return window handle for the device/focus window
//-----------------------------------------------------------------------------
MD3DERROR d3dGetWindowHandle( HWND *hWnd )
{
	_ASSERTE( g_bInitialized );

	*hWnd = m_hRenderWnd;
	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: ConfirmDeviceCallBack()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT ConfirmDeviceCallBack( DDCAPS* pddDriverCaps,
								D3DDEVICEDESC* pd3dDeviceDesc )
{

	// Check driver caps
	if( (pddDriverCaps->dwCaps & DDCAPS_3D) == 0 ||
		(pddDriverCaps->dwCaps & DDCAPS_NOHARDWARE) != 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: no DDCAPS_3D or DDCAPS_NOHARDWARE\n" );
		return E_FAIL;
	}

	// General device caps
	if( (pd3dDeviceDesc->dwFlags & D3DDD_DEVCAPS) == 0 ||
		(pd3dDeviceDesc->dwDevCaps & D3DDEVCAPS_DRAWPRIMTLVERTEX) == 0 ||
		(pd3dDeviceDesc->dwDevCaps & D3DDEVCAPS_FLOATTLVERTEX) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: no D3DDEVCAPS_DRAWPRIMTLVERTEX or no D3DDEVCAPS_FLOATTLVERTEX\n" );
		return E_FAIL;
	}

	// Min texture size
	if( pd3dDeviceDesc->dwMinTextureWidth > 64 || 
		pd3dDeviceDesc->dwMinTextureHeight > 64 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: Min texture size doesn't suit us.\n" );
		return E_FAIL;
	}

	// Max texture size
	if( pd3dDeviceDesc->dwMaxTextureWidth < 256 || 
		pd3dDeviceDesc->dwMaxTextureHeight < 256 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: Max texture size doesn't suit us.\n" );
		return E_FAIL;
	}

	// Texture ops
	if( (pd3dDeviceDesc->dwTextureOpCaps & 
		(D3DTOP_DISABLE|D3DTOP_MODULATE|D3DTOP_SELECTARG1)) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: no D3DTOP_DISABLE|D3DTOP_MODULATE|D3DTOP_SELECTARG1\n" );
		return E_FAIL;
	}

    // Triangle Caps

	if( (pd3dDeviceDesc->dwFlags & D3DDD_TRICAPS) == 0 ) {
		MD3DLOG( 2, "[Device Enum]: Device rejected: no D3DDD_TRICAPS\n" );
		return E_FAIL;
	}

    LPD3DPRIMCAPS pdpcTriCaps = &pd3dDeviceDesc->dpcTriCaps;

	// Z compare func
	if( (pdpcTriCaps->dwZCmpCaps & 
		(D3DPCMPCAPS_LESSEQUAL|D3DPCMPCAPS_ALWAYS)) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: ZFUNC no D3DPCMPCAPS_LESSEQUAL|D3DPCMPCAPS_ALWAYS\n" );
		return E_FAIL;
	}

	// Alpha blend caps (SrcBlend)
	if( (pdpcTriCaps->dwSrcBlendCaps &
		(D3DPBLENDCAPS_SRCALPHA|D3DPBLENDCAPS_ONE|D3DPBLENDCAPS_ZERO)) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: SRCBLEND no D3DPBLENDCAPS_SRCALPHA|D3DPBLENDCAPS_ONE|D3DPBLENDCAPS_ZERO\n" );
		return E_FAIL;
	}

	// Alpha blend caps (DestBlend)
	if( (pdpcTriCaps->dwDestBlendCaps &
		(D3DPBLENDCAPS_SRCALPHA|D3DPBLENDCAPS_INVSRCALPHA|D3DPBLENDCAPS_ONE)) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: DESTBLEND no D3DPBLENDCAPS_SRCALPHA|D3DPBLENDCAPS_INVSRCALPHA|D3DPBLENDCAPS_ONE\n" );
		return E_FAIL;
	}

	// Alpha test (func) caps
	if( (pdpcTriCaps->dwAlphaCmpCaps &
		(D3DPCMPCAPS_GREATEREQUAL|D3DPCMPCAPS_LESS|D3DPCMPCAPS_ALWAYS)) == 0 ) {

		MD3DLOG( 2, "[Device Enum]: Device rejected: ALPHAFUNC no D3DPCMPCAPS_GREATEREQUAL|D3DPCMPCAPS_LESS|D3DPCMPCAPS_ALWAYS\n" );
		return E_FAIL;
	}

	// dwShadeCaps BUGBUG
	// dwTextureCaps, dwTextureFilterCaps ...

	// All ok
	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Initialize3D()
// Desc: Creates the internal objects 
//-----------------------------------------------------------------------------
HRESULT Initialize3D( HWND hWnd, GUID* pDriverGUID, 
                    GUID* pDeviceGUID, DDSURFACEDESC2* pMode,
					DWORD dwFlags,BOOL InitAllManager )
{
	HRESULT hr;


    // Setup state for windowed/fullscreen mode
    //m_bIsFullscreen = ( dwFlags & MD3D_FULLSCREEN ) ? TRUE : FALSE;
 
    // Create the D3D rendering environment (surfaces, device, viewport, etc.)
    if( FAILED( hr = CreateEnvironment( pDriverGUID, pDeviceGUID, pMode,
		                                dwFlags,InitAllManager ) ) )
	{
		DestroyObjects();
		return hr;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CreateEnvironment()
// Desc: Creates the internal objects for the framework
//-----------------------------------------------------------------------------
HRESULT CreateEnvironment(GUID* pDriverGUID,GUID* pDeviceGUID,DDSURFACEDESC2* pMode,DWORD dwFlags,BOOL InitAllManager)
{
    HRESULT hr;

    // Select the default memory type, for whether the device is HW or SW
    if( IsEqualIID( *pDeviceGUID, IID_IDirect3DHALDevice) )
        m_dwDeviceMemType = DDSCAPS_VIDEOMEMORY;
    else if( IsEqualIID( *pDeviceGUID, IID_IDirect3DTnLHalDevice) )
        m_dwDeviceMemType = DDSCAPS_VIDEOMEMORY;
    else
        m_dwDeviceMemType = DDSCAPS_SYSTEMMEMORY;
	// Create the DDraw object
	if(g_pDD==0)
		if(DirectDrawCreateEx(pDriverGUID,(VOID**)&g_pDD,IID_IDirectDraw7,0))
		{
			DEBUG_MSG( TEXT("Could not create DirectDraw") );
			return MD3DERR_NODIRECTDRAW;
		}
	if(FAILED(g_pDD->SetCooperativeLevel(m_hRenderWnd,DDSCL_FPUPRESERVE |((g_bIsFullscreen)?DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN:DDSCL_NORMAL))))
//	if(FAILED(g_pDD->SetCooperativeLevel(m_hRenderWnd,((g_bIsFullscreen)?DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN:DDSCL_NORMAL))))
    {
        DEBUG_MSG( TEXT("Couldn't set coop level") );
        return MD3DERR_GENERIC;
    }

	// Create the front and back buffers, and attach a clipper
	if( FAILED( hr = CreateBuffers( pMode, dwFlags ) ) )
		return hr;

	if(pDeviceGUID==0) return MD3DERR_GENERIC;
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	g_pDD->GetDisplayMode( &ddsd );
	if( ddsd.ddpfPixelFormat.dwRGBBitCount <= 8 )
		return MD3DERR_INVALIDMODE;

	if( FAILED( g_pDD->QueryInterface( IID_IDirect3D7, (VOID**)&m_pD3D ) ) )
	{ // Query DirectDraw for access to Direct3D
		DEBUG_MSG( TEXT("Couldn't query the Direct3D interface") );
		return MD3DERR_NODIRECT3D;
	}

	// Create and attach the zbuffer
	if( FAILED( hr = CreateZBuffer(pDeviceGUID) ) )
		return hr;

	if(FAILED(m_pD3D->CreateDevice(IID_IDirect3DTnLHalDevice,g_pddsBackBuffer,&g_pd3dDevice)))
		if(FAILED(m_pD3D->CreateDevice(*pDeviceGUID,g_pddsBackBuffer,&g_pd3dDevice)))
		{
			DEBUG_MSG( TEXT("Couldn't create the D3DDevice") );
			return MD3DERR_NODIRECT3D;
		}
	D3DVIEWPORT7 vp = {0,0,g_dwRenderWidth,g_dwRenderHeight,0,1};
	if(FAILED(g_pd3dDevice->SetViewport(&vp)))
	{
		DEBUG_MSG( TEXT("Error: Couldn't set current viewport to device") );
		return MD3DERR_GENERIC;
	}
	
	if(InitAllManager)
	{
		// Initialize texture manager
		if( FAILED( hr = __d3dInitTextureManager() ) )
			return hr;

		// Initialize sprite manager
		if( FAILED( hr = __d3dInitSpriteManager() ) )
			return hr;

	}

	// Initialize gamma support
	if( FAILED( hr = __d3dInitGammaSupport() ) )
		return hr;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: EnumZBufferFormatsCallback()
// Desc: Enumeration function to report valid pixel formats for z-buffers.
//-----------------------------------------------------------------------------
HRESULT WINAPI EnumZBufferFormatsCallback( DDPIXELFORMAT* pddpf,
                                                  VOID* pddpfDesired )
{
    if( NULL==pddpf || NULL==pddpfDesired )
        return D3DENUMRET_CANCEL;

    // If the current pixel format's match the desired ones (DDPF_ZBUFFER and
    // possibly DDPF_STENCILBUFFER), lets copy it and return. This function is
    // not choosy...it accepts the first valid format that comes along.
    if( pddpf->dwFlags == ((DDPIXELFORMAT*)pddpfDesired)->dwFlags )
    {
        memcpy( pddpfDesired, pddpf, sizeof(DDPIXELFORMAT) );

		// We're happy with a 16-bit z-buffer. Otherwise, keep looking.
		if( pddpf->dwZBufferBitDepth == 16 )
			return D3DENUMRET_CANCEL;
    }

    return D3DENUMRET_OK;
}

    
    
//-----------------------------------------------------------------------------
// Name: CreateBuffers()
// Desc: Creates the primary and (optional) backbuffer for rendering.
//       Windowed mode and fullscreen mode are handled differently.
//-----------------------------------------------------------------------------
HRESULT CreateBuffers( DDSURFACEDESC2* pddsd, DWORD dwFlags )
{
    HRESULT hr;

	if( dwFlags & MD3D_FULLSCREEN )
	{
		// Get the dimensions of the viewport and screen bounds
		// Store the rectangle which contains the renderer
		SetRect( &g_rcViewportRect, 0, 0, pddsd->dwWidth, pddsd->dwHeight );
		memcpy( &g_rcScreenRect, &g_rcViewportRect, sizeof(RECT) );
   
		// Set the display mode to the requested dimensions. Check for
		// 320x200x8 modes, and set flag to avoid using ModeX
		DWORD dwModeFlags = 0;

		if( (320==g_dwRenderWidth) && (200==g_dwRenderHeight) && 
			(8==pddsd->ddpfPixelFormat.dwRGBBitCount) )
			dwModeFlags |= DDSDM_STANDARDVGAMODE;

		if( FAILED( g_pDD->SetDisplayMode( g_dwRenderWidth, g_dwRenderHeight,
									pddsd->ddpfPixelFormat.dwRGBBitCount,
									pddsd->dwRefreshRate, dwModeFlags ) ) )
		{
			DEBUG_MSG( TEXT("Can't set display mode") );
			return MD3DERR_BADDISPLAYMODE;
		}

		// Create the primary surface
		DDSURFACEDESC2 ddsd;
		D3DUtil_InitSurfaceDesc( ddsd, DDSD_CAPS );
		ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE|DDSCAPS_3DDEVICE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
		ddsd.dwFlags			= DDSD_BACKBUFFERCOUNT;
		ddsd.dwBackBufferCount	= 1;

		if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsFrontBuffer, NULL ) ) )
		{
			DEBUG_MSG( TEXT("Error: Can't create primary surface") );
			if( hr != DDERR_OUTOFVIDEOMEMORY )
				return MD3DERR_GENERIC;
			DEBUG_MSG( TEXT("Error: Out of video memory") );
			return DDERR_OUTOFVIDEOMEMORY;
		}

		// Get the backbuffer. For fullscreen mode, the backbuffer was created
		// along with the primary, but windowed mode still needs to create one.
		// Get a ptr to the back buffer, which will be our render target
		DDSCAPS2 ddscaps = { DDSCAPS_BACKBUFFER, 0, 0, 0 };
		if(FAILED(hr=g_pddsFrontBuffer->GetAttachedSurface(&ddscaps,&g_pddsBackBuffer)))
		{
			DEBUG_MSG( TEXT("Error: Can't get/create the backbuffer") );
			if( hr != DDERR_INVALIDOBJECT )
				DEBUG_MSG( TEXT("Error: DDERR_INVALIDOBJECT") );
			if( hr != DDERR_INVALIDPARAMS )
				DEBUG_MSG( TEXT("Error: DDERR_INVALIDPARAMS") );
			if( hr != DDERR_NOTFOUND )
				DEBUG_MSG( TEXT("Error: DDERR_NOTFOUND") );
			if( hr != DDERR_SURFACELOST )
				DEBUG_MSG( TEXT("Error: DDERR_SURFACELOST") );
			return MD3DERR_GENERIC;
		}
	}
	else // Set up buffers for windowed rendering
	{
		// Get the dimensions of the viewport and screen bounds
		GetClientRect( m_hRenderWnd, &g_rcViewportRect );
		g_rcViewportRect.right=g_rcViewportRect.left+g_dwRenderWidth;
		g_rcViewportRect.bottom=g_rcViewportRect.top+g_dwRenderHeight;

		g_rcScreenRect=g_rcViewportRect;
		ClientToScreen( m_hRenderWnd, (POINT*)&g_rcScreenRect.left );
		ClientToScreen( m_hRenderWnd, (POINT*)&g_rcScreenRect.right );
    
		// Create the primary surface
		DDSURFACEDESC2 ddsd;
		D3DUtil_InitSurfaceDesc( ddsd, DDSD_CAPS );
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    
		if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsFrontBuffer, NULL ) ) )
		{
			DEBUG_MSG( TEXT("Error: Can't create primary surface") );
			if( hr != DDERR_OUTOFVIDEOMEMORY )
				return MD3DERR_GENERIC;
			DEBUG_MSG( TEXT("Error: Out of video memory") );
			return DDERR_OUTOFVIDEOMEMORY;
		}

		// If in windowed-mode, create a clipper object
		LPDIRECTDRAWCLIPPER pcClipper;
		if( FAILED( hr = g_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
		{
			DEBUG_MSG( TEXT("Error: Couldn't create clipper") );
			return MD3DERR_GENERIC;
		}

		// Associate the clipper with the window
		pcClipper->SetHWnd( 0, m_hRenderWnd );
		g_pddsFrontBuffer->SetClipper( pcClipper );
		SAFE_RELEASE( pcClipper );

		// Get the backbuffer. For fullscreen mode, the backbuffer was created
		// along with the primary, but windowed mode still needs to create one.
		// Create the back buffer (the render target)   
		ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddsd.dwWidth        = g_dwRenderWidth;
		ddsd.dwHeight       = g_dwRenderHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;

		if(FAILED(hr=g_pDD->CreateSurface(&ddsd,&g_pddsBackBuffer,NULL)))
		{
			DEBUG_MSG( TEXT("Error: Couldn't create the backbuffer") );
			if( hr != DDERR_OUTOFVIDEOMEMORY )
				return MD3DERR_GENERIC;
			DEBUG_MSG( TEXT("Error: Out of video memory") );
			return DDERR_OUTOFVIDEOMEMORY;
		}
	}
	// Get the pixel format of the primary
	DDSURFACEDESC2 ddsd1;
	D3DUtil_InitSurfaceDesc( ddsd1, 0 );

	if( FAILED( g_pddsFrontBuffer->GetSurfaceDesc( &ddsd1 ) ) ) {
		DEBUG_MSG( TEXT("Error: Can't get surface description for the primary surface") );
		return MD3DERR_GENERIC;
	}

	memcpy( &g_ddpfPrimary, &ddsd1.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );

    return S_OK;
}

HRESULT CreateZBuffer(GUID* pDeviceGUID)
{
	HRESULT hr;

    // Check if the device supports z-bufferless hidden surface removal. If so,
    // we don't really need a z-buffer
    DWORD dwRasterCaps = g_ddDeviceDesc.dpcTriCaps.dwRasterCaps;
    if( dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
        return S_OK;

    // Get z-buffer dimensions from the render target
    // Setup the surface desc for the z-buffer.
    DDSURFACEDESC2 ddsd;
    D3DUtil_InitSurfaceDesc(ddsd,DDSD_WIDTH|DDSD_HEIGHT|DDSD_CAPS|DDSD_PIXELFORMAT);
    ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | m_dwDeviceMemType;
    ddsd.dwWidth        = g_dwRenderWidth;
    ddsd.dwHeight       = g_dwRenderHeight;
	ZeroMemory(&ddsd.ddpfPixelFormat,sizeof(DDPIXELFORMAT));
	ddsd.ddpfPixelFormat.dwFlags = DDPF_ZBUFFER;
    m_pD3D->EnumZBufferFormats(*pDeviceGUID,EnumZBufferFormatsCallback,(VOID*)&ddsd.ddpfPixelFormat);
    if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &m_pddsZBuffer, NULL ) ) )
    {
        DEBUG_MSG( TEXT("Error: Couldn't create a ZBuffer surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return MD3DERR_GENERIC;
		DEBUG_MSG( TEXT("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
    }
	if( FAILED( g_pddsBackBuffer->AddAttachedSurface( m_pddsZBuffer ) ) )
    {
        DEBUG_MSG( TEXT("Error: Couldn't attach zbuffer to render surface") );
        return MD3DERR_GENERIC;
    }
    return S_OK;
}
HRESULT Cleanup3D()
{
	HRESULT hr;
    if(g_bInitialized==FALSE) return S_OK;
	__d3dCloseSpriteManager();	// Shutdown sprite manager
	__d3dCloseTextureManager();	// Shutdown texture manager
	__d3dCloseGammaSupport();	// Shutdown gamma support
	hr = DestroyObjects();		// Destroy all DD and D3D objects
    D3DEnum_FreeResources();
	return hr;
}
HRESULT DestroyObjects(BOOL bReInit)
{
    LONG nDD  = 0L; // Number of outstanding DDraw references
    LONG nD3D = 0L; // Number of outstanding D3DDevice references
    // Do a safe check for releasing the D3DDEVICE. RefCount must be zero.
    if( g_pDD ) g_pDD->SetCooperativeLevel( m_hRenderWnd, DDSCL_FPUPRESERVE |DDSCL_NORMAL );
//    if( g_pDD ) g_pDD->SetCooperativeLevel( m_hRenderWnd,DDSCL_NORMAL );
    if( g_pd3dDevice )
        if( 0 < ( nD3D = g_pd3dDevice->Release() ) )
            DEBUG_MSG( TEXT("Error: D3DDevice object is still referenced!") );
	g_pd3dDevice = NULL;
    SAFE_RELEASE( g_pddsBackBuffer );
    SAFE_RELEASE( m_pddsZBuffer );
    SAFE_RELEASE( g_pddsFrontBuffer );
    SAFE_RELEASE( m_pD3D );
	if(g_pDD&&bReInit==FALSE)
	{ // Do a safe check for releasing DDRAW. RefCount must be zero.
		g_pDD->RestoreDisplayMode(); 
        if( 0 < ( nDD=g_pDD->Release() ) )
            DEBUG_MSG( TEXT("Error: DirectDraw object is still referenced!"));
		g_pDD=0;
	}
    return ( nDD==0 && nD3D==0 ) ? S_OK : MD3DERR_GENERIC;
}


//-----------------------------------------------------------------------------
// Name: RestoreSurfaces()
// Desc: Checks for lost surfaces and restores them if lost. Note: Don't
//       restore render surface, since it's just a duplicate ptr.
//-----------------------------------------------------------------------------
HRESULT RestoreSurfaces()
{
	if( NULL == g_bInitialized )
		return MD3DERR_NOTINITIALIZED;

	HRESULT hr=0;
	if(FAILED(hr=g_pDD->RestoreAllSurfaces()))
	{
		DEBUG_MSG( TEXT( "RestoreSurfaces(): error IDirectDraw7::RestoreAllSurfaces()\n" ) );
		return hr;
	}
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dTestCooperativeLevel()
// Desc: Test if we have exclusive access to device
//-----------------------------------------------------------------------------
MD3DERROR d3dTestCooperativeLevel()
{
	if( NULL == g_bInitialized )
		return MD3DERR_NOTINITIALIZED;

	HRESULT hr;
	if( FAILED( hr = g_pDD->TestCooperativeLevel() ) )
	{
		if( DDERR_NOEXCLUSIVEMODE == hr )
		{	
			DEBUG_MSG( TEXT( "d3dTestCooperativeLevel(): exclusive access was lost\n" ) );
			return MD3DERR_NOEXCLUSIVEMODE;
		}

		DEBUG_MSG( TEXT( "d3dTestCooperativeLevel(): there was an error while testing for cooperative level\n" ) );
		return MD3DERR_GENERIC;
	}

	//BUGBUG
	//DEBUG_MSG( TEXT( "d3dTestCooperativeLevel(): OK\n" ) );

	return MD3D_OK;
}

//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT Render3DEnvironment()
{
    // Check the cooperative level before rendering
//    if( FAILED( g_pFramework->GetDirectDraw()->TestCooperativeLevel() ) )
//        return S_OK;

    //Render the scene

    // Show the frame on the primary surface.
//    if( DDERR_SURFACELOST == g_pFramework->ShowFrame() )
//    {
//        g_pFramework->RestoreSurfaces();
//    }

    return S_OK;
}


MD3DERROR d3dGetDisplayMode(DWORD& width,DWORD& height,DWORD& bpp)
{

	LPDIRECTDRAW g_pDD1;
    HRESULT hr = DirectDrawCreate( NULL, &g_pDD1, NULL );
    if( FAILED( hr ) )
        return hr;

	LPDIRECTDRAW7 g_pDD4;
    hr = g_pDD1->QueryInterface( IID_IDirectDraw7, (VOID**)&g_pDD4 );
    if( FAILED( hr ) )
        return hr;
 
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
    g_pDD4->GetDisplayMode(&ddsd);

	height=ddsd.dwHeight;
	width=ddsd.dwWidth;
	bpp=ddsd.ddpfPixelFormat.dwRGBBitCount;

	g_pDD4->Release();
	g_pDD1->Release();

	return MD3D_OK;
}


MD3DERROR d3dFlipToGdiSurface()
{
	if(g_bIsFullscreen && g_pDD)
		return g_pDD->FlipToGDISurface();

	return DD_OK;
}

BOOL IsMultiTextureSupport()
{
	return g_bCanDoMultiTexture;
}
void InitIsMultiTextureSupport()
{
    D3DDEVICEDESC7 ddHwDesc;
    g_pd3dDevice->GetCaps( &ddHwDesc);
    if( ddHwDesc.wMaxSimultaneousTextures > 1 )
        if( ddHwDesc.dwTextureOpCaps & D3DTEXOPCAPS_MODULATE )
            g_bCanDoMultiTexture = TRUE;
}


MD3DERROR d3dReInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle )
{
	BOOL is_back=m_pddsBackBufferSM!=0;
	HRESULT hr;
	_ASSERTE( g_bInitialized );

	if(is_back)
	{
		hr=d3dReleaseBackBuffer();
		if(FAILED(hr))
			return hr;
	}

	// Shutdown gamma support
	__d3dCloseGammaSupport();
	hr = DestroyObjects(TRUE);
    D3DEnum_FreeResources();

	g_bInitialized=FALSE;

	hr=d3dInitX(dwWidth,dwHeight,dwBpp,dwInitFlags,hInst, hIcon,szTitle,FALSE);

	if(SUCCEEDED(hr))
	{
		if(is_back)
			d3dCreateBackBuffer();
		d3dClear(0);
	}
	return hr;
}

MD3DERROR d3dSetFocusLossBehavior( BOOL bSleep )
{
	g_bSleepIfNonActive = bSleep;
	return TRUE;
}

