#pragma once

#include <memory>

namespace network
{

class OnlineGameSetup final
{
public:
	static OnlineGameSetup &get();

	enum class Command
	{
		enter_screen = 0,
		exit_screen,
		next_game,
		previous_game,
		start_game
	};

	void handle_command(Command command);
	void quant();

private:
	OnlineGameSetup();

	void start_request();
	void stop_request();
	void process_request();

	void set_next_game();
	void set_previous_game();
	void start_game();

	struct Properties;
	std::unique_ptr<Properties> _properties;
};

}
