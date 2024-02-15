#include "jpeg_fm.h"

#include <stdio.h>
#include <vector>
#define XMD_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "xtool.h"

namespace
{
	struct DecodedImage final {
		int width = 0;
		int height = 0;
		int components = 0;
		unsigned char *data = nullptr;
		
		explicit DecodedImage(XStream &file) {
			std::vector<unsigned char> buffer(file.size());
			file.read(buffer.data(), buffer.size());
			data = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &components, 0);
			if (data == nullptr) {
				ErrH.Abort("failed to load JPEG file");
			}

			if (components != 3) {
				ErrH.Abort("wrong JPEG image color components number");
			}
		}

		~DecodedImage() {
			if (data != nullptr) {
				stbi_image_free(data);
			}
		}
	};
}

unsigned short *loadJPG(XStream *file, int &width, int &height) {
	DecodedImage decoded(*file);

	width = decoded.width;
	height = decoded.height;
	auto image = new unsigned short[width * height];

	unsigned char *p = decoded.data;
	size_t cursor = 0;
	for (int i = 0; i < width * height; i++) {
		unsigned short color = (*p++ << 8) & 0x0F800;
		color += (*p++ << 3) & 0x7E0;
		color += (*p++ >> 3) & 0x1F;
		image[cursor++] = color;
	}

	return image;
}

int loadClBufJ(XStream &file, short unsigned int (*ClBuf)[2048][2048]) {
	DecodedImage decoded(file);

	if (decoded.width != 2048 || decoded.height != 2048) {
		ErrH.Abort("wrong JPEG image dimensions");
	}

	unsigned char *p = decoded.data;
	for (int i = 0; i < decoded.height; i++) {
		for (int j = 0; j < decoded.width; j++) {
			unsigned short color = (*p++ << 8) & 0x0F800;
			color += (*p++ << 3) & 0x7E0;
			color += (*p++ >> 3) & 0x1F;
			ClBuf[0][i][j] = color;
		}
	}

	return 1;
}
