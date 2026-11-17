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
