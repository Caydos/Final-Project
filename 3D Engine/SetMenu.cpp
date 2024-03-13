#include "Set.h"
#include "Arrows.h"

static bool isSetsMenuOpen = true;
static float originScale = 1.0f;
static const char** blocksName = nullptr;
static int blockCount = 0;
static Sets::Set* selectedSet = nullptr;


void Sets::Menu(GameData* _gameData, float _yOffset)
{
	glm::ivec2 windowDimensions = _gameData->window.GetDimensions();
	ImGui::SetNextWindowPos(ImVec2(windowDimensions.x - windowDimensions.x * 0.2, _yOffset));
	ImGui::SetNextWindowSize(ImVec2(windowDimensions.x * 0.2, windowDimensions.y - _yOffset));
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
				bool selected = false;
				if (sets->at(parentSetId) == selectedSet)
				{
					selected = true;
					Sets::SetEditedSet(sets->at(parentSetId));
				}
				if (ImGui::Checkbox(std::string("Select##Selection" + name).c_str(), &selected))
				{
					selectedSet = sets->at(parentSetId);
				}
				ImGui::Text(std::string("Block count : " + std::to_string(sets->at(parentSetId)->GetBlocks()->size())).c_str());
				ImGui::Text("Position : ");
				glm::vec3 position = sets->at(parentSetId)->GetPosition();
				if (ImGui::DragFloat3(std::string("##Position" + name).c_str(), &position.x, 0.05f))
				{
					sets->at(parentSetId)->SetPosition(position);
				}
				glm::vec3 scale = sets->at(parentSetId)->GetScale();
				if (ImGui::SliderFloat(std::string("Scale##Scale" + name).c_str(), &scale.x, 0.0f, 1.0f))
				{
					sets->at(parentSetId)->SetScale(scale.x);
				}
				//Rotations
				{
					glm::vec3 rotation = sets->at(parentSetId)->GetRotation();
					if (ImGui::SliderFloat(std::string("X Axis##Rotation" + name).c_str(), &rotation.x, 0.0f, 360.0f, "%.0f"))
					{
						rotation.x = std::round(rotation.x / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
					if (ImGui::SliderFloat(std::string("Y Axis##Rotation" + name).c_str(), &rotation.y, 0.0f, 360.0f, "%.0f"))
					{
						rotation.y = std::round(rotation.y / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
					if (ImGui::SliderFloat(std::string("Z Axis##Rotation" + name).c_str(), &rotation.z, 0.0f, 360.0f, "%.0f"))
					{
						rotation.z = std::round(rotation.z / 90.0f) * 90.0f;
						sets->at(parentSetId)->SetRotation(rotation);
					}
				}
				if (blocksName == nullptr)
				{
					std::vector<Blocks::BlockType*> blocks = Blocks::GetAll();
					blockCount = blocks.size();
					blocksName = new const char* [blockCount];
					for (size_t i = 0; i < blockCount; ++i)
					{
						std::string materialName = blocks[i]->GetName();
						char* nameCopy = new char[materialName.length() + 1];
						strcpy(nameCopy, materialName.c_str());
						blocksName[i] = nameCopy;
					}
				}
				if (!sets->at(parentSetId)->HasOrigin())
				{
					int marker = 0;
					ImGui::Text("Origin block :");
					if (ImGui::Combo("##Originblocks", &marker, blocksName, blockCount))
					{
						Blocks::Block block;
						block.GenerateModel();
						Blocks::MaterialCheck(&block, blocksName[marker]);
						Blocks::BlockType* type = block.GetType();
						if (type != nullptr)
						{
							block.SetScale(type->GetScale());
							sets->at(parentSetId)->InsertBlock(block);
						}
						else
						{
							std::cout << "No texture" << std::endl;
							block.EraseModel();
						}
					}
				}

				ImGui::Text("Move Origin : ");
				glm::vec3 originPos(0);
				if (ImGui::DragFloat3(std::string("##MoveOrigin" + name).c_str(), &originPos.x, 0.05f))
				{
					sets->at(parentSetId)->MoveOrigin(originPos);
				}
				if (ImGui::Button(std::string("Save##SaveSet" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x,0)))
				{
					sets->at(parentSetId)->Save();
				}

				if (ImGui::Button(std::string("Close##CloseSet" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x,0)))
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