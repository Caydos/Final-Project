#include "Monster.h"

static std::vector<Monster::Monster*> monsterPool;
static std::vector<glm::vec3> oui;

Monster::Monster::Monster()
{
    // Initialisation de la direction de manière aléatoire
    //changeInX = (rand() % 2) == 0;
    //if (changeInX) 
    //{
    //    directionX = (rand() % 2) == 0 ? 0.1f : -0.1f; // Direction initiale aléatoire en X
    //}
    //else 
    //{
    //    directionZ = (rand() % 2) == 0 ? 0.1f : -0.1f; // Direction initiale aléatoire en Z
    //}
    //movementClock.Restart(); // Redémarrer l'horloge
}

Monster::Monster::~Monster()
{
}

//Methode 1: Move du mob juste sur l'axe X (tout droit)
//void Monster::Monster::Update(GameData* _gameData)
//{
//	//Déplacement
//	glm::vec3 currentPosition = this->ped->GetPosition();
//	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.1f);
//	currentPosition += velocity * _gameData->dt;
//	this->ped->SetPosition(currentPosition, true);
//}

//Methode 2 : Deplacement du mob qui suit le joueur (il s'en balec des murs donc longe et se coince)
//void Monster::Monster::Update(GameData* _gameData) 
//{
//    if (!this->ped || !targetPlayer) return;
//
//    //position du joueur
//    glm::vec3 playerPosition = targetPlayer->GetPed()->GetPosition();
//
//    //position du monstre
//    glm::vec3 monsterPosition = this->ped->GetPosition();
//
//    //Calculez la direction vers le joueur sur les axes x et z uniquement
//    glm::vec3 direction = glm::normalize(glm::vec3(playerPosition.x - monsterPosition.x, 0.0f, playerPosition.z - monsterPosition.z));
//
//    //vitesse de déplacement du monstre
//    float speed = 0.9f;
//
//    //nouveau déplacement
//    glm::vec3 movement = direction * speed * std::min(_gameData->dt, 1.0f);
//
//    // Mise à jour de  la position du monstre
//    this->ped->SetPosition(glm::vec3(monsterPosition.x + movement.x, monsterPosition.y, monsterPosition.z + movement.z), true);
//}

//Methode 3 : Changer la direction du mob aleatoirement sur l'axe X ou Z toutes les x secondes
//void Monster::Monster::Update(GameData* _gameData) 
//{
//    if (!this->ped) return;
//
//
//    //Si le timer depasse x secondes
//    if (movementClock.GetElapsedTime() >= 10000) 
//    {
//        movementClock.Restart(); //Reset la clock 
//
//        // Choisir aléatoirement de continuer dans la même direction ou de changer de direction
//        if (changeInX) 
//        {
//            directionX = (rand() % 2) == 0 ? directionX : -directionX;
//        }
//        else 
//        {
//            directionZ = (rand() % 2) == 0 ? directionZ : -directionZ;
//        }
//
//        // Choisir l'axe de changement x ou z
//        changeInX = !changeInX;
//        if (changeInX) 
//        {
//            directionX = (rand() % 2) == 0 ? 0.1f : -0.1f;
//            directionZ = 0.0f; // Assurer que le mouvement se fait seulement sur un axe à la fois
//        }
//        else 
//        {
//            directionZ = (rand() % 2) == 0 ? 0.1f : -0.1f;
//            directionX = 0.0f; // Assurer que le mouvement se fait seulement sur un axe à la fois
//        }
//    }
//
//   
//    glm::vec3 currentPosition = this->ped->GetPosition();
//    glm::vec3 movement = glm::vec3(directionX, 0.0f, directionZ) * std::min(_gameData->dt, 0.05f) ;
//    this->ped->SetPosition(currentPosition + movement, true);
//}

