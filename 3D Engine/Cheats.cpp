#include "Cheats.h"
#include "Scripting.h"

void Cheats::Tick(GameData* _gameData)
{
	Players::Player* player = Scripting::GetPlayer();
	// Cheat code
	if (_gameData->window.IsKeyPressed(Keys::F1))
	{
		player->GetPed()->SetBodyType(Physics::Type::GHOST);
	}
	if (_gameData->window.IsKeyPressed(Keys::F2))
	{
		player->GetPed()->SetBodyType(Physics::Type::RIGID);
	}
	if (_gameData->window.IsKeyPressed(Keys::F3))
	{
		std::cout << player->GetPed()->GetPosition().x << " " << player->GetPed()->GetPosition().y << " " << player->GetPed()->GetPosition().z << std::endl;
	}
	if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
	{
		PauseMenu::Open();
	}
}
