#ifndef SHADER_COMMON_H
#define SHADER_COMMON_H

#include <simd/simd.h>

struct BasicUniforms
{
	simd_float4x4 projection_matrix;
};

enum class TextureColorOperation
{
	Disable,
	Texture,
	Modulate
};

struct FragmentShaderParameters
{
	TextureColorOperation color_operation_1;
	TextureColorOperation color_operation_2;
};

#endif
