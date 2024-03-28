

#ifndef MONSTER_H
#define MONSTER_H

#include "Common.h"
#include "Scene.h"
#include "Set.h"
#include "Bounds.h"
#include "Collisions.h"
#include "Maze.h"

namespace Monster
{
    Bounds::Box GetBoundingBox();
    void CreateMonster();
    void GenerateMonster();
    void UpdateMonster(GameData* _gameData);
    void MovementMob(GameData* _gameData);
}

#endif

