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

@fs back_buffer_fs

in vec2 uv;

layout(binding = 0) uniform utexture2D texture_1;
@sampler_type sampler_1 nonfiltering
layout(binding = 0) uniform sampler sampler_1;

out vec4 result_color;

vec4 from_rgb565(uint color) {
    const uint red_mask = 0xF800U; // 0b1111100000000000
    const uint green_mask = 0x7E0U; // 0b0000011111100000
    const uint blue_mask = 0x1FU; // 0b0000000000011111

    uint r = (color & red_mask) >> 11;
    uint g = (color & green_mask) >> 5;
    uint b = color & blue_mask;

    vec4 result;
    result.r = ((r * 527U + 23U) >> 6) / 255.0f;
    result.g = ((g * 259U + 33U) >> 6) / 255.0f;
    result.b = ((b * 527U + 23U) >> 6) / 255.0f;
    result.a = 1.0f;

    return result;
}

void main() {
    // DirectX vertical axis flip
    uint color = texture(usampler2D(texture_1, sampler_1), vec2(uv.x, 1.0f - uv.y)).r;
    result_color = from_rgb565(color);
}
@end

@program back_buffer quad_vs back_buffer_fs
