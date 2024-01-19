#ifndef WORLD_OBJECT_H
#define WOLD_OBJECT_H
#include "Drawable.h"

class WorldObject : public Drawable
{
public:
	WorldObject()
	{

	}
	~WorldObject()
	{

	}
	void SetVertices(float* _address, unsigned int _count)
	{
		this->vertices = _address;
		this->vertexCount = _count;

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Draw()
	{
		if (this->texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, this->texture->id);
		}
		this->shader->use();

		// create transformations
		glBindVertexArray(this->VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->position);
		model = glm::rotate(model, glm::radians(this->angle)/*(float)glfwGetTime()*/, glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, scale);
		this->shader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	float durability;
private:

};

#endif // !3D_OBJECT_H