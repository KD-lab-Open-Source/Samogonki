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
  void set_option(D3DRENDERSTATETYPE type, DWORD value) { _options[type] = value; }

  DWORD get_option(D3DRENDERSTATETYPE type) const { return _options.at(type); }

  void set_texture(DWORD handle, DWORD stage) { _texture_stages[stage].texture_handle = handle; }

  std::optional<DWORD> get_texture(DWORD stage) const { return _texture_stages[stage].texture_handle; }

  void reset_texture_stage() {
    for (auto &stage : _texture_stages) {
      stage.texture_handle = std::nullopt;
    }
  }

  void set_texture_stage_state(DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value) {
    _texture_stages[stage].states[state] = value;
  }

  DWORD get_texture_stage_state(DWORD stage, D3DTEXTURESTAGESTATETYPE state) const {
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
  std::unordered_map<D3DRENDERSTATETYPE, DWORD> _options;

  struct TextureStage {
    std::optional<DWORD> texture_handle;
    std::unordered_map<D3DTEXTURESTAGESTATETYPE, DWORD> states;

    TextureStage() : texture_handle(std::nullopt) {}

    bool operator==(const TextureStage& other) const {
      return texture_handle == other.texture_handle && states == other.states;
    }
  };
  std::array<TextureStage, 10> _texture_stages;
};
}  // namespace graphics::d3d
#endif  // MOONSHINE_RUNNERS_RENDER_STATE_H
