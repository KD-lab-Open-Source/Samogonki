// The sokol_gfx implementation, and the one place that picks its backend. Code that
// differs per backend (creating the context, the swapchain) lives in Display.
#if defined(EMSCRIPTEN)
#define SOKOL_GLES3
#else
#define SOKOL_GLCORE
#endif

#define SOKOL_GFX_IMPL
#include <sokol_gfx.h>
