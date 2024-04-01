#include "Blocks.h"
#include "Files.h"
#include "Set.h"

static std::vector<Blocks::BlockType*> blocks;
static std::vector<Blocks::Instance*> generationQueue;
static std::vector<Blocks::Instance*> removalQueue;

void Blocks::QueueInstanceGeneration(Instance* _instance)
{
	generationQueue.push_back(_instance);
}

void Blocks::QueueInstanceRemoval(Instance* _instance)
{
	removalQueue.push_back(_instance);
}

void Blocks::UpdateInstanceQueue()
{
	//for (size_t queueId = 0; queueId < generationQueue.size(); queueId++)
	//{
	//	generationQueue[queueId].
	//}
	//for (size_t blockId = 0; blockId < blocks.size(); blockId++)
	//{
	//	blocks[blockId]->RemoveInstance(_instance);
	//}
}

void Blocks::Initialize()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < folders.size(); folderId++)
	{
		Load(folders[folderId]);
	}
}

void Blocks::Load(std::string _name)
{
	Blocks::BlockType* block = new Blocks::BlockType;
	std::vector<std::string> files = Files::GetAllAtPath((std::string(BLOCKS_DIRECTORY) + _name).c_str());
	for (size_t fileId = 0; fileId < files.size(); fileId++)
	{
		size_t lastindex = files[fileId].find_last_of(".");
		std::string extension = files[fileId].substr(lastindex);
		std::string filename_without_extension = files[fileId].substr(0, lastindex);

		if (extension == BLOCKS_FILE_EXTENSION)
		{
			std::string rawName = files[fileId].substr(0, lastindex);
			std::string fileContent = Files::GetFileContent((std::string(BLOCKS_DIRECTORY) + _name + "/" + files[fileId]).c_str());
			json content;
			try
			{
				content = json::parse(fileContent);
				if (content.contains("attributes"))
				{
					if (content["attributes"].contains("scale"))
					{
						glm::vec3 scale;
						if (content["attributes"]["scale"].is_array())
						{
							scale = glm::vec3(content["attributes"]["scale"][0], content["attributes"]["scale"][1], content["attributes"]["scale"][2]);
						}
						else
						{
							scale = glm::vec3(content["attributes"]["scale"]);
						}
						block->SetScale(scale);
					}
					if (content["attributes"].contains("lightDependent"))
					{
						block->SetLightDependency(content["attributes"]["lightDependent"]);
					}
					if (content["attributes"].contains("diffuse"))
					{
						block->SetDiffuse(glm::vec3(content["attributes"]["diffuse"][0],
							content["attributes"]["diffuse"][1],
							content["attributes"]["diffuse"][2]));
					}
					if (content["attributes"].contains("specular"))
					{
						block->SetSpecular(glm::vec3(content["attributes"]["specular"][0],
							content["attributes"]["specular"][1],
							content["attributes"]["specular"][2]));
					}
					if (content["attributes"].contains("shininess"))
					{
						block->SetShininess(content["attributes"]["shininess"]);
					}


					if (content["attributes"].contains("light"))
					{
						std::cout << "Loading block Light" << std::endl;
						Lighting::Light light;
						if (content["attributes"]["light"].contains("position"))
						{
							light.SetPosition(glm::vec3(content["attributes"]["light"]["position"][0],
								content["attributes"]["light"]["position"][1],
								content["attributes"]["light"]["position"][2]));
						}
						if (content["attributes"]["light"].contains("direction"))
						{
							light.SetDirection(glm::vec3(content["attributes"]["light"]["direction"][0],
														content["attributes"]["light"]["direction"][1], 
														content["attributes"]["light"]["direction"][2]));
						}
						if (content["attributes"]["light"].contains("ambient"))
						{
							light.SetAmbient(glm::vec3(content["attributes"]["light"]["ambient"][0],
								content["attributes"]["light"]["ambient"][1],
								content["attributes"]["light"]["ambient"][2]));
						}
						if (content["attributes"]["light"].contains("diffuse"))
						{
							light.SetDiffuse(glm::vec3(content["attributes"]["light"]["diffuse"][0],
								content["attributes"]["light"]["diffuse"][1],
								content["attributes"]["light"]["diffuse"][2]));
						}
						if (content["attributes"]["light"].contains("specular"))
						{
							light.SetSpecular(glm::vec3(content["attributes"]["light"]["specular"][0],
								content["attributes"]["light"]["specular"][1],
								content["attributes"]["light"]["specular"][2]));
						}
						if (content["attributes"]["light"].contains("constant"))
						{
							light.SetConstant(content["attributes"]["light"]["constant"]);
						}
						if (content["attributes"]["light"].contains("linear"))
						{
							light.SetLinear(content["attributes"]["light"]["linear"]);
						}
						if (content["attributes"]["light"].contains("quadratic"))
						{
							light.SetQuadratic(content["attributes"]["light"]["quadratic"]);
						}
						if (content["attributes"]["light"].contains("cutOff"))
						{
							light.SetCutOff(content["attributes"]["light"]["cutOff"]);
						}
						if (content["attributes"]["light"].contains("outerCutOff"))
						{
							light.SetOuterCutOff(content["attributes"]["light"]["outerCutOff"]);
						}
						light.SetActive(true);
						light.SetType(Lighting::SPOT);
						light.SetName(block->GetName() + " spot light");
						block->SetLightEmission(true);
						block->SetLight(light);
					}
				}
			}
			catch (nlohmann::json::parse_error& e)
			{
				std::cerr << "JSON parsing error: " << e.what() << '\n';
				Logger::Write("Failed to parse Json file : ", rawName, "\n");
				continue;
			}
		}
		else if (extension == BLOCKS_TEXTURE_EXTENSION || extension == ".png")
		{
			Texture* texture = new Texture;
			texture->LoadFromFile((std::string(BLOCKS_DIRECTORY) + _name + "/" + files[fileId]).c_str());
			if (filename_without_extension == "tex")
			{
				block->SetTexture(texture);
			}
			else if (filename_without_extension == "map")
			{
				block->SetEffectsTexture(texture);
			}
		}
	}
	block->SetShader(GetGameData()->shaders[Shaders::GEOMETRY]);
	block->SetName(_name);
	blocks.push_back(block);
}

