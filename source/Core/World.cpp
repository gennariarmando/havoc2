#include "World.h"
#include "ABaseDevice.h"
#include "Camera.h"
#include "Common.h"
#include "Entity.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CWorld, "CWorld");

CWorld World;

void CWorld::Construct() {
    m_pMap = std::make_unique<CMap>();
    m_fGravity = 9.81f;
}

void CWorld::Init() {
    LoadLevel(LEVEL_DOWNTOWN_SP);

    //std::shared_ptr<CEntity> player = std::make_shared<CEntity>();
}

void CWorld::Update() {
   
}

void CWorld::LateUpdate() {

}

void CWorld::Render() {
    if (!m_pMap)
        return;

    m_pMap->Render();
}

void CWorld::Destruct() {

}

void CWorld::Shutdown() {

}

void CWorld::LoadLevel(glm::uint8 area) {
    m_pMap.release();

    switch (area) {
    case LEVEL_DOWNTOWN_SP:
        m_pMap = std::make_unique<CMap>("GTA2/data/wil.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_RESIDENT_SP:
        m_pMap = std::make_unique<CMap>("GTA2/data/ste.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_INDUST_SP:
        m_pMap = std::make_unique<CMap>("GTA2/data/bil.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_DOWNTOWN_MP:
        m_pMap = std::make_unique<CMap>("GTA2/data/wil-multi.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_RESIDENT_MP:
        m_pMap = std::make_unique<CMap>("GTA2/data/ste-multi.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_INDUST_MP:
        m_pMap = std::make_unique<CMap>("GTA2/data/bil-multi.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_MP1:
        m_pMap = std::make_unique<CMap>("GTA2/data/MP1-comp.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_MP2:
        m_pMap = std::make_unique<CMap>("GTA2/data/MP2-comp.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_MP3:
        m_pMap = std::make_unique<CMap>("GTA2/data/MP5-comp.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_A:
        m_pMap = std::make_unique<CMap>("GTA2/data/lorne2e.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_B:
        m_pMap = std::make_unique<CMap>("GTA2/data/lorne2m.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_C:
        m_pMap = std::make_unique<CMap>("GTA2/data/lorne2h.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_D:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike1e.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_E:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike1m.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_F:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike1h.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_G:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike2e.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_BONUS_H:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike2m.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_BONUS_I:
        m_pMap = std::make_unique<CMap>("GTA2/data/mike2h.gmp", "GTA2/data/bil.sty");
        break;
    }
}
