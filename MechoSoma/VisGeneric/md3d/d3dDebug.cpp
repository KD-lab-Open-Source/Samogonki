//*******************************************************************
//	
//	Mechosoma D3D Wrapper API
//  -- debugging tools
//
//  K-D Lab::Spika
//
//*******************************************************************

#include "md3d.h"
#include <crtdbg.h>
#include <stdio.h>

extern LPDIRECT3DDEVICE7    g_pd3dDevice;

static DWORD		m_dwDebugLevel = 0;
static TCHAR*		m_szLogFile = NULL;
static FILE*		m_fd = NULL;

extern BOOL		g_bInitialized;


// Local prototypes

void DecodeTSSArg( LPSTR lpBuf, DWORD dwArg );


// Enum decode

#define DECODE16( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE8( dwCode, s9, s10, s11, s12, s13, s14, s15, s16 )

#define DECODE15( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE7( dwCode, s9, s10, s11, s12, s13, s14, s15 )

#define DECODE14( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE6( dwCode, s9, s10, s11, s12, s13, s14 )

#define DECODE13( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE5( dwCode, s9, s10, s11, s12, s13 )

#define DECODE12( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE4( dwCode, s9, s10, s11, s12 )

#define DECODE11( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE3( dwCode, s9, s10, s11 )

#define DECODE10( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE2( dwCode, s9, s10 )

#define DECODE9( dwCode, s1, s2, s3, s4, s5, s6, s7, s8, s9 ) \
	DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE1( dwCode, s9 )

#define DECODE8( dwCode, s1, s2, s3, s4, s5, s6, s7, s8 ) \
	DECODE4( dwCode, s1, s2, s3, s4 ) \
	DECODE4( dwCode, s5, s6, s7, s8 )

#define DECODE7( dwCode, s1, s2, s3, s4, s5, s6, s7 ) \
	DECODE4( dwCode, s1, s2, s3, s4 ) \
	DECODE3( dwCode, s5, s6, s7 )

#define DECODE6( dwCode, s1, s2, s3, s4, s5, s6 ) \
	DECODE4( dwCode, s1, s2, s3, s4 ) \
	DECODE2( dwCode, s5, s6 )

#define DECODE5( dwCode, s1, s2, s3, s4, s5 ) \
	DECODE4( dwCode, s1, s2, s3, s4 ) \
	DECODE1( dwCode, s5 )

#define DECODE4( dwCode, s1, s2, s3, s4 ) \
	DECODE2( dwCode, s1, s2 ) \
	DECODE2( dwCode, s3, s4 )

#define DECODE3( dwCode, s1, s2, s3 ) \
	DECODE2( dwCode, s1, s2 ) \
	DECODE1( dwCode, s3 )

#define DECODE2( dwCode, s1, s2 ) \
	DECODE_SINGLE( dwCode, s1 ) \
	DECODE_SINGLE( dwCode, s2 )

#define DECODE1( dwCode, s1 ) DECODE_SINGLE( dwCode, s1 )


// Enum decode

