#include "Hospital.h"
#include "Sprite.h"
#include "Interaction.h"

static bool initialized = false;
static bool displayKeyPad = false;
static Sprite keypadSprites[13];
static Map::Stage* stage = nullptr;
static const glm::vec2 keypadBackSize(486,525);
static const glm::vec2 keypadPosition(1920 / 2 - keypadBackSize.x,1080 / 2 - keypadBackSize.y);
static const glm::vec2 keypadKeySize(133.5, 117.75);

static const char* keyNames[] = {
	"Button 1.jpg",
	"Button 2.jpg",
	"Button 3.jpg",
	"Button 4.jpg",
	"Button 5.jpg",
	"Button 6.jpg",
	"Button 7.jpg",
	"Button 8.jpg",
	"Button 9.jpg",

	"Button ast.jpg",
	"Button 0.jpg",
	"Button #.jpg",
};

void InitializeKeyPad()
{
	GameData* gameData = GetGameData();
	keypadSprites[0].Load("../Textures/Keypad/Background.jpg", glm::vec3(keypadPosition,0.0), glm::vec3(keypadBackSize, 0.0), 1);
	keypadSprites[0].BindShader(gameData->shaders[Shaders::UI]);

	const float padding = 22.5;
	for (size_t i = 0; i < 12; i++)
	{
		int row = i / 3;
		int column = i % 3;
		Texture* texture = new Texture;
		texture->LoadFromFile((std::string("../Textures/Keypad/") + keyNames[i]).c_str());
		keypadSprites[i + 1].SetTexture(texture);
		keypadSprites[i + 1].SetScale(glm::vec3(keypadKeySize, 0.0f));
		keypadSprites[i + 1].SetPosition(glm::vec3(padding + column * (keypadKeySize.x + padding) + keypadPosition.x, padding / 2+  row * (keypadKeySize.y + padding / 2) + keypadPosition.y, 0.0));
		keypadSprites[i + 1].BindShader(gameData->shaders[Shaders::UI]);
	}
}

void KeyPadInteraction(char** _args)
{
	displayKeyPad = true;
	GetGameData()->window.Focus(false);
}

void Hospital::RegisterInteractions()
{
	Interactions::Register("KeyPad", &KeyPadInteraction);
}

void Hospital::Initialize(GameData* _gameData)
{
	if (initialized) { return; }
	InitializeKeyPad();
	//Players::Player* player = Scripting::GetPlayer();
	//stage = Map::GetStage(Map::StageEnum::HOSPITALS);
	//if (stage == nullptr)
	//{
	//	Logger::Write("Unable to find hospital level stage\n");
	//	return;
	//}
	//for (size_t chunkId = 0; chunkId < stage->chunckList.size(); chunkId++)
	//{
	//	std::vector<Map::Decor> decors = stage->chunckList[chunkId].decor;
	//	for (size_t i = 0; i < decors.size(); i++)
	//	{
	//		std::cout << decors[i].name << std::endl;
	//	}
	//}


	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }



	if (displayKeyPad)
	{
		for (size_t spriteId = 0; spriteId < 13; spriteId++)
		{
			keypadSprites[spriteId].Draw();
		}
		if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
		{
			displayKeyPad = false;
			_gameData->window.Focus(false);
		}
	}
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}
