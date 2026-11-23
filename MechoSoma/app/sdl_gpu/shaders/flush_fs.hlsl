// The offscreen buffer drawn to the window, with the game's gamma option applied, as the
// original's hardware gamma ramp did to the whole screen. Port of flush_fs in
// sokol/flush.glsl.
#include "common.hlsl"

cbuffer flush_fs_params : register(b0, space3) {
    float4 gamma;   // x, y, z: each channel's exponent, 1 / its gamma; w: 0 when all are 1
};

Texture2D<float4> texture_1 : register(t0, space2);
SamplerState sampler_1 : register(s0, space2);

float4 main(TexturedVaryings input) : SV_Target0 {
    float4 color = texture_1.Sample(sampler_1, input.uv);
    // Skipped at gamma 1, so the default leaves every pixel exactly as it was.
    if (gamma.w != 0.0f) {
        color.rgb = pow(color.rgb, gamma.xyz);
    }
    return color;
}
