#pragma once

#include <memory>

#include <SDL3/SDL.h>
#include <sokol_gfx.h>

#include "window.h"

namespace graphics {

// The window and the OpenGL context sokol draws with. Everything sokol needs to know about
// the window system lives here, so the sokol backend only talks to sokol.
class Display final {
 public:
  // A window of the game's screen size; setVideoMode() then picks windowed or full screen.
  Display(int width, int height);
  ~Display();

  Display(const Display&) = delete;
  Display(Display&&) = delete;
  Display& operator=(const Display&) = delete;
  Display& operator=(Display&&) = delete;

  Window& window() { return *_window; }

  // What sg_setup needs to know about the context.
  sg_environment environment() const;

  // The window as a render target, for the pass that draws into it.
  sg_swapchain swapchain() const;

  // Shows the frame drawn into the swapchain.
  void present();

 private:
  std::unique_ptr<Window> _window;
  SDL_GLContext _context = nullptr;
};

}  // namespace graphics
