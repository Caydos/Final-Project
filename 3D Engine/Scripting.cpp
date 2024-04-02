#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "Maze.h"
#include "Collisions.h"
#include "Monster.h"
#include "LoadingScreen.h"
#include "FrustumCulling.h"
#include "Sprite.h"
#include "Audio.h"

static bool initialized = false;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lighting::Light* flashLight;
static Lighting::Light* directionalLight;
static Sets::Set* set;
static std::thread mazeThread;
static bool generated = false;
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

bool drawHelpers = false;
static Clock inputClock;

static Audio::Sound* footSteps;

void Generation()
{
	Clock loadingClock;
	loadingClock.Restart();
	Maze::GenerateMaze(3, 1);
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	Monster::GenerateMonster(); //Pop les mob
	generated = true;
}

void Scritping::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		footSteps = Audio::CreateSound();
		footSteps->LoadFromFile("../Sounds/Footsteps.wav");
		footSteps->Loop(true);

		Scene::Initialize(_gameData);
		Scene::World::SetSkyboxState(false);

		FPVCam = Scene::World::NewCamera(glm::vec3(3.0f, 1.2f, 3.0f));
		Scene::World::FocusCamera(_gameData, FPVCam);


		player = Players::Create();
		player->SetFootStepSound(footSteps);

		Peds::Ped* playerPed = Peds::Create();
		playerPed->Initialize();
		playerPed->GenerateRenderingInstance();
		playerPed->LoadFromJson(json::parse(Files::GetFileContent("../Sets/MC/Character.json")));
		playerPed->SetName("Character");
		playerPed->SetPath("../Sets/MC/");
		playerPed->SetCamera(_gameData->camera);
		playerPed->SetBodyType(Physics::Type::RIGID);
		player->SetPed(playerPed);

		playerPed->SetPosition(glm::vec3(3.650f, 1.850f, 1.1f), true);

		directionalLight = Scene::Lights::Create();
		directionalLight->SetType(Lighting::LightType::DIRECTIONAL);
		directionalLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		directionalLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		directionalLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		directionalLight->SetDirection(glm::vec3(0.0f, 1.0f, 0.0f));
		directionalLight->SetName("Directional");
		directionalLight->SetActive(true);

		flashLight = Scene::Lights::Create();
		flashLight->SetType(Lighting::LightType::SPOT);
		flashLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		flashLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		flashLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		flashLight->SetConstant(1.0f);
		flashLight->SetLinear(0.09f);
		flashLight->SetQuadratic(0.0032f);

		flashLight->SetCutOff(90.339f);
		flashLight->SetOuterCutOff(90.764f);
		flashLight->SetName("FlashLight");
		flashLight->SetActive(false);


		_gameData->window.Focus(false);

		//SPRITE
		title.Load("", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);
		background.Load("", glm::vec3(0, 0, 0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0), 1);

		textureTitle = new Texture;
		textureTitle->LoadFromFile("../Textures/Menu/Upstairs.png");
		textureBack = new Texture;
		textureBack->LoadFromFile("../Textures/Menu/Rien.png");

		animation.Load(0.5, "../Textures/Menu/UpstairsBug1.png", "../Textures/Menu/UpstairsBug2.png", "../Textures/Menu/UpstairsBug3.png");
		animation2.Load(0.5, "../Textures/Menu/Gresillement.png", "../Textures/Menu/Gresillement2.png");

		start.Load("../Textures/Menu/Start.png", glm::vec3(1500, 500, 0), glm::vec3(147, 58, 0), 1);
		options.Load("../Textures/Menu/Options.png", glm::vec3(1450, 665, 0), glm::vec3(247, 69, 0), 1);
		quit.Load("../Textures/Menu/Quit.png", glm::vec3(1500, 830, 0), glm::vec3(136, 58, 0), 1);
		selection.Load("../Textures/Menu/Selection.png", glm::vec3(1700, 500, 0), glm::vec3(49, 67, 0), 1);


		tempClock.Restart();

		mazeThread = std::thread(Generation);
		mazeThread.detach();

		initialized = true;
	}
	Scene::Tick(_gameData);
	if (generated)
	{
		if (_gameData->window.IsFocused())
		{
			player->Control(_gameData);
			Peds::Simulate(_gameData);
			//player->GetPed()->DrawBoundingBox();
			Monster::MovementMob(_gameData);




			// Cheat code
			if (_gameData->window.IsKeyPressed(Keys::F1))
			{
				player->GetPed()->SetBodyType(Physics::Type::GHOST);
			}
			if (_gameData->window.IsKeyPressed(Keys::F2))
			{
				player->GetPed()->SetBodyType(Physics::Type::RIGID);
			}
			if (_gameData->window.IsKeyPressed(Keys::F3))
			{
				//Respawn
			}
			if (_gameData->window.IsKeyPressed(Keys::F4) && inputClock.GetElapsedTime() > 125)
			{
				std::vector<Sets::Set*>* sets = Sets::GetAll();
				for (size_t i = 0; i < sets->size(); i++)
				{
					if (sets->at(i)->GetName() == "Books")
					{
						sets->at(i)->CalculateBoundingBox();
						sets->at(i)->DrawBoundingBox();
						player->GetPed()->SetPosition(sets->at(i)->GetPosition());
						break;
					}
				}
				inputClock.Restart();
			}

			if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
			{
				_gameData->window.Focus(false);
				animation.clock.Restart();
				tempClock.Restart();
				title.SetTexture(textureTitle);
				background.SetTexture(textureBack);
			}
			// Assuming your crosshair is at the center of the screen
			glm::vec4 ray_clip = glm::vec4(0.0, 0.0, -1.0, 1.0);

			// Convert to Eye Space
			glm::mat4 inv_projection = glm::inverse(Scene::World::GetProjection());
			glm::vec4 ray_eye = inv_projection * ray_clip;
			ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // Homogenize

			// Convert to World Space
			glm::mat4 inv_view = glm::inverse(Scene::World::GetView());
			glm::vec4 ray_wor = inv_view * ray_eye;
			// Don't forget to normalize the ray direction
			glm::vec3 ray_world_direction = glm::normalize(glm::vec3(ray_wor));

			RayCasting::Ray ray;
			glm::vec3 camPosition = Scene::World::GetCamera()->Position;
			ray.origin = camPosition;
			ray.direction = ray_world_direction;
			std::vector<Sets::Set*>* sets = Sets::GetAll();
			for (size_t i = 0; i < sets->size(); i++)
			{
				if (sets->at(i)->GetName() == "Books")
				{
					float rslt = RayCasting::Intersect(ray, sets->at(i)->GetBoundingBox());
					if (rslt != -1 && rslt < 1.0f)
					{
						std::cout << "Hover" << std::endl;
						if (_gameData->window.IsKeyPressed(Keys::E))
						{
							std::cout << "Key pressed" << std::endl;
							Sets::Erase(sets->at(i));
							break;
						}
					}
				}
			}
		}
		else
		{
			glm::vec2 mousePos = _gameData->window.GetCursorPosition();

			if (start.IsMouseOverQuad(mousePos))
			{
				selection.SetPosition(glm::vec3(1700, 500, 0));
				if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
				{
					_gameData->window.Focus(true);
				}
			}
			if (options.IsMouseOverQuad(mousePos))
			{
				selection.SetPosition(glm::vec3(1750, 665, 0));
				if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
				{
				}
			}
			if (quit.IsMouseOverQuad(mousePos))
			{
				selection.SetPosition(glm::vec3(1700, 830, 0));
				if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
				{
					_gameData->window.Close();
				}
			}

			title.SetTexture(textureTitle);
			background.SetTexture(textureBack);
			if (tempClock.GetElapsedTime() > 6000)
			{

				if ((animation.imageCourante == animation.textures.size() - 1) && (animation.clock.GetElapsedTime() / 1000 >= animation.tempsMax))
				{
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
		}
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}
}
