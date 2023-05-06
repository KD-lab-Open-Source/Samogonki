#include "back_buffer.h"

#include <cassert>

#include "sokol-shader.h"
#include "xtool.h"

using namespace graphics;

BackBuffer::BackBuffer(int width, int height) : _buffer(width * height), _pitch(width * sizeof(uint16_t)),
                                                _width(width), _height(height) {
  _quadShader = sg_make_shader(back_buffer_shader_desc(sg_query_backend()));
  if (_quadShader.id == SG_INVALID_ID) {
    XAssert("sg_make_shader");
  }

  sg_image_desc description = {
      .width = width,
      .height = height,
      .usage = SG_USAGE_DYNAMIC,
      .pixel_format = SG_PIXELFORMAT_R16UI
  };
  _texture = sg_make_image(description);

  _dummyBuffer = sg_make_buffer(sg_buffer_desc{
      .size = 1,
      .type = SG_BUFFERTYPE_VERTEXBUFFER,
      .usage = SG_USAGE_DYNAMIC,
  });
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

  sg_pass_action defaultPassAction = {};
  sg_begin_default_pass(defaultPassAction, _width, _height);

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

  sg_end_pass();
  sg_commit();
}
