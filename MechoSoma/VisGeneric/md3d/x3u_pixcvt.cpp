//*******************************************************************
//	X3D Utility Library 
//	Pixel conversion routines
//
//  K-D Lab::Spika
//*******************************************************************

// Includes ------------------------------------------------------- //

#include "x3dutil.h"
#include <stdio.h>
#include <crtdbg.h>
#include "md3d.h"


// 16 bit to 16 bit generic converter class template

template <int iRShiftDir, int iGShiftDir, int iBShiftDir>
class x3uPixelConverterGenCvt16 : public x3uPixelConverter
{

//public:
//	x3uPixelConverterGenCvt16( DWORD dwRBitMaskLow, int iRShift, DWORD dwGBitMaskLow, int iGShift, DWORD dwBBitMaskLow, int iBShift );
//	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight );
//	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight, DWORD dwColorKey );

//private:
//	VOID ConvertLine( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount );
//	VOID ConvertLine( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount, DWORD dwColorKey );

	// DWORD bitmasks hold two 16-bit bitmasks, in high and low words
//	DWORD dwRBitMask;	
//	DWORD dwGBitMask;
//	DWORD dwBBitMask;

//	DWORD dwRShift;
//	DWORD dwGShift;
//	DWORD dwBShift;

public:

	// Constructor
	x3uPixelConverterGenCvt16( DWORD dwRBitMaskLow, int iRShift, DWORD dwGBitMaskLow, int iGShift, DWORD dwBBitMaskLow, int iBShift )
	{
		dwRBitMask = (dwRBitMaskLow & 0xFFFF) | (dwRBitMaskLow << 16);	
		dwGBitMask = (dwGBitMaskLow & 0xFFFF) | (dwGBitMaskLow << 16);	
		dwBBitMask = (dwBBitMaskLow & 0xFFFF) | (dwBBitMaskLow << 16);	
		dwRShift = (DWORD)abs(iRShift);
		dwGShift = (DWORD)abs(iGShift);
		dwBShift = (DWORD)abs(iBShift);
	}

	//------------------------------------------------------------------------
	// 16 bit to 16 bit generic conversion, no colorkey
	//------------------------------------------------------------------------
	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight )
	{
		_ASSERTE( lpSrc );
		_ASSERTE( lpDest );	
		_ASSERTE( dwWidth > 0 );
		_ASSERTE( dwHeight > 0 );
		_ASSERTE( dwSrcPitch >= dwWidth*2 );
		_ASSERTE( dwDestPitch >= dwWidth*2 );

		while( dwHeight-- ) {
			ConvertLine( lpDest, lpSrc, dwWidth );
			lpSrc = (BYTE*)lpSrc + dwSrcPitch;
			lpDest = (BYTE*)lpDest + dwDestPitch;
		}
	}

	//------------------------------------------------------------------------
	// 16 bit to 16 bit generic conversion, with colorkey
	//------------------------------------------------------------------------
	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight, DWORD dwColorKey )
	{
		_ASSERTE( lpSrc );
		_ASSERTE( lpDest );
		_ASSERTE( dwWidth > 0 );
		_ASSERTE( dwHeight > 0 );
		_ASSERTE( dwSrcPitch >= dwWidth*2 );
		_ASSERTE( dwDestPitch >= dwWidth*2 );

		while( dwHeight-- ) {
			ConvertLine( lpDest, lpSrc, dwWidth, dwColorKey );
			lpSrc = (BYTE*)lpSrc + dwSrcPitch;
			lpDest = (BYTE*)lpDest + dwDestPitch;
		}
	}


