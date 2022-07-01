#include "World.h"
#include "AGraphicDevice.h"
#include "ACamera.h"
#include "Player.h"

CWorld World;

std::vector<tLevelList> levelList = {
	{ "data/wil.gmp", "data/wil.sty" },
	{ "data/ste.gmp", "data/ste.sty" },
	{ "data/bil.gmp", "data/bil.sty" },
	{ "data/wil-multi.gmp", "data/wil.sty" },
	{ "data/ste-multi.gmp", "data/ste.sty" },
	{ "data/bil-multi.gmp", "data/bil.sty" },
	{ "data/MP1-comp.gmp", "data/bil.sty" },
	{ "data/MP2-comp.gmp", "data/wil.sty" },
	{ "data/MP5-comp.gmp", "data/ste.sty" },
	{ "data/lorne2e.gmp", "data/wil.sty" },
	{ "data/lorne2m.gmp", "data/wil.sty" },
	{ "data/lorne2h.gmp", "data/wil.sty" },
	{ "data/mike1e.gmp", "data/ste.sty" },
	{ "data/mike1m.gmp", "data/ste.sty" },
	{ "data/mike1h.gmp", "data/ste.sty" },
	{ "data/mike2e.gmp", "data/bil.sty" },
	{ "data/mike2m.gmp", "data/bil.sty" },
	{ "data/mike2h.gmp", "data/bil.sty" },
};

CWorld::CWorld() {
	m_vEntities = {};
	m_pPlayer = std::make_shared<CPlayerInfo>();

	m_pMap = nullptr;
	m_pStyle = nullptr;
	m_fGravity = DEFAULT_GRAVITY_VALUE;

	m_pPhysicsWorld = std::make_shared<physics::PhysicsWorld>(m_PhysicsCommon.createPhysicsWorld());


}

void CWorld::Add(CEntity* e) {
	m_vEntities.push_back(e);
}

void CWorld::Remove(CEntity* e) {
	m_vEntities.remove(e);
}

bool CWorld::InitMap(glm::uint8 level) {
	if (m_pMap)
		m_pMap.reset();
	m_pMap = std::make_shared<CMap>(levelList.at(level).gmp);


	return true;
}

bool CWorld::InitStyle(glm::uint8 level) {
	if (m_pStyle)
		m_pStyle.reset();
	m_pStyle = std::make_shared<CStyle>(levelList.at(level).sty);

	return true;
}

bool CWorld::InitPlayer() {
	CPlayerPed::SetupPlayerPed();

	Camera.GetMode() = MODE_FOLLOWENTITY;
	Camera.SetTargetEntity(m_pPlayer->m_pPed.get());

	return true;
}
