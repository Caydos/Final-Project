#include "Crosshair.h"
#include "Files.h"


Crosshairs::Crosshair::Crosshair()
{
	this->name = "Default";
	this->verticalSize = 50;
	this->horizontalSize = 50;

	GameData* gameData = GetGameData();

	this->lines[0].SetScale(this->horizontalSize, this->horizontalSize, this->horizontalSize);
	this->lines[1].SetScale(this->verticalSize, this->verticalSize, this->verticalSize);
	this->lines[1].SetRotation(glm::vec3(.0f,.0f,90.0f));

	this->active = true;
}

Crosshairs::Crosshair::~Crosshair()
{
}

void Crosshairs::Crosshair::Initialize()
{
	GameData* gameData = GetGameData();
	this->lines[0].BindShader(gameData->shaders[Shaders::UI]);
	this->lines[1].BindShader(gameData->shaders[Shaders::UI]);
	this->lines[0].SetPosition(glm::vec3(gameData->resolution[0] / 2, gameData->resolution[1] / 2, .0f));
	this->lines[1].SetPosition(glm::vec3(gameData->resolution[0] / 2, gameData->resolution[1] / 2, .0f));

	this->LoadFromFile("Default");
}

std::string Crosshairs::Crosshair::GetName()
{
	return this->name;
}

void Crosshairs::Crosshair::SetName(std::string _name)
{
	this->name = _name;
}

bool Crosshairs::Crosshair::IsActive()
{
	return this->active;
}

void Crosshairs::Crosshair::SetActive(bool _active)
{
	this->active = _active;
}

Colors::Color Crosshairs::Crosshair::GetColor()
{
	return this->color;
}

void Crosshairs::Crosshair::SetColor(Colors::Color _color)
{
	this->color = _color;
	lines[0].SetColor(_color);
	lines[1].SetColor(_color);
}

float Crosshairs::Crosshair::GetHorizontalSize()
{
	return this->horizontalSize;
}

void Crosshairs::Crosshair::SetHorizontalSize(float _size)
{
	this->horizontalSize = _size;
	lines[0].SetScale(glm::vec3(_size, _size, .0f));
}


float Crosshairs::Crosshair::GetVerticalSize()
{
	return this->verticalSize;
}

void Crosshairs::Crosshair::SetVerticalSize(float _size)
{
	this->verticalSize = _size;
	lines[1].SetScale(glm::vec3(_size, _size, .0f));
}

void Crosshairs::Crosshair::LoadFromFile(const char* _fileName)
{
	this->SetName(_fileName);
	std::string path = CROSSHAIR_DIRECTORY;
	path += this->GetName();
	path += CROSSHAIR_FILE_EXTENSION;

	std::string crosshairFileContent = Files::GetFileContent(path.c_str());
	if (crosshairFileContent.size() > 3)
	{
		json content = json::parse(crosshairFileContent);
		if (content.contains("horizontalSize"))
		{
			this->SetHorizontalSize(content["horizontalSize"]);
		}
		if (content.contains("verticalSize"))
		{
			this->SetVerticalSize(content["verticalSize"]);
		}
		if (content.contains("color"))
		{
			this->SetColor(Colors::Color(content["color"][0], content["color"][1], content["color"][2], content["color"][3]));
		}
		if (content.contains("active"))
		{
			this->SetActive(content["active"]);
		}
		return;
	}
	std::cout << "Failed to load crossair " << _fileName << '.' << std::endl;
}

void Crosshairs::Crosshair::Save()
{
	json jsonObject;
	jsonObject["horizontalSize"] = this->GetHorizontalSize();
	jsonObject["verticalSize"] = this->GetVerticalSize();
	jsonObject["color"] = this->GetColor().values;
	jsonObject["active"] = this->IsActive();

	Files::Create(CROSSHAIR_DIRECTORY, this->GetName().c_str(), CROSSHAIR_FILE_EXTENSION, jsonObject.dump().c_str());
}

void Crosshairs::Crosshair::Draw()
{
	lines[0].Draw();
	lines[1].Draw();
}

static Crosshairs::Crosshair crosshair;
static bool initialized = false;


Crosshairs::Crosshair* Crosshairs::Get()
{
	return &crosshair;
}

void Crosshairs::Draw()
{
	if (!initialized)
	{
		crosshair.Initialize();
		initialized = true;
	}
	if (crosshair.IsActive())
	{
		crosshair.Draw();
	}
}

void Crosshairs::Menu(GameData* _gameData)
{
	if (ImGui::TreeNode("Crosshair"))
	{
		bool activation = crosshair.IsActive();
		if (ImGui::Checkbox("Activation##Crosshair", &activation))
		{
			crosshair.SetActive(activation);
		}

		int horizontalSize = crosshair.GetHorizontalSize();
		if (ImGui::SliderInt("Horizontal size##Crosshair", &horizontalSize, 1, _gameData->resolution[0] / 2))
		{
			crosshair.SetHorizontalSize(horizontalSize);
		}
		int verticalSize = crosshair.GetVerticalSize();
		if (ImGui::SliderInt("Vertical size##Crosshair", &verticalSize, 1, _gameData->resolution[1] / 2))
		{
			crosshair.SetVerticalSize(verticalSize);
		}

		ImVec4 actualColor = crosshair.GetColor().imguiValues;
		ImGui::Text("Alpha :");
		if (ImGui::SliderFloat("##COLOR_ALPHA", &actualColor.w, 0.0f, 1.0f))
		{
			crosshair.SetColor(Colors::Color(actualColor.x, actualColor.y, actualColor.z, actualColor.w));
		}

		for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
		{
			ImGui::PushID(i);
			if (ImGui::ColorButton("##DefaultLightColors##crosshair", Colors::DefaultColors[i]))
			{
				crosshair.SetColor(Colors::Color(Colors::DefaultColors[i].x, Colors::DefaultColors[i].y, Colors::DefaultColors[i].z, Colors::DefaultColors[i].w));
			}
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::NewLine();

		ImVec4 color = crosshair.GetColor().imguiValues;
		if (ImGui::ColorPicker4("Custom Color##crosshair", (float*)&color))
		{
			crosshair.SetColor(Colors::Color(color.x, color.y, color.z, color.w));
		}

		if (ImGui::Button("Save##crosshair"))
		{
			crosshair.Save();
		}

		ImGui::TreePop();
	}
}
