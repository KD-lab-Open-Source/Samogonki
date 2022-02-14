//*******************************************************************
//	
//	Mechosoma D3D Wrapper API
//  -- sprite stuff
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "Md3d.h"
#include <cassert>
#include <cstring>
#define _ASSERTE assert
#define _ASSERT assert

#define _CRT_ERROR 0
#define _RPT0(X, Y)
#define _RPTF0(X, Y)
#include <math.h>

 
struct TVertex
{
	D3DVALUE	x,y,z,rhw;
	DWORD		rgba;
	D3DVALUE	u,v;
};

typedef struct {

	DWORD dwHandle;			// If the 31-st bit is 1, it's a child sprite
							// otherwise it's a parent. 0 means the slot is free.

	DWORD dwWidth;			// Width of the sprite
	DWORD dwHeight;			// Height of the sprite

	DWORD dwFlags;			// Sprite modes etc.
	DWORD dwAlphaRef;		
	DWORD dwAlphaFactor;
	DWORD dwColorFactor;

	TVertex Vertices[4];	// Vertices for the triangle fan

	union {

		// Parent sprite
		struct {
		DWORD dwChildrenCount;		// Number of children for this parent
		DWORD dwTexHandle;			// Texture handle
		};

		// Child sprite 
		struct {
		DWORD dwParentHandle;		// Handle of the parent sprite
		DWORD dwLeft;				// Coordinates of the upper-left corner 
		DWORD dwTop;				// on the parent sprite
		};
	};
} TSpriteSlot;


// Local prototypes
static DWORD FindUnusedSlot();
static DWORD CreateNewSlot();

// Md3d globals
extern BOOL		g_bInScene;			// TRUE when in BeginScene/EndScene bracket


// Local vars

BOOL g_SpriteMgrInitialized = FALSE;
static D3DVALUE m_dvSpriteZ;
static BOOL	m_bSpriteZEnable = FALSE;


#define SLOTS_INITIAL_SIZE 200
#define SLOTS_EXPAND_CHUNK 50

TSpriteSlot* m_lpSpriteSlots = NULL;
DWORD m_dwSpriteSlotsCount = 0;
DWORD m_dwSpriteSlotsUsed = 0;


