#ifndef MATERIAL_H
#define MATERIAL_H
#include "Texture.h"
#include <glm/vec3.hpp>
#include "Files.h"

#define MATERIAL_DIRECTORY "../Materials/"
#define MATERIAL_FILE_EXTENSION ".json"

class Material
{
public:
	Material() {};
	~Material() {};

	Texture* GetTexture() { return this->texture; }
	void SetTexture(Texture* _texture) { this->texture = _texture; }

	glm::vec3 GetDiffuse() { return this->diffuse; }
	void SetDiffuse(glm::vec3 _diffuse) { this->diffuse = _diffuse; }

	glm::vec3 GetSpecular() { return this->specular; }
	void SetSpecular(glm::vec3 _specular) { this->specular = _specular; }

	float GetShininess() { return this->shininess; }
	void SetShininess(float _shininess) { this->shininess = _shininess; }

	void SetName(const char* _fileName) { this->name = _fileName; }
	std::string GetName() { return this->name; }

	void LoadFromFile(const char* _fileName)
	{
		this->SetName(_fileName);

		std::string path = MATERIAL_DIRECTORY;
		path += _fileName;
		path += '/';


		std::string matFile = path + _fileName;
		matFile += MATERIAL_FILE_EXTENSION;
		std::string content = Files::GetFileContent(matFile.c_str());
		if (content.size() < 3)
		{
			std::cout << "Unable to load " << _fileName << " material." << std::endl;
			return;
		}
		json jsonParsed = json::parse(content);
		diffuse = glm::vec3(jsonParsed["diffuse"][0], jsonParsed["diffuse"][1], jsonParsed["diffuse"][2]);
		specular = glm::vec3(jsonParsed["specular"][0], jsonParsed["specular"][1], jsonParsed["specular"][2]);
		shininess = jsonParsed["shininess"];

		std::string imgFile = path + _fileName;
		imgFile += ".jpg";

		Texture* txt = new Texture;
		txt->LoadFromFile(imgFile.c_str());
		this->texture = txt;
	};
private:
	std::string name;
	Texture* texture;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

#endif // !MATERIAL_H