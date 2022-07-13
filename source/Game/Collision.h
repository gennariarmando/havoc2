#pragma once
#include "common.h"

struct tBoundingBox {
    glm::vec3 max;
    glm::vec3 min;
};

struct tBoundingSphere {
    glm::vec3 center;
    float radius;
};

class CCollision {
public:
    bool PointToPlane(glm::vec3 const& point, glm::vec3 const& normal, float offset);
    bool PointInAABB(glm::vec3 const& point, tBoundingBox const& b);
    bool AABBtoAABB(tBoundingBox const& b1, tBoundingBox const& b2);
    bool SphereToSphere(tBoundingSphere const& s1, tBoundingSphere const& s2);
    bool SphereToPlane(tBoundingSphere const& s, glm::vec3 const& point, glm::vec3 const& normal);
};

extern CCollision Collision;
