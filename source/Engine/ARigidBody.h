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
public:
	glm::vec3 m_vPos;
	float m_fHeading;
	glm::vec3 m_vVelocity;
	glm::int8 m_nBodyType;
	bool m_bEnabled;

public:
	ARigidBody();
	~ARigidBody();

	const glm::vec3 GetLinearVelocity();

public:
	const glm::vec3 GetPosition();
	const float GetHeading();

public:
	void SetPosition(glm::vec3 const& pos);
	void SetHeading(float angle);
	void SetType(glm::uint8 type);
	glm::uint8 GetType();
	void SetLinearVelocity(glm::vec3 const& vel);
	void ApplyLinearVelocity(glm::vec3 const& vel);
	void ClearVelocity();
	void SetEnabled(bool on);
	bool const& IsEnabled();
};
