#include "PauseMenu.h"
#include "Sprite.h"
#include "Audio.h"
#include "MainMenu.h"
#include "MenuOptions.h"

static bool initialized = false;
static bool displayed = true;
static bool beenOpen = false;
static Sprite title;
static Sprite background;
static Sprite back;
static Sprite back1;
static Sprite back2;
static Sprite selection;
static Sprite logo;
static Sprite options;
static Sprite resume;

static Audio::Sound* beep;
static Audio::Sound* interfaceText;

void PauseMenu::Initialize(GameData* _gameData)
{
	//SPRITE
	title.Load("../Textures/Pause/Pause.png", glm::vec3(56, 93, 0), glm::vec3(633, 170, 0), 1);
	background.Load("../Textures/Pause/Background.png", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);
	back1.Load("../Textures/Pause/Back1.png", glm::vec3(0, 0, 0), glm::vec3(894, 1080, 0), 1);
	back2.Load("../Textures/Pause/Back2.png", glm::vec3(0, 0, 0), glm::vec3(678, 1080, 0), 1);
	logo.Load("../Textures/Pause/Logo.png", glm::vec3(832, 20, 0), glm::vec3(1013, 1053, 0), 1);

	resume.Load("../Textures/Pause/Resume.png", glm::vec3(177, 468, 0), glm::vec3(275, 56, 0), 1);
	options.Load("../Textures/Pause/Options.png", glm::vec3(177, 640, 0), glm::vec3(265, 70, 0), 1);
	back.Load("../Textures/Pause/Back.png", glm::vec3(177, 820, 0), glm::vec3(471, 55, 0), 1);
	selection.Load("../Textures/Pause/Fleche.png", glm::vec3(100, 465, 0), glm::vec3(49, 67, 0), 1);


	beep = Audio::CreateSound();
	beep->LoadFromFile("../Sounds/Tampon.wav");
	beep->SetVolume(5.0f);

	interfaceText = Audio::CreateSound();
	interfaceText->LoadFromFile("../Sounds/UI/InterfaceSound.wav");
	interfaceText->SetVolume(5.0f);

	if (MainMenu::IsOpen() == true)
	{
		PauseMenu::Close();
		GetGameData()->window.Focus(false);
	}
	else
	{
		PauseMenu::Open();
	}

	initialized = true;
}

void PauseMenu::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	if (displayed)
	{
		glm::vec2 mousePos = _gameData->window.GetCursorPosition();


		if (resume.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(100, 465, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				PauseMenu::Close();
			}
			else
			{
				if (beep->IsPlaying())
				{
					beep->Pause();
				}
			}
		}
		if (options.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(100, 637, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				PauseMenu::Close();
				MenuOptions::Open();
				beenOpen = true;
			}
			else
			{
				if (beep->IsPlaying())
				{
					beep->Pause();
				}
			}
		}
		if (back.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(100, 817, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				PauseMenu::Close();
				MainMenu::Open();
			}
			else
			{
				if (beep->IsPlaying())
				{
					beep->Pause();
				}
			}
		}


		background.Draw();
		back1.Draw();
		back2.Draw();
		logo.Draw();
		title.Draw();
		resume.Draw();
		back.Draw();
		options.Draw();
		selection.Draw();
	}
	else
	{
		if (_gameData->window.IsKeyPressed(Keys::ESCAPE) && _gameData->window.IsFocused())
		{
			PauseMenu::Open();
		}
	}
}

void PauseMenu::CleanUp()
{
}

void PauseMenu::Open()
{
	GetGameData()->window.Focus(false);
	displayed = true;
}

void PauseMenu::Close()
{
	GetGameData()->window.Focus(true);
	displayed = false;
}

bool PauseMenu::GetOpen()
{
	return beenOpen;
}

void PauseMenu::SetOpen(bool _open)
{
	beenOpen = _open;
}
