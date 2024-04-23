#include "Monster.h"

static std::vector<Monster::Monster*> monsterPool;
static Monster::Monster* monster = nullptr;

#define RIGHT_MONSTER glm::vec3(1,0,0);
#define LEFT_MONSTER glm::vec3(-1,0,0);
#define FRONT_MONSTER glm::vec3(0,0,1);
#define BACK_MONSTER glm::vec3(0,0,-1);
#define DIRECTION_FRONT 1;
#define DIRECTION_BACK 2;
#define DIRECTION_RIGHT 3;
#define DIRECTION_LEFT 4;


Monster::Monster::Monster()
{

}

Monster::Monster::~Monster()
{
}

void Monster::Monster::Update(GameData* _gameData)
{

	glm::vec3 currentPosition = this->ped->GetPosition();



	if (this->ped->GetCollision())
	{

		std::cout << "collision" << std::endl;

		int tempRandom = (rand() % 4) + 1;
		std::cout << tempRandom << std::endl;





		if (tempRandom == 3 && this->direction != 3)
		{
			this->velocity = RIGHT_MONSTER;
			this->direction = DIRECTION_RIGHT;
		}
		else if (tempRandom == 4 && this->direction != 4)
		{
			this->velocity = LEFT_MONSTER;
			this->direction = DIRECTION_LEFT;
		}
		else if (tempRandom == 1 && this->direction != 1)
		{
			this->velocity = FRONT_MONSTER;
			this->direction = DIRECTION_FRONT;
		}
		else if (tempRandom == 2 && this->direction != 2)
		{
			this->velocity = BACK_MONSTER;
			this->direction = DIRECTION_BACK;
		}
	}

	// Appliquer la nouvelle position basée sur la vélocité et le delta temps
	glm::vec3 newPosition = currentPosition + this->velocity * _gameData->dt;
	this->ped->SetPosition(newPosition, true);
}


void Monster::Monster::SetTargetPlayer(Players::Player* _player)
{
	targetPlayer = _player;
}

void Monster::Monster::SetVelocity(glm::vec3 _velocity)
{
	this->velocity = _velocity;
}

void Monster::Monster::SetDirection(int _direction)
{
	this->direction = _direction;
}


Peds::Ped* Monster::Monster::GetPed()
{
	return this->ped;
}

void Monster::Monster::SetPed(Peds::Ped* _ped)
{
	this->ped = _ped;
}

Monster::Monster* Monster::Create()
{
	Monster* monster = new Monster;
	monsterPool.push_back(monster);
	return monster;
}

void Monster::Erase(Monster* monster)
{
}

//std::vector<Monster::Monster> GetPool()
//{
//    return nullptr;
//}