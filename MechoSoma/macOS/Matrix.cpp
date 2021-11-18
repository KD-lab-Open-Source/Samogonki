#include "Matrix.h"

using namespace graphics;

simd_float4x4 matrix_make_rows(float m00, float m10, float m20, float m30,
                               float m01, float m11, float m21, float m31,
                               float m02, float m12, float m22, float m32,
                               float m03, float m13, float m23, float m33)
{
    return (simd_float4x4){
        {
            { m00, m01, m02, m03 },     // each line here provides column data
            { m10, m11, m12, m13 },
            { m20, m21, m22, m23 },
            { m30, m31, m32, m33 }
        }
    };
}

simd_float4x4 matrix::make_perspective_projection(float fov_y, float aspect, float near, float far)
{
    float y_scale = 1.0f / tanf(fov_y * 0.5f);
    float x_scale = y_scale / aspect;
    float z_scale = far / (far - near);

    return matrix_make_rows(
        x_scale, 0, 0, 0,
        0, y_scale, 0, 0,
        0, 0, z_scale, -near * z_scale,
        0, 0, 1.0f, 0
    );
}

simd_float4x4 matrix::make_ortho_projection(float left, float right, float bottom, float top, float near, float far)
{
    return matrix_make_rows(
        2.0f / (right - left), 0, 0, (left + right) / (left - right),
        0, 2.0f / (top - bottom), 0, (top + bottom) / (bottom - top),
        0, 0, 1.0f / (far - near), near / (near - far),
        0, 0, 0, 1.0f
    );
}

simd_float4x4 matrix::make_look_at(simd_float3 eye, simd_float3 target, simd_float3 up)
{
    simd_float3 z = simd_normalize(target - eye);
    simd_float3 x = simd_normalize(simd_cross(up, z));
    simd_float3 y = simd_cross(z, x);
    simd_float3 t{-simd_dot(x, eye), -simd_dot(y, eye), -simd_dot(z, eye)};

    return matrix_make_rows(
        x.x, x.y, x.z, t.x,
        y.x, y.y, y.z, t.y,
        z.x, z.y, z.z, t.z,
        0, 0, 0, 1.0f
    );
}

simd_float4x4 matrix::make_rotation(simd_float3 axis, float radians)
{
    axis = vector_normalize(axis);
    float ct = cosf(radians);
    float st = sinf(radians);
    float ci = 1.0f - ct;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return matrix_make_rows(
        ct + x * x * ci, x * y * ci - z * st, x * z * ci + y * st, 0,
        y * x * ci + z * st, ct + y * y * ci, y * z * ci - x * st, 0,
        z * x * ci - y * st, z * y * ci + x * st, ct + z * z * ci, 0,
        0, 0, 0, 1.0f
    );
}

simd_float4x4 matrix::make_scale(simd_float3 s)
{
    return matrix_make_rows(
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1.0f
    );
}

simd_float4x4 matrix::make_transform(simd_float3 t)
{
    return matrix_make_rows(
        1.0f, 0, 0, t.x,
        0, 1.0f, 0, t.y,
        0, 0, 1.0f, t.z,
        0, 0, 0, 1.0f
    );
}
