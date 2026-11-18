// What the SDL GPU shaders share: the outputs each vertex stage hands its fragment stage,
// and SDL GPU's differences from OpenGL, whose matrices the game builds.
//
// Conventions every shader here follows (SDL_CreateGPUShader documents them):
//   - Vertex inputs are TEXCOORD<location>. SDL GPU binds attributes by location only,
//     and its Direct3D 12 backend names every input element TEXCOORD, so any other
//     semantic fails pipeline creation there.
//   - Vertex stage: textures and samplers in (t#, space0) and (s#, space0), uniform
//     buffers in (b#, space1).
//   - Fragment stage: textures and samplers in (t#, space2) and (s#, space2), uniform
//     buffers in (b#, space3). Texture n is bound with sampler n.
//   - A texture read texel by texel (Load) is bound like the others, with a sampler the
//     shader doesn't use: every backend keeps the register number (in MSL, texture(n)).
//     shadercross's reflection lists such a texture as a storage texture; it isn't one.
//   - Matrices are column-major, as GLSL's and as the renderer stores them, so
//     mul(matrix, vector) is GLSL's matrix * vector.
//   - The entry point is main. (SPIRV-Cross renames it main0 in Metal.)
#ifndef COMMON_HLSL
#define COMMON_HLSL

// The game's projections map z to -w..w, as OpenGL does; SDL GPU, like Direct3D, Metal
// and Vulkan, clips z to 0..w. The depth this writes is the one OpenGL writes.
float4 to_gpu_clip_space(float4 position) {
    position.z = 0.5f * (position.z + position.w);
    return position;
}

// A point in the shadow camera's clip space, as texture coordinates and depth in its
// shadow map. Row 0 of a render target holds the top of its clip space (y = +1), so
// unlike OpenGL, v grows downwards. The depth matches to_gpu_clip_space's.
float3 shadow_map_position(float4 shadow_clip) {
    return float3(0.5f + 0.5f * shadow_clip.x, 0.5f - 0.5f * shadow_clip.y, 0.5f + 0.5f * shadow_clip.z);
}

// The outputs of every scene vertex stage (scene_vs, mesh_vs, tile_vs) and the inputs of
// scene_fs. Linked by location in SPIR-V and MSL, so all of them use this one struct.
struct SceneVaryings {
    float4 position : SV_Position;
    float4 diffuse_color : TEXCOORD0;
    float4 specular_color : TEXCOORD1;
    float2 uv : TEXCOORD2;
    float3 shadow_pos : TEXCOORD3;   // shadow map texture coordinates and depth
    float2 world_xy : TEXCOORD4;     // for the terrain's static shadow cells
};

// The outputs of the other vertex stages: a position and texture coordinates.
struct TexturedVaryings {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

#endif
