@vs scene_vs
layout(std140, binding = 0) uniform scene_vs_params {
    mat4 projection_matrix;
    mat4 view_to_shadow;   // camera space to the shadow map's clip space (dynamic shadows)
    mat4 view_to_world;    // camera space to world, for the terrain's static shadow cells
};

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 diffuse_in;
layout(location = 2) in vec4 specular_in;
layout(location = 3) in vec2 uv_in;

out vec4 diffuse_color;
out vec4 specular_color;
out vec2 uv;
out vec3 shadow_pos;
out vec2 world_xy;

void main() {
    vec4 view_pos = vec4(pos.x, pos.y, pos.z, 1.0f);
    gl_Position = projection_matrix * view_pos;
    diffuse_color = diffuse_in;
    specular_color = specular_in;
    uv = uv_in;
    shadow_pos = (view_to_shadow * view_pos).xyz * 0.5f + 0.5f;
    world_xy = (view_to_world * view_pos).xy;
}
@end

// A mesh kept on the GPU. It does per vertex what cPolyDispatcher::Draw(cMesh) does on
// the CPU (SetPointMesh): the same fixed-point lighting, the metal highlight and the
// terrain shadow test, so the colours match the CPU path. The fragment stage is
// scene_fs, shared with every other draw.
@vs mesh_vs
// Floats only, so GL gets the block as one vec4 array, set with a single call per draw.
// The last six hold whole numbers, which floats keep exactly; main() makes them integers.
layout(std140, binding = 0) uniform mesh_vs_params {
    mat4 projection_matrix;
    mat4 object_to_view;
    mat4 object_to_world;
    vec4 light;            // xyz: light direction in object space; w: cos(light, view)
    vec4 view;             // xyz: view direction in object space
    vec4 ambient;          // 0..255
    vec4 multiply;         // 0..255, alpha in w
    vec4 add;              // 0..255
    vec4 metal;            // 0..255
    vec4 flags;            // x: diffuse lighting, y: metal, z: terrain shadow, w: clamp near z
    vec4 terrain;          // x, y: world wrap masks, z: height shift, w: Z_SHADE_MAX
};

layout(binding = 2) uniform texture2D terrain_heights;
layout(binding = 3) uniform texture2D terrain_attributes;
layout(binding = 1) uniform sampler terrain_sampler;

layout(location = 0) in vec3 pos;
layout(location = 1) in ivec4 normal_in;   // the tile's char normal, -127..127
layout(location = 2) in vec2 uv_in;

out vec4 diffuse_color;
out vec4 specular_color;
out vec2 uv;
out vec3 shadow_pos;   // meshes take no dynamic shadows (scene_fs)
out vec2 world_xy;

// The C library's round(): halves away from zero.
int round_c(float x) {
    return int(x < 0.0f ? -floor(-x + 0.5f) : floor(x + 0.5f));
}

