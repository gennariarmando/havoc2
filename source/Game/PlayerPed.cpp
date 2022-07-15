#include "PlayerPed.h"
#include "World.h"
#include "AInput.h"
#include "AConsole.h"

CPlayerPed::CPlayerPed() {

}

CPlayerPed::~CPlayerPed() {

}

void CPlayerPed::SetupPlayerPed() {
	CPlayerPed* playerPed = new CPlayerPed();
	World.GetPlayer()->m_pPed = playerPed;

	World.Add(playerPed);

	// Set initial pos
	playerPed->GetRigidBody()->SetPosition({ 10.0f, 10.0f, 10.0f });
}

void CPlayerPed::Update() {
	bool keyForward = Input.GetKeyDown(KEY_W);
	bool keyBackward = Input.GetKeyDown(KEY_S);
	bool keyLeft = Input.GetKeyDown(KEY_A);
	bool keyRight = Input.GetKeyDown(KEY_D);
	bool keyJump = Input.GetKeyJustDown(KEY_SPACE);

	//m_fHeadingVelocity = 0.0f;
	//
	//m_fMoveSpeed = 2.0f;
	//m_fRotSpeed = 3.0f;
	//
	//float forward = 0.0f;
	//if (keyForward)
	//	forward = m_fMoveSpeed;
	//else if (keyBackward)
	//	forward = -m_fMoveSpeed;
	//
	//m_vVelocity.x = m_vFront.x * forward;
	//m_vVelocity.y = m_vFront.y * forward;
	//
	//if (keyLeft)
	//	m_fHeadingVelocity = -1.0f;
	//else if (keyRight)
	//	m_fHeadingVelocity = 1.0f;
	//
	//if (keyJump)
	//	m_vVelocity.z = 5.0f;

	CPed::Update();
}