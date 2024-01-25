#include "Editor.h"
#include "Files.h"
#include "Colors.h"
#include "Scene.h"
#include "Decor.h"
#include "Model.h"
#include "Clock.h"
#include "TexturePicker.h"
#include "ObjectsMenu.h"
#include "LightMenu.h"
#include "Crosshair.h"

static bool initialized = false;
static bool displayed = false;
static Clock inputClock;

void Editor::Initialize(GameData* _gameData)
{

	initialized = true;
}

bool Editor::IsDisplayed()
{
	return displayed;
}
void Editor::SetDisplay(bool _status)
{
	displayed = _status;
}

#pragma region Menu
static Editor::OutputFileType outputFileType;
static char filename[128] = ""; // Buffer for the filename
static bool showFilenameInput = false; // To toggle the visibility of input field
static float mainMenuBarHeight = 0;
static bool isLeftSideMenuOpen = true;
static bool isRightSideMenuOpen = true;

static Cube* lastItem = nullptr;

static Scene::World::Component* activeComponent = nullptr;

void Editor::Menu(GameData* _gameData)
{
	if (activeComponent != nullptr)
	{
		lastItem = activeComponent->GetRaycastHit();
	}
	else
	{
		lastItem = nullptr;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tool"))
		{
			if (ImGui::MenuItem("Test##ToolBar"))
			{
				Files::GetFilePath("");
			}
			if (ImGui::BeginMenu("Settings##ToolBar"))
			{
				if (ImGui::SliderFloat("Sensitivity##ToolBar", &_gameData->settings.sentivity, 0.01f, 1.5f))
				{
					_gameData->camera->MouseSensitivity = _gameData->settings.sentivity;
				}
				if (ImGui::MenuItem("Save##ToolBar"))
				{
					Settings::Save(_gameData);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Close##ToolBar"))
			{
				_gameData->window.Close();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("New", ""))
			{
				//if (ImGui::MenuItem("Scene"))
				//{
				//	outputFileType = Editor::SCENE;
				//	showFilenameInput = true;
				//}
				if (ImGui::MenuItem("Decor"))
				{
					outputFileType = Editor::DECOR;
					showFilenameInput = true;
				}
				if (ImGui::MenuItem("Model"))
				{
					outputFileType = Editor::MODEL;
					showFilenameInput = true;
				}
				//if (ImGui::MenuItem("Entity"))
				//{
				//	outputFileType = Editor::ENTITY;
				//	showFilenameInput = true;
				//}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Open", ""))
			{
				if (ImGui::BeginMenu("Decor", ""))
				{
					std::vector<std::string> availableFiles = Files::GetAllAtPath(DECOR_DIRECTORY);
					for (const auto& file : availableFiles)
					{
						if (ImGui::MenuItem(file.c_str()))
						{
							Decors::Decor* decor = new Decors::Decor(filename);
							size_t lastindex = file.find_last_of(".");

							std::string rawname = file.substr(0, lastindex);
							if (decor->LoadFromFile(rawname.c_str()))
							{
								Scene::World::InsertComponent(decor);
								activeComponent = decor;
							}
							else
							{
								delete decor;
								std::cout << "File opening error for file : " << file << std::endl;
							}
						}
					}
					ImGui::EndMenu();
				}
				else if (ImGui::BeginMenu("Models", ""))
				{
					std::vector<std::string> availableFiles = Files::GetAllAtPath(MODEL_DIRECTORY);
					for (const auto& file : availableFiles)
					{
						if (ImGui::MenuItem(file.c_str()))
						{
							Models::Model* model = new Models::Model(filename);
							size_t lastindex = file.find_last_of(".");

							std::string rawname = file.substr(0, lastindex);
							if (model->LoadFromFile(rawname.c_str()))
							{
								Scene::World::InsertComponent(model);
								activeComponent = model;
							}
							else
							{
								delete model;
								std::cout << "File opening error for file : " << file << std::endl;
							}
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (activeComponent != nullptr)
			{
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					activeComponent->Save();
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
		mainMenuBarHeight = ImGui::GetItemRectSize().y;
	}

	glm::ivec2 windowDimensions = _gameData->window.GetDimensions();
	ImGui::SetNextWindowPos(ImVec2(0, mainMenuBarHeight));
	ImGui::SetNextWindowSize(ImVec2(windowDimensions.x * 0.2, windowDimensions.y - mainMenuBarHeight));

	if (ImGui::Begin("Configuration", &isLeftSideMenuOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::TreeNode("Scene Configuration"))
		{
			if (ImGui::TreeNode("Background Color"))
			{
				ImVec4 actualColor = Scene::GetClearColor().imguiValues;
				for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
				{
					ImGui::PushID(i);
					if (ImGui::ColorButton("##DefaultColors", Colors::DefaultColors[i]))
					{
						actualColor = Colors::DefaultColors[i];
						Scene::SetClearColor(Colors::Color(actualColor.x, actualColor.y, actualColor.z, actualColor.w));
					}
					ImGui::PopID();
					ImGui::SameLine();
				}
				ImGui::NewLine();
				if (ImGui::ColorPicker4("Custom Color", (float*)&actualColor))
				{
					Scene::SetClearColor(Colors::Color(actualColor.x, actualColor.y, actualColor.z, actualColor.w));
				}
				ImGui::TreePop();
			}

			LightMenu::Basic(_gameData);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera Configuration"))
		{
			int cameraFov = Scene::World::GetCameraFov();
			ImGui::Text("FOV :");
			if (ImGui::SliderInt("##FOV", &cameraFov, 60, 90))
			{
				Scene::World::SetCameraFov(cameraFov);
			}
			Camera* cam = Scene::World::GetCamera();
			ImGui::Text("Speed :");
			ImGui::SliderFloat("##CamSpeed", &cam->MovementSpeed, 0.01f, 20.0f);

			if (ImGui::Button("Respawn"))
			{
				cam->Position = glm::vec3(.0f);
			}

			ImGui::TreePop();
		}
		Crosshairs::Menu(_gameData);

		if (lastItem != nullptr)
		{
			ObjectsMenu::Basic(_gameData, lastItem);
		}

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(windowDimensions.x - windowDimensions.x * 0.2, mainMenuBarHeight));
	ImGui::SetNextWindowSize(ImVec2(windowDimensions.x * 0.2, windowDimensions.y - mainMenuBarHeight));
	if (ImGui::Begin("Projects", &isRightSideMenuOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		std::vector<Scene::World::Component*>* components = Scene::World::GetComponents();
		if (components->size() > 0)
		{
			for (auto it : *components)
			{
				if (it != nullptr)
				{
					if (it == activeComponent)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1)); // Yellow color for the active component
					}

					bool node_open = ImGui::TreeNode((it->GetName() + "##" + std::to_string((int)it)).c_str());
					if (ImGui::IsItemClicked())
					{
						activeComponent = it;
					}
					if (node_open)
					{
						if (ImGui::Button((std::string("Select##") + std::to_string((int)it)).c_str()))
						{
							activeComponent = it;
						}

						ImGui::PopStyleColor();
						if (ImGui::TreeNode((std::string("Parameters##") + std::to_string((int)it)).c_str()))
						{
							ImGui::Text("Show : ");

							ImGui::SameLine();
							bool visible = it->IsVisible();
							if (ImGui::Checkbox((std::string("Toggle##") + std::to_string((int)it)).c_str(), &visible))
							{
								it->SetVisible(visible);
							}

							ImGui::TreePop();
						}
						ImGui::NewLine();
						float fullWidth = ImGui::GetContentRegionAvail().x;
						if (ImGui::Button((std::string("Save##") + std::to_string((int)it)).c_str(), ImVec2(fullWidth, 0)))
						{
							it->Save();
						}
						if (ImGui::Button((std::string("Close##") + std::to_string((int)it)).c_str(), ImVec2(fullWidth, 0)))
						{
							if (it == activeComponent)
							{
								activeComponent = nullptr;
							}
							Scene::World::RemoveComponent(it);
						}

						ImGui::TreePop();
					}
					ImGui::PopStyleColor();
				}
			}
		}
		else
		{
			ImGui::Text("No components available.");
		}
		ImGui::End();
	}
	if (showFilenameInput)
	{
		ImGui::OpenPopup("Enter Filename");
	}

	if (ImGui::BeginPopupModal("Enter Filename", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
		if (ImGui::Button("OK"))
		{
			switch (outputFileType)
			{
			case Editor::SCENE: { break; }
			case Editor::DECOR:
			{
				Decors::Decor* decor = new Decors::Decor(filename);
				Scene::World::InsertComponent(decor);
				Cube testCube;
				testCube.GenerateGraphicsBuffers();
				testCube.SetColor(Colors::Black);
				testCube.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
				decor->InsertObject(testCube);
				decor->Save();
				activeComponent = decor;
				break;
			}
			case Editor::MODEL:
			{
				Models::Model* model = new Models::Model(filename);
				Scene::World::InsertComponent(model);
				Cube testCube;
				testCube.GenerateGraphicsBuffers();
				testCube.SetColor(Colors::Black);
				testCube.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
				model->InsertObject(testCube);
				model->Save();
				activeComponent = model;
				break;
			}
			case Editor::ENTITY: { break; }
			default:
				break;
			}

			showFilenameInput = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			showFilenameInput = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

#pragma endregion


void Editor::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	Cube* rayCastHit = nullptr;
	if (activeComponent != nullptr && activeComponent->IsVisible())
	{
		rayCastHit = activeComponent->Raycast();
	}
	if (!Editor::IsDisplayed() && !TexturePicker::IsActive()
		&& activeComponent != nullptr && rayCastHit != nullptr
		&& activeComponent->IsVisible())
	{
		if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_LEFT) && inputClock.GetElapsedTime() > 125)
		{
			Cube* hotBarItem = TexturePicker::GetHotBarItem();

			Material* material = hotBarItem->GetMaterial();
			if (material != nullptr)
			{
				Cube insertedObj;
				insertedObj.GenerateGraphicsBuffers();
				insertedObj.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
				insertedObj.SetMaterial(material, true);
				insertedObj.SetPosition(rayCastHit->GetPosition());
				activeComponent->InsertObject(insertedObj);
			}
			inputClock.Restart();
		}
		else if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_RIGHT) && inputClock.GetElapsedTime() > 125)
		{
			Cube* hit = activeComponent->GetRaycastHit();
			activeComponent->RemoveObject(hit);
			inputClock.Restart();
		}
	}
	if (_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT) && inputClock.GetElapsedTime() > 125)
	{
		if (_gameData->window.yScroll > 0)
		{
			activeComponent->Move(glm::vec3(.0f, .0f, activeComponent->GetScale()));
			inputClock.Restart();
		}
		else if (_gameData->window.yScroll < 0)
		{
			activeComponent->Move(glm::vec3(.0f, .0f, -activeComponent->GetScale()));
			inputClock.Restart();
		}
	}
}

void Editor::Close()
{
	lastItem = nullptr;
	ObjectsMenu::CleanUp();
	LightMenu::CleanUp();
}