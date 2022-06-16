#pragma once
#include "common.h"

class CCollision {
public:
    static bool TestVertices(std::vector<glm::vec3> v1, std::vector<glm::vec3> v2);
    static bool AABB(glm::vec4 const& a, glm::vec4 const& b);
};
