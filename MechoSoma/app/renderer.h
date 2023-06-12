//
// Created by caiiiycuk on 08.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDERER_H
#define MOONSHINE_RUNNERS_RENDERER_H

#include <array>
#include <memory>
#include <vector>

#include <sokol_gfx.h>

#include "Md3d.h"
#include "render_state.h"

namespace graphics {

class BackBuffer;
class OffscreenBuffer;

void setupRenderer();
extern void swapWindow();

class TextureManager;

class Renderer final {
 public:
  static inline std::unique_ptr<Renderer> shared;

 public:
  Renderer();
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  TextureManager& get_texture_manager();

  MD3DERROR d3dClear(DWORD dwColor);
  MD3DERROR d3dFlip(bool WaitVerticalBlank);
  MD3DERROR d3dBeginScene();
  MD3DERROR d3dEndScene();
  MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState);
  MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD* lpdwRenderState);
  MD3DERROR d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue);
  MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount);
  MD3DERROR d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                                DWORD dwIndexCount);

  MD3DERROR d3dSetTexture(DWORD dwHandle, DWORD dwStage);
  MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend);

  MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                                 DWORD dwIndexCount, DWORD dwHandleTex0, DWORD dwHandleTex1);

  MD3DERROR d3dLockBackBuffer(VOID **lplpSurface, DWORD *lpdwPitch);
  MD3DERROR d3dUnlockBackBuffer();
  MD3DERROR d3dFlushBackBuffer(RECT *lprcRect);

 private:
  void prepare_render_state(size_t index_count);
  void add_vertex(DWORD vertex_type, LPVOID vertices, DWORD index);

  const size_t _vertex_count = 35000;

  sg_shader _sceneShader;
  sg_image _nullTexture;

  std::unique_ptr<float> _position_buffer;
  std::unique_ptr<float> _color_buffer;
  std::unique_ptr<float> _uv_buffer;
  std::unique_ptr<uint32_t> _index_buffer;

  sg_buffer sg_position_buffer;
  sg_buffer sg_color_buffer;
  sg_buffer sg_uv_buffer;
  sg_buffer sg_index_buffer;

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
