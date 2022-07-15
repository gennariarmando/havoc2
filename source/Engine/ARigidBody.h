#pragma once
#include "common.h"
#include "APhysics.h"

class ARigidBody {
public:
	physics::RigidBody* m_pBody;

public:
	ARigidBody();
	~ARigidBody();

public:
	glm::vec3 const& GetPosition();
	glm::vec3 const& GetRotation();

public:
	void SetPosition(glm::vec3 const& pos);
	void SetRotation(glm::vec3 const& rot);
};
