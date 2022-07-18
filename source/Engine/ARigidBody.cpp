#include "ARigidBody.h"

ARigidBody::ARigidBody() {
	m_pBody = Physics.m_pPhysicsWorld->createRigidBody({});

	m_pBody->setAngularLockAxisFactor({ 0.0f, 0.0f, 1.0f });

}

ARigidBody::~ARigidBody() {
	Physics.m_pPhysicsWorld->destroyRigidBody(m_pBody);
}

glm::vec3 const& ARigidBody::GetPosition() {
	rp3d::Vector3 pos = m_pBody->getTransform().getPosition();
	return { pos.x, pos.y, pos.z };
}

glm::vec3 const& ARigidBody::GetRotation() {
	float angle = 0.0f;
	rp3d::Vector3 axis = { 0.0f, 0.0f, 0.0f };
	m_pBody->getTransform().getOrientation().getRotationAngleAxis(angle, axis);

	return { axis.x, axis.y, axis.z };
}

float const& ARigidBody::GetAngle() {
	float angle = 0.0f;
	rp3d::Vector3 axis = { 0.0f, 0.0f, 0.0f };
	m_pBody->getTransform().getOrientation().getRotationAngleAxis(angle, axis);

	return angle;
}

glm::vec3 const& ARigidBody::GetLinearVelocity() {
	rp3d::Vector3 vel = m_pBody->getLinearVelocity();
	return { vel.x, vel.y, vel.z };
}

glm::vec3 const& ARigidBody::GetAngularVelocity() {
	rp3d::Vector3 vel = m_pBody->getAngularVelocity();
	return { vel.x, vel.y, vel.z };
}

// Set world position
void ARigidBody::SetPosition(glm::vec3 const& pos) {
	rp3d::Transform t = m_pBody->getTransform();
	t.setPosition({ pos.x, pos.y, pos.z });
	m_pBody->setTransform(t);
}

// Set rotation axis in radians
void ARigidBody::SetRotation(glm::vec3 const& rot) {
	rp3d::Transform t = m_pBody->getTransform();
	rp3d::Quaternion q = rp3d::Quaternion::identity();
	q = rp3d::Quaternion::fromEulerAngles({ rot.x, rot.y, rot.z });
	t.setOrientation(q);
	m_pBody->setTransform(t);
}

void ARigidBody::SetHeading(float angle) {
	glm::vec3 rot = GetRotation();
	SetRotation({ rot.x, rot.y, angle });
}

void ARigidBody::SetType(glm::uint8 type) {
	m_pBody->setType(rp3d::BodyType::STATIC);
}

glm::uint8 ARigidBody::GetType() {
	return static_cast<glm::uint8>(m_pBody->getType());
}

void ARigidBody::AddCollisionTypeBox(glm::vec3 const& pos, glm::vec3 const& scale) {
	rp3d::BoxShape* boxShape = Physics.m_pPhysicsCommon->createBoxShape({ scale.x, scale.y, scale.z });
	rp3d::Transform t = rp3d::Transform::identity();
	t.setPosition({ pos.x, pos.y, pos.z });

	m_pBody->addCollider(boxShape, t);
}

void ARigidBody::AddCollisionTypeSphere(glm::vec3 const& pos, float const& radius) {
	rp3d::SphereShape* sphereShape = Physics.m_pPhysicsCommon->createSphereShape(radius);
	rp3d::Transform t = rp3d::Transform::identity();
	t.setPosition({ pos.x, pos.y, pos.z });

	m_pBody->addCollider(sphereShape, t);
}

void ARigidBody::SetCollisionPosition(glm::uint32 index, glm::vec3 const& pos) {
	rp3d::Transform t = rp3d::Transform::identity();
	t.setPosition({ pos.x, pos.y, pos.z });

	m_pBody->getCollider(index)->setLocalToBodyTransform(t);
}

void ARigidBody::RemoveCollision(glm::uint32 index) {
	m_pBody->removeCollider(m_pBody->getCollider(index));
}

void ARigidBody::ApplyLinearVelocity(glm::vec3 const& vel) {
	glm::vec3 previousVelocity = GetLinearVelocity();
	glm::vec3 newVelocity = previousVelocity + vel;

	SetLinearVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
}

void ARigidBody::ApplyAngularVelocity(glm::vec3 const& vel) {
	glm::vec3 previousVelocity = GetAngularVelocity();
	glm::vec3 newVelocity = previousVelocity + vel;

	SetAngularVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
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
