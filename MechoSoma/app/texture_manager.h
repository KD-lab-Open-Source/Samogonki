//
// Created by caiiiycuk on 07.02.22.
//

#ifndef MOONSHINE_RUNNERS_TEXTURE_MANAGER_H
#define MOONSHINE_RUNNERS_TEXTURE_MANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "IGraph3d.h"
#include "render_backend.h"

namespace graphics
{

class TextureManager final
{
 public:
  static constexpr unsigned int max_textures_count = 2048;

 public:
  // mipmaps: give each texture a full mip chain (for trilinear filtering).
  TextureManager(RenderBackend& backend, bool mipmaps);

  TextureManager(const TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager& operator=(TextureManager&&) = delete;

  // The texture to draw with, uploaded first if the game changed it; 0 if it has no
  // pixels yet. Call it outside a pass.
  GpuTexture get(uint32_t dwHandle);
  void delete_textures();

  MD3DERROR getTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData);
  MD3DERROR createTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwTexFormatID, uint32_t* lpdwHandle);
  MD3DERROR deleteTexture(uint32_t dwHandle);
  MD3DERROR lockTexture(uint32_t dwHandle, void** lplpTexture, uint32_t* lpPitch);
  MD3DERROR lockTexture(
      uint32_t dwHandle,
      uint32_t dwLeft,
      uint32_t dwTop,
      uint32_t dwRight,
      uint32_t dwBottom,
      void** lplpTexture,
      uint32_t* lpPitch
  );
  MD3DERROR unlockTexture(uint32_t dwHandle);

 private:
  struct TextureEntry
  {
    uint32_t original_format_id;
    uint32_t width;
    uint32_t height;
    // Created at the first upload. Most textures are uploaded once; one uploaded again is
    // one the game draws into.
    GpuTexture texture = 0;
    std::vector<char> lock_buffer;
    uint32_t pitch;
    bool is_locked = false;
    bool is_deleted = false;
    bool is_need_update = false;
  };

  void update_texture(TextureEntry& entry);
  int mipmap_count(const TextureEntry& entry) const;

 private:
  using TextureEntryPtr = std::unique_ptr<TextureEntry>;
  std::unordered_map<uint32_t, TextureEntryPtr> _textures;
  uint32_t _lastTextureKey = 0;
  RenderBackend& _backend;
  const bool _mipmaps;

  // The converted RGBA8 image: level 0, then each mip level, back to back.
  std::vector<uint8_t> _rgba_buffer;
};

}

#endif  // MOONSHINE_RUNNERS_TEXTURE_MANAGER_H
