#include "LightMenu.h"
#include "Scene.h"

static const char** lightsName = nullptr;
static int lightCount = 0;
static int currentLightIndex = 0;

void LightMenu::Basic(GameData* _gameData)
{
	if (ImGui::TreeNode("Lighting"))
	{
		std::vector<Lighting::Spot*>* spots = Scene::Lights::GetSpots();
		for (size_t spotId = 0; spotId < spots->size(); spotId++)
		{
			std::string spotSubName("##spot" + std::to_string((int)spots->at(spotId)));
			if (ImGui::TreeNode(("Light n°" + std::to_string(spotId) + spotSubName).c_str()))
			{
				Lighting::Spot* actualSpot = spots->at(spotId);
				bool needUpdate = false;
				bool activated = (actualSpot->activation > 0.0) ? true : false;
				if (ImGui::Checkbox(("Light Emission" + spotSubName).c_str(), &activated))
				{
					needUpdate = true;
					actualSpot->activation = (activated) ? 1.0f : 0.0f;
				}

				if (ImGui::SliderFloat(("Constant" + spotSubName).c_str(), &actualSpot->constant, 0.0f, 1.0f))
				{
					needUpdate = true;
				}
				if (ImGui::SliderFloat(("Linear" + spotSubName).c_str(), &actualSpot->linear, 0.0f, 1.0f))
				{
					needUpdate = true;
				}
				if (ImGui::SliderFloat(("Quadratic" + spotSubName).c_str(), &actualSpot->quadratic, 0.0f, 1.0f))
				{
					needUpdate = true;
				}

				float cutOff = glm::degrees(glm::acos(actualSpot->cutOff));
				float outerCutOff = glm::degrees(glm::acos(actualSpot->outerCutOff));
				if (ImGui::SliderFloat(("Inner Radius" + spotSubName).c_str(), &cutOff, 0.0f, outerCutOff))
				{
					needUpdate = true;
					actualSpot->cutOff = glm::cos(glm::radians(cutOff));
				}
				if (ImGui::SliderFloat(("Outer Raduis" + spotSubName).c_str(), &outerCutOff, 0.0f, 90.0f))
				{
					needUpdate = true;
					if (cutOff > outerCutOff)
					{
						actualSpot->cutOff = glm::cos(glm::radians(outerCutOff * 0.90));
					}
					actualSpot->outerCutOff = glm::cos(glm::radians(outerCutOff));
				}

				if (ImGui::TreeNode(("Ambiant Alpha (Intensity)" + spotSubName).c_str()))
				{
					if (ImGui::SliderFloat(("R" + spotSubName).c_str(), &actualSpot->ambient.x, 0.0f, 1.0f))
					{
						needUpdate = true;
					}
					if (ImGui::SliderFloat(("G" + spotSubName).c_str(), &actualSpot->ambient.y, 0.0f, 1.0f))
					{
						needUpdate = true;
					}
					if (ImGui::SliderFloat(("B" + spotSubName).c_str(), &actualSpot->ambient.z, 0.0f, 1.0f))
					{
						needUpdate = true;
					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode(("Diffuse color" + spotSubName).c_str()))
				{
					glm::vec3 oDiffuse = actualSpot->diffuse;
					ImVec4 diffuse(oDiffuse.x, oDiffuse.y, oDiffuse.z, 1.0f);
					for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
					{
						ImGui::PushID(i);
						if (ImGui::ColorButton(("##DefaultLightColors" + spotSubName).c_str(), Colors::DefaultColors[i]))
						{
							diffuse = Colors::DefaultColors[i];
							actualSpot->diffuse = glm::vec3(diffuse.x, diffuse.y, diffuse.z);
							needUpdate = true;
						}
						ImGui::PopID();
						ImGui::SameLine();
					}
					ImGui::NewLine();
					if (ImGui::ColorPicker4(("Custom Color" + spotSubName).c_str(), (float*)&diffuse))
					{
						actualSpot->diffuse =  glm::vec3(diffuse.x, diffuse.y, diffuse.z);
						needUpdate = true;
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode(("Specular reflection" + spotSubName).c_str()))
				{
					glm::vec3 oSpecular = actualSpot->specular;
					ImVec4 specular(oSpecular.x, oSpecular.y, oSpecular.z, 1.0f);
					for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
					{
						ImGui::PushID(i);
						if (ImGui::ColorButton(("##DefaultLightColors##2" + spotSubName).c_str(), Colors::DefaultColors[i]))
						{
							specular = Colors::DefaultColors[i];
							actualSpot->specular = glm::vec3(specular.x, specular.y, specular.z);
							needUpdate = true;
						}
						ImGui::PopID();
						ImGui::SameLine();
					}
					ImGui::NewLine();
					if (ImGui::ColorPicker4(("Custom Color##2" + spotSubName).c_str(), (float*)&specular))
					{
						actualSpot->specular = glm::vec3(specular.x, specular.y, specular.z);
						needUpdate = true;
					}
					ImGui::TreePop();
				}

				if (needUpdate)
				{
					Lighting::UpdateSpot(actualSpot);
					Scene::Lights::UpdateSpot(actualSpot);
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	//bool isSceneLightningEnabled = Scene::World::IsConsideringLightning();
	//if (ImGui::TreeNode("Lighting"))
	//{
	//	if (ImGui::Checkbox("Consider lightning", &isSceneLightningEnabled))
	//	{
	//		Scene::World::ConsiderLightning(isSceneLightningEnabled, _gameData);
	//	}

	//	std::vector<Lighting::Light>* lights = Scene::Lights::GetLights();
	//	if (lightsName == nullptr)
	//	{
	//		lightCount = lights->size();
	//		lightsName = new const char* [lightCount];
	//		for (size_t i = 0; i < lightCount; ++i) {
	//			std::string materialName = (*lights)[i].GetName();
	//			char* nameCopy = new char[materialName.length() + 1];
	//			strcpy(nameCopy, materialName.c_str());
	//			lightsName[i] = nameCopy;
	//		}
	//	}

	//	ImGui::Text("Select Light:");
	//	ImGui::Combo("##Lights", &currentLightIndex, lightsName, lightCount);

	//	Lighting::Light* selectedLight = &lights->at(currentLightIndex);
	//	LightMenu::Object(_gameData, selectedLight);

	//	ImGui::TreePop();
	//}
}
//
//void LightMenu::Object(GameData* _gameData, Lighting::Light* _light)
//{
//	//if (_light != nullptr)
//	//{
//	//	if (ImGui::TreeNode((_light->GetName() + " Parameters").c_str()))
//	//	{
//	//		bool isLightActivated = _light->IsActive();
//	//		if (ImGui::Checkbox("Light Emission##_light", &isLightActivated))
//	//		{
//	//			_light->SetActive(isLightActivated);
//	//			Scene::Lights::UpdateShader(_gameData);
//	//		}
//	//		if (_light->GetType() == Lighting::DIRECTIONAL)
//	//		{
//	//			ImGui::Text("Direction :");
//	//			glm::vec3 direction = _light->GetDirection();
//	//			if (ImGui::SliderFloat("X##_lightDirection", &direction.x, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("Y##_lightDirection", &direction.y, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("Z##_lightDirection", &direction.z, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::Button("Reset Direction##_light"))
//	//			{
//	//				direction = glm::vec3(.0f);
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//		}
//	//		else if (_light->GetType() == Lighting::POINT)
//	//		{
//	//			ImGui::Text("Direction :");
//	//			glm::vec3 direction = _light->GetDirection();
//	//			float constant = _light->GetConstant();
//	//			if (ImGui::SliderFloat("Constant##_lightConstant", &constant, 0.0f, 1.0f))
//	//			{
//	//				_light->SetConstant(constant);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			float linear = _light->GetLinear();
//	//			if (ImGui::SliderFloat("Linear##_lightLinear", &linear, 0.0f, 1.0f))
//	//			{
//	//				_light->SetLinear(linear);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			float quadratic = _light->GetQuadratic();
//	//			if (ImGui::SliderFloat("Quadratic##_lightQuadratic", &quadratic, 0.0f, 1.0f))
//	//			{
//	//				_light->SetQuadratic(quadratic);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//		}
//	//		else if (_light->GetType() == Lighting::SPOT)
//	//		{
//	//			ImGui::Text("Direction :");
//	//			glm::vec3 direction = _light->GetDirection();
//	//			if (ImGui::SliderFloat("X##_lightDirection", &direction.x, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("Y##_lightDirection", &direction.y, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("Z##_lightDirection", &direction.z, -1.0f, 1.0f))
//	//			{
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::Button("Reset Direction##_lightReset"))
//	//			{
//	//				direction = glm::vec3(.0f);
//	//				_light->SetDirection(direction);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			float constant = _light->GetConstant();
//	//			if (ImGui::SliderFloat("Constant##_lightConstant", &constant, 0.0f, 1.0f))
//	//			{
//	//				_light->SetConstant(constant);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			float linear = _light->GetLinear();
//	//			if (ImGui::SliderFloat("Linear##_lightLinear", &linear, 0.0f, 1.0f))
//	//			{
//	//				_light->SetLinear(linear);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			float quadratic = _light->GetQuadratic();
//	//			if (ImGui::SliderFloat("Quadratic##_lightQuadratic", &quadratic, 0.0f, 1.0f))
//	//			{
//	//				_light->SetQuadratic(quadratic);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//
//	//			float cutOff = _light->GetCutOff();
//	//			float outerCutOff = _light->GetOuterCutOff();
//	//			if (ImGui::SliderFloat("Inner Radius##_lightCutoff", &cutOff, 0.0f, outerCutOff))
//	//			{
//	//				_light->SetCutOff(cutOff);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("Outer Raduis##_lightCutoff", &outerCutOff, 0.0f, 90.0f))
//	//			{
//	//				_light->SetOuterCutOff(outerCutOff);
//	//				if (cutOff > outerCutOff)
//	//				{
//	//					_light->SetCutOff(outerCutOff * 0.90);
//	//				}
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//		}
//
//	//		if (ImGui::TreeNode("Ambiant Alpha (Intensity)##_light"))
//	//		{
//	//			glm::vec3 ambient = _light->GetAmbient();
//
//	//			if (ImGui::SliderFloat("R##ambient##_light", &ambient.x, 0.0f, 1.0f))
//	//			{
//	//				_light->SetAmbient(ambient);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("G##ambient##_light", &ambient.y, 0.0f, 1.0f))
//	//			{
//	//				_light->SetAmbient(ambient);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			if (ImGui::SliderFloat("B##ambient##_light", &ambient.z, 0.0f, 1.0f))
//	//			{
//	//				_light->SetAmbient(ambient);
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			ImGui::TreePop();
//	//		}
//
//	//		if (ImGui::TreeNode("Diffuse color##_light"))
//	//		{
//	//			glm::vec3 oDiffuse = _light->GetDiffuse();
//	//			ImVec4 diffuse(oDiffuse.x, oDiffuse.y, oDiffuse.z, 1.0f);
//	//			for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
//	//			{
//	//				ImGui::PushID(i);
//	//				if (ImGui::ColorButton("##DefaultLightColors##_light", Colors::DefaultColors[i]))
//	//				{
//	//					diffuse = Colors::DefaultColors[i];
//	//					_light->SetDiffuse(glm::vec3(diffuse.x, diffuse.y, diffuse.z));
//	//					Scene::Lights::UpdateShader(_gameData);
//	//				}
//	//				ImGui::PopID();
//	//				ImGui::SameLine();
//	//			}
//	//			ImGui::NewLine();
//	//			if (ImGui::ColorPicker4("Custom Color##_light", (float*)&diffuse))
//	//			{
//	//				_light->SetDiffuse(glm::vec3(diffuse.x, diffuse.y, diffuse.z));
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			ImGui::TreePop();
//	//		}
//	//		if (ImGui::TreeNode("Specular reflection##_light"))
//	//		{
//	//			glm::vec3 oSpecular = _light->GetSpecular();
//	//			ImVec4 specular(oSpecular.x, oSpecular.y, oSpecular.z, 1.0f);
//	//			for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
//	//			{
//	//				ImGui::PushID(i);
//	//				if (ImGui::ColorButton("##DefaultLightColors##2##_light", Colors::DefaultColors[i]))
//	//				{
//	//					specular = Colors::DefaultColors[i];
//	//					_light->SetSpecular(glm::vec3(specular.x, specular.y, specular.z));
//	//					Scene::Lights::UpdateShader(_gameData);
//	//				}
//	//				ImGui::PopID();
//	//				ImGui::SameLine();
//	//			}
//	//			ImGui::NewLine();
//	//			if (ImGui::ColorPicker4("Custom Color##2##_light", (float*)&specular))
//	//			{
//	//				_light->SetSpecular(glm::vec3(specular.x, specular.y, specular.z));
//	//				Scene::Lights::UpdateShader(_gameData);
//	//			}
//	//			ImGui::TreePop();
//	//		}
//	//		ImGui::TreePop();
//	//	}
//	//}
//}


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