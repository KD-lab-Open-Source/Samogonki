//
// Created by caiiiycuk on 09.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDER_STATE_H
#define MOONSHINE_RUNNERS_RENDER_STATE_H

#include <array>
#include <iostream>
#include <optional>
#include <unordered_map>

#include "Md3d.h"
#include "sokol-shader.h"

namespace graphics::d3d {

enum class TextureColorOperation { Disable, Texture, Modulate };

struct FragmentShaderParameters {
  TextureColorOperation color_operation_1;
  TextureColorOperation color_operation_2;
};

class RenderState final {
 public:
  void set_option(D3DRENDERSTATETYPE type, uint32_t value) { _options[type] = value; }

  uint32_t get_option(D3DRENDERSTATETYPE type) const { return _options.at(type); }

  void set_texture(uint32_t handle, uint32_t stage) { _texture_stages[stage].texture_handle = handle; }

  std::optional<uint32_t> get_texture(uint32_t stage) const { return _texture_stages[stage].texture_handle; }

  void reset_texture_stage() {
    for (auto &stage : _texture_stages) {
      stage.texture_handle = std::nullopt;
    }
  }

  void set_texture_stage_state(uint32_t stage, D3DTEXTURESTAGESTATETYPE state, uint32_t value) {
    _texture_stages[stage].states[state] = value;
  }

  uint32_t get_texture_stage_state(uint32_t stage, D3DTEXTURESTAGESTATETYPE state) const {
    return _texture_stages[stage].states.at(state);
  }

  FragmentShaderParameters get_fragment_shader_parameters() const {
    FragmentShaderParameters result;
    switch (get_texture_stage_state(0, D3DTSS_COLOROP)) {
      case D3DTOP_DISABLE:
        result.color_operation_1 = TextureColorOperation::Disable;
        break;

      case D3DTOP_SELECTARG1:
        result.color_operation_1 = TextureColorOperation::Texture;
        break;

      case D3DTOP_MODULATE:
        result.color_operation_1 = TextureColorOperation::Modulate;
        break;

      default: {
        std::cout << "unsupported color operation 1" << std::endl;
        abort();
      }
    }

    switch (get_texture_stage_state(1, D3DTSS_COLOROP)) {
      case D3DTOP_DISABLE:
        result.color_operation_2 = TextureColorOperation::Disable;
        break;

      case D3DTOP_MODULATE:
        result.color_operation_2 = TextureColorOperation::Modulate;
        break;

      default: {
        std::cout << "unsupported color operation 2" << std::endl;
        abort();
      }
    }

    return result;
  }

  bool operator==(const RenderState& other) const {
    return _options == other._options && _texture_stages == other._texture_stages;
  }

 private:
  std::unordered_map<D3DRENDERSTATETYPE, uint32_t> _options;

  struct TextureStage {
    std::optional<uint32_t> texture_handle;
    std::unordered_map<D3DTEXTURESTAGESTATETYPE, uint32_t> states;

    TextureStage() : texture_handle(std::nullopt) {}

    bool operator==(const TextureStage& other) const {
      return texture_handle == other.texture_handle && states == other.states;
    }
  };
  std::array<TextureStage, 10> _texture_stages;
};
}  // namespace graphics::d3d
#endif  // MOONSHINE_RUNNERS_RENDER_STATE_H
