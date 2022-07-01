#pragma once
#include "common.h"
#include "ASpriteObject.h"
#include "Collision.h"

enum eEntityType {
	ENTITYTYPE_NONE,
	ENTITYTYPE_PED,
	ENTITYTYPE_VEHICLE,
	ENTITYTYPE_OBJECT,
};

class CEntity {
public:
	std::shared_ptr<ASpriteObject> m_pSpriteObject;

	std::shared_ptr<physics::RigidBody> m_pRigidBody;

	eEntityType m_eType;
	glm::vec3 m_vPosition;
	glm::vec3 m_vFront;
	glm::vec3 m_vUp;
	glm::vec3 m_vRight;
	glm::vec3 m_vScale;
	glm::vec3 m_vRotation;
	float m_fHeading;
	glm::vec3 m_vVelocity;
	float m_fHeadingVelocity;
	float m_fRotSpeed;
	float m_fMoveSpeed;

public:
	bool m_bUsesCollision : 1;
	bool m_bIsStatic : 1;
	bool m_bInAir : 1;

public:
	CEntity();
	virtual void Update();
	virtual void Render();

public:
	void ApplyGravity();
	void UpdateEntityVectors();

};
