#include "common.h"

#include <format>
#include <fstream>
#include <stdexcept>

namespace common
{

std::string load_file(const std::filesystem::path &file_path)
{
	const auto file_size = std::filesystem::file_size(file_path);
	std::ifstream file(file_path, std::ios::in | std::ios::binary);
	if (!file)
	{
		throw std::runtime_error(std::format("open file={}", file_path.string()));
	}

	std::string buffer(file_size, 0);
	file.read(buffer.data(), file_size);
	if (!file)
	{
		throw std::runtime_error(std::format("read file={}", file_path.string()));
	}

	return buffer;
}

}
