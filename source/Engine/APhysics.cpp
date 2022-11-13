#include "APhysics.h"
#include "ATime.h"
#include "World.h"

APhysics Physics;

APhysics::APhysics() {

}

APhysics::~APhysics() {

}

bool APhysics::Init() {
	return true;
}

void APhysics::Shutdown() {

}

void APhysics::BeginFrame() {
	for (auto& it : World.GetEntityList()) {
		if (it->GetRigidBody()->GetType() == BODYTYPE_DYNAMIC) {
			// Apply gravity
			it->m_pRigidBody->m_vPos.z -= DEFAULT_GRAVITY_VALUE * Time.GetDeltaTime();
		
			it->m_pRigidBody->m_vPos += it->m_pRigidBody->m_vVelocity * Time.GetDeltaTime();
			it->m_pRigidBody->m_vVelocity = { 0.0f, 0.0f, 0.0f };
		}
	}
}

void APhysics::EndFrame() {

}
