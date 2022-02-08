#pragma once

#include "Md3d.h"

namespace graphics {

struct TextureManagerInterface {
  virtual ~TextureManagerInterface() = default;

  virtual MD3DERROR d3dGetTextureFormatData(DWORD dwTexFormatID, M3DTEXTUREFORMAT* pData) = 0;
  virtual MD3DERROR d3dCreateTexture(DWORD dwWidth, DWORD dwHeight, DWORD dwTexFormatID, DWORD* lpdwHandle) = 0;
  virtual MD3DERROR d3dDeleteTexture(DWORD dwHandle) = 0;
  virtual MD3DERROR d3dLockTexture(DWORD dwHandle, VOID** lplpTexture, DWORD* lpPitch) = 0;
  virtual MD3DERROR d3dLockTexture(DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom,
                                   VOID** lplpTexture, DWORD* lpPitch) = 0;
  virtual MD3DERROR d3dUnlockTexture(DWORD dwHandle) = 0;
};

}  // namespace graphics
