#include "MainMenu.h"
#include "Sprite.h"
#include "Audio.h"
#include "MenuOptions.h"

static bool initialized = false;
static bool displayed = true;
static bool beenOpen = true;
static AnimationM animation;
static AnimationM animation2;
static Sprite title;
static Sprite background;
static Sprite start;
static Sprite options;
static Sprite quit;
static Sprite selection;
static Clock tempClock;
static Texture* textureTitle;
static Texture* textureBack;


static Sprite creajeux;

static Audio::Sound* gresillement;
static Audio::Sound* beep;
static Audio::Sound* interfaceText;
static Audio::Sound* menuMusic;

void MainMenu::Initialize(GameData* _gameData)
{
	//SPRITE
	title.Load("", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);
	background.Load("", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);

	textureTitle = new Texture;
	textureTitle->LoadFromFile("../Textures/Menu/Upstairs.png");
	textureBack = new Texture;
	textureBack->LoadFromFile("../Textures/Menu/MenuBackground.png");

	animation.Load(0.5, "../Textures/Menu/UpstairsBug1.png", "../Textures/Menu/UpstairsBug2.png", "../Textures/Menu/UpstairsBug3.png");
	animation2.Load(0.5, "../Textures/Menu/Gresillement.png", "../Textures/Menu/Gresillement2.png");

	start.Load("../Textures/Menu/Start.png", glm::vec3(1500, 500, 0), glm::vec3(147, 58, 0), 1);
	options.Load("../Textures/Menu/Options.png", glm::vec3(1450, 665, 0), glm::vec3(247, 69, 0), 1);
	quit.Load("../Textures/Menu/Quit.png", glm::vec3(1500, 830, 0), glm::vec3(136, 58, 0), 1);
	selection.Load("../Textures/Menu/Selection.png", glm::vec3(1700, 500, 0), glm::vec3(49, 67, 0), 1);

	creajeux.Load("../Textures/Creajeux.png", glm::vec3(1777, 961,0), glm::vec3(150, 150, 0), 1);

	tempClock.Restart();
	Open();


	gresillement = Audio::CreateSound();
	gresillement->LoadFromFile("../Sounds/Gresillement.wav");
	gresillement->SetVolume(0.1f);

	beep = Audio::CreateSound();
	beep->LoadFromFile("../Sounds/Tampon.wav");
	beep->SetVolume(5.0f);

	interfaceText = Audio::CreateSound();
	interfaceText->LoadFromFile("../Sounds/UI/InterfaceSound.wav");
	interfaceText->SetVolume(5.0f);

	menuMusic = Audio::CreateSound();
	menuMusic->LoadFromFile("../Sounds/Hospital/Neon.wav");
	menuMusic->Loop(true);
	menuMusic->SetVolume(0.1f);
	//menuMusic->Play(); //s'entend partout

	initialized = true;
}

void MainMenu::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	if (displayed)
	{
		glm::vec2 mousePos = _gameData->window.GetCursorPosition();

		if (start.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(1700, 500, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				MainMenu::Close();
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
			selection.SetPosition(glm::vec3(1750, 665, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				MainMenu::Close();
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
		if (quit.IsMouseOverQuad(mousePos))
		{
			selection.SetPosition(glm::vec3(1700, 830, 0));
			interfaceText->Play();
			if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (!beep->IsPlaying())
				{
					beep->Play();
				}
				_gameData->window.Close();
			}
			else
			{
				if (beep->IsPlaying())
				{
					beep->Pause();
				}
			}
		}

		title.SetTexture(textureTitle);
		background.SetTexture(textureBack);
		if (tempClock.GetElapsedTime() > 6000)
		{

			if ((animation.imageCourante == animation.textures.size() - 1) && (animation.clock.GetElapsedTime() / 1000 >= animation.tempsMax))
			{
				//gresillement->Play();
				tempClock.Restart();
				animation.Animate(&title);
				title.SetTexture(textureTitle);
				animation2.Animate(&background);
				background.SetTexture(textureBack);


			}
			else
			{
				animation.Animate(&title);
				animation2.Animate(&background);
			}
		}


		background.Draw();
		title.Draw();
		start.Draw();
		quit.Draw();
		options.Draw();
		selection.Draw();
		creajeux.Draw();
	}
}

void MainMenu::CleanUp()
{
}


void MainMenu::Open()
{
	GetGameData()->window.Focus(false);
	animation.clock.Restart();
	tempClock.Restart();
	title.SetTexture(textureTitle);
	background.SetTexture(textureBack);
	displayed = true;
}

void MainMenu::Close()
{
	menuMusic->Pause();

	GetGameData()->window.Focus(true);
	displayed = false;
}

bool MainMenu::GetOpen()
{
	return beenOpen;
}

void MainMenu::SetOpen(bool _open)
{
	beenOpen = _open;
}

bool MainMenu::IsOpen()
{
	return displayed;
}