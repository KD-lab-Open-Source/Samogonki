#include "Renderer.h"

#include "Matrix.h"
#include "VertexType.h"

using namespace graphics;

std::unique_ptr<graphics::Renderer> Renderer::shared;

RendererInterface& graphics::get_renderer()
{
	return *Renderer::shared;
}

Renderer::Renderer(id<MTLDevice> device, id<MTLLibrary> library) :
	_device(device),
	_library(library),
	_texture_manager(std::make_unique<TextureManager>(device))
{
	_command_queue = [device newCommandQueue];

	setup_depth_states();
	setup_pipeline_states();

	{
		const auto length = _vertex_count * 3 * sizeof(float);
		_position_buffer = [device newBufferWithLength:length options:MTLResourceStorageModeShared];
	}

	{
		const auto length = _vertex_count * 4 * sizeof(float);
		_color_buffer = [device newBufferWithLength:length options:MTLResourceStorageModeShared];
	}

	{
		const auto length = _vertex_count * 2 * sizeof(float);
		_uv_buffer = [device newBufferWithLength:length options:MTLResourceStorageModeShared];
	}

	{
		const auto length = _vertex_count * 3 * sizeof(uint32_t);
		_index_buffer = [device newBufferWithLength:length options:MTLResourceStorageModeShared];
	}
}

Renderer::~Renderer() {}

TextureManagerInterface& Renderer::get_texture_manager()
{
	return *_texture_manager;
}

void Renderer::begin(MTKView* view)
{
	_view = view;

	_uniforms.projection_matrix = matrix::make_ortho_projection(0, 1024, 768, 0, -1, 1);
	_command_buffer = [_command_queue.get() commandBuffer];
}

void Renderer::end()
{
	[_command_buffer presentDrawable:_view.currentDrawable];
	[_command_buffer commit];
}

MD3DERROR Renderer::d3dClear(DWORD dwColor)
{
	const auto red = static_cast<float>((dwColor >> 16) & 0xFF) / 255.0f;
	const auto green = static_cast<float>((dwColor >> 8) & 0xFF) / 255.0f;
	const auto blue = static_cast<float>(dwColor & 0xFF) / 255.0f;
	_clear_color = MTLClearColorMake(red, green, blue, 1.0f);
	return MD3D_OK;
}

MD3DERROR Renderer::d3dBeginScene()
{
	_commands.clear();
	return MD3D_OK;
}

MD3DERROR Renderer::d3dEndScene()
{
	auto render_pass_descriptor = _view.currentRenderPassDescriptor;
	if (render_pass_descriptor == nil)
	{
		return MD3D_OK;
	}

	auto render_encoder = [_command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];

	// prepare pipeline
	[render_encoder setVertexBuffer:_position_buffer.get() offset:0 atIndex:0];
	[render_encoder setVertexBuffer:_color_buffer.get() offset:0 atIndex:1];
	[render_encoder setVertexBuffer:_uv_buffer.get() offset:0 atIndex:2];

	[render_encoder setVertexBytes:&_uniforms length:sizeof(BasicUniforms) atIndex:3];

	for (const auto& command : _commands)
	{
		switch (command.render_state.get_option(D3DRENDERSTATE_CULLMODE))
		{
			case D3DCULL_CW:
			{
				[render_encoder setFrontFacingWinding:MTLWindingCounterClockwise];
				[render_encoder setCullMode:MTLCullModeBack];
				break;
			}

			case D3DCULL_NONE:
			{
				[render_encoder setFrontFacingWinding:MTLWindingCounterClockwise];
				[render_encoder setCullMode:MTLCullModeNone];
				break;
			}

			default:
				break;
		}

		if (command.render_state.get_option(D3DRENDERSTATE_ZWRITEENABLE) == FALSE)
		{
			[render_encoder setDepthStencilState:_depth_states[DepthStateType::Disabled].get()];
		}
		else
		{
			[render_encoder setDepthStencilState:_depth_states[DepthStateType::Enabled].get()];
		}

		if (command.render_state.get_option(D3DRENDERSTATE_ALPHABLENDENABLE) == FALSE)
		{
			[render_encoder setRenderPipelineState:_pipeline_states[PipelineStateType::Default].get()];
		}
		else
		{
			[render_encoder setRenderPipelineState:_pipeline_states[PipelineStateType::Blending].get()];
		}

		for (DWORD i = 0; i < 2; i++)
		{
			auto texture_handle = command.render_state.get_texture(i);
			if (texture_handle)
			{
				id<MTLTexture> texture = _texture_manager->get(*texture_handle);
				if (texture != nil)
				{
					[render_encoder setFragmentTexture:texture atIndex:i];
				}
			}
		}

		auto parameters = command.render_state.get_fragment_shader_parameters();
		[render_encoder setFragmentBytes:&parameters length:sizeof(FragmentShaderParameters) atIndex:0];

		if (command.index_buffer_view.length == 0)
		{
			const auto start = command.vertex_buffer_view.offset;
			const auto count = command.vertex_buffer_view.length;
			[render_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:start vertexCount:count];
		}
		else
		{
			const auto offset = command.index_buffer_view.offset * sizeof(uint32_t);
			[render_encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
				indexCount:command.index_buffer_view.length
				indexType:MTLIndexTypeUInt32
				indexBuffer:_index_buffer.get()
				indexBufferOffset:offset
			];
		}
	}

	[render_encoder endEncoding];

	return MD3D_OK;
}

