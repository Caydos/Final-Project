#include "Player.h"

Players::Player::Player()
{
}

Players::Player::~Player()
{
}

void Players::Player::Control(GameData* _gameData)
{
	Camera* camera = this->ped->GetCamera();
	if (this->ped != nullptr && camera != nullptr)
	{
		camera->ProcessMouseMovement(_gameData->window.xoffset, _gameData->window.yoffset, true, _gameData->dt);
		glm::vec3 horizontalFront = glm::normalize(glm::vec3(camera->Front.x, 0.0f, camera->Front.z));
		float velocity = this->ped->GetSpeed() * _gameData->dt;

		glm::vec3 velocityVec(.0f);
		if (_gameData->window.IsKeyPressed(Keys::W))
		{
			velocityVec += horizontalFront * velocity;
		}
		if (_gameData->window.IsKeyPressed(Keys::S))
		{
			velocityVec -= horizontalFront * velocity;
		}
		if (_gameData->window.IsKeyPressed(Keys::A))
		{
			velocityVec -= glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity;
		}
		if (_gameData->window.IsKeyPressed(Keys::D))
		{
			velocityVec += glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity;
		}
		if (_gameData->window.IsKeyPressed(Keys::SPACE))
		{
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
		{
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
		{
			this->ped->Run();
			velocityVec.x *= this->ped->GetRunningMultiplier();
			velocityVec.z *= this->ped->GetRunningMultiplier();
		}
		this->ped->PushVelocity(velocityVec, true);
		this->ped->Update();
	}
}
std::vector<Players::Player*> playerPool;

Peds::Ped* Players::Player::GetPed()
{
	return this->ped;
}

void Players::Player::SetPed(Peds::Ped* _ped)
{
	this->ped = _ped;
}

Players::Player* Players::Create()
{
	Player* player = new Player;
	playerPool.push_back(player);
	return player;
}

void Players::Erase(Player* _player)
{
}

std::vector<Players::Player*>* Players::GetPool()
{
	return nullptr;
}
