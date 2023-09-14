//
// Created by caiiiycuk on 07.02.22.
//

#ifndef MOONSHINE_RUNNERS_TEXTURE_MANAGER_H
#define MOONSHINE_RUNNERS_TEXTURE_MANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>

#include <sokol_gfx.h>

#include "Md3d.h"

namespace graphics
{

class TextureManager final
{
 public:
  static constexpr unsigned int max_textures_count = 2048;

 public:
  TextureManager();

  TextureManager(const TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager& operator=(TextureManager&&) = delete;

  sg_image* get(uint32_t dwHandle);
  void delete_textures();

  MD3DERROR d3dGetTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData);
  MD3DERROR d3dCreateTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwTexFormatID, uint32_t* lpdwHandle);
  MD3DERROR d3dDeleteTexture(uint32_t dwHandle);
  MD3DERROR d3dLockTexture(uint32_t dwHandle, void** lplpTexture, uint32_t* lpPitch);
  MD3DERROR d3dLockTexture(
      uint32_t dwHandle,
      uint32_t dwLeft,
      uint32_t dwTop,
      uint32_t dwRight,
      uint32_t dwBottom,
      void** lplpTexture,
      uint32_t* lpPitch
  );
  MD3DERROR d3dUnlockTexture(uint32_t dwHandle);

 private:
  struct TextureEntry
  {
    uint32_t original_format_id;
    sg_image texture{SG_INVALID_ID};
    std::vector<char> lock_buffer;
    uint32_t pitch;
    bool is_locked;
    bool is_deleted;
    bool is_need_update;
  };

  void update_texture(TextureEntry& entry);

 private:
  using TextureEntryPtr = std::unique_ptr<TextureEntry>;
  std::unordered_map<uint32_t, TextureEntryPtr> _textures;
  uint32_t _lastTextureKey = 0;

  std::vector<char> _rgba_buffer;
};

}

#endif  // MOONSHINE_RUNNERS_TEXTURE_MANAGER_H
