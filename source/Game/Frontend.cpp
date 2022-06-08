#include "Frontend.h"
#include "Font.h"
#include "Screen.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CFrontend, "CFrontend");

CFrontend Frontend;

std::vector<std::string> frontendTexFileNames = {
	"1",
	"1_Options",
	"1_Play",
	"1_Quit",
	"2",
	"2_Bonus1",
	"2_Bonus2",
	"2_Bonus3",
	"2_League",
	"2_Level1",
	"2_Level2",
	"2_Level3",
	"2_Name",
	"2_Restart",
	"3",
	"3_Tables",
	"Credits",
	"DemoInfo",
	"GameComplete",
	"LevelComplete",
	"Mask",
	"Mask2",
	"Mask3",
	"MPLose",
	"PlayerDead",
};

void CFrontend::Construct() {
	m_pStyle = NULL;
	m_pFrontendSprites = { };
}

void CFrontend::Init() {
	m_pStyle = std::make_shared<CStyle>("GTA2/data/fstyle.sty");
	Font.Init();

	for (auto& it : frontendTexFileNames) {
		std::shared_ptr<CSprite2D> sprite = std::make_shared<CSprite2D>("GTA2/data/frontend", it);
		m_pFrontendSprites.push_back(sprite);
	}
}

void CFrontend::Update() {

}

void CFrontend::Draw2D() {
	//m_pFrontendSprites.at(FE_1)->Draw(HUD_X(278.0f), HUD_Y(0.0f), HUD_W(362.0f), HUD_H(480.0f), glm::vec4(1.0f));
	//m_pFrontendSprites.at(FE_1_PLAY)->Draw(HUD_X(0.0f), HUD_Y(0.0f), HUD_W(278.0f), HUD_H(480.0f), glm::vec4(1.0f));
}

void CFrontend::Shutdown() {

}
