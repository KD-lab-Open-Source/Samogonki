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

// The game's gamma option, as the original's hardware gamma ramp applied it to the whole
// screen.
@fs flush_fs
layout(std140, binding = 3) uniform flush_fs_params {
    vec4 gamma;   // x, y, z: each channel's exponent, 1 / its gamma; w: 0 when all are 1
};

in vec2 uv;

layout(binding = 0) uniform texture2D texture_1;
layout(binding = 0) uniform sampler sampler_1;

out vec4 result_color;

void main() {
    result_color = texture(sampler2D(texture_1, sampler_1), uv);
    // Skipped at gamma 1, so the default leaves every pixel exactly as it was.
    if (gamma.w != 0.0f) {
        result_color.rgb = pow(result_color.rgb, gamma.xyz);
    }
}
@end

@program flush flush_vs flush_fs
