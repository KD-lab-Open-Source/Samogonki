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

#include <algorithm>
#include <cstdint>

//#define _PROFILE_D3D

// Types ---------------------------------------------------------- //
enum D3DTEXTURESTAGESTATETYPE {
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
};

enum D3DRENDERSTATETYPE {
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
};

#define D3DRENDERSTATE_TEXTUREPERSPECTIVE       (D3DRENDERSTATETYPE)4
#define D3DRENDERSTATE_FILLMODE                 (D3DRENDERSTATETYPE)8
#define D3DRENDERSTATE_SHADEMODE                (D3DRENDERSTATETYPE)9
#define D3DRENDERSTATE_CULLMODE                 (D3DRENDERSTATETYPE)22
#define D3DRENDERSTATE_ZFUNC                    (D3DRENDERSTATETYPE)23
#define D3DRENDERSTATE_ALPHAREF                 (D3DRENDERSTATETYPE)24
#define D3DRENDERSTATE_ALPHAFUNC                (D3DRENDERSTATETYPE)25
#define D3DRENDERSTATE_SUBPIXEL             		(D3DRENDERSTATETYPE)31
#define D3DRENDERSTATE_ZBIAS                    (D3DRENDERSTATETYPE)47

enum D3DZBUFFERTYPE {
  D3DZB_FALSE                 = 0,
  D3DZB_TRUE                  = 1, // Z buffering
  D3DZB_USEW                  = 2, // W buffering
  D3DZB_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
};

struct DDGAMMARAMP {
  uint16_t red[256];
  uint16_t green[256];
  uint16_t blue[256];
};

