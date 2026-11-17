// Frame geometry built on the CPU (M3D_DRAW_COMMAND): already in camera space and lit
// by the game. The fragment stage is scene_fs.
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
