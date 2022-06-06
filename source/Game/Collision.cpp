#include "Collision.h"

bool CCollision::TestVertices(std::vector<glm::vec3> v1, std::vector<glm::vec3> v2) {
    bool collision = false;
    for (auto& iter1 : v1) {
        for (auto& iter2 : v2) {
            float dist = glm::distance(iter1, iter2);

            if (dist <= 0.1f)
                return true;
        }
    }

    return false;
}
