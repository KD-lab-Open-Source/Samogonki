// The offscreen buffer drawn to the window. Port of flush_fs in sokol/flush.glsl.
#include "common.hlsl"

Texture2D<float4> texture_1 : register(t0, space2);
SamplerState sampler_1 : register(s0, space2);

float4 main(TexturedVaryings input) : SV_Target0 {
    return texture_1.Sample(sampler_1, input.uv);
}
