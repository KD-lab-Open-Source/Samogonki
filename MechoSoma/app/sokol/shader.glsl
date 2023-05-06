// Scene program

@vs scene_vs
uniform vs_params {
    mat4 projection_matrix;
};

in vec3 pos;
in vec4 color0;
in vec2 uv0;

out vec4 color;
out vec2 uv;

void main() {
    gl_Position = projection_matrix * vec4(pos, 1.0f);
    color = color0;
    uv = uv0;
}
@end

@fs scene_fs
uniform fs_params {
    int color_operation_1;
    int color_operation_2;
};

in vec4 color;
in vec2 uv;

uniform sampler2D texture_1;
uniform sampler2D texture_2;

out vec4 result_color;
void main() {
    // s_repeat
    switch (color_operation_1) {
        case 0: /* Disable */ {
            result_color = color;
        } break;
        case 1: /* Texture */ {
            result_color = texture(texture_1, uv);
        } break;
        case 2: /* Modulate */ {
            result_color = texture(texture_1, uv) * color;
        } break;
        default: {
            result_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    // s_clamp
    if (color_operation_2 == 2 /* Modulate */) {
        result_color *= texture(texture_2, uv);
    }
}
@end

@program scene scene_vs scene_fs

// Back Buffer program

@vs back_buffer_vs

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

@fs back_buffer_fs

in vec2 uv;

uniform usampler2D input_texture;

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
    uint color = texture(input_texture, vec2(uv.x, 1.0f - uv.y)).r;
    result_color = from_rgb565(color);
}

@end

@program back_buffer back_buffer_vs back_buffer_fs
