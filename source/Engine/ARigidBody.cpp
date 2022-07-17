#include "ARigidBody.h"

ARigidBody::ARigidBody() {
	m_pBody = Physics.m_pPhysicsWorld->createRigidBody({});
	m_pBody->setAngularLockAxisFactor({ 0.0f, 0.0f, 1.0f });
}

ARigidBody::~ARigidBody() {
	Physics.m_pPhysicsWorld->destroyRigidBody(m_pBody);
}

glm::vec3 const& ARigidBody::GetPosition() {
	phy::Vector3 pos = m_pBody->getTransform().getPosition();
	return { pos.x, pos.y, pos.z };
}

glm::vec3 const& ARigidBody::GetRotation() {
	phy::Vector3 rot = m_pBody->getTransform().getOrientation().getVectorV();
	return { rot.x, rot.y, rot.z };
}

float const& ARigidBody::GetHeading() {
	return GetRotation().z;
}

void ARigidBody::SetPosition(glm::vec3 const& pos) {
	phy::Transform t = m_pBody->getTransform();
	t.setPosition({ pos.x, pos.y, pos.z });
	m_pBody->setTransform(t);
}

void ARigidBody::SetRotation(glm::vec3 const& rot) {
	phy::Transform t = m_pBody->getTransform();
	phy::Quaternion q;
	q = q.fromEulerAngles({ rot.x, rot.y, rot.z });
	t.setOrientation(q);
	m_pBody->setTransform(t);
}

void ARigidBody::SetHeading(float angle) {
	glm::vec3 rot = GetRotation();
	SetRotation({ rot.x, rot.y, angle });
}

void ARigidBody::SetType(glm::uint8 type) {
	m_pBody->setType(static_cast<phy::BodyType>(type));
}

glm::uint8 ARigidBody::GetType() {
	return static_cast<glm::uint8>(m_pBody->getType());
}

void ARigidBody::AddCollisionTypeBox(glm::vec3 const& pos, glm::vec3 const& scale) {
	phy::BoxShape* shape = Physics.m_pPhysicsCommon->createBoxShape({ scale.x, scale.y, scale.z });
	phy::Transform t = phy::Transform::identity();

	m_pBody->addCollider(shape, t);
}

void ARigidBody::AddCollisionTypeSphere(glm::vec3 const& pos, float const& radius) {
	phy::SphereShape* shape = Physics.m_pPhysicsCommon->createSphereShape(radius);
	phy::Transform t = phy::Transform::identity();

	m_pBody->addCollider(shape, t);
}

void ARigidBody::SetCollisionPosition(glm::uint32 index, glm::vec3 const& pos) {
	phy::Transform t = phy::Transform::identity();
	t.setPosition({ pos.x, pos.y, pos.z });

	m_pBody->getCollider(index)->setLocalToBodyTransform(t);
}

void ARigidBody::RemoveCollision(glm::uint32 index) {
	m_pBody->removeCollider(m_pBody->getCollider(index));
}

void ARigidBody::SetLinearVelocity(glm::vec3 const& vel) {
	m_pBody->setLinearVelocity({ vel.x, vel.y, vel.z });
}

void ARigidBody::SetAngularVelocity(glm::vec3 const& vel) {
	m_pBody->setAngularVelocity({ vel.x, vel.y, vel.z });
}

void ARigidBody::ClearVelocity() {
	SetLinearVelocity({ 0.0f, 0.0f, 0.0f });
	SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
}

void ARigidBody::SetEnabled(bool on) {
	m_pBody->setIsActive(on);
}

bool const& ARigidBody::IsEnabled() {
	return m_pBody->isActive();
}
