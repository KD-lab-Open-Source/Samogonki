//*******************************************************************
//	
//	Mechosoma D3D Wrapper API
//
//  K-D Lab::Spika
//
//*******************************************************************
#pragma once
// Includes ------------------------------------------------------- //
#ifndef STRICT
#define STRICT 1
#endif

#ifdef _WIN32
#include <windows.h>
#include <d3d.h>
#else
#include "d3d_port.h"
#endif

//#define _PROFILE_D3D

// Types ---------------------------------------------------------- //

typedef DWORD MD3DERROR;

#ifndef _WIN32
#define FAILED(hr) (((MD3DERROR)(hr)) < 0)
#endif

typedef struct {
	DWORD dwTotalBitCount;
	BOOL bPalette8;

	DWORD dwAlphaBitCount;
	DWORD dwRBitCount;
	DWORD dwGBitCount;
	DWORD dwBBitCount;

	DWORD dwAlphaBitMask;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;

	DWORD dwAlphaBitShift;
	DWORD dwRBitShift;
	DWORD dwGBitShift;
	DWORD dwBBitShift;
} M3DTEXTUREFORMAT;

typedef struct {
	DWORD dwFormat;
	DWORD dwWidth, dwHeight;
	DWORD dwFlags;				// Slot attributes
	DWORD dwRequired;
	DWORD dwDesired;
	DWORD dwCreated;
} MD3DCACHESLOTPROFILE;


// For pixel format conversion

typedef struct {
	DWORD dwRBitMask;
	DWORD dwRShiftL;
	DWORD dwRShiftR;

	DWORD dwGBitMask;
	DWORD dwGShiftL;
	DWORD dwGShiftR;

	DWORD dwBBitMask;
	DWORD dwBShiftL;
	DWORD dwBShiftR;
} CVTPARAMS;


// Texture blending modes
typedef enum {
  MD3DTB_DIFFUSE = 1,
  MD3DTB_TEXTURE1 = 2, 
  MD3DTB_TEXTURE1_MOD_DIFFUSE = 3
} MD3DTEXTUREBLEND;

// Gamma support modes

typedef enum {
  MD3DGAMMA_RAMDACCALIBRATED = 1,
  MD3DGAMMA_RAMDAC = 2,
  MD3DGAMMA_EMULATION = 3
} MD3DGAMMASUPPORT;


// Caps

typedef enum {
  MD3DCAPS_WFOG = 1,
} MD3DCAPS;


struct MD3DMODE
{
	DWORD dx,dy;
	DWORD bitperpixel;
};

// Function prototypes -------------------------------------------- //

//////array ����� ������������� ���������� ���������� ��� ������ delete[] pArray;
MD3DERROR d3dEnumVideoMode(int* pnumvideomode,MD3DMODE** ppArray);
MD3DERROR d3dGetAvailableVidMem (DWORD* allvideomem);

MD3DERROR d3dInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle );
MD3DERROR d3dReInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle );

MD3DERROR d3dClose();
MD3DERROR d3dQueryCaps( MD3DCAPS Caps, DWORD *dwData );
MD3DERROR d3dGetWindowHandle( HWND *hWnd );
MD3DERROR d3dClear(DWORD);
MD3DERROR d3dFlip(bool WaitVerticalBlank=true);
MD3DERROR d3dFlipToGdiSurface();///�������� ����� �������������� �������� � �.�.


MD3DERROR d3dCreateBackBuffer();
MD3DERROR d3dReleaseBackBuffer();
MD3DERROR d3dGetBackBufferFormat( DWORD *dwFormat );
MD3DERROR d3dLockBackBuffer( VOID **lplpSurface, DWORD *lpdwPitch );
MD3DERROR d3dUnlockBackBuffer();
MD3DERROR d3dFlushBackBuffer( RECT *lprcRect );
MD3DERROR d3dSetBackBufferColorKey( DWORD dwColor );
MD3DERROR d3dEnableBackBufferColorKey( BOOL bEnable );

