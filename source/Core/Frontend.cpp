#include "Frontend.h"
#include "Font.h"
#include "Screen.h"
#include "ABaseDevice.h"
#include "Common.h"
#include "World.h"
#include "Hud.h"
#include "ABaseEngine.h"
#include "LoadingScreen.h"

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

CFrontend::CFrontend() {
	m_pStyle = NULL;
	m_pFrontendSprites = { };
	m_bMenuActive = true;
	m_nCurrentPage = MENUPAGE_MAIN;
	m_nCurrentItem = 0;
	m_vMenuPages.resize(NUM_MENUPAGES);
	m_fItemColorPulse = 1.0f;
	m_vPreviousPages = {};
}

void CFrontend::BeginPlay() {
	m_pStyle = std::make_shared<CStyle>("data/fstyle.sty");
	m_pStyle->BuildEverything();

	for (auto& it : frontendTexFileNames) {
		std::shared_ptr<CSprite2D> sprite = std::make_shared<CSprite2D>();
		sprite->SetTexture("data/frontend", it);
		m_pFrontendSprites.push_back(sprite);
	}

	// MENUPAGE_MAIN
	if (tMenuPage* page = AddPage(MENUPAGE_MAIN, { { FE_1_PLAY, FE_1_OPTIONS, FE_1_QUIT }, { } } )) {
		AddItem(page, { MENUACTION_STARTGAME, "PLAY", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "OPTIONS", MENUPAGE_OPTIONS });
		AddItem(page, { MENUACTION_CHANGEPAGE, "QUIT", MENUPAGE_CREDITS });
	}

	// MENUPAGE_PLAY
	if (tMenuPage* page = AddPage(MENUPAGE_PLAY, { { FE_2_NAME, FE_2_RESTART, FE_2_LEAGUE, FE_2_LEVEL1 }, { } } )) {
		AddItem(page, { MENUACTION_SETPLAYERNAME, "PLAYER NAME", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "RESUME SAVED STATUS", MENUPAGE_RESUMESAVEDSTATUS });
		AddItem(page, { MENUACTION_CHANGEPAGE, "VIEW HIGH SCORES", MENUPAGE_VIEWHIGHSCORES });
		AddItem(page, { MENUACTION_CHANGEPAGE, "START PLAY IN AREA", MENUPAGE_STARTPLAYINAREA });
	}

	// MENUPAGE_OPTIONS
	if (tMenuPage* page = AddPage(MENUPAGE_OPTIONS, { { FE_1_OPTIONS, FE_1_OPTIONS }, { } })) {
		AddItem(page, { MENUACTION_CHANGEPAGE, "VIDEO", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "AUDIO", MENUPAGE_NONE, });
	}

	// MENUPAGE_CREDITS
	if (tMenuPage* page = AddPage(MENUPAGE_CREDITS, { { FE_CREDITS }, { } })) {

	}
}

void CFrontend::Update() {
	if (!m_bMenuActive)
		return;

	if (GetCurrentPage()->menuItems.size() > 0) {
		bool up = BaseDevice->GetKeyJustDown(KEY_UP);
		bool down = BaseDevice->GetKeyJustDown(KEY_DOWN);
		bool left = BaseDevice->GetKeyJustDown(KEY_LEFT);
		bool right = BaseDevice->GetKeyJustDown(KEY_RIGHT);
		bool enter = BaseDevice->GetKeyJustDown(KEY_ENTER);
		bool back = BaseDevice->GetKeyJustDown(KEY_ESCAPE);

		if (up)
			m_nCurrentItem--;

		if (down)
			m_nCurrentItem++;

		m_nCurrentItem = ClampInverse(m_nCurrentItem, 0, GetCurrentPage()->menuItems.size() - 1);

		if (back)
			GoBack();

		if (enter || left || right) {
			switch (GetCurrentItem()->action) {
			case MENUACTION_CHANGEPAGE:
				if (enter)
					ChangeMenuPage(GetCurrentItem()->targetPage);
				break;
			case MENUACTION_STARTGAME:
				if (enter) {
					ChangeMenuPage(GetCurrentItem()->targetPage);
					DoStuffBeforeStartingGame();
				}
				return;
			default:
				break;
			}
		}

		static bool decrease = false;
		if (m_fItemColorPulse > 1.0f)
			decrease = true;
		else if (m_fItemColorPulse < 0.5f)
			decrease = false;

		if (decrease)
			m_fItemColorPulse -= 1.0f * DeviceObjects.m_fDeltaTime;
		else
			m_fItemColorPulse += 2.0f * DeviceObjects.m_fDeltaTime;
	}
}

