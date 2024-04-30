#ifndef SPRITE_H
#define SPRITE_H
#include "Drawable.h"
#include "Common.h"
#include "Clock.h"

#define SPRITE_VERTEX_COUNT 24
#define SPRITE_DIRECTORY "../Textures/"
#define SPRITE_NUMBER 6 


class Sprite : public Drawable
{
	using Drawable::Drawable;
public:
	Sprite()
	{
	}

	~Sprite()
	{
	}


	void Load(const char* _path, glm::vec3 _position, glm::vec3 _scale, int _mode)
	{
		if (strlen(_path) > 1)
		{
			Texture* texture = new Texture;
			texture->LoadFromFile(_path);
			this->SetTexture(texture);
		}

		this->SetPosition(_position);
		this->SetScale(_scale);

		mode = _mode;

	}

	//void ReleaseTexture()
	//{
	//	if (texture != nullptr)
	//	{
	//		delete texture;
	//		texture = nullptr;
	//		std::cout << "test" << std::endl;
	//	}
	//}

	//void LoadTexture(const char* _path)
	//{
	//	ReleaseTexture();
	//	this->texture = new Texture;
	//	this->texture->LoadFromFile(_path);
	//	this->SetTexture(this->texture);
	//}

	void Initialize(void)
	{
		GameData* gameData = GetGameData();

		this->vertexCount = SPRITE_VERTEX_COUNT;
		float verticesX[SPRITE_VERTEX_COUNT] = {
			// Positions   //Coordonnées de texture
			0.0f, 1.0f, 0.0f, 0.0f, // Bas gauche
			1.0f, 0.0f, 1.0f, 1.0f, // Haut droit
			0.0f, 0.0f, 0.0f, 1.0f, // Haut gauche

			0.0f, 1.0f, 0.0f, 0.0f, // Bas gauche
			1.0f, 1.0f, 1.0f, 0.0f, // Bas droit
			1.0f, 0.0f, 1.0f, 1.0f  // Haut droit
		};


		this->BindShader(gameData->shaders[Shaders::UI]);
		std::memcpy(this->vertices, verticesX, sizeof(verticesX));
		glGenVertexArrays(1, GetVAOAddress());
		glGenBuffers(1, GetVBOAddress());
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(*GetVAOAddress());

		glBindBuffer(GL_ARRAY_BUFFER, *GetVBOAddress());
		glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float), this->vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));  // Coordonnées de texture
		glEnableVertexAttribArray(1);


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

		glActiveTexture(GL_TEXTURE0);
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

		mode = this->mode;

		shader->setMat4("transform", transform);
		shader->setInt("mode", mode);
		shader->setVec4("color", glm::vec4(this->color.values[0], this->color.values[1], this->color.values[2], this->color.values[3]));
		shader->setFloat("opacity", this->GetOpacity());

		glBindVertexArray(*GetVAOAddress()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	bool IsMouseOverQuad(const glm::vec2& mousePos)
	{
		glm::vec3 scale = this->GetScale();
		glm::vec3 position = this->GetPosition();
		glm::vec2 halfScale(scale.x / 2, scale.y / 2);
		if (mousePos.x >= position.x
			&& mousePos.x <= position.x + scale.x
			&& mousePos.y >= position.y
			&& mousePos.y <= position.y + scale.y
			)
		{
			return true;
		}

		return false;
	}


private:
	glm::mat4 model;
	Colors::Color color;
	float vertices[SPRITE_VERTEX_COUNT];
	int vertexCount;
	int mode = 1;
};


struct AnimationM
{
	std::vector<Texture*> textures;
	Clock clock;
	float tempsMax = 2.0f;
	int imageCourante = 0;

	void AddTexture(const char* _path)
	{
		Texture* texture = new Texture;
		texture->LoadFromFile(_path);
		textures.push_back(texture);
	}

	template<typename ...string>
	void Load(float _tempsMax, string..._txtpath)
	{
		this->tempsMax = _tempsMax;
		((AddTexture(_txtpath)), ...);
	}

	void Animate(Sprite* _sprite)
	{

		float tempsCourant = this->clock.GetElapsedTime() / 1000;
		if (tempsCourant >= this->tempsMax) //Gestion du dépassement de l'animation
		{
			this->clock.Restart();
			tempsCourant = 0;
		}

		//Animation
		imageCourante = (tempsCourant * textures.size()) / this->tempsMax;
		_sprite->SetTexture(textures[imageCourante]);
	}
};
#endif // !SPRITE_H