MD3DERROR d3dScreenShot( VOID *lpBuffer, DWORD dwSize );
MD3DERROR d3dEndScene();
MD3DERROR d3dBeginScene();
MD3DERROR d3dTestCooperativeLevel();

#ifdef _PROFILE_D3D
void d3dGetTransferMemoryVideo(DWORD& byte_per_frame,
			DWORD& n256,DWORD& n128,DWORD& n64,DWORD& n32);
#endif // _PROFILE_D3D


MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE, DWORD);
MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState);
MD3DERROR d3dSetTextureStageState(DWORD dwStage,D3DTEXTURESTAGESTATETYPE dwState,DWORD dwValue);

MD3DERROR d3dTriangles(DWORD, LPVOID, DWORD);
MD3DERROR d3dTriangleStrip(DWORD, LPVOID, DWORD);
MD3DERROR d3dTriangleFan(DWORD, LPVOID, DWORD);
MD3DERROR d3dPoints(DWORD, LPVOID, DWORD);
MD3DERROR d3dTrianglesIndexed(DWORD, LPVOID, DWORD, LPWORD, DWORD);

MD3DERROR d3dGetTextureFormatData( DWORD, M3DTEXTUREFORMAT* );
MD3DERROR d3dCreateTexture(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat,DWORD* lpdwHandle);
MD3DERROR d3dDeleteTexture( DWORD );
MD3DERROR d3dLockTexture( DWORD dwHandle, VOID **lplpTexture, DWORD *lplpPitch );
MD3DERROR d3dLockTexture( DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom, VOID **lplpTexture, DWORD *lplpPitch );
MD3DERROR d3dUnlockTexture( DWORD );
MD3DERROR d3dSetTexture(DWORD hTexture,DWORD StageState=0);

MD3DERROR d3dSetTextureBlendMode( MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend );

MD3DERROR d3dCreateSprite( DWORD dwWidth, DWORD dwHeight, DWORD dwFormat, 
						   DWORD dwFlags, DWORD* lpdwHandle );
MD3DERROR d3dCreateChildSprite( DWORD dwParentHandle, DWORD dwLeft, DWORD dwTop, 
							    DWORD dwWidth, DWORD dwHeight, DWORD* lpdwHandle );
MD3DERROR d3dDeleteSprite( DWORD dwHandle );
MD3DERROR d3dLockSprite( DWORD dwHandle, VOID **lplpSprite, DWORD *lplpPitch );
MD3DERROR d3dUnlockSprite( DWORD dwHandle );
MD3DERROR d3dSetSpriteMode( DWORD dwHandle, DWORD dwMode, DWORD dwValue );
MD3DERROR d3dSetSpriteRect( DWORD dwHandle, D3DVALUE dvLeft, D3DVALUE dvTop, D3DVALUE dvRight, D3DVALUE dvBottom );
MD3DERROR d3dDrawSprite( DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, DWORD dwOrigin,
						 D3DVALUE dvScaleX, D3DVALUE dvScaleY, D3DVALUE dvRotate );
MD3DERROR d3dDrawSpriteZ( DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, D3DVALUE dvZ, 
						  DWORD dwOrigin, D3DVALUE dvScaleX, D3DVALUE dvScaleY, 
						  D3DVALUE dvRotate );

MD3DERROR d3dSetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma );
MD3DERROR d3dGetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma );
MD3DERROR d3dSetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight );
MD3DERROR d3dGetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight );
MD3DERROR d3dSetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow );
MD3DERROR d3dGetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow );

MD3DERROR d3dSetProjectionMatrix( FLOAT fFOV, FLOAT fNearPlane, FLOAT fFarPlane );
MD3DERROR d3dSetProjectionMatrixToIdentity();

MD3DERROR d3dSetFogParameters( DWORD dwMode, DWORD dwColor, FLOAT fStart, FLOAT fEnd, FLOAT fDensity );
MD3DERROR d3dEnableFog( BOOL bEnable );

