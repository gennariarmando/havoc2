#include "ARigidBody.h"

ARigidBody::ARigidBody() {
	m_pBody = Physics.m_pPhysicsWorld->createRigidBody({});
	SetPosition({ 0.0f, 0.0f, 0.0f });
	SetRotation({ 0.0f, 0.0f, 0.0f });
	m_pBody->setAngularLockAxisFactor({ 0.0f, 0.0f, 1.0f });
}

ARigidBody::~ARigidBody() {
	Physics.m_pPhysicsWorld->destroyRigidBody(m_pBody);
}

const glm::vec3 ARigidBody::GetPosition() {
	rp3d::Vector3 pos = m_pBody->getTransform().getPosition();
	return { pos.x, pos.y, pos.z };
}

const glm::vec3 ARigidBody::GetRotation() {
	rp3d::Quaternion q = m_pBody->getTransform().getOrientation();
	glm::vec3 axis = {};

	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	axis.x = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		axis.y = std::copysign(glm::pi<float>() / 2, sinp);
	else
		axis.y = std::asin(sinp);

	float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	axis.z = std::atan2(siny_cosp, cosy_cosp);

	return { axis.x * 2.0f, axis.y * 2.0f, axis.z * 2.0f };
}

const float ARigidBody::GetAngle() {
	float angle = 0.0f;
	rp3d::Vector3 axis = { 0.0f, 0.0f, 0.0f };
	m_pBody->getTransform().getOrientation().getRotationAngleAxis(angle, axis);

	return angle;
}

const glm::vec3 ARigidBody::GetLinearVelocity() {
	rp3d::Vector3 vel = m_pBody->getLinearVelocity();
	return { vel.x, vel.y, vel.z };
}

const glm::vec3 ARigidBody::GetAngularVelocity() {
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

void ARigidBody::SetRotation(glm::quat const& rot) {
	rp3d::Transform t = m_pBody->getTransform();
	rp3d::Quaternion q = { rot.x, rot.y, rot.z, rot.w };

	t.setOrientation(q);
	m_pBody->setTransform(t);
}

void ARigidBody::SetHeading(float angle) {
	SetRotation({ 0.0f, 0.0f, angle });
}

void ARigidBody::SetType(glm::uint8 type) {
	m_pBody->setType(static_cast<rp3d::BodyType>(type));
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

void ARigidBody::AddCollisionTypeConvex(glm::vec3 const& pos, std::vector<glm::vec3> const& v, std::vector<glm::int32> const& i) {
	rp3d::TriangleVertexArray* triangleArray =
		new rp3d::TriangleVertexArray(v.size(), v.data(), 3 * sizeof(float), 6,
			i.data(), 3 * sizeof(int),
			rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	rp3d::TriangleMesh* triangleMesh = Physics.m_pPhysicsCommon->createTriangleMesh();

	// Add the triangle vertex array to the triangle mesh 
	triangleMesh->addSubpart(triangleArray);

	// Create the concave mesh shape 
	rp3d::ConcaveMeshShape* concaveMesh = Physics.m_pPhysicsCommon->createConcaveMeshShape(triangleMesh);

	rp3d::Transform t = rp3d::Transform::identity();
	t.setPosition({ pos.x, pos.y, pos.z });

	m_pBody->addCollider(concaveMesh, t);
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
