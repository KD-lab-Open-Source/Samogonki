//
// Created by caiiiycuk on 09.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDER_STATE_H
#define MOONSHINE_RUNNERS_RENDER_STATE_H

#include <array>
#include <optional>

#include "IGraph3d.h"
#include "xerrhand.h"

namespace graphics {
struct RenderState final {
  bool operator==(const RenderState& other) const {
    return is_depth_test_enabled == other.is_depth_test_enabled
      && is_depth_write_enabled == other.is_depth_write_enabled
      && is_alpha_test_enabled == other.is_alpha_test_enabled
      && alpha_reference == other.alpha_reference
      && is_alpha_blend_enabled == other.is_alpha_blend_enabled
      && source_blend_mode == other.source_blend_mode
      && destination_blend_mode == other.destination_blend_mode
      && texture_address == other.texture_address
      && color_operation1 == other.color_operation1
      && color_operation2 == other.color_operation2
      && textures == other.textures
      && projection_matrix == other.projection_matrix
      && viewport == other.viewport;
  }

  bool is_depth_test_enabled = true;
  bool is_depth_write_enabled = true;
  bool is_alpha_test_enabled = false;
  int alpha_reference = 1;
  bool is_alpha_blend_enabled = false;
  eBlendMode source_blend_mode = BLEND_SRCALPHA;
  eBlendMode destination_blend_mode = BLEND_INVSRCALPHA;
  eRenderStateTextureAddress texture_address = TADDRESS_WRAP;

  int color_operation1 = 0;
  int color_operation2 = 0;
  std::array<std::optional<uint32_t>, 2> textures{};

  std::optional<D3DMATRIX> projection_matrix{};
  std::optional<MD3DRECT> viewport{};
};

}

#endif  // MOONSHINE_RUNNERS_RENDER_STATE_H
