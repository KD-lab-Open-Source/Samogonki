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


extern BOOL	g_bInitialized;
extern BOOL	g_bIsFullscreen;    

extern LPDIRECT3DDEVICE7	g_pd3dDevice;       // The D3D device

extern LPDIRECTDRAWSURFACE7	g_pddsFrontBuffer;  // The primary surface
extern LPDIRECTDRAWSURFACE7	g_pddsBackBuffer;   // The backbuffer surface

extern DWORD	g_dwRenderWidth;
extern DWORD	g_dwRenderHeight;
extern RECT		g_rcScreenRect;					// Screen rect 
extern BOOL		g_bInScene;		 // TRUE when in BeginScene/EndScene bracket

MD3DERROR d3dTriangles(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

    if(FAILED(g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,dwVertexTypeDesc,lpvVertices,dwVertexCount,0)))
	{
		DEBUG_MSG( TEXT("d3dTriangles() DrawPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}



MD3DERROR d3dTriangleStrip(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

    if(FAILED(g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,dwVertexTypeDesc,lpvVertices,dwVertexCount,0)))
    {
		DEBUG_MSG( TEXT("d3dTriangleStrip() DrawPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}



MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

	HRESULT hr=g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,dwVertexTypeDesc,lpvVertices,dwVertexCount,0);
	if(hr==D3DERR_INVALIDRAMPTEXTURE)
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed D3DERR_INVALIDRAMPTEXTURE\n") );
	if(hr==D3DERR_INVALIDPRIMITIVETYPE)
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed D3DERR_INVALIDPRIMITIVETYPE\n") );
	if(hr==D3DERR_INVALIDVERTEXTYPE)
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed D3DERR_INVALIDVERTEXTYPE\n") );
	if(hr==DDERR_INVALIDPARAMS)
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed DDERR_INVALIDPARAMS\n") );
	if(hr==DDERR_WASSTILLDRAWING)
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed DDERR_WASSTILLDRAWING\n") );

	if( FAILED( hr ) )
    {
		DEBUG_MSG( TEXT("d3dTriangleFan() DrawPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}



MD3DERROR d3dPoints(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

    if(FAILED(g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST,dwVertexTypeDesc,lpvVertices,dwVertexCount,0)))
    {
		DEBUG_MSG( TEXT("d3dPoints() DrawPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}



MD3DERROR d3dTrianglesIndexed(DWORD dwVertexTypeDesc, 
							  LPVOID lpvVertices, DWORD dwVertexCount, 
							  LPWORD lpwIndices, DWORD dwIndexCount)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

	HRESULT hr;
    if(FAILED(hr=g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,dwVertexTypeDesc,
		lpvVertices,dwVertexCount,lpwIndices,dwIndexCount,0)))
    {
		DEBUG_MSG( TEXT("d3dTrianglesIndexed() DrawIndexedPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}


MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

    if( FAILED( g_pd3dDevice->SetRenderState( dwRenderStateType, dwRenderState ) ) )
    {
		DEBUG_MSG( TEXT("d3dSetRenderState() SetRenderState failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}


MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

    if( FAILED( g_pd3dDevice->GetRenderState( dwRenderStateType, lpdwRenderState ) ) )
    {
		DEBUG_MSG( TEXT("d3dSetRenderState() SetRenderState failed\n") );
		return MD3DERR_GENERIC;
    }

	return MD3D_OK;
}


////////////////////multitexture///////////
MD3DERROR d3dTriangles2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
						DWORD dwHandleTex0,DWORD dwHandleTex1
						)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

	int num_uv=(dwVertexTypeDesc&D3DFVF_TEXCOUNT_MASK)>>D3DFVF_TEXCOUNT_SHIFT;
	_ASSERTE(num_uv>=1 );

    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	if(IsMultiTextureSupport())
	{
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, num_uv>1?1:0 );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );

        // Draw the walls in multi-texture mode
        d3dSetTexture( dwHandleTex0,0);
		d3dSetTexture( dwHandleTex1,1);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,dwVertexTypeDesc,lpvVertices,dwVertexCount,0);

		d3dSetTexture( 0,1 );
	}else
	{
        d3dSetTexture( dwHandleTex0,0 );
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,dwVertexTypeDesc,lpvVertices,dwVertexCount,0);

        // Multi-pass using color blending
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ZERO );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

        // Use the lightmap texture for the 2nd pass
        d3dSetTexture( dwHandleTex1,0);
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, num_uv>1?1:0 );
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,dwVertexTypeDesc,lpvVertices,dwVertexCount,0);

		g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	}

	
/*
    if( FAILED( g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 
									dwVertexTypeDesc, lpvVertices, 
									dwVertexCount, dwFlags ) ) )
	{
		DEBUG_MSG( TEXT("d3dTriangles() DrawPrimitive failed\n") );
		return MD3DERR_GENERIC;
    }
*/
	return MD3D_OK;
}

MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, 
							  LPVOID lpvVertices, DWORD dwVertexCount, 
							  LPWORD lpwIndices, DWORD dwIndexCount,
							  DWORD dwHandleTex0,DWORD dwHandleTex1)
{
	_ASSERTE( g_bInitialized );
	_ASSERTE( g_bInScene );

	MD3DERROR hr=MD3D_OK;

	int num_uv=(dwVertexTypeDesc&D3DFVF_TEXCOUNT_MASK)>>D3DFVF_TEXCOUNT_SHIFT;
	_ASSERTE(num_uv>=1 );

	DWORD colorop0,alphaop0;
	g_pd3dDevice->GetTextureStageState(0,D3DTSS_COLOROP,&colorop0);
	g_pd3dDevice->GetTextureStageState(0,D3DTSS_ALPHAOP,&alphaop0);

    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );

    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	if(IsMultiTextureSupport())
	{
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, num_uv>1?1:0 );

        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
        g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );

		DWORD clamp;
		g_pd3dDevice->GetTextureStageState(1,D3DTSS_ADDRESS,&clamp);
		g_pd3dDevice->SetTextureStageState(1,D3DTSS_ADDRESS,D3DTADDRESS_CLAMP);

        // Draw the walls in multi-texture mode
        d3dSetTexture( dwHandleTex0,0 );
		d3dSetTexture( dwHandleTex1,1 );

		hr=d3dTrianglesIndexed(dwVertexTypeDesc,lpvVertices,dwVertexCount,lpwIndices,dwIndexCount);

		g_pd3dDevice->SetTexture(1,0);

		g_pd3dDevice->SetTextureStageState(1,D3DTSS_ADDRESS,clamp);

		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}else
	{
		DWORD SrcBlend,DestBlend,AlphaBlendEnable;
        g_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND,  &SrcBlend );
        g_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &DestBlend );
        g_pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &AlphaBlendEnable );

		g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   alphaop0);

        d3dSetTexture( dwHandleTex0,0 );
		d3dTrianglesIndexed(dwVertexTypeDesc, 
							  lpvVertices,dwVertexCount, 
							  lpwIndices,dwIndexCount);

        // Multi-pass using color blending
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ZERO );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

        // Use the lightmap texture for the 2nd pass
        d3dSetTexture( dwHandleTex1,0 );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, num_uv>1?1:0 );

		hr=d3dTrianglesIndexed(dwVertexTypeDesc, 
							  lpvVertices,dwVertexCount, 
							  lpwIndices,dwIndexCount);

        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,  SrcBlend );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, DestBlend );
        g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, AlphaBlendEnable );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	}

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   colorop0 );
	
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   alphaop0 );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	
    g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	return hr;
}
