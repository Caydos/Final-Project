#ifndef CUBE_H
#define CUBE_H
#include "Common.h"
#include "Drawable.h"
#include "Material.h"
#include "Lightning.h"
#include "Files.h"
#include "Bounds.h"

#define CUBE_VERTEX_COUNT 396

class Cube : public Drawable
{
	using Drawable::Drawable;
public:
	Cube()
	{
		this->lightDependent = true;
		this->light = nullptr;
		this->material = nullptr;
		this->id = -1;
		this->attachment = nullptr;
	}
	~Cube()
	{

	}

	void GenerateGraphicsBuffers(void)
	{
		SetInitialize(true);

		glGenVertexArrays(1, this->GetVAOAddress());
		glGenBuffers(1, this->GetVBOAddress());
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(*this->GetVAOAddress());

		glBindBuffer(GL_ARRAY_BUFFER, *this->GetVBOAddress());
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// lightning normal attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glm::mat4 GetModel() { return this->model; }
	void SetAttachment(glm::mat4* _attachment)
	{
		this->attachment = _attachment;
		this->RemoveGraphicsBuffers();
		this->SetUpdated(true);
	}
	void RemoveAttachment()
	{
		this->attachment = nullptr;
		this->GenerateGraphicsBuffers();
		this->SetUpdated(true);
	}

	bool GetLightDependency() { return this->lightDependent; }
	void SetLightDependency(bool _value = true)
	{
		this->lightDependent = _value;
	}


	Colors::Color GetColor() { return this->color; }
	void SetColor(Colors::Color _color)
	{
		this->color = _color;
	}

	void SetMaterial(Material* _material, bool _overrideTexture = false)
	{
		this->material = _material;
		if (_overrideTexture)
		{
			this->SetTexture(_material->GetTexture());
		}
	}
	Material* GetMaterial() { return this->material; }
	void RemoveMaterial(bool _overrideTexture = true)
	{
		this->SetMaterial(nullptr);
		if (_overrideTexture)
		{
			this->SetTexture(nullptr);
		}
	}

	Lightning::Light* GetLight() { return this->light; }
	void SetLight(Lightning::Light* _light)
	{
		this->light = _light;
	}

	void SetPositionFromScreenPoint(float _x, float _y)
	{
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
		GameData* gameData = GetGameData();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), gameData->resolution[0] / gameData->resolution[1], 0.1f, 100.0f);

