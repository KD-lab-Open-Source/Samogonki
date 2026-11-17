// A terrain tile kept on the GPU. It does per vertex what cPolyDispatcher::Draw(cTileMap)
// does on the CPU: bend the flat world into the planet around the camera, then move it
// to camera space. The fragment stage is scene_fs.
@vs tile_vs
layout(std140, binding = 0) uniform tile_vs_params {
    mat4 projection_matrix;
    mat4 tile_to_view;
    mat4 view_to_shadow;   // as scene_vs, for the dynamic shadows
    mat4 view_to_world;
    vec4 share;            // x, y: the tile's offset from the camera; z: planet radius;
                           // w: clamp near z
};

layout(location = 0) in vec3 pos;      // whole numbers, in tile coordinates
layout(location = 1) in vec4 color_in;
layout(location = 2) in vec2 uv_in;

out vec4 diffuse_color;
out vec4 specular_color;
out vec2 uv;
out vec3 shadow_pos;
out vec2 world_xy;

void main() {
    // SHARE_FLOAT(r2) = ShareFloat[r2 >> 5], the table InitArrayShare fills with
    // R - sqrt(R * R - 32 * index), and R where that would be negative.
    ivec2 r = ivec2(share.xy) + ivec2(pos.xy);
    int radius = int(share.z);
    int index = (r.x * r.x + r.y * r.y) >> 5;
    float bend = float(radius) - sqrt(float(max(radius * radius - index * 32, 0)));

    // cConvertor::ConvertPoint: with a perspective camera, a point within 1 of the eye
    // plane is put at z = 1.
    vec4 view_pos = tile_to_view * vec4(pos.xy, pos.z - bend, 1.0f);
    if (share.w != 0.0f && abs(view_pos.z) <= 1.0f) {
        view_pos.z = 1.0f;
    }
    gl_Position = projection_matrix * view_pos;
    diffuse_color = color_in;
    specular_color = vec4(0.0f);
    uv = uv_in;
    shadow_pos = (view_to_shadow * view_pos).xyz * 0.5f + 0.5f;
    world_xy = (view_to_world * view_pos).xy;
}
@end
