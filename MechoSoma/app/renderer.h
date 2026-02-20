//
// Created by caiiiycuk on 08.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDERER_H
#define MOONSHINE_RUNNERS_RENDERER_H

#include <array>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <sokol_gfx.h>

#include "IGraph3d.h"
#include "render_state.h"

namespace graphics {

class BackBuffer;
class OffscreenBuffer;
class TextureManager;

class Renderer final {
 public:
  Renderer(int width, int height, bool isFullScreen);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  TextureManager& get_texture_manager();
  void setVideoMode(int width, int height, bool isFullScreen);
  bool isInScene() const;

  MD3DERROR clear(uint32_t dwColor);
  MD3DERROR flip(bool WaitVerticalBlank);
  MD3DERROR setClipRect(const MD3DRECT &lprcClipRect);
  MD3DERROR resetClipRect();
  MD3DERROR beginScene();
  MD3DERROR endScene();
  MD3DERROR setProjectionMatrix(const D3DMATRIX &matrix);
  MD3DERROR resetProjectionMatrix();

  MD3DERROR setRenderState(eRenderStateOption option, int value);
  MD3DERROR setMaterial(eMaterialMode material);
  MD3DERROR setTexture(uint32_t dwHandle, uint32_t dwStage);

  MD3DERROR beginDrawCommand(M3D_DRAW_COMMAND &command);
  MD3DERROR endDrawCommand(const M3D_DRAW_COMMAND &command);

  MD3DERROR lockBackBuffer(void** lplpSurface, uint32_t* lpdwPitch);
  MD3DERROR unlockBackBuffer();
  MD3DERROR flushBackBuffer(MD3DRECT* lprcRect);

 private:
  void prepare_render_state();

private:
  const size_t max_vertex_count = 100000;

  SDL_Window *_window = nullptr;
  SDL_GLContext _context = nullptr;
  std::array<float, 16> _projectionMatrix;

  sg_shader _sceneShader;
  sg_image _nullTexture;
  sg_view _nullTextureView;

  std::vector<float> _position_buffer;
  std::vector<float> _diffuse_color_buffer;
  std::vector<float> _specular_color_buffer;
  std::vector<float> _uv_buffer;
  std::vector<uint32_t> _index_buffer;

  sg_buffer sg_position_buffer;
  sg_buffer sg_diffuse_color_buffer;
  sg_buffer sg_specular_color_buffer;
  sg_buffer sg_uv_buffer;
  sg_buffer sg_index_buffer;
  sg_sampler _clamp_sampler;
  sg_sampler _repeat_sampler;

  std::unique_ptr<OffscreenBuffer> _offscreenBuffer;
  std::unique_ptr<BackBuffer> _backBuffer;
  std::unique_ptr<TextureManager> _texture_manager;

  sg_pass_action defaultPassAction = {};
  RenderState _render_state;

  struct BufferView
  {
    size_t length;
    size_t offset;

    size_t end() const
    {
      return length + offset;
    }

    BufferView next() const
    {
      return BufferView{0, end()};
    }
  };

  struct DrawCommand
  {
    RenderState render_state;
    BufferView vertex_buffer_view;
    BufferView index_buffer_view;
  };

  std::vector<DrawCommand> _commands;
  bool _is_back_buffer_flush = false;
  bool _is_in_scene = false;
};

}  // namespace graphics

#endif  // MOONSHINE_RUNNERS_RENDERER_H
