#include "Set.h"
#include "Arrows.h"

static bool isSetsMenuOpen = true;
static float originScale = 1.0f;

void Sets::Menu(GameData* _gameData)
{
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

				if (ImGui::Button(std::string("Place Origin Block##OriginBlock" + parentSetId).c_str()))
				{
					sets->at(parentSetId)->PlaceOriginBlock(originScale);
				}

				
				if (ImGui::SliderFloat(std::string("Origin Block Scale##OriginBlock" + parentSetId).c_str(), &originScale, 0.01f, 1.0f))
				{
					sets->at(parentSetId)->SetOriginBlockScale(originScale);
				}

				ImGui::Text("Move Origin : ");
				glm::vec3 originPos(0);
				if (ImGui::DragFloat3(std::string("##MoveOrigin" + parentSetId).c_str(), &originPos.x, 0.05f))
				{
					sets->at(parentSetId)->MoveOrigin(originPos);
				}

				if (ImGui::Button(std::string("Save##SaveSet" + parentSetId).c_str()))
				{
					sets->at(parentSetId)->Save();
				}

				if (ImGui::Button(std::string("Close##CloseSet" + parentSetId).c_str()))
				{
					sets->at(parentSetId)->Erase();
					sets->erase(sets->begin() + parentSetId);
					break;
				}

				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}