//-----------------------------------------------------------------------------
// Name: __d3dInitSpriteManager()
// Desc: Initialize sprite manager
//-----------------------------------------------------------------------------
MD3DERROR __d3dInitSpriteManager()
{
	_ASSERTE( FALSE == g_SpriteMgrInitialized );

	// Initialize sprite slots

	m_lpSpriteSlots = (TSpriteSlot*)malloc( sizeof(TSpriteSlot)*SLOTS_INITIAL_SIZE );
	if( NULL == m_lpSpriteSlots )
	{
		_RPTF0( _CRT_ERROR, "md3d: unable to allocate memory for internal data structures.\n" );
		return MD3DERR_OUTOFMEMORY;
	}
	memset( m_lpSpriteSlots, 0, sizeof(TSpriteSlot)*SLOTS_INITIAL_SIZE );
	m_dwSpriteSlotsCount = SLOTS_INITIAL_SIZE;
	m_dwSpriteSlotsUsed = 1;	// First slot is never used

	m_bSpriteZEnable = FALSE;

	// Init done.
	g_SpriteMgrInitialized = TRUE;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: __d3dCloseSpriteManager()
// Desc: Shutdown sprite manager
//-----------------------------------------------------------------------------
MD3DERROR __d3dCloseSpriteManager()
{
	_ASSERTE( g_SpriteMgrInitialized );

	// Free sprite slots
	free( m_lpSpriteSlots );
	m_lpSpriteSlots = NULL;
	m_dwSpriteSlotsCount = 0;
	m_dwSpriteSlotsUsed = 0;

	// Deinit done.
	g_SpriteMgrInitialized = FALSE;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dCreateSprite
// Desc: Create a new (parent) sprite and return a handle to it
//-----------------------------------------------------------------------------
MD3DERROR d3dCreateSprite( DWORD dwWidth, DWORD dwHeight, DWORD dwFormat, 
						   DWORD dwFlags, DWORD* lpdwHandle )
{
	_ASSERTE( g_SpriteMgrInitialized );
//	_ASSERTE( FALSE == g_bInScene );

	// Try to find an unused slot.
	DWORD dwSlot;
	if( 0 == ( dwSlot = FindUnusedSlot() ) )
	{
		// No unused slots. Try to create a new one.
		if( 0 == ( dwSlot = CreateNewSlot() ) )
		{
			_RPT0( _CRT_ERROR, "MD3D: unable to allocate memory for internal data structures.\n" );
			*lpdwHandle = 0;
			return MD3DERR_OUTOFMEMORY;
		}
	}

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwSlot];

	// Create the texture

	DWORD hr;
	DWORD dwTexHandle;

	if( FAILED( hr = d3dCreateTexture( dwWidth, dwHeight, dwFormat, &dwTexHandle ) ) ) {
		_RPT0( _CRT_ERROR, "MD3D: unable to create texture for a sprite.\n" );
		*lpdwHandle = 0;
		return hr;
	}

	// Fill the slot for the new parent sprite

	lpSprite->dwHandle = dwSlot;
	lpSprite->dwWidth = dwWidth;
	lpSprite->dwHeight = dwHeight;
	lpSprite->dwChildrenCount = 0;
	lpSprite->dwTexHandle = dwTexHandle;

	lpSprite->dwFlags = dwFlags;
	lpSprite->dwAlphaRef = 0;
	lpSprite->dwAlphaFactor = 255;
	lpSprite->dwColorFactor = RGB_MAKE(255,255,255);

	// Vertices

	for( DWORD i = 0; i < 4; i++ ) {
		lpSprite->Vertices[i].x = 0.0f;
		lpSprite->Vertices[i].y = 0.0f;
		lpSprite->Vertices[i].z = 0.0f;
		lpSprite->Vertices[i].rhw = 1.0f;
		lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
	}
	lpSprite->Vertices[0].u = 0.0f;
	lpSprite->Vertices[0].v = 0.0f;
	lpSprite->Vertices[1].u = 1.0f;
	lpSprite->Vertices[1].v = 0.0f;
	lpSprite->Vertices[2].u = 1.0f;
	lpSprite->Vertices[2].v = 1.0f;
	lpSprite->Vertices[3].u = 0.0f;
	lpSprite->Vertices[3].v = 1.0f;

	// Return the new handle
	*lpdwHandle = dwSlot;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dCreateChildSprite
// Desc: Create a new child sprite and return a handle to it
//-----------------------------------------------------------------------------
MD3DERROR d3dCreateChildSprite( DWORD dwParentHandle, DWORD dwLeft, DWORD dwTop, 
							    DWORD dwWidth, DWORD dwHeight, DWORD* lpdwHandle )
{

	_ASSERTE( g_SpriteMgrInitialized );
//	_ASSERTE( FALSE == g_bInScene );

	// Check if the parent handle is valid
	_ASSERTE( dwParentHandle > 0 && dwParentHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( m_lpSpriteSlots[dwParentHandle].dwHandle == dwParentHandle );


	// Try to find an unused slot.
	DWORD dwSlot;
	if( 0 == ( dwSlot = FindUnusedSlot() ) )
	{
		// No unused slots. Try to create a new one.
		if( 0 == ( dwSlot = CreateNewSlot() ) )
		{
			_RPT0( _CRT_ERROR, "MD3D: unable to allocate memory for internal data structures.\n" );
			*lpdwHandle = 0;
			return MD3DERR_OUTOFMEMORY;
		}
	}

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwSlot];

	// Pointer to the parent sprite
	TSpriteSlot *lpParentSprite = &m_lpSpriteSlots[dwParentHandle];


	// Fill the slot and give the user the new handle

	lpSprite->dwHandle = dwSlot | 0x80000000;
	lpSprite->dwParentHandle = dwParentHandle;

	lpSprite->dwLeft = dwLeft;
	lpSprite->dwTop = dwTop;
	lpSprite->dwWidth = dwWidth;
	lpSprite->dwHeight = dwHeight;

	lpSprite->dwFlags = lpParentSprite->dwFlags;
	lpSprite->dwAlphaRef = lpParentSprite->dwAlphaRef;
	lpSprite->dwAlphaFactor = lpParentSprite->dwAlphaFactor;
	lpSprite->dwColorFactor = lpParentSprite->dwColorFactor;

	for( DWORD i = 0; i < 4; i++ ) {
		lpSprite->Vertices[i].x = 0.0f;
		lpSprite->Vertices[i].y = 0.0f;
		lpSprite->Vertices[i].z = 0.0f;
		lpSprite->Vertices[i].rhw = 1.0f;
		lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
	}

	D3DVALUE dvLeft = D3DVALUE(lpSprite->dwLeft) / D3DVALUE(lpParentSprite->dwWidth);
	D3DVALUE dvTop = D3DVALUE(lpSprite->dwTop) / D3DVALUE(lpParentSprite->dwHeight);
	D3DVALUE dvRight = D3DVALUE(lpSprite->dwLeft + lpSprite->dwWidth) / D3DVALUE(lpParentSprite->dwWidth);
	D3DVALUE dvBottom = D3DVALUE(lpSprite->dwTop + lpSprite->dwHeight) / D3DVALUE(lpParentSprite->dwHeight);

	lpSprite->Vertices[0].u = dvLeft;
	lpSprite->Vertices[0].v = dvTop;
	lpSprite->Vertices[1].u = dvRight;
	lpSprite->Vertices[1].v = dvTop;
	lpSprite->Vertices[2].u = dvRight;
	lpSprite->Vertices[2].v = dvBottom;
	lpSprite->Vertices[3].u = dvLeft;
	lpSprite->Vertices[3].v = dvBottom;

	// Increment parent sprite's child count
	lpParentSprite->dwChildrenCount++;

	// Return the handle
	*lpdwHandle = dwSlot;

	return MD3D_OK;
}



//-----------------------------------------------------------------------------
// Name: d3dDeleteSprite
// Desc: Delete a sprite by handle
//-----------------------------------------------------------------------------
MD3DERROR d3dDeleteSprite( DWORD dwHandle )
{
	_ASSERTE( g_SpriteMgrInitialized );
//	_ASSERTE( FALSE == g_bInScene );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];

	// If this is a child sprite, just free the slot
	if( 0 != (lpSprite->dwHandle & 0x80000000) ) {
		// Mark the slot as free
		lpSprite->dwHandle = 0;

		// Decrement the child count for the parent slot
		m_lpSpriteSlots[lpSprite->dwParentHandle].dwChildrenCount--;

		return MD3D_OK;
	}

	// Else it's a parent slot

	// First, free all the children if any
	if( 0 != lpSprite->dwChildrenCount ) {
		
		DWORD dwSlot;
		for( dwSlot = 1; dwSlot < m_dwSpriteSlotsUsed; dwSlot++ )
		{
			if( (m_lpSpriteSlots[dwSlot].dwHandle & 0x80000000) != 0 && 
				 m_lpSpriteSlots[dwSlot].dwParentHandle == dwHandle ) {

				// Mark the child slot as free
				m_lpSpriteSlots[dwSlot].dwHandle = 0;

#ifdef _DEBUG
				lpSprite->dwChildrenCount--;
#endif
			}
		}
		_ASSERTE( 0 == lpSprite->dwChildrenCount );
	}

	// Free the texture handle 
	DWORD hr;
	hr = d3dDeleteTexture( lpSprite->dwTexHandle );
	_ASSERT( 0 == hr );

	// Mark the slot as free
	lpSprite->dwHandle = 0;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dLockSprite
// Desc: Lock the sprite texture in system memory
//-----------------------------------------------------------------------------
MD3DERROR d3dLockSprite( DWORD dwHandle, VOID **lplpSprite, DWORD *lplpPitch )
{

	_ASSERTE( g_SpriteMgrInitialized );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];

	// Lock the sprite texture
	DWORD hr;
	if( (lpSprite->dwHandle & 0x80000000) == 0 ) {
		// It's a parent sprite
		hr = d3dLockTexture( lpSprite->dwTexHandle, lplpSprite, lplpPitch );
	} else {
		// It's a child sprite
		DWORD dwTexHandle = m_lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;
		hr = d3dLockTexture( dwTexHandle, lpSprite->dwLeft, lpSprite->dwTop,
							 lpSprite->dwLeft + lpSprite->dwWidth-1, 
							 lpSprite->dwTop + lpSprite->dwHeight-1,	
							 lplpSprite, lplpPitch );
	}

	if( FAILED(hr) ){
		*lplpSprite = 0;
		*lplpPitch = 0;
		return hr;
	}

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dUnlockSprite
// Desc: Unlock the sprite texture in system memory
//-----------------------------------------------------------------------------
MD3DERROR d3dUnlockSprite( DWORD dwHandle )
{
	_ASSERTE( g_SpriteMgrInitialized );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );


	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];

	// Get the texture handle
	DWORD dwTexHandle;
	if( (lpSprite->dwHandle & 0x80000000) == 0 ) {
		// It's a parent sprite
		dwTexHandle = lpSprite->dwTexHandle;
	} else {
		// It's a child sprite
		dwTexHandle = m_lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;
	}

	return d3dUnlockTexture( dwTexHandle );
}


//-----------------------------------------------------------------------------
// Name: d3dSetSpriteMode
// Desc: Set sprite drawing modes
//-----------------------------------------------------------------------------
MD3DERROR d3dSetSpriteMode( DWORD dwHandle, DWORD dwMode, DWORD dwValue )
{
	_ASSERTE( g_SpriteMgrInitialized );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];
	DWORD i;

	// Set mode for this sprite

	switch( dwMode ) {
	case MD3DSP_ALPHATESTENABLE:
		if( dwValue )
			lpSprite->dwFlags |= MD3DSP_USEALPHATEST;
		else
			lpSprite->dwFlags &= ~MD3DSP_USEALPHATEST;

		break;

	case MD3DSP_ALPHABLENDENABLE:
		if( dwValue )
			lpSprite->dwFlags |= MD3DSP_USEALPHABLEND;
		else
			lpSprite->dwFlags &= ~MD3DSP_USEALPHABLEND;

		break;

	case MD3DSP_ALPHAREF:
		lpSprite->dwAlphaRef = dwValue;
		break;

	case MD3DSP_COLORFACTOR:
		lpSprite->dwColorFactor = dwValue & RGBA_MAKE(255,255,255,0);
		for( i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
		}
		break;

	case MD3DSP_ALPHAFACTOR:
		lpSprite->dwAlphaFactor = dwValue;
		for( i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
		}
		break;
	}


	// See if this sprite has children

	if( (lpSprite->dwHandle & 0x80000000) == 0 && lpSprite->dwChildrenCount != 0) {
		for( DWORD dwSlot = 1; dwSlot < m_dwSpriteSlotsUsed; dwSlot++ )
		{
			if( (m_lpSpriteSlots[dwSlot].dwHandle & 0x80000000) != 0 && 
				 m_lpSpriteSlots[dwSlot].dwParentHandle == dwHandle ) {

				// Pointer to the child sprite
				TSpriteSlot *lpChild = &m_lpSpriteSlots[dwSlot];

				// Set mode for this child

				switch( dwMode ) {
				case MD3DSP_ALPHATESTENABLE:
					if( dwValue )
						lpSprite->dwFlags |= MD3DSP_USEALPHATEST;
					else
						lpSprite->dwFlags &= ~MD3DSP_USEALPHATEST;

					break;

				case MD3DSP_ALPHABLENDENABLE:
					if( dwValue )
						lpSprite->dwFlags |= MD3DSP_USEALPHABLEND;
					else
						lpSprite->dwFlags &= ~MD3DSP_USEALPHABLEND;

					break;

				case MD3DSP_ALPHAREF:
					lpSprite->dwAlphaRef = dwValue;
					break;

				case MD3DSP_COLORFACTOR:
					lpSprite->dwColorFactor = dwValue & RGBA_MAKE(255,255,255,0);
					for( i = 0; i < 4; i++ ) {
						lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
					}
					break;

				case MD3DSP_ALPHAFACTOR:
					lpSprite->dwAlphaFactor = dwValue;
					for( i = 0; i < 4; i++ ) {
						lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
					}
					break;
				}
			}
		}
	}

	return MD3D_OK;
}



