#pragma once

#import <MetalKit/MetalKit.h>

#include <array>
#include <memory>
#include <simd/simd.h>

#include "EnumArray.h"
#include "Ref.h"
#include "RendererInterface.h"
#include "RenderState.h"
#include "ShaderCommon.h"
#include "TextureManager.h"

namespace graphics
{

class Renderer final : public RendererInterface
{
public:
	static std::unique_ptr<Renderer> shared;

	Renderer(id<MTLDevice> device, id<MTLLibrary> library);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void begin(MTKView* view);
	void end();

	TextureManagerInterface& get_texture_manager() override;

	MD3DERROR d3dClear(DWORD dwColor) override;
	MD3DERROR d3dBeginScene() override;
	MD3DERROR d3dEndScene() override;
	MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) override;
	MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState) override;
	MD3DERROR d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) override;
	MD3DERROR d3dTriangles(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) override;
	MD3DERROR d3dTriangleStrip(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) override;
	MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) override;
	MD3DERROR d3dTrianglesIndexed(
		DWORD dwVertexTypeDesc,
		LPVOID lpvVertices,
		DWORD dwVertexCount,
		LPWORD lpwIndices,
		DWORD dwIndexCount
	) override;

	MD3DERROR d3dSetTexture(DWORD dwHandle, DWORD dwStage) override;
	MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) override;

	MD3DERROR d3dTrianglesIndexed2(
		DWORD dwVertexTypeDesc,
		LPVOID lpvVertices,
		DWORD dwVertexCount,
		LPWORD lpwIndices,
		DWORD dwIndexCount,
		DWORD dwHandleTex0,
		DWORD dwHandleTex1
	) override;

private:
	void setup_depth_states();
	void setup_pipeline_states();
	void prepare_render_state(size_t index_count);
	size_t get_pipeline_state(const d3d::RenderState&);
	void add_vertex(DWORD vertex_type, LPVOID vertices, DWORD index);

private:
	const size_t _vertex_count = 35000;

	id<MTLDevice> _device;
	id<MTLLibrary> _library;
	std::unique_ptr<TextureManager> _texture_manager;
	objc::Ref<id<MTLCommandQueue>> _command_queue;
	BasicUniforms _uniforms;
	MTLClearColor _clear_color;

	objc::Ref<id<MTLBuffer>> _position_buffer;
	objc::Ref<id<MTLBuffer>> _color_buffer;
	objc::Ref<id<MTLBuffer>> _uv_buffer;
	objc::Ref<id<MTLBuffer>> _index_buffer;

	MTKView* _view = nil;
	id<MTLCommandBuffer> _command_buffer = nil;

	d3d::RenderState _render_state;

	enum class DepthStateType
	{
		Disabled,
		Enabled
	};
	using DepthStatePtr = objc::Ref<id<MTLDepthStencilState>>;
	EnumArray<DepthStateType, DepthStatePtr, 2> _depth_states;

	enum class PipelineStateType
	{
		Default,
		Blending
	};
	using PipelineStatePtr = objc::Ref<id<MTLRenderPipelineState>>;
	EnumArray<PipelineStateType, PipelineStatePtr, 2> _pipeline_states;

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
};

}