// GetAttributePoint(...) & ATTRIBUTE_SHADOW: the vertex is under the ground, or less
// than Z_SHADE_MAX above ground the terrain marks as shadowed (At_SHADOW).
bool terrain_shadow(vec3 world, ivec4 map) {
    ivec3 g = ivec3(round_c(world.x), round_c(world.y), round_c(world.z));
    ivec2 cell = ivec2((g.x & map.x) >> 1, (g.y & map.y) >> 1);
    int height = int(texelFetch(sampler2D(terrain_heights, terrain_sampler), cell, 0).r * 255.0f + 0.5f);
    int cell_flags = int(texelFetch(sampler2D(terrain_attributes, terrain_sampler), cell, 0).r * 255.0f + 0.5f);
    int altitude = height << map.z;
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

void main() {
    const ivec4 ambient_i = ivec4(ambient);
    const ivec4 multiply_i = ivec4(multiply);
    const ivec4 add_i = ivec4(add);
    const ivec4 metal_i = ivec4(metal);
    const ivec4 flags_i = ivec4(flags);
    const ivec4 terrain_i = ivec4(terrain);

    // cConvertor::ConvertPoint: with a perspective camera, a point within 1 of the eye
    // plane is put at z = 1.
    vec4 view_pos = object_to_view * vec4(pos, 1.0f);
    if (flags_i.w != 0 && abs(view_pos.z) <= 1.0f) {
        view_pos.z = 1.0f;
    }
    gl_Position = projection_matrix * view_pos;
    uv = uv_in;
    shadow_pos = vec3(0.0f);
    world_xy = vec2(0.0f);

    vec3 normal = vec3(normal_in.xyz);
    float cos_normal_light = normal.x * light.x + normal.y * light.y + normal.z * light.z;

    // cLight::CalcToneDiffuse, then the diffuse colour of SetPointMesh.
    ivec3 diffuse;
    if (flags_i.x != 0) {
        int tone = 127 + round_c(cos_normal_light);
        if (flags_i.z != 0 && terrain_shadow((object_to_world * vec4(pos, 1.0f)).xyz, terrain_i)) {
            tone >>= 1;
        }
        diffuse = min(((tone * multiply_i.rgb) >> 8) + ambient_i.rgb, ivec3(255));
    } else {
        diffuse = min(multiply_i.rgb + ambient_i.rgb, ivec3(255));
    }
    diffuse_color = vec4(vec3(clamp(diffuse, ivec3(0), ivec3(255))), float(multiply_i.a)) / 255.0f;

    // cLight::CalcToneMetal, then the specular colour of SetPointMesh.
    ivec3 specular = add_i.rgb;
    if (flags_i.y != 0) {
        float cos_normal_view = normal.x * view.x + normal.y * view.y + normal.z * view.z;
        float cos_reflect_view = 2.0f * (cos_normal_light * cos_normal_view) / (127.0f * 127.0f) - light.w;
        int tone = cos_reflect_view < 0.0f ? 0 : cosn_char(round_c(cos_reflect_view * 127.0f));
        specular = min(add_i.rgb + ((tone * metal_i.rgb) >> 7), ivec3(255));
    }
    specular_color = vec4(vec3(clamp(specular, ivec3(0), ivec3(255))), 255.0f) / 255.0f;
}
@end

@fs scene_fs
@image_sample_type shadow_map depth
@sampler_type shadow_sampler comparison

layout(std140, binding = 1) uniform scene_fs_params {
    int color_operation_1;
    int color_operation_2;
    int alpha_test_enabled;
    int alpha_reference;
};

// Dynamic shadows, in a block of their own so that both blocks stay one array in GL.
layout(std140, binding = 2) uniform scene_shadow_params {
    vec4 shadow;           // x: on, y: what a shadowed colour is multiplied by,
                           // z: filter step (shadow map texture coordinates), w: depth bias
    vec4 shadow_terrain;   // x, y: world wrap masks; z: skip the static shadow cells
};

in vec4 diffuse_color;
in vec4 specular_color;
in vec2 uv;
in vec3 shadow_pos;
in vec2 world_xy;

layout(binding = 0) uniform texture2D texture_1;
layout(binding = 1) uniform texture2D texture_2;
layout(binding = 4) uniform texture2D shadow_map;
layout(binding = 5) uniform texture2D shadow_cells;   // the terrain attribute map (M3D_TERRAIN)
layout(binding = 0) uniform sampler sampler_1;
layout(binding = 2) uniform sampler shadow_sampler;
layout(binding = 3) uniform sampler shadow_cells_sampler;

out vec4 result_color;

// How much of the dynamic shadow falls here: 0 lit, 1 shadowed. A caster nearer the light
// than this point covers it; 3 x 3 filtered taps soften the edge.
float dynamic_shadow() {
    if (shadow.x == 0.0f || any(lessThan(shadow_pos.xy, vec2(0.0f))) || any(greaterThan(shadow_pos.xy, vec2(1.0f)))) {
        return 0.0f;
    }
    // As cPolyDispatcher::BuildShade: terrain in static shadow (At_SHADOW) gets no darker.
    if (shadow_terrain.z != 0.0f) {
        ivec2 world = ivec2(floor(world_xy)) & ivec2(shadow_terrain.xy);
        int cell_flags = int(texelFetch(sampler2D(shadow_cells, shadow_cells_sampler), world >> 1, 0).r * 255.0f + 0.5f);
        if ((cell_flags & 0xC0) != 0) {
            return 0.0f;
        }
    }
    // Ground below every caster lies past the map's far plane: it counts as depth 1.
    float depth = min(shadow_pos.z, 1.0f) - shadow.w;
    float lit = 0.0f;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 tap = shadow_pos.xy + vec2(float(x), float(y)) * shadow.z;
            lit += texture(sampler2DShadow(shadow_map, shadow_sampler), vec3(tap, depth));
        }
    }
    return 1.0f - lit / 9.0f;
}