//-----------------------------------------------------------------------------
// Name: d3dSetSpriteRect
// Desc: Set sprite drawing rectangle
//-----------------------------------------------------------------------------
MD3DERROR d3dSetSpriteRect( DWORD dwHandle, D3DVALUE dvLeft, D3DVALUE dvTop, D3DVALUE dvRight, D3DVALUE dvBottom )
{
	_ASSERTE( g_SpriteMgrInitialized );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Make sure the coordinates are valid
	_ASSERTE( dvLeft <= dvRight );
	_ASSERTE( dvTop <= dvBottom );
	_ASSERTE( dvLeft >= 0.0f && dvLeft <= 1.0f );
	_ASSERTE( dvTop >= 0.0f && dvTop <= 1.0f );
	_ASSERTE( dvRight >= 0.0f && dvRight <= 1.0f );
	_ASSERTE( dvBottom >= 0.0f && dvBottom <= 1.0f );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];

	// Change texture coordinates to the rectangle specified
	lpSprite->Vertices[0].u = dvLeft;
	lpSprite->Vertices[0].v = dvTop;
	lpSprite->Vertices[1].u = dvRight;
	lpSprite->Vertices[1].v = dvTop;
	lpSprite->Vertices[2].u = dvRight;
	lpSprite->Vertices[2].v = dvBottom;
	lpSprite->Vertices[3].u = dvLeft;
	lpSprite->Vertices[3].v = dvBottom;

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dDrawSprite
// Desc: Draw the sprite
//-----------------------------------------------------------------------------
MD3DERROR d3dDrawSprite( DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, DWORD dwOrigin,
						 D3DVALUE dvScaleX, D3DVALUE dvScaleY, D3DVALUE dvRotate )
{
	_ASSERTE( g_SpriteMgrInitialized );
	_ASSERTE( g_bInScene );

	// Check if the handle is valid
	_ASSERTE( dwHandle > 0 && dwHandle < m_dwSpriteSlotsUsed );
	_ASSERTE( (m_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &m_lpSpriteSlots[dwHandle];

	// Texutre handle
	DWORD dwTexHandle;
	if( (lpSprite->dwHandle & 0x80000000) == 0 )
		dwTexHandle = lpSprite->dwTexHandle;
	else
		dwTexHandle = m_lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;

	DWORD hr;

	// Set current texture to the sprite texture
	hr = d3dSetTexture( dwTexHandle );
	if( FAILED(hr) )
		return hr;

	// Calculate coordinates

	D3DVALUE dvWidth, dvHeight;
	D3DVALUE dvLeft, dvTop, dvRight, dvBottom;

	if( lpSprite->dwHandle & 0x80000000 )
	{
		// Child
		dvWidth = D3DVALUE(lpSprite->dwWidth);
		dvHeight = D3DVALUE(lpSprite->dwHeight);
	} else {
		dvWidth = D3DVALUE(lpSprite->dwWidth) * (lpSprite->Vertices[1].u - lpSprite->Vertices[0].u);
		dvHeight = D3DVALUE(lpSprite->dwHeight) * (lpSprite->Vertices[2].v - lpSprite->Vertices[1].v);
	}

	switch(dwOrigin) {
	case MD3DORG_CENTER:
		if( dvScaleX == 1.0f ) {
			dvLeft = dvX - dvWidth/2.0f;
			dvRight = dvX + dvWidth/2.0f;
		} else {
			dvLeft = dvX - (dvWidth/2.0f)*dvScaleX;
			dvRight = dvX + (dvWidth/2.0f)*dvScaleX;
		}
		if( dvScaleY == 1.0f ) {
			dvTop = dvY - dvHeight/2.0f;
			dvBottom = dvY + dvHeight/2.0f;
		} else {
			dvTop = dvY - (dvHeight/2.0f)*dvScaleY;
			dvBottom = dvY + (dvHeight/2.0f)*dvScaleY;
		}

		break;

	case MD3DORG_TOPLEFT:
		dvLeft = dvX;
		dvTop = dvY;

		if( dvScaleX == 1.0f ) {
			dvRight = dvX + dvWidth;
		} else {
			dvRight = dvX + dvWidth*dvScaleX;
		}
		if( dvScaleY == 1.0f ) {
			dvBottom = dvY + dvHeight;
		} else {
			dvBottom = dvY + dvHeight*dvScaleY;
		}

		break;
	}

	lpSprite->Vertices[0].x = dvLeft;
	lpSprite->Vertices[0].y = dvTop;

	lpSprite->Vertices[1].x = dvRight;
	lpSprite->Vertices[1].y = dvTop;

	lpSprite->Vertices[2].x = dvRight;
	lpSprite->Vertices[2].y = dvBottom;

	lpSprite->Vertices[3].x = dvLeft;
	lpSprite->Vertices[3].y = dvBottom;
	
	// See if we have to rotate the sprite

	if( dvRotate != 0.0f ) {
		D3DVALUE sin_a = (float)sin(dvRotate);
		D3DVALUE cos_a = (float)cos(dvRotate);

		D3DVALUE xc = (dvRight+dvLeft)/2.0f;
		D3DVALUE yc = (dvBottom+dvTop)/2.0f;
		D3DVALUE x;
		D3DVALUE y;
		for( DWORD i = 0; i < 4; i++ ) {
			x = lpSprite->Vertices[i].x - xc;
			y = lpSprite->Vertices[i].y - yc;
			lpSprite->Vertices[i].x = x*cos_a - y*sin_a + xc;
			lpSprite->Vertices[i].y = x*sin_a + y*cos_a + yc;
		}
	}

	// If Z is enabled, set z values in the vertices
	if( m_bSpriteZEnable ) {
		for( DWORD i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].z = m_dvSpriteZ;
		}
	}


	// Save current render states

	DWORD dwAlphaTestEnable;
	DWORD dwAlphaFunc;
	DWORD dwAlphaRef;
	DWORD dwAlphaBlendEnable;
	DWORD dwSrcFactor;
	DWORD dwDestFactor;
	DWORD dwZEnable;
	DWORD dwZWriteEnable;

	d3dGetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTestEnable );
	d3dGetRenderState( D3DRENDERSTATE_ALPHAFUNC, &dwAlphaFunc );
	d3dGetRenderState( D3DRENDERSTATE_ALPHAREF, &dwAlphaRef );

	d3dGetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlendEnable );
	d3dGetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcFactor );
	d3dGetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestFactor );

	d3dGetRenderState( D3DRENDERSTATE_ZENABLE, &dwZEnable );
	d3dGetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable );

	// Set render states

	if( lpSprite->dwFlags & MD3DSP_USEALPHATEST ) {
		d3dSetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		d3dSetRenderState( D3DRENDERSTATE_ALPHAREF, lpSprite->dwAlphaRef );
	} else {
		d3dSetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, FALSE );
		d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_ALWAYS );
	}

	if( lpSprite->dwFlags & MD3DSP_USEALPHABLEND ) {
		d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		d3dSetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		d3dSetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	} else {
		d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	}

	if( m_bSpriteZEnable ) {
		d3dSetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
		d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
	} else {
		d3dSetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_FALSE );
		d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
	}

	d3dSetTextureBlendMode( MD3DTB_TEXTURE1_MOD_DIFFUSE, 
				//MD3DTB_TEXTURE1);//Для совсем слабеньких карточек
				MD3DTB_TEXTURE1_MOD_DIFFUSE );

	d3dSetRenderState( D3DRENDERSTATE_SPECULARENABLE, FALSE );

