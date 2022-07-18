#include "Ped.h"

CPed::CPed() {
	GetRigidBody()->SetType(BODYTYPE_DYNAMIC);
	GetRigidBody()->AddCollisionTypeSphere({ 0.0f, 0.0f, 0.0f }, 1.0f);
}

void CPed::Update() {
	CEntity::Update();

	// Teleport ped in case falls in void.
	glm::vec3 pos = GetRigidBody()->GetPosition();
	if (GetRigidBody()->GetPosition().z < -50.0f) {
		GetRigidBody()->SetPosition({ pos.x, pos.y, 50.0f });
		GetRigidBody()->ClearVelocity();
	}
}
