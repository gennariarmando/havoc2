#pragma once
#include "common.h"
#include "APhysics.h"

enum eBodyType {
	BODYTYPE_STATIC,
	BODYTYPE_KINEMATIC,
	BODYTYPE_DYNAMIC,
};

struct tCollider {
	glm::uint32 index;

};

class ARigidBody {
private:
	rp3d::RigidBody* m_pBody;

public:
	ARigidBody();
	~ARigidBody();

public:
	glm::vec3 const& GetPosition();
	glm::vec3 const& GetRotation();
	float const& GetAngle();

	glm::vec3 const& GetLinearVelocity();
	glm::vec3 const& GetAngularVelocity();

public:
	void SetPosition(glm::vec3 const& pos);
	void SetRotation(glm::vec3 const& rot);
	void SetHeading(float angle);
	void SetType(glm::uint8 type);
	glm::uint8 GetType();
	void AddCollisionTypeBox(glm::vec3 const& pos, glm::vec3 const& scale);
	void AddCollisionTypeSphere(glm::vec3 const& pos, float const& radius);
	void SetCollisionPosition(glm::uint32 index, glm::vec3 const& pos);
	void RemoveCollision(glm::uint32 index);
	void SetLinearVelocity(glm::vec3 const& vel);
	void SetAngularVelocity(glm::vec3 const& vel);
	void ApplyLinearVelocity(glm::vec3 const& vel);
	void ApplyAngularVelocity(glm::vec3 const& vel);
	void ClearVelocity();
	void SetEnabled(bool on);
	bool const& IsEnabled();
};
