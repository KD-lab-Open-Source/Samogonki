@vs vs
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

@fs fs
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

@program samogonki vs fs