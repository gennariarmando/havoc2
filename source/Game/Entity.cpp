#include "Entity.h"
#include "World.h"
#include "ATime.h"
#include "ACamera.h"
#include "Map.h"
#include "AConsole.h"
#include "AScreen.h"

CEntity::CEntity() {
	{
		m_pSpriteObject = std::make_shared<ASpriteObject>();
		m_pSpriteObject->m_pSprite->SetTexture(World.GetStyle()->GetSprite().at(130));
	}

	m_eType = ENTITYTYPE_NONE;
	m_vPosition = {};
	m_vFront = glm::vec3(0.0f, -1.0f, 0.0f);
	m_vUp = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vRight = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vScale = { 1.0f, 1.0f, 1.0f };
	m_vRotation = { 0.0f, 0.0f, 1.0f };
	m_fHeading = 0.0f;

	m_vVelocity = { 0.0f, 0.0f, 0.0f };
	m_fHeadingVelocity = 0.0f;
	m_fRotSpeed = 1.0f;
	m_fMoveSpeed = 1.0f;

	// Flags
	m_bUsesCollision = false;
	m_bIsStatic = false;
	m_bInAir = false;
}

void CEntity::Render() {
	m_pSpriteObject->SetAllValues(m_vPosition, m_vScale, m_vRotation, m_fHeading - glm::half_pi<float>(), glm::vec4(1.0f));
	m_pSpriteObject->Render();
}

void CEntity::Update() {
	UpdateEntityVectors();

	m_bInAir = false;

	ApplyGravity();
}

void CEntity::ApplyGravity() {
	if (!m_bIsStatic)
		m_vVelocity.z -= World.GetGravity() * Time.GetDeltaTime();
}

void CEntity::UpdateEntityVectors() {
	glm::vec3 f;
	f.x = cos(m_fHeading) * cos(0.0f);
	f.y = sin(m_fHeading) * cos(0.0f);
	f.z = sin(0.0f);

	glm::vec3 u(0.0f, 0.0f, 1.0f);

	m_vFront = glm::normalize(f);
	m_vRight = glm::normalize(glm::cross(u, m_vFront));
	m_vUp = glm::normalize(glm::cross(m_vFront, m_vRight));

	m_vPosition.x += m_vVelocity.x * Time.GetDeltaTime();
	m_vPosition.y += m_vVelocity.y * Time.GetDeltaTime();
	m_vPosition.z += m_vVelocity.z * Time.GetDeltaTime();
	m_fHeading += (m_fHeadingVelocity * m_fRotSpeed) * Time.GetDeltaTime();
	m_fHeading = LimitAngle(m_fHeading);
}
