#pragma once
#include "common.h"
#include "APhysics.h"

class ACollisionBody {
public:
	physics::CollisionBody* m_pBody;

public:
	ACollisionBody();
	~ACollisionBody();

public:
	glm::vec3 const& GetPosition();
	glm::vec3 const& GetRotation();

public:
	void SetPosition(glm::vec3 const& pos);
	void SetRotation(glm::vec3 const& rot);
};
