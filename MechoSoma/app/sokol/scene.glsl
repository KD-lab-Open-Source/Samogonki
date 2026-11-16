@vs scene_vs
layout(std140, binding = 0) uniform scene_vs_params {
    mat4 projection_matrix;
};

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 diffuse_in;
layout(location = 2) in vec4 specular_in;
layout(location = 3) in vec2 uv_in;

out vec4 diffuse_color;
out vec4 specular_color;
out vec2 uv;

void main() {
    gl_Position = projection_matrix * vec4(pos.x, pos.y, pos.z, 1.0f);
    diffuse_color = diffuse_in;
    specular_color = specular_in;
    uv = uv_in;
}
@end

@fs scene_fs
layout(std140, binding = 1) uniform scene_fs_params {
    int color_operation_1;
    int color_operation_2;
    int alpha_test_enabled;
    int alpha_reference;
};

in vec4 diffuse_color;
in vec4 specular_color;
in vec2 uv;

layout(binding = 0) uniform texture2D texture_1;
layout(binding = 1) uniform texture2D texture_2;
layout(binding = 0) uniform sampler sampler_1;

out vec4 result_color;

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

    // As Direct3D did with SPECULARENABLE: the specular colour is added after all the
    // texture stages, to RGB only, and saturated. The game writes a non-zero specular
    // only for materials with MAT_COLOR_ADD_SPECULAR (metal highlights, AddCol flashes).
    result_color.rgb = min(result_color.rgb + specular_color.rgb, 1.0f);

    if (alpha_test_enabled != 0 && (255.0f * result_color.a) < alpha_reference) {
        discard;
    }
}
@end

@program scene scene_vs scene_fs
