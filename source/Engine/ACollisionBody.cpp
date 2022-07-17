#include "ACollisionBody.h"

ACollisionBody::ACollisionBody() {
	m_pBody = Physics.m_pPhysicsWorld->createCollisionBody({});
}

ACollisionBody::~ACollisionBody() {
	Physics.m_pPhysicsWorld->destroyCollisionBody(m_pBody);
}

glm::vec3 const& ACollisionBody::GetPosition() {
	physics::Vector3 pos = m_pBody->getTransform().getPosition();
	return { pos.x, pos.y, pos.z };
}

glm::vec3 const& ACollisionBody::GetRotation() {
	physics::Vector3 rot = m_pBody->getTransform().getOrientation().getVectorV();
	return { rot.x, rot.y, rot.z };
}

void ACollisionBody::SetPosition(glm::vec3 const& pos) {
	physics::Transform t = m_pBody->getTransform();
	t.setPosition({ pos.x, pos.y, pos.z });
	m_pBody->setTransform(t);
}

void ACollisionBody::SetRotation(glm::vec3 const& rot) {
	physics::Transform t = m_pBody->getTransform();
	physics::Quaternion q;
	q.fromEulerAngles({ rot.x, rot.y, rot.z });
	t.setOrientation(q);
	m_pBody->setTransform(t);
}
