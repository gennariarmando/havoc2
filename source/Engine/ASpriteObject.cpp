#include "ASpriteObject.h"

ASpriteObject::ASpriteObject() {
	m_pSprite = new ASprite();
	m_vPosition = glm::vec4(0.0f);
	m_vScale = glm::vec2(1.0f);
	m_vRotation = glm::vec4(0.0f);
	m_fAngle = 0.0f;
	m_vColor = glm::vec4(1.0f);
}

ASpriteObject::~ASpriteObject() {
	delete m_pSprite;
}

void ASpriteObject::Render() {
	m_pSprite->Draw(m_vPosition, m_vScale, m_vRotation, m_fAngle, m_vColor);
}

void ASpriteObject::SetAllValues(glm::vec3 const& pos, glm::vec2 scale, glm::vec3 rot, float angle, glm::vec4 col) {
	m_vPosition = pos;
	m_vPosition.z += 1.1f;
	m_vScale = scale;
	m_vRotation = rot;
	m_fAngle = angle;
	m_vColor = col;
}
