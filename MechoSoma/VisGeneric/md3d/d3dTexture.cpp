#include "md3d.h"
#include <crtdbg.h>
#include <stdio.h>
#include "d3dUtil.h"
#include "BaseClass.h"

typedef struct {
	DWORD dwFormatID;
	BOOL bSupported;
	DDPIXELFORMAT ddpfPixelFormat;

	// This is used for matching 
	DWORD dwRGBBitCount;
	BOOL bPalette8;
	DWORD dwRBitCount;
	DWORD dwGBitCount;
	DWORD dwBBitCount;
	DWORD dwAlphaBitCount;

	// Calculated shifts for RGBA 
	DWORD dwRBitShift;
	DWORD dwGBitShift;
	DWORD dwBBitShift;
	DWORD dwAlphaBitShift;

} TTextureFormat;

cBaseArrayManager <IDirectDrawSurface7>	TextureLibrary;

// Local prototypes

static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT*, VOID* );
static DWORD GetMaskBitShift( DWORD );
static DWORD GetMaskBitCount( DWORD );

// Md3d globals

extern LPDIRECT3DDEVICE7    g_pd3dDevice;       // The D3D device
extern LPDIRECTDRAW7        g_pDD;              // The DirectDraw object
extern DWORD                m_dwDeviceMemType;

#define TEXFMT_COUNT 6

TTextureFormat g_TextureFormats[TEXFMT_COUNT] = {
	{ 
	  D3DTEXFMT_RGB565, FALSE, 
	  {0,0,0,0,0,0,0,0}, 
	  16, FALSE, 5, 6, 5, 0,
	  0, 0, 0, 0 },

	{ D3DTEXFMT_RGB555, FALSE, 
	  {0,0,0,0,0,0,0,0}, 
	  16, FALSE, 5, 5, 5, 0,
	  0, 0, 0, 0 },

	{ D3DTEXFMT_ARGB4444, FALSE, 
	  {0,0,0,0,0,0,0,0}, 
	  16, FALSE, 4, 4, 4, 4,
	  0, 0, 0, 0 },
	
	{ D3DTEXFMT_ARGB1555, FALSE, 
	  {0,0,0,0,0,0,0,0}, 
	  16, FALSE, 5, 5, 5, 1,
	  0, 0, 0, 0 },
	
	{ D3DTEXFMT_RGBA8888, FALSE, 
	  {0,0,0,0,0,0,0,0},
	  32, FALSE, 8, 8, 8, 8,
	  0, 0, 0, 0 },

	{ D3DTEXFMT_PAL8, FALSE, 
	  {0,0,0,0,0,0,0,0},
	  8, TRUE, 0, 0, 0, 0,
	  0, 0, 0, 0 },
};

MD3DERROR __d3dInitTextureManager()
{
	HRESULT hr;
	// Enumerate texture formats and fill the texture info array
	if(FAILED(hr=g_pd3dDevice->EnumTextureFormats(TextureSearchCallback,NULL)))
	{
		DEBUG_MSG( TEXT( "md3d: texture format enumeration failed\n" ) );
		return hr;
	}
	TextureLibrary.Attach((LPDIRECTDRAWSURFACE7)0xFFFFFFFF);
	return MD3D_OK;
}

MD3DERROR __d3dCloseTextureManager()
{
	for(int i=1;i<TextureLibrary.length();i++)
		if(TextureLibrary[i])
		{
			TextureLibrary[i]->Release();
			TextureLibrary.Detach(TextureLibrary[i]);
		}
	TextureLibrary.Detach((LPDIRECTDRAWSURFACE7)0xFFFFFFFF);
	TextureLibrary.length()=0;
	return MD3D_OK;
}

