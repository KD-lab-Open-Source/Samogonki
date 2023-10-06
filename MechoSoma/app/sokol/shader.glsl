// Scene program

@vs scene_vs
uniform scene_vs_params {
    mat4 projection_matrix;
};

in vec3 pos;
in vec4 color0;
in vec2 uv0;

out vec4 color;
out vec2 uv;

void main() {
    gl_Position = projection_matrix * vec4(pos.x, pos.y, pos.z, 1.0f);
    color = color0;
    uv = uv0;
}
@end

@fs scene_fs
uniform scene_fs_params {
    int color_operation_1;
    int color_operation_2;
    int alpha_test_enabled;
    int alpha_reference;
};

in vec4 color;
in vec2 uv;

uniform texture2D texture_1;
uniform texture2D texture_2;

uniform sampler sampler_1;

out vec4 result_color;
void main() {
    // s_repeat
    switch (color_operation_1) {
        case 0: /* Disable */ {
            result_color = color;
        } break;
        case 1: /* Texture */ {
            result_color = texture(sampler2D(texture_1, sampler_1), uv);
        } break;
        case 2: /* Modulate */ {
            result_color = texture(sampler2D(texture_1, sampler_1), uv) * color;
        } break;
        default: {
            result_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    // s_clamp
    if (color_operation_2 == 2 /* Modulate */) {
        result_color *= texture(sampler2D(texture_2, sampler_1), uv);
    }

    if (alpha_test_enabled != 0 && (255.0f * result_color.a) < alpha_reference) {
        discard;
    }
}
@end

@program scene scene_vs scene_fs

// Quad vertex shader

@vs quad_vs

out vec2 uv;

vec4 triangle_position(int vertex_index) {
    float x = -1.0f + float((vertex_index & 1) << 2);
    float y = -1.0f + float((vertex_index & 2) << 1);
    return vec4(x, y, 0.0f, 1.0f);
}

vec2 triangle_uv(int vertex_index) {
    float u = (vertex_index == 1) ? 2.0f : 0.0f;
    float v = (vertex_index == 2) ? 2.0f : 0.0f;
    return vec2(u, v);
}

void main() {
    gl_Position = triangle_position(gl_VertexIndex);
    uv = triangle_uv(gl_VertexIndex);
}
@end

// Back Buffer program

@fs back_buffer_fs

in vec2 uv;

uniform utexture2D input_texture;
uniform sampler sampler_1;

vec4 from_rgb565(uint color) {
    const uint red_mask = 0xF800; // 0b1111100000000000
    const uint green_mask = 0x7E0; // 0b0000011111100000
    const uint blue_mask = 0x1F; // 0b0000000000011111

    uint r = (color & red_mask) >> 11;
    uint g = (color & green_mask) >> 5;
    uint b = color & blue_mask;

    vec4 result;
    result[0] = ((r * 527 + 23) >> 6) / 255.0f;
    result[1] = ((g * 259 + 33) >> 6) / 255.0f;
    result[2] = ((b * 527 + 23) >> 6) / 255.0f;
    result[3] = 1.0f;

    return result;
}

out vec4 result_color;
void main() {
    // DirectX vertical axis flip
    uint color = texture(usampler2D(input_texture, sampler_1), vec2(uv.x, 1.0f - uv.y)).r;
    result_color = from_rgb565(color);
}

@end

@program back_buffer quad_vs back_buffer_fs

// Flush program

@vs flush_vs
uniform flush_vs_params {
    float max_x;
};

out vec2 uv;
void main() {
    uv.x = float(((uint(gl_VertexIndex) + 2u) / 3u) % 2u);
    uv.y = float(((uint(gl_VertexIndex) + 1u) / 3u) % 2u);
    gl_Position = vec4(clamp(-1.0f + 2.0f * uv.x, -max_x, max_x), -1.0f + 2.0f * uv.y, 0, 1.0f);
}

@end

@fs flush_fs

in vec2 uv;

uniform texture2D offscreen_texture;
uniform sampler sampler_1;

out vec4 result_color;
void main() {
    result_color = texture(sampler2D(offscreen_texture, sampler_1), uv);
}

@end

@program flush flush_vs flush_fs
