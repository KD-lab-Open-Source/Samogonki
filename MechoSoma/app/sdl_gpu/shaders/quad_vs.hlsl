// One triangle covering the target, for the 2D back buffer (back_buffer_fs). Drawn with
// three vertices and no vertex buffer. Port of quad_vs in sokol/back_buffer.glsl.
#include "common.hlsl"

TexturedVaryings main(uint vertex_index : SV_VertexID) {
    TexturedVaryings output;
    const float x = -1.0f + float((vertex_index & 1u) << 2);
    const float y = -1.0f + float((vertex_index & 2u) << 1);
    output.position = float4(x, y, 0.0f, 1.0f);
    output.uv = float2(vertex_index == 1u ? 2.0f : 0.0f, vertex_index == 2u ? 2.0f : 0.0f);
    return output;
}
