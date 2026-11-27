// Frame geometry built on the CPU (M3D_DRAW_COMMAND): already in camera space and lit
// by the game. The fragment stage is scene_fs.
#include "common.hlsl"

cbuffer scene_vs_params : register(b0, space1) {
    column_major float4x4 projection_matrix;
    column_major float4x4 view_to_shadow;   // camera space to the shadow map's clip space
    column_major float4x4 view_to_world;    // camera space to world, for the static shadow cells
};

struct Input {
    float3 pos : TEXCOORD0;
    float4 diffuse : TEXCOORD1;
    float4 specular : TEXCOORD2;
    float2 uv : TEXCOORD3;
};

SceneVaryings main(Input input) {
    const float4 view_pos = float4(input.pos, 1.0f);

    SceneVaryings output;
    output.position = to_gpu_clip_space(mul(projection_matrix, view_pos));
    output.diffuse_color = input.diffuse;
    output.specular_color = input.specular;
    output.uv = input.uv;
    output.shadow_pos = shadow_map_position(mul(view_to_shadow, view_pos));
    output.world_xy = mul(view_to_world, view_pos).xy;
    return output;
}
