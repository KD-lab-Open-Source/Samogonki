#include "offscreen_buffer.h"

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-shader.h"
#endif

#include "xerrhand.h"

using namespace graphics;

OffscreenBuffer::OffscreenBuffer(int width, int height,  int drawableWidth, int drawableHeight) 
  : _width(width), _height(height), _drawableWidth(drawableWidth), _drawableHeight(drawableHeight) {
  _shader = sg_make_shader(flush_shader_desc(sg_query_backend()));
  if (_shader.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_shader", XERR_USER, 0, "");
  }

  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    _sampler = sg_make_sampler(description);
  }

  sg_image_desc image_description{};
  image_description.render_target = true;
  image_description.width = width;
  image_description.height = height;
  image_description.num_slices = 1;
  image_description.num_mipmaps = 1;
  image_description.sample_count = 1;
  _colorTexture = sg_make_image(&image_description);
  if (_colorTexture.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_image", XERR_USER, 0, "");
  }

  image_description.pixel_format = SG_PIXELFORMAT_DEPTH;
  sg_image depth_image = sg_make_image(&image_description);

  sg_pass_desc pass_description{};
  pass_description.color_attachments[0].image = _colorTexture;
  pass_description.depth_stencil_attachment.image = depth_image;
  _renderingPass = sg_make_pass(pass_description);
  if (_renderingPass.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_pass", XERR_USER, 0, "");
  }

  sg_buffer_desc buffer_description{};
  buffer_description.size = 1;
  buffer_description.type = SG_BUFFERTYPE_VERTEXBUFFER;
  buffer_description.usage = SG_USAGE_DYNAMIC;
  _dummyBuffer = sg_make_buffer(buffer_description);

  _clipSpaceMaxX = 1.0f * width / height * drawableHeight / drawableWidth;
}

OffscreenBuffer::~OffscreenBuffer() {
  sg_destroy_buffer(_dummyBuffer);
  sg_destroy_pass(_renderingPass);
  sg_destroy_sampler(_sampler);
  sg_destroy_image(_colorTexture);
  sg_destroy_shader(_shader);
}

sg_pass OffscreenBuffer::getRenderingPass() const {
  return _renderingPass;
}

void OffscreenBuffer::flush() {
  sg_pass_action defaultPassAction = {};
  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  defaultPassAction.colors[0].clear_value = {0, 0, 0, 1};
  sg_begin_default_pass(defaultPassAction, _drawableWidth, _drawableHeight);

  sg_pipeline_desc description = {};
  description.shader = _shader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

  sg_bindings bindings = {};
  bindings.fs.images[0] = _colorTexture;
  bindings.fs.samplers[0] = _sampler;
  bindings.vertex_buffers[0] = _dummyBuffer;

  auto pipeline = sg_make_pipeline(description);
  sg_apply_pipeline(pipeline);
  sg_apply_bindings(bindings);

  auto vs_params = flush_vs_params_t{
    .max_x = _clipSpaceMaxX
  };
  sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_flush_vs_params, SG_RANGE(vs_params));

  sg_draw(0, 6, 1);
  sg_destroy_pipeline(pipeline);

  sg_end_pass();
  sg_commit();
}
