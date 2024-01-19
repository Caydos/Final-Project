#ifndef RAYCASTING_H
#define RAYCASTING_H
#include <glm/ext/vector_float3.hpp>
#include "Bounds.h"

namespace RayCasting
{
    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    float Intersect(const Ray& ray, const Bounds::Box& box);

    struct Cube
    {
        glm::vec3 center;
        float size;
    };

    enum Face { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, NONE };

    float IntersectPlane(const glm::vec3& planeNormal, const glm::vec3& planePoint, const Ray& ray);
    bool IsPointInBounds(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds);
    Face IntersectedFace(const Ray& ray, const Cube& cube);
}

#endif // !RAYCASTING_H