static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf, VOID* param )
{
    if( NULL==pddpf )
        return DDENUMRET_OK;

    // Count the alpha bits for the pixel format
    DWORD dwAlphaBitCount = 0;
    if( pddpf->dwFlags & DDPF_ALPHAPIXELS )
		dwAlphaBitCount = GetMaskBitCount( pddpf->dwRGBAlphaBitMask );

    // Skip any bumpmapping modes
    if( pddpf->dwFlags & (DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;
        
    // Skip any luminance-only or alpha-only modes
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_ALPHA) )
        return DDENUMRET_OK;

    // Skip any YUV or FourCC modes
    if( pddpf->dwFlags & (DDPF_YUV|DDPF_FOURCC) )
        return DDENUMRET_OK;

	// Skip 1-, 2- and 4-bit palettized textures
    if( pddpf->dwFlags & (DDPF_PALETTEINDEXED1 |
		                  DDPF_PALETTEINDEXED2 |
						  DDPF_PALETTEINDEXED4) )
        return DDENUMRET_OK;

    // Check for 8-bit palettized formats
    if( pddpf->dwFlags & DDPF_PALETTEINDEXED8 )
    {
        // Skip palettized formats that have an added alpha channel
        if( 0 != dwAlphaBitCount )
            return DDENUMRET_OK;
        
        // Search a PAL8 format in the array
		DWORD i;
		for( i=0; i<TEXFMT_COUNT; i++ )
			if( g_TextureFormats[i].bPalette8 )
				break;

		if( i<TEXFMT_COUNT )
		{
			g_TextureFormats[i].bSupported = TRUE;
			memcpy( &g_TextureFormats[i].ddpfPixelFormat, pddpf, sizeof(DDPIXELFORMAT) );
		}

		// Keep looking
        return DDENUMRET_OK;
    }

    // Else, skip any non-RGB formats
    if( 0 == ( pddpf->dwFlags & DDPF_RGB ) )
        return DDENUMRET_OK;

	// Get bit counts from masks for RGB components
	DWORD dwRBitCount = GetMaskBitCount( pddpf->dwRBitMask );
	DWORD dwGBitCount = GetMaskBitCount( pddpf->dwGBitMask );
	DWORD dwBBitCount = GetMaskBitCount( pddpf->dwBBitMask );

	// Match a format in our array with the current format
	for( DWORD i=0; i<TEXFMT_COUNT; i++ )
	{
		// Skip palletized entries
		if( g_TextureFormats[i].bPalette8 )
			continue;

		if( g_TextureFormats[i].dwRGBBitCount != pddpf->dwRGBBitCount ||
			g_TextureFormats[i].dwRBitCount != dwRBitCount ||
			g_TextureFormats[i].dwGBitCount != dwGBitCount ||
			g_TextureFormats[i].dwBBitCount != dwBBitCount ||
			g_TextureFormats[i].dwAlphaBitCount != dwAlphaBitCount )
			continue;

		// Found. 

		g_TextureFormats[i].bSupported = TRUE;
		memcpy( &g_TextureFormats[i].ddpfPixelFormat, pddpf, sizeof(DDPIXELFORMAT) );

		g_TextureFormats[i].dwRBitShift = GetMaskBitShift( pddpf->dwRBitMask );
		g_TextureFormats[i].dwGBitShift = GetMaskBitShift( pddpf->dwGBitMask );
		g_TextureFormats[i].dwBBitShift = GetMaskBitShift( pddpf->dwBBitMask );
		g_TextureFormats[i].dwAlphaBitShift = GetMaskBitShift( pddpf->dwRGBAlphaBitMask );
	}


    // Keep enumerating
    return DDENUMRET_OK;
}

static DWORD GetMaskBitShift( DWORD dwBits )
{
	if( 0 == dwBits )
		return 0;

	DWORD dwShift = 0;
    for( ; 0==(dwBits&1); dwBits>>=1 )
        dwShift++;

	return dwShift;
}
static DWORD GetMaskBitCount( DWORD dwBits )
{
	DWORD dwBitCount = 0;

    for( ; dwBits; dwBits>>=1 )
        dwBitCount += ( dwBits & 0x1 );

	return dwBitCount;
}

