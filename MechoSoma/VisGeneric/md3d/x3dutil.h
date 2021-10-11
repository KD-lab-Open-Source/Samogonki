//*******************************************************************
//	X3D Utility Library 
//
//  K-D Lab::Spika
//*******************************************************************

#include <ddraw.h>

class x3uPixelConverter 
{
public:
	virtual VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight ) = 0;
	virtual VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight, DWORD dwColorKey ) = 0;
};

class x3uPixelConverterFactory
{
public:
	x3uPixelConverter* CreatePixelConverter( DDPIXELFORMAT *lpddpfSrcFormat, DDPIXELFORMAT *lpddpfDestFormat );
};

