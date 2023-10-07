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

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-shader.h"
#endif

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
    _projection_matrix = std::nullopt;
  }

  void set_texture_stage_state(uint32_t stage, D3DTEXTURESTAGESTATETYPE state, uint32_t value) {
    _texture_stages[stage].states[state] = value;
  }

  std::optional<uint32_t> get_texture_stage_state(uint32_t stage, D3DTEXTURESTAGESTATETYPE state) const {
    const auto p = _texture_stages[stage].states.find(state);
    if (p == _texture_stages[stage].states.end()) {
      return std::nullopt;
    }
    return p->second;
  }

  FragmentShaderParameters get_fragment_shader_parameters() const {
    FragmentShaderParameters result;
    switch (_texture_stages[0].states.at(D3DTSS_COLOROP)) {
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

    switch (_texture_stages[1].states.at(D3DTSS_COLOROP)) {
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

  void set_projection_matrix(const D3DMATRIX& matrix) {
    _projection_matrix = matrix;
  }

  std::optional<D3DMATRIX> get_projection_matrix() const {
    return _projection_matrix;
  }

  void reset_projection_matrix() {
    _projection_matrix = std::nullopt;
  }

  bool operator==(const RenderState& other) const {
    return _options == other._options && _texture_stages == other._texture_stages && _projection_matrix == other._projection_matrix;
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

  std::optional<D3DMATRIX> _projection_matrix;
};
}  // namespace graphics::d3d
#endif  // MOONSHINE_RUNNERS_RENDER_STATE_H
