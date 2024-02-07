#ifndef OCCLUSION_H
#define OCCLUSION_H
#include "Common.h"
#include "Bounds.h"

namespace Occlusion
{
    //bool isBoundingBoxBehindAnother(
    //    const glm::vec3& camPos, const glm::vec3& camViewDir,
    //    const glm::vec3& box1Min, const glm::vec3& box1Max,
    //    const glm::vec3& box2Min, const glm::vec3& box2Max) {

    //    // Calculate bounding box centers
    //    glm::vec3 box1Center = (box1Min + box1Max) / 2.0f;
    //    glm::vec3 box2Center = (box2Min + box2Max) / 2.0f;

    //    // Direction vectors from the camera to the bounding box centers
    //    glm::vec3 dirToBox1 = glm::normalize(box1Center - camPos);
    //    glm::vec3 dirToBox2 = glm::normalize(box2Center - camPos);

    //    // Project these direction vectors onto the camera's view direction
    //    float projection1 = glm::dot(dirToBox1, camViewDir);
    //    float projection2 = glm::dot(dirToBox2, camViewDir);

    //    // Distance from the camera to the bounding box centers
    //    float distance1 = glm::length(box1Center - camPos);
    //    float distance2 = glm::length(box2Center - camPos);

    //    // Determine if box2 is behind box1
    //    return projection1 > projection2 && distance1 < distance2;
    //}
	bool IsOccluded(Bounds::Box _firstBox, Bounds::Box _secondBox)
	{





		return false;
	}
}


#endif // !OCCLUSION_H