#include "World.h"
#include "ABaseGL.h"
#include "Camera.h"
#include "Common.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CWorld, "CWorld");

CWorld World;

void CWorld::Construct() {
    b2World = NULL;
    map = NULL;
}

void CWorld::Init() {
    map = std::make_unique<CMap>("GTA2/data/wil.gmp", "GTA2/data/wil.sty");
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
