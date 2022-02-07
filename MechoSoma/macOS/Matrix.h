#pragma once

#include <simd/simd.h>

namespace graphics::matrix
{

// left hand
simd_float4x4 make_perspective_projection(float fov_y, float aspect, float near, float far);

// left hand
simd_float4x4 make_ortho_projection(float left, float right, float bottom, float top, float near, float far);

// left hand
simd_float4x4 make_look_at(simd_float3 eye, simd_float3 target, simd_float3 up);

simd_float4x4 make_rotation(simd_float3 axis, float angle);
simd_float4x4 make_scale(simd_float3 s);
simd_float4x4 make_transform(simd_float3 t);

}
