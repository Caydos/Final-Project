#include "LightMenu.h"
#include "Scene.h"

static const char** lightsName = nullptr;
static int lightCount = 0;
static int currentLightIndex = 0;

void LightMenu::Basic(GameData* _gameData)
{
	bool isSceneLightningEnabled = Scene::World::IsConsideringLightning();
	if (ImGui::TreeNode("Lightning"))
	{
		if (ImGui::Checkbox("Consider lightning", &isSceneLightningEnabled))
		{
			Scene::World::ConsiderLightning(isSceneLightningEnabled);
		}

		std::vector<Lightning::Light>* lights = Scene::Lights::GetLights();
		if (lightsName == nullptr)
		{
			lightCount = lights->size();
			lightsName = new const char* [lightCount];
			for (size_t i = 0; i < lightCount; ++i) {
				std::string materialName = (*lights)[i].GetName();
				char* nameCopy = new char[materialName.length() + 1];
				strcpy(nameCopy, materialName.c_str());
				lightsName[i] = nameCopy;
			}
		}

		ImGui::Text("Select Light:");
		ImGui::Combo("##Lights", &currentLightIndex, lightsName, lightCount);

		Lightning::Light* selectedLight = &lights->at(currentLightIndex);
		LightMenu::Object(_gameData, selectedLight);

		ImGui::TreePop();
	}
}

