#include "Set.h"
#include "Arrows.h"

static bool isSetsMenuOpen = true;
static float originScale = 1.0f;
static const char** blocksName = nullptr;
static int blockCount = 0;
static Sets::Set* selectedSet = nullptr;
static char filename[128] = ""; // Buffer for the filename
static bool showFilenameInput = false; // To toggle the visibility of input field

bool Sets::ChildTree(GameData* _gameData, Set* _parentSet)
{
	std::vector<Set*> childs = _parentSet->GetChilds();
	std::string name = std::string("Childrens" + std::string("##sets-") + std::to_string((int)_parentSet));
	if (ImGui::TreeNode(name.c_str()))
	{
		ImVec2 size(ImGui::GetContentRegionAvail().x, 0);
		if (ImGui::Button(std::string("Create##CreateChildSet" + name).c_str(), ImVec2(size.x * 0.4f, 0)))
		{
			showFilenameInput = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(std::string("Open##OpenChildSet" + name).c_str(), ImVec2(size.x * 0.4f, 0)))
		{
			Files::FileSearch fileSearch = Files::OpenSearchDialog();
			if (fileSearch.achieved && fileSearch.extension == SETS_FILE_EXTENSION)
			{
				Sets::Set* childSet = Sets::Create();
				std::string content = Files::GetFileContent(fileSearch.originalPath.c_str());
				childSet->SetParent(_parentSet, false);
				childSet->SetRenderingInstance(_parentSet->GetRenderingInstance());

				try
				{
					auto parsedContent = json::parse(content);
					childSet->LoadFromJson(parsedContent);
				}
				catch (json::parse_error& e)
				{
					std::cerr << "JSON parsing error: " << e.what() << '\n';
					Sets::Erase(childSet);
				}

				childSet->SetName(fileSearch.name);
				childSet->SetPath(fileSearch.path);
				_parentSet->CalculateBoundingBox();
				_parentSet->CheckVisibility();
				_parentSet->AppplyVisibility();
			}
		}
		if (!childs.size()) { return false; }
		for (size_t childId = 0; childId < childs.size(); childId++)
		{
			bool kill = SetTree(_gameData, childs[childId]);
			if (kill) { return true; }
		}
		ImGui::TreePop();
	}

	if (showFilenameInput)
	{
		ImGui::OpenPopup("Enter child Filename");
	}

	if (ImGui::BeginPopupModal("Enter child Filename", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
		if (ImGui::Button("OK"))
		{
			Sets::Set* newSet = Sets::Create();
			newSet->SetName(filename);

			newSet->SetParent(_parentSet, false);
			newSet->SetRenderingInstance(_parentSet->GetRenderingInstance());
			
			newSet->SetPath(_parentSet->GetPath());

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
	return false;
}

bool Sets::SetTree(GameData* _gameData, Set* _set)
{
	std::string name = std::string(_set->GetName() + std::string("##sets-") + std::to_string((int)_set));
	if (ImGui::TreeNode(name.c_str()))
	{
		bool selected = false;
		if (_set == selectedSet)
		{
			selected = true;
			Sets::SetEditedSet(_set);
		}
		if (ImGui::Checkbox(std::string("Select##Selection" + name).c_str(), &selected))
		{
			selectedSet = _set;
		}
		ImGui::Text(std::string("Block count : " + std::to_string(_set->GetBlocks()->size())).c_str());
		Bounds::Box boundingBox = _set->GetBoundingBox();
		glm::vec3 size = boundingBox.max - boundingBox.min;
		ImGui::Text(std::string("Size : (" + std::to_string(size.x) + "," + std::to_string(size.y) + "," + std::to_string(size.z) + ")").c_str());
		ImGui::Text("Position : ");
		glm::vec3 position = _set->GetPosition();
		if (ImGui::DragFloat3(std::string("##Position" + name).c_str(), &position.x, 0.05f))
		{
			_set->SetPosition(position);
		}
		glm::vec3 scale = _set->GetScale();
		if (ImGui::SliderFloat(std::string("Scale##Scale" + name).c_str(), &scale.x, 0.0f, 1.0f))
		{
			_set->SetScale(scale.x);
		}
		//Rotations
		{
			glm::vec3 rotation = _set->GetRotation();
			if (ImGui::SliderFloat(std::string("X Axis##Rotation" + name).c_str(), &rotation.x, 0.0f, 360.0f, "%.0f"))
			{
				//rotation.x = std::round(rotation.x / 90.0f) * 90.0f;
				_set->SetRotation(rotation);
			}
			if (ImGui::SliderFloat(std::string("Y Axis##Rotation" + name).c_str(), &rotation.y, 0.0f, 360.0f, "%.0f"))
			{
				//rotation.y = std::round(rotation.y / 90.0f) * 90.0f;
				_set->SetRotation(rotation);
			}
			if (ImGui::SliderFloat(std::string("Z Axis##Rotation" + name).c_str(), &rotation.z, 0.0f, 360.0f, "%.0f"))
			{
				//rotation.z = std::round(rotation.z / 90.0f) * 90.0f;
				_set->SetRotation(rotation);
			}
		}
		if (blocksName == nullptr)
		{
			std::vector<Blocks::BlockType*> blocks = Blocks::GetAll();
			blockCount = blocks.size();
			blocksName = new const char* [blockCount + 1];
			blocksName[0] = new char[5];
			blocksName[0] = "None";
			for (size_t i = 0; i < blockCount; ++i)
			{
				std::string materialName = blocks[i]->GetName();
				char* nameCopy = new char[materialName.length() + 1];
				strcpy(nameCopy, materialName.c_str());
				blocksName[i + 1] = nameCopy;
			}
		}
		if (!_set->HasOrigin())
		{
			int marker = 0;
			ImGui::Text("Origin block :");
			if (ImGui::Combo("##Originblocks", &marker, blocksName, blockCount))
			{
				if (marker > 0)
				{
					Blocks::Block block;
					block.GenerateModel();
					Blocks::MaterialCheck(&block, blocksName[marker]);
					Blocks::BlockType* type = block.GetType();
					if (type != nullptr)
					{
						block.SetScale(type->GetScale());
						_set->InsertBlock(block);
					}
					else
					{
						std::cout << "No texture" << std::endl;
						block.EraseModel();
					}
				}
			}
		}

		ImGui::Text("Move Origin : ");
		glm::vec3 originPos(0);
		if (ImGui::DragFloat3(std::string("##MoveOrigin" + name).c_str(), &originPos.x, 0.05f))
		{
			_set->MoveOrigin(originPos);
		}
		if (ImGui::Button(std::string("Save##SaveSet" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			_set->Save();
		}

		if (ImGui::Button(std::string("Close##CloseSet" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			//_set->Erase();	
			//std::vector<Set*>* sets = Sets::GetAll();
			//for (size_t setId = 0; setId < sets->size(); setId++)
			//{
			//	if (sets->at(setId) == _set)
			//	{
			//		sets->erase(sets->begin() + setId);
			//		break;
			//	}
			//}
			Sets::Erase(_set);
			return true;
		}

		bool kill = ChildTree(_gameData, _set);
		if (kill) { return true; }
		ImGui::TreePop();
	}
	return false;
}

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
			if (sets->at(parentSetId)->GetParent() != nullptr) { continue; }//Avoid all sets childs insertion
			bool kill = SetTree(_gameData, sets->at(parentSetId));
			if (kill) { break; }
		}
		ImGui::End();
	}
}