MD3DERROR Renderer::d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState)
{
	_render_state.set_option(dwRenderStateType, dwRenderState);
	return MD3D_OK;
}

MD3DERROR Renderer::d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState)
{
	*lpdwRenderState = _render_state.get_option(dwRenderStateType);
	return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue)
{
	_render_state.set_texture_stage_state(dwStage, dwState, dwValue);
	return MD3D_OK;
}

MD3DERROR Renderer::d3dTriangles(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	return MD3D_OK;
}

MD3DERROR Renderer::d3dTriangleStrip(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	return MD3D_OK;
}

MD3DERROR Renderer::d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount)
{
	// The system uses vertices v2, v3, and v1 to draw the first triangle;
	// v3, v4, and v1 to draw the second triangle; v4, v5, and v1 to draw the third triangle; and so on.
	// When flat shading is enabled, the system shades the triangle with the color from its first vertex.

	if (dwVertexCount < 3)
	{
		return MD3DERR_ILLEGALCALL;
	}

	prepare_render_state(0);

	for (DWORD i = 1; i < dwVertexCount - 1; i++)
	{
		add_vertex(dwVertexTypeDesc, lpvVertices, 0);
		add_vertex(dwVertexTypeDesc, lpvVertices, i);
		add_vertex(dwVertexTypeDesc, lpvVertices, i + 1);
	}

	return MD3D_OK;
}

