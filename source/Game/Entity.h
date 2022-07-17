#pragma once
#include "common.h"
#include "ASpriteObject.h"
#include "ARigidBody.h"

enum eEntityType {
	ENTITYTYPE_NONE,
	ENTITYTYPE_PED,
	ENTITYTYPE_VEHICLE,
	ENTITYTYPE_OBJECT,
};

class CEntity {
public:
	ARigidBody* m_pRigidBody;
	ASpriteObject* m_pSpriteObject;
	eEntityType m_eType;
	glm::vec3 m_vFront;
	glm::vec3 m_vRight;
	glm::vec3 m_vUp;

public:
	ARigidBody* const& GetRigidBody() { return m_pRigidBody; }
	ASpriteObject* const& GetSpriteObject() { return m_pSpriteObject; }
	eEntityType const& GetType() { return m_eType; }

public:
	bool m_bUsesCollision : 1;
	bool m_bIsStatic : 1;
	bool m_bInAir : 1;

public:
	CEntity();
	~CEntity();
	virtual void Update();
	virtual void Render();

public:
	void UpdateEntityVectors();
};
