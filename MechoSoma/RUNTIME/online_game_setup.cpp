#include "online_game_setup.h"

#include <array>
#include <cstdlib>
#include <string_view>
#include <vector>

#include "wininet_api.h"
#include "xerrhand.h"
#include "xtool.h"
#include "ACI_SCR.H"
#include "SCRIPTS/ACI_IDS.H"

extern int mchPBEM_DataFlag;
extern int mchPBEM_Game;
extern int mchPBEM_GameID;

extern aciScreenObject *acsGetObject(int screen_id, int object_id);

using namespace network;

namespace
{

std::string_view *split_string(
	const std::string_view input,
	char delimiter,
	std::string_view *output_begin,
	std::string_view *output_end
)
{
	auto current_part = output_begin;
	size_t start = 0;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] != delimiter)
		{
			continue;
		}

		if (start < i)
		{
			*current_part = input.substr(start, i);
			current_part++;
			if (current_part == output_end)
			{
				break;
			}
		}

		start = i + 1;
	}

	if (start < input.size() && current_part != output_end)
	{
		*current_part = input.substr(start);
		current_part++;
	}

	return current_part;
}

}

struct OnlineGameSetup::Properties
{
	enum class State
	{
		idle,
		requesting,
		ready
	};

	State state = State::idle;
	aciScreenInputField *continue_option = nullptr;
	aciScreenInputField *game_parameter = nullptr;
};

OnlineGameSetup &OnlineGameSetup::get()
{
	static OnlineGameSetup setup;
	return setup;
}

void OnlineGameSetup::handle_command(Command command)
{
	switch (command)
	{
		case Command::enter_screen:
			start_request();
			break;

		case Command::exit_screen:
			stop_request();
			break;

		case Command::next_game:
			set_next_game();
			break;

		case Command::previous_game:
			set_previous_game();
			break;

		case Command::start_game:
			start_game();
			break;
	}
}

void OnlineGameSetup::quant()
{
	if (_properties->state != Properties::State::requesting)
	{
		return;
	}

	process_request();
}

OnlineGameSetup::OnlineGameSetup() : _properties(std::make_unique<Properties>())
{
	auto get_field = [](int object_id) -> aciScreenInputField *
	{
		auto p = acsGetObject(ACS_ONLINE_GAME_CFG_SCR, object_id);
		if (!p || p->type != ACS_INPUT_FIELD_OBJ)
		{
			return nullptr;
		}

		return static_cast<aciScreenInputField *>(p);
	};

	_properties->continue_option = get_field(ACS_CONTINUE_OPTION);
	if (_properties->continue_option == nullptr)
	{
		ErrH.Abort("ACS_CONTINUE_OPTION not found", XERR_USER, 0, "");
	}

	_properties->game_parameter = get_field(ACS_ONLINE_GAME_PRM);
	if (_properties->game_parameter == nullptr)
	{
		ErrH.Abort("ACS_ONLINE_GAME_PRM not found", XERR_USER, 0, "");
	}
}

void OnlineGameSetup::start_request()
{
	if (_properties->state != Properties::State::idle)
	{
		return;
	}

	wi_D.connect(wiServerName, wiServerPort);
	wi_D.open_request(WI_GET, "/game-on-line/games");

	_properties->state = Properties::State::requesting;
}

void OnlineGameSetup::stop_request()
{
	wi_D.close_request();
	_properties->state = Properties::State::idle;
}

void OnlineGameSetup::process_request()
{
	wi_D.quant();

	if (wi_D.get_status() == WI_ERROR)
	{
		_properties->state = Properties::State::ready;
		return;
	}

	if (wi_D.get_status() != WI_FINISHED)
	{
		return;
	}
	_properties->state = Properties::State::ready;

	const std::string_view input(wi_D.input_buffer(), wi_D.input_size());
	std::array<std::string_view, 10> numbers{};
	const auto end = split_string(input, ',', numbers.begin(), numbers.end());

	auto field = _properties->game_parameter;
	field->MaxState = end - numbers.begin();
	if (field->MaxState == 0)
	{
		printf("OnlineGameSetup: game list is empty\n");
		return;
	}

	field->MaxStrLen = 32;
	field->alloc_str();

	for (int i = 0; i < field->MaxState; i++)
	{
		field->statePtr[i] = strndup(numbers[i].data(), numbers[i].size());
	}

	field->set_state(0);
	field->flags &= ~(ACS_BLOCKED | ACS_HIDDEN);

	_properties->continue_option->flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
}

void OnlineGameSetup::set_next_game()
{
	auto field = _properties->game_parameter;
	if (field->CurState < (field->MaxState - 1))
	{
		field->set_state(field->CurState + 1);
	}
}

void OnlineGameSetup::set_previous_game()
{
	auto field = _properties->game_parameter;
	if (field->CurState > 0)
	{
		field->set_state(field->CurState - 1);
	}
}

void OnlineGameSetup::start_game()
{
	auto field = _properties->game_parameter;
	auto state = field->statePtr[field->CurState];
	mchPBEM_GameID = atoi(state);

	mchPBEM_Game = 1;
	mchPBEM_DataFlag = 1;
	acsScrD -> QuantCode = 4;
	acsScrD -> flags |= ACS_NEED_EXIT;
}
