#include "Bounds.h"



bool Bounds::AreColliding(const Box& box1, const Box& box2, glm::vec3& collisionNormal)
{
    float ra, rb;
    glm::mat3 R, AbsR;

    // Compute rotation matrix expressing box2 in box1’s coordinate frame
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R[i][j] = glm::dot(box1.rotation[i], box2.rotation[j]);
        }
    }

    // Compute translation vector t
    glm::vec3 t = box2.position - box1.position;
    // Bring the translation into box1's coordinate frame
    t = glm::vec3(glm::dot(t, box1.rotation[0]), glm::dot(t, box1.rotation[1]), glm::dot(t, box1.rotation[2]));

    // Compute common subexpressions. Add in an epsilon term to counteract arithmetic errors when two edges are parallel and their cross product is (near) null (this might not be necessary but is often included for safety).
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            AbsR[i][j] = fabs(R[i][j]) + std::numeric_limits<float>::epsilon();
        }
    }

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++) {
        ra = box1.extents[i];
        rb = box2.extents[0] * AbsR[i][0] + box2.extents[1] * AbsR[i][1] + box2.extents[2] * AbsR[i][2];
        if (fabs(t[i]) > ra + rb) return false;
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = box1.extents[0] * AbsR[0][i] + box1.extents[1] * AbsR[1][i] + box1.extents[2] * AbsR[2][i];
        rb = box2.extents[i];
        if (fabs(glm::dot(t, box1.rotation[i])) > ra + rb) return false;
    }

    // Test axis L = A0 x B0
    ra = box1.extents[1] * AbsR[2][0] + box1.extents[2] * AbsR[1][0];
    rb = box2.extents[1] * AbsR[0][2] + box2.extents[2] * AbsR[0][1];
    if (fabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

    // ... (additional axis tests need to be implemented here, 9 in total for cross products)

    // No separating axis found, the OBBs must be intersecting
    return true;
}

std::vector<glm::vec3> Bounds::GetCorners(const Box& box)
{
    // Local coordinates of the box corners relative to its center
    std::vector<glm::vec3> localCorners = {
        { -box.extents.x, -box.extents.y, -box.extents.z },
        {  box.extents.x, -box.extents.y, -box.extents.z },
        {  box.extents.x,  box.extents.y, -box.extents.z },
        { -box.extents.x,  box.extents.y, -box.extents.z },
        { -box.extents.x, -box.extents.y,  box.extents.z },
        {  box.extents.x, -box.extents.y,  box.extents.z },
        {  box.extents.x,  box.extents.y,  box.extents.z },
        { -box.extents.x,  box.extents.y,  box.extents.z }
    };

    std::vector<glm::vec3> worldCorners;
    worldCorners.reserve(8);

    // Transform the local corners to world space
    for (const auto& corner : localCorners)
    {
        glm::vec3 worldCorner = box.position + box.rotation * corner;
        worldCorners.push_back(worldCorner);
    }

    return worldCorners;
}



Bounds::BoundingBox::BoundingBox()
{
    this->initialized = false;
}
Bounds::BoundingBox::~BoundingBox() {}
#include "Blocks.h"
#include <glm/gtx/matrix_decompose.hpp>
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
    glm::vec3 min = box.position - box.extents;
    glm::vec3 max = box.position + box.extents;
    //std::cout << "Min Bounds point : " << min.x << " " << min.y << " " << min.z << std::endl;
    //std::cout << "Max Bounds point : " << max.x << " " << max.y << " " << max.z << std::endl;

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
    // Create a translation matrix
    //for (int i = 0; i < 3; ++i)
    //{
    //    for (int j = 0; j < 3; ++j)
    //    {
    //        this->model[i][j] = this->box.rotation[i][j];
    //    }
    //}
    //this->model *= glm::mat4(this->box.rotation);
    //this->model = glm::rotate(this->model, glm::radians(45.0f), glm::vec3(1, 0, 0));
    //this->model = glm::rotate(this->model, glm::radians(45.0f), glm::vec3(0, 1, 0));
    //this->model = glm::rotate(this->model, glm::radians(45.0f), glm::vec3(0, 0, 1));

    if (initialized)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->lines), this->lines, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

Bounds::Box::Box()
{
    this->extents = glm::vec3(1.0f);
}
