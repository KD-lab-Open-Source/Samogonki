#pragma once

#include <vector>

#include <sokol_gfx.h>

namespace graphics {

class BackBuffer final {
 public:
  BackBuffer(int width, int height);
  ~BackBuffer();

  struct Address {
    uint16_t *address = nullptr;
    uint32_t pitch = 0;
  };
  Address lock();
  void unlock();
  void flush();

 private:
  const int _width;
  const int _height;
  sg_shader _quadShader;
  sg_buffer _dummyBuffer;
  sg_image _texture;
  sg_sampler _sampler;
  std::vector<uint16_t> _buffer;
  uint32_t _pitch;
  bool _isLocked = false;
};

}
