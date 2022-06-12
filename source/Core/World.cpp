#include "World.h"
#include "ABaseDevice.h"
#include "ABaseEngine.h"
#include "Camera.h"
#include "Common.h"
#include "Entity.h"
#include "Frontend.h"
#include "LoadingScreen.h"

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
    m_pMap = std::make_shared<CMap>();
    m_pStyle = std::make_shared<CStyle>();

    m_fGravity = 9.81f;
}

void CWorld::BeginPlay() {

}

void CWorld::Update() {
    if (Frontend.m_bMenuActive)
        return;

    if (!m_pMap || !m_pStyle)
        return;

    m_pMap->BuildEverything();
    m_pStyle->BuildEverything();

    if (!Frontend.IsLoading())
        Frontend.CloseMenu();
}

void CWorld::LateUpdate() {

}

void CWorld::Render() {
    if (!m_pMap || !m_pStyle)
        return;

    m_pMap->Render(m_pStyle);
}

void CWorld::EndPlay() {

}

void CWorld::LoadLevel(glm::uint8 area) {
    m_pMap.reset();
    m_pMap = std::make_shared<CMap>(levelList.at(area).gmp); 

    m_pStyle.reset();
    m_pStyle = std::make_shared<CStyle>(levelList.at(area).sty);
}
