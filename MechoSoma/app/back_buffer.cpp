#include "back_buffer.h"

#include <cassert>

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-shader.h"
#endif

#include "xtool.h"

using namespace graphics;

BackBuffer::BackBuffer(int width, int height) : _buffer(width * height), _pitch(width * sizeof(uint16_t)),
                                                _width(width), _height(height) {
  _quadShader = sg_make_shader(back_buffer_shader_desc(sg_query_backend()));
  if (_quadShader.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_shader", XERR_USER, 0, "");
  }

  sg_image_desc description = {};
  description.width = width;
  description.height = height;
  description.usage = SG_USAGE_DYNAMIC;
  description.pixel_format = SG_PIXELFORMAT_R16UI;
  _texture = sg_make_image(description);

  sg_buffer_desc buffer_description{};
  buffer_description.size = 1;
  buffer_description.type = SG_BUFFERTYPE_VERTEXBUFFER;
  buffer_description.usage = SG_USAGE_DYNAMIC;
  _dummyBuffer = sg_make_buffer(buffer_description);
}

BackBuffer::~BackBuffer() {
  sg_destroy_buffer(_dummyBuffer);
  sg_destroy_image(_texture);
  sg_destroy_shader(_quadShader);
}

BackBuffer::Address BackBuffer::lock() {
  assert(!_isLocked);

  _isLocked = true;
  return {_buffer.data(), _pitch};
}

void BackBuffer::unlock() {
  assert(_isLocked);

  _isLocked = false;
  sg_image_data data;
  data.subimage[0][0] = sg_range{
      .ptr = _buffer.data(),
      .size = _buffer.size() * sizeof(uint16_t)
  };
  sg_update_image(_texture, data);
}

void BackBuffer::flush() {
  assert(!_isLocked);

  sg_pipeline_desc description = {};
  description.shader = _quadShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

  sg_bindings bindings = {};
  bindings.fs_images[0] = _texture;
  bindings.vertex_buffers[0] = _dummyBuffer;

  auto pipeline = sg_make_pipeline(description);
  sg_apply_pipeline(pipeline);
  sg_apply_bindings(bindings);
  sg_draw(0, 3, 1);
  sg_destroy_pipeline(pipeline);
}
