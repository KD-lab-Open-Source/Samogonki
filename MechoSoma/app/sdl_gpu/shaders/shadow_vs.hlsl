// A shadow caster: a GPU mesh drawn into a camera's shadow map, depth only. The fragment
// stage is shadow_fs.
#include "common.hlsl"

cbuffer shadow_vs_params : register(b0, space1) {
    column_major float4x4 object_to_shadow;
};

struct Input {
    float3 pos : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

TexturedVaryings main(Input input) {
    TexturedVaryings output;
    output.position = to_gpu_clip_space(mul(object_to_shadow, float4(input.pos, 1.0f)));
    output.uv = input.uv;
    return output;
}
