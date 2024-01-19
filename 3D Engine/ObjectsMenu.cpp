#include "ObjectsMenu.h"
#include "Scene.h"
#include "LightMenu.h"

static const char** materialsName = nullptr;
static int materialCount = 0;

static ImVec4 currentColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


void ObjectsMenu::Basic(GameData* _gameData, Cube* _object)
{
	if (ImGui::TreeNode("Block Configuration"))
	{
		if (ImGui::TreeNode("Movement"))
		{
			if (ImGui::Button("-"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x - scale.x, pos.y, pos.z);
			}
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x + scale.x, pos.y, pos.z);
			}

			if (ImGui::Button("-##y"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x, pos.y - scale.y, pos.z);
			}
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			if (ImGui::Button("+##y"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x, pos.y + scale.y, pos.z);
			}

			if (ImGui::Button("-##z"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x, pos.y, pos.z - scale.z);
			}
			ImGui::SameLine();
			ImGui::Text("Z");
			ImGui::SameLine();
			if (ImGui::Button("+##z"))
			{
				glm::vec3 pos = _object->GetPosition();
				glm::vec3 scale = _object->GetScale();
				_object->SetPosition(pos.x, pos.y, pos.z + scale.z);
			}

			ImGui::TreePop();
		}

		LightMenu::Object(_gameData, _object->GetLight());

		bool materialUsage = false;
		if (_object->GetMaterial() != nullptr)
		{
			materialUsage = true;
			int currentMaterialIndex = 0;
			if (materialsName == nullptr)
			{
				materialsName = Scene::GetMaterialsAsStringArray();
			}
			int materialCount = Scene::GetMaterialsCount();
			for (size_t i = 0; i < materialCount; i++)
			{
				if (strcmp(_object->GetMaterial()->GetName().c_str(), materialsName[i]) == 0)
				{
					currentMaterialIndex = i;
				}
			}

			ImGui::Text("Select Material:");
			if (ImGui::Combo("##Material", &currentMaterialIndex, materialsName, materialCount))
			{
				std::string selectedMaterial = materialsName[currentMaterialIndex];
				std::vector<Material>* materials = Scene::GetMaterials();
				for (size_t i = 0; i < materials->size(); i++)
				{
					if (strcmp(selectedMaterial.c_str(), materials->at(i).GetName().c_str()) == 0)
					{
						_object->SetMaterial(&materials->at(i), true);
					}
				}
			}
		}
		if (ImGui::Checkbox("Use Material", &materialUsage))
		{
			if (materialUsage)
			{
				std::vector<Material>* materials = Scene::GetMaterials();
				_object->SetMaterial(&materials->at(rand() % materials->size()), true);
			}
			else
			{
				_object->RemoveMaterial(true);
			}
		}


		if (ImGui::TreeNode("Colors"))
		{
			Colors::Color actualColor = _object->GetColor();
			currentColor = { actualColor.values[0], actualColor.values[1], actualColor.values[2], actualColor.values[3] };
			ImGui::Text("Alpha :");
			if (ImGui::SliderFloat("##COLOR_ALPHA", &currentColor.w, 0.0f, 1.0f))
			{
				_object->SetColor(Colors::Color(currentColor.x, currentColor.y, currentColor.z, currentColor.w));
			}

			for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
			{
				ImGui::PushID(i);
				if (ImGui::ColorButton("##DefaultColors", Colors::DefaultColors[i]))
				{
					currentColor = Colors::DefaultColors[i];
					_object->SetColor(Colors::Color(currentColor.x, currentColor.y, currentColor.z, currentColor.w));
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
			if (ImGui::ColorPicker4("Custom Color", (float*)&currentColor))
			{
				_object->SetColor(Colors::Color(currentColor.x, currentColor.y, currentColor.z, currentColor.w));
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lightning options##CubeLight"))
		{
			bool _objectLightDependecy = _object->GetLightDependency();
			if (ImGui::Checkbox("Light Dependant", &_objectLightDependecy))
			{
				_object->SetLightDependency(_objectLightDependecy);
			}

			Lightning::Light* cubeLight = _object->GetLight();
			bool isCubeALight = (cubeLight == nullptr) ? false : true;
			int currentLightType = 0;
			if (cubeLight != nullptr)
			{
				currentLightType = cubeLight->GetType() + 1;
			}
			ImGui::Text("Select Light Type:");
			if (ImGui::Combo("##LightType", &currentLightType, Lightning::LightTypes, IM_ARRAYSIZE(Lightning::LightTypes)))
			{
				if (currentLightType)
				{
					Lightning::Light light;
					light.SetName((std::string("Cube light : ") + Lightning::LightTypes[currentLightType]));
					light.SetType((Lightning::LightType)(currentLightType - 1));
					light.SetPosition(_object->GetPosition());

					cubeLight = Scene::Lights::InsertLight(_gameData, light);
					_object->SetLight(cubeLight);
					LightMenu::CleanUp();
				}
				else if (cubeLight != nullptr)
				{
					Scene::Lights::Remove(cubeLight);
					_object->SetLight(nullptr);
					cubeLight = nullptr;
				}
			}

			LightMenu::Object(_gameData, cubeLight);

			ImGui::TreePop();
		}
	}
}

void ObjectsMenu::CleanUp()
{
	if (materialsName != nullptr)
	{
		for (size_t i = 0; i < materialCount; ++i)
		{
			delete[] materialsName[i];
		}
		delete[] materialsName;
	}
	materialCount = 0;
}