MD3DERROR Renderer::d3dTrianglesIndexed(
	DWORD dwVertexTypeDesc,
	LPVOID lpvVertices,
	DWORD dwVertexCount,
	LPWORD lpwIndices,
	DWORD dwIndexCount
)
{
	prepare_render_state(dwVertexCount);

	auto& last_command = _commands.back();
	const auto vertex_offset = last_command.vertex_buffer_view.end();

	for (DWORD i = 0; i < dwVertexCount; i++)
	{
		add_vertex(dwVertexTypeDesc, lpvVertices, i);
	}

	const auto start_index = last_command.index_buffer_view.end();
	auto p = static_cast<uint32_t*>(_index_buffer.get().contents);
	for (DWORD i = 0; i < dwIndexCount; i++)
	{
		p[start_index + i] = vertex_offset + lpwIndices[i];
	}
	last_command.index_buffer_view.length += dwIndexCount;

	return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTexture(DWORD dwHandle, DWORD dwStage)
{
	_render_state.set_texture(dwHandle, dwStage);
	return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend)
{
	switch (tbRGBBlend)
	{
		case MD3DTB_DIFFUSE:
			d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			break;
		case MD3DTB_TEXTURE1:
			d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			break;
		case MD3DTB_TEXTURE1_MOD_DIFFUSE:
			d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			break;
		default:
			return MD3DERR_INVALIDPARAM;
	}

	switch (tbAlphaBlend)
	{
		case MD3DTB_DIFFUSE:
			d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			break;
		case MD3DTB_TEXTURE1:
			d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			break;
		case MD3DTB_TEXTURE1_MOD_DIFFUSE:
			d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		default:
			return MD3DERR_INVALIDPARAM;
	}

	return MD3D_OK;
}

MD3DERROR Renderer::d3dTrianglesIndexed2(
	DWORD dwVertexTypeDesc,
	LPVOID lpvVertices,
	DWORD dwVertexCount,
	LPWORD lpwIndices,
	DWORD dwIndexCount,
	DWORD dwHandleTex0,
	DWORD dwHandleTex1
)
{
	d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

	d3dSetTexture(dwHandleTex0, 0);
	d3dSetTexture(dwHandleTex1, 1);
	auto result = d3dTrianglesIndexed(dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount);

	d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	return result;
}

void Renderer::setup_depth_states()
{
	{
		objc::Ref<MTLDepthStencilDescriptor*> descriptor([MTLDepthStencilDescriptor new]);
		descriptor.get().depthWriteEnabled = NO;
		descriptor.get().depthCompareFunction = MTLCompareFunctionLessEqual;

		DepthStatePtr depth_state([_device newDepthStencilStateWithDescriptor:descriptor.get()]);
		if (!depth_state)
		{
			throw std::runtime_error("setup_depth_state");
		}
		_depth_states[DepthStateType::Disabled] = depth_state;
	}

	{
		objc::Ref<MTLDepthStencilDescriptor*> descriptor([MTLDepthStencilDescriptor new]);
		descriptor.get().depthWriteEnabled = YES;
		descriptor.get().depthCompareFunction = MTLCompareFunctionLessEqual;

		DepthStatePtr depth_state([_device newDepthStencilStateWithDescriptor:descriptor.get()]);
		if (!depth_state)
		{
			throw std::runtime_error("setup_depth_state");
		}
		_depth_states[DepthStateType::Enabled] = depth_state;
	}
}

void Renderer::setup_pipeline_states()
{
	objc::Ref<MTLVertexDescriptor*> vd([MTLVertexDescriptor new]);
	vd.get().attributes[0].format = MTLVertexFormatFloat3;
	vd.get().attributes[0].bufferIndex = 0;
	vd.get().attributes[0].offset = 0;

	vd.get().attributes[1].format = MTLVertexFormatFloat4;
	vd.get().attributes[1].bufferIndex = 1;
	vd.get().attributes[1].offset = 0;

	vd.get().attributes[2].format = MTLVertexFormatFloat2;
	vd.get().attributes[2].bufferIndex = 2;
	vd.get().attributes[2].offset = 0;

	vd.get().layouts[0].stride = 12;
	vd.get().layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;

	vd.get().layouts[1].stride = 16;
	vd.get().layouts[1].stepFunction = MTLVertexStepFunctionPerVertex;

	vd.get().layouts[2].stride = 8;
	vd.get().layouts[2].stepFunction = MTLVertexStepFunctionPerVertex;

	objc::Ref<id<MTLFunction>> vertex_function([_library newFunctionWithName:@"vertex_shader"]);
	if (!vertex_function)
	{
		throw std::runtime_error("vertex function");
	}

	objc::Ref<id<MTLFunction>> fragment_function([_library newFunctionWithName:@"fragment_shader"]);
	if (!fragment_function)
	{
		throw std::runtime_error("fragment function");
	}

	{
		objc::Ref<MTLRenderPipelineDescriptor*> pd([MTLRenderPipelineDescriptor new]);
		pd.get().vertexFunction = vertex_function.get();
		pd.get().vertexDescriptor = vd.get();
		pd.get().fragmentFunction = fragment_function.get();
		pd.get().colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		pd.get().depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

		NSError* error = nil;
		objc::Ref<id<MTLRenderPipelineState>> pipeline_state(
			[_device newRenderPipelineStateWithDescriptor:pd.get() error:&error]
		);
		if (!pipeline_state)
		{
			throw std::runtime_error("new render pipeline state");
		}
		_pipeline_states[PipelineStateType::Default] = pipeline_state;
	}

	{
		objc::Ref<MTLRenderPipelineDescriptor*> pd([MTLRenderPipelineDescriptor new]);
		pd.get().vertexFunction = vertex_function.get();
		pd.get().vertexDescriptor = vd.get();
		pd.get().fragmentFunction = fragment_function.get();
		pd.get().colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		pd.get().depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

		pd.get().colorAttachments[0].blendingEnabled = YES;

		pd.get().colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
		pd.get().colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

		pd.get().colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
		pd.get().colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

		NSError* error = nil;
		PipelineStatePtr pipeline_state(
			[_device newRenderPipelineStateWithDescriptor:pd.get() error:&error]
		);
		if (!pipeline_state)
		{
			throw std::runtime_error("new render pipeline state");
		}
		_pipeline_states[PipelineStateType::Blending] = pipeline_state;
	}
}

void Renderer::prepare_render_state(size_t index_count)
{
	if (_commands.empty())
	{
		_commands.emplace_back(DrawCommand{
			_render_state,
			BufferView{0, 0},
			BufferView{0, 0}
		});
		return;
	}

	const auto& last_command = _commands.back();

	bool use_index = index_count > 0;
	bool last_use_index = last_command.index_buffer_view.length > 0;

	if (_render_state == last_command.render_state && last_use_index == use_index)
	{
		return;
	}

	_commands.emplace_back(DrawCommand{
		_render_state,
		last_command.vertex_buffer_view.next(),
		last_command.index_buffer_view.next()
	});
}

void Renderer::add_vertex(DWORD vertex_type, LPVOID vertices, DWORD index)
{
	assert(!_commands.empty());

	auto& last_command = _commands.back();
	size_t i = last_command.vertex_buffer_view.end();

	VertexType type{vertex_type};
	auto vertex_offset = static_cast<uint8_t*>(vertices) + type.get_vertex_size() * index;

	if (vertex_type & D3DFVF_XYZRHW)
	{
		auto p = reinterpret_cast<const float*>(vertex_offset);

		auto position = static_cast<float*>(_position_buffer.get().contents);
		position[3 * i] = p[0];
		position[3 * i + 1] = p[1];
		position[3 * i + 2] = p[2];

		vertex_offset += type.get_offset(D3DFVF_XYZRHW);
	}

	if (vertex_type & D3DFVF_DIFFUSE)
	{
		auto p = reinterpret_cast<const DWORD*>(vertex_offset);
		const auto argb = p[0];

		auto color = static_cast<float*>(_color_buffer.get().contents);
		color[4 * i] = static_cast<float>((argb >> 16) & 0xFF) / 255.0f;
		color[4 * i + 1] = static_cast<float>((argb >> 8) & 0xFF) / 255.0f;
		color[4 * i + 2] = static_cast<float>(argb & 0xFF) / 255.0f;
		color[4 * i + 3] = static_cast<float>((argb >> 24) & 0xFF) / 255.0f;

		vertex_offset += type.get_offset(D3DFVF_DIFFUSE);
	}

	if (vertex_type & D3DFVF_SPECULAR)
	{
		vertex_offset += type.get_offset(D3DFVF_SPECULAR);
	}

	if (vertex_type & D3DFVF_TEX1)
	{
		auto p = reinterpret_cast<const float*>(vertex_offset);

		auto uv = static_cast<float*>(_uv_buffer.get().contents);
		uv[2 * i] = p[0];
		uv[2 * i + 1] = p[1];
	}

	last_command.vertex_buffer_view.length += 1;
}
