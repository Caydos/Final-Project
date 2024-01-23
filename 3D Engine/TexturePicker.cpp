#include "TexturePicker.h"
#include "Quad.h"
#include "Clock.h"
#include "Material.h"
#define HOT_BAR_ITEMS_COUNT 10

static bool initialized = false;
static Quad pickerBackground;
static std::vector<Quad> invColliders;
static std::vector<Cube> invCubes;
static const float defaultInvCubeScale = .05f;

static Quad hotBar[HOT_BAR_ITEMS_COUNT];
static Cube hotBarCubes[HOT_BAR_ITEMS_COUNT];
static int selectedObjectId = -1;
static int hotbarObjectId = -1;
static const float defaultHotBarCubeScale = .03f;
static float defaultHotBarScale;
static int hotBarHandId = 0;
static Colors::Color hotBarHandColor = Colors::BrightGrey;

static bool isMenuDisplayed = false;
static const int menuColumnCount = 8;
static Colors::Color UIColor(TO_RGBA(50), TO_RGBA(57), TO_RGBA(61));
static Clock inputClock;


void TexturePicker::Initialize(GameData* _gameData, std::vector<Material>* _materials)
{
	inputClock.Restart();
	glm::vec2 dimensions = _gameData->window.GetDimensions();

	pickerBackground.Initialize();
	pickerBackground.BindShader(_gameData->shaders[Shaders::UI]);
	pickerBackground.SetScale(dimensions.x * 0.75, dimensions.y * 0.75, 0.0f);
	pickerBackground.SetPosition(dimensions.x / 2.0f, dimensions.y * 0.45f, 0.0f);
	pickerBackground.SetColor(UIColor);
	pickerBackground.SetOpacity(0.8f);

	glm::vec3 scale = pickerBackground.GetScale();
	glm::vec3 position = pickerBackground.GetPosition();
	float horizontalSpacing = scale.x / menuColumnCount;
	float verticalSpacing = scale.x * 0.15;
	defaultHotBarScale = scale.x * 0.06;
	for (size_t i = 0; i < HOT_BAR_ITEMS_COUNT; i++)
	{
		hotBar[i].Initialize();
		hotBar[i].BindShader(_gameData->shaders[Shaders::UI]);
		hotBar[i].SetScale(defaultHotBarScale, defaultHotBarScale, 0.0f);

		int row = i / HOT_BAR_ITEMS_COUNT;
		int col = i % HOT_BAR_ITEMS_COUNT;
		float horizontalSpacing = scale.x / (HOT_BAR_ITEMS_COUNT + .0f);
		float xPosition = (position.x - scale.x / 2) + col * horizontalSpacing + horizontalSpacing / 2;

		hotBar[i].SetPosition(xPosition, dimensions.y * 0.925f, 1.0f);
		hotBar[i].SetColor(UIColor);
		hotBar[i].SetOpacity(0.8f);

		hotBarCubes[i].GenerateGraphicsBuffers();
		hotBarCubes[i].BindShader(_gameData->shaders[Shaders::UI_OBJECT]);
		//hotBarCubes[i].SetMaterial(&_materials->at(i), true);
		hotBarCubes[i].SetScale(defaultHotBarCubeScale, defaultHotBarCubeScale, defaultHotBarCubeScale);
		hotBarCubes[i].SetPositionFromScreenPoint(xPosition, dimensions.y * 0.925f);
	}
	hotBar[hotBarHandId].SetColor(hotBarHandColor);

	invColliders.resize(_materials->size());
	invCubes.resize(_materials->size());
	for (int index = 0; index < _materials->size(); index++)
	{
		invColliders[index].Initialize();
		invColliders[index].BindShader(_gameData->shaders[Shaders::UI]);
		invColliders[index].SetTexture(((*_materials)[index].GetTexture()));
		invColliders[index].SetScale(scale.x * 0.1, scale.x * 0.1, 0.0f);

		invCubes[index].GenerateGraphicsBuffers();
		invCubes[index].BindShader(_gameData->shaders[Shaders::UI_OBJECT]);

		invCubes[index].SetMaterial(&_materials->at(index), true);

		invCubes[index].SetScale(defaultInvCubeScale, defaultInvCubeScale, defaultInvCubeScale);

		// Calculate row and column for current element
		int row = index / menuColumnCount;
		int col = index % menuColumnCount;

		// Calculate centered position
		float xPosition = (position.x - scale.x / 2) + col * horizontalSpacing + horizontalSpacing / 2;
		float yPosition = (position.y - scale.y / 2) + row * verticalSpacing + verticalSpacing / 2;

		invColliders[index].SetPosition(xPosition, yPosition, 1.0f);
		invCubes[index].SetPositionFromScreenPoint(xPosition, yPosition);
	}


	initialized = true;
}

