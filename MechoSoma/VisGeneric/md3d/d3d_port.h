#ifndef _D3D_PORT_H
#define _D3D_PORT_H

#include <cstdint>

#define VOID void

#ifndef OBJC_BOOL_DEFINED
typedef int BOOL;
#endif

typedef unsigned char BYTE;
typedef uint32_t DWORD;
typedef float FLOAT;
typedef int32_t LONG;
typedef char TCHAR;
typedef unsigned short WORD;

typedef void *LPVOID;
typedef WORD *LPWORD;
typedef void *PVOID;

typedef PVOID HANDLE;
typedef LONG HRESULT;

typedef HANDLE HDC;
typedef HANDLE HICON;
typedef HANDLE HINSTANCE;
typedef HANDLE HWND;

#define TRUE 1
#define FALSE 0

typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef struct tagPALETTEENTRY {
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;

typedef enum _D3DRENDERSTATETYPE
{
	D3DRENDERSTATE_ANTIALIAS          = 2,    /* D3DANTIALIASMODE */
	D3DRENDERSTATE_ZENABLE            = 7,		/* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	D3DRENDERSTATE_ZWRITEENABLE       = 14,		/* TRUE to enable z writes */
	D3DRENDERSTATE_ALPHATESTENABLE    = 15,		/* TRUE to enable alpha tests */
	D3DRENDERSTATE_LASTPIXEL          = 16,   /* TRUE for last-pixel on lines */
	D3DRENDERSTATE_SRCBLEND           = 19,		/* D3DBLEND */
	D3DRENDERSTATE_DESTBLEND          = 20,		/* D3DBLEND */
	D3DRENDERSTATE_DITHERENABLE       = 26,		/* TRUE to enable dithering */
	D3DRENDERSTATE_ALPHABLENDENABLE   = 27,		/* TRUE to enable alpha blending */
	D3DRENDERSTATE_FOGENABLE          = 28,		/* TRUE to enable fog blending */
	D3DRENDERSTATE_SPECULARENABLE     = 29,		/* TRUE to enable specular */
	D3DRENDERSTATE_FOGCOLOR           = 34,		/* D3DCOLOR */
	D3DRENDERSTATE_FOGTABLEMODE       = 35,		/* D3DFOGMODE */
	D3DRENDERSTATE_FOGSTART           = 36,		/* Fog start (for both vertex and pixel fog) */
    D3DRENDERSTATE_FOGEND             = 37,		/* Fog end      */
    D3DRENDERSTATE_FOGDENSITY         = 38,		/* Fog density  */
    D3DRENDERSTATE_COLORKEYENABLE     = 41,   /* TRUE to enable source colorkeyed textures */
    D3DRENDERSTATE_RANGEFOGENABLE     = 48,   /* Enables range-based fog */
    D3DRENDERSTATE_TEXTUREFACTOR      = 60,   /* D3DCOLOR used for multi-texture blend */
    D3DRENDERSTATE_CLIPPING            = 136,
    D3DRENDERSTATE_LIGHTING            = 137,
    D3DRENDERSTATE_AMBIENT             = 139,
    D3DRENDERSTATE_FOGVERTEXMODE       = 140,
    D3DRENDERSTATE_COLORVERTEX         = 141,
    D3DRENDERSTATE_COLORKEYBLENDENABLE = 144,
    D3DRENDERSTATE_DIFFUSEMATERIALSOURCE    = 145,
	D3DRENDERSTATE_SPECULARMATERIALSOURCE   = 146,
    D3DRENDERSTATE_AMBIENTMATERIALSOURCE    = 147,
    D3DRENDERSTATE_EMISSIVEMATERIALSOURCE   = 148,
	D3DRENDERSTATE_VERTEXBLEND              = 151,
    D3DRENDERSTATE_CLIPPLANEENABLE          = 152,

    D3DRENDERSTATE_FOGTABLESTART      = 36,   /* Fog table start    */
    D3DRENDERSTATE_FOGTABLEEND        = 37,   /* Fog table end      */
    D3DRENDERSTATE_FOGTABLEDENSITY    = 38,   /* Fog table density  */

} D3DRENDERSTATETYPE;

#define D3DRENDERSTATE_TEXTUREPERSPECTIVE       (D3DRENDERSTATETYPE)4
#define D3DRENDERSTATE_FILLMODE                 (D3DRENDERSTATETYPE)8
#define D3DRENDERSTATE_SHADEMODE                (D3DRENDERSTATETYPE)9
#define D3DRENDERSTATE_CULLMODE                 (D3DRENDERSTATETYPE)22
#define D3DRENDERSTATE_ZFUNC                    (D3DRENDERSTATETYPE)23
#define D3DRENDERSTATE_ALPHAREF                 (D3DRENDERSTATETYPE)24
#define D3DRENDERSTATE_ALPHAFUNC                (D3DRENDERSTATETYPE)25
#define D3DRENDERSTATE_SUBPIXEL          		(D3DRENDERSTATETYPE)31
#define D3DRENDERSTATE_ZBIAS                    (D3DRENDERSTATETYPE)47

typedef enum _D3DBLEND {
    D3DBLEND_ZERO              = 1,
    D3DBLEND_ONE               = 2,
    D3DBLEND_SRCCOLOR          = 3,
    D3DBLEND_INVSRCCOLOR       = 4,
    D3DBLEND_SRCALPHA          = 5,
    D3DBLEND_INVSRCALPHA       = 6,
    D3DBLEND_DESTALPHA         = 7,
    D3DBLEND_INVDESTALPHA      = 8,
    D3DBLEND_DESTCOLOR         = 9,
    D3DBLEND_INVDESTCOLOR      = 10,
    D3DBLEND_SRCALPHASAT       = 11,
    D3DBLEND_BOTHSRCALPHA      = 12,
    D3DBLEND_BOTHINVSRCALPHA   = 13,
#if(DIRECT3D_VERSION >= 0x0500)
    D3DBLEND_FORCE_DWORD       = 0x7fffffff, /* force 32-bit size enum */
#endif /* DIRECT3D_VERSION >= 0x0500 */
} D3DBLEND;

typedef enum _D3DZBUFFERTYPE {
    D3DZB_FALSE                 = 0,
    D3DZB_TRUE                  = 1, // Z buffering
    D3DZB_USEW                  = 2, // W buffering
    D3DZB_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
} D3DZBUFFERTYPE;

typedef float D3DVALUE, *LPD3DVALUE;

typedef struct IDirect3DDevice3 *LPDIRECT3DDEVICE3;
typedef struct IDirect3DDevice7 *LPDIRECT3DDEVICE7;

/*
 * DDGAMMARAMP
 */
typedef struct _DDGAMMARAMP
{
    WORD                red[256];
    WORD                green[256];
    WORD                blue[256];
} DDGAMMARAMP;
typedef DDGAMMARAMP *LPDDGAMMARAMP;

typedef enum _D3DTEXTUREADDRESS {
    D3DTADDRESS_WRAP = 1,
    D3DTADDRESS_MIRROR = 2,
    D3DTADDRESS_CLAMP = 3,
    D3DTADDRESS_BORDER = 4,
    D3DTADDRESS_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} D3DTEXTUREADDRESS;

#define D3DFVF_XYZ              0x002
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_DIFFUSE          0x040
#define D3DFVF_SPECULAR         0x080
#define D3DFVF_TEX1             0x100

#ifndef D3DCOLOR_DEFINED
typedef DWORD D3DCOLOR;
#define D3DCOLOR_DEFINED
#endif
typedef DWORD *LPD3DCOLOR;

#define RGB_MAKE(r, g, b)       ((D3DCOLOR) (((r) << 16) | ((g) << 8) | (b)))
#define RGBA_MAKE(r, g, b, a)   ((D3DCOLOR) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

typedef enum _D3DCULL {
    D3DCULL_NONE               = 1,
    D3DCULL_CW                 = 2,
    D3DCULL_CCW                = 3,
    D3DCULL_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} D3DCULL;

typedef enum _D3DTEXTURESTAGESTATETYPE
{
    D3DTSS_COLOROP        =  1, /* D3DTEXTUREOP - per-stage blending controls for color channels */
    D3DTSS_COLORARG1      =  2, /* D3DTA_* (texture arg) */
    D3DTSS_COLORARG2      =  3, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAOP        =  4, /* D3DTEXTUREOP - per-stage blending controls for alpha channel */
    D3DTSS_ALPHAARG1      =  5, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAARG2      =  6, /* D3DTA_* (texture arg) */
    D3DTSS_BUMPENVMAT00   =  7, /* D3DVALUE (bump mapping matrix) */
    D3DTSS_BUMPENVMAT01   =  8, /* D3DVALUE (bump mapping matrix) */
    D3DTSS_BUMPENVMAT10   =  9, /* D3DVALUE (bump mapping matrix) */
    D3DTSS_BUMPENVMAT11   = 10, /* D3DVALUE (bump mapping matrix) */
    D3DTSS_TEXCOORDINDEX  = 11, /* identifies which set of texture coordinates index this texture */
    D3DTSS_ADDRESS        = 12, /* D3DTEXTUREADDRESS for both coordinates */
    D3DTSS_ADDRESSU       = 13, /* D3DTEXTUREADDRESS for U coordinate */
    D3DTSS_ADDRESSV       = 14, /* D3DTEXTUREADDRESS for V coordinate */
    D3DTSS_BORDERCOLOR    = 15, /* D3DCOLOR */
    D3DTSS_MAGFILTER      = 16, /* D3DTEXTUREMAGFILTER filter to use for magnification */
    D3DTSS_MINFILTER      = 17, /* D3DTEXTUREMINFILTER filter to use for minification */
    D3DTSS_MIPFILTER      = 18, /* D3DTEXTUREMIPFILTER filter to use between mipmaps during minification */
    D3DTSS_MIPMAPLODBIAS  = 19, /* D3DVALUE Mipmap LOD bias */
    D3DTSS_MAXMIPLEVEL    = 20, /* DWORD 0..(n-1) LOD index of largest map to use (0 == largest) */
    D3DTSS_MAXANISOTROPY  = 21, /* DWORD maximum anisotropy */
    D3DTSS_BUMPENVLSCALE  = 22, /* D3DVALUE scale for bump map luminance */
    D3DTSS_BUMPENVLOFFSET = 23, /* D3DVALUE offset for bump map luminance */
    D3DTSS_TEXTURETRANSFORMFLAGS = 24, /* D3DTEXTURETRANSFORMFLAGS controls texture transform */
    D3DTSS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} D3DTEXTURESTAGESTATETYPE;

typedef enum _D3DTEXTUREOP
{
// Control
    D3DTOP_DISABLE    = 1,      // disables stage
    D3DTOP_SELECTARG1 = 2,      // the default
    D3DTOP_SELECTARG2 = 3,

// Modulate
    D3DTOP_MODULATE   = 4,      // multiply args together
    D3DTOP_MODULATE2X = 5,      // multiply and  1 bit
    D3DTOP_MODULATE4X = 6,      // multiply and  2 bits

// Add
    D3DTOP_ADD          =  7,   // add arguments together
    D3DTOP_ADDSIGNED    =  8,   // add with -0.5 bias
    D3DTOP_ADDSIGNED2X  =  9,   // as above but left  1 bit
    D3DTOP_SUBTRACT     = 10,   // Arg1 - Arg2, with no saturation
    D3DTOP_ADDSMOOTH    = 11,   // add 2 args, subtract product
                                // Arg1 + Arg2 - Arg1*Arg2
                                // = Arg1 + (1-Arg1)*Arg2

// Linear alpha blend: Arg1*(Alpha) + Arg2*(1-Alpha)
    D3DTOP_BLENDDIFFUSEALPHA    = 12, // iterated alpha
    D3DTOP_BLENDTEXTUREALPHA    = 13, // texture alpha
    D3DTOP_BLENDFACTORALPHA     = 14, // alpha from D3DRENDERSTATE_TEXTUREFACTOR
    // Linear alpha blend with pre-multiplied arg1 input: Arg1 + Arg2*(1-Alpha)
    D3DTOP_BLENDTEXTUREALPHAPM  = 15, // texture alpha
    D3DTOP_BLENDCURRENTALPHA    = 16, // by alpha of current color

// Specular mapping
    D3DTOP_PREMODULATE            = 17,     // modulate with next texture before use
    D3DTOP_MODULATEALPHA_ADDCOLOR = 18,     // Arg1.RGB + Arg1.A*Arg2.RGB
                                            // COLOROP only
    D3DTOP_MODULATECOLOR_ADDALPHA = 19,     // Arg1.RGB*Arg2.RGB + Arg1.A
                                            // COLOROP only
    D3DTOP_MODULATEINVALPHA_ADDCOLOR = 20,  // (1-Arg1.A)*Arg2.RGB + Arg1.RGB
                                            // COLOROP only
    D3DTOP_MODULATEINVCOLOR_ADDALPHA = 21,  // (1-Arg1.RGB)*Arg2.RGB + Arg1.A
                                            // COLOROP only

// Bump mapping
    D3DTOP_BUMPENVMAP           = 22, // per pixel env map perturbation
    D3DTOP_BUMPENVMAPLUMINANCE  = 23, // with luminance channel
    // This can do either diffuse or specular bump mapping with correct input.
    // Performs the function (Arg1.R*Arg2.R + Arg1.G*Arg2.G + Arg1.B*Arg2.B)
    // where each component has been scaled and offset to make it signed.
    // The result is replicated into all four (including alpha) channels.
    // This is a valid COLOROP only.
    D3DTOP_DOTPRODUCT3          = 24,

    D3DTOP_FORCE_DWORD = 0x7fffffff,
} D3DTEXTUREOP;

/*
 * Values for COLORARG1,2 and ALPHAARG1,2 texture blending operations
 * set in texture processing stage controls in D3DRENDERSTATE.
 */
#define D3DTA_SELECTMASK        0x0000000f  // mask for arg selector
#define D3DTA_DIFFUSE           0x00000000  // select diffuse color
#define D3DTA_CURRENT           0x00000001  // select result of previous stage
#define D3DTA_TEXTURE           0x00000002  // select texture color
#define D3DTA_TFACTOR           0x00000003  // select RENDERSTATE_TEXTUREFACTOR
#define D3DTA_SPECULAR          0x00000004  // select specular color
#define D3DTA_COMPLEMENT        0x00000010  // take 1.0 - x
#define D3DTA_ALPHAREPLICATE    0x00000020  // replicate alpha to color components

typedef enum _D3DTEXTUREMINFILTER
{
    D3DTFN_POINT        = 1,    // nearest
    D3DTFN_LINEAR       = 2,    // linear interpolation
    D3DTFN_ANISOTROPIC  = 3,    //
    D3DTFN_FORCE_DWORD  = 0x7fffffff,   // force 32-bit size enum
} D3DTEXTUREMINFILTER;

typedef enum _D3DTEXTUREMIPFILTER
{
    D3DTFP_NONE         = 1,    // mipmapping disabled (use MAG filter)
    D3DTFP_POINT        = 2,    // nearest
    D3DTFP_LINEAR       = 3,    // linear interpolation
    D3DTFP_FORCE_DWORD  = 0x7fffffff,   // force 32-bit size enum
} D3DTEXTUREMIPFILTER;

typedef enum _D3DANTIALIASMODE {
    D3DANTIALIAS_NONE          = 0,
    D3DANTIALIAS_SORTDEPENDENT = 1,
    D3DANTIALIAS_SORTINDEPENDENT = 2,
    D3DANTIALIAS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} D3DANTIALIASMODE;

typedef enum _D3DSHADEMODE {
    D3DSHADE_FLAT              = 1,
    D3DSHADE_GOURAUD           = 2,
    D3DSHADE_PHONG             = 3,
    D3DSHADE_FORCE_DWORD       = 0x7fffffff, /* force 32-bit size enum */
} D3DSHADEMODE;

typedef enum _D3DFILLMODE {
    D3DFILL_POINT          = 1,
    D3DFILL_WIREFRAME          = 2,
    D3DFILL_SOLID          = 3,
    D3DFILL_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} D3DFILLMODE;

typedef enum _D3DCMPFUNC {
    D3DCMP_NEVER               = 1,
    D3DCMP_LESS                = 2,
    D3DCMP_EQUAL               = 3,
    D3DCMP_LESSEQUAL           = 4,
    D3DCMP_GREATER             = 5,
    D3DCMP_NOTEQUAL            = 6,
    D3DCMP_GREATEREQUAL        = 7,
    D3DCMP_ALWAYS              = 8,
    D3DCMP_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
} D3DCMPFUNC;

typedef enum _D3DFOGMODE {
    D3DFOG_NONE                = 0,
    D3DFOG_EXP                 = 1,
    D3DFOG_EXP2                = 2,
    D3DFOG_LINEAR              = 3,
    D3DFOG_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
} D3DFOGMODE;

// Values for material source
typedef enum _D3DMATERIALCOLORSOURCE
{
    D3DMCS_MATERIAL = 0,                // Color from material is used
    D3DMCS_COLOR1   = 1,                // Diffuse vertex color is used
    D3DMCS_COLOR2   = 2,                // Specular vertex color is used
    D3DMCS_FORCE_DWORD = 0x7fffffff,    // force 32-bit size enum
} D3DMATERIALCOLORSOURCE;

// The D3DVERTEXBLENDFLAGS type is used with D3DRENDERSTATE_VERTEXBLEND state.
//
typedef enum _D3DVERTEXBLENDFLAGS
{
    D3DVBLEND_DISABLE  = 0, // Disable vertex blending
    D3DVBLEND_1WEIGHT  = 1, // blend between 2 matrices
    D3DVBLEND_2WEIGHTS = 2, // blend between 3 matrices
    D3DVBLEND_3WEIGHTS = 3, // blend between 4 matrices
} D3DVERTEXBLENDFLAGS;

#endif