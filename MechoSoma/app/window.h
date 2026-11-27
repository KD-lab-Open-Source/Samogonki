#pragma once

#include <SDL3/SDL.h>

#include "render_backend.h"

namespace graphics {

// The game's window. The SDL GPU backend claims it for its device's swapchain.
class Window final {
 public:
  // A window of the game's screen size; setVideoMode() then picks windowed or full screen.
  Window(int width, int height);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = delete;

  // Sizes the window to the game's screen, or makes it cover the desktop. Returns how the
  // game's screen maps onto the window; the mouse code scales by it.
  RenderBackend::Scale setVideoMode(int width, int height, bool isFullScreen);

  // The window's size in pixels: on a HiDPI screen, larger than its size in points.
  int drawableWidth() const;
  int drawableHeight() const;

  SDL_Window* sdl() const { return _window; }

 private:
  SDL_Window* _window = nullptr;
};

}  // namespace graphics
