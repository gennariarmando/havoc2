#include "PlayerPed.h"
#include "World.h"
#include "AInput.h"
#include "AConsole.h"
#include "ATime.h"

CPlayerPed::CPlayerPed() {

}

CPlayerPed::~CPlayerPed() {

}

void CPlayerPed::SetupPlayerPed() {
	CPlayerPed* playerPed = new CPlayerPed();
	World.GetPlayer()->m_pPed = playerPed;

	World.Add(playerPed);

	// Set initial pos
	playerPed->GetRigidBody()->SetPosition({ 159.50f, 139.50f, 50.0f });
	playerPed->GetRigidBody()->SetHeading(glm::radians<float>(0.0f));
}

void CPlayerPed::Update() {
	bool keyForward = Input.GetKeyDown(KEY_W);
	bool keyBackward = Input.GetKeyDown(KEY_S);
	bool keyLeft = Input.GetKeyDown(KEY_A);
	bool keyRight = Input.GetKeyDown(KEY_D);
	bool keyJump = Input.GetKeyJustDown(KEY_SPACE);
	float moveSpeed = 2.0f;

	float forward = 0.0f;
	if (keyForward)
		forward = moveSpeed;
	else if (keyBackward)
		forward = -moveSpeed;

	glm::vec3 direction = m_vFront * forward;
	direction.z = GetRigidBody()->GetLinearVelocity().z;
	GetRigidBody()->SetLinearVelocity(direction);

	float heading = 0.0f;
	if (keyRight) {
		if (GetRigidBody()->GetAngle() > glm::radians<float>(359.0f))
			GetRigidBody()->SetHeading(glm::radians<float>(0.0f));

		heading -= moveSpeed;
	}
	else if (keyLeft) {
		if (GetRigidBody()->GetAngle() < glm::radians<float>(1.0f))
			GetRigidBody()->SetHeading(glm::radians<float>(360.0f));

		heading += moveSpeed;
	}

	GetRigidBody()->SetAngularVelocity({ 0.0f, 0.0f, heading });

	CPed::Update();
}
