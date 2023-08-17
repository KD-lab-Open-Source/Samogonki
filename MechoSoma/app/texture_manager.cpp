//
// Created by caiiiycuk on 07.02.22.
//

#include "texture_manager.h"

#include <array>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace graphics;

struct TTextureFormat {
  uint32_t dwFormatID;
  bool bSupported;

  uint32_t dwRGBAlphaBitMask;
  uint32_t dwRBitMask;
  uint32_t dwGBitMask;
  uint32_t dwBBitMask;

  // This is used for matching
  uint32_t dwRGBBitCount;
  bool bPalette8;
  uint32_t dwRBitCount;
  uint32_t dwGBitCount;
  uint32_t dwBBitCount;
  uint32_t dwAlphaBitCount;

  // Calculated shifts for RGBA
  uint32_t dwRBitShift;
  uint32_t dwGBitShift;
  uint32_t dwBBitShift;
  uint32_t dwAlphaBitShift;
};

/* clang-format off */
static std::array<TTextureFormat, 6> texture_formats
    {
        TTextureFormat
            {
                D3DTEXFMT_RGB565,
                true,
                0,
                0b1111100000000000,
                0b0000011111100000,
                0b0000000000011111,
                16,
                false,
                5,
                6,
                5,
                0,
                0,
                0,
                0,
                0
            },
        TTextureFormat
            {
                D3DTEXFMT_RGB555,
                false,
                0,
                0,
                0,
                0,
                16,
                false,
                5,
                5,
                5,
                0,
                0,
                0,
                0,
                0
            },
        TTextureFormat
        {
            D3DTEXFMT_ARGB4444,
            true,
            0b1111000000000000,
            0b0000111100000000,
            0b0000000011110000,
            0b0000000000001111,
            16,
            false,
            4,
            4,
            4,
            4,
            0,
            0,
            0,
            0
        },
        TTextureFormat
        {
            D3DTEXFMT_ARGB1555,
            true,
            0b1000000000000000,
            0b0111110000000000,
            0b0000001111100000,
            0b0000000000011111,
            16,
            false,
            5,
            5,
            5,
            1,
            0,
            0,
            0,
            0
        },
        TTextureFormat
        {
            D3DTEXFMT_RGBA8888,
            false,
            0x000000FF,
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            32,
            false,
            8,
            8,
            8,
            8,
            0,
            0,
            0,
            0
        },
        TTextureFormat
        {
            D3DTEXFMT_PAL8,
            true,
            0,
            0,
            0,
            0,
            8,
            true,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        }
    };
/* clang-format on */

constexpr uint32_t GetMaskBitShift(uint32_t dwBits) {
  if (0 == dwBits) {
    return 0;
  }

  uint32_t dwShift = 0;
  for (; (dwBits & 1) == 0; dwBits >>= 1) {
    dwShift++;
  }

  return dwShift;
}

constexpr uint32_t GetMaskBitCount(uint32_t dwBits) {
  uint32_t dwBitCount = 0;
  for (; dwBits; dwBits >>= 1) {
    dwBitCount += (dwBits & 0x1);
  }

  return dwBitCount;
}

TextureManager::TextureManager() {
  for (auto& format : texture_formats) {
    format.dwRBitShift = GetMaskBitShift(format.dwRBitMask);
    format.dwGBitShift = GetMaskBitShift(format.dwGBitMask);
    format.dwBBitShift = GetMaskBitShift(format.dwBBitMask);
    format.dwAlphaBitShift = GetMaskBitShift(format.dwRGBAlphaBitMask);
  }
}

