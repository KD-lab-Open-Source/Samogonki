// The fragment stage every scene draw shares: frame geometry, GPU meshes and GPU tiles.
// The texture stages as the game set them up for Direct3D, the dynamic shadows, and the
// specular colour added last. Port of sokol/scene_fs.glsl.
#include "common.hlsl"

cbuffer scene_fs_params : register(b0, space3) {
    int color_operation_1;
    int color_operation_2;
    int alpha_test_enabled;
    int alpha_reference;
};

// Dynamic shadows.
cbuffer scene_shadow_params : register(b1, space3) {
    float4 shadow;           // x: on, y: what a shadowed colour is multiplied by,
                             // z: filter step (shadow map texture coordinates), w: depth bias
    float4 shadow_terrain;   // x, y: world wrap masks; z: skip the static shadow cells
};

Texture2D<float4> texture_1 : register(t0, space2);
Texture2D<float4> texture_2 : register(t1, space2);
Texture2D<float> shadow_map : register(t2, space2);
Texture2D<float4> shadow_cells : register(t3, space2);   // the terrain attribute map (M3D_TERRAIN)
SamplerState sampler_1 : register(s0, space2);
SamplerState sampler_2 : register(s1, space2);
SamplerComparisonState shadow_sampler : register(s2, space2);
SamplerState shadow_cells_sampler : register(s3, space2);   // unused: the cells are read by texel

// How much of the dynamic shadow falls here: 0 lit, 1 shadowed. A caster nearer the light
// than this point covers it; 3 x 3 filtered taps soften the edge.
float dynamic_shadow(float3 shadow_pos, float2 world_xy) {
    if (shadow.x == 0.0f || any(shadow_pos.xy < 0.0f) || any(shadow_pos.xy > 1.0f)) {
        return 0.0f;
    }
    // As cPolyDispatcher::BuildShade: terrain in static shadow (At_SHADOW) gets no darker.
    if (shadow_terrain.z != 0.0f) {
        const int2 world = int2(floor(world_xy)) & int2(shadow_terrain.xy);
        const int cell_flags = int(shadow_cells.Load(int3(world >> 1, 0)).r * 255.0f + 0.5f);
        if ((cell_flags & 0xC0) != 0) {
            return 0.0f;
        }
    }
    // Ground below every caster lies past the map's far plane: it counts as depth 1.
    const float depth = min(shadow_pos.z, 1.0f) - shadow.w;
    float lit = 0.0f;
    [unroll] for (int y = -1; y <= 1; y++) {
        [unroll] for (int x = -1; x <= 1; x++) {
            const float2 tap = shadow_pos.xy + float2(x, y) * shadow.z;
            lit += shadow_map.SampleCmpLevelZero(shadow_sampler, tap, depth);
        }
    }
    return 1.0f - lit / 9.0f;
}

float4 main(SceneVaryings input) : SV_Target0 {
    float4 result_color;

    // s_repeat
    switch (color_operation_1) {
        case 0: /* Disable */
            result_color = input.diffuse_color;
            break;
        case 1: /* Texture */
            result_color = texture_1.Sample(sampler_1, input.uv);
            break;
        case 2: /* Modulate */
            result_color = texture_1.Sample(sampler_1, input.uv) * input.diffuse_color;
            break;
        default:
            result_color = float4(0.0f, 0.0f, 0.0f, 0.0f);
            break;
    }

    // s_clamp
    if (color_operation_2 == 2 /* Modulate */) {
        result_color *= texture_2.Sample(sampler_2, input.uv);
    }

    // Where the CPU path modulated by its shadow light map.
    result_color.rgb *= lerp(1.0f, shadow.y, dynamic_shadow(input.shadow_pos, input.world_xy));

    // As Direct3D did with SPECULARENABLE: the specular colour is added after all the
    // texture stages, to RGB only, and saturated. The game writes a non-zero specular
    // only for materials with MAT_COLOR_ADD_SPECULAR (metal highlights, AddCol flashes).
    result_color.rgb = min(result_color.rgb + input.specular_color.rgb, 1.0f);

    if (alpha_test_enabled != 0 && 255.0f * result_color.a < float(alpha_reference)) {
        discard;
    }
    return result_color;
}
