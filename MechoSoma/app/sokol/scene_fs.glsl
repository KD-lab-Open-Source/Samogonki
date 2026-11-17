// The fragment stage every scene draw shares: frame geometry, GPU meshes and GPU tiles.
// The texture stages as the game set them up for Direct3D, the dynamic shadows, and the
// specular colour added last.
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