void CFrontend::Draw2D() {
	if (!m_bMenuActive)
		return;

	DrawBackground();

	switch (m_nCurrentPage) {
	case MENUPAGE_CREDITS:
		DrawCredits();
		break;
	default:
		break;
	}

	if (m_vMenuPages.size() < 1 || GetCurrentPage()->menuItems.size() < 1)
		return;

	glm::uint32 i = 0;
	for (auto& item : GetCurrentPage()->menuItems) {
		Font.Reset();
		Font.SetFontAlignment(FONT_ALIGN_LEFT);
		Font.SetFontStyle(FONT_STYLE_MENU);
		Font.SetWrapX(SCREEN_SCALE_RIGHT(0.0f));
		Font.SetScale(SCREEN_SCALE_Y(11.0f));

		if (m_nCurrentItem == i)
			Font.SetColor(glm::vec4(ToVec4(static_cast<glm::uint8>(m_fItemColorPulse * 255), 16, 0, 255)));
		else
			Font.SetColor(glm::vec4(1.0f));

		float spacing = 20.0f * i;
		Font.PrintString({ SCREEN_SCALE_X(278.0f + 22.0f), SCREEN_SCALE_Y(240.0f + 11.0f + spacing) }, item.name);
		i++;
	}
}

void CFrontend::EndPlay() {
	for (auto& it : m_pFrontendSprites) {
		it->Delete();
	}
}

void CFrontend::OpenMenu(glm::int32 page) {
	m_bMenuActive = true;
	m_nCurrentPage = page;
}

void CFrontend::CloseMenu() {
	m_bMenuActive = false;
}

void CFrontend::DrawCredits() {
	BaseDevice->QuitGame();
}

void CFrontend::DrawBackground() {
	const glm::int8 background = GetCurrentPage()->background.at(m_nCurrentItem);
	bool fullBackground = false;
	glm::uint8 left = -1;
	glm::uint8 right = -1;

	switch (background) {
	case FE_1_PLAY:
	case FE_1_OPTIONS:
	case FE_1_QUIT:
		left = background;
		right = FE_1;
		break;
	case FE_2_BONUS1:
	case FE_2_BONUS2:
	case FE_2_BONUS3:
	case FE_2_LEAGUE:
	case FE_2_LEVEL1:
	case FE_2_LEVEL2:
	case FE_2_LEVEL3:
	case FE_2_NAME:
	case FE_2_RESTART:
		left = background;
		right = FE_2;
		break;
	case FE_3_TABLES:
	case FE_CREDITS:
	case FE_DEMOINFO:
	case FE_GAMECOMPLETE:
	case FE_LEVELCOMPLETE:
	case FE_MPLOSE:
	case FE_PLAYERDEAD:
		fullBackground = true;
		break;
	}

	if (fullBackground) {
		m_pFrontendSprites.at(background)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(640.0f), SCREEN_SCALE_H(480.0f), glm::vec4(1.0f));
	}
	else {
		m_pFrontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(278.0f), SCREEN_SCALE_H(480.0f), glm::vec4(1.0f));
		m_pFrontendSprites.at(right)->Draw(SCREEN_SCALE_X(278.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(362.0f), SCREEN_SCALE_H(480.0f), glm::vec4(1.0f));
	}
}

void CFrontend::ChangeMenuPage(glm::int32 page, glm::int32 item) {
	if (page == MENUPAGE_NONE || m_vMenuPages.size() < page)
		return;

	tPreviousPage t = {};
	t.page = m_nCurrentPage;
	t.item = m_nCurrentItem;
	m_vPreviousPages.push_back(t);
	m_nCurrentPage = page;
	m_nCurrentItem = item;
}

void CFrontend::GoBack() {
	if (m_vPreviousPages.empty())
		return;

	ChangeMenuPage(m_vPreviousPages.back().page, m_vPreviousPages.back().item);
	m_vPreviousPages.pop_back();
}

void CFrontend::DoStuffBeforeStartingGame() {
	CLoadingScreen* t;
	t = NewObject<CLoadingScreen>();

	auto a = []() { World.LoadLevel(LEVEL_DOWNTOWN_SP); };
	BaseEngine.AddFun(a);
	CloseMenu();
}

bool CFrontend::IsLoading() {
	return (BaseEngine.IsLoading() || World.m_pMap->IsLoading() || World.m_pStyle->IsLoading());
}

tMenuPage* CFrontend::AddPage(eMenuPages index, tMenuPage page) {
	m_vMenuPages.at(index) = page;
	return &m_vMenuPages.at(index);
}

void CFrontend::AddItem(tMenuPage* page, tMenuItem item) {
	page->menuItems.push_back(item);
}
