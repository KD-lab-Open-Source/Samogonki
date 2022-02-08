#pragma once

#include <array>

template<typename E, class T, std::size_t N>
class EnumArray final
{
public:
	T& operator[](E e)
	{
		return _array[static_cast<std::size_t>(e)];
	}

	const T& operator[](E e) const
	{
		return _array[static_cast<std::size_t>(e)];
	}

private:
	std::array<T, N> _array;
};
