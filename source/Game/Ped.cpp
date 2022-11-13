#include "Ped.h"

CPed::CPed() {
	GetRigidBody()->SetType(BODYTYPE_DYNAMIC);
}

void CPed::Update() {
	CEntity::Update();

	// Teleport ped in case falls in void.
	glm::vec3 pos = GetRigidBody()->GetPosition();
	if (GetRigidBody()->GetPosition().z <= 1.0f) {
		GetRigidBody()->SetPosition({ pos.x, pos.y, 1.0f });
	}
}
