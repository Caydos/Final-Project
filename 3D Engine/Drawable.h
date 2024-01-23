#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>

#include "Shader.h"
#include "Colors.h"
#include "Texture.h"

class Drawable
{
public:
	Drawable()
	{
		this->shader = nullptr;
		this->texture = nullptr;
		this->position = { .0f,.0f,.0f };
		this->rotation = { .0f,.0f,.0f };
		this->scale = { 1.0f,1.0f,1.0f };
		this->opacity = 1.0f;
		this->initialized = false;
		this->updated = true;
	}
	~Drawable()
	{
	}

	void RemoveGraphicsBuffers()
	{
		if (this->IsInitialized())
		{
			this->SetInitialize(false);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, this->GetVBOAddress());

			glBindVertexArray(0);
			glDeleteVertexArrays(1, this->GetVAOAddress());
		}
	}

	virtual void Draw() = 0;

	bool IsUpdated() { return this->updated; }
	void SetUpdated(bool _updated) { this->updated = _updated; }

	glm::vec3 GetPosition() { return this->position; }
	void SetPosition(float _x, float _y, float _z) { this->position = glm::vec3(_x, _y, _z); this->updated = true; }
	void SetPosition(glm::vec3 _position) { this->position = _position; this->updated = true; }
	void Move(glm::vec3 _position) { this->position += _position; this->updated = true; }
	void Move(float _x, float _y, float _z)
	{
		this->position.x += _x;
		this->position.y += _y;
		this->position.z += _z;
		this->updated = true;
	}

	glm::vec3 GetRotation() { return this->rotation; }
	void SetRotation(float _x, float _y, float _z) { this->rotation = glm::vec3(_x, _y, _z); this->updated = true; }
	void SetRotation(glm::vec3 _rotation) { this->rotation = _rotation; this->updated = true; }
	void Rotate(glm::vec3 _rotation) { this->rotation += _rotation; this->updated = true; }
	void Rotate(float _x, float _y, float _z)
	{
		this->rotation.x += _x;
		this->rotation.y += _y;
		this->rotation.z += _z;
		this->updated = true;
	}

	glm::vec3 GetScale() { return this->scale; }
	void SetScale(float _x, float _y, float _z) { this->scale = glm::vec3(_x, _y, _z); this->updated = true;}
	void SetScale(float _scale) { this->scale = glm::vec3(_scale, _scale, _scale); this->updated = true;}
	void SetScale(glm::vec3 _scale) { this->scale = _scale; this->updated = true; }
	void Scale(glm::vec3 _scale){ this->scale += _scale; this->updated = true; }
	void Scale(float _x, float _y, float _z)
	{
		this->scale.x += _x;
		this->scale.y += _y;
		this->scale.z += _z;
		this->updated = true;
	}

	float GetOpacity() { return this->opacity; }
	void SetOpacity(float _opacity = 1.0f) { this->opacity = _opacity; }


	Shaders::Shader* GetShader() { return this->shader; }
	void BindShader(Shaders::Shader* _shader) { this->shader = _shader; }
	Texture* GetTexture() { return this->texture; }
	void SetTexture(Texture* _texture) { this->texture = _texture; }
	unsigned int GetVBO() { return this->VBO; }
	unsigned int* GetVBOAddress() { return &this->VBO; }
	unsigned int GetVAO() { return this->VAO; }
	unsigned int* GetVAOAddress() { return &this->VAO; }
	unsigned int GetEBO() { return this->EBO; }
	unsigned int* GetEBOAddress() { return &this->EBO; }
	void SetInitialize(bool _state = true) { this->initialized = _state; }
	bool IsInitialized() { return this->initialized; }

private:
	bool updated;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float opacity;

	Shaders::Shader* shader;
	Texture* texture;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	bool initialized;
};


#endif // !DRAWABLE_H
