// The scene shaders, compiled into one header:
//   sokol-shdc -l glsl410 -i MechoSoma/app/sokol/scene.glsl -o MechoSoma/app/sokol-scene-shader.h
//
// Every program but the shadow pass ends in scene_fs, so every vertex stage writes the
// outputs it reads: diffuse_color, specular_color, uv, shadow_pos and world_xy. The web
// build uses its own header, sokol-shader-em.h, which has only the scene program.

@include scene_vs.glsl
@include mesh_vs.glsl
@include scene_fs.glsl
@include tile_vs.glsl
@include shadow.glsl

// CPU-built frame geometry (M3D_DRAW_COMMAND).
@program scene scene_vs scene_fs

// Meshes kept on the GPU (CreateMesh, DrawMesh).
@program mesh mesh_vs scene_fs

// Terrain tiles kept on the GPU (CreateTiles, DrawTile).
@program tile tile_vs scene_fs

// Dynamic shadow casters, depth only (SetShadowCamera).
@program shadow shadow_vs shadow_fs
