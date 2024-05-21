#include "MenuOptions.h"
#include "Sprite.h"
#include "Audio.h"
#include "MainMenu.h"
#include "PauseMenu.h"

static bool initialized = false;
static bool displayed = true;
static AnimationM animation;
static AnimationM animation2;
static Sprite options;
static Sprite back;
static Sprite selection;
static Sprite bouton1;
static Sprite bouton2;
static Sprite background;
static Clock tempClock;



void MenuOptions::Initialize(GameData* _gameData)
{
	//SPRITE
	options.Load("../Textures/Options/Options.png", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);
	background.Load("../Textures/Menu/MenuBackground.png", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);
	back.Load("../Textures/Options/Back.png", glm::vec3(1463, 830, 0), glm::vec3(144, 55, 0), 1);
	bouton1.Load("../Textures/Options/Bouton.png", glm::vec3(1450, 665, 0), glm::vec3(103, 105, 0), 1);
	bouton2.Load("../Textures/Options/Bouton.png", glm::vec3(1500, 830, 0), glm::vec3(103, 105, 0), 1);
	selection.Load("../Textures/Options/Fleche.png", glm::vec3(1700, 500, 0), glm::vec3(49, 67, 0), 1);

	tempClock.Restart();
	Open();

	if (MainMenu::IsOpen() == true)
	{
		MenuOptions::Close();
		GetGameData()->window.Focus(false);
	}
	else
	{
		MenuOptions::Open();
	}

	initialized = true;
}

void MenuOptions::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	if (displayed)
	{
		glm::vec2 mousePos = _gameData->window.GetCursorPosition();

		if (bouton1.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(1700, 500, 0));
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
			}
			else
			{
			}
		}
		if (bouton2.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(1750, 665, 0));
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{

			}
			else
			{
			}
		}
		if (back.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(1700, 830, 0));
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (PauseMenu::GetOpen())
				{
					MenuOptions::Close();
					PauseMenu::Open();
					PauseMenu::SetOpen(false);
					std::this_thread::sleep_for(std::chrono::milliseconds(125));

				}
				else if (MainMenu::GetOpen())
				{
					MenuOptions::Close();
					MainMenu::Open();
					MainMenu::SetOpen(false);
					std::this_thread::sleep_for(std::chrono::milliseconds(125));
				}
			}
			else
			{
			}
		}


		if (MainMenu::GetOpen())
		{
		background.Draw();
		}
		options.Draw();
		bouton1.Draw();
		bouton2.Draw();
		selection.Draw();
		back.Draw();
	}
}

void MenuOptions::CleanUp()
{
}


void MenuOptions::Open()
{
	GetGameData()->window.Focus(false);
	animation.clock.Restart();
	tempClock.Restart();
	displayed = true;
}

void MenuOptions::Close()
{
	GetGameData()->window.Focus(true);
	displayed = false;
}

bool MenuOptions::IsOpen()
{
	if (displayed == true)
	{
		return true;
	}
}