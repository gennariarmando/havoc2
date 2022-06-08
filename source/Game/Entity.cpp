#include "Entity.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CEntity, "CEntity");

void CEntity::Construct() {
	m_vPosition = { 0.0f, 0.0f, 0.0f };
	m_pFlipbook = std::make_shared<CFlipbook>();
}