void main() {
    // s_repeat
    switch (color_operation_1) {
        case 0: /* Disable */ {
            result_color = diffuse_color;
        } break;
        case 1: /* Texture */ {
            result_color = texture(sampler2D(texture_1, sampler_1), uv);
        } break;
        case 2: /* Modulate */ {
            result_color = texture(sampler2D(texture_1, sampler_1), uv) * diffuse_color;
        } break;
        default: {
            result_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    // s_clamp
    if (color_operation_2 == 2 /* Modulate */) {
        result_color *= texture(sampler2D(texture_2, sampler_1), uv);
    }

    // Where the CPU path modulated by its shadow light map.
    result_color.rgb *= mix(1.0f, shadow.y, dynamic_shadow());

    // As Direct3D did with SPECULARENABLE: the specular colour is added after all the
    // texture stages, to RGB only, and saturated. The game writes a non-zero specular
    // only for materials with MAT_COLOR_ADD_SPECULAR (metal highlights, AddCol flashes).
    result_color.rgb = min(result_color.rgb + specular_color.rgb, 1.0f);

    if (alpha_test_enabled != 0 && (255.0f * result_color.a) < alpha_reference) {
        discard;
    }
}
@end

// A terrain tile kept on the GPU. It does per vertex what cPolyDispatcher::Draw(cTileMap)
// does on the CPU: bend the flat world into the planet around the camera, then move it
// to camera space. The fragment stage is scene_fs.
@vs tile_vs
layout(std140, binding = 0) uniform tile_vs_params {
    mat4 projection_matrix;
    mat4 tile_to_view;
    mat4 view_to_shadow;   // as scene_vs, for the dynamic shadows
    mat4 view_to_world;
    vec4 share;            // x, y: the tile's offset from the camera; z: planet radius;
                           // w: clamp near z
};

layout(location = 0) in vec3 pos;      // whole numbers, in tile coordinates
layout(location = 1) in vec4 color_in;
layout(location = 2) in vec2 uv_in;

out vec4 diffuse_color;
out vec4 specular_color;
out vec2 uv;
out vec3 shadow_pos;
out vec2 world_xy;

void main() {
    // SHARE_FLOAT(r2) = ShareFloat[r2 >> 5], the table InitArrayShare fills with
    // R - sqrt(R * R - 32 * index), and R where that would be negative.
    ivec2 r = ivec2(share.xy) + ivec2(pos.xy);
    int radius = int(share.z);
    int index = (r.x * r.x + r.y * r.y) >> 5;
    float bend = float(radius) - sqrt(float(max(radius * radius - index * 32, 0)));

    // cConvertor::ConvertPoint: with a perspective camera, a point within 1 of the eye
    // plane is put at z = 1.
    vec4 view_pos = tile_to_view * vec4(pos.xy, pos.z - bend, 1.0f);
    if (share.w != 0.0f && abs(view_pos.z) <= 1.0f) {
        view_pos.z = 1.0f;
    }
    gl_Position = projection_matrix * view_pos;
    diffuse_color = color_in;
    specular_color = vec4(0.0f);
    uv = uv_in;
    shadow_pos = (view_to_shadow * view_pos).xyz * 0.5f + 0.5f;
    world_xy = (view_to_world * view_pos).xy;
}
@end

// A shadow caster: a GPU mesh drawn into a camera's shadow map, depth only. Tiles with a
// texture mask (ATTRMAT_MASK) cast through it, as cMesh::DrawShadeDynamic does.
@vs shadow_vs
layout(std140, binding = 0) uniform shadow_vs_params {
    mat4 object_to_shadow;
};

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv_in;

out vec2 uv;

void main() {
    gl_Position = object_to_shadow * vec4(pos, 1.0f);
    uv = uv_in;
}
@end

@fs shadow_fs
layout(std140, binding = 1) uniform shadow_fs_params {
    vec4 alpha_test;   // x: on, y: reference, 0..255
};

in vec2 uv;

layout(binding = 0) uniform texture2D texture_1;
layout(binding = 0) uniform sampler sampler_1;

void main() {
    if (alpha_test.x != 0.0f && 255.0f * texture(sampler2D(texture_1, sampler_1), uv).a < alpha_test.y) {
        discard;
    }
}
@end

@program scene scene_vs scene_fs

@program mesh mesh_vs scene_fs

@program tile tile_vs scene_fs

@program shadow shadow_vs shadow_fs
