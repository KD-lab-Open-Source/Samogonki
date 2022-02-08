#pragma once

#import <Foundation/Foundation.h>

namespace objc
{
	template<class T>
	class Ref
	{
	public:
		Ref() = default;

		explicit Ref(T object) : _object(object) {}

		~Ref()
		{
			[_object release];
		}

		Ref(const Ref<T>& value)
		{
			_object = value._object;
			[_object retain];
		}

		T get() const
		{
			return _object;
		}

		Ref<T>& operator=(const Ref<T>& value)
		{
			_object = value._object;
			[_object retain];
			return *this;
		}

		Ref<T>& operator=(T object)
		{
			[_object release];
			_object = object;
			return *this;
		}

		bool operator!()
		{
			return _object == nil;
		}

	private:
		T _object = nil;
	};
}
