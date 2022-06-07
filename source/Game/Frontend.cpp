#include "Frontend.h"
#include "Font.h"
#include "Screen.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CFrontend, "CFrontend");

CFrontend Frontend;

void CFrontend::Init() {
	m_pStyle = std::make_shared<CStyle>("GTA2/data/fstyle.sty");
	Font.Init();

}

void CFrontend::Update() {

}

void CFrontend::Draw2D() {

}

void CFrontend::Shutdown() {

}