void LightMenu::Object(GameData* _gameData, Lightning::Light* _light)
{
	if (_light != nullptr)
	{
		if (ImGui::TreeNode((_light->GetName() + " Parameters").c_str()))
		{
			bool isLightActivated = _light->IsActive();
			if (ImGui::Checkbox("Light Emission##_light", &isLightActivated))
			{
				_light->SetActive(isLightActivated);
				Scene::Lights::UpdateShader(_gameData);
			}
			if (_light->GetType() == Lightning::DIRECTIONAL)
			{
				ImGui::Text("Direction :");
				glm::vec3 direction = _light->GetDirection();
				if (ImGui::SliderFloat("X##_lightDirection", &direction.x, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("Y##_lightDirection", &direction.y, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("Z##_lightDirection", &direction.z, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::Button("Reset Direction##_light"))
				{
					direction = glm::vec3(.0f);
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
			}
			else if (_light->GetType() == Lightning::POINT)
			{
				ImGui::Text("Direction :");
				glm::vec3 direction = _light->GetDirection();
				float constant = _light->GetConstant();
				if (ImGui::SliderFloat("Constant##_lightConstant", &constant, 0.0f, 1.0f))
				{
					_light->SetConstant(constant);
					Scene::Lights::UpdateShader(_gameData);
				}
				float linear = _light->GetLinear();
				if (ImGui::SliderFloat("Linear##_lightLinear", &linear, 0.0f, 1.0f))
				{
					_light->SetLinear(linear);
					Scene::Lights::UpdateShader(_gameData);
				}
				float quadratic = _light->GetQuadratic();
				if (ImGui::SliderFloat("Quadratic##_lightQuadratic", &quadratic, 0.0f, 1.0f))
				{
					_light->SetQuadratic(quadratic);
					Scene::Lights::UpdateShader(_gameData);
				}
			}
			else if (_light->GetType() == Lightning::SPOT)
			{
				ImGui::Text("Direction :");
				glm::vec3 direction = _light->GetDirection();
				if (ImGui::SliderFloat("X##_lightDirection", &direction.x, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("Y##_lightDirection", &direction.y, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("Z##_lightDirection", &direction.z, -1.0f, 1.0f))
				{
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::Button("Reset Direction##_lightReset"))
				{
					direction = glm::vec3(.0f);
					_light->SetDirection(direction);
					Scene::Lights::UpdateShader(_gameData);
				}
				float constant = _light->GetConstant();
				if (ImGui::SliderFloat("Constant##_lightConstant", &constant, 0.0f, 1.0f))
				{
					_light->SetConstant(constant);
					Scene::Lights::UpdateShader(_gameData);
				}
				float linear = _light->GetLinear();
				if (ImGui::SliderFloat("Linear##_lightLinear", &linear, 0.0f, 1.0f))
				{
					_light->SetLinear(linear);
					Scene::Lights::UpdateShader(_gameData);
				}
				float quadratic = _light->GetQuadratic();
				if (ImGui::SliderFloat("Quadratic##_lightQuadratic", &quadratic, 0.0f, 1.0f))
				{
					_light->SetQuadratic(quadratic);
					Scene::Lights::UpdateShader(_gameData);
				}

				float cutOff = _light->GetCutOff();
				float outerCutOff = _light->GetOuterCutOff();
				if (ImGui::SliderFloat("Inner Radius##_lightCutoff", &cutOff, 0.0f, outerCutOff))
				{
					_light->SetCutOff(cutOff);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("Outer Raduis##_lightCutoff", &outerCutOff, 0.0f, 90.0f))
				{
					_light->SetOuterCutOff(outerCutOff);
					if (cutOff > outerCutOff)
					{
						_light->SetCutOff(outerCutOff * 0.90);
					}
					Scene::Lights::UpdateShader(_gameData);
				}
			}

			if (ImGui::TreeNode("Ambiant Alpha (Intensity)##_light"))
			{
				glm::vec3 ambient = _light->GetAmbient();

				if (ImGui::SliderFloat("R##ambient##_light", &ambient.x, 0.0f, 1.0f))
				{
					_light->SetAmbient(ambient);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("G##ambient##_light", &ambient.y, 0.0f, 1.0f))
				{
					_light->SetAmbient(ambient);
					Scene::Lights::UpdateShader(_gameData);
				}
				if (ImGui::SliderFloat("B##ambient##_light", &ambient.z, 0.0f, 1.0f))
				{
					_light->SetAmbient(ambient);
					Scene::Lights::UpdateShader(_gameData);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Diffuse color##_light"))
			{
				glm::vec3 oDiffuse = _light->GetDiffuse();
				ImVec4 diffuse(oDiffuse.x, oDiffuse.y, oDiffuse.z, 1.0f);
				for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
				{
					ImGui::PushID(i);
					if (ImGui::ColorButton("##DefaultLightColors##_light", Colors::DefaultColors[i]))
					{
						diffuse = Colors::DefaultColors[i];
						_light->SetDiffuse(glm::vec3(diffuse.x, diffuse.y, diffuse.z));
						Scene::Lights::UpdateShader(_gameData);
					}
					ImGui::PopID();
					ImGui::SameLine();
				}
				ImGui::NewLine();
				if (ImGui::ColorPicker4("Custom Color##_light", (float*)&diffuse))
				{
					_light->SetDiffuse(glm::vec3(diffuse.x, diffuse.y, diffuse.z));
					Scene::Lights::UpdateShader(_gameData);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Specular reflection##_light"))
			{
				glm::vec3 oSpecular = _light->GetSpecular();
				ImVec4 specular(oSpecular.x, oSpecular.y, oSpecular.z, 1.0f);
				for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
				{
					ImGui::PushID(i);
					if (ImGui::ColorButton("##DefaultLightColors##2##_light", Colors::DefaultColors[i]))
					{
						specular = Colors::DefaultColors[i];
						_light->SetSpecular(glm::vec3(specular.x, specular.y, specular.z));
						Scene::Lights::UpdateShader(_gameData);
					}
					ImGui::PopID();
					ImGui::SameLine();
				}
				ImGui::NewLine();
				if (ImGui::ColorPicker4("Custom Color##2##_light", (float*)&specular))
				{
					_light->SetSpecular(glm::vec3(specular.x, specular.y, specular.z));
					Scene::Lights::UpdateShader(_gameData);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}


void LightMenu::CleanUp()
{
	if (lightsName != nullptr)
	{
		for (size_t i = 0; i < lightCount; ++i)
		{
			delete[] lightsName[i];
			lightsName[i] = nullptr;
		}
		delete[] lightsName;
		lightsName = nullptr;
	}
	lightCount = 0;
}