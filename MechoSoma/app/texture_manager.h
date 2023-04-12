//
// Created by caiiiycuk on 07.02.22.
//

#ifndef MOONSHINE_RUNNERS_TEXTURE_MANAGER_H
#define MOONSHINE_RUNNERS_TEXTURE_MANAGER_H

#include "texture_manager_interface.h"

#include <memory>
#include <vector>
#include <unordered_map>

#include <sokol_gfx.h>

namespace graphics
{

class TextureManager final : public TextureManagerInterface
{
 public:
  TextureManager();

  sg_image* get(DWORD dwHandle);
  void delete_textures();

  TextureManager(const TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager& operator=(TextureManager&&) = delete;

  MD3DERROR d3dGetTextureFormatData(DWORD dwTexFormatID, M3DTEXTUREFORMAT* pData) override;
  MD3DERROR d3dCreateTexture(DWORD dwWidth, DWORD dwHeight, DWORD dwTexFormatID, DWORD* lpdwHandle) override;
  MD3DERROR d3dDeleteTexture(DWORD dwHandle) override;
  MD3DERROR d3dLockTexture(DWORD dwHandle, VOID** lplpTexture, DWORD* lpPitch) override;
  MD3DERROR d3dLockTexture(
      DWORD dwHandle,
      DWORD dwLeft,
      DWORD dwTop,
      DWORD dwRight,
      DWORD dwBottom,
      VOID** lplpTexture,
      DWORD* lpPitch
  ) override;
  MD3DERROR d3dUnlockTexture(DWORD dwHandle) override;

 private:
  struct TextureEntry
  {
    DWORD original_format_id;
    sg_image texture{SG_INVALID_ID};
    std::vector<char> lock_buffer;
    DWORD pitch;
    bool is_locked;
    bool is_deleted;
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