enum D3DTEXTUREADDRESS {
  D3DTADDRESS_WRAP = 1,
  D3DTADDRESS_MIRROR = 2,
  D3DTADDRESS_CLAMP = 3,
  D3DTADDRESS_BORDER = 4,
  D3DTADDRESS_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DCULL {
  D3DCULL_NONE               = 1,
  D3DCULL_CW                 = 2,
  D3DCULL_CCW                = 3,
  D3DCULL_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
};

#define D3DFVF_XYZ              0x002
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_DIFFUSE          0x040
#define D3DFVF_SPECULAR         0x080
#define D3DFVF_TEX1             0x100

enum D3DBLEND {
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
    D3DBLEND_FORCE_DWORD       = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DTEXTUREOP {
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
};

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

enum D3DTEXTUREMINFILTER {
  D3DTFN_POINT        = 1,    // nearest
  D3DTFN_LINEAR       = 2,    // linear interpolation
  D3DTFN_ANISOTROPIC  = 3,    //
  D3DTFN_FORCE_DWORD  = 0x7fffffff,   // force 32-bit size enum
};

enum D3DTEXTUREMIPFILTER {
  D3DTFP_NONE         = 1,    // mipmapping disabled (use MAG filter)
  D3DTFP_POINT        = 2,    // nearest
  D3DTFP_LINEAR       = 3,    // linear interpolation
  D3DTFP_FORCE_DWORD  = 0x7fffffff,   // force 32-bit size enum
};

enum D3DANTIALIASMODE {
  D3DANTIALIAS_NONE          = 0,
  D3DANTIALIAS_SORTDEPENDENT = 1,
  D3DANTIALIAS_SORTINDEPENDENT = 2,
  D3DANTIALIAS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DSHADEMODE {
  D3DSHADE_FLAT              = 1,
  D3DSHADE_GOURAUD           = 2,
  D3DSHADE_PHONG             = 3,
  D3DSHADE_FORCE_DWORD       = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DFILLMODE {
  D3DFILL_POINT          = 1,
  D3DFILL_WIREFRAME          = 2,
  D3DFILL_SOLID          = 3,
  D3DFILL_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DCMPFUNC {
  D3DCMP_NEVER               = 1,
  D3DCMP_LESS                = 2,
  D3DCMP_EQUAL               = 3,
  D3DCMP_LESSEQUAL           = 4,
  D3DCMP_GREATER             = 5,
  D3DCMP_NOTEQUAL            = 6,
  D3DCMP_GREATEREQUAL        = 7,
  D3DCMP_ALWAYS              = 8,
  D3DCMP_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DFOGMODE {
  D3DFOG_NONE                = 0,
  D3DFOG_EXP                 = 1,
  D3DFOG_EXP2                = 2,
  D3DFOG_LINEAR              = 3,
  D3DFOG_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
};

// Values for material source
enum D3DMATERIALCOLORSOURCE {
  D3DMCS_MATERIAL = 0,                // Color from material is used
  D3DMCS_COLOR1   = 1,                // Diffuse vertex color is used
  D3DMCS_COLOR2   = 2,                // Specular vertex color is used
  D3DMCS_FORCE_DWORD = 0x7fffffff,    // force 32-bit size enum
};

// The D3DVERTEXBLENDFLAGS type is used with D3DRENDERSTATE_VERTEXBLEND state.
//
enum D3DVERTEXBLENDFLAGS {
  D3DVBLEND_DISABLE  = 0, // Disable vertex blending
  D3DVBLEND_1WEIGHT  = 1, // blend between 2 matrices
  D3DVBLEND_2WEIGHTS = 2, // blend between 3 matrices
  D3DVBLEND_3WEIGHTS = 3, // blend between 4 matrices
};

typedef uint32_t D3DCOLOR;

#define RGB_MAKE(r, g, b)       ((D3DCOLOR) (((r) << 16) | ((g) << 8) | (b)))
#define RGBA_MAKE(r, g, b, a)   ((D3DCOLOR) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

struct D3DMATRIX {
  float        _11, _12, _13, _14;
  float        _21, _22, _23, _24;
  float        _31, _32, _33, _34;
  float        _41, _42, _43, _44;

  bool operator==(const D3DMATRIX& other) const {
    return std::equal(&_11, &_11 + 16, &other._11);
  }
};

typedef uint32_t MD3DERROR;

struct M3DTEXTUREFORMAT {
  uint32_t dwTotalBitCount;
  bool bPalette8;

  uint32_t dwAlphaBitCount;
  uint32_t dwRBitCount;
  uint32_t dwGBitCount;
  uint32_t dwBBitCount;

  uint32_t dwAlphaBitMask;
  uint32_t dwRBitMask;
  uint32_t dwGBitMask;
  uint32_t dwBBitMask;

  uint32_t dwAlphaBitShift;
  uint32_t dwRBitShift;
  uint32_t dwGBitShift;
  uint32_t dwBBitShift;
};

struct MD3DCACHESLOTPROFILE {
  uint32_t dwFormat;
  uint32_t dwWidth, dwHeight;
  uint32_t dwFlags;				// Slot attributes
  uint32_t dwRequired;
  uint32_t dwDesired;
  uint32_t dwCreated;
};

struct MD3DRECT {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;

  bool operator==(const MD3DRECT& other) const {
    return std::equal(&left, &left + 4, &other.left);
  }
};

// For pixel format conversion

struct CVTPARAMS {
  uint32_t dwRBitMask;
  uint32_t dwRShiftL;
  uint32_t dwRShiftR;

  uint32_t dwGBitMask;
  uint32_t dwGShiftL;
  uint32_t dwGShiftR;

  uint32_t dwBBitMask;
  uint32_t dwBShiftL;
  uint32_t dwBShiftR;
};


// Texture blending modes
enum MD3DTEXTUREBLEND {
  MD3DTB_DIFFUSE = 1,
  MD3DTB_TEXTURE1 = 2, 
  MD3DTB_TEXTURE1_MOD_DIFFUSE = 3
};

// Gamma support modes

enum MD3DGAMMASUPPORT {
  MD3DGAMMA_RAMDACCALIBRATED = 1,
  MD3DGAMMA_RAMDAC = 2,
  MD3DGAMMA_EMULATION = 3
};


// Caps

enum MD3DCAPS {
  MD3DCAPS_WFOG = 1,
};


struct MD3DMODE {
  uint32_t dx, dy;
  uint32_t bitPerPixel;
};

// Function prototypes -------------------------------------------- //

//////array после использования необходимо уничтожать при помощи delete[] pArray;
MD3DERROR d3dEnumVideoMode(int* pNumVideoMode, MD3DMODE** ppArray);
MD3DERROR d3dGetAvailableVidMem (uint32_t* allVideoMem);

MD3DERROR d3dInit(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwBpp, uint32_t dwInitFlags, void* hInst, void* hIcon, char* szTitle);
MD3DERROR d3dReInit(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwBpp, uint32_t dwInitFlags, void* hInst, void* hIcon, char* szTitle);

MD3DERROR d3dClose();
MD3DERROR d3dQueryCaps(MD3DCAPS Caps, uint32_t *dwData);
MD3DERROR d3dGetWindowHandle(void *hWnd);
MD3DERROR d3dClear(uint32_t);
MD3DERROR d3dFlip(bool WaitVerticalBlank=true);
MD3DERROR d3dFlipToGdiSurface();///Вызывать перед использованием диалогов и т.д.


MD3DERROR d3dCreateBackBuffer();
MD3DERROR d3dReleaseBackBuffer();
MD3DERROR d3dGetBackBufferFormat(uint32_t *dwFormat);
MD3DERROR d3dLockBackBuffer(void **lplpSurface, uint32_t *lpdwPitch);
MD3DERROR d3dUnlockBackBuffer();
MD3DERROR d3dFlushBackBuffer(MD3DRECT* lprcRect);
MD3DERROR d3dSetBackBufferColorKey(uint32_t dwColor);
MD3DERROR d3dEnableBackBufferColorKey(bool bEnable);

MD3DERROR d3dScreenShot(void *lpBuffer, uint32_t dwSize);
MD3DERROR d3dEndScene();
MD3DERROR d3dBeginScene();
MD3DERROR d3dTestCooperativeLevel();
MD3DERROR d3dSetProjectionMatrix(const D3DMATRIX &matrix);
MD3DERROR d3dResetProjectionMatrix();

#ifdef _PROFILE_D3D
void d3dGetTransferMemoryVideo(DWORD& byte_per_frame, DWORD& n256,DWORD& n128,DWORD& n64,DWORD& n32);
#endif // _PROFILE_D3D


MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE, uint32_t);
MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t *lpdwRenderState);
MD3DERROR d3dSetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue);

MD3DERROR d3dTriangles(uint32_t, void*, uint32_t);
MD3DERROR d3dTriangleStrip(uint32_t, void *, uint32_t);
MD3DERROR d3dTriangleFan(uint32_t, void *, uint32_t);
MD3DERROR d3dPoints(uint32_t, void *, uint32_t);
MD3DERROR d3dTrianglesIndexed(uint32_t, void *, uint32_t, uint16_t *, uint32_t);

MD3DERROR d3dGetTextureFormatData(uint32_t, M3DTEXTUREFORMAT* );
MD3DERROR d3dCreateTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwFormat, uint32_t* lpdwHandle);
MD3DERROR d3dDeleteTexture(uint32_t);
MD3DERROR d3dLockTexture(uint32_t dwHandle, void **lplpTexture, uint32_t *lplpPitch );
MD3DERROR d3dLockTexture(uint32_t dwHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwRight, uint32_t dwBottom, void **lplpTexture, uint32_t *lplpPitch);
MD3DERROR d3dUnlockTexture(uint32_t);
MD3DERROR d3dSetTexture(uint32_t hTexture, uint32_t StageState = 0);

MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend);

MD3DERROR d3dCreateSprite(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwFormat, uint32_t dwFlags, uint32_t* lpdwHandle);
MD3DERROR d3dCreateChildSprite(uint32_t dwParentHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwWidth, uint32_t dwHeight, uint32_t* lpdwHandle);
MD3DERROR d3dDeleteSprite(uint32_t dwHandle);
MD3DERROR d3dLockSprite(uint32_t dwHandle, void **lplpSprite, uint32_t *lplpPitch);
MD3DERROR d3dUnlockSprite(uint32_t dwHandle);
MD3DERROR d3dSetSpriteMode(uint32_t dwHandle, uint32_t dwMode, uint32_t dwValue);
MD3DERROR d3dSetSpriteRect(uint32_t dwHandle, float dvLeft, float dvTop, float dvRight, float dvBottom);
MD3DERROR d3dDrawSprite(uint32_t dwHandle, float dvX, float dvY, uint32_t dwOrigin, float dvScaleX, float dvScaleY, float dvRotate);
MD3DERROR d3dDrawSpriteZ(uint32_t dwHandle, float dvX, float dvY, float dvZ, uint32_t dwOrigin, float dvScaleX, float dvScaleY, float dvRotate);

MD3DERROR d3dSetAdjustedGamma(float fRGamma, float fGGamma, float fBGamma);
MD3DERROR d3dGetAdjustedGamma(float *pfRGamma, float *pfGGamma, float *pfBGamma);
MD3DERROR d3dSetGammaFxHighlight(float fRHilight, float fGHilight, float fBHilight);
MD3DERROR d3dGetGammaFxHighlight(float *pfRHilight, float *pfGHilight, float *pfBHilight);
MD3DERROR d3dSetGammaFxShadow(float fRShadow, float fGShadow, float fBShadow);
MD3DERROR d3dGetGammaFxShadow(float *pfRShadow, float *pfGShadow, float *pfBShadow);

MD3DERROR d3dSetClipRect(const MD3DRECT &lprcClipRect);
MD3DERROR d3dResetClipRect();
MD3DERROR d3dSetProjectionMatrix(float fFOV, float fNearPlane, float fFarPlane);
MD3DERROR d3dSetProjectionMatrixToIdentity();

MD3DERROR d3dSetFogParameters(uint32_t dwMode, uint32_t dwColor, float fStart, float fEnd, float fDensity);
MD3DERROR d3dEnableFog(bool bEnable);

MD3DERROR d3dSetFocusLossBehavior(bool bSleep);


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

// Generic (облом общего вида)
#define MD3DERR_GENERIC				0x83000001	

#define MD3DERR_NODIRECTDRAW		0x83000002	// Не удалось создать DirectDraw-объект (DX не установлен или установлен криво)
#define MD3DERR_NODIRECT3D			0x83000003	// Нету 3D-железа или драйвера
#define MD3DERR_ENUMERATIONFAILED	0x83000004	// Не удалось получить информацию о DirectDraw-драйверах (DX не установлен или установлен криво)
#define MD3DERR_NOCOMPATIBLEDEVICES 0x83000005	// Ни один из найденных акселераторов не удовлетворяет нашим требованиям
#define MD3DERR_OUTOFMEMORY			0x83000006
#define MD3DERR_OUTOFVIDEOMEMORY	0x83000007	// Недостаточно видеопамяти
#define MD3DERR_VMCACHEFULL			0x83000008	// Videomemory texture cache is full
#define MD3DERR_BADDISPLAYMODE		0x83000009	// Текущий режим дисплея не позволяет рендерить в окно
#define MD3DERR_INVALIDMODE			0x8300000a	// Указанный режим дисплея не поддерживается
#define MD3DERR_NOZBUFFER			0x8300000b	// Не удалось создать Z-буфер (возможно не хватает видеопамяти)
#define MD3DERR_NOTINITIALIZED		0x8300000c	// md3d не инициализирован
#define MD3DERR_ILLEGALCALL			0x8300000d	// Функцию нельзя вызывать в этом контексте
#define MD3DERR_NOEXCLUSIVEMODE		0x8300000e	// Кто-то другой получил exclusive-доступ к устройству
#define MD3DERR_INVALIDPARAM		0x8300000f	// Ошибка в параметрах вызова

#define MD3DERR_UNSUPPORTED			0x83000010	// Запрошенная операция не поддерживается железом

// Debugging stuff -------------------------------------------------- //

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

bool d3dIsActive(); //Активно ли приложение в данный момент

//Текущие установки экрана
MD3DERROR d3dGetDisplayMode(uint32_t& width, uint32_t& height, uint32_t& bpp);

/*
  Операции для работы с мультитекстуированием
*/
//Может ли карточка поддерживать мультитекстуирование
bool IsMultiTextureSupport();

//dwVertexTypeDesc обязательно должен иметь хотябы один набор текстурных координат
//Эта функция работает даже в случае IsMultiTextureSupport()==FALSE
MD3DERROR d3dTrianglesIndexed2(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount, uint16_t* lpwIndices, uint32_t dwIndexCount, uint32_t dwHandleTex0, uint32_t dwHandleTex1);
