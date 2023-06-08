//
// Created by caiiiycuk on 08.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDERER_H
#define MOONSHINE_RUNNERS_RENDERER_H

#include <array>
#include <memory>
#include <vector>

#include <sokol_gfx.h>

#include "renderer_interface.h"
#include "render_state.h"
#include "texture_manager_interface.h"

namespace graphics {

class BackBuffer;
class OffscreenBuffer;
class TextureManager;

class Renderer final : public RendererInterface {
 public:
  static std::unique_ptr<Renderer> shared;

  Renderer();
  ~Renderer() override;

  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void flush();

  TextureManagerInterface& get_texture_manager() override;

  MD3DERROR d3dClear(DWORD dwColor) override;
  MD3DERROR d3dBeginScene() override;
  MD3DERROR d3dEndScene() override;
  MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) override;
  MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD* lpdwRenderState) override;
  MD3DERROR d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) override;
  MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) override;
  MD3DERROR d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                                DWORD dwIndexCount) override;

  MD3DERROR d3dSetTexture(DWORD dwHandle, DWORD dwStage) override;
  MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) override;

  MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                                 DWORD dwIndexCount, DWORD dwHandleTex0, DWORD dwHandleTex1) override;

  MD3DERROR d3dLockBackBuffer(VOID **lplpSurface, DWORD *lpdwPitch) override;
  MD3DERROR d3dUnlockBackBuffer() override;
  MD3DERROR d3dFlushBackBuffer(RECT *lprcRect) override;

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
