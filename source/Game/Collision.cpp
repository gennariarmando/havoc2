#include "Collision.h"

CCollision Collision;

const float epsilon = 0.0f;

bool CCollision::PointToPlane(glm::vec3 const& point, glm::vec3 const& normal, float offset) {
	float dist = glm::dot(point, normal) - offset;
	bool collision = false;

	if (dist > epsilon) {
		// In front
	}
	else if (dist < -epsilon) {
		// Behind
	}
	else
		collision = true;

	return collision;
}

bool CCollision::PointInAABB(glm::vec3 const& point, tBoundingBox const& b) {
	bool collision = (point.x > b.min.x && point.x < b.max.x) &&
		(point.y > b.min.y && point.y < b.max.y) &&
		(point.z > b.min.z && point.z < b.max.z);

	return collision;
}

bool CCollision::AABBtoAABB(const tBoundingBox& b1, const tBoundingBox& b2) {
	bool collision = (b1.max.x > b2.min.x && b1.min.x < b2.max.x) &&
		(b1.max.y > b2.min.y && b1.min.y < b2.max.y) &&
		(b1.max.z > b2.min.z && b1.min.z < b2.max.z);

	return collision;
}

bool CCollision::SphereToSphere(tBoundingSphere const& s1, tBoundingSphere const& s2) {
	glm::vec3 vecDist = s2.center - s2.center;
	float dist = glm::dot(vecDist, vecDist);

	float fRadiiSumSquared(s1.radius + s1.radius);
	fRadiiSumSquared *= fRadiiSumSquared;

	if (dist <= fRadiiSumSquared)
		return true;

	return false;
}

bool CCollision::SphereToPlane(tBoundingSphere const& s, glm::vec3 const& point, glm::vec3 const& normal) {
	glm::vec3 vecTemp = s.center - point;
	float dist = glm::dot(vecTemp, normal);

	if (dist > s.radius)
		return false;

	return true;
}
