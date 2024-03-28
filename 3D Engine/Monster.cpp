

#include "Monster.h"

static Sets::Set* monster;

Bounds::Box Monster::GetBoundingBox()
{
    return monster->GetBoundingBox();
}

void Monster::CreateMonster()
{
    monster = Sets::Create();
    monster->GenerateRenderingInstance();
    monster->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Props/HSP_Extinguisher.json")));
    monster->SetName("Monster");
    glm::vec3 initialPosition = glm::vec3(3.6, 0.3, 0.5);
    monster->SetPosition(initialPosition);
    monster->GetBoundingBox(); //Choper la boite englobante

}

void Monster::UpdateMonster(GameData* _gameData)
{
    glm::vec3 currentPosition = monster->GetPosition();
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.1f); //Init la vitesse en fonction de l'axe

    currentPosition += velocity * _gameData->dt; //Incrementer la vitesse pour le faire avancer

    //Nouvelle pos du mob
    monster->SetPosition(currentPosition);
}

void Monster::MovementMob(GameData* _gameData)
{
    Monster::UpdateMonster(_gameData);
}

void Monster::GenerateMonster()
{
    CreateMonster();
}