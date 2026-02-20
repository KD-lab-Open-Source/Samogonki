#pragma once

#include <sokol_gfx.h>

namespace graphics {

class OffscreenBuffer final {
 public:
  OffscreenBuffer(int width, int height, int drawableWidth, int drawableHeight);
  ~OffscreenBuffer();

  void begin_pass(sg_pass_action action);
  void flush();

  int getWidth() const;
  int getHeight() const;

 private:
  const int _width;
  const int _height;
  const int _drawableWidth;
  const int _drawableHeight;
  float _clipSpaceMaxX;

  sg_shader _shader;
  sg_image _colorTexture;
  sg_view _colorAttachmentView;
  sg_view _colorTextureView;
  sg_image _depthTexture;
  sg_view _depthTextureView;
  sg_sampler _sampler;
  sg_buffer _dummyBuffer;
};

}
