#include "PlayerPed.h"
#include "World.h"
#include "AInput.h"
#include "AConsole.h"
#include "ATime.h"
#include "ACamera.h"

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
	//playerPed->GetRigidBody()->SetPosition({ 20.50f, 10.50f, 10.0f });
	playerPed->GetRigidBody()->SetHeading(glm::radians<float>(0.0f));
}

void CPlayerPed::Update() {
	if (Camera.GetMode() != MODE_DEBUG3D) {
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

		float heading = GetRigidBody()->GetHeading();
		if (keyRight) {
			//if (GetRigidBody()->GetHeading() > glm::radians<float>(359.0f))
			//	GetRigidBody()->SetHeading(glm::radians<float>(0.0f));

			heading += moveSpeed * 1.0f * Time.GetDeltaTime();
		}
		else if (keyLeft) {
			//if (GetRigidBody()->GetHeading() < glm::radians<float>(1.0f))
			//	GetRigidBody()->SetHeading(glm::radians<float>(360.0f));

			heading -= moveSpeed * 1.0f * Time.GetDeltaTime();
		}

		GetRigidBody()->SetHeading(heading);
	}

	CPed::Update();
}