void Blocks::MaterialCheck(Block* _block, const char* _materialName)
{
	if (_materialName == nullptr)
	{
		if (blocks.size() > 0)
		{
			_block->SetType(blocks[0]);
		}
		return;
	}
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->GetName() == _materialName)
		{
			_block->SetType(blocks[i]);
			return;
		}
	}
}

void Blocks::Refresh()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < blocks.size(); folderId++)
	{
		for (size_t blockId = 0; blockId < blocks.size(); blockId++)
		{
			if (folders[folderId] == blocks[blockId]->GetName())
			{
				blocks[blockId]->GetTexture()->Refresh();
				continue;
			}
		}
		Load(folders[folderId]);
	}
}

void Blocks::Draw()
{
	UpdateInstanceQueue();
	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i]->Draw();
	}
}

std::vector<Blocks::BlockType*> Blocks::GetAll()
{
	return blocks;
}

void Blocks::Menu::Menu(GameData* _gameData)
{
	if (ImGui::TreeNode("Blocks Configuration"))
	{
		for (size_t blockId = 0; blockId < blocks.size(); blockId++)
		{
			std::string name = "##Block" + std::to_string(blockId) + blocks[blockId]->GetName();
			if (ImGui::TreeNode(std::string(blocks[blockId]->GetName() + name).c_str()))
			{
				if (ImGui::TreeNode((std::string("Material") + name).c_str()))
				{
					glm::vec3 dffColor = blocks[blockId]->GetDiffuse();
					ImVec4 diffuseColor(dffColor.x, dffColor.y, dffColor.z, 1.0);
					if (ImGui::ColorPicker3(std::string("Diffuse" + name).c_str(), (float*)&diffuseColor))
					{
						blocks[blockId]->SetDiffuse(glm::vec3(diffuseColor.x, diffuseColor.y, diffuseColor.z));
					}

					float shininess = blocks[blockId]->GetShininess();
					if (ImGui::SliderFloat(std::string("Shininess" + name).c_str(), &shininess, 0.0f, 1000.0f))
					{
						blocks[blockId]->SetShininess(shininess);
					}


					if (ImGui::Button(std::string("Save" + name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						//blocks[blockId]->Save();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode((std::string("Light") + name).c_str()))
				{
					//bool emitter;
					//if (ImGui::Checkbox((std::string("Light : ") + name).c_str(), &emitter))
					//{

					//}
					if (blocks[blockId]->IsLightEmitter())
					{
						Lighting::Light light = blocks[blockId]->GetLight();
						float constant = light.GetConstant();
						if (ImGui::SliderFloat((std::string("Constant : ") + name).c_str(), &constant, 0.0, 1.0))
						{
							light.SetConstant(constant);
						}
						float linear = light.GetLinear();
						if (ImGui::SliderFloat((std::string("Linear : ") + name).c_str(), &linear, 0.0, 1.0))
						{
							light.SetLinear(linear);
						}
						float quadratic = light.GetQuadratic();
						if (ImGui::SliderFloat((std::string("Quadratic : ") + name).c_str(), &quadratic, 0.0, 1.0))
						{
							light.SetQuadratic(quadratic);
						}

						float cutOff = light.GetCutOff();
						if (ImGui::SliderFloat((std::string("cutOff : ") + name).c_str(), &cutOff, 0.0, 200.0))
						{
							light.SetCutOff(cutOff);
						}

						float outerCutOff = light.GetOuterCutOff();
						if (ImGui::SliderFloat((std::string("outerCutOff : ") + name).c_str(), &outerCutOff, 0.0, 360.0))
						{
							light.SetOuterCutOff(outerCutOff);
						}


						blocks[blockId]->SetLight(light);
					}
					std::vector<Sets::Set*>* sets = Sets::GetAll();
					for (size_t i = 0; i < sets->size(); i++)
					{
						sets->at(i)->ApplyTransformation();
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}
