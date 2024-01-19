#ifndef LINE_H
#define LINE_H
#include "Drawable.h"
#include "Common.h"

#define LINE_VERTEX_COUNT 4

class Line : public Drawable
{
	using Drawable::Drawable;
public:
	Line()
	{

	}

	~Line()
	{
	}

	void Initialize(void)
	{
		this->vertexCount = LINE_VERTEX_COUNT;
		float verticesX[LINE_VERTEX_COUNT] = {
			-.5,.0f,
			.5f,.0f
		};

		std::memcpy(this->vertices, verticesX, sizeof(verticesX));
		glGenVertexArrays(1, GetVAOAddress());
		glGenBuffers(1, GetVBOAddress());
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(*GetVAOAddress());

		glBindBuffer(GL_ARRAY_BUFFER, *GetVBOAddress());
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->SetInitialize(true);
	}

	Colors::Color GetColor() { return this->color; }
	void SetColor(Colors::Color _color)
	{
		this->color = _color;
	}

	void Draw() override
	{
		if (!this->IsInitialized())
		{
			this->Initialize();
		}

		Texture* texture = this->GetTexture();
		if (this->GetTexture() != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, texture->id);
		}

		Shaders::Shader* shader = this->GetShader();
		shader->use();

		GameData* gameData = GetGameData();
		glm::mat4 projection = glm::ortho(0.0f, gameData->resolution[0] + .0f, gameData->resolution[1] + .0f, 0.0f, -1.0f, 1.0f); // Pixels

		if (this->IsUpdated())
		{
			this->model = glm::mat4(1.0f);
			model = glm::translate(model, this->GetPosition());
			glm::vec3 rotation = this->GetRotation();
			model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
			model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
			model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
			model = glm::scale(this->model, this->GetScale());
			this->SetUpdated(false);
		}

		glm::mat4 transform = projection * model;

		shader->setMat4("transform", transform);

		shader->setVec4("color", glm::vec4(this->color.values[0], this->color.values[1], this->color.values[2], this->color.values[3]));
		shader->setFloat("opacity", this->GetOpacity());

		glBindVertexArray(*GetVAOAddress()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_LINES, 0, 2);
	}

private:
	glm::mat4 model;
	Colors::Color color;
	float vertices[LINE_VERTEX_COUNT];
	int vertexCount;
};


#endif // !LINE_H