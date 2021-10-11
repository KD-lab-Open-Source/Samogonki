//*******************************************************************
//	Mechosoma D3D Wrapper API
//  -- TSS pipe stuff
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include <stdio.h>
#include "d3dUtil.h"


// Md3d globals
extern LPDIRECT3DDEVICE7    g_pd3dDevice;       // The D3D device
extern BOOL					g_bInScene;			// TRUE when in BeginScene/EndScene bracket

// Module globals
DWORD	m_dwCurrentBlendMode;		// Currently set blending mode

// Macro for making combined blend mode
#define BLEND_MODE(c,a) ((c)|((a)<<8))


//-----------------------------------------------------------------------------
// Name: __TSS_InitPipeline
// Desc: Set default TSS configuration
//-----------------------------------------------------------------------------
HRESULT __TSS_InitPipeline()
{
	_ASSERTE( FALSE == g_bInScene );

	DWORD i;

	g_pd3dDevice->BeginScene();

	// Default setup for a generic card

	for( i=0; i<8; i++ ) 
	{
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_TEXCOORDINDEX, i );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( i, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	}

	g_pd3dDevice->EndScene();

	// Init current blend mode to something never used
	m_dwCurrentBlendMode = 0xFFFFFFFF;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: d3dSetTextureBlendMode
// Desc: Configure the TSS pipe for the given texture blending operation
//-----------------------------------------------------------------------------
MD3DERROR d3dSetTextureBlendMode( MD3DTEXTUREBLEND tbRGBBlendMode, MD3DTEXTUREBLEND tbAlphaBlendMode )
{
	_ASSERTE( g_bInScene );

	DWORD dwBlendMode = BLEND_MODE( tbRGBBlendMode, tbAlphaBlendMode );
	if( dwBlendMode == m_dwCurrentBlendMode )
		return MD3D_OK;

	switch(tbRGBBlendMode)
	{
	case MD3DTB_DIFFUSE:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		break;
	case MD3DTB_TEXTURE1:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		break;
	case MD3DTB_TEXTURE1_MOD_DIFFUSE:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		break;
	default:
		_RPT0( _CRT_ERROR, "d3dSetTextureBlendMode: Blending mode unsupported." );
		return MD3DERR_INVALIDPARAM;
	}

	switch(tbAlphaBlendMode)
	{
	case MD3DTB_DIFFUSE:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		break;
	case MD3DTB_TEXTURE1:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		break;
	case MD3DTB_TEXTURE1_MOD_DIFFUSE:
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		break;
	default:
		_RPT0( _CRT_ERROR, "d3dSetTextureBlendMode: Blending mode unsupported." );
		return MD3DERR_INVALIDPARAM;
	}
/*
	// Validate TSS setup
	DWORD dwPasses;
	if( FAILED( hr = g_pd3dDevice->ValidateDevice( &dwPasses ) ) )
	{
#define SW(x) case x:_RPT0(_CRT_ERROR,#x);break;
		switch(hr)
		{
		SW(DDERR_INVALIDOBJECT);
		SW(DDERR_INVALIDPARAMS  );
		SW(D3DERR_CONFLICTINGTEXTUREFILTER );
		SW(D3DERR_CONFLICTINGTEXTUREPALETTE );
		SW(D3DERR_TOOMANYOPERATIONS );
		SW(D3DERR_UNSUPPORTEDALPHAARG );
		SW(D3DERR_UNSUPPORTEDALPHAOPERATION );
		SW(D3DERR_UNSUPPORTEDCOLORARG );
		SW(D3DERR_UNSUPPORTEDCOLOROPERATION );
		SW(D3DERR_UNSUPPORTEDFACTORVALUE );
		SW(D3DERR_UNSUPPORTEDTEXTUREFILTER );
		SW(D3DERR_WRONGTEXTUREFORMAT );
		}
#undef SW

		_RPT2( _CRT_ERROR, "d3dSetTextureBlendMode: ValidateDevice failed.\n dwBlendMode=0x%X. Error=0x%x.\n", dwBlendMode, hr );
		//MD3DLOG2( 0, "d3dSetTextureBlendMode: ValidateDevice failed.\n dwBlendMode=0x%X. Error=0x%x.\n", dwBlendMode, hr );
		return MD3DERR_UNSUPPORTED;
	}
	MD3DLOG1( 3, "d3dSetTextureBlendMode: Succeeded. Passes=%d.\n", dwPasses );
*/
	// Remember this blend mode
	m_dwCurrentBlendMode = dwBlendMode;

	return MD3D_OK;
}
MD3DERROR d3dSetTextureStageState(DWORD dwStage,D3DTEXTURESTAGESTATETYPE dwState,DWORD dwValue)
{
	if(FAILED(g_pd3dDevice->SetTextureStageState(dwStage,dwState,dwValue)))
		DEBUG_MSG( TEXT("d3dSetTextureStageState() SetTextureStageState failed\n"));
	return MD3D_OK;
}

