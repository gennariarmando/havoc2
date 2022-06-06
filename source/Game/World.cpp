#include "World.h"
#include "ABaseGL.h"
#include "Camera.h"
#include "Common.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CWorld, "CWorld");

CWorld World;
CSprite2D sprite;

void CWorld::Construct() {
    b2World = NULL;
    map = NULL;
}

void CWorld::Init() {
    map = std::make_unique<CMap>("GTA2/data/wil.gmp", "GTA2/data/wil.sty");

    //sprite.GetTexture()->GetID() = map->GetStyle().GetTexture().at(TEXTUREID_JESUSSAVE_START)->GetID();
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
    //sprite.Draw(glm::vec4(0.0f, 0.0f, 32.0f, 32.0f), glm::vec4(1.0f));
}

void CWorld::Draw2DDebug() {

}

void CWorld::Destruct() {

}

void CWorld::Shutdown() {

}
