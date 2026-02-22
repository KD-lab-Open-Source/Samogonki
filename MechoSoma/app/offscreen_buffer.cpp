#include "offscreen_buffer.h"

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-flush-shader.h"
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
    if (_sampler.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_sampler", XERR_USER, 0, "");
    }
  }

  {
    sg_image_desc description{};
    description.usage.color_attachment = true;
    description.width = width;
    description.height = height;
    description.num_slices = 1;
    description.num_mipmaps = 1;
    description.sample_count = 1;
    _colorTexture = sg_make_image(&description);
    if (_colorTexture.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_image", XERR_USER, 0, "");
    }

    description.usage.depth_stencil_attachment = true;
    description.pixel_format = SG_PIXELFORMAT_DEPTH_STENCIL;
    _depthTexture = sg_make_image(&description);
    if (_depthTexture.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_image", XERR_USER, 0, "");
    }
  }

  {
    sg_view_desc description{};
    description.color_attachment.image = _colorTexture;
    _colorAttachmentView = sg_make_view(&description);
    if (_colorAttachmentView.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
    }
  }

  {
    sg_view_desc description{};
    description.texture.image = _colorTexture;
    _colorTextureView = sg_make_view(&description);
    if (_colorTextureView.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
    }
  }

  {
    sg_view_desc description{};
    description.depth_stencil_attachment.image = _depthTexture;
    _depthTextureView = sg_make_view(&description);
    if (_depthTextureView.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
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

  _clipSpaceMaxX = 1.0f * width / height * drawableHeight / drawableWidth;
}

OffscreenBuffer::~OffscreenBuffer() {
  sg_destroy_buffer(_dummyBuffer);
  sg_destroy_sampler(_sampler);
  sg_destroy_view(_colorTextureView);
  sg_destroy_view(_colorAttachmentView);
  sg_destroy_image(_colorTexture);
  sg_destroy_shader(_shader);
}

void OffscreenBuffer::begin_pass(sg_pass_action action) {
  sg_pass pass{};
  pass.action = action;
  pass.attachments.colors[0] = _colorAttachmentView;
  pass.attachments.depth_stencil = _depthTextureView;
  sg_begin_pass(&pass);
}

void OffscreenBuffer::flush() {
  sg_pass_action defaultPassAction = {};
  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  defaultPassAction.colors[0].clear_value = {0, 0, 0, 1};

  sg_pass pass{};
  pass.action = defaultPassAction;
  pass.swapchain.width = _drawableWidth;
  pass.swapchain.height = _drawableHeight;
  pass.swapchain.color_format = SG_PIXELFORMAT_RGBA8;
  pass.swapchain.depth_format = SG_PIXELFORMAT_DEPTH_STENCIL;
  pass.swapchain.gl.framebuffer = 0;
  sg_begin_pass(&pass);

  sg_pipeline_desc description = {};
  description.shader = _shader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

  sg_bindings bindings = {};
  bindings.views[VIEW_texture_1] = _colorTextureView;
  bindings.samplers[SMP_sampler_1] = _sampler;
  bindings.vertex_buffers[0] = _dummyBuffer;

  auto pipeline = sg_make_pipeline(description);
  sg_apply_pipeline(pipeline);
  sg_apply_bindings(bindings);

  auto vs_params = flush_vs_params_t{
    .max_x = _clipSpaceMaxX
  };
  sg_apply_uniforms(UB_flush_vs_params, SG_RANGE(vs_params));

  sg_draw(0, 6, 1);
  sg_destroy_pipeline(pipeline);

  sg_end_pass();
  sg_commit();
}

int OffscreenBuffer::getWidth() const {
  return _width;
}

int OffscreenBuffer::getHeight() const {
  return _height;
}
