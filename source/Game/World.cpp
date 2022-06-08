#include "World.h"
#include "ABaseGL.h"
#include "Camera.h"
#include "Common.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CWorld, "CWorld");

CWorld World;

void CWorld::Construct() {
    map = std::make_unique<CMap>();
}

void CWorld::Init() {
    LoadLevel(LEVEL_DOWNTOWN_SP);
}

void CWorld::Update() {

}

void CWorld::LateUpdate() {

}

void CWorld::Render() {
    if (!map)
        return;

    map->Render();
}

void CWorld::Draw2D() {

}

void CWorld::Draw2DDebug() {

}

void CWorld::Destruct() {

}

void CWorld::Shutdown() {

}

void CWorld::LoadLevel(glm::uint8 area) {
    map.release();

    switch (area) {
    case LEVEL_DOWNTOWN_SP:
        map = std::make_unique<CMap>("GTA2/data/wil.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_RESIDENT_SP:
        map = std::make_unique<CMap>("GTA2/data/ste.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_INDUST_SP:
        map = std::make_unique<CMap>("GTA2/data/bil.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_DOWNTOWN_MP:
        map = std::make_unique<CMap>("GTA2/data/wil-multi.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_RESIDENT_MP:
        map = std::make_unique<CMap>("GTA2/data/ste-multi.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_INDUST_MP:
        map = std::make_unique<CMap>("GTA2/data/bil-multi.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_MP1:
        map = std::make_unique<CMap>("GTA2/data/MP1-comp.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_MP2:
        map = std::make_unique<CMap>("GTA2/data/MP2-comp.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_MP3:
        map = std::make_unique<CMap>("GTA2/data/MP5-comp.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_A:
        map = std::make_unique<CMap>("GTA2/data/lorne2e.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_B:
        map = std::make_unique<CMap>("GTA2/data/lorne2m.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_C:
        map = std::make_unique<CMap>("GTA2/data/lorne2h.gmp", "GTA2/data/wil.sty");
        break;
    case LEVEL_BONUS_D:
        map = std::make_unique<CMap>("GTA2/data/mike1e.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_E:
        map = std::make_unique<CMap>("GTA2/data/mike1m.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_F:
        map = std::make_unique<CMap>("GTA2/data/mike1h.gmp", "GTA2/data/ste.sty");
        break;
    case LEVEL_BONUS_G:
        map = std::make_unique<CMap>("GTA2/data/mike2e.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_BONUS_H:
        map = std::make_unique<CMap>("GTA2/data/mike2m.gmp", "GTA2/data/bil.sty");
        break;
    case LEVEL_BONUS_I:
        map = std::make_unique<CMap>("GTA2/data/mike2h.gmp", "GTA2/data/bil.sty");
        break;
    }
}
