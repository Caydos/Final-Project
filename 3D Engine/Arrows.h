#ifndef ARROWS_H
#define ARROWS_H

#include "Common.h"
#include "Drawable.h"

class Arrow : public Drawable
{
	using Drawable::Drawable;
public:
	Arrow()
	{
		this->lightDependent = false;
		this->id = -1;
	}
	~Arrow()
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

	Colors::Color GetColor() { return this->color; }
	void SetColor(Colors::Color _color)
	{
		this->color = _color;
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
			Logger::Write("Arrow graphics not initalized\n");
			this->GenerateGraphicsBuffers();
			return;
		}

		Colors::Color color = this->GetColor();

		Shaders::Shader* shader = this->GetShader();
		shader->use();

		shader->setVec4("color", glm::vec4(color.values[0], color.values[1], color.values[2], color.values[3]));
		shader->setBool("lightDependent", this->lightDependent);
		shader->setInt("mode", 0);
		shader->setFloat("opacity", this->GetOpacity());

		// create transformations
		glBindVertexArray(this->GetVAO());

		if (this->IsUpdated())
		{
			this->Update();
		}
		shader->setMat4("model", this->model);

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawArrays(GL_LINES, 0, 2);

		// Draw the arrowhead
		glDrawArrays(GL_TRIANGLES, 2, 3);
	}

	void Update()
	{
		this->model = glm::mat4(1.0f);
		this->model = glm::translate(this->model, this->GetPosition());
		/*(float)glfwGetTime()*/
		glm::vec3 rotation = this->GetRotation();
		this->model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->model = glm::scale(this->model, this->GetScale());
		this->SetUpdated(false);
	}


	int GetId() { return this->id; }
	void SetId(int _id) { this->id = _id; }

private:
	glm::mat4 model;
	Colors::Color color;
	bool lightDependent;
	float vertices[40] = {
		// Line vertices (x, y, z)
		0.0f,  0.5f, 0.0f,		0.0f, 0.0f,  0.0f,0.0f,0.0f,
		0.0f, -0.5f, 0.0f,		0.0f, 0.0f,  0.0f,0.0f,0.0f,
		// Arrowhead vertices (x, y, z)
		-0.05f, -0.4f, 0.0f,	0.0f, 0.0f,  0.0f,0.0f,0.0f,
		0.0f, -0.5f, 0.0f,		0.0f, 0.0f,  0.0f,0.0f,0.0f,
		0.05f, -0.4f, 0.0f,		0.0f, 0.0f,  0.0f,0.0f,0.0f,
	};
	int vertexCount = 40;
	int id;

};


#endif // !ARROWS_H