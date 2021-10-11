//*******************************************************************
//	Mechosoma D3D Wrapper API
//  Back buffer
//
//  K-D Lab::Spika
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include "D3DUtil.h"
#include "x3dutil.h"

//-------------------------------------------------
// Externals 
//-------------------------------------------------

extern BOOL		g_bInitialized;

extern LPDIRECTDRAW7		g_pDD;              // The DirectDraw object
extern LPDIRECTDRAWSURFACE7	g_pddsFrontBuffer;  // The primary surface
extern LPDIRECTDRAWSURFACE7	g_pddsBackBuffer;   // The backbuffer surface
extern LPDIRECTDRAWSURFACE7 m_pddsZBuffer;      // The zbuffer surface
extern DDPIXELFORMAT		g_ddpfPrimary;
extern DWORD			g_dwRenderWidth;    // Dimensions of the render target
extern DWORD			g_dwRenderHeight;

extern BOOL		g_bInScene;		// TRUE when in BeginScene/EndScene bracket

//-------------------------------------------------
// Local module variables
//-------------------------------------------------

LPDIRECTDRAWSURFACE7	m_pddsBackBufferSM = NULL;
static BOOL					m_bUseColorkey = FALSE;
static DWORD				m_dwColorKey;
static DWORD				m_dwBackBufferFormat;
static DDPIXELFORMAT		m_ddpfBB;

static x3uPixelConverterFactory	m_x3uConverterFactory;
static x3uPixelConverter		*m_lpPixelConverter = NULL;

void ParseBitMask( DWORD dwBits, DWORD *lpdwZeros, DWORD *lpdwOnes );
//-----------------------------------------------------------------------------
// Name: d3dCreateBackBuffer
// Desc: Create an offscreen plain for back buffer
//-----------------------------------------------------------------------------
MD3DERROR d3dCreateBackBuffer()
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( NULL == m_pddsBackBufferSM );
	_ASSERTE( FALSE == g_bInScene );

	// Set pixel format for the back buffer depending 
	// on the bit count of the primary

	ZeroMemory( &m_ddpfBB, sizeof(DDPIXELFORMAT) );
	m_ddpfBB.dwSize = sizeof(DDPIXELFORMAT);
	m_ddpfBB.dwFlags = DDPF_RGB;
/*
	switch( g_ddpfPrimary.dwRGBBitCount)
	{
	case 16:	
		// RGB 565
		m_ddpfBB.dwRGBBitCount = 16;
		m_ddpfBB.dwRBitMask = 0xF800;
		m_ddpfBB.dwGBitMask = 0x07E0;
		m_ddpfBB.dwBBitMask = 0x001F;

//		m_ddpfBB.dwRBitMask = g_ddpfPrimary.dwRBitMask;
//		m_ddpfBB.dwGBitMask = g_ddpfPrimary.dwGBitMask;
//		m_ddpfBB.dwBBitMask = g_ddpfPrimary.dwBBitMask;

		DWORD dwZeros,dwOnes;
		ParseBitMask(g_ddpfPrimary.dwGBitMask,&dwZeros,&dwOnes);
		m_dwBackBufferFormat = MD3DBBFORMAT_RGB565;
			//dwOnes==5?MD3DBBFORMAT_RGB555:MD3DBBFORMAT_RGB565;
		break;

	case 32:
		// RGB 888 (32 bit)
		m_ddpfBB.dwRGBBitCount = 32;
		m_ddpfBB.dwRBitMask = g_ddpfPrimary.dwRBitMask;
		m_ddpfBB.dwGBitMask = g_ddpfPrimary.dwGBitMask;
		m_ddpfBB.dwBBitMask = g_ddpfPrimary.dwBBitMask;
		m_dwBackBufferFormat = MD3DBBFORMAT_RGB888;
		break;

	case 24:
		m_ddpfBB.dwRGBBitCount = g_ddpfPrimary.dwRGBBitCount;
		m_ddpfBB.dwRBitMask = g_ddpfPrimary.dwRBitMask;
		m_ddpfBB.dwGBitMask = g_ddpfPrimary.dwGBitMask;
		m_ddpfBB.dwBBitMask = g_ddpfPrimary.dwBBitMask;
		m_dwBackBufferFormat = MD3DBBFORMAT_RGB888;
		break;
		
	default:
		_RPT0( _CRT_ERROR, "Unsupported bit depth of the primary surface." );
	}
*/

	//Создавать m_pddsBackBufferSM всегда одного формата
	m_ddpfBB.dwRGBBitCount = 16;
	m_ddpfBB.dwRBitMask = 0xF800;
	m_ddpfBB.dwGBitMask = 0x07E0;
	m_ddpfBB.dwBBitMask = 0x001F;

	DWORD dwZeros,dwOnes;
	ParseBitMask(g_ddpfPrimary.dwGBitMask,&dwZeros,&dwOnes);
	m_dwBackBufferFormat = MD3DBBFORMAT_RGB565;

	// Create a back buffer using the pixel format of the primary

	DDSURFACEDESC2 ddsdBB;
	HRESULT hr;

	D3DUtil_InitSurfaceDesc( ddsdBB, DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT );
	ddsdBB.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;

	ddsdBB.dwWidth = g_dwRenderWidth;
	ddsdBB.dwHeight = g_dwRenderHeight;

	if( FAILED( hr = g_pDD->CreateSurface( &ddsdBB, &m_pddsBackBufferSM, NULL ) ) )
	{
		if( hr == DDERR_OUTOFMEMORY ) {
			_RPT0( _CRT_ERROR, "d3dCreateBackBuffer: Can't create offscreen surface (out of memory)." );
			return MD3DERR_OUTOFMEMORY;
		}
		_RPT0( _CRT_ERROR, "d3dCreateBackBuffer: Can't create offscreen surface." );
		return MD3DERR_GENERIC;
	}

	// See if the format of the primary matches our standard format
	// and if not, create a pixel converter

	if( m_ddpfBB.dwRBitMask != g_ddpfPrimary.dwRBitMask ||
		m_ddpfBB.dwGBitMask != g_ddpfPrimary.dwGBitMask ||
		m_ddpfBB.dwBBitMask != g_ddpfPrimary.dwBBitMask ||
		m_ddpfBB.dwRGBBitCount !=g_ddpfPrimary.dwRGBBitCount)
	{
		m_lpPixelConverter = m_x3uConverterFactory.CreatePixelConverter( &m_ddpfBB, &g_ddpfPrimary );
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dReleaseBackBuffer
// Desc: Release offscreen plain used for back buffer
//-----------------------------------------------------------------------------
MD3DERROR d3dReleaseBackBuffer()
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );
	_ASSERTE( FALSE == g_bInScene );

	DWORD ref;
	ref = m_pddsBackBufferSM->Release();
	_ASSERTE( 0 == ref );
	m_pddsBackBufferSM = NULL;

	if( m_lpPixelConverter ) {
		delete m_lpPixelConverter;
		m_lpPixelConverter = NULL;
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dGetBackBufferFormat
// Desc: Get back buffer format
//-----------------------------------------------------------------------------
MD3DERROR d3dGetBackBufferFormat( DWORD *dwFormat )
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );

	*dwFormat = m_dwBackBufferFormat;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dLockBackBuffer
