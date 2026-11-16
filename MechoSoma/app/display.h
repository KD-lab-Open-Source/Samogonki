#pragma once

#include <SDL2/SDL.h>
#include <sokol_gfx.h>

namespace graphics {

// The window the game draws into and the 3D API context that goes with it, an OpenGL
// context for now. Everything that depends on the window system or on the API sokol
// runs on lives here, so the renderer only talks to sokol.
class Display final {
 public:
  // A window of the game's screen size; setVideoMode() then picks windowed or full screen.
  Display(int width, int height);
  ~Display();

  Display(const Display&) = delete;
  Display(Display&&) = delete;
  Display& operator=(const Display&) = delete;
  Display& operator=(Display&&) = delete;

  // How the game's screen maps onto the window; the mouse code scales by it.
  struct Scale {
    float x;
    float y;
  };

  // Sizes the window to the game's screen, or makes it cover the desktop.
  Scale setVideoMode(int width, int height, bool isFullScreen);

  // The window's size in pixels: on a HiDPI screen, larger than its size in points.
  int drawableWidth() const;
  int drawableHeight() const;

  // What sg_setup needs to know about the context.
  sg_environment environment() const;

  // The window as a render target, for the pass that draws into it.
  sg_swapchain swapchain() const;

  // Shows the frame drawn into the swapchain.
  void present();

 private:
  SDL_Window* _window = nullptr;
  SDL_GLContext _context = nullptr;
};

}  // namespace graphics
