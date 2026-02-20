#include "back_buffer.h"

#include <cassert>

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-back-buffer-shader.h"
#endif

#include "xtool.h"

using namespace graphics;

BackBuffer::BackBuffer(int width, int height) : _buffer(width * height), _pitch(width * sizeof(uint16_t)),
                                                _width(width), _height(height) {
  _quadShader = sg_make_shader(back_buffer_shader_desc(sg_query_backend()));
  if (_quadShader.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_shader", XERR_USER, 0, "");
  }

  {
    sg_image_desc description{};
    description.width = width;
    description.height = height;
    description.num_slices = 1;
    description.num_mipmaps = 1;
    description.usage.dynamic_update = true;
    description.pixel_format = SG_PIXELFORMAT_R16UI;
    description.sample_count = 1;
    _texture = sg_make_image(description);
    if (_texture.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_image", XERR_USER, 0, "");
    }
  }

  {
    sg_view_desc description{};
    description.texture.image = _texture;
    _textureView = sg_make_view(&description);
    if (_textureView.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
    }
  }

  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_NEAREST;
    description.mag_filter = SG_FILTER_NEAREST;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    _sampler = sg_make_sampler(description);
    if (_sampler.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_sampler", XERR_USER, 0, "");
    }
  }

  {
    sg_buffer_desc description{};
    description.size = 1;
    description.usage.vertex_buffer = true;
    description.usage.dynamic_update = true;
    _dummyBuffer = sg_make_buffer(description);
    if (_dummyBuffer.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_buffer", XERR_USER, 0, "");
    }
  }
}

BackBuffer::~BackBuffer() {
  sg_destroy_buffer(_dummyBuffer);
  sg_destroy_sampler(_sampler);
  sg_destroy_view(_textureView);
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
  data.mip_levels[0].ptr = _buffer.data();
  data.mip_levels[0].size = _buffer.size() * sizeof(uint16_t);
  sg_update_image(_texture, data);
}

void BackBuffer::flush() {
  assert(!_isLocked);

  sg_pipeline_desc description = {};
  description.shader = _quadShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

  sg_bindings bindings = {};
  bindings.views[VIEW_texture_1] = _textureView;
  bindings.samplers[SMP_sampler_1] = _sampler;
  bindings.vertex_buffers[0] = _dummyBuffer;

  auto pipeline = sg_make_pipeline(description);
  sg_apply_pipeline(pipeline);
  sg_apply_bindings(bindings);
  sg_draw(0, 3, 1);
  sg_destroy_pipeline(pipeline);
}