#define BEGIN_DECODE(s, var) \
	{ \
	char buf[120]; \
	sprintf( buf, "%-33s = ", #s ); \
	lpd3dDev->GetRenderState( s, &(var) ); \
	switch( var ) {

#define END_DECODE \
	default: \
	strcat( buf, "*UNKNOWN*" ); \
	} \
	strcat( buf, "\n" ); \
	MD3DLOG( dwDebugLevel, buf ); \
	}

#define DECODE_SINGLE( dwCode, a ) \
	case a: \
	strcat( buf, #a ); \
	break;

// Boolean

#define DECODE_BOOL( s, var ) \
    lpd3dDev->GetRenderState( s, &(var) ); \
	MD3DLOG2( dwDebugLevel, "%-33s = %s\n", #s, (var)? "TRUE" : "FALSE" );

// RGBA

#define DECODE_RGBA( s, var ) \
    lpd3dDev->GetRenderState( s, &(var) ); \
	MD3DLOG5( dwDebugLevel, "%-33s = R.G.B.A: %d.%d.%d.%d\n", #s, RGBA_GETRED(var), RGBA_GETGREEN(var), RGBA_GETBLUE(var), RGBA_GETALPHA(var) );

// Float

#define DECODE_FLOAT( s, var ) \
    lpd3dDev->GetRenderState( s, &(var) ); \
	MD3DLOG2( dwDebugLevel, "%-33s = %f\n", #s, (float)(var) );

// Decimal

#define DECODE_DECIMAL( s, var ) \
    lpd3dDev->GetRenderState( s, &(var) ); \
	MD3DLOG2( dwDebugLevel, "%-33s = %d\n", #s, (var) );

// Hex

#define DECODE_HEX( s, var ) \
    lpd3dDev->GetRenderState( s, &(var) ); \
	MD3DLOG2( dwDebugLevel, "%-33s = %#X\n", #s, (var) );

// Dump all renderstates

void __d3dDumpAllRenderStates( DWORD dwDebugLevel, DWORD dwFlags, LPDIRECT3DDEVICE7 lpd3dDev )
{

	lpd3dDev=g_pd3dDevice;
	if(dwFlags==0)
		dwFlags=0xFFFFFFFF;

	DWORD dwState;

	MD3DLOG( dwDebugLevel, "=== Dumping RenderStates ===\n" );


	if( dwFlags & MD3DRSDUMP_ALPHASTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Alpha Blend, Alpha Test, Color Key\n" );

		// D3DRENDERSTATE_ALPHABLENDENABLE
		DECODE_BOOL( D3DRENDERSTATE_ALPHABLENDENABLE, dwState )

		// D3DRENDERSTATE_SRCBLEND
		BEGIN_DECODE( D3DRENDERSTATE_SRCBLEND, dwState )
		DECODE13( dwState, D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT, D3DBLEND_BOTHSRCALPHA, D3DBLEND_BOTHINVSRCALPHA )
		END_DECODE

		// D3DRENDERSTATE_DESTBLEND
		BEGIN_DECODE( D3DRENDERSTATE_DESTBLEND, dwState )
		DECODE13( dwState, D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT, D3DBLEND_BOTHSRCALPHA, D3DBLEND_BOTHINVSRCALPHA )
		END_DECODE

		// D3DRENDERSTATE_ALPHATESTENABLE
		DECODE_BOOL( D3DRENDERSTATE_ALPHATESTENABLE, dwState )

		// D3DRENDERSTATE_ALPHAFUNC
		BEGIN_DECODE( D3DRENDERSTATE_ALPHAFUNC, dwState )
		DECODE8( dwState, D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL, D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS )
		END_DECODE

		// D3DRENDERSTATE_ALPHAREF
		DECODE_DECIMAL( D3DRENDERSTATE_ALPHAREF, dwState )

		// D3DRENDERSTATE_COLORKEYENABLE
		DECODE_BOOL( D3DRENDERSTATE_COLORKEYENABLE, dwState )
	}


	if( dwFlags & MD3DRSDUMP_TEXTURESTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Texture\n" );

		// D3DRENDERSTATE_TEXTUREFACTOR
		DECODE_RGBA( D3DRENDERSTATE_TEXTUREFACTOR, dwState )

		// D3DRENDERSTATE_BORDERCOLOR
		DECODE_RGBA( D3DRENDERSTATE_BORDERCOLOR, dwState )

		// D3DRENDERSTATE_TEXTUREPERSPECTIVE
		DECODE_BOOL( D3DRENDERSTATE_TEXTUREPERSPECTIVE, dwState )

		// D3DRENDERSTATE_MIPMAPLODBIAS
		DECODE_FLOAT( D3DRENDERSTATE_MIPMAPLODBIAS, dwState )

		MD3DLOG( dwDebugLevel, " (these are legacy states, but can be used for stage 0)\n" );

		// D3DRENDERSTATE_TEXTUREADDRESS
		BEGIN_DECODE( D3DRENDERSTATE_TEXTUREADDRESS, dwState )
		DECODE4( dwState, D3DTADDRESS_WRAP, D3DTADDRESS_MIRROR, D3DTADDRESS_CLAMP, D3DTADDRESS_BORDER )
		END_DECODE

		// D3DRENDERSTATE_TEXTUREADDRESSU
		BEGIN_DECODE( D3DRENDERSTATE_TEXTUREADDRESSU, dwState )
		DECODE4( dwState, D3DTADDRESS_WRAP, D3DTADDRESS_MIRROR, D3DTADDRESS_CLAMP, D3DTADDRESS_BORDER )
		END_DECODE

		// D3DRENDERSTATE_TEXTUREADDRESSV
		BEGIN_DECODE( D3DRENDERSTATE_TEXTUREADDRESSV, dwState )
		DECODE4( dwState, D3DTADDRESS_WRAP, D3DTADDRESS_MIRROR, D3DTADDRESS_CLAMP, D3DTADDRESS_BORDER )
		END_DECODE

		// D3DRENDERSTATE_TEXTUREMAG
		BEGIN_DECODE( D3DRENDERSTATE_TEXTUREMAG, dwState )
		DECODE6( dwState, D3DFILTER_NEAREST, D3DFILTER_LINEAR, D3DFILTER_MIPNEAREST, D3DFILTER_MIPLINEAR, D3DFILTER_LINEARMIPNEAREST, D3DFILTER_LINEARMIPLINEAR )
		END_DECODE

		// D3DRENDERSTATE_TEXTUREMIN
		BEGIN_DECODE( D3DRENDERSTATE_TEXTUREMIN, dwState )
		DECODE6( dwState, D3DFILTER_NEAREST, D3DFILTER_LINEAR, D3DFILTER_MIPNEAREST, D3DFILTER_MIPLINEAR, D3DFILTER_LINEARMIPNEAREST, D3DFILTER_LINEARMIPLINEAR )
		END_DECODE

		// D3DRENDERSTATE_ANISOTROPY
		DECODE_FLOAT( D3DRENDERSTATE_ANISOTROPY, dwState )
	}


	if( dwFlags & MD3DRSDUMP_MISC ) 
	{
		MD3DLOG( dwDebugLevel, "=== Misc.\n" );

		// D3DRENDERSTATE_CULLMODE
		BEGIN_DECODE( D3DRENDERSTATE_CULLMODE, dwState )
		DECODE3( dwState, D3DCULL_NONE, D3DCULL_CW, D3DCULL_CCW )
		END_DECODE

		// D3DRENDERSTATE_FILLMODE
		BEGIN_DECODE( D3DRENDERSTATE_FILLMODE, dwState )
		DECODE3( dwState, D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID )
		END_DECODE

		// D3DRENDERSTATE_SHADEMODE
		BEGIN_DECODE( D3DRENDERSTATE_SHADEMODE, dwState )
		DECODE3( dwState, D3DSHADE_FLAT, D3DSHADE_GOURAUD, D3DSHADE_PHONG )
		END_DECODE

		// D3DRENDERSTATE_LASTPIXEL
		DECODE_BOOL( D3DRENDERSTATE_LASTPIXEL, dwState )

		// D3DRENDERSTATE_EDGEANTIALIAS
		DECODE_BOOL( D3DRENDERSTATE_EDGEANTIALIAS, dwState )

		// D3DRENDERSTATE_ANTIALIAS
		BEGIN_DECODE( D3DRENDERSTATE_ANTIALIAS, dwState )
		DECODE3( dwState, D3DANTIALIAS_NONE, D3DANTIALIAS_SORTDEPENDENT, D3DANTIALIAS_SORTINDEPENDENT )
		END_DECODE

		// D3DRENDERSTATE_DITHERENABLE
		DECODE_BOOL( D3DRENDERSTATE_DITHERENABLE, dwState )

		// D3DRENDERSTATE_SPECULARENABLE
		DECODE_BOOL( D3DRENDERSTATE_SPECULARENABLE, dwState )

		// D3DRENDERSTATE_TRANSLUCENTSORTINDEPENDENT
		DECODE_BOOL( D3DRENDERSTATE_TRANSLUCENTSORTINDEPENDENT, dwState )

		// D3DRENDERSTATE_SUBPIXEL
		DECODE_BOOL( D3DRENDERSTATE_SUBPIXEL, dwState )

		// D3DRENDERSTATE_SUBPIXELX
		DECODE_BOOL( D3DRENDERSTATE_SUBPIXELX, dwState )
	}


	if( dwFlags & MD3DRSDUMP_ZBUFFERSTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Z buffer\n" );

		// D3DRENDERSTATE_ZENABLE
		BEGIN_DECODE( D3DRENDERSTATE_ZENABLE, dwState )
		DECODE3( dwState, D3DZB_FALSE, D3DZB_TRUE, D3DZB_USEW )
		END_DECODE

		// D3DRENDERSTATE_ZFUNC
		BEGIN_DECODE( D3DRENDERSTATE_ZFUNC, dwState )
		DECODE8( dwState, D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL, D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS )
		END_DECODE

		// D3DRENDERSTATE_ZWRITEENABLE
		DECODE_BOOL( D3DRENDERSTATE_ZWRITEENABLE, dwState )

		// D3DRENDERSTATE_ZBIAS
		DECODE_DECIMAL( D3DRENDERSTATE_ZBIAS, dwState )
	}


	if( dwFlags & MD3DRSDUMP_STENCILSTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Stencil\n" );

		// D3DRENDERSTATE_STENCILENABLE
		DECODE_BOOL( D3DRENDERSTATE_STENCILENABLE, dwState )

		// D3DRENDERSTATE_STENCILFAIL
		BEGIN_DECODE( D3DRENDERSTATE_STENCILFAIL, dwState )
		DECODE8( dwState, D3DSTENCILOP_KEEP, D3DSTENCILOP_ZERO, D3DSTENCILOP_REPLACE, D3DSTENCILOP_INCRSAT, D3DSTENCILOP_DECRSAT, D3DSTENCILOP_INVERT, D3DSTENCILOP_INCR, D3DSTENCILOP_DECR )
		END_DECODE

		// D3DRENDERSTATE_STENCILZFAIL
		BEGIN_DECODE( D3DRENDERSTATE_STENCILZFAIL, dwState )
		DECODE8( dwState, D3DSTENCILOP_KEEP, D3DSTENCILOP_ZERO, D3DSTENCILOP_REPLACE, D3DSTENCILOP_INCRSAT, D3DSTENCILOP_DECRSAT, D3DSTENCILOP_INVERT, D3DSTENCILOP_INCR, D3DSTENCILOP_DECR )
		END_DECODE

		// D3DRENDERSTATE_STENCILPASS
		BEGIN_DECODE( D3DRENDERSTATE_STENCILPASS, dwState )
		DECODE8( dwState, D3DSTENCILOP_KEEP, D3DSTENCILOP_ZERO, D3DSTENCILOP_REPLACE, D3DSTENCILOP_INCRSAT, D3DSTENCILOP_DECRSAT, D3DSTENCILOP_INVERT, D3DSTENCILOP_INCR, D3DSTENCILOP_DECR )
		END_DECODE

		// D3DRENDERSTATE_STENCILFUNC
		BEGIN_DECODE( D3DRENDERSTATE_STENCILFUNC, dwState )
		DECODE8( dwState, D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL, D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS )
		END_DECODE

		// D3DRENDERSTATE_STENCILREF
		DECODE_DECIMAL( D3DRENDERSTATE_STENCILREF, dwState )
    
		// D3DRENDERSTATE_STENCILMASK
		DECODE_HEX( D3DRENDERSTATE_STENCILMASK, dwState )

		// D3DRENDERSTATE_STENCILWRITEMASK
		DECODE_HEX( D3DRENDERSTATE_STENCILWRITEMASK, dwState )
	}


	if( dwFlags & MD3DRSDUMP_FOGSTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Fog\n" );

		// D3DRENDERSTATE_FOGENABLE
		DECODE_BOOL( D3DRENDERSTATE_FOGENABLE, dwState )

		// D3DRENDERSTATE_FOGCOLOR
		DECODE_RGBA( D3DRENDERSTATE_FOGCOLOR, dwState )

		// D3DRENDERSTATE_FOGTABLEMODE
		BEGIN_DECODE( D3DRENDERSTATE_FOGTABLEMODE, dwState )
		DECODE4( dwState, D3DFOG_NONE, D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR )
		END_DECODE

		// D3DRENDERSTATE_FOGTABLESTART
		DECODE_FLOAT( D3DRENDERSTATE_FOGTABLESTART, dwState )

		// D3DRENDERSTATE_FOGTABLEEND
		DECODE_FLOAT( D3DRENDERSTATE_FOGTABLEEND, dwState )

		// D3DRENDERSTATE_FOGTABLEDENSITY
		DECODE_FLOAT( D3DRENDERSTATE_FOGTABLEDENSITY, dwState )

		// D3DRENDERSTATE_RANGEFOGENABLE
		DECODE_BOOL( D3DRENDERSTATE_RANGEFOGENABLE, dwState )
	}


	if( dwFlags & MD3DRSDUMP_STIPPLESTUFF ) 
	{
		MD3DLOG( dwDebugLevel, "=== Stipple\n" );

		// D3DRENDERSTATE_STIPPLEDALPHA
		DECODE_BOOL( D3DRENDERSTATE_STIPPLEDALPHA, dwState )

		// D3DRENDERSTATE_STIPPLEENABLE
		DECODE_BOOL( D3DRENDERSTATE_STIPPLEENABLE, dwState )
	}

	MD3DLOG( dwDebugLevel, "============================\n" );
}


// TSS enum decode

#define TSS_BEGIN_DECODE(state, var) \
	{ \
	char buf[120]; \
	sprintf( buf, "%-33s = ", #state ); \
	switch( var ) {

#define TSS_END_DECODE \
	default: \
	strcat( buf, "*UNKNOWN*" ); \
	} \
	strcat( buf, "\n" ); \
	MD3DLOG( dwDebugLevel, buf ); \
	}


void __d3dDumpAllTSStates( DWORD dwDebugLevel, DWORD dwFlags, LPDIRECT3DDEVICE7 lpd3dDev )
{
	lpd3dDev=g_pd3dDevice;
	if(dwFlags==0)
		dwFlags=0xFFFFFFFF;

	DWORD dwState, i;
	char buf[120];

	MD3DLOG( dwDebugLevel, "=== Dumping TSS state ===\n" );

	for( i = 0; i < 8; i++ )
	{
		MD3DLOG1( dwDebugLevel, "--- Stage: %d ---\n", i );

		// D3DTSS_COLOROP

		lpd3dDev->GetTextureStageState( i, D3DTSS_COLOROP, &dwState ); 
		if( dwState < D3DTOP_PREMODULATE ) {
			TSS_BEGIN_DECODE( D3DTSS_COLOROP, dwState )
			DECODE16( dwState, D3DTOP_DISABLE, D3DTOP_SELECTARG1, D3DTOP_SELECTARG2, D3DTOP_MODULATE, D3DTOP_MODULATE2X, D3DTOP_MODULATE4X, D3DTOP_ADD, D3DTOP_ADDSIGNED, D3DTOP_ADDSIGNED2X, 
			 				   D3DTOP_SUBTRACT, D3DTOP_ADDSMOOTH, D3DTOP_BLENDDIFFUSEALPHA, D3DTOP_BLENDTEXTUREALPHA, D3DTOP_BLENDFACTORALPHA, D3DTOP_BLENDTEXTUREALPHAPM, D3DTOP_BLENDCURRENTALPHA )
			TSS_END_DECODE
		} else {
			TSS_BEGIN_DECODE( D3DTSS_COLOROP, dwState )
			DECODE8( dwState, D3DTOP_PREMODULATE, D3DTOP_MODULATEALPHA_ADDCOLOR, D3DTOP_MODULATECOLOR_ADDALPHA, D3DTOP_MODULATEINVALPHA_ADDCOLOR, D3DTOP_MODULATEINVCOLOR_ADDALPHA, D3DTOP_BUMPENVMAP, D3DTOP_BUMPENVMAPLUMINANCE, D3DTOP_DOTPRODUCT3 )
			TSS_END_DECODE
		} 
 
		// D3DTSS_COLORARG1
		lpd3dDev->GetTextureStageState( i, D3DTSS_COLORARG1, &(dwState) ); 
		DecodeTSSArg( buf, dwState );
		MD3DLOG2( dwDebugLevel, "%-33s = %s\n", "D3DTSS_COLORARG1", buf );

		// D3DTSS_COLORARG2
		lpd3dDev->GetTextureStageState( i, D3DTSS_COLORARG2, &(dwState) ); 
		DecodeTSSArg( buf, dwState );
		MD3DLOG2( dwDebugLevel, "%-33s = %s\n", "D3DTSS_COLORARG2", buf );

		// D3DTSS_ALPHAOP

		lpd3dDev->GetTextureStageState( i, D3DTSS_ALPHAOP, &dwState ); 
		if( dwState < D3DTOP_PREMODULATE ) {
			TSS_BEGIN_DECODE( D3DTSS_ALPHAOP, dwState )
			DECODE16( dwState, D3DTOP_DISABLE, D3DTOP_SELECTARG1, D3DTOP_SELECTARG2, D3DTOP_MODULATE, D3DTOP_MODULATE2X, D3DTOP_MODULATE4X, D3DTOP_ADD, D3DTOP_ADDSIGNED, D3DTOP_ADDSIGNED2X, 
			 				   D3DTOP_SUBTRACT, D3DTOP_ADDSMOOTH, D3DTOP_BLENDDIFFUSEALPHA, D3DTOP_BLENDTEXTUREALPHA, D3DTOP_BLENDFACTORALPHA, D3DTOP_BLENDTEXTUREALPHAPM, D3DTOP_BLENDCURRENTALPHA )
			TSS_END_DECODE
		} else {
			TSS_BEGIN_DECODE( D3DTSS_ALPHAOP, dwState )
			DECODE8( dwState, D3DTOP_PREMODULATE, D3DTOP_MODULATEALPHA_ADDCOLOR, D3DTOP_MODULATECOLOR_ADDALPHA, D3DTOP_MODULATEINVALPHA_ADDCOLOR, D3DTOP_MODULATEINVCOLOR_ADDALPHA, D3DTOP_BUMPENVMAP, D3DTOP_BUMPENVMAPLUMINANCE, D3DTOP_DOTPRODUCT3 )
			TSS_END_DECODE
		} 

		// D3DTSS_ALPHAARG1
		lpd3dDev->GetTextureStageState( i, D3DTSS_ALPHAARG1, &(dwState) ); 
		DecodeTSSArg( buf, dwState );
		MD3DLOG2( dwDebugLevel, "%-33s = %s\n", "D3DTSS_ALPHAARG1", buf );

		// D3DTSS_ALPHAARG2
		lpd3dDev->GetTextureStageState( i, D3DTSS_ALPHAARG2, &(dwState) ); 
		DecodeTSSArg( buf, dwState );
		MD3DLOG2( dwDebugLevel, "%-33s = %s\n", "D3DTSS_ALPHAARG2", buf );
	}

/*
    D3DTSS_BUMPENVMAT00
	D3DTSS_BUMPENVMAT01
    D3DTSS_BUMPENVMAT10
	D3DTSS_BUMPENVMAT11
    D3DTSS_TEXCOORDINDEX
	D3DTSS_ADDRESS
    D3DTSS_ADDRESSU
	D3DTSS_ADDRESSV
    D3DTSS_BORDERCOLOR
	D3DTSS_MAGFILTER
    D3DTSS_MINFILTER
	D3DTSS_MIPFILTER
    D3DTSS_MIPMAPLODBIAS
	D3DTSS_MAXMIPLEVEL
    D3DTSS_MAXANISOTROPY
	D3DTSS_BUMPENVLSCALE
    D3DTSS_BUMPENVLOFFSET
*/

}


void DecodeTSSArg( LPSTR lpBuf, DWORD dwArg ) 
{
	switch( dwArg & D3DTA_SELECTMASK )
	{
	case D3DTA_CURRENT:
		strcpy( lpBuf, "D3DTA_CURRENT" );
		break;

	case D3DTA_DIFFUSE:
		strcpy( lpBuf, "D3DTA_DIFFUSE" );
		break;

	case D3DTA_TEXTURE:
		strcpy( lpBuf, "D3DTA_TEXTURE" );
		break;

	case D3DTA_TFACTOR:
		strcpy( lpBuf, "D3DTA_TFACTOR" );
		break;

	default:
		strcpy( lpBuf, "*UNKNOWN*" );
		break;
	}

	if( dwArg & D3DTA_ALPHAREPLICATE )
		strcat( lpBuf, " | D3DTA_ALPHAREPLICATE" ); 

	if( dwArg & D3DTA_COMPLEMENT )
		strcat( lpBuf, " | D3DTA_COMPLEMENT" ); 
}


// Call this before d3dInit to specify log file name
void d3dSetLogFile( TCHAR *szLogFile )
{
	_ASSERTE( !g_bInitialized );

	m_szLogFile = szLogFile;
}


// Specify debug level (may be called many times to adjust 
// the level as needed)
void d3dSetDebugLevel( DWORD dwDebugLevel )
{
	_ASSERTE( dwDebugLevel >= 0 && dwDebugLevel <= 5 );
	m_dwDebugLevel = dwDebugLevel;
}


// Called on d3dInit
void __d3dStartLogging()
{
	if( m_szLogFile ) {
		m_fd = fopen( m_szLogFile, "wb");
		_ASSERTE( m_fd );
	}
}


// Called on d3dClose
void __d3dStopLogging()
{
	if( m_fd ) {
		fclose( m_fd );
		m_fd = NULL;
	}
}


// Used by the D3DLOGx macros
void __d3dLogMessage( DWORD dwLevel, TCHAR *szFormat, ... )
{
	va_list args;
	static char buf[200];

//	if( dwLevel > m_dwDebugLevel )return;
	
	va_start( args, szFormat );

	if( m_fd ) {
		fprintf( m_fd, "MD3D: " );
		vfprintf( m_fd, szFormat, args );
		fflush( m_fd );
	} else {
		vsprintf( &buf[0], szFormat, args );
		OutputDebugString( "MD3D: " );
		OutputDebugString( &buf[0] );
	}
}


CHAR* __d3dBinaryDump( LPVOID lpvData, DWORD dwByteCount )
{
	static char* lpBuffer = NULL;
	static DWORD dwBufferSize = 0;
	static char* lpHexDigits = "0123456789ABCDEF";

	if( NULL == lpBuffer ) {
		lpBuffer = (char*)malloc( dwByteCount*2+1 );
		_ASSERTE( lpBuffer );
		dwBufferSize = dwByteCount*2+1;
	}

	if( dwBufferSize < dwByteCount*2+1 ) {
		lpBuffer = (char*)realloc( lpBuffer, dwByteCount*2+1 );
		_ASSERTE( lpBuffer );
		dwBufferSize = dwByteCount*2+1;
	}

	char* lpSrc = (char*)lpvData;
	char* lpDest = lpBuffer;

	while( dwByteCount-- ) {
		*lpDest++ = lpHexDigits[*lpSrc & 0x0F];
		*lpDest++ = lpHexDigits[(*lpSrc & 0xF0) >> 4];
		lpSrc++;
	}
	*lpDest = 0;

	// &&&
	*lpBuffer = 0;

	return lpBuffer;
}


CHAR* __d3dGuidToStr( GUID* pGUID )
{
	static char szBuffer[40];

	if( pGUID )
		sprintf( szBuffer, "%08lX-%04hX-%04hX-%02hX%02hX-%02hX%02hX%02hX%02hX%02hX%02hX", 
			pGUID->Data1, pGUID->Data2, pGUID->Data3, 
			(WORD)pGUID->Data4[0], (WORD)pGUID->Data4[1], 
			(WORD)pGUID->Data4[2], (WORD)pGUID->Data4[3], 
			(WORD)pGUID->Data4[4], (WORD)pGUID->Data4[5], 
			(WORD)pGUID->Data4[6], (WORD)pGUID->Data4[7] ); 
	else
		sprintf( szBuffer, "null" ); 
	
	return szBuffer;
}



/*
_CRTIMP int __cdecl _CrtDbgReport(
        int,
        const char *,
        int,
        const char *,
        const char *,
...);
*/
