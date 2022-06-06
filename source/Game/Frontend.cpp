#include "Frontend.h"
#include "Font.h"
#include "Screen.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CFrontend, "CFrontend");

CFrontend Frontend;

void CFrontend::Init() {
	m_pStyle = std::make_shared<CStyle>("GTA2/data/wil.sty");
	Font.Init(m_pStyle);

}

void CFrontend::Update() {

}

void CFrontend::Draw2D() {
	Font.SetFontStyle(FONT_SMALL_MESSAGES);
	Font.SetFontAlignment(FONT_ALIGN_RIGHT);
	Font.SetScale({HUD_W(1.4f), HUD_H(1.4f)});
	Font.PrintString({ HUD_RIGHT(0.0f), HUD_Y(0.0f)}, "$0");
}

void CFrontend::Shutdown() {

}
