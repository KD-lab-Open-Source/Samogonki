#include "window.h"

#include "xerrhand.h"

using namespace graphics;

Window::Window(int width, int height, SDL_WindowFlags flags) {
  // A window is shown when created; on a HiDPI screen its back buffer has the screen's
  // pixels (drawableWidth/Height), while its size and the mouse stay in points.
  _window = SDL_CreateWindow("Moonshine Runners", width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | flags);
  if (_window == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow(): %s\n", SDL_GetError());
    ErrH.Abort("SDL_CreateWindow", XERR_USER, 0, "");
  }
  SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  // SDL 2 delivered text input (player names) from the start wherever there is no
  // on-screen keyboard; SDL 3 waits for this.
  if (!SDL_HasScreenKeyboardSupport()) {
    SDL_StartTextInput(_window);
  }
}

Window::~Window() {
  SDL_DestroyWindow(_window);
}

RenderBackend::Scale Window::setVideoMode(int width, int height, bool isFullScreen) {
  if (!isFullScreen) {
    SDL_SetWindowFullscreen(_window, false);
    SDL_SetWindowSize(_window, width, height);
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowCursor();
    return {1.0f, 1.0f};
  }

  // No exclusive mode: full screen covers the desktop of the display the window is on.
  SDL_SetWindowFullscreenMode(_window, nullptr);
  SDL_SetWindowFullscreen(_window, true);
  SDL_HideCursor();

  const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(_window));
  if (displayMode == nullptr || displayMode->w <= 0 || displayMode->h <= 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetCurrentDisplayMode(): %s\n", SDL_GetError());
    return {1.0f, 1.0f};
  }
  return {1.0f * width / displayMode->w, 1.0f * height / displayMode->h};
}

int Window::drawableWidth() const {
  int width = 0;
  SDL_GetWindowSizeInPixels(_window, &width, nullptr);
  return width;
}

int Window::drawableHeight() const {
  int height = 0;
  SDL_GetWindowSizeInPixels(_window, nullptr, &height);
  return height;
}