//	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_CCW);
	d3dTriangleFan( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, &(lpSprite->Vertices), 4 );
//	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_CW);


	// Restore render states

	d3dSetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTestEnable );
	d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC, dwAlphaFunc );
	d3dSetRenderState( D3DRENDERSTATE_ALPHAREF, dwAlphaRef );

	d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlendEnable );
	d3dSetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcFactor );
	d3dSetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestFactor );

	d3dSetRenderState( D3DRENDERSTATE_ZENABLE, dwZEnable );
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable );

	return MD3D_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dDrawSpriteZ
// Desc: Draw a sprite with Z
//-----------------------------------------------------------------------------
MD3DERROR d3dDrawSpriteZ( DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, D3DVALUE dvZ, 
						  DWORD dwOrigin, D3DVALUE dvScaleX, D3DVALUE dvScaleY, 
						  D3DVALUE dvRotate )
{

	m_dvSpriteZ = dvZ;
	m_bSpriteZEnable = TRUE;

	MD3DERROR hr;
	hr = d3dDrawSprite( dwHandle, dvX, dvY, dwOrigin, dvScaleX, dvScaleY, dvRotate );

	m_bSpriteZEnable = FALSE;

	return hr;
}


//-----------------------------------------------------------------------------
// Helper functions for managing sprite slots 
//-----------------------------------------------------------------------------