MD3DERROR d3dSetFocusLossBehavior( BOOL bSleep );


// Constants ------------------------------------------------------ //


// Flags used for the d3dInit() function

#define MD3D_FULLSCREEN    0x00000001 // Use fullscreen mode
#define MD3D_ALTDEVICE	   0x00000010 // Use alternative device (add-in card)


// Back buffer formats

#define MD3DBBFORMAT_RGB565		1
#define MD3DBBFORMAT_RGB888		2
#define MD3DBBFORMAT_RGB555		3

// Texture formats

#define D3DTEXFMT_RGB565		1
#define D3DTEXFMT_RGB555		2
#define D3DTEXFMT_ARGB4444		3
#define D3DTEXFMT_ARGB1555		4
#define D3DTEXFMT_RGBA8888		5
#define D3DTEXFMT_PAL8			6

// Texture creation flags

#define MD3DTEX_DONOTPERSIST	0x00000001

// Sprite creation flags

#define MD3DSP_USEALPHATEST		0x00000001
#define MD3DSP_USEALPHABLEND	0x00000002

// Sprite modes

#define MD3DSP_ALPHATESTENABLE		1
#define MD3DSP_ALPHAREF				2
#define MD3DSP_ALPHABLENDENABLE		3
#define MD3DSP_COLORFACTOR			4
#define MD3DSP_ALPHAFACTOR			5

// Origins for d3dDrawSprite

#define MD3DORG_TOPLEFT			1
#define MD3DORG_CENTER			2


// Errors

#define MD3D_OK 0

// Generic (����� ������ ����)
#define MD3DERR_GENERIC				0x83000001	

#define MD3DERR_NODIRECTDRAW		0x83000002	// �� ������� ������� DirectDraw-������ (DX �� ���������� ��� ���������� �����)
#define MD3DERR_NODIRECT3D			0x83000003	// ���� 3D-������ ��� ��������
#define MD3DERR_ENUMERATIONFAILED	0x83000004	// �� ������� �������� ���������� � DirectDraw-��������� (DX �� ���������� ��� ���������� �����)
#define MD3DERR_NOCOMPATIBLEDEVICES 0x83000005	// �� ���� �� ��������� ������������� �� ������������� ����� �����������
#define MD3DERR_OUTOFMEMORY			0x83000006
#define MD3DERR_OUTOFVIDEOMEMORY	0x83000007	// ������������ �����������
#define MD3DERR_VMCACHEFULL			0x83000008	// Videomemory texture cache is full
#define MD3DERR_BADDISPLAYMODE		0x83000009	// ������� ����� ������� �� ��������� ��������� � ����
#define MD3DERR_INVALIDMODE			0x8300000a	// ��������� ����� ������� �� ��������������
#define MD3DERR_NOZBUFFER			0x8300000b	// �� ������� ������� Z-����� (�������� �� ������� �����������)
#define MD3DERR_NOTINITIALIZED		0x8300000c	// md3d �� ���������������
#define MD3DERR_ILLEGALCALL			0x8300000d	// ������� ������ �������� � ���� ���������
#define MD3DERR_NOEXCLUSIVEMODE		0x8300000e	// ���-�� ������ ������� exclusive-������ � ����������
#define MD3DERR_INVALIDPARAM		0x8300000f	// ������ � ���������� ������

#define MD3DERR_UNSUPPORTED			0x83000010	// ����������� �������� �� �������������� �������

// Debugging stuff -------------------------------------------------- //

// Call this before d3dInit to specify log file name
void d3dSetLogFile( TCHAR *szLogFile );

// Specify debug level (may be called many times to adjust 
// the level as needed)
void d3dSetDebugLevel( DWORD dwDebugLevel );

// Dump current render states 
void __d3dDumpAllRenderStates( DWORD dwDebugLevel, DWORD dwFlags, LPDIRECT3DDEVICE7 lpd3dDev );

