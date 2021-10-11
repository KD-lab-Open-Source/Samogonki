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

extern LPDIRECTDRAW7	g_pDD;
extern BOOL				g_bInitialized;
extern BOOL				g_bInScene;
extern LPDIRECTDRAWSURFACE7 g_pddsFrontBuffer;
extern DDPIXELFORMAT	g_ddpfPrimary;
extern DWORD			g_dwRenderWidth;    // Dimensions of the render target
extern DWORD			g_dwRenderHeight;
extern HWND				m_hRenderWnd;

void ParseBitMask( DWORD dwBits, DWORD *lpdwZeros, DWORD *lpdwOnes );

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
//-----------------------------------------------------------------------------
// Name: d3dScreenShot( VOID *lpBuffer, DWORD dwSize )
// Desc: Get a screenshot into the supplied buffer (in TARGA format)
//       3 BYTE RGB
//-----------------------------------------------------------------------------
MD3DERROR d3dScreenShot( VOID *lpBuffer, DWORD dwSize )
{
#pragma pack(1)
	struct RGB24
	{
		BYTE r;
		BYTE g;
		BYTE b;
	};
#pragma pack()

	DWORD dwLines = g_dwRenderHeight;
	DWORD dwWidth = g_dwRenderWidth;

	HRESULT hr;
	_ASSERTE( g_bInitialized );
	_ASSERTE( !g_bInScene );
	if( dwSize < g_dwRenderWidth*g_dwRenderHeight*3 )
	{
		_RPT0( _CRT_ERROR, "d3dScreenShot: Buffer is too little.\n" );
		return MD3DERR_INVALIDPARAM;	
	}

	// Make sure the size of the buffer is large enough
	BITMAPINFOHEADER bm;
	bm.biSize=sizeof(bm);
	bm.biWidth=g_dwRenderWidth;
	bm.biHeight=g_dwRenderHeight;
	bm.biPlanes=1;
	bm.biBitCount=g_ddpfPrimary.dwRGBBitCount==16?16:24;
	bm.biCompression=BI_RGB;
	bm.biSizeImage=0;
	bm.biXPelsPerMeter=0;
	bm.biYPelsPerMeter=0;
	bm.biClrUsed=0;
	bm.biClrImportant=0;

	BYTE* pDIBBits;
	HBITMAP hbm=CreateDIBSection(NULL,
		(BITMAPINFO*)&bm,
		DIB_RGB_COLORS,
		(void**)&pDIBBits,
		NULL,0);

	if(hbm==NULL)
		return -1;

	HDC mem_dc=CreateCompatibleDC(GetDC(NULL));
	if(mem_dc==NULL)
		return -1;

	HBITMAP hOldBitmap=(HBITMAP)SelectObject(mem_dc,hbm);
  
	HDC front_dc;
	if(FAILED(hr=g_pddsFrontBuffer->GetDC(&front_dc)))
		return hr;

	POINT pt={0,0};
	ClientToScreen(m_hRenderWnd,&pt);
	BitBlt(mem_dc,0,0,dwWidth,dwLines,front_dc,pt.x,pt.y,SRCCOPY);

	g_pddsFrontBuffer->ReleaseDC(front_dc);

	if(g_ddpfPrimary.dwRGBBitCount==16)
	{
		DWORD dwSize = WIDTHBYTES(dwWidth*16);
		for(DWORD y=0;y<dwLines;y++)
		{
			WORD* in=(WORD*)(pDIBBits+dwSize*(dwLines-1-y));
//			WORD* in=(WORD*)(pDIBBits+dwSize*y);
			RGB24* out=(RGB24*)((BYTE*)lpBuffer+y*dwWidth*3);

			for(DWORD x=0;x<dwWidth;x++,in++,out++)
			{
				out->r=(*in&31)<<3;
				out->g=((*in>>5)&31)<<3;
				out->b=((*in>>10)&31)<<3;
			}
		}
	}else
	{
		DWORD dwSize = WIDTHBYTES(dwWidth*24);
		for(DWORD y=0;y<dwLines;y++)
		{
			RGB24* in=(RGB24*)(pDIBBits+dwSize*(dwLines-1-y));
//			RGB24* in=(RGB24*)(pDIBBits+dwSize*y);
			RGB24* out=(RGB24*)((BYTE*)lpBuffer+y*dwWidth*3);

			for(DWORD x=0;x<dwWidth;x++,in++,out++)
			{
				out->r=in->r;
				out->g=in->g;
				out->b=in->b;
			}
		}
	}

	SelectObject(mem_dc,hOldBitmap);
	DeleteDC(mem_dc);
	DeleteObject(hbm);

	return MD3D_OK;
}


void ParseBitMask( DWORD dwBits, DWORD *lpdwZeros, DWORD *lpdwOnes )
{
	if( 0 == dwBits ) {
		*lpdwZeros = 0;
		*lpdwOnes = 0;
		return;
	}

	DWORD dwZeros = 0;
	DWORD dwOnes = 0;

    for( ; 0==(dwBits&1); dwBits>>=1 )
        dwZeros++;

    for( ; 1==(dwBits&1); dwBits>>=1 )
        dwOnes++;

	*lpdwZeros = dwZeros;
	*lpdwOnes = dwOnes;
}
