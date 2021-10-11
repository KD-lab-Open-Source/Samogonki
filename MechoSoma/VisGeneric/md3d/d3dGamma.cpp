//*******************************************************************
//	Mechosoma D3D Wrapper API
//  Gamma support
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include <math.h>
#include <stdio.h>
#include "d3dutil.h"

// md3d globals

extern LPDIRECTDRAW7        g_pDD;              // The DirectDraw object
extern LPDIRECTDRAWSURFACE7 g_pddsFrontBuffer;  // The primary surface
extern LPDIRECT3DDEVICE7    g_pd3dDevice;       // The D3D device
extern BOOL					g_bInitialized;
extern RECT		g_rcScreenRect;					// Screen rect 

// Module globals

static MD3DGAMMASUPPORT m_gmGammaSupport=MD3DGAMMA_EMULATION;
static LPDIRECTDRAWGAMMACONTROL m_lpDDGamma=NULL;
static DDGAMMARAMP	m_aOriginalLUT;
static DDGAMMARAMP	m_aCurrentLUT;

static float	m_fRGamma;
static float	m_fGGamma;
static float	m_fBGamma;
static float	m_fRHilight;
static float	m_fGHilight;
static float	m_fBHilight;
static float	m_fRShadow;
static float	m_fGShadow;
static float	m_fBShadow;

// Prototypes

void __d3dCalcGammaFromRamp( float *lpfGamma, WORD *lpwRamp );
void __d3dUpdateGamma();
void __d3dRecalculateGamma();
void __d3dCalcRampData( WORD *lpwRamp, float fGamma, float fShadow, float fHilight );
inline float clampf( float x, float min, float max );


///////////////////////////////////////////////////////
inline float clampf( float x, float min, float max )
{
	if( x < min ) 
		x = min;
	else if( x > max ) 
		x = max;

	return x;
}


//*******************************************************************
// Init 
//*******************************************************************
MD3DERROR __d3dInitGammaSupport()
{
	HRESULT hr;

	_ASSERTE( FALSE == g_bInitialized );

	// Get HAL caps
	DDCAPS ddDriverCaps;
	ZeroMemory( &ddDriverCaps, sizeof(ddDriverCaps) );
	ddDriverCaps.dwSize = sizeof(ddDriverCaps);
	hr = g_pDD->GetCaps( &ddDriverCaps, NULL );
	_ASSERTE( SUCCEEDED( hr ) );

	// Assume no gamma HW
	m_gmGammaSupport = MD3DGAMMA_EMULATION;

	// Default gamma for emulation mode
	m_fRGamma = 1.0f;
	m_fGGamma = 1.0f;
	m_fBGamma = 1.0f;

	// Defaults for effects 

	m_fRHilight = 0.0;
	m_fGHilight = 0.0;
	m_fBHilight = 0.0;

	m_fRShadow = 0.0;
	m_fGShadow = 0.0;
	m_fBShadow = 0.0;

	if( ddDriverCaps.dwCaps2 & DDCAPS2_PRIMARYGAMMA ) {

		// Try to get GammaControl interface.
		hr = g_pddsFrontBuffer->QueryInterface(IID_IDirectDrawGammaControl, 
														(LPVOID *)&m_lpDDGamma);
		if( SUCCEEDED( hr ) ) {

			// We have hardware gamma support
			m_gmGammaSupport = MD3DGAMMA_RAMDAC;

			// Get a copy of the original GammaRamp to restore in the end 
			hr = m_lpDDGamma->GetGammaRamp( 0, &m_aOriginalLUT );
			_ASSERTE( SUCCEEDED( hr ) );

			// Get estimate gamma levels from the current ramp
			hr = m_lpDDGamma->GetGammaRamp( 0, &m_aCurrentLUT );
			_ASSERTE( SUCCEEDED( hr ) );
			__d3dCalcGammaFromRamp( &m_fRGamma, m_aCurrentLUT.red );
			__d3dCalcGammaFromRamp( &m_fGGamma, m_aCurrentLUT.green );
			__d3dCalcGammaFromRamp( &m_fBGamma, m_aCurrentLUT.blue );

			// See if calibration is supported
			if( ddDriverCaps.dwCaps2 & DDCAPS2_CANCALIBRATEGAMMA ) {
				m_gmGammaSupport = MD3DGAMMA_RAMDACCALIBRATED;
			}
		}
	}

	switch( m_gmGammaSupport ) {
	case MD3DGAMMA_RAMDACCALIBRATED:
		MD3DLOG( 1, "d3dGamma: Gamma Support: MD3DGAMMA_RAMDACCALIBRATED\n" );
		break;

	case MD3DGAMMA_RAMDAC:
		MD3DLOG( 1, "d3dGamma: Gamma Support: MD3DGAMMA_RAMDAC\n" );
		break;

	case MD3DGAMMA_EMULATION:
		MD3DLOG( 1, "d3dGamma: Gamma Support: MD3DGAMMA_EMULATION\n" );
		break;
	}

	return MD3D_OK;
}


//*******************************************************************
// De-init 
//*******************************************************************
MD3DERROR __d3dCloseGammaSupport()
{

	if(m_lpDDGamma)
	{
		HRESULT hr;
		hr = m_lpDDGamma->SetGammaRamp( 0, &m_aOriginalLUT );
//		_ASSERTE( SUCCEEDED( hr ) );
		m_lpDDGamma->Release();
		m_lpDDGamma=NULL;
	}

	return MD3D_OK;
}

