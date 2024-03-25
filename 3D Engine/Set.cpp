#include "Set.h"
#include "RayCasting.h"
#include "Scene.h"
#include "Clock.h"
#include "Inventory.h"
#include "Arrows.h"
#include <glm/gtx/matrix_decompose.hpp>

static std::vector<Sets::Set*> sets;
static Clock inputClock;
static Arrow arrows[3];
static bool initialized = false;
static bool selecting = false;
static Sets::Set* parentSet;


void Sets::UpdateVisibility()
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		sets[i]->CheckVisibility();
	}
}

Sets::Set* Sets::Create()
{
	Set* newSet = new Set;
	newSet->Initialize();
	sets.push_back(newSet);
	return newSet;
}

void Sets::Insert(Set* _set)
{
	sets.push_back(_set);
}

void Sets::Erase(Set* _set)
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		if (sets[i] == _set)
		{
			sets[i]->Erase();
			delete sets[i];
			sets.erase(sets.begin() + i);
			return;
		}
	}
}

std::vector<Sets::Set*>* Sets::GetAll()
{
	return &sets;
}

static bool holding = false;
static Blocks::Block* hittedOne = nullptr;
static glm::vec3 position;

void Sets::Edition(GameData* _gameData, bool _allowControls)
{
	if (!initialized)
	{
		arrows[0].GenerateGraphicsBuffers();
		arrows[0].BindShader(_gameData->shaders[Shaders::SINGLE_DRAW]);
		arrows[0].SetColor(Colors::Red);
		arrows[0].SetRotation(glm::vec3(180.0f, 0.0f, .0f));

		arrows[1].GenerateGraphicsBuffers();
		arrows[1].BindShader(_gameData->shaders[Shaders::SINGLE_DRAW]);
		arrows[1].SetColor(Colors::Green);
		arrows[1].SetRotation(glm::vec3(-90.0f, 0.0f, .0f));

		arrows[2].GenerateGraphicsBuffers();
		arrows[2].BindShader(_gameData->shaders[Shaders::SINGLE_DRAW]);
		arrows[2].SetColor(Colors::Blue);
		arrows[2].SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));
		initialized = true;
	}
	if (parentSet != nullptr)
	{
		arrows[0].SetPosition(parentSet->GetPosition());
		arrows[0].Draw();

		arrows[1].SetPosition(parentSet->GetPosition());
		arrows[1].Draw();

		arrows[2].SetPosition(parentSet->GetPosition());
		arrows[2].Draw();

		Colors::Color color = Colors::White;
		Shaders::Shader* shader = _gameData->shaders[Shaders::SINGLE_DRAW];
		shader->use();
		shader->setBool("instanceUsage", false);
		shader->setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader->setBool("lightDependent", false);
		shader->setInt("mode", 0);
		shader->setMat4("model", glm::mat4(1.0f));
		shader->setFloat("opacity", 1.0f);
		parentSet->DrawBoundingBox();
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

	RayCasting::Face hittedFace;
	float closest = 100.0f;

	if (!holding)
	{
		hittedOne = nullptr;
	}

	if (parentSet != nullptr && parentSet->IsVisible() && !holding)
	{
		std::vector<Blocks::Block>* setBlocks = parentSet->GetBlocks();

		for (size_t i = 0; i < setBlocks->size(); i++)
		{
			auto it = &setBlocks->at(i);
			float halfSize = it->GetScale().x / 2.0f; // Since scale is 1.0, half-size on each axis is 0.5

			glm::mat4 transformation = it->GetModel();
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transformation, scale, rotation, translation, skew, perspective);

			Bounds::Box boxAABB;
			boxAABB.min = translation - glm::vec3(halfSize, halfSize, halfSize);
			boxAABB.max = translation + glm::vec3(halfSize, halfSize, halfSize);
			float rslt = RayCasting::Intersect(ray, boxAABB);

			if (rslt > .0f && rslt < closest)
			{
				hittedOne = it;
				RayCasting::Cube cube = { translation, scale.x };
				hittedFace = IntersectedFace(ray, cube);
				closest = rslt;
				position = translation;
			}
		}
	}

	if (hittedOne != nullptr && parentSet != nullptr && _allowControls)
	{
		Blocks::BlockType* blockType = Inventory::GetHotBarBlock();

		if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_LEFT) && inputClock.GetElapsedTime() > 125 && !holding)
		{
			parentSet->RemoveBlock(hittedOne);
			inputClock.Restart();
			return;
		}
		if (blockType == nullptr) { return; }
		if (!holding)
		{
			switch (hittedFace)
			{
			case RayCasting::FRONT:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, -1);
				position.z += (hittedOne->GetScale().z / 2.0f + blockType->GetScale().z / 2.0f);
				break;
			}
			case RayCasting::BACK:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, 1);
				position.z -= (hittedOne->GetScale().z / 2.0f + blockType->GetScale().z / 2.0f);
				break;
			}
			case RayCasting::LEFT:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, 1);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, 0);
				position.x -= (hittedOne->GetScale().x / 2.0f + blockType->GetScale().x / 2.0f);
				break;
			}
			case RayCasting::RIGHT:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, -1);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, 0);
				position.x += (hittedOne->GetScale().x / 2.0f + blockType->GetScale().x / 2.0f);
				break;
			}
			case RayCasting::TOP:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, -1);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, 0);
				position.y += (hittedOne->GetScale().y / 2.0f + blockType->GetScale().y / 2.0f);
				break;
			}
			case RayCasting::BOTTOM:
			{
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::X, 0);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Y, 1);
				Blocks::Ghost::RestrictAxis(Blocks::Ghost::Z, 0);
				position.y -= (hittedOne->GetScale().y / 2.0f + blockType->GetScale().y / 2.0f);
				break;
			}
			default: break;
			}
			Blocks::Ghost::SetScale(blockType->GetScale());

			Blocks::Ghost::SetStartPosition(position);
		}
		std::vector<Blocks::Block>* setBlocks = parentSet->GetBlocks();
		for (size_t i = 0; i < setBlocks->size(); i++)
		{
			if (setBlocks->at(i).GetPosition() == position)
			{
				return;
			}
		}
		Blocks::Ghost::Draw(_gameData);

		if (holding)
		{
			if (!_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_RIGHT) && inputClock.GetElapsedTime() > 125)
			{
				holding = false;
				inputClock.Restart();
				std::vector<Blocks::BlockType*> types = Blocks::GetAll();
				Blocks::BlockType* finalType = nullptr;
				for (size_t typeId = 0; typeId < types.size(); typeId++)
				{
					if (types[typeId]->GetName() == blockType->GetName().c_str())
					{
						finalType = types[typeId];
						break;
					}
				}


				std::vector<glm::mat4> models = Blocks::Ghost::GetModels();
				glm::vec3 setPosition = parentSet->GetPosition();

				for (size_t modelId = 0; modelId < models.size(); modelId++)
				{
					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 translation;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(models[modelId], scale, rotation, translation, skew, perspective);

					Blocks::Block originBlock;
					originBlock.GenerateModel();
					originBlock.SetScale(blockType->GetScale());

					glm::vec3 localPosition = translation - setPosition;

					originBlock.SetPosition(localPosition);

					originBlock.SetType(finalType);

					parentSet->InsertBlock(originBlock, false);
				}
				parentSet->CalculateBoundingBox();
				return;
			}

			Blocks::Ghost::SetRay(ray);
		}
		else if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_RIGHT) && inputClock.GetElapsedTime() > 125)
		{
			holding = true;
			inputClock.Restart();
		}
	}
}

void Sets::SetEditedSet(Set* _set)
{
	parentSet = _set;
}
