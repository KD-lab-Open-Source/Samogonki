//*******************************************************************
//	
//	Mechosoma D3D Wrapper API
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include "D3DUtil.h"

// Module variables

//static FLOAT m_fNearPlane;
//static FLOAT m_fFarPlane;

// Externals

extern BOOL g_bInitialized;
extern BOOL g_bIsFullscreen;    

extern LPDIRECT3DDEVICE7	g_pd3dDevice;       // The D3D device

extern LPDIRECTDRAWSURFACE7	g_pddsFrontBuffer;  // The primary surface
extern LPDIRECTDRAWSURFACE7	g_pddsBackBuffer;   // The backbuffer surface

extern DWORD	g_dwRenderWidth;
extern DWORD	g_dwRenderHeight;
extern RECT		g_rcScreenRect;					// Screen rect 
extern RECT		g_rcViewportRect;

extern BOOL		g_bInScene;		 // TRUE when in BeginScene/EndScene bracket

extern DWORD	g_dwSceneStamp;	 // Incremented on every EndScene and used 
								 // by texture manager

extern D3DDEVICEDESC		g_ddDeviceDesc;

void __d3dUpdateGamma();

// External function prototypes

HRESULT RestoreSurfaces();

HRESULT RestoreAll();
//-----------------------------------------------------------------------------
// Name: d3dQueryCaps()
// Desc: Query device capabilities
//-----------------------------------------------------------------------------
MD3DERROR d3dQueryCaps( MD3DCAPS Caps, DWORD *dwData )
{
	switch(Caps)
	{
	case MD3DCAPS_WFOG:
		if( g_ddDeviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_WFOG )
			*dwData = TRUE;
		else
			*dwData = FALSE;
		break;	

	default:
		_ASSERTE(FALSE);
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dBeginScene()
// Desc: Begin scene
//-----------------------------------------------------------------------------
MD3DERROR d3dBeginScene()
{
	_ASSERTE( g_bInitialized );
	if(g_bInScene==TRUE) return MD3D_OK; 
//	_ASSERTE( !g_bInScene );

	HRESULT hr;
	hr=RestoreAll();
	if(hr!=MD3D_OK)
		return hr;

    if( FAILED( hr = g_pd3dDevice->BeginScene() ) )
    {
		if( DDERR_SURFACELOST == hr ) 
		{
			if( FAILED( RestoreSurfaces() ) )
			{
				DEBUG_MSG( TEXT("d3dBeginScene() surface was lost and couldn't be restored\n") );
				return MD3DERR_GENERIC;
			}
			if( FAILED( g_pd3dDevice->BeginScene() ) )
			{
				DEBUG_MSG( TEXT("d3dBeginScene() BeginScene failed\n") );
				return MD3DERR_GENERIC;
			}
		}
		 
		DEBUG_MSG( TEXT("d3dBeginScene() BeginScene failed\n") );
		return MD3DERR_GENERIC;
    }

	g_bInScene = TRUE;
	extern DWORD m_dwCurrentBlendMode;		// Currently set blending mode
	m_dwCurrentBlendMode=0xffffffff;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dEndScene()
// Desc: Begin scene
//-----------------------------------------------------------------------------
MD3DERROR d3dEndScene()
{
	_ASSERTE( g_bInitialized );
//	_ASSERTE( g_bInScene );
	if(g_bInScene==FALSE) return MD3D_OK;

	g_bInScene = FALSE;

	HRESULT hr;
    if( FAILED( hr = g_pd3dDevice->EndScene() ) )
    {
//		if( DDERR_SURFACELOST == hr ) 
		{
			if( FAILED( RestoreSurfaces() ) )
			{
				DEBUG_MSG( TEXT("d3dEndScene(): surface was lost and couldn't be restored\n") );
				return MD3DERR_GENERIC;
			}
			if( FAILED( hr = g_pd3dDevice->EndScene() ) )
			{
				DEBUG_MSG( TEXT("d3dEndScene() EndScene failed\n") );
				return MD3DERR_GENERIC;
			}
		}
		 
		DEBUG_MSG( TEXT("d3dEndScene() EndScene failed\n") );
		return MD3DERR_GENERIC;
    }

	// Increment scene count
	g_dwSceneStamp++;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dSetClipRect()
// Desc: Set clipping rectangle
//-----------------------------------------------------------------------------
MD3DERROR d3dSetClipRect(RECT* lprcClipRect)
{
	_ASSERTE( g_bInitialized );
//	_ASSERTE( !g_bInScene );

	MD3DLOG4( 1, "d3dSetClipRect: left:%d, top:%d, right:%d, bottom:%d\n", 
			lprcClipRect->left, lprcClipRect->top, 
			lprcClipRect->right, lprcClipRect->bottom );

    // Set the parameters to the new viewport
	D3DVIEWPORT7 vp = {lprcClipRect->left,lprcClipRect->top,lprcClipRect->right,lprcClipRect->bottom,0,1 };
	if( FAILED( g_pd3dDevice->SetViewport( &vp ) ) )
	{
        DEBUG_MSG( TEXT("Error: Couldn't set the viewport data") );
        return MD3DERR_GENERIC;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: d3dFlip()
// Desc: Flip the primary/back buffer
//-----------------------------------------------------------------------------
#ifdef _PROFILE_D3D
extern DWORD num_256,num_128,num_64,num_32;
#endif _PROFILE_D3D

MD3DERROR d3dFlip(bool WaitVerticalBlank)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( !g_bInScene );

    // If we have a back buffer, perform a flip.
	if(g_pddsBackBuffer==0) return MD3D_OK;
	if(g_bIsFullscreen)
		HRESULT hr=g_pddsFrontBuffer->Flip(NULL,WaitVerticalBlank?DDFLIP_WAIT:DDFLIP_NOVSYNC);
	else
		g_pddsFrontBuffer->Blt(&g_rcScreenRect,g_pddsBackBuffer,&g_rcViewportRect,0,0);
#ifdef _PROFILE_D3D
	num_256=num_128=num_64=num_32=0;
#endif _PROFILE_D3D

	__d3dUpdateGamma();
	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dClear()
// Desc: Clear the screen
//-----------------------------------------------------------------------------
MD3DERROR d3dClear(DWORD dwColor)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( !g_bInScene );
	HRESULT hr;
	hr=RestoreAll();
	if(hr!=MD3D_OK)
		return hr;
	D3DVIEWPORT7 vp={0,0,g_dwRenderWidth,g_dwRenderHeight,0.0f,1.0f};
	if(SUCCEEDED(g_pd3dDevice->SetViewport(&vp)))
		if(SUCCEEDED(g_pd3dDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,dwColor,1,0L)))
			return MD3D_OK;
	return MD3DERR_GENERIC;
}

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

//-----------------------------------------------------------------------------
// Name: d3dSetFogParameters()
// Desc: Sets fog parameters 
//-----------------------------------------------------------------------------
MD3DERROR d3dSetFogParameters( DWORD dwMode, DWORD dwColor,
							   FLOAT fStart, FLOAT fEnd, FLOAT fDensity )
{
	_ASSERTE( fDensity >= 0.0 && fDensity <= 1.0f );

	//DX7:
	//g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_NONE );

	g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, dwMode );
	g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGCOLOR,  dwColor );
	g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLESTART,   FtoDW(fStart) );
	g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEEND,     FtoDW(fEnd) );
	g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEDENSITY, FtoDW(fDensity) );

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dEnableFog()
// Desc: Enable/disable fog 
//-----------------------------------------------------------------------------
MD3DERROR d3dEnableFog( BOOL bEnable )
{
    g_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, bEnable );

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dSetProjectionMatrix()
// Desc: Build and load the projection matrix
//-----------------------------------------------------------------------------
MD3DERROR d3dSetProjectionMatrix( FLOAT fFOV, FLOAT fNearPlane, FLOAT fFarPlane )
{
	FLOAT fAspect;
	D3DMATRIX matProj;
	HRESULT hr;

	fAspect = 1.0;	// BUGBUG: this will not work right if we use D3D transforms!
	hr = D3DUtil_SetProjectionMatrix( matProj, fFOV, fAspect, fNearPlane, fFarPlane );
	_ASSERTE(SUCCEEDED(hr));

	hr = g_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );
	_ASSERTE(SUCCEEDED(hr));

	return MD3D_OK;
}

//-----------------------------------------------------------------------------
// Name: d3dSetProjectionMatrixToIdentity()
// Desc: Load identity projection matrix
//-----------------------------------------------------------------------------
MD3DERROR d3dSetProjectionMatrixToIdentity()
{
	D3DMATRIX mat;
	HRESULT hr;

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = 1.0f;
    mat._22 = 1.0f;
    mat._33 = 1.0f;
    mat._44 = 1.0f;

	hr = g_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &mat );
	_ASSERTE(SUCCEEDED(hr));

	return MD3D_OK;
}
