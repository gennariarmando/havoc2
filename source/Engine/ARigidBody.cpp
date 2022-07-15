#include "ARigidBody.h"

ARigidBody::ARigidBody() {
	physics::Transform t = physics::Transform::identity();
	m_pBody = Physics.m_pPhysicsWorld->createRigidBody(t);
}

ARigidBody::~ARigidBody() {
	Physics.m_pPhysicsWorld->destroyRigidBody(m_pBody);
}

glm::vec3 const& ARigidBody::GetPosition() {
	physics::Vector3 pos = m_pBody->getTransform().getPosition();
	return { pos.x, pos.y, pos.z };
}

glm::vec3 const& ARigidBody::GetRotation() {
	physics::Vector3 rot = m_pBody->getTransform().getOrientation().getVectorV();
	return { rot.x, rot.y, rot.z };
}

void ARigidBody::SetPosition(glm::vec3 const& pos) {
	physics::Transform t = m_pBody->getTransform();
	t.setPosition({ pos.x, pos.y, pos.z });
	m_pBody->setTransform(t);
}

void ARigidBody::SetRotation(glm::vec3 const& rot) {
	physics::Transform t = m_pBody->getTransform();
	physics::Quaternion q;
	q.fromEulerAngles({ rot.x, rot.y, rot.z });
	t.setOrientation(q);
	m_pBody->setTransform(t);
}
