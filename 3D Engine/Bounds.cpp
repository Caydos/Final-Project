#include "Bounds.h"
#include <glm/gtx/matrix_decompose.hpp>



Bounds::BoundingBox::BoundingBox()
{
    this->initialized = false;
}
Bounds::BoundingBox::~BoundingBox() {}

void Bounds::BoundingBox::Initialize()
{
    // Create VBO and VAO
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);

    // Bind VAO and VBO, upload vertex data
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->lines), this->lines, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->shader = GetGameData()->shaders[Shaders::SINGLE_DRAW];
    this->initialized = true;
}


void Bounds::BoundingBox::Draw()
{
    if (!initialized)
    {
        this->Initialize();
    }

    this->shader->setBool("instanceUsage", false);
    this->shader->setMat4("model", this->model);
    this->shader->setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    this->shader->setFloat("opacity", 1.0f);
    glBindVertexArray(this->VAO);

    // Draw the lines
    glDrawArrays(GL_LINES, 0, 24);

    // Unbind the VAO
    glBindVertexArray(0);
}

Bounds::Box Bounds::BoundingBox::GetBox()
{
    return this->box;
}

void Bounds::BoundingBox::SetBox(Box _box)
{
    if (!initialized)
    {
        this->Initialize();
    }
    this->box = _box;
    glm::vec3 min = _box.min;
    glm::vec3 max = _box.max;

    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z)
    };

    int lineIndices[24] = {
            0, 1, 1, 3, 3, 2, 2, 0, // Front face
            4, 5, 5, 7, 7, 6, 6, 4, // Back face
            0, 4, 1, 5, 2, 6, 3, 7  // Connectors
    };

    for (int i = 0; i < 24; ++i)
    {
        glm::vec3 point = corners[lineIndices[i]];
        lines[i * 3] = point.x;
        lines[i * 3 + 1] = point.y;
        lines[i * 3 + 2] = point.z;
    }

    this->model = glm::mat4(1.0f);

    if (initialized)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->lines), this->lines, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

Bounds::Box::Box()
{
}
