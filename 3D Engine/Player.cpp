#include "Player.h"

Players::Player::Player()
{
	this->footSteps = nullptr;
	this->footSteps2 = nullptr;
	this->fastBreathing = nullptr;
}

Players::Player::~Player()
{
}
const float sideSpeedMultiplier = 0.8f;

void Players::Player::SetFootStepSound(Audio::Sound* _sound)
{
	this->footSteps = _sound;
}

void Players::Player::SetFootStepSound2(Audio::Sound* _sound)
{
	this->footSteps2 = _sound;
}

void Players::Player::SetFastBreathing(Audio::Sound* _sound)
{
	this->fastBreathing = _sound;
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

		glm::vec3 pedPosition = this->ped->GetPosition();
		this->footSteps->SetPosition(pedPosition);
		this->footSteps2->SetPosition(pedPosition);
		this->fastBreathing->SetPosition(pedPosition);

		if (_gameData->window.IsKeyPressed(Keys::W))
		{
			velocityVec += horizontalFront * velocity;
			if (!this->footSteps->IsPlaying() && !_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
			{
				this->footSteps->Play();
			}
		}
		else
		{
			if (this->footSteps->IsPlaying())
			{
				this->footSteps->Pause();
			}
		}
		if (_gameData->window.IsKeyPressed(Keys::S))
		{
			velocityVec -= horizontalFront * velocity;
			if (!this->footSteps->IsPlaying() && !_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
			{
				this->footSteps->Play();
			}
		}
		if (_gameData->window.IsKeyPressed(Keys::A))
		{
			velocityVec -= glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity * sideSpeedMultiplier;
			if (!this->footSteps->IsPlaying() && !_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
			{
				this->footSteps->Play();
			}
		}
		if (_gameData->window.IsKeyPressed(Keys::D))
		{
			velocityVec += glm::normalize(glm::vec3(camera->Right.x, 0.0f, camera->Right.z)) * velocity * sideSpeedMultiplier;
			if (!this->footSteps->IsPlaying() && !_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
			{
				this->footSteps->Play();
			}
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
			this->ped->Run();
			velocityVec.x *= this->ped->GetRunningMultiplier();
			velocityVec.z *= this->ped->GetRunningMultiplier();
			if (!this->footSteps2->IsPlaying() && !this->fastBreathing->IsPlaying())
			{
				this->footSteps2->Play();
				this->fastBreathing->Play();
				this->footSteps->Pause();
			}
		}
		else
		{
			if (this->footSteps2->IsPlaying() && this->fastBreathing->IsPlaying())
			{
				this->footSteps2->Pause();
				this->fastBreathing->Pause();
			}
		}

		this->ped->PushVelocity(velocityVec, true);
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
