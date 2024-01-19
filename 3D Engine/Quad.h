#ifndef QUAD_H
#define QUAD_H
#include "Drawable.h"
#include "Common.h"

#define QUAD_VERTEX_COUNT 20

class Quad : public Drawable
{
	using Drawable::Drawable;
public:
	Quad()
	{
		hovered = false;
	}

	~Quad()
	{
	}

	void Initialize(void)
	{
		this->vertexCount = QUAD_VERTEX_COUNT;
		float vertices[QUAD_VERTEX_COUNT] = {
			0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,   // top left 
		};
		std::memcpy(this->vertices, vertices, this->vertexCount * sizeof(float));

		indices[0] = 0; indices[1] = 1; indices[2] = 3;
		indices[3] = 1; indices[4] = 2; indices[5] = 3;

		glGenVertexArrays(1, this->GetVAOAddress());
		glGenBuffers(1, this->GetVBOAddress());
		glGenBuffers(1, this->GetEBOAddress());
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(*this->GetVAOAddress());


		glBindBuffer(GL_ARRAY_BUFFER, *this->GetVBOAddress());
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *this->GetEBOAddress());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
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
		// Setup view projection
		glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH + .0f, SCREEN_HEIGHT + .0f, 0.0f, -1.0f, 1.0f); // Pixels

		// Create transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->GetPosition());
		glm::vec3 rotation = this->GetRotation();
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, this->GetScale());

		glm::mat4 transform = projection * model;

		Shaders::Shader* shader = this->GetShader();
		shader->use();

		shader->setMat4("transform", transform);

		shader->setInt("mode", mode);
		shader->setVec4("color", glm::vec4(color.values[0], color.values[1], color.values[2], color.values[3]));
		shader->setFloat("opacity", this->GetOpacity());

		glBindVertexArray(*this->GetVAOAddress()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glm::mat4 GetModelMatrix() 
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->GetPosition());
		glm::vec3 rotation = this->GetRotation();
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, this->GetScale());
		return model;
	}

	bool IsMouseOverQuad(const glm::vec2& mousePos) 
	{
		hovered = false;
		glm::vec3 scale = this->GetScale();
		glm::vec3 position = this->GetPosition();
		glm::vec2 halfScale(scale.x / 2, scale.y / 2);
		if (mousePos.x >= position.x - halfScale.x
			&& mousePos.x <= position.x + halfScale.x
			&& mousePos.y >= position.y - halfScale.y
			&& mousePos.y <= position.y + halfScale.y
			)
		{
			hovered = true;
		}

		return hovered;
	}

	bool IsHovered() { return hovered; }


private:
	Colors::Color color;
	unsigned int indices[6];
	float vertices[QUAD_VERTEX_COUNT];
	int vertexCount;
	bool hovered;
};


#endif // !QUAD_H