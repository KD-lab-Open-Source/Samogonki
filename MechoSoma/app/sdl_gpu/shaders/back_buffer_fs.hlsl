// The game's 2D back buffer: 16-bit RGB 565 pixels, uploaded as an unsigned integer
// texture and converted here. Port of back_buffer_fs in sokol/back_buffer.glsl.
#include "common.hlsl"

Texture2D<uint> texture_1 : register(t0, space2);
SamplerState sampler_1 : register(s0, space2);   // unused: an integer texture is read by texel

float4 from_rgb565(uint color) {
    const uint r = (color & 0xF800u) >> 11;
    const uint g = (color & 0x07E0u) >> 5;
    const uint b = color & 0x001Fu;
    return float4(((r * 527u + 23u) >> 6) / 255.0f,
                  ((g * 259u + 33u) >> 6) / 255.0f,
                  ((b * 527u + 23u) >> 6) / 255.0f,
                  1.0f);
}

float4 main(TexturedVaryings input) : SV_Target0 {
    // The buffer's first row is the top of the screen, and v = 0 at the bottom here, as in
    // the GL shader (DirectX's vertical axis). Nearest texel, as its non-filtering sampler.
    uint width;
    uint height;
    texture_1.GetDimensions(width, height);
    const float2 uv = float2(input.uv.x, 1.0f - input.uv.y);
    const int2 texel = clamp(int2(floor(uv * float2(width, height))), int2(0, 0), int2(width, height) - 1);
    return from_rgb565(texture_1.Load(int3(texel, 0)));
}
