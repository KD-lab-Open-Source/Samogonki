//
// Created by caiiiycuk on 07.02.22.
//

#include "texture_manager.h"

#include <array>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "xerrhand.h"

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

namespace {
  // Halves a square RGBA8 image of src_size x src_size into dst. Colours are averaged
  // weighted by alpha, so fully transparent texels, whose colour is arbitrary (often
  // black), don't bleed into the edges of sprites and masks. For opaque texels this
  // is the plain average.
  void downsample(const uint8_t* src, uint32_t src_size, uint8_t* dst) {
    const uint32_t dst_size = src_size / 2;
    for (uint32_t y = 0; y < dst_size; y++) {
      const uint8_t* row0 = src + 4 * (2 * y) * src_size;
      const uint8_t* row1 = row0 + 4 * src_size;
      for (uint32_t x = 0; x < dst_size; x++) {
        const uint8_t* p[4] = {row0 + 8 * x, row0 + 8 * x + 4, row1 + 8 * x, row1 + 8 * x + 4};
        const uint32_t alpha = p[0][3] + p[1][3] + p[2][3] + p[3][3];
        uint8_t* out = dst + 4 * (y * dst_size + x);
        for (int c = 0; c < 3; c++) {
          if (alpha == 0) {
            out[c] = static_cast<uint8_t>((p[0][c] + p[1][c] + p[2][c] + p[3][c] + 2) / 4);
          } else {
            const uint32_t sum = p[0][c] * p[0][3] + p[1][c] * p[1][3] + p[2][c] * p[2][3] + p[3][c] * p[3][3];
            out[c] = static_cast<uint8_t>((sum + alpha / 2) / alpha);
          }
        }
        out[3] = static_cast<uint8_t>((alpha + 2) / 4);
      }
    }
  }
}

TextureManager::TextureManager(RenderBackend& backend, bool mipmaps) : _backend(backend), _mipmaps(mipmaps) {
  for (auto& format : texture_formats) {
    format.dwRBitShift = GetMaskBitShift(format.dwRBitMask);
    format.dwGBitShift = GetMaskBitShift(format.dwGBitMask);
    format.dwBBitShift = GetMaskBitShift(format.dwBBitMask);
    format.dwAlphaBitShift = GetMaskBitShift(format.dwRGBAlphaBitMask);
  }
}

MD3DERROR TextureManager::getTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData) {
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

MD3DERROR TextureManager::createTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwTexFormatID, uint32_t* lpdwHandle) {
  assert(_textures.size() < max_textures_count);
  assert(dwWidth == dwHeight);
  assert(GetMaskBitCount(dwWidth) == 1);
  assert(GetMaskBitCount(dwHeight) == 1);
  assert(dwHeight <= 256);
  assert(dwWidth <= 256);

  auto p = std::find_if(texture_formats.cbegin(), texture_formats.cend(),
                        [&](const auto& format) { return format.dwFormatID == dwTexFormatID && format.bSupported; });
  if (p == texture_formats.cend()) {
    printf("TextureManager::createTexture: unknown format=%x\n", dwTexFormatID);
    return MD3DERR_ILLEGALCALL;
  }

  assert(p->bPalette8 == false);

  *lpdwHandle = _lastTextureKey;

  // The GPU image is made at the first upload (update_texture), once the pixels exist.
  auto t = std::make_unique<TextureEntry>();
  t->original_format_id = dwTexFormatID;
  t->width = dwWidth;
  t->height = dwHeight;
  t->pitch = dwWidth * (p->dwRGBBitCount / 8);
  t->lock_buffer.resize(t->pitch * dwHeight);
  _textures.emplace(_lastTextureKey, std::move(t));

  _lastTextureKey++;
  return MD3D_OK;
}

MD3DERROR TextureManager::deleteTexture(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return MD3DERR_ILLEGALCALL;
  }

  entry->second->is_deleted = true;
  return MD3D_OK;
}

MD3DERROR TextureManager::lockTexture(uint32_t dwHandle, void** lplpTexture, uint32_t* lpPitch) {
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

MD3DERROR TextureManager::lockTexture(uint32_t dwHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwRight, uint32_t dwBottom,
                                         void** lplpTexture, uint32_t* lpPitch) {
  return lockTexture(dwHandle, lplpTexture, lpPitch);
}

MD3DERROR TextureManager::unlockTexture(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return MD3DERR_ILLEGALCALL;
  }

  if (!entry->second->is_locked) {
    return MD3DERR_ILLEGALCALL;
  }

  entry->second->is_need_update = true;
  entry->second->is_locked = false;

  return MD3D_OK;
}

int TextureManager::mipmap_count(const TextureEntry& entry) const {
  // The game's textures are square powers of two (createTexture asserts it); give
  // anything else a single level rather than guess.
  const bool square_power_of_two = entry.width == entry.height && (entry.width & (entry.width - 1)) == 0;
  if (!_mipmaps || !square_power_of_two || entry.width == 0) {
    return 1;
  }
  int count = 1;
  for (uint32_t size = entry.width; size > 1; size /= 2) {
    count++;
  }
  return count;  // 256 x 256: 9 levels, down to 1 x 1
}

void TextureManager::update_texture(TextureEntry& entry) {
  const int levels = mipmap_count(entry);
  size_t total = 0;
  for (int level = 0; level < levels; level++) {
    total += 4 * static_cast<size_t>(entry.width >> level) * (entry.height >> level);
  }
  _rgba_buffer.resize(total);

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
      ErrH.Abort("Not supported texture format", XERR_USER, 0, "");
    } break;
  }

  // Level 0 is converted above; each further level halves the one before it.
  TextureLevels data{};
  data.count = levels;
  size_t offset = 0;
  for (int level = 0; level < levels; level++) {
    const uint32_t width = entry.width >> level;
    const uint32_t height = entry.height >> level;
    const size_t size = 4 * static_cast<size_t>(width) * height;
    uint8_t* pixels = _rgba_buffer.data() + offset;
    if (level > 0) {
      downsample(pixels - 4 * static_cast<size_t>(2 * width) * (2 * height), 2 * width, pixels);
    }
    data.pixels[level] = pixels;
    data.sizes[level] = size;
    offset += size;
  }

  const int width = static_cast<int>(entry.width);
  const int height = static_cast<int>(entry.height);
  if (entry.texture == 0) {
    entry.texture = _backend.createTexture(width, height, levels, &data);
  } else {
    // Uploaded again: the game draws into this texture (shadows into a light map, a
    // terrain tile rebuilt from the colour map).
    _backend.updateTexture(entry.texture, data);
  }
}

GpuTexture TextureManager::get(uint32_t dwHandle) {
  const auto entry = _textures.find(dwHandle);
  assert(entry != _textures.end());
  if (entry == _textures.end()) {
    return 0;
  }

  if (entry->second->is_need_update) {
    update_texture(*entry->second);
    entry->second->is_need_update = false;
  }

  // A texture that was never unlocked has no image yet (0); the renderer then binds its
  // empty texture.
  return entry->second->texture;
}

void TextureManager::delete_textures() {
  for (auto i = _textures.begin(); i != _textures.end(); ) {
    if (i->second->is_deleted) {
      if (i->second->texture != 0) {
        _backend.destroyTexture(i->second->texture);
      }
      i = _textures.erase(i);
    } else {
      i++;
    }
  }
}