// Flags for __d3dDumpAllRenderStates

#define MD3DRSDUMP_ALL				0xFFFFFFFF

#define MD3DRSDUMP_ALPHASTUFF		0x00000001
#define MD3DRSDUMP_TEXTURESTUFF		0x00000002
#define MD3DRSDUMP_MISC				0x00000004
#define MD3DRSDUMP_ZBUFFERSTUFF		0x00000008
#define MD3DRSDUMP_STENCILSTUFF		0x00000010
#define MD3DRSDUMP_FOGSTUFF			0x00000020
#define MD3DRSDUMP_STIPPLESTUFF		0x00000040


// Dump TSS state 
void __d3dDumpAllTSStates( DWORD dwDebugLevel, DWORD dwFlags, LPDIRECT3DDEVICE7 lpd3dDev );


// Output a debug message to the log when the current debug level
// if greater or equal to dwLevel
void __d3dLogMessage( DWORD dwLevel, TCHAR *szFormat, ... );

// Macros for debug logging
#ifdef _DEBUG
#define MD3DLOG(l,f) __d3dLogMessage(l,f)
#define MD3DLOG1(l,f,a1) __d3dLogMessage(l,f,a1)
#define MD3DLOG2(l,f,a1,a2) __d3dLogMessage(l,f,a1,a2)
#define MD3DLOG3(l,f,a1,a2,a3) __d3dLogMessage(l,f,a1,a2,a3)
#define MD3DLOG4(l,f,a1,a2,a3,a4) __d3dLogMessage(l,f,a1,a2,a3,a4)
#define MD3DLOG5(l,f,a1,a2,a3,a4,a5) __d3dLogMessage(l,f,a1,a2,a3,a4,a5)
#define MD3DLOG6(l,f,a1,a2,a3,a4,a5,a6) __d3dLogMessage(l,f,a1,a2,a3,a4,a5,a6)
#define MD3DLOG7(l,f,a1,a2,a3,a4,a5,a6,a7) __d3dLogMessage(l,f,a1,a2,a3,a4,a5,a6,a7)
#else
#define MD3DLOG(l,f)
#define MD3DLOG1(l,f,a1)
#define MD3DLOG2(l,f,a1,a2)
#define MD3DLOG3(l,f,a1,a2,a3)
#define MD3DLOG4(l,f,a1,a2,a3,a4)
#define MD3DLOG5(l,f,a1,a2,a3,a4,a5)
#define MD3DLOG6(l,f,a1,a2,a3,a4,a5,a6)
#define MD3DLOG7(l,f,a1,a2,a3,a4,a5,a6,a7)
#endif

TCHAR* __d3dBinaryDump( LPVOID, DWORD );

BOOL d3dIsActive();//������� �� ���������� � ������ ������

//������� ��������� ������
MD3DERROR d3dGetDisplayMode(DWORD& width,DWORD& height,DWORD& bpp);

/*
	�������, ����������� �������� �� BackBuffer
	� ������� ����������� �������

	GetBackBufferDC - �������� hdc � ������ ��������
	ReleaseBackBufferDC ������� hdc ���������� � ������� GetBackBufferDC 

	hdc �� ������ ����� ��������� �����������, ��� ���
	��� ��������� handle, ������� ����� ������ ���� ����.
*/
MD3DERROR GetBackBufferDC(HDC* hdc);
MD3DERROR ReleaseBackBufferDC(HDC hdc);

/*
	�������� ��� ������ � ���������������������
*/
//����� �� �������� ������������ ��������������������
BOOL IsMultiTextureSupport();

//dwVertexTypeDesc ����������� ������ ����� ������ ���� ����� ���������� ���������
//��� ������� �������� ���� � ������ IsMultiTextureSupport()==FALSE
MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, 
							  LPVOID lpvVertices, DWORD dwVertexCount, 
							  LPWORD lpwIndices, DWORD dwIndexCount,
							  DWORD dwHandleTex0,DWORD dwHandleTex1);
