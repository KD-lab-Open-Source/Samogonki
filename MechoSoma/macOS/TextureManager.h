#pragma once

#import <Metal/Metal.h>

#include <vector>

#include "Ref.h"
#include "TextureManagerInterface.h"

namespace graphics
{

class TextureManager final : public TextureManagerInterface
{
public:
	explicit TextureManager(id<MTLDevice> device);

	id<MTLTexture> get(DWORD dwHandle);

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
		objc::Ref<id<MTLTexture>> texture;
		std::vector<char> lock_buffer;
		DWORD pitch;
		bool is_locked;
	};

	void update_texture(TextureEntry& entry);

private:
	id<MTLDevice> _device;

	using TextureEntryPtr = std::unique_ptr<TextureEntry>;
	std::vector<TextureEntryPtr> _textures;

	std::vector<char> _argb_buffer;
	std::vector<char> _bgra_buffer;
};

}
