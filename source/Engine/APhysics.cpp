#include "APhysics.h"
#include "ATime.h"
#include "World.h"

APhysics Physics;

bool APhysics::Init() {
	m_pPhysicsCommon = new physics::PhysicsCommon();
	m_pPhysicsWorld = m_pPhysicsCommon->createPhysicsWorld();

	if (!m_pPhysicsWorld)
		return false;

	m_pPhysicsWorld->setGravity({ 0.0f, 0.0f, -DEFAULT_GRAVITY_VALUE });

	return true;
}

void APhysics::Shutdown() {
	World.DestroyAllEntities();
	m_pPhysicsCommon->destroyPhysicsWorld(m_pPhysicsWorld);
	delete m_pPhysicsCommon;
}

void APhysics::BeginFrame() {
	m_pPhysicsWorld->update(1.0f / 60.0f);
}

void APhysics::EndFrame() {

}