void TexturePicker::Render(GameData* _gameData)
{
	Shaders::Shader* shader = _gameData->shaders[Shaders::UI_OBJECT];
	shader->use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), _gameData->resolution[0] / _gameData->resolution[1], 0.1f, 100.0f);
	shader->setMat4("projection", projection);

	glm::vec2 cursorPos = _gameData->window.GetCursorPosition();
	for (size_t i = 0; i < HOT_BAR_ITEMS_COUNT; i++)
	{
		if (hotBar[i].IsMouseOverQuad(cursorPos) && selectedObjectId != -1)
		{
			if (hotbarObjectId != -1 && hotbarObjectId != i)
			{
				hotBar[hotbarObjectId].SetScale(defaultHotBarScale, defaultHotBarScale, defaultHotBarScale);
			}
			hotbarObjectId = i;
			if (hotBar[i].GetScale().x < 2 * defaultHotBarScale)
			{
				float amount = _gameData->dt * 600;
				hotBar[i].Scale(glm::vec3(amount));
			}
		}
		else if (hotBar[i].GetScale().x > defaultHotBarScale && i != hotbarObjectId)
		{
			float amount = _gameData->dt * 600;
			hotBar[i].Scale(glm::vec3(-amount));
			if (hotBar[i].GetScale().x < defaultHotBarScale)
			{
				hotBar[i].SetScale(defaultHotBarScale, defaultHotBarScale, defaultHotBarScale);
			}
		}
		else if (i == hotbarObjectId)
		{
			hotbarObjectId = -1;
		}

		hotBar[i].Draw();
		if (hotBarCubes[i].GetTexture() != nullptr)
		{
			shader->use();
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			hotBarCubes[i].Draw();
			glDisable(GL_DEPTH_TEST);
		}
	}
	if (TexturePicker::IsActive())
	{
		//pickerBackground.GetShader()->use();
		pickerBackground.Draw();
		shader->use();

		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		for (size_t i = 0; i < invColliders.size(); i++)
		{
			// Rotations
			invCubes[i].Rotate(glm::vec3(5.0f * _gameData->dt));

			// Collisions
			if (invColliders[i].IsMouseOverQuad(cursorPos) && selectedObjectId == -1)
			{
				if (_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
				{
					selectedObjectId = i;
				}

				if (invCubes[i].GetScale().x < 2 * defaultInvCubeScale)
				{
					float amount = _gameData->dt / 2;
					invCubes[i].Scale(glm::vec3(amount));
				}
			}
			else if (invColliders[i].GetScale().x > defaultInvCubeScale && i != selectedObjectId)
			{
				float amount = _gameData->dt / 2;
				invCubes[i].Scale(glm::vec3(-amount));
				if (invCubes[i].GetScale().x < defaultInvCubeScale)
				{
					invCubes[i].SetScale(defaultInvCubeScale, defaultInvCubeScale, defaultInvCubeScale);
				}
			}
			else if (i == selectedObjectId && !_gameData->window.IsKeyPressed(Keys::MOUSE_BUTTON_LEFT))
			{
				if (hotbarObjectId != -1)
				{
					hotBarCubes[hotbarObjectId].SetMaterial(invCubes[selectedObjectId].GetMaterial(), true);
				}

				glm::vec3 pos = invColliders[selectedObjectId].GetPosition();
				invCubes[selectedObjectId].SetPositionFromScreenPoint(pos.x, pos.y);
				selectedObjectId = -1;
			}
			invCubes[i].Draw();
		}
		glDisable(GL_DEPTH_TEST);
	}
	glEnable(GL_DEPTH_TEST);

	if (selectedObjectId > -1)
	{
		glm::vec2 cursorPos = _gameData->window.GetCursorPosition();
		invCubes[selectedObjectId].SetPositionFromScreenPoint(cursorPos.x, cursorPos.y);
	}
}

void TexturePicker::Inputs(GameData* _gameData)
{
	if (inputClock.GetElapsedTime() > 50.0f)
	{
		if (_gameData->window.yScroll < .0f)
		{
			if (hotBarHandId < 9)
			{
				hotBar[hotBarHandId].SetColor(UIColor);
				hotBarHandId++;
				hotBar[hotBarHandId].SetColor(hotBarHandColor);
			}
			inputClock.Restart();
		}
		else if (_gameData->window.yScroll > .0f)
		{
			if (hotBarHandId)
			{
				hotBar[hotBarHandId].SetColor(UIColor);
				hotBarHandId--;
				hotBar[hotBarHandId].SetColor(hotBarHandColor);
			}
			inputClock.Restart();
		}
	}
	if (inputClock.GetElapsedTime() > 125.0f)
	{
		if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_1))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 0;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_2))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 1;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_3))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 2;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_4))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 3;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_5))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 4;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_6))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 5;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_7))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 6;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_8))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 7;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_9))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 8;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
		else if (_gameData->window.IsKeyPressed(Keys::NUMBER_ROW_0))
		{
			hotBar[hotBarHandId].SetColor(UIColor);
			hotBarHandId = 9;
			hotBar[hotBarHandId].SetColor(hotBarHandColor);
		}
	}
}

bool TexturePicker::IsActive(void)
{
	return isMenuDisplayed;
}

void TexturePicker::SetActive(bool _status)
{
	for (size_t i = 0; i < invColliders.size(); i++)
	{
		invCubes[i].SetRotation(.0f, .0f, .0f);
	}
	isMenuDisplayed = _status;
}

Cube* TexturePicker::GetHotBarItem()
{
	return &hotBarCubes[hotBarHandId];
}
