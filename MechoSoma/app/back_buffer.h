#pragma once

#include <cstdint>

#include <sokol_gfx.h>

namespace graphics {

// The game's 2D back buffer on the GPU (sokol): RGB 565 pixels, drawn over the whole
// offscreen buffer. Renderer keeps the pixels the game writes.
class BackBuffer final {
 public:
  BackBuffer(int width, int height);
  ~BackBuffer();

  // A whole frame of pixels, width x height.
  void upload(const uint16_t* pixels);
  // Draws it, inside the scene pass.
  void flush();

 private:
  const int _width;
  const int _height;
  sg_shader _quadShader;
  sg_pipeline _pipeline;
  sg_buffer _dummyBuffer;
  sg_image _texture;
  sg_view _textureView;
  sg_sampler _sampler;
};

}