// Desc: Lock back buffer and get the surface address and pitch
//-----------------------------------------------------------------------------
MD3DERROR d3dLockBackBuffer( VOID **lplpSurface, DWORD *lpdwPitch )
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );

	DDSURFACEDESC2 ddsd;
	D3DUtil_InitSurfaceDesc( ddsd );

	DWORD dwFlags = DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT;

	HRESULT hr;
	if( FAILED( hr = m_pddsBackBufferSM->Lock( NULL, &ddsd, dwFlags, NULL  ) ) )
	{
		if( DDERR_SURFACEBUSY == hr )
		{
			_RPT0( _CRT_ERROR, "d3dLockBackBuffer: Failed to lock surface: surface busy (probably already locked)." );
			return MD3DERR_ILLEGALCALL;
		}

		_RPT0( _CRT_ERROR, "d3dLockBackBuffer: Failed to lock surface." );
		return MD3DERR_GENERIC;
	}

	*lplpSurface = ddsd.lpSurface;
	*lpdwPitch = ddsd.lPitch;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dUnlockBackBuffer
// Desc: Unlock back buffer 
//-----------------------------------------------------------------------------
MD3DERROR d3dUnlockBackBuffer()
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );

	HRESULT hr;
	if( FAILED( hr = m_pddsBackBufferSM->Unlock( NULL ) ) )
	{
		if( DDERR_NOTLOCKED == hr )
		{
			_RPT0( _CRT_ERROR, "d3dUnlockBackBuffer: Failed to unlock surface (the surface wasn't locked)." );
			return MD3DERR_ILLEGALCALL;
		}
		_RPT0( _CRT_ERROR, "d3dUnlockBackBuffer: Failed to unlock surface." );
		return MD3DERR_GENERIC;
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dFlushBackBuffer
// Desc: Blit a rectangle from the offscreen plain to the render target
//       (lprcRect == NULL means blit the entire surface)
//-----------------------------------------------------------------------------
MD3DERROR d3dFlushBackBuffer( RECT *lprcRect )
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );
	_ASSERTE( FALSE == g_bInScene );

	if( m_lpPixelConverter )
	{
		// Lock back buffer

		HRESULT hr;
		DDSURFACEDESC2 ddsdBB;
		D3DUtil_InitSurfaceDesc( ddsdBB );

		if( FAILED( hr = m_pddsBackBufferSM->Lock( NULL, &ddsdBB, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL  ) ) )
		{
			if( DDERR_SURFACEBUSY == hr )
			{
				_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to lock back buffer: surface busy (probably already locked)." );
				return MD3DERR_ILLEGALCALL;
			}

			_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to lock back buffer." );
			return MD3DERR_GENERIC;
		}

		// Lock render target

		DDSURFACEDESC2 ddsdFB;
		D3DUtil_InitSurfaceDesc( ddsdFB );

		if( FAILED( hr = g_pddsBackBuffer->Lock( NULL, &ddsdFB, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL  ) ) )
		{
			if( DDERR_SURFACEBUSY == hr )
			{
				_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to lock render target: surface busy (probably already locked)." );
				return MD3DERR_ILLEGALCALL;
			}

			_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to lock render target." );
			return MD3DERR_GENERIC;
		}

		ddsdFB.lpSurface=(BYTE*)ddsdFB.lpSurface+
			lprcRect->top*ddsdFB.lPitch+lprcRect->left*
					(ddsdFB.ddpfPixelFormat.dwRGBBitCount/8);

		ddsdBB.lpSurface=(BYTE*)ddsdBB.lpSurface+
			lprcRect->top*ddsdBB.lPitch+lprcRect->left*2;
					//(ddsdBB.ddpfPixelFormat.dwRGBBitCount/8);
		// Convert pixels

		DWORD dwWidth, dwHeight;
		if( lprcRect ) {
			dwWidth = lprcRect->right - lprcRect->left;
			dwHeight = lprcRect->bottom - lprcRect->top;
		} else {
			dwWidth = g_dwRenderWidth;
			dwHeight = g_dwRenderHeight;
		}

		if( m_bUseColorkey ) 
		{
			m_lpPixelConverter->BltConv( ddsdFB.lpSurface, ddsdFB.lPitch, 
										 ddsdBB.lpSurface, ddsdBB.lPitch, 
										 dwWidth, dwHeight, m_dwColorKey );
		} else {
			m_lpPixelConverter->BltConv( ddsdFB.lpSurface, ddsdFB.lPitch, 
										 ddsdBB.lpSurface, ddsdBB.lPitch, 
										 dwWidth, dwHeight );
		}

		// Unlock render target
		if( FAILED( hr = g_pddsBackBuffer->Unlock(NULL) ) )
		{
			_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to unlock render target." );
			return MD3DERR_GENERIC;
		}

		// Unlock back buffer
		if( FAILED( hr = m_pddsBackBufferSM->Unlock(NULL) ) )
		{
			_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Failed to unlock back buffer." );
			return MD3DERR_GENERIC;
		}

	} else {

		// Determine left-top coordinates on the destination surf
		DWORD dwX;
		DWORD dwY;
		if( lprcRect ) {
			dwX = lprcRect->left;
			dwY = lprcRect->top;
		} else {
			dwX = 0;
			dwY = 0;
		}

		// Transfer mode for blit
		DWORD dwTrans;
		dwTrans = DDBLTFAST_WAIT;
		if( m_bUseColorkey ) {
			dwTrans |= DDBLTFAST_SRCCOLORKEY;
		}

		if( FAILED( g_pddsBackBuffer->BltFast( dwX, dwY, m_pddsBackBufferSM, lprcRect, dwTrans ) ) )
		{
			_RPT0( _CRT_ERROR, "d3dFlushBackBuffer: Blit to front/back buffer failed." );
			return MD3DERR_GENERIC;
		}
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dSetBackBufferColorKey
// Desc: Set source-blit colorkey for back buffer
//-----------------------------------------------------------------------------
MD3DERROR d3dSetBackBufferColorKey( DWORD dwColor )
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );

	DDCOLORKEY DDColorKey;
	DDColorKey.dwColorSpaceLowValue = dwColor;
	DDColorKey.dwColorSpaceHighValue = dwColor;

	m_dwColorKey = dwColor;

	if( FAILED( m_pddsBackBufferSM->SetColorKey( DDCKEY_SRCBLT, &DDColorKey ) ) )
	{
		_RPT0( _CRT_ERROR, "d3dSetBackBufferColorKey: Failed to set color key for back buffer." );
		return MD3DERR_GENERIC;
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dEnableBackBufferColorKey
// Desc: Enable/disable colorkey'ed blits for backbuffer
//-----------------------------------------------------------------------------
MD3DERROR d3dEnableBackBufferColorKey( BOOL bEnable )
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( m_pddsBackBufferSM );

	m_bUseColorkey = bEnable;

	return MD3D_OK;
}


MD3DERROR GetBackBufferDC(HDC* hdc)
{
	if(g_pddsBackBuffer==NULL)return MD3DERR_NOZBUFFER;
	return g_pddsBackBuffer->GetDC(hdc);
}

MD3DERROR ReleaseBackBufferDC(HDC hdc)
{
	if(g_pddsBackBuffer==NULL)return MD3DERR_NOZBUFFER;
	return g_pddsBackBuffer->ReleaseDC(hdc);
}
