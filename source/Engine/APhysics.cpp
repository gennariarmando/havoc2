#include "APhysics.h"
#include "ATime.h"
#include "World.h"

APhysics Physics;

bool APhysics::Init() {
	m_pPhysicsCommon = new rp3d::PhysicsCommon();
	m_pPhysicsWorld = m_pPhysicsCommon->createPhysicsWorld();

	if (!m_pPhysicsWorld)
		return false;

	m_pPhysicsWorld->setGravity({ 0.0f, 0.0f, 0.0f });

	return true;
}

void APhysics::Shutdown() {
	World.DestroyAllEntities();
	m_pPhysicsCommon->destroyPhysicsWorld(m_pPhysicsWorld);
}

void APhysics::BeginFrame() {
	if (!m_pPhysicsWorld)
		return;

	m_pPhysicsWorld->update(Time.GetDeltaTime());
}

void APhysics::EndFrame() {
	if (!m_pPhysicsWorld)
		return;

}
