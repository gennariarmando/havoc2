#include "Entity.h"
#include "World.h"
#include "ATime.h"
#include "ACamera.h"
#include "Map.h"
#include "AConsole.h"
#include "AScreen.h"
#include "AEngine.h"
#include "Font.h"
#include "Collision.h"

CEntity::CEntity() {
	m_pRigidBody = new ARigidBody();
	m_pSpriteObject = new ASpriteObject();
	m_pSpriteObject->m_pSprite->SetTexture(World.GetStyle()->GetSprite().at(130)->GetID());

	m_eType = ENTITYTYPE_NONE;

	// Flags
	m_bUsesCollision = false;
	m_bIsStatic = false;
	m_bInAir = false;
}

CEntity::~CEntity() {
	delete m_pRigidBody;
	delete m_pSpriteObject;
}

void CEntity::Render() {
	m_pSpriteObject->SetAllValues(m_pRigidBody->GetPosition(), { 1.0f, 1.0f }, m_pRigidBody->GetRotation(), 0.0f - glm::half_pi<float>(), glm::vec4(1.0f));
	m_pSpriteObject->Render();
}

void CEntity::Update() {
	glm::vec3 pos = GetRigidBody()->GetPosition();
	if (GetRigidBody()->GetPosition().z < -50.0f) {
		GetRigidBody()->SetPosition({ pos.x, pos.y, 50.0f });
		GetRigidBody()->m_pBody->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
	}
}
