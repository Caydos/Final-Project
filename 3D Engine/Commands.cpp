#include "Commands.h"
#include "Common.h"
#include "Network.h"

void Commands::Thread(bool _running)
{
	std::string command;
	while (_running)
	{
		std::getline(std::cin, command);

		// Process the command
		if (command == "connect")
		{
			if (!Network::Connection::Create("51.178.46.32", 55301))
			{
				Logger::Write("Failed to connect");
				//Hud::Notify("Failed to connect", 6);
			}
		}
		command.clear();
	}
}
