#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Drawable.h"

#define TRIANGLE_VERTEX_COUNT 15

class Triangle : public Drawable
{
	using Drawable::Drawable;
public:
	Triangle()
	{
	}

	~Triangle()
	{
	}

	void Initialize(void)
	{
		this->vertexCount = TRIANGLE_VERTEX_COUNT;
		float vertices[TRIANGLE_VERTEX_COUNT] = {
			-0.5f, -0.5f, 0.0f,		0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		1.0f, 1.0f
		};
		std::memcpy(this->vertices, vertices, this->vertexCount * sizeof(float));

		glGenVertexArrays(1, this->GetVAOAddress());
		glGenBuffers(1, this->GetVBOAddress());
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(*this->GetVAOAddress());

		glBindBuffer(GL_ARRAY_BUFFER, *this->GetVBOAddress());
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->SetInitialize(true);
	}

	void Draw() override
	{
		if (!this->IsInitialized())
		{
			this->Initialize();
		}
		Texture* texture = this->GetTexture();
		if (texture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->id);
		}

		Shaders::Shader* shader = this->GetShader();
		shader->use();

		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, this->GetPosition());
		glm::vec3 rotation = this->GetRotation();
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, this->GetScale());

		shader->use();

		// get matrix's uniform location and set matrix
		unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(*this->GetVAOAddress()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	float vertices[TRIANGLE_VERTEX_COUNT];
	int vertexCount;
};


#endif // !TRIANGLE_H