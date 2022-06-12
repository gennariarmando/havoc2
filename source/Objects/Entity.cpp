#include "Entity.h"

CEntity::CEntity() {
	m_vPosition = { 0.0f, 0.0f, 0.0f };
	m_vScale = { 1.0f, 1.0f };
	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pFlipbook = std::make_shared<CFlipbook>();
	m_pSprite = std::make_shared<CSprite>();
}

void CEntity::BeginPlay() {

}

void CEntity::Update() {

}

void CEntity::LateUpdate() {

}

void CEntity::Render() {
	m_pSprite->DrawPlane(m_vPosition, m_vScale, m_vColor);
}

void CEntity::EndPlay() {

}
