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

#include "Md3d.h"
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

  MD3DERROR d3dClear(uint32_t dwColor);
  MD3DERROR d3dFlip(bool WaitVerticalBlank);
  MD3DERROR d3dBeginScene();
  MD3DERROR d3dEndScene();
  MD3DERROR d3dSetProjectionMatrix(const D3DMATRIX &matrix);
  MD3DERROR d3dResetProjectionMatrix();
  MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t dwRenderState);
  MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t* lpdwRenderState);
  MD3DERROR d3dSetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue);
  MD3DERROR d3dTriangleFan(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount);
  MD3DERROR d3dTrianglesIndexed(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount, uint16_t* lpwIndices,
                                uint32_t dwIndexCount);

  MD3DERROR d3dSetTexture(uint32_t dwHandle, uint32_t dwStage);
  MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend);

  MD3DERROR d3dTrianglesIndexed2(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount, uint16_t *lpwIndices,
                                 uint32_t dwIndexCount, uint32_t dwHandleTex0, uint32_t dwHandleTex1);

  MD3DERROR d3dLockBackBuffer(void** lplpSurface, uint32_t* lpdwPitch);
  MD3DERROR d3dUnlockBackBuffer();
  MD3DERROR d3dFlushBackBuffer(MD3DRECT* lprcRect);

 private:
  void prepare_render_state(size_t index_count);
  void add_vertex(uint32_t vertex_type, void* vertices, uint32_t index);

private:
  const size_t _vertex_count = 65000;

  SDL_Window *_window = nullptr;
  SDL_GLContext _context = nullptr;
  std::array<float, 16> _projectionMatrix;

  sg_shader _sceneShader;
  sg_image _nullTexture;

  std::vector<float> _position_buffer;
  std::vector<float> _color_buffer;
  std::vector<float> _uv_buffer;
  std::vector<uint32_t> _index_buffer;

  sg_buffer sg_position_buffer;
  sg_buffer sg_color_buffer;
  sg_buffer sg_uv_buffer;
  sg_buffer sg_index_buffer;
  sg_sampler _clamp_sampler;
  sg_sampler _repeat_sampler;

  std::unique_ptr<OffscreenBuffer> _offscreenBuffer;
  std::unique_ptr<BackBuffer> _backBuffer;
  std::unique_ptr<TextureManager> _texture_manager;

  sg_pass_action defaultPassAction = {};
  d3d::RenderState _render_state;

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
    d3d::RenderState render_state;
    BufferView vertex_buffer_view;
    BufferView index_buffer_view;
  };

  std::vector<DrawCommand> _commands;
  bool _is_back_buffer_flush = false;
};

}  // namespace graphics

#endif  // MOONSHINE_RUNNERS_RENDERER_H