MD3DERROR TextureManager::d3dGetTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData) {
  for (const auto& format : texture_formats) {
    if (format.dwFormatID == dwTexFormatID && format.bSupported) {
      pData->dwTotalBitCount = format.dwRGBBitCount;
      pData->bPalette8 = format.bPalette8;

      pData->dwAlphaBitCount = format.dwAlphaBitCount;
      pData->dwRBitCount = format.dwRBitCount;
      pData->dwGBitCount = format.dwGBitCount;
      pData->dwBBitCount = format.dwBBitCount;

      pData->dwAlphaBitMask = format.dwRGBAlphaBitMask;
      pData->dwRBitMask = format.dwRBitMask;
      pData->dwGBitMask = format.dwGBitMask;
      pData->dwBBitMask = format.dwBBitMask;

      pData->dwAlphaBitShift = format.dwAlphaBitShift;
      pData->dwRBitShift = format.dwRBitShift;
      pData->dwGBitShift = format.dwGBitShift;
      pData->dwBBitShift = format.dwBBitShift;

      return MD3D_OK;
    }
  }
  return MD3DERR_ILLEGALCALL;
}

MD3DERROR TextureManager::d3dCreateTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwTexFormatID, uint32_t* lpdwHandle) {
  assert(_textures.size() < max_textures_count);
  assert(dwWidth == dwHeight);
  assert(GetMaskBitCount(dwWidth) == 1);
  assert(GetMaskBitCount(dwHeight) == 1);
  assert(dwHeight <= 256);
  assert(dwWidth <= 256);

  auto p = std::find_if(texture_formats.cbegin(), texture_formats.cend(),
                        [&](const auto& format) { return format.dwFormatID == dwTexFormatID && format.bSupported; });
  if (p == texture_formats.cend()) {
    return MD3DERR_ILLEGALCALL;
  }

  auto dataSize = dwWidth * dwHeight * (p->bPalette8 ? 1 : 4);
  auto data = std::vector<char>(dataSize);
  sg_image_desc description = {};
  description.width = dwWidth;
  description.height = dwHeight;
  description.pixel_format = p->bPalette8 ? SG_PIXELFORMAT_R8 : SG_PIXELFORMAT_RGBA8;
  assert(p->bPalette8 == false);
  description.usage = SG_USAGE_DYNAMIC;
  description.min_filter = SG_FILTER_LINEAR;
  description.mag_filter = SG_FILTER_LINEAR;
  auto texture = sg_make_image(description);

  *lpdwHandle = _lastTextureKey;

  const uint32_t pitch = dwWidth * (p->dwRGBBitCount / 8);
  auto t = std::make_unique<TextureEntry>(
    TextureEntry{dwTexFormatID, texture, std::vector<char>(pitch * dwHeight), pitch, false, false}
  );
  _textures.emplace(_lastTextureKey, std::move(t));

  _lastTextureKey++;
  return MD3D_OK;
}

MD3DERROR TextureManager::d3dDeleteTexture(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return MD3DERR_ILLEGALCALL;
  }

  entry->second->is_deleted = true;
  return MD3D_OK;
}

MD3DERROR TextureManager::d3dLockTexture(uint32_t dwHandle, void** lplpTexture, uint32_t* lpPitch) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return MD3DERR_ILLEGALCALL;
  }

  if (entry->second->is_locked) {
    return MD3DERR_ILLEGALCALL;
  }
  entry->second->is_locked = true;

  *lplpTexture = reinterpret_cast<void*>(entry->second->lock_buffer.data());
  *lpPitch = entry->second->pitch;

  return MD3D_OK;
}

MD3DERROR TextureManager::d3dLockTexture(uint32_t dwHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwRight, uint32_t dwBottom,
                                         void** lplpTexture, uint32_t* lpPitch) {
  return d3dLockTexture(dwHandle, lplpTexture, lpPitch);
}

MD3DERROR TextureManager::d3dUnlockTexture(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return MD3DERR_ILLEGALCALL;
  }

  if (!entry->second->is_locked) {
    return MD3DERR_ILLEGALCALL;
  }

  update_texture(*entry->second);
  entry->second->is_locked = false;

  return MD3D_OK;
}

