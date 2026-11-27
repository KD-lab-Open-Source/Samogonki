// A terrain tile kept on the GPU. It does per vertex what cPolyDispatcher::Draw(cTileMap)
// does on the CPU: bend the flat world into the planet around the camera, then move it
// to camera space. The fragment stage is scene_fs.
#include "common.hlsl"

cbuffer tile_vs_params : register(b0, space1) {
    column_major float4x4 projection_matrix;
    column_major float4x4 tile_to_view;
    column_major float4x4 view_to_shadow;   // as scene_vs, for the dynamic shadows
    column_major float4x4 view_to_world;
    float4 share;   // x, y: the tile's offset from the camera; z: planet radius; w: clamp near z
};

struct Input {
    float3 pos : TEXCOORD0;   // whole numbers, in tile coordinates
    float4 color : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

SceneVaryings main(Input input) {
    // SHARE_FLOAT(r2) = ShareFloat[r2 >> 5], the table InitArrayShare fills with
    // R - sqrt(R * R - 32 * index), and R where that would be negative.
    const int2 r = int2(share.xy) + int2(input.pos.xy);
    const int radius = int(share.z);
    const int index = (r.x * r.x + r.y * r.y) >> 5;
    const float bend = float(radius) - sqrt(float(max(radius * radius - index * 32, 0)));

    // cConvertor::ConvertPoint: with a perspective camera, a point within 1 of the eye
    // plane is put at z = 1.
    float4 view_pos = mul(tile_to_view, float4(input.pos.xy, input.pos.z - bend, 1.0f));
    if (share.w != 0.0f && abs(view_pos.z) <= 1.0f) {
        view_pos.z = 1.0f;
    }

    SceneVaryings output;
    output.position = to_gpu_clip_space(mul(projection_matrix, view_pos));
    output.diffuse_color = input.color;
    output.specular_color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.uv = input.uv;
    output.shadow_pos = shadow_map_position(mul(view_to_shadow, view_pos));
    output.world_xy = mul(view_to_world, view_pos).xy;
    return output;
}
