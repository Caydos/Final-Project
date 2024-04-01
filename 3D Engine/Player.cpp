#include "Player.h"

Players::Player::Player()
{
	this->footSteps = nullptr;
}

Players::Player::~Player()
{
}
const float sideSpeedMultiplier = 0.8f;

void Players::Player::SetFootStepSound(Audio::Sound* _sound)
{
	this->footSteps = _sound;
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
			velocityVec -= glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity * sideSpeedMultiplier;
		}
		if (_gameData->window.IsKeyPressed(Keys::D))
		{
			velocityVec += glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity * sideSpeedMultiplier;
		}
		if (_gameData->window.IsKeyPressed(Keys::SPACE) && jumpClock.GetElapsedTime() > 500/* && this->ped->GetBody().velocity.y == 0*/)
		{
			if (this->ped->GetBodyType() == Physics::Type::RIGID)
			{
				if (jumpClock.GetElapsedTime() > 500)
				{
					jumpClock.Restart();
					velocityVec.y += 0.0125;
				}
			}
			else
			{
				velocityVec.y += 0.0125;
			}
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
		{
			velocityVec.y -= velocity;
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
		{
			if (!this->footSteps->IsPlaying())
			{
				this->footSteps->Play();
			}
			this->ped->Run();
			velocityVec.x *= this->ped->GetRunningMultiplier();
			velocityVec.z *= this->ped->GetRunningMultiplier();
			this->footSteps->SetPosition(this->ped->GetPosition());
		}
		else
		{
			if (this->footSteps->IsPlaying())
			{
				this->footSteps->Pause();
			}
		}

		this->ped->PushVelocity(velocityVec, true);
		//this->ped->Update();
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
