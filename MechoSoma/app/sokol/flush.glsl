@vs flush_vs
layout(std140, binding = 2) uniform flush_vs_params {
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

layout(binding = 0) uniform texture2D texture_1;
layout(binding = 0) uniform sampler sampler_1;

out vec4 result_color;

void main() {
    result_color = texture(sampler2D(texture_1, sampler_1), uv);
}
@end

@program flush flush_vs flush_fs
