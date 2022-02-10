//
// Created by caiiiycuk on 07.02.22.
//

#include "texture_manager.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace graphics;

struct TTextureFormat {
  DWORD dwFormatID;
  BOOL bSupported;

  DWORD dwRGBAlphaBitMask;
  DWORD dwRBitMask;
  DWORD dwGBitMask;
  DWORD dwBBitMask;

  // This is used for matching
  DWORD dwRGBBitCount;
  BOOL bPalette8;
  DWORD dwRBitCount;
  DWORD dwGBitCount;
  DWORD dwBBitCount;
  DWORD dwAlphaBitCount;

  // Calculated shifts for RGBA
  DWORD dwRBitShift;
  DWORD dwGBitShift;
  DWORD dwBBitShift;
  DWORD dwAlphaBitShift;
};

/* clang-format off */
static std::array<TTextureFormat, 6> texture_formats
    {
        TTextureFormat
            {
                D3DTEXFMT_RGB565,
                TRUE,
                0,
                0b0000000000011111,
                0b0000011111100000,
                0b1111100000000000,
                16,
                FALSE,
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
                FALSE,
                0,
                0,
                0,
                0,
                16,
                FALSE,
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
                TRUE,
                0b1111000000000000,
                0b0000000000001111,
                0b0000000011110000,
                0b0000111100000000,
                16,
                FALSE,
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
            TRUE,
            0b1000000000000000,
            0b0000000000011111,
            0b0000001111100000,
            0b0111110000000000,
            16,
            FALSE,
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
            TRUE,
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000,
            32,
            FALSE,
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
            TRUE,
            0,
            0,
            0,
            0,
            8,
            TRUE,
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

constexpr DWORD GetMaskBitShift(DWORD dwBits) {
  if (0 == dwBits) {
    return 0;
  }

  DWORD dwShift = 0;
  for (; (dwBits & 1) == 0; dwBits >>= 1) {
    dwShift++;
  }

  return dwShift;
}

constexpr DWORD GetMaskBitCount(DWORD dwBits) {
  DWORD dwBitCount = 0;
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

MD3DERROR TextureManager::d3dGetTextureFormatData(DWORD dwTexFormatID, M3DTEXTUREFORMAT* pData) {
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

MD3DERROR TextureManager::d3dCreateTexture(DWORD dwWidth, DWORD dwHeight, DWORD dwTexFormatID, DWORD* lpdwHandle) {
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
  auto texture = sg_make_image(description);

  *lpdwHandle = _textures.size();

  const DWORD pitch = dwWidth * (p->dwRGBBitCount / 8);
  _textures.push_back(std::make_unique<TextureEntry>(
      TextureEntry{dwTexFormatID, texture, std::vector<char>(pitch * dwHeight), pitch, false}));

  return MD3D_OK;
}

MD3DERROR TextureManager::d3dDeleteTexture(DWORD dwHandle) {
  // TODO: @caiiiycuk why we not delete it?
  if (dwHandle >= _textures.size()) {
    return MD3DERR_ILLEGALCALL;
  }

  return MD3D_OK;
}

MD3DERROR TextureManager::d3dLockTexture(DWORD dwHandle, VOID** lplpTexture, DWORD* lpPitch) {
  if (dwHandle >= _textures.size()) {
    return MD3DERR_ILLEGALCALL;
  }

  auto entry = _textures[dwHandle].get();
  if (entry->is_locked) {
    return MD3DERR_ILLEGALCALL;
  }
  entry->is_locked = true;

  *lplpTexture = reinterpret_cast<VOID*>(entry->lock_buffer.data());
  *lpPitch = entry->pitch;

  return MD3D_OK;
}

MD3DERROR TextureManager::d3dLockTexture(DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom,
                                         VOID** lplpTexture, DWORD* lpPitch) {
  return d3dLockTexture(dwHandle, lplpTexture, lpPitch);
}

MD3DERROR TextureManager::d3dUnlockTexture(DWORD dwHandle) {
  if (dwHandle >= _textures.size()) {
    return MD3DERR_ILLEGALCALL;
  }

  auto entry = _textures[dwHandle].get();
  if (!entry->is_locked) {
    return MD3DERR_ILLEGALCALL;
  }

  update_texture(*entry);
  entry->is_locked = false;

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
      const uint16_t red_mask =   0b0000000000011111;
      const uint16_t green_mask = 0b0000011111100000;
      const uint16_t blue_mask =  0b1111100000000000;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const auto color = input[i];

        uint8_t r = color & red_mask;
        uint8_t g = (color & green_mask) >> 5;
        uint8_t b = (color & blue_mask) >> 11;

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
      const uint16_t red_mask =   0b0000000000011111;
      const uint16_t green_mask = 0b0000001111100000;
      const uint16_t blue_mask =  0b0111110000000000;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const auto color = input[i];

        uint8_t r = color & red_mask;
        uint8_t g = (color & green_mask) >> 5;
        uint8_t b = (color & blue_mask) >> 10;

        auto result = (uint8_t*)(output + i);
        result[0] = (r * 527 + 23) >> 6;
        result[1] = (g * 527 + 23) >> 6;
        result[2] = (b * 527 + 23) >> 6;
        result[3] = 255;
      }
    } break;

    case D3DTEXFMT_ARGB4444: {
      const auto input = reinterpret_cast<const uint16_t*>(entry.lock_buffer.data());
      auto output = reinterpret_cast<uint32_t*>(_rgba_buffer.data());

      // clang-format off
      const uint16_t alpha_mask = 0b1111000000000000;
      const uint16_t blue_mask =  0b0000111100000000;
      const uint16_t green_mask = 0b0000000011110000;
      const uint16_t red_mask =   0b0000000000001111;
      // clang-format on

      const size_t count = entry.lock_buffer.size() / sizeof(uint16_t);
      for (size_t i = 0; i < count; i++) {
        const uint16_t color = input[i];

        uint8_t a = (color & alpha_mask) >> 12;
        uint8_t r = (color & red_mask);
        uint8_t g = (color & green_mask) >> 4;
        uint8_t b = (color & blue_mask) >> 8;

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

sg_image* TextureManager::get(DWORD dwHandle) {
  if (dwHandle >= _textures.size()) {
    return nullptr;
  }
  return &_textures[dwHandle]->texture;
}
