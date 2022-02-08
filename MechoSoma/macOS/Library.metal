#include <metal_stdlib>
#include <simd/simd.h>

#include "ShaderCommon.h"

using namespace metal;
using namespace simd;

struct InputVertex
{
	float3 position [[attribute(0)]];
	float4 color [[attribute(1)]];
	float2 uv [[attribute(2)]];
};

struct ProjectedVertex
{
	float4 position [[position]];
	float4 color;
	float2 uv;
};

vertex ProjectedVertex vertex_shader
(
	InputVertex input_vertex [[stage_in]],
	constant BasicUniforms& uniforms [[buffer(3)]]
)
{
	ProjectedVertex output;
	output.position = uniforms.projection_matrix * float4(input_vertex.position, 1.0f);
	output.color = input_vertex.color;
	output.uv = input_vertex.uv;
	return output;
}

fragment half4 fragment_shader
(
	ProjectedVertex input [[stage_in]],
	constant FragmentShaderParameters& parameters [[buffer(0)]],
	texture2d<float> texture_1 [[texture(0)]],
	texture2d<float> texture_2 [[texture(1)]]
)
{
	constexpr sampler s_repeat(address::repeat, filter::linear);
	constexpr sampler s_clamp(address::clamp_to_edge, filter::linear);

	float4 result_color;
	switch (parameters.color_operation_1)
	{
	case TextureColorOperation::Disable:
	{
		result_color = input.color;
		break;
	}

	case TextureColorOperation::Texture:
	{
		result_color = texture_1.sample(s_repeat, input.uv);
		break;
	}

	case TextureColorOperation::Modulate:
	{
		result_color = texture_1.sample(s_repeat, input.uv) * input.color;
		break;
	}

	default:
		result_color = 0.0f;
	}

	if (parameters.color_operation_2 == TextureColorOperation::Modulate)
	{
		result_color *= texture_2.sample(s_clamp, input.uv);
	}

	return half4(result_color);
}
