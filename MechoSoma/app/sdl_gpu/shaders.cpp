#include "sdl_gpu/shaders.h"

#include <iterator>

// Generated at build time: a <name>_code[] / <name>_size pair per shader.
#include "sdl_gpu_shaders.h"

namespace graphics::sdl_gpu {
namespace {

// SPIRV-Cross renames the entry point on the way to Metal: main is reserved there.
#if defined(__APPLE__)
constexpr const char* entry_point = "main0";
#else
constexpr const char* entry_point = "main";
#endif

struct ShaderCode {
  const unsigned char* code;
  size_t size;
  SDL_GPUShaderStage stage;
  // The textures the shader reads, each bound with a sampler. That includes the ones read
  // texel by texel (Load), which shadercross's reflection lists as storage textures
  // (shaders/common.hlsl).
  Uint32 textures;
  Uint32 uniform_buffers;
};

// In the order of Shader.
constexpr ShaderCode shaders[] = {
    {scene_vs_code, sizeof(scene_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 0, 1},
    {mesh_vs_code, sizeof(mesh_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 2, 1},
    {tile_vs_code, sizeof(tile_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 0, 1},
    {scene_fs_code, sizeof(scene_fs_code), SDL_GPU_SHADERSTAGE_FRAGMENT, 4, 2},
    {shadow_vs_code, sizeof(shadow_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 0, 1},
    {shadow_fs_code, sizeof(shadow_fs_code), SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1},
    {quad_vs_code, sizeof(quad_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 0, 0},
    {back_buffer_fs_code, sizeof(back_buffer_fs_code), SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0},
    {flush_vs_code, sizeof(flush_vs_code), SDL_GPU_SHADERSTAGE_VERTEX, 0, 1},
    {flush_fs_code, sizeof(flush_fs_code), SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1},
};
static_assert(std::size(shaders) == static_cast<size_t>(Shader::count), "a shader without code, or code without a shader");

}  // namespace

SDL_GPUShader* create_shader(SDL_GPUDevice* device, Shader shader) {
  const auto& code = shaders[static_cast<size_t>(shader)];
  SDL_GPUShaderCreateInfo info{};
  info.code = code.code;
  info.code_size = code.size;
  info.entrypoint = entry_point;
  info.format = shader_format;
  info.stage = code.stage;
  info.num_samplers = code.textures;
  info.num_uniform_buffers = code.uniform_buffers;
  return SDL_CreateGPUShader(device, &info);
}

}  // namespace graphics::sdl_gpu
