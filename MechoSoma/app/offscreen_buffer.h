#pragma once

#include <sokol_gfx.h>

namespace graphics {

class OffscreenBuffer final {
 public:
  OffscreenBuffer(int width, int height, int drawableWidth, int drawableHeight);
  ~OffscreenBuffer();

  sg_pass getRenderingPass() const;
  void flush();

 private:
  const int _width;
  const int _height;
  const int _drawableWidth;
  const int _drawableHeight;
  float _clipSpaceMaxX;

  sg_shader _shader;
  sg_pass _renderingPass;
  sg_image _colorTexture;
  sg_buffer _dummyBuffer;  
};

}
