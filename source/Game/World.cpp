#include "World.h"
#include "ABaseGL.h"
#include "Camera.h"
#include "Common.h"
#include "Style.h"
#include "Map.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CWorld, "CWorld");

CWorld World;

CMap map;
CSprite2D sprite;

void CWorld::Construct() {
    world = NULL;

}

void CWorld::Init() {
    map.Read("GTA2/data/wil.gmp", "GTA2/data/wil.sty");
}

void CWorld::Update() {


}

void CWorld::LateUpdate() {


}

void CWorld::Render() {
    map.Render();
}

void CWorld::Draw2D() {
    //CSprite2D sprite;
    //
    //sprite.GetTexture()->GetID() = map.sty.GetSprite().at(0)->GetID();
    //sprite.Draw(0.0f, 0.0f, 128, 128, glm::vec4(1.0f));
}

void CWorld::Draw2DDebug() {

}

void CWorld::Destruct() {

}

void CWorld::Shutdown() {

}
