#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Drawable.h"

class Rectangle : public Drawable
{
	using Drawable::Drawable;
public:
	Rectangle()
	{
		indices[0] = 0; indices[1] = 1; indices[2] = 3;
		indices[3] = 1; indices[4] = 2; indices[5] = 3;
	}

	~Rectangle()
	{
	}

	void SetVertices(float* _address, unsigned int _count)
	{
		this->vertices = _address;
		this->vertexCount = _count;

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(this->VAO);


		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Draw()
	{
		if (this->texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, this->texture->id);
		}
		// Setup view projection
		float aspectRatio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		// Create transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->position);
		model = glm::rotate(model, /*(float)glfwGetTime()*/glm::radians(this->angle), glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, this->scale);

		glm::mat4 transform = projection * model;


		this->shader->use();

		// get matrix's uniform location and set matrix
		unsigned int transformLoc = glGetUniformLocation(this->shader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(this->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	unsigned int indices[6];
};


#endif // !RECTANGLE_H