// A mesh kept on the GPU. It does per vertex what cPolyDispatcher::Draw(cMesh) does on
// the CPU (SetPointMesh): the same fixed-point lighting, the metal highlight and the
// terrain shadow test, so the colours match the CPU path. The fragment stage is
// scene_fs. Port of sokol/mesh_vs.glsl.
#include "common.hlsl"

// The last six vectors hold whole numbers, which floats keep exactly; main() makes them
// integers.
cbuffer mesh_vs_params : register(b0, space1) {
    column_major float4x4 projection_matrix;
    column_major float4x4 object_to_view;
    column_major float4x4 object_to_world;
    float4 light;      // xyz: light direction in object space; w: cos(light, view)
    float4 view;       // xyz: view direction in object space
    float4 ambient;    // 0..255
    float4 multiply;   // 0..255, alpha in w
    float4 add;        // 0..255
    float4 metal;      // 0..255
    float4 flags;      // x: diffuse lighting, y: metal, z: terrain shadow, w: clamp near z
    float4 terrain;    // x, y: world wrap masks, z: height shift, w: Z_SHADE_MAX
};

// The terrain's height and attribute maps (M3D_TERRAIN), read texel by texel. The
// samplers aren't used, but SDL GPU binds every texture with one.
Texture2D<float4> terrain_heights : register(t0, space0);
Texture2D<float4> terrain_attributes : register(t1, space0);
SamplerState terrain_heights_sampler : register(s0, space0);
SamplerState terrain_attributes_sampler : register(s1, space0);

struct Input {
    float3 pos : TEXCOORD0;
    int4 normal : TEXCOORD1;   // the tile's char normal, -127..127
    float2 uv : TEXCOORD2;
};

// The C library's round(): halves away from zero.
int round_c(float x) {
    return int(x < 0.0f ? -floor(-x + 0.5f) : floor(x + 0.5f));
}

// GetAttributePoint(...) & ATTRIBUTE_SHADOW: the vertex is under the ground, or less
// than Z_SHADE_MAX above ground the terrain marks as shadowed (At_SHADOW).
bool terrain_shadow(float3 world, int4 map) {
    const int3 g = int3(round_c(world.x), round_c(world.y), round_c(world.z));
    const int2 cell = int2((g.x & map.x) >> 1, (g.y & map.y) >> 1);
    const int height = int(terrain_heights.Load(int3(cell, 0)).r * 255.0f + 0.5f);
    const int cell_flags = int(terrain_attributes.Load(int3(cell, 0)).r * 255.0f + 0.5f);
    const int altitude = height << map.z;
    return ((cell_flags & 0xC0) != 0 && altitude + map.w > g.z) || altitude > g.z;
}

// COSN_CHAR: cos to the 8th power in 1/128 steps, as Maths.cpp builds the table.
int cosn_char(int j) {
    int i = j;
    for (int k = 0; k < 7; k++) {
        i = (i * j) >> 7;
    }
    return min(i, 127) << 1;
}

SceneVaryings main(Input input) {
    const int4 ambient_i = int4(ambient);
    const int4 multiply_i = int4(multiply);
    const int4 add_i = int4(add);
    const int4 metal_i = int4(metal);
    const int4 flags_i = int4(flags);
    const int4 terrain_i = int4(terrain);

    SceneVaryings output;

    // cConvertor::ConvertPoint: with a perspective camera, a point within 1 of the eye
    // plane is put at z = 1.
    float4 view_pos = mul(object_to_view, float4(input.pos, 1.0f));
    if (flags_i.w != 0 && abs(view_pos.z) <= 1.0f) {
        view_pos.z = 1.0f;
    }
    output.position = to_gpu_clip_space(mul(projection_matrix, view_pos));
    output.uv = input.uv;
    output.shadow_pos = float3(0.0f, 0.0f, 0.0f);   // meshes take no dynamic shadows (scene_fs)
    output.world_xy = float2(0.0f, 0.0f);

    const float3 normal = float3(input.normal.xyz);
    const float cos_normal_light = normal.x * light.x + normal.y * light.y + normal.z * light.z;

    // cLight::CalcToneDiffuse, then the diffuse colour of SetPointMesh.
    int3 diffuse;
    if (flags_i.x != 0) {
        int tone = 127 + round_c(cos_normal_light);
        if (flags_i.z != 0 && terrain_shadow(mul(object_to_world, float4(input.pos, 1.0f)).xyz, terrain_i)) {
            tone >>= 1;
        }
        diffuse = min(((tone * multiply_i.rgb) >> 8) + ambient_i.rgb, 255);
    } else {
        diffuse = min(multiply_i.rgb + ambient_i.rgb, 255);
    }
    output.diffuse_color = float4(float3(clamp(diffuse, 0, 255)), float(multiply_i.a)) / 255.0f;

    // cLight::CalcToneMetal, then the specular colour of SetPointMesh.
    int3 specular = add_i.rgb;
    if (flags_i.y != 0) {
        const float cos_normal_view = normal.x * view.x + normal.y * view.y + normal.z * view.z;
        const float cos_reflect_view = 2.0f * (cos_normal_light * cos_normal_view) / (127.0f * 127.0f) - light.w;
        const int tone = cos_reflect_view < 0.0f ? 0 : cosn_char(round_c(cos_reflect_view * 127.0f));
        specular = min(add_i.rgb + ((tone * metal_i.rgb) >> 7), 255);
    }
    output.specular_color = float4(float3(clamp(specular, 0, 255)), 255.0f) / 255.0f;
    return output;
}
