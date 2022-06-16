#include "Collision.h"

bool CCollision::TestVertices(std::vector<glm::vec3> v1, std::vector<glm::vec3> v2) {
	for (auto& it1 : v1) {
		for (auto& it2 : v2) {
			if (glm::distance(it1, it2) < 0.1f)
				return true;
		}
	}

	return false;
}

bool CCollision::AABB(glm::vec4 const& a, glm::vec4 const& b) {
	bool collision = false;

	if (a.x <= b.x + b.z &&
		a.x + a.z >= b.x &&
		a.y <= b.y + b.w &&
		a.y + a.w >= b.y)
		collision = true;

	return collision;
}
