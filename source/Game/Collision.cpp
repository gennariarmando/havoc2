#include "Collision.h"

bool CCollision::AABB(glm::vec4 const& a, glm::vec4 const& b) {
	bool collision = false;

	if (a.x <= b.x + b.z &&
		a.x + a.z >= b.x &&
		a.y <= b.y + b.w &&
		a.y + a.w >= b.y)
		collision = true;

	return collision;
}
