#include "Levels.h"
#include "Ambiance.h"

static bool initialized = false;
static Levels::Level level = Levels::Level::HOSPITAL;
static void (*levels[Levels::Level::LEVELCOUNT])(GameData* _gameData);

void Levels::Initialize(GameData* _gameData)
{
	levels[Level::ENTRANCE] = &Entrance::Tick;
	levels[Level::HOSPITAL] = &Hospital::Tick;
	levels[Level::LIBRARY] = &Library::Tick;
	levels[Level::GARDEN] = &Garden::Tick;

	initialized = true;
}

void Levels::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	levels[level](_gameData);
	Ambiance::Tick(_gameData);
}

Levels::Level Levels::Get()
{
	return level;
}

void Levels::Set(Level _level)
{
	level = _level;
}
