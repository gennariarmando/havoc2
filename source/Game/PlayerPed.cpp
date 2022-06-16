#include "PlayerPed.h"
#include "World.h"
#include "AInput.h"
#include "AConsole.h"

CPlayerPed::CPlayerPed() {

}

CPlayerPed::~CPlayerPed() {

}

void CPlayerPed::SetupPlayerPed() {
	std::shared_ptr<CPlayerPed> player = std::make_shared<CPlayerPed>();
	World.GetPlayer()->m_pPed = player;

	World.Add(player.get());

	// Set initial pos
	player->m_vPosition = glm::vec3(159.50f, 139.50f, 50.0f);
}

void CPlayerPed::Update() {
	bool keyForward = Input.GetKeyDown(KEY_W);
	bool keyBackward = Input.GetKeyDown(KEY_S);
	bool keyLeft = Input.GetKeyDown(KEY_A);
	bool keyRight = Input.GetKeyDown(KEY_D);

	m_fHeadingVelocity = 0.0f;

	m_fMoveSpeed = 2.0f;
	m_fRotSpeed = 3.0f;

	float forward = 0.0f;
	if (keyForward)
		forward = m_fMoveSpeed;
	else if (keyBackward)
		forward = -m_fMoveSpeed;

	if (abs(forward) > 0.0f) {
		m_vVelocity.x = m_vFront.x * forward;
		m_vVelocity.y = m_vFront.x * forward;
	}

	if (keyLeft)
		m_fHeadingVelocity = -1.0f;
	else if (keyRight)
		m_fHeadingVelocity = 1.0f;

	CPed::Update();
}