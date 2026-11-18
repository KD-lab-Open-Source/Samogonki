#include "display.h"

#include "xerrhand.h"

using namespace graphics;

namespace {
  constexpr sg_pixel_format color_format = SG_PIXELFORMAT_RGBA8;
  constexpr sg_pixel_format depth_format = SG_PIXELFORMAT_DEPTH_STENCIL;

  // Logs the context the driver actually created, for bug reports. glGetString is looked
  // up through SDL because the Windows build does not link opengl32.
  void log_context() {
#if defined(_WIN32) && !defined(_WIN64)
    using GetString = const unsigned char*(__stdcall*)(unsigned int);
#else
    using GetString = const unsigned char* (*)(unsigned int);
#endif
    const auto get_string = reinterpret_cast<GetString>(SDL_GL_GetProcAddress("glGetString"));
    if (get_string == nullptr) {
      return;
    }
    constexpr unsigned int gl_renderer = 0x1F01;
    constexpr unsigned int gl_version = 0x1F02;
    const auto renderer = get_string(gl_renderer);
    const auto version = get_string(gl_version);
    SDL_Log("Display: OpenGL %s, %s", version ? reinterpret_cast<const char*>(version) : "?",
            renderer ? reinterpret_cast<const char*>(renderer) : "?");
  }
}

Display::Display(int width, int height) {
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#ifdef EMSCRIPTEN
  // WebGL 2, for sokol's GLES3 backend.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
  // GL 4.1 core, for sokol's GLCORE backend: the shaders are compiled for GLSL 4.10,
  // and 4.1 is also the newest version macOS offers.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

  // A window is shown when created; on a HiDPI screen its back buffer has the screen's
  // pixels (drawableWidth/Height), while its size and the mouse stay in points.
  _window = SDL_CreateWindow("Moonshine Runners", width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL);
  if (_window == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow(): %s\n", SDL_GetError());
    ErrH.Abort("SDL_CreateWindow", XERR_USER, 0, "");
  }
  SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  _context = SDL_GL_CreateContext(_window);
  if (_context == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): %s\n", SDL_GetError());
    ErrH.Abort("SDL_GL_CreateContext", XERR_USER, 0, "");
  }
  log_context();

  SDL_GL_SetSwapInterval(1);

  // SDL 2 delivered text input (player names) from the start wherever there is no
  // on-screen keyboard; SDL 3 waits for this.
  if (!SDL_HasScreenKeyboardSupport()) {
    SDL_StartTextInput(_window);
  }
}

Display::~Display() {
  SDL_GL_DestroyContext(_context);
  SDL_DestroyWindow(_window);
}

Display::Scale Display::setVideoMode(int width, int height, bool isFullScreen) {
  if (!isFullScreen) {
    SDL_SetWindowFullscreen(_window, false);
    SDL_SetWindowSize(_window, width, height);
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowCursor();
    return Scale{1.0f, 1.0f};
  }

  // No exclusive mode: full screen covers the desktop of the display the window is on.
  SDL_SetWindowFullscreenMode(_window, nullptr);
  SDL_SetWindowFullscreen(_window, true);
  SDL_HideCursor();

  const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(_window));
  if (displayMode == nullptr || displayMode->w <= 0 || displayMode->h <= 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetCurrentDisplayMode(): %s\n", SDL_GetError());
    return Scale{1.0f, 1.0f};
  }
  return Scale{1.0f * width / displayMode->w, 1.0f * height / displayMode->h};
}

int Display::drawableWidth() const {
  int width = 0;
  SDL_GetWindowSizeInPixels(_window, &width, nullptr);
  return width;
}

int Display::drawableHeight() const {
  int height = 0;
  SDL_GetWindowSizeInPixels(_window, nullptr, &height);
  return height;
}

sg_environment Display::environment() const {
  sg_environment environment{};
  environment.defaults.color_format = color_format;
  environment.defaults.depth_format = depth_format;
  environment.defaults.sample_count = 1;
  return environment;
}

sg_swapchain Display::swapchain() const {
  sg_swapchain swapchain{};
  SDL_GetWindowSizeInPixels(_window, &swapchain.width, &swapchain.height);
  swapchain.sample_count = 1;
  swapchain.color_format = color_format;
  swapchain.depth_format = depth_format;
  swapchain.gl.framebuffer = 0;  // the window's own framebuffer
  return swapchain;
}

void Display::present() {
  SDL_GL_SwapWindow(_window);
}