		// Model matrix for the cube
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		glm::mat4 tmpView(1.0f);
		glm::vec3 screenPos = glm::vec3(_x, SCREEN_HEIGHT - _y, 1.0f);
		glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);
		worldPos = worldPos / (worldPos.z * -1.f);
		this->SetPosition(worldPos.x, worldPos.y, worldPos.z);
	}

	void Draw() override
	{
		if (!this->IsInitialized())
		{
			Logger::Write("Cube graphics not initalized\n");
			this->GenerateGraphicsBuffers();
			return;
		}
		int mode = 0;
		Texture* texture = this->GetTexture();
		Colors::Color color = this->GetColor();
		if (texture != nullptr)
		{
			mode++;
			glBindTexture(GL_TEXTURE_2D, texture->id);
			if (color.values[3] > 0.0f)
			{
				mode++;
			}
		}
		Shaders::Shader* shader = this->GetShader();
		shader->use();

		shader->setVec4("color", glm::vec4(color.values[0], color.values[1], color.values[2], color.values[3]));
		shader->setBool("lightDependent", this->lightDependent);
		shader->setInt("mode", mode);
		shader->setFloat("opacity", this->GetOpacity());

		if (this->material != nullptr)
		{
			shader->setVec3("material.diffuse", this->material->GetDiffuse());
			shader->setVec3("material.specular", this->material->GetSpecular());
			shader->setFloat("material.shininess", this->material->GetShininess());
		}

		// create transformations
		glBindVertexArray(this->GetVAO());

		if (this->IsUpdated())
		{
			this->Update();
		}
		shader->setMat4("model", this->model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Update()
	{
		this->model = (this->attachment != nullptr) ? *this->attachment : glm::mat4(1.0f);
		this->model = glm::translate(this->model, this->GetPosition());
		/*(float)glfwGetTime()*/
		glm::vec3 rotation = this->GetRotation();
		this->model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->model = glm::scale(this->model, this->GetScale());
		this->SetUpdated(false);
	}


	float* GetVertices()
	{
		return vertices;
	}
	int GetVertexCount()
	{
		return CUBE_VERTEX_COUNT;
	}

	int GetId() { return this->id; }
	void SetId(int _id) { this->id = _id; }


	/* Options */
	glm::vec3 CalculateMinPoint()
	{
		glm::vec3 position = this->GetPosition();
		glm::vec3 scale = this->GetScale();
		return glm::vec3(position.x - scale.x / 2, position.y - scale.y / 2, position.z - scale.z / 2);
	}
	glm::vec3 CalculateMaxPoint()
	{
		glm::vec3 position = this->GetPosition();
		glm::vec3 scale = this->GetScale();
		return glm::vec3(position.x + scale.x / 2, position.y + scale.y / 2, position.z + scale.z / 2);
	}

	json Encode()
	{
		json object;
		glm::vec3 position = this->GetPosition();
		object["position"] = { position.x, position.y, position.z };
		glm::vec3 rotation = this->GetRotation();
		object["rotation"] = { rotation.x, rotation.y, rotation.z };

		Colors::Color color = this->GetColor();
		object["colors"] = { color.values[0], color.values[1],color.values[2],color.values[3] };

		if (this->GetMaterial() != nullptr)
		{
			object["material"] = this->GetMaterial()->GetName();
		}

		if (this->GetLight() != nullptr)
		{
			object["light"] = this->GetLight()->Encode();
		}

		object["type"] = "Cube";


		return object;
	}
private:
	glm::mat4 model;
	glm::mat4* attachment;
	Lightning::Light* light;
	Colors::Color color;
	Material* material;
	bool lightDependent;
	float vertices[CUBE_VERTEX_COUNT] = {
		//front face			//Texture				//Lightning normal
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,				0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		1.0f / 6, 1.0f,			0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f / 6, 0.0f,			0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		1.0f / 6, 0.0f,			0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,				0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,				0.0f, 0.0f, 1.0f,

		// right face
		0.5f, -0.5f, 0.5f,		1.0f / 6, 1.0f,			1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f / 6 * 2, 1.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f / 6 * 2, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f / 6 * 2, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f / 6, 0.0f,			1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f / 6, 1.0f,			1.0f, 0.0f, 0.0f,

		//back face
		0.5f, -0.5f, -0.5f,		1.0f / 6 * 3, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f / 6 * 4, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		1.0f / 6 * 4, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		1.0f / 6 * 4, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		1.0f / 6 * 3, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		1.0f / 6 * 3, 1.0f,		0.0f, 0.0f, -1.0f,

		// left face
		-0.5f, -0.5f, -0.5f,	1.0f / 6 * 2, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f / 6 * 3, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f / 6 * 3, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f / 6 * 3, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f / 6 * 2, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f / 6 * 2, 1.0f,		-1.0f, 0.0f, 0.0f,

		// top face
		-0.5f, 0.5f, 0.5f,		1.0f / 6 * 5, 1.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f / 6 * 6, 1.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f / 6 * 6, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f / 6 * 6, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f / 6 * 5, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f / 6 * 5, 1.0f,		0.0f, 1.0f, 0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f,	1.0f / 6 * 4, 1.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f / 6 * 5, 1.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f / 6 * 5, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f / 6 * 5, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f / 6 * 4, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f / 6 * 4, 1.0f,		0.0f, -1.0f, 0.0f,

	};
	int vertexCount = CUBE_VERTEX_COUNT;
	int id;

};


#endif // !CUBE_H