//*******************************************************************
// ”становка user-adjustable гаммы
// работает дл€ всех трех режимов
// 0.2 <= fXGamma <= 5.0
//*******************************************************************
MD3DERROR d3dSetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma )
{
	_ASSERTE( 0.0 <= fRGamma && fRGamma <= 5.0 );
	_ASSERTE( 0.0 <= fGGamma && fGGamma <= 5.0 );
	_ASSERTE( 0.0 <= fBGamma && fBGamma <= 5.0 );

	m_fRGamma = fRGamma;
	m_fGGamma = fGGamma;
	m_fBGamma = fBGamma;

	__d3dRecalculateGamma();

	return MD3D_OK;
}


MD3DERROR d3dGetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma )
{
	*pfRGamma = m_fRGamma;
	*pfGGamma = m_fGGamma;
	*pfBGamma = m_fBGamma;

	return MD3D_OK;
}


//*******************************************************************
// Ёффект 'highlight'
// работает в режимах MD3DGAMMA_RAMDACCALIBRATED и MD3DGAMMA_RAMDAC
// -1 <= fXHilight <= 1 (0 = off)
//*******************************************************************
MD3DERROR d3dSetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight )
{
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );
	_ASSERTE( -1.0 <= fRHilight && fRHilight <= 1.0 );
	_ASSERTE( -1.0 <= fGHilight && fGHilight <= 1.0 );
	_ASSERTE( -1.0 <= fBHilight && fBHilight <= 1.0 );

	m_fRHilight = fRHilight;
	m_fGHilight = fGHilight;
	m_fBHilight = fBHilight;

	__d3dRecalculateGamma();

	return MD3D_OK;
}

MD3DERROR d3dGetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight )
{
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );

	*pfRHilight = m_fRHilight;
	*pfGHilight = m_fGHilight;
	*pfBHilight = m_fBHilight;

	return MD3D_OK;
}



//*******************************************************************
// Ёффект 'shadow'
// работает в режимах MD3DGAMMA_RAMDACCALIBRATED и MD3DGAMMA_RAMDAC
// -1.0 <= fxShadow <= 1.0 (0 = off)
//*******************************************************************
MD3DERROR d3dSetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow )
{
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );
	_ASSERTE( -1.0 <= fRShadow && fRShadow <= 1.0 );
	_ASSERTE( -1.0 <= fGShadow && fGShadow <= 1.0 );
	_ASSERTE( -1.0 <= fBShadow && fBShadow <= 1.0 );

	m_fRShadow = fRShadow;
	m_fGShadow = fGShadow;
	m_fBShadow = fBShadow;

	__d3dRecalculateGamma();

	return MD3D_OK;
}

MD3DERROR d3dGetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow )
{
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );

	*pfRShadow = m_fRShadow;
	*pfGShadow = m_fGShadow;
	*pfBShadow = m_fBShadow;

	return MD3D_OK;
}


//*******************************************************************
// Calculate gamma ramp by gamma value
//*******************************************************************
void __d3dCalcRampData( WORD *lpwRamp, float fGamma, float fShadow, float fHilight )
{

	float x1 = -fShadow/2;
	float x2 = 1.0f - fHilight/2.0f;
	float dx = x2-x1;
	if( 0.0 == dx ) dx = 0.0000001f;

	float oog = 1/fGamma;

	// Calculate the new ramp
	for( DWORD i = 0; i < 256; i++ ) {
		float x, y1, y2;

		x = float(i) / 256.0f;
		y1 = i?expf(logf(x) * oog):0;	// gamma correction
		y2 = 1/dx * y1 - x1/dx;		// effects

		lpwRamp[i] = short(clampf( y2*65536.0f, 0.0f, 65535.0f ));
	}
}

void __d3dRecalculateGamma()
{
	if(m_lpDDGamma==NULL)return;
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );

	// Calculate the ramps for red, green and blue
	__d3dCalcRampData( m_aCurrentLUT.red,   m_fRGamma, m_fRShadow, m_fRHilight );
	__d3dCalcRampData( m_aCurrentLUT.green, m_fGGamma, m_fGShadow, m_fGHilight );
	__d3dCalcRampData( m_aCurrentLUT.blue,  m_fBGamma, m_fBShadow, m_fBHilight );

	// Load gamma ramp
	HRESULT hr;
	hr = m_lpDDGamma->SetGammaRamp( 0, &m_aCurrentLUT );
	_ASSERTE( SUCCEEDED( hr ) );
}

void __d3dUpdateGamma()
{
	if(m_lpDDGamma==NULL)return;
	_ASSERTE( MD3DGAMMA_EMULATION != m_gmGammaSupport );

	// Load gamma ramp
	HRESULT hr;
	hr = m_lpDDGamma->SetGammaRamp( 0, &m_aCurrentLUT );
//	_ASSERTE( SUCCEEDED( hr ) );
}

//*******************************************************************
// Calculate an estimate gamma value based on RAMDAC ramp
//*******************************************************************
void __d3dCalcGammaFromRamp( float *lpfGamma, WORD *lpwRamp )
{
	float fGamma = 0.0f;
	DWORD dwCount = 0;
	for( DWORD i = 120; i < 136; i++ ) {
		if( lpwRamp[i] != 0 ) {
			fGamma += logf(float(i)/256.0f) / logf(float(lpwRamp[i])/65536.0f);
			dwCount++;
		}
	}

	*lpfGamma = fGamma / float(dwCount);
}
