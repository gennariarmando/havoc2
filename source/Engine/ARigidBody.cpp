#include "ARigidBody.h"

ARigidBody::ARigidBody() {
	m_vPos = {};
	m_fHeading = 0.0f;
	m_vVelocity = { 0.0f, 0.0f, 0.0f };
	m_nBodyType = BODYTYPE_STATIC;
	m_bEnabled = true;
}

ARigidBody::~ARigidBody() {

}

const glm::vec3 ARigidBody::GetLinearVelocity() {
	return m_vVelocity;
}

const glm::vec3 ARigidBody::GetPosition() {
	return m_vPos;
}

const float ARigidBody::GetHeading() {
	float a = m_fHeading;
	a = ConstrainAngle(a);
	return a;
}

void ARigidBody::SetPosition(glm::vec3 const& pos) {
	m_vPos = pos;
}

void ARigidBody::SetHeading(float angle) {
	m_fHeading = angle;
}

void ARigidBody::SetType(glm::uint8 type) {
	m_nBodyType = type;
}

glm::uint8 ARigidBody::GetType() {
	return m_nBodyType;
}

void ARigidBody::ApplyLinearVelocity(glm::vec3 const& vel) {
	glm::vec3 previousVelocity = GetLinearVelocity();
	glm::vec3 newVelocity = previousVelocity + vel;

	SetLinearVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
}

void ARigidBody::SetLinearVelocity(glm::vec3 const& vel) {
	m_vVelocity = vel;
}

void ARigidBody::ClearVelocity() {
	SetLinearVelocity({ 0.0f, 0.0f, 0.0f });
}

void ARigidBody::SetEnabled(bool on) {
	m_bEnabled = on;
}

bool const& ARigidBody::IsEnabled() {
	return m_bEnabled;
}