//Methode 5: Mettre des points de changement de direction a une certain distance
void Monster::Monster::Update(GameData* _gameData)
{

    //float distance = glm::distance(currentPosition, glm::vec3(3.65, currentPosition.y, 31.3));

    glm::vec3 currentPosition = this->ped->GetPosition();
    glm::vec3 velocity;

    oui.push_back(glm::vec3(3.65, 0, 6.2));
    oui.push_back(glm::vec3(3.65, 0, 31.3));
    oui.push_back(glm::vec3(28.75, 0, 31.3));
    oui.push_back(glm::vec3(28.75, 0, 6.2));

    // Définition explicite des points de décision pour le changement de direction
    const float eastLimit = 28.75f;
    const float westLimit = 3.65f;
    const float northLimit = 31.3f;
    const float southLimit = 6.2f;
    const float specialX = 16.2f; // Point spécial en X
    const float specialZ = 18.7f; // Point spécial en X


    // Au point spécial X, choisir aléatoirement de tourner ou continuer tout droit
    if (currentPosition.z >= northLimit && currentPosition.x < eastLimit)
    {
        velocity = glm::vec3(4.0f, 0.0f, 0.0f); // Vers l'est
        this->ped->SetRotation(0, 90, 0, true);
    }
    else if (currentPosition.x >= eastLimit && currentPosition.z > southLimit)
    {
        velocity = glm::vec3(0.0f, 0.0f, -4.0f); // Vers le sud
        this->ped->SetRotation(0, 180, 0, true);
    }
    else if (currentPosition.z <= southLimit && currentPosition.x > westLimit)
    {
        velocity = glm::vec3(-4.0f, 0.0f, 0.0f); // Vers l'ouest
        this->ped->SetRotation(0, 270, 0, true);
    }

    else if (currentPosition.x <= westLimit)
    {
        velocity = glm::vec3(0.0f, 0.0f, 0.5f); // Vers le nord
        this->ped->SetRotation(0, 0, 0, true);
    }
    //if (currentPosition.x >= specialX && currentPosition.x < specialX + 0.1) // Utiliser une petite marge pour le choix
    //{
    //    if ((rand() % 2) == 0) // 50% de chance de tourner
    //    {
    //        velocity = glm::vec3(0.0f, 0.0f, currentPosition.z > northLimit ? -4.0f : 4.0f); // Choisir la direction sur l'axe Z en fonction de la position actuelle
    //        this->ped->SetRotation(0, currentPosition.z > northLimit ? 180 : 0, 0, true);
    //    }
    //    else // Continuer tout droit
    //    {
    //    }
    //}
    //// Nouvelle logique pour le point spécial Z
    //if (currentPosition.z >= specialZ && currentPosition.z < specialZ + 0.1) // Utiliser une petite marge pour le choix
    //{
    //    if ((rand() % 2) == 0) // 50% de chance de tourner
    //    {
    //        velocity = glm::vec3(currentPosition.x > specialX ? -4.0f : 4.0f, 0.0f, 0.0f); // Choisir la direction sur l'axe X en fonction de la position actuelle
    //        this->ped->SetRotation(0, currentPosition.x > specialX ? 270 : 90, 0, true);
    //    }
    //    else // Continuer tout droit
    //    {
    //        //// Ajuster la direction pour continuer le long de l'axe Z si nécessaire
    //        //velocity = glm::vec3(0.0f, 0.0f, currentPosition.z < northLimit ? 4.0f : -4.0f);
    //        //this->ped->SetRotation(0, currentPosition.z < northLimit ? 0 : 180, 0, true);
    //    }
    //}


    // Appliquer la nouvelle position basée sur la vélocité et le delta temps
    glm::vec3 newPosition = currentPosition + velocity * _gameData->dt;
    this->ped->SetPosition(newPosition, true);
}


void Monster::Monster::SetTargetPlayer(Players::Player* _player)
{
    targetPlayer = _player;
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

std::vector<Monster::Monster*>* GetPool()
{
    return nullptr;
}

//bool Monster::Monster::MonsterInFreeCell()
//{
//    for (auto& chunk : maze) //Parcourir chaque chunk
//    {
//        for (auto& cell : chunk.stageList[0].chunk.cellList) 
//        {
//            bool freeCell = true;
//            for (auto& mur : cell.wallList)
//            { // Vérifier tous les murs de la cellule
//                if (mur.isVisible) 
//                { // Si un mur est visible, la cellule est pas libre
//                    freeCell = false;
//                    break;
//                }
//            }
//            if (freeCell) //Si une cellule libre
//            { 
//                // Positionner le monstre ici
//                return true;
//            }
//        }
//    }
//    return false;
//}