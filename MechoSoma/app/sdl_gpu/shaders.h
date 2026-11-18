#pragma once

#include <SDL3/SDL_gpu.h>

namespace graphics::sdl_gpu {

// The SDL GPU renderer's shaders. The HLSL in sdl_gpu/shaders is compiled at build time
// to the one format this platform's SDL GPU backend loads, and embedded in the program
// (see the shader toolchain in the top-level CMakeLists.txt).
enum class Shader {
  scene_vs,        // CPU-built frame geometry
  mesh_vs,         // meshes kept on the GPU
  tile_vs,         // terrain tiles kept on the GPU
  scene_fs,        // the fragment stage of the three above
  shadow_vs,       // shadow casters, depth only
  shadow_fs,
  quad_vs,         // the 2D back buffer
  back_buffer_fs,
  flush_vs,        // the offscreen buffer drawn to the window
  flush_fs,
  count
};

// The format the shaders are compiled to. Create the device with only this one, so SDL
// picks a backend there is code for: Direct3D 12, Metal or Vulkan.
#if defined(_WIN32)
inline constexpr SDL_GPUShaderFormat shader_format = SDL_GPU_SHADERFORMAT_DXIL;
#elif defined(__APPLE__)
inline constexpr SDL_GPUShaderFormat shader_format = SDL_GPU_SHADERFORMAT_MSL;
#else
inline constexpr SDL_GPUShaderFormat shader_format = SDL_GPU_SHADERFORMAT_SPIRV;
#endif

// Null on failure; SDL_GetError() says why.
SDL_GPUShader* create_shader(SDL_GPUDevice* device, Shader shader);

}  // namespace graphics::sdl_gpu
