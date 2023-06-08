#pragma once

#include <sokol_gfx.h>

namespace graphics {

class OffscreenBuffer final {
 public:
  OffscreenBuffer(int width, int height);
  sg_pass getRenderingPass() const;
  void flush();

 private:
  const int _width;
  const int _height;
  sg_shader _shader;
  sg_pass _renderingPass;
  sg_image _colorTexture;
  sg_buffer _dummyBuffer;  
};

}
