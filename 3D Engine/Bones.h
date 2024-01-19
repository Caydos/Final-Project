#ifndef BONES_H
#define BONES_H
#include "Common.h"
#include "Cube.h"

class Bone
{
public:
    Bone()
    {
        parent = nullptr;
        matrice = new glm::mat4;
        *matrice = glm::mat4(1.0f);
    }
    ~Bone() {}

    void Draw()
    {
        for (size_t i = 0; i < this->objects.size(); i++)
        {
            this->objects[i].Draw();
        }
    }

    void InsertCube()
    {
        GameData* _gameData = GetGameData();
        Cube cube;
        cube.Initialize();
        cube.SetColor(Colors::Black);
        cube.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
        cube.SetAttachment(this->matrice);
        objects.push_back(cube);
    }


    void SetRotation()
    {
        glm::vec3 rotation(1.0f, 0.5f, 0.2f);
        *this->matrice = glm::rotate(*this->matrice, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        *this->matrice = glm::rotate(*this->matrice, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        *this->matrice = glm::rotate(*this->matrice, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i].SetUpdated(true);
        }
    }


private:
    Bone* parent;
    glm::mat4* matrice;

    std::vector<Cube> objects;

    glm::vec3 position;
    glm::vec3 rotation;
};

class Skeleton
{
public:
    Skeleton() {}
    ~Skeleton() {}

    void Draw()
    {
        for (size_t i = 0; i < this->bones.size(); i++)
        {
            this->bones[i].Draw();
        }
    }

    void InsertBones()
    {
        Bone bone;

        bone.InsertCube();
        bone.SetRotation();

        bones.push_back(bone);
    }

private:
    std::vector<Bone> bones;

};


#endif // !BONES_H