private:

	//------------------------------------------------------------------------
	// 16 bit to 16 bit generic conversion, no colorkey
	//------------------------------------------------------------------------
	VOID ConvertLine( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount )
	{
		DWORD dwInPixel, dwOutPixel;

		// If lpSrc is not DWORD-aligned, convert one pixel
		// to make it aligned.

		if( ((DWORD)lpSrc & 0x3) != 0 ) {

			dwInPixel = *(WORD*)lpSrc;
			dwOutPixel = 0;
		
			// Red
			if( iRShiftDir < 0 ) 
				dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
			else if( iRShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
			else 
				dwOutPixel |= (dwInPixel & dwRBitMask);

			// Green
			if( iGShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
			else if( iGShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
			else 
				dwOutPixel |= (dwInPixel & dwGBitMask);

			// Blue
			if( iBShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
			else if( iBShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
			else 
				dwOutPixel |= (dwInPixel & dwBBitMask);

			*(WORD*)lpDest = (WORD)dwOutPixel;

			lpSrc = (WORD*)lpSrc + 1;
			lpDest = (WORD*)lpDest + 1;

			dwPixelCount--;
		}

		// Convert data 2 pixels at once

		while( dwPixelCount >= 2 ) {

			dwInPixel = *(DWORD*)lpSrc;
			dwOutPixel = 0;

			// Red
			if( iRShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
			else if( iRShiftDir > 0 )
				dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
			else 
				dwOutPixel |= (dwInPixel & dwRBitMask);

			// Green
			if( iGShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
			else if( iGShiftDir > 0 )
				dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
			else 
				dwOutPixel |= (dwInPixel & dwGBitMask);

			// Blue
			if( iBShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
			else if( iBShiftDir > 0 )
				dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
			else
				dwOutPixel |= (dwInPixel & dwBBitMask);

			*(DWORD*)lpDest = dwOutPixel;

			lpSrc = (DWORD*)lpSrc + 1;
			lpDest = (DWORD*)lpDest + 1;

			dwPixelCount -= 2;
		}

		// Convert the remaining one pixel, if any

		if( dwPixelCount > 0 ) {

			dwInPixel = *(WORD*)lpSrc;
			dwOutPixel = 0;
		
			// Red
			if( iRShiftDir < 0 ) 
				dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
			else if( iRShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
			else 
				dwOutPixel |= (dwInPixel & dwRBitMask);

			// Green
			if( iGShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
			else if( iGShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
			else 
				dwOutPixel |= (dwInPixel & dwGBitMask);

			// Blue
			if( iBShiftDir < 0 )
				dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
			else if( iBShiftDir > 0 ) 
				dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
			else 
				dwOutPixel |= (dwInPixel & dwBBitMask);

			*(WORD*)lpDest = (WORD)dwOutPixel;
		}
	}


	//------------------------------------------------------------------------
	// 16 bit to 16 bit generic conversion, with colorkey
	//------------------------------------------------------------------------
	VOID ConvertLine( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount, DWORD dwColorKey ) 
	{
		DWORD dwInPixel, dwOutPixel;

		// Ignore high word in dwColorKey 
		dwColorKey &= 0xFFFF;

		// If lpSrc is not DWORD-aligned, convert one pixel
		// to make it aligned.

		if( ((DWORD)lpSrc & 0x3) != 0 ) {

			dwInPixel = *(WORD*)lpSrc;

			if( dwInPixel != dwColorKey ) { 

				dwOutPixel = 0;
			
				// Red
				if( iRShiftDir < 0 ) 
					dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
				else if( iRShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
				else 
					dwOutPixel |= (dwInPixel & dwRBitMask);

				// Green
				if( iGShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
				else if( iGShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
				else 
					dwOutPixel |= (dwInPixel & dwGBitMask);

				// Blue
				if( iBShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
				else if( iBShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
				else 
					dwOutPixel |= (dwInPixel & dwBBitMask);

				*(WORD*)lpDest = (WORD)dwOutPixel;
			}

			lpSrc = (WORD*)lpSrc + 1;
			lpDest = (WORD*)lpDest + 1;

			dwPixelCount--;
		}

		// Convert data 2 pixels at once

		DWORD dwCKey = dwColorKey | (dwColorKey << 16);

		while( dwPixelCount >= 2 ) {

			dwInPixel = *(DWORD*)lpSrc;

			if( dwInPixel != dwCKey ) {

				dwOutPixel = 0;

				// Red
				if( iRShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
				else if( iRShiftDir > 0 )
					dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
				else 
					dwOutPixel |= (dwInPixel & dwRBitMask);

				// Green
				if( iGShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
				else if( iGShiftDir > 0 )
					dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
				else 
					dwOutPixel |= (dwInPixel & dwGBitMask);

				// Blue
				if( iBShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
				else if( iBShiftDir > 0 )
					dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
				else
					dwOutPixel |= (dwInPixel & dwBBitMask);

				// Now we have to test the two pixels separately and
				// write only those that pass

				if( (dwCKey & 0xFFFF) != (dwInPixel & 0xFFFF) && 
					(dwCKey & 0xFFFF0000) != (dwInPixel & 0xFFFF0000) )
					// Both pixels pass
					*(DWORD*)lpDest = dwOutPixel;
				else if( (dwCKey & 0xFFFF) != (dwInPixel & 0xFFFF) )
					// First passes
					*(WORD*)lpDest = (WORD)(dwOutPixel & 0xFFFF);
				else
					// Second passes
					*((WORD*)lpDest+1) = (WORD)(dwOutPixel >> 16);
			} 

			lpSrc = (DWORD*)lpSrc + 1;
			lpDest = (DWORD*)lpDest + 1;

			dwPixelCount -= 2;
		}

		// Convert the remaining one pixel, if any

		if( dwPixelCount > 0 ) {

			dwInPixel = *(WORD*)lpSrc;

			if( dwInPixel != dwColorKey ) { 

				dwOutPixel = 0;
			
				// Red
				if( iRShiftDir < 0 ) 
					dwOutPixel |= (dwInPixel & dwRBitMask) << dwRShift;
				else if( iRShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwRBitMask) >> dwRShift;
				else 
					dwOutPixel |= (dwInPixel & dwRBitMask);

				// Green
				if( iGShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwGBitMask) << dwGShift;
				else if( iGShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwGBitMask) >> dwGShift;
				else 
					dwOutPixel |= (dwInPixel & dwGBitMask);

				// Blue
				if( iBShiftDir < 0 )
					dwOutPixel |= (dwInPixel & dwBBitMask) << dwBShift;
				else if( iBShiftDir > 0 ) 
					dwOutPixel |= (dwInPixel & dwBBitMask) >> dwBShift;
				else 
					dwOutPixel |= (dwInPixel & dwBBitMask);

				*(WORD*)lpDest = (WORD)dwOutPixel;
			}
		}
	}

	// DWORD bitmasks hold two 16-bit bitmasks, in high and low words
	DWORD dwRBitMask;	
	DWORD dwGBitMask;
	DWORD dwBBitMask;

	DWORD dwRShift;
	DWORD dwGShift;
	DWORD dwBShift;
};



#if 0

//------------------------------------------------------------------------
// Template for special-case conversion: 
// 16 bit to 16 bit, no colorkey
//------------------------------------------------------------------------

template <WORD wRMask, int iRShift, WORD wGMask, int iGShift, WORD wBMask, int iBShift>
void _scvt_16( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount )
{
	WORD wInPixel, wOutPixel;
	DWORD dwInPixel, dwOutPixel;

	// If lpSrc is not DWORD-aligned, convert one pixel
	// and make it aligned.

	if( ((DWORD)lpSrc & 0x3) != 0 ) {

		wInPixel = *(WORD*)lpSrc;
		wOutPixel = 0;
	
		// Red
		if( iRShift < 0 )
			wOutPixel |= (wInPixel & wRMask) << -iRShift;
		else
			wOutPixel |= (wInPixel & wRMask) >> iRShift;

		// Green
		if( iGShift < 0 )
			wOutPixel |= (wInPixel & wGMask) << -iGShift;
		else 
			wOutPixel |= (wInPixel & wGMask) >> iGShift;

		// Blue
		if( iBShift < 0 )
			wOutPixel |= (wInPixel & wBMask) << -iBShift;
		else 
			wOutPixel |= (wInPixel & wBMask) >> iBShift;

		*(WORD*)lpDest = wOutPixel;

		lpSrc = (WORD*)lpSrc + 1;
		lpDest = (WORD*)lpDest + 1;

		dwPixelCount--;
	}

	// Make double-word masks

	DWORD dwRMask = wRMask | (wRMask << 16);
	DWORD dwGMask = wGMask | (wGMask << 16);
	DWORD dwBMask = wBMask | (wBMask << 16);

	// Convert data 2 pixels at once

	while( dwPixelCount >= 2 ) {

		dwInPixel = *(DWORD*)lpSrc;
		dwOutPixel = 0;

		// Red
		if( iRShift < 0 )
			dwOutPixel |= (dwInPixel & dwRMask) << -iRShift;
		else 
			dwOutPixel |= (dwInPixel & dwRMask) >> iRShift;

		// Green
		if( iGShift < 0 )
			dwOutPixel |= (dwInPixel & dwGMask) << -iGShift;
		else 
			dwOutPixel |= (dwInPixel & dwGMask) >> iGShift;

		// Blue
		if( iBShift < 0 )
			dwOutPixel |= (dwInPixel & dwBMask) << -iBShift;
		else 
			dwOutPixel |= (dwInPixel & dwBMask) >> iBShift;

		*(DWORD*)lpDest = dwOutPixel;

		lpSrc = (DWORD*)lpSrc + 1;
		lpDest = (DWORD*)lpDest + 1;

		dwPixelCount -= 2;
	}

	// Convert the remaining one pixel, if any

	if( dwPixelCount > 0 ) {

		wInPixel = *(WORD*)lpSrc;
		wOutPixel = 0;
	
		// Red
		if( iRShift < 0 )
			wOutPixel |= (wInPixel & wRMask) << -iRShift;
		else
			wOutPixel |= (wInPixel & wRMask) >> iRShift;

		// Green
		if( iGShift < 0 )
			wOutPixel |= (wInPixel & wGMask) << -iGShift;
		else 
			wOutPixel |= (wInPixel & wGMask) >> iGShift;

		// Blue
		if( iBShift < 0 )
			wOutPixel |= (wInPixel & wBMask) << -iBShift;
		else 
			wOutPixel |= (wInPixel & wBMask) >> iBShift;

		*(WORD*)lpDest = wOutPixel;
	}
}


//------------------------------------------------------------------------
// Template for special-case conversion: 
// 16 bit to 16 bit, colorkey, generic CPU
//------------------------------------------------------------------------

template <WORD wRMask, int iRShift, WORD wGMask, int iGShift, WORD wBMask, int iBShift>
void _scvtcc_16( VOID *lpDest, VOID *lpSrc, DWORD dwPixelCount, WORD wColorKey )
{
	WORD wInPixel, wOutPixel;
	DWORD dwInPixel, dwOutPixel;

	// If lpSrc is not DWORD-aligned, convert one pixel
	// and make it aligned.

	if( ((DWORD)lpSrc & 0x3) != 0 ) {

		wInPixel = *(WORD*)lpSrc;

		// Check the current pixel against colorkey
		if( wColorKey != wInPixel ) {

			wOutPixel = 0;
		
			// Red
			if( iRShift < 0 )
				wOutPixel |= (wInPixel & wRMask) << -iRShift;
			else
				wOutPixel |= (wInPixel & wRMask) >> iRShift;

			// Green
			if( iGShift < 0 )
				wOutPixel |= (wInPixel & wGMask) << -iGShift;
			else 
				wOutPixel |= (wInPixel & wGMask) >> iGShift;

			// Blue
			if( iBShift < 0 )
				wOutPixel |= (wInPixel & wBMask) << -iBShift;
			else 
				wOutPixel |= (wInPixel & wBMask) >> iBShift;

			*(WORD*)lpDest = wOutPixel;
		}

		lpSrc = (WORD*)lpSrc + 1;
		lpDest = (WORD*)lpDest + 1;

		dwPixelCount--;
	}

	// Make double-word masks

	DWORD dwRMask = wRMask | (wRMask << 16);
	DWORD dwGMask = wGMask | (wGMask << 16);
	DWORD dwBMask = wBMask | (wBMask << 16);

	// Make double-word colorkey
	DWORD dwColorKey = wColorKey | (wColorKey << 16);

	// Convert data 2 pixels at once
	while( dwPixelCount >= 2 ) {

		dwInPixel = *(DWORD*)lpSrc;

		// Check the current 2 pixels against colorkey
		if( dwColorKey != dwInPixel ) {

			// At least one pixel passes

			dwOutPixel = 0;

			// Red
			if( iRShift < 0 )
				dwOutPixel |= (dwInPixel & dwRMask) << -iRShift;
			else 
				dwOutPixel |= (dwInPixel & dwRMask) >> iRShift;

			// Green
			if( iGShift < 0 )
				dwOutPixel |= (dwInPixel & dwGMask) << -iGShift;
			else 
				dwOutPixel |= (dwInPixel & dwGMask) >> iGShift;

			// Blue
			if( iBShift < 0 )
				dwOutPixel |= (dwInPixel & dwBMask) << -iBShift;
			else 
				dwOutPixel |= (dwInPixel & dwBMask) >> iBShift;


			// Now we have to test the two pixels separately and
			// write only those that pass

			if( (dwColorKey & 0xFFFF) != (dwInPixel & 0xFFFF) && 
				(dwColorKey & 0xFFFF0000) != (dwInPixel & 0xFFFF0000) ) {

				// Both pixels pass
				*(DWORD*)lpDest = dwOutPixel;

				// Here i'm trying to play with VC optimizer to make 
				// things faster (and it works more or less)

				lpSrc = (DWORD*)lpSrc + 1;
				lpDest = (DWORD*)lpDest + 1;

				dwPixelCount -= 2;

				continue;
			} 

			if( (dwColorKey & 0xFFFF) != (dwInPixel & 0xFFFF) )
				// First passes
				*(WORD*)lpDest = (WORD)(dwOutPixel & 0xFFFF);
			else
				// Second passes
				*((WORD*)lpDest+1) = (WORD)(dwOutPixel >> 16);
		} 				

		lpSrc = (DWORD*)lpSrc + 1;
		lpDest = (DWORD*)lpDest + 1;

		dwPixelCount -= 2;
	}

	// Convert the remaining one pixel, if any

	if( dwPixelCount > 0 ) {

		wInPixel = *(WORD*)lpSrc;

		// Check the pixel against colorkey
		if( wColorKey != wInPixel ) {

			wOutPixel = 0;
		
			// Red
			if( iRShift < 0 )
				wOutPixel |= (wInPixel & wRMask) << -iRShift;
			else
				wOutPixel |= (wInPixel & wRMask) >> iRShift;

			// Green
			if( iGShift < 0 )
				wOutPixel |= (wInPixel & wGMask) << -iGShift;
			else 
				wOutPixel |= (wInPixel & wGMask) >> iGShift;

			// Blue
			if( iBShift < 0 )
				wOutPixel |= (wInPixel & wBMask) << -iBShift;
			else 
				wOutPixel |= (wInPixel & wBMask) >> iBShift;

			*(WORD*)lpDest = wOutPixel;
		}
	}
}

#endif


////////////////////////////////////24/32 бита
#pragma pack(1)
struct RGB24
{
	BYTE b;
	BYTE g;
	BYTE r;
};
struct RGB32
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};
#pragma pack()

const int r_shl=11,
	g_shl=5,
	b_shl=0,
	r_mask=5,
	g_mask=6,
	b_mask=5,
	rmask=31,
	gmask=63,
	bmask=31;

template <class SRGB>
class x3uPixelConverterTrueColor:public x3uPixelConverter
{
public:
	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight )
	{
		_ASSERTE( lpSrc );
		_ASSERTE( lpDest );	
		_ASSERTE( dwWidth > 0 );
		_ASSERTE( dwHeight > 0 );
		_ASSERTE( dwSrcPitch >= dwWidth*2);
		_ASSERTE( dwDestPitch >= dwWidth*sizeof(SRGB));

		while( dwHeight-- )
		{
			WORD* ps=(WORD*)lpSrc;
			SRGB* pd=(SRGB*)lpDest;

			for(DWORD i=0;i<dwWidth;i++,ps++,pd++)
			{
				pd->r=((*ps>>r_shl)&rmask)<<(8-r_mask);
				pd->g=((*ps>>g_shl)&gmask)<<(8-g_mask);
				pd->b=((*ps>>b_shl)&gmask)<<(8-b_mask);
			}

			lpSrc = (BYTE*)lpSrc + dwSrcPitch;
			lpDest = (BYTE*)lpDest + dwDestPitch;
		}
	}

	VOID BltConv( VOID *lpDest, DWORD dwDestPitch, VOID *lpSrc, DWORD dwSrcPitch, DWORD dwWidth, DWORD dwHeight, DWORD dwColorKey )
	{
		WORD wColorKey=WORD(dwColorKey&0xFFFF);
		_ASSERTE( lpSrc );
		_ASSERTE( lpDest );	
		_ASSERTE( dwWidth > 0 );
		_ASSERTE( dwHeight > 0 );
		_ASSERTE( dwSrcPitch >= dwWidth*2);
		_ASSERTE( dwDestPitch >= dwWidth*sizeof(SRGB));

		while( dwHeight-- )
		{
			WORD* ps=(WORD*)lpSrc;
			SRGB* pd=(SRGB*)lpDest;

			for(DWORD i=0;i<dwWidth;i++,ps++,pd++)
			if(*ps!=wColorKey)
			{
				pd->r=((*ps>>r_shl)&rmask)<<(8-r_mask);
				pd->g=((*ps>>g_shl)&gmask)<<(8-g_mask);
				pd->b=((*ps>>b_shl)&gmask)<<(8-b_mask);
			}

			lpSrc = (BYTE*)lpSrc + dwSrcPitch;
			lpDest = (BYTE*)lpDest + dwDestPitch;
		}
	}
};


//------------------------------------------------------------------------
// Converter factory
//------------------------------------------------------------------------

// This is to get a unique hash code for three shifts, 
// depending on their sign
#define HASH_SHIFTS(r,g,b) ( ((r<0) ? 0xF00 : ((r>0)?0x100:0)) | \
							 ((g<0) ? 0x0F0 : ((g>0)?0x010:0)) | \
							 ((b<0) ? 0x00F : ((b>0)?0x001:0)) )

// TRUE if a and b masks are aligned at the MSB, FALSE otherwise
inline BOOL MSB_ALIGNED( DWORD a, DWORD b ) { return (a^b)<a && (a^b)<b; }
 
x3uPixelConverter* x3uPixelConverterFactory::CreatePixelConverter( DDPIXELFORMAT *lpddpfSrcFormat, DDPIXELFORMAT *lpddpfDestFormat ) 
{
	_ASSERTE( lpddpfSrcFormat );
	_ASSERTE( sizeof(DDPIXELFORMAT) == lpddpfSrcFormat->dwSize );
	_ASSERTE( lpddpfDestFormat );
	_ASSERTE( sizeof(DDPIXELFORMAT) == lpddpfDestFormat->dwSize );

	_ASSERTE( lpddpfSrcFormat->dwFlags & DDPF_RGB );
	_ASSERTE( lpddpfDestFormat->dwFlags & DDPF_RGB );
	_ASSERTE( 16 == lpddpfSrcFormat->dwRGBBitCount );

	if(lpddpfDestFormat->dwRGBBitCount==24)
	{
		return new x3uPixelConverterTrueColor<RGB24>;
	}
	if(lpddpfDestFormat->dwRGBBitCount==32)
	{
		return new x3uPixelConverterTrueColor<RGB32>;
	}

	_ASSERTE( 16 == lpddpfDestFormat->dwRGBBitCount );
	_ASSERTE( lpddpfSrcFormat->dwRBitMask );
	_ASSERTE( lpddpfSrcFormat->dwGBitMask );
	_ASSERTE( lpddpfSrcFormat->dwBBitMask );

	DWORD dwSrcMask, dwDestMask;
	DWORD dwRBitMask, dwGBitMask, dwBBitMask;
	int iRShift, iGShift, iBShift;

	// Red

	dwSrcMask = lpddpfSrcFormat->dwRBitMask;
	dwDestMask = lpddpfDestFormat->dwRBitMask;
	iRShift = 0;
	if( dwSrcMask < dwDestMask ) {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask <<= 1;
			iRShift--;
		}
		dwRBitMask = (dwSrcMask & dwDestMask) >> (-iRShift);
	} else {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask >>= 1;
			iRShift++;
		}
		dwRBitMask = (dwSrcMask & dwDestMask) << iRShift;
	}

	// Green

	dwSrcMask = lpddpfSrcFormat->dwGBitMask;
	dwDestMask = lpddpfDestFormat->dwGBitMask;
	iGShift = 0;
	if( dwSrcMask < dwDestMask ) {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask <<= 1;
			iGShift--;
		}
		dwGBitMask = (dwSrcMask & dwDestMask) >> (-iGShift);
	} else {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask >>= 1;
			iGShift++;
		}
		dwGBitMask = (dwSrcMask & dwDestMask) << iGShift;
	}

	// Blue

	dwSrcMask = lpddpfSrcFormat->dwBBitMask;
	dwDestMask = lpddpfDestFormat->dwBBitMask;
	iBShift = 0;
	if( dwSrcMask < dwDestMask ) {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask <<= 1;
			iBShift--;
		}
		dwBBitMask = (dwSrcMask & dwDestMask) >> (-iBShift);
	} else {
		while( !MSB_ALIGNED(dwSrcMask, dwDestMask) ) {
			dwSrcMask >>= 1;
			iBShift++;
		}
		dwBBitMask = (dwSrcMask & dwDestMask) << iBShift;
	}

	// Create an optimized converter if possible

	// &&&

#ifdef _DEBUG
//	char buf[200];
//	sprintf( buf, "An unoptimized converter used.\n\nR: 0x%LX, %d | G: 0x%LX, %d | B: 0x%LX, %d\n\n(this warning may be safely ignored)\n", dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
//	_RPT0( _CRT_ERROR, buf );
#endif

	// Create a generic converter based on shift directions

	switch( HASH_SHIFTS( iRShift, iGShift, iBShift ) ) 
	{

	case HASH_SHIFTS(0,0,0):
		return new x3uPixelConverterGenCvt16<0,0,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,0,1):
		return new x3uPixelConverterGenCvt16<0,0,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,0,-1):
		return new x3uPixelConverterGenCvt16<0,0,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(0,1,0):
		return new x3uPixelConverterGenCvt16<0,1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,1,1):
		return new x3uPixelConverterGenCvt16<0,1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,1,-1):
		return new x3uPixelConverterGenCvt16<0,1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(0,-1,0):
		return new x3uPixelConverterGenCvt16<0,-1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,-1,1):
		return new x3uPixelConverterGenCvt16<0,-1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(0,-1,-1):
		return new x3uPixelConverterGenCvt16<0,-1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	//---

	case HASH_SHIFTS(1,0,0):
		return new x3uPixelConverterGenCvt16<1,0,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,0,1):
		return new x3uPixelConverterGenCvt16<1,0,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,0,-1):
		return new x3uPixelConverterGenCvt16<1,0,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(1,1,0):
		return new x3uPixelConverterGenCvt16<1,1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,1,1):
		return new x3uPixelConverterGenCvt16<1,1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,1,-1):
		return new x3uPixelConverterGenCvt16<1,1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(1,-1,0):
		return new x3uPixelConverterGenCvt16<1,-1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,-1,1):
		return new x3uPixelConverterGenCvt16<1,-1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(1,-1,-1):
		return new x3uPixelConverterGenCvt16<1,-1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	//---

	case HASH_SHIFTS(-1,0,0):
		return new x3uPixelConverterGenCvt16<-1,0,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,0,1):
		return new x3uPixelConverterGenCvt16<-1,0,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,0,-1):
		return new x3uPixelConverterGenCvt16<-1,0,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(-1,1,0):
		return new x3uPixelConverterGenCvt16<-1,1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,1,1):
		return new x3uPixelConverterGenCvt16<-1,1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,1,-1):
		return new x3uPixelConverterGenCvt16<-1,1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;

	case HASH_SHIFTS(-1,-1,0):
		return new x3uPixelConverterGenCvt16<-1,-1,0>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,-1,1):
		return new x3uPixelConverterGenCvt16<-1,-1,1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	case HASH_SHIFTS(-1,-1,-1):
		return new x3uPixelConverterGenCvt16<-1,-1,-1>( dwRBitMask, iRShift, dwGBitMask, iGShift, dwBBitMask, iBShift );
		break;
	}

	return NULL;
}
