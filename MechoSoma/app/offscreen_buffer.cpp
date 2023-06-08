#include "offscreen_buffer.h"

#include "sokol-shader.h"
#include "xerrhand.h"

using namespace graphics;

OffscreenBuffer::OffscreenBuffer(int width, int height) : _width(width), _height(height) {
  _shader = sg_make_shader(flush_shader_desc(sg_query_backend()));
  if (_shader.id == SG_INVALID_ID) {
    XAssert("sg_make_shader");
  }

  sg_image_desc image_description{};
  image_description.render_target = true;
  image_description.width = width;
  image_description.height = height;
  image_description.sample_count = 1;
  image_description.min_filter = SG_FILTER_LINEAR;
  image_description.mag_filter = SG_FILTER_LINEAR;
  _colorTexture = sg_make_image(&image_description);

  image_description.pixel_format = SG_PIXELFORMAT_DEPTH;
  sg_image depth_image = sg_make_image(&image_description);

  sg_pass_desc pass_description{};
  pass_description.color_attachments[0].image = _colorTexture;
  pass_description.depth_stencil_attachment.image = depth_image;
  _renderingPass = sg_make_pass(pass_description);
  if (_renderingPass.id == SG_INVALID_ID) {
    XAssert("sg_make_pass");
  }

  sg_buffer_desc buffer_description{};
  buffer_description.size = 1;
  buffer_description.type = SG_BUFFERTYPE_VERTEXBUFFER;
  buffer_description.usage = SG_USAGE_DYNAMIC;
  _dummyBuffer = sg_make_buffer(buffer_description);
}

sg_pass OffscreenBuffer::getRenderingPass() const {
  return _renderingPass;
}

void OffscreenBuffer::flush() {
  sg_pass_action defaultPassAction = {};
  sg_begin_default_pass(defaultPassAction, _width, _height);

  sg_pipeline_desc description = {};
  description.shader = _shader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

  sg_bindings bindings = {};
  bindings.fs_images[0] = _colorTexture;
  bindings.vertex_buffers[0] = _dummyBuffer;

  auto pipeline = sg_make_pipeline(description);
  sg_apply_pipeline(pipeline);
  sg_apply_bindings(bindings);
  sg_draw(0, 3, 1);
  sg_destroy_pipeline(pipeline);

  sg_end_pass();
  sg_commit();
}