void TextureManager::update_texture(TextureEntry& entry) {
  auto info = sg_query_image_info(entry.texture);
  const auto image_height = info.height;
  const auto image_width = info.width;
  const size_t size = 4 * image_width * image_height;
  _rgba_buffer.resize(size);

  switch (entry.original_format_id) {
    case D3DTEXFMT_RGB565: {
      const auto input = reinterpret_cast<const uint16_t*>(entry.lock_buffer.data());
      auto output = reinterpret_cast<uint32_t*>(_rgba_buffer.data());

      // clang-format off
      const uint16_t red_mask =   0b1111100000000000;
      const uint16_t green_mask = 0b0000011111100000;
      const uint16_t blue_mask =  0b0000000000011111;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const auto color = input[i];

        uint8_t r = (color & red_mask) >> 11;
        uint8_t g = (color & green_mask) >> 5;
        uint8_t b = color & blue_mask;

        auto result = (uint8_t*)(output + i);
        result[0] = (r * 527 + 23) >> 6;
        result[1] = (g * 259 + 33) >> 6;
        result[2] = (b * 527 + 23) >> 6;
        result[3] = 255;
      }
    } break;

    case D3DTEXFMT_ARGB1555:
    case D3DTEXFMT_RGB555: {
      const auto input = reinterpret_cast<const uint16_t*>(entry.lock_buffer.data());
      auto output = reinterpret_cast<uint32_t*>(_rgba_buffer.data());

      // clang-format off
      const uint16_t alpha_mask = 0b1000000000000000;
      const uint16_t red_mask =   0b0111110000000000;
      const uint16_t green_mask = 0b0000001111100000;
      const uint16_t blue_mask =  0b0000000000011111;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const auto color = input[i];

        uint8_t a = (color & alpha_mask) > 0 || entry.original_format_id == D3DTEXFMT_RGB555 ? 255 : 0;
        uint8_t r = (color & red_mask) >> 10;
        uint8_t g = (color & green_mask) >> 5;
        uint8_t b = color & blue_mask;

        auto result = (uint8_t*)(output + i);
        result[0] = (r * 527 + 23) >> 6;
        result[1] = (g * 527 + 23) >> 6;
        result[2] = (b * 527 + 23) >> 6;
        result[3] = a;
      }
    } break;

    case D3DTEXFMT_ARGB4444: {
      const auto input = reinterpret_cast<const uint16_t*>(entry.lock_buffer.data());
      auto output = reinterpret_cast<uint32_t*>(_rgba_buffer.data());

      // clang-format off
      const uint16_t alpha_mask = 0b1111000000000000;
      const uint16_t red_mask =   0b0000111100000000;
      const uint16_t green_mask = 0b0000000011110000;
      const uint16_t blue_mask =  0b0000000000001111;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const uint16_t color = input[i];

        uint8_t a = (color & alpha_mask) >> 12;
        uint8_t r = (color & red_mask) >> 8;
        uint8_t g = (color & green_mask) >> 4;
        uint8_t b = color & blue_mask;

        auto rgba = (uint8_t*)(output + i);
        rgba[0] = (r * 255 + 7) / 15;
        rgba[1] = (g * 255 + 7) / 15;
        rgba[2] = (b * 255 + 7) / 15;
        rgba[3] = (a * 255 + 7) / 15;
      }
    } break;

    default: {
      std::cout << "Not supported texture format" << std::endl;
      abort();
    } break;
  }

  sg_image_data imageData;
  imageData.subimage[0][0] = {
      .ptr = _rgba_buffer.data(),
      .size = size,
  };
  sg_update_image(entry.texture, imageData);
}

sg_image* TextureManager::get(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return nullptr;
  }
  return &entry->second->texture;
}

void TextureManager::delete_textures() {
  for (auto i = _textures.begin(); i != _textures.end(); ) {
    if (i->second->is_deleted) {
      sg_destroy_image(i->second->texture);
      i = _textures.erase(i);
    } else {
      i++;
    }
  }
}
