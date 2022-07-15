#include "World.h"
#include "AGraphicDevice.h"
#include "ACamera.h"

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
	m_pPlayer = new CPlayerInfo();

	m_pMap = new CMap();
	m_pStyle = new CStyle();
}

CWorld::~CWorld() {
	if (m_pMap)
		delete m_pMap;

	if (m_pStyle)
		delete m_pStyle;
}

void CWorld::Add(CEntity* e) {
	m_vEntities.push_back(e);
}

void CWorld::Remove(CEntity* e) {
	m_vEntities.remove(e);
}

bool CWorld::InitMap(glm::uint8 level) {
	m_pMap->Load(levelList.at(level).gmp);
	return true;
}

bool CWorld::InitStyle(glm::uint8 level) {
	m_pStyle->Load(levelList.at(level).sty);
	return true;
}

bool CWorld::InitPlayer() {
	CPlayerPed::SetupPlayerPed();

	Camera.GetMode() = MODE_FOLLOWENTITY;
	Camera.SetTargetEntity(m_pPlayer->m_pPed);

	return true;
}

void CWorld::DestroyAllEntities() {
	for (auto& it : m_vEntities) {
		delete it;
	}
}
