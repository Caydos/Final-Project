#include "RayCasting.h"
#include <utility>
#include <limits>
#include <cmath>
#include <glm/geometric.hpp>

float RayCasting::Intersect(const Ray& ray, const Bounds::Box& box)
{
    float tMin = 0.0f;
    float tMax = INFINITY;

    for (int i = 0; i < 3; ++i) {
        if (std::abs(ray.direction[i]) < std::numeric_limits<float>::epsilon()) {
            // Ray is parallel to slab. No hit if origin not within slab
            if (ray.origin[i] < box.min[i] || ray.origin[i] > box.max[i]) {
                return -1.0f;
            }
        }
        else {
            // Compute intersection t value of ray with near and far plane of slab
            float ood = 1.0f / ray.direction[i];
            float t1 = (box.min[i] - ray.origin[i]) * ood;
            float t2 = (box.max[i] - ray.origin[i]) * ood;

            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) std::swap(t1, t2);

            // Compute the intersection of slab intersection intervals
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            // Exit with no collision as soon as slab intersection becomes empty
            if (tMin > tMax) return -1.0f;
        }
    }

    // Ray intersects all 3 slabs. Return point of first intersection
    return tMin;
}



float RayCasting::IntersectPlane(const glm::vec3& planeNormal, const glm::vec3& planePoint, const Ray& ray)
{
    float denom = glm::dot(planeNormal, ray.direction);
    if (std::abs(denom) > 1e-8) { // Avoid division by zero (or near zero)
        glm::vec3 p0l0 = planePoint - ray.origin;
        float t = glm::dot(p0l0, planeNormal) / denom;
        return t;
    }
    return -1; // No intersection
}

bool RayCasting::IsPointInBounds(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds) {
    return (point.x >= minBounds.x && point.x <= maxBounds.x) &&
        (point.y >= minBounds.y && point.y <= maxBounds.y) &&
        (point.z >= minBounds.z && point.z <= maxBounds.z);
}

RayCasting::Face RayCasting::IntersectedFace(const Ray& ray, const Cube& cube) {
    float epsilon = 1e-4f; // Small value to expand bounds
    float maxDistance = 100.0f; // Maximum valid intersection distance

    
    glm::vec3 halfSize(cube.size / 2, cube.size / 2, cube.size / 2);
    glm::vec3 minBounds = cube.center - halfSize - glm::vec3(epsilon);
    glm::vec3 maxBounds = cube.center + halfSize + glm::vec3(epsilon);


    float closestT = std::numeric_limits<float>::max();
    Face closestFace = NONE;

    // Check each face
    std::pair<glm::vec3, Face> faces[6] = {
        { glm::vec3(1, 0, 0), RIGHT },
        { glm::vec3(-1, 0, 0), LEFT },
        { glm::vec3(0, 1, 0), TOP },
        { glm::vec3(0, -1, 0), BOTTOM },
        { glm::vec3(0, 0, 1), FRONT },
        { glm::vec3(0, 0, -1), BACK }
    };

    for (const auto& [normal, face] : faces) {
        glm::vec3 planePoint = cube.center + halfSize * normal;
        float t = IntersectPlane(normal, planePoint, ray);

        if (t > 0 && t < maxDistance) { // Check if intersection is in front of the ray
            glm::vec3 intersectionPoint = ray.origin + ray.direction * t;
            if (IsPointInBounds(intersectionPoint, minBounds, maxBounds) && t < closestT) {
                closestT = t;
                closestFace = face;
            }
        }
    }

    return closestFace;
}
