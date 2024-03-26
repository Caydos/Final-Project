#include "Blocks.h"
#include "DefaultVertices.h"
#include "SidesArrows.h"
#include "Clock.h"
#include <glm/gtx/matrix_decompose.hpp>

static unsigned int VAO;
static unsigned int vertexVBO;
static unsigned int instanceVBO;
static std::vector<glm::mat4> models;
static Colors::Color color = Colors::Color(1.0f, 1.0f, 1.0f, 0.3f);
static glm::vec3 scale;
static glm::vec3 position;
static bool ghostInitialized = false;

static Clock scrollClock;
static SideArrow arrows[2];
static int AxisRestrictions[3];
static int axisId = 0;
static glm::vec3 rotations[3][2] = {
	{glm::vec3(0.0f, 0.0f, .0f), glm::vec3(0.0f, 90.0f, .0f)},//flat
	{glm::vec3(90.0f, 0.0f, .0f), glm::vec3(90.0f, 90.0f, .0f)},//Up 1
	{glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(90.0f, 0.0f, 90.0f)},//Up 2
};
static glm::vec3 planeNormals[3] = {
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(1, 0, 0),
};

static unsigned int maxBlocks = 18;

void Blocks::Ghost::Draw(GameData* _gameData)
{
	if (!ghostInitialized)
	{
		arrows[0].GenerateGraphicsBuffers();
		arrows[0].BindShader(_gameData->shaders[Shaders::SINGLE_DRAW]);
		arrows[0].SetColor(Colors::Color(TO_RGBA(35), TO_RGBA(77), TO_RGBA(168), 0.7f));
		arrows[0].SetRotation(rotations[axisId][0]);

		arrows[1].GenerateGraphicsBuffers();
		arrows[1].BindShader(_gameData->shaders[Shaders::SINGLE_DRAW]);
		arrows[1].SetColor(Colors::Color(TO_RGBA(35), TO_RGBA(77), TO_RGBA(168), 0.7f));
		arrows[1].SetRotation(rotations[axisId][1]);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vertexVBO);
		glGenBuffers(1, &instanceVBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(float), &cubeVertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Lightning normal attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		models.push_back(glm::mat4(1.0f));
		ghostInitialized = true;
	}
	if (_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
	{
		if (_gameData->window.yScroll < 0 && scrollClock.GetElapsedTime() > 50)
		{
			scrollClock.Restart();
			axisId++;
			if (axisId >= 3)
			{
				axisId = 0;
			}
			arrows[0].SetRotation(rotations[axisId][0]);
			arrows[1].SetRotation(rotations[axisId][1]);
		}
		else if (_gameData->window.yScroll > 0 && scrollClock.GetElapsedTime() > 50)
		{
			scrollClock.Restart();
			axisId--;
			if (axisId < 0)
			{
				axisId = 2;
			}
			arrows[0].SetRotation(rotations[axisId][0]);
			arrows[1].SetRotation(rotations[axisId][1]);
		}
	}
	Shaders::Shader* shader = _gameData->shaders[Shaders::SINGLE_DRAW];
	shader->use();

	shader->setBool("instanceUsage", true);
	shader->setVec4("color", glm::vec4(color.values[0], color.values[1], color.values[2], color.values[3]));
	shader->setBool("lightDependent", false);
	shader->setInt("mode", 0);
	shader->setFloat("opacity", 1.0f);
	glBindVertexArray(VAO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, models.size());

	glBindVertexArray(0);
	shader->setBool("instanceUsage", false);
	arrows[0].Draw();
	arrows[1].Draw();
}

void Blocks::Ghost::CalculateTransformations()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i] = glm::mat4(1.0f);
		models[i] = glm::translate(models[i], position);
		models[i] = glm::scale(models[i], scale);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 3); // 4 is an offset, assuming 0, 1, 2, 3 are used for the cube's vertex data
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 3, 1); // Tell OpenGL this is an instanced vertex attribute.
	}
}

void Blocks::Ghost::SetScale(glm::vec3 _scale)
{
	scale = _scale;
	arrows[0].SetScale(_scale);
	arrows[1].SetScale(_scale);
	CalculateTransformations();
}

void Blocks::Ghost::SetStartPosition(glm::vec3 _position)
{
	position = _position;
	arrows[0].SetPosition(_position);
	arrows[1].SetPosition(_position);

	models.clear();
	models.push_back(glm::mat4(1.0f));


	CalculateTransformations();
}

void Blocks::Ghost::SetRay(RayCasting::Ray _ray)
{
	glm::vec3 planeNormal = planeNormals[axisId];
	// Find the dominant axis
	glm::vec3 absNormal = glm::abs(planeNormal);
	int dominantAxis = 0; // 0 for X, 1 for Y, 2 for Z
	if (absNormal.y > absNormal.x && absNormal.y > absNormal.z) {
		dominantAxis = 1;
	}
	else if (absNormal.z > absNormal.x && absNormal.z > absNormal.y) {
		dominantAxis = 2;
	}

	float planeDistance = -glm::dot(planeNormal, position);
	float denom = glm::dot(planeNormal, _ray.direction);

	if (abs(denom) > 1e-8) {
		float t = -(glm::dot(planeNormal, _ray.origin) + planeDistance) / denom;
		if (t >= 0) {
			glm::vec3 destination = _ray.origin + t * _ray.direction;
			models.clear();

			// Determine the range based on the intersection point and the original position
			glm::ivec3 gridNumbers = glm::ivec3((destination - position) / scale);
			glm::ivec3 minGrid = glm::min(glm::ivec3(0), gridNumbers);
			glm::ivec3 maxGrid = glm::max(glm::ivec3(0), gridNumbers);
			unsigned int count = 0;
			for (int xId = minGrid.x; xId <= maxGrid.x; ++xId) {
				for (int yId = minGrid.y; yId <= maxGrid.y; ++yId) {
					for (int zId = minGrid.z; zId <= maxGrid.z; ++zId) {
						if (dominantAxis == 0 && xId != 0) continue;
						if (dominantAxis == 1 && yId != 0) continue;
						if (dominantAxis == 2 && zId != 0) continue;
						glm::vec3 offset = glm::vec3(xId, yId, zId) * scale;
						glm::mat4 model = glm::mat4(1.0f);
						model = glm::translate(model, position + offset);
						model = glm::scale(model, scale);
						count++;
						models.push_back(model);
						//if (count >= maxBlocks) { break; }
					}
					//if (count >= maxBlocks) { break; }
				}
				//if (count >= maxBlocks) { break; }
			}

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

			for (unsigned int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(i + 3); // 4 is an offset, assuming 0, 1, 2, 3 are used for the cube's vertex data
				glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisor(i + 3, 1); // Tell OpenGL this is an instanced vertex attribute.
			}
		}
	}
}

void Blocks::Ghost::RestrictAxis(Axis _axis, int _value)
{
	AxisRestrictions[_axis] = _value;
}

std::vector<glm::mat4> Blocks::Ghost::GetModels()
{
	return models;
}