static DWORD FindUnusedSlot() 
{
	DWORD dwSlot;

	for( dwSlot = 1; dwSlot < m_dwSpriteSlotsUsed; dwSlot++ )
	{
		if( 0 == m_lpSpriteSlots[dwSlot].dwHandle )
			return dwSlot;	// Found
	}

	// Not found.
	return 0;
}


static DWORD CreateNewSlot()
{
	if( m_dwSpriteSlotsUsed < m_dwSpriteSlotsCount )
	{
		// We still have free space
		m_dwSpriteSlotsUsed++;
		return m_dwSpriteSlotsUsed-1;
	}

	// Else we need to expand the array

	if( NULL == ( m_lpSpriteSlots = (TSpriteSlot *)realloc( m_lpSpriteSlots, 
		 (m_dwSpriteSlotsCount+SLOTS_EXPAND_CHUNK)*sizeof(TSpriteSlot) ) ) )
	{
		_RPTF0( _CRT_ERROR, "md3d: unable to allocate memory for internal data structures.\n" );
		return MD3DERR_OUTOFMEMORY;
	}

	// Clear the newly allocated block
	memset( m_lpSpriteSlots + m_dwSpriteSlotsCount, 0, SLOTS_EXPAND_CHUNK*sizeof(TSpriteSlot) );

	m_dwSpriteSlotsCount += SLOTS_EXPAND_CHUNK;

	m_dwSpriteSlotsUsed++;
	return m_dwSpriteSlotsUsed-1;
}

