#include "XJoystick.h"

#include <array>
#include <memory>

#include <SDL2/SDL.h>

#include "xtool.h"

class XJoystick final
{
public:
	explicit XJoystick(int id)
	{
		_joystick = SDL_JoystickOpen(id);
		if (_joystick == nullptr)
		{
			ErrH.Abort("SDL_JoystickOpen");
		}

		_buttonsCount = SDL_JoystickNumButtons(_joystick);
	}

	~XJoystick()
	{
		SDL_JoystickClose(_joystick);
	}

	void SetHandler(XJoystickHandler handler)
	{
		_handler = handler;
	}

	void Update()
	{
		const auto time = clocki();
		if (time < _last_clock)
		{
			return;
		}
		_last_clock = time + 20;

		SDL_JoystickUpdate();
		_x = ConvertAxisToGameRange(SDL_JoystickGetAxis(_joystick, 0), false);
		_y = ConvertAxisToGameRange(SDL_JoystickGetAxis(_joystick, 1), true);

		if (_handler != nullptr)
		{
			_handler(XJ_ChangeXAxis, _x, 0);
			_handler(XJ_ChangeYAxis, _y, 0);
		}

		for (int i = 0; i < _buttonsCount; i++)
		{
			_buttons[i] = SDL_JoystickGetButton(_joystick, i);
			if (_handler != nullptr)
			{
				_handler(_buttons[i] ? XJ_PressButton : XJ_UnPressButton, VK_BUTTON_1 + i, 0);
			}
		}
	}

	int GetXAxis() const
	{
		return _x;
	}

	int GetYAxis() const
	{
		return _y;
	}

	bool IsButtonPressed(int code) const
	{
		const auto index = code - VK_BUTTON_1;
		return index < _buttonsCount ? _buttons[index] : false;
	}

private:
	static int ConvertAxisToGameRange(int value, bool is_inverted)
	{
		constexpr int original_axis_max = 32767;
		constexpr int original_axis_min = -32768;

		constexpr int deadzone_max = 0.25f * original_axis_max;
		constexpr int deadzone_min = 0.25f * original_axis_min;
		constexpr int axis_max = original_axis_max - deadzone_max;
		constexpr int axis_min = original_axis_min - deadzone_min;

		if (value > deadzone_min && value < deadzone_max)
		{
			return 0;
		}

		float result = 0;
		if (value < 0)
		{
			const auto normal = 1.0f * (value - deadzone_min) / axis_min;
			result = normal * RANGE_MIN;
		}
		else
		{
			const auto normal = 1.0f * (value - deadzone_max) / axis_max;
			result = normal * RANGE_MAX;
		}

		return static_cast<int>(is_inverted ? -result : result);
	}

private:
	SDL_Joystick *_joystick = nullptr;
	XJoystickHandler _handler = nullptr;
	int _last_clock = 0;
	int _x = 0;
	int _y = 0;
	int _buttonsCount = 0;
	std::array<unsigned char, 32> _buttons{};
};

std::unique_ptr<XJoystick> joystick;

int XJoystickInit(int ErrHUsed)
{
	if (SDL_NumJoysticks() == 0)
	{
		return 0;
	}

	joystick = std::make_unique<XJoystick>(0);

	return 1;
} 

void XJoystickCleanup(void)
{
	joystick.reset();
} 

void XJoystickQuant()
{
	if (joystick != nullptr)
	{
		joystick->Update();
	}
}

int XJoystickIsButtonPressed(int button, int joystick_ID)
{
	return joystick != nullptr ? joystick->IsButtonPressed(button) : 0;
}

int XJoystickGetXAxis(int joystick_ID)
{
	return joystick != nullptr ? joystick->GetXAxis() : 0;
}

int XJoystickGetYAxis(int joystick_ID)
{
	return joystick != nullptr ? joystick->GetYAxis() : 0;
}

XJoystickHandler XJoystickSetHandler(XJoystickHandler handler)
{
	if (joystick != nullptr)
	{
		joystick->SetHandler(handler);
	}
	return nullptr;
}
