#include "Sync.h"
#include "Network.h"
#include "Peds.h"
#include "Scripting.h"
#include "Hospital.h"
static std::thread syncThread;

void Synchronization::Loop(void)
{
	while (true)
	{/*always send player data and each X time send entity data as encrypted tags*/
		if (GetGameData()->window.IsFocused())
		{
			Players::Player* player = Scripting::GetPlayer();
			if (player != nullptr)
			{
				Peds::Ped* playerPed = Scripting::GetPlayerPed();
				if (playerPed != nullptr)
				{
					glm::vec3 pedPos = playerPed->GetPosition();
					glm::vec3 pedRot = playerPed->GetRotation();
					Network::Events::TriggerServer("Sync::PlayerUpdate", pedPos.x, pedPos.y, pedPos.z, pedRot.y);
					std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_TIME));
				}
			}
		}
	}
}


static int serverId = -1;
void Established(char** _args)
{
	char* message = _args[0];
	serverId = ToInt(_args[1]);

	std::cout << message << std::endl;
	std::cout << "My server id is : " << serverId << "." << std::endl;

	syncThread = std::thread(&Synchronization::Loop);
	syncThread.detach();
}

void Synchronization::PedSyncEvents(void)
{
	Network::Events::Register("Connections::Established", &Established);
	Network::Events::Register("Sync::PlayerUpdate", &Scripting::PlayerUpdate);
	Network::Events::Register("Sync::ClownUpdate", &Hospital::ClownUpdate);
	Network::Events::Register("Sync::TeddyUpdate", &Hospital::TeddyUpdate);
}
int Synchronization::GetServerId()
{
	return serverId;
}