MD3DERROR d3dGetTextureFormatData( DWORD dwTexFormatID, M3DTEXTUREFORMAT* pData )
{
	for( DWORD i=0; i<TEXFMT_COUNT; i++ )
	{
		if( dwTexFormatID == g_TextureFormats[i].dwFormatID && 
			TRUE == g_TextureFormats[i].bSupported )
		{
			pData->dwTotalBitCount = g_TextureFormats[i].dwRGBBitCount;
			pData->bPalette8 = g_TextureFormats[i].bPalette8;

			pData->dwAlphaBitCount = g_TextureFormats[i].dwAlphaBitCount;
			pData->dwRBitCount = g_TextureFormats[i].dwRBitCount;
			pData->dwGBitCount = g_TextureFormats[i].dwGBitCount;
			pData->dwBBitCount = g_TextureFormats[i].dwBBitCount;

			pData->dwAlphaBitMask = g_TextureFormats[i].ddpfPixelFormat.dwRGBAlphaBitMask;
			pData->dwRBitMask = g_TextureFormats[i].ddpfPixelFormat.dwRBitMask;
			pData->dwGBitMask = g_TextureFormats[i].ddpfPixelFormat.dwGBitMask;
			pData->dwBBitMask = g_TextureFormats[i].ddpfPixelFormat.dwBBitMask;

			pData->dwAlphaBitShift = g_TextureFormats[i].dwAlphaBitShift;
			pData->dwRBitShift = g_TextureFormats[i].dwRBitShift;
			pData->dwGBitShift = g_TextureFormats[i].dwGBitShift;
			pData->dwBBitShift = g_TextureFormats[i].dwBBitShift;

			return MD3D_OK;
		}
	}
	return MD3DERR_ILLEGALCALL;
}
LPDIRECTDRAWSURFACE7 d3dCreateSurface(int x,int y,int SurfaceFormat)
{
	_ASSERTE(x&&y&&SurfaceFormat>=0&&SurfaceFormat<TEXFMT_COUNT);
    DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(DDSURFACEDESC2));
    ddsd.dwSize				= sizeof(DDSURFACEDESC2);
    ddsd.dwFlags			= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
	ddsd.dwWidth			= x;
    ddsd.dwHeight			= y;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_TEXTURE;
	if(m_dwDeviceMemType==DDSCAPS_VIDEOMEMORY)
		ddsd.ddsCaps.dwCaps2=DDSCAPS2_TEXTUREMANAGE;
	else // if(m_dwDeviceMemType==DDSCAPS_SYSTEMMEMORY)
		ddsd.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
	ddsd.ddpfPixelFormat=g_TextureFormats[SurfaceFormat].ddpfPixelFormat;
	LPDIRECTDRAWSURFACE7 lpDDSurface=0;
	g_pDD->CreateSurface(&ddsd,&lpDDSurface,NULL);
	_ASSERTE(lpDDSurface);
	return lpDDSurface;
}
MD3DERROR d3dCreateTexture(DWORD dwWidth, DWORD dwHeight, DWORD dwTexFormatID, DWORD* lpdwHandle )
{
	_ASSERTE( dwWidth == dwHeight );
	_ASSERTE( GetMaskBitCount(dwWidth) == 1 );
	_ASSERTE( GetMaskBitCount(dwHeight) == 1 );
	_ASSERTE( dwHeight <= 256 );
	_ASSERTE( dwWidth <= 256 );

	DWORD dwTexFmtIndex;
	for( dwTexFmtIndex=0; dwTexFmtIndex<TEXFMT_COUNT; dwTexFmtIndex++ )
	{
		if( dwTexFormatID == g_TextureFormats[dwTexFmtIndex].dwFormatID && 
			TRUE == g_TextureFormats[dwTexFmtIndex].bSupported )
			break;
	}
	_ASSERTE( dwTexFmtIndex<TEXFMT_COUNT );
	*lpdwHandle=TextureLibrary.Attach(d3dCreateSurface(dwWidth,dwHeight,dwTexFmtIndex));
	return MD3D_OK;
}
MD3DERROR d3dDeleteTexture(DWORD dwHandle)
{
	_ASSERTE(dwHandle>0&&TextureLibrary[dwHandle]);
	TextureLibrary[dwHandle]->Release();
	TextureLibrary.Detach(TextureLibrary[dwHandle]);
	return MD3D_OK;
}
MD3DERROR d3dLockTexture( DWORD dwHandle, VOID **lplpTexture, DWORD *lpPitch )
{
	_ASSERTE(dwHandle>0&&TextureLibrary[dwHandle]);
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(ddsd);
	if(FAILED(TextureLibrary[dwHandle]->Lock(NULL,&ddsd,DDLOCK_WAIT,0)))
		return MD3DERR_GENERIC;
	*lplpTexture=ddsd.lpSurface;
	*lpPitch=ddsd.lPitch;
	return MD3D_OK;
}
MD3DERROR d3dLockTexture( DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom,
						  VOID **lplpTexture, DWORD *lpPitch )
{
	_ASSERTE(dwHandle>0&&TextureLibrary[dwHandle]);
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(ddsd);
	if(FAILED(TextureLibrary[dwHandle]->Lock(NULL,&ddsd,DDLOCK_WAIT,0)))
		return MD3DERR_GENERIC;
	*lplpTexture=ddsd.lpSurface;
	*lpPitch=ddsd.lPitch;
	return MD3D_OK;
}
MD3DERROR d3dUnlockTexture( DWORD dwHandle )
{
	_ASSERTE(dwHandle>0&&TextureLibrary[dwHandle]);
	if(FAILED(TextureLibrary[dwHandle]->Unlock(NULL)))
		return MD3DERR_GENERIC;
	return MD3D_OK;
}
MD3DERROR d3dSetTexture(DWORD dwHandle,DWORD dwStage)
{
	_ASSERTE( dwStage>=0 && dwStage<2);
	_ASSERTE(dwHandle>0&&TextureLibrary[dwHandle]);
	if(FAILED(g_pd3dDevice->SetTexture(dwStage,TextureLibrary[dwHandle])))
	{
		DEBUG_MSG( TEXT("d3dSetTexture() SetTexture failed\n") );
		return MD3DERR_GENERIC;
	}
	return MD3D_OK;
}

