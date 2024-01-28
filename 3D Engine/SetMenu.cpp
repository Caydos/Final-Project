#include "Set.h"
#include "Arrows.h"
static Arrow arrows[3];
static bool initialized = false;
static bool isSetsMenuOpen = true;


void Sets::Menu(GameData* _gameData)
{
	if (!initialized)
	{
		arrows[0].GenerateGraphicsBuffers();
		arrows[0].BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
		arrows[0].SetColor(Colors::Red);

		arrows[1].GenerateGraphicsBuffers();
		arrows[1].BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
		arrows[1].SetColor(Colors::Green);

		arrows[2].GenerateGraphicsBuffers();
		arrows[2].BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
		arrows[2].SetColor(Colors::Blue);
		initialized = true;
	}
	arrows[0].Draw();
	//arrows[1].Draw();
	//arrows[2].Draw();

	glm::ivec2 windowDimensions = _gameData->window.GetDimensions();
	ImGui::SetNextWindowPos(ImVec2(windowDimensions.x - windowDimensions.x * 0.2, 0));
	ImGui::SetNextWindowSize(ImVec2(windowDimensions.x * 0.2, windowDimensions.y));
	if (ImGui::Begin("Projects", &isSetsMenuOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		std::vector<Set*>* sets = Sets::GetAll();
		if (sets->size() < 1)
		{
			ImGui::Text("No sets available.");
		}
		for (size_t parentSetId = 0; parentSetId < sets->size(); parentSetId++)
		{
			std::string name = std::string(sets->at(parentSetId)->GetName() + std::string("##sets-") + std::to_string(parentSetId));
			if (ImGui::TreeNode(name.c_str()))
			{
				ImGui::Text("Position : ");
				glm::vec3 position = sets->at(parentSetId)->GetPosition();
				if (ImGui::DragFloat3(std::string("##Position" + name).c_str(), &position.x, 0.05f))
				{
					sets->at(parentSetId)->SetPosition(position);
				}

				//Rotations
				{
					glm::vec3 rotation = sets->at(parentSetId)->GetRotation();
					if (ImGui::SliderFloat("X Axis", &rotation.x, 0.0f, 360.0f, "%.0f"))
					{
						rotation.x = std::round(rotation.x / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
					if (ImGui::SliderFloat("Y Axis", &rotation.y, 0.0f, 360.0f, "%.0f"))
					{
						rotation.y = std::round(rotation.y / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
					if (ImGui::SliderFloat("Z Axis", &rotation.z, 0.0f, 360.0f, "%.0f"))
					{
						rotation.z = std::round(rotation.z / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
				}

				if (ImGui::Button(std::string("Place Origin Block##OriginBlock" + name).c_str()))
				{
					sets->at(parentSetId)->PlaceOriginBlock();
				}

				ImGui::Text("Move Origin : ");
				glm::vec3 originPos(0);
				if (ImGui::DragFloat3(std::string("##MoveOrigin" + name).c_str(), &originPos.x, 0.05f))
				{
					sets->at(parentSetId)->MoveOrigin(originPos);
				}

				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}