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
	m_pSpriteObject->m_pSprite->SetTexture(World.GetStyle()->GetSprite().at(130));

	m_eType = ENTITYTYPE_NONE;
	m_vFront = {};
	m_vRight = {};
	m_vUp = {};

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
	float a = GetRigidBody()->GetAngle() - glm::half_pi<float>();
	m_pSpriteObject->SetAllValues(GetRigidBody()->GetPosition(), { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, a, glm::vec4(1.0f));
	m_pSpriteObject->Render();
}

void CEntity::Update() {
	if (!GetRigidBody())
		return;

	UpdateEntityVectors();
} 
void CEntity::UpdateEntityVectors() {
	glm::vec3 f;
	f.x = cos(GetRigidBody()->GetAngle()) * cos(0.0f);
	f.y = sin(GetRigidBody()->GetAngle()) * cos(0.0f);
	f.z = sin(0.0f);

	glm::vec3 u(0.0f, 0.0f, 1.0f);

	m_vFront = glm::normalize(f);
	m_vRight = glm::normalize(glm::cross(u, m_vFront));
	m_vUp = glm::normalize(glm::cross(m_vFront, m_vRight));
}
