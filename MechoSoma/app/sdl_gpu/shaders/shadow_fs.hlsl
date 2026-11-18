// A shadow caster's fragment stage: depth only. Tiles with a texture mask (ATTRMAT_MASK)
// cast through it, as cMesh::DrawShadeDynamic does. Port of shadow_fs in
// sokol/shadow.glsl.
#include "common.hlsl"

cbuffer shadow_fs_params : register(b0, space3) {
    float4 alpha_test;   // x: on, y: reference, 0..255
};

Texture2D<float4> texture_1 : register(t0, space2);
SamplerState sampler_1 : register(s0, space2);

void main(TexturedVaryings input) {
    if (alpha_test.x != 0.0f && 255.0f * texture_1.Sample(sampler_1, input.uv).a < alpha_test.y) {
        discard;
    }
}
