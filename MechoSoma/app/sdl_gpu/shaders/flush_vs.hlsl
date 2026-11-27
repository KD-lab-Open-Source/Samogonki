// The offscreen buffer drawn to the window: two triangles, six vertices and no vertex
// buffer, narrowed to max_x to keep the game's aspect ratio. The fragment stage is
// flush_fs.
#include "common.hlsl"

cbuffer flush_vs_params : register(b0, space1) {
    float max_x;
};

TexturedVaryings main(uint vertex_index : SV_VertexID) {
    const float u = float(((vertex_index + 2u) / 3u) % 2u);
    const float v = float(((vertex_index + 1u) / 3u) % 2u);

    TexturedVaryings output;
    output.position = float4(clamp(-1.0f + 2.0f * u, -max_x, max_x), -1.0f + 2.0f * v, 0.0f, 1.0f);
    // The offscreen buffer is a render target, whose row 0 is the top: unlike OpenGL,
    // the bottom of the screen (v = 0 here) is at the bottom of the texture.
    output.uv = float2(u, 1.0f - v);
    return output;
}
