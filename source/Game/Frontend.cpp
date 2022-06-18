#include "Frontend.h"
#include "Font.h"
#include "AScreen.h"
#include "Common.h"
#include "Game.h"
#include "LoadingScreen.h"
#include "AInput.h"
#include "ATime.h"
#include "AEngine.h"
#include "AGraphicDevice.h"
#include "AConsole.h"

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
	Clear();
}
CFrontend::~CFrontend() {
	Clear();
}

bool CFrontend::Init() {
	if (m_bInitialized)
		return true;

	m_pStyle = std::make_shared<CStyle>("data/fstyle.sty");

	for (auto& it : frontendTexFileNames) {
		std::shared_ptr<ASprite> sprite = std::make_shared<ASprite>();
		if (!sprite->SetTexture("data/frontend", it))
			return false;
		m_pFrontendSprites.push_back(sprite);
	}

	m_vMenuPages.resize(NUM_MENUPAGES);
	// MENUPAGE_MAIN
	if (tMenuPage* page = AddPage(MENUPAGE_MAIN, { MENUPAGE_NONE, 0, { FE_1_PLAY, FE_1_OPTIONS, FE_1_QUIT }, { } } )) {
		AddItem(page, { MENUACTION_STARTGAME, "PLAY", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "OPTIONS", MENUPAGE_OPTIONS });
		AddItem(page, { MENUACTION_CHANGEPAGE, "QUIT", MENUPAGE_CREDITS });
	}

	// MENUPAGE_PLAY
	if (tMenuPage* page = AddPage(MENUPAGE_PLAY, { MENUPAGE_MAIN, 0, { FE_2_NAME, FE_2_RESTART, FE_2_LEAGUE, FE_2_LEVEL1 }, { } } )) {
		AddItem(page, { MENUACTION_SETPLAYERNAME, "PLAYER NAME", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "RESUME SAVED STATUS", MENUPAGE_RESUMESAVEDSTATUS });
		AddItem(page, { MENUACTION_CHANGEPAGE, "VIEW HIGH SCORES", MENUPAGE_VIEWHIGHSCORES });
		AddItem(page, { MENUACTION_CHANGEPAGE, "START PLAY IN AREA", MENUPAGE_STARTPLAYINAREA });
	}

	// MENUPAGE_OPTIONS
	if (tMenuPage* page = AddPage(MENUPAGE_OPTIONS, { MENUPAGE_MAIN, 1, { FE_1_OPTIONS, FE_1_OPTIONS }, { } })) {
		AddItem(page, { MENUACTION_CHANGEPAGE, "VIDEO", MENUPAGE_VIDEO });
	}

	// MENUPAGE_CREDITS
	if (tMenuPage* page = AddPage(MENUPAGE_CREDITS, { MENUPAGE_NONE, 0, { FE_CREDITS }, { } })) {

	}

	// MENUPAGE_VIDEO
	if (tMenuPage* page = AddPage(MENUPAGE_VIDEO, { MENUPAGE_OPTIONS, 0, { FE_1_OPTIONS }, { } })) {
		AddItem(page, { MENUACTION_SCREENRES, "RESOLUTION", MENUPAGE_VIDEO });

	}

	m_bInitialized = true;
	return true;
}

void CFrontend::Update() {
	if (!m_bInitialized || !m_bMenuActive)
		return;

	bool up = Input.GetKeyJustDown(KEY_UP);
	bool down = Input.GetKeyJustDown(KEY_DOWN);
	bool left = Input.GetKeyJustDown(KEY_LEFT);
	bool right = Input.GetKeyJustDown(KEY_RIGHT);
	bool enter = Input.GetKeyJustDown(KEY_ENTER);
	bool back = Input.GetKeyJustDown(KEY_ESCAPE);

	if (abs(Input.m_NewMouse.delta.x) > 0.0f || abs(Input.m_NewMouse.delta.y) > 0.0f) {
		GraphicDevice.SetCursorOnOff(true);
		m_bDrawMouse = true;
	}
	else if (up || down || left || right || enter || back) {
		GraphicDevice.SetCursorOnOff(false);
		m_bDrawMouse = false;
	}

	if (GetCurrentPage()->menuItems.size() > 0) {
		if (up)
			m_nCurrentItem--;

		if (down)
			m_nCurrentItem++;

		m_nCurrentItem = ClampInverse(m_nCurrentItem, 0, GetCurrentPage()->menuItems.size() - 1);

		if (back)
			GoBack();

		if (enter || left || right)
			ProcessMenuOptions(enter, left ? -1 : right ? 1 : 0);

		if (m_fItemColorPulse >= 1.0f)
			m_bItemColorPulseSwap = true;
		else if (m_fItemColorPulse <= 0.5f)
			m_bItemColorPulseSwap = false;

		if (m_bItemColorPulseSwap)
			m_fItemColorPulse -= 1.0f * Time.GetDeltaTime();
		else
			m_fItemColorPulse += 2.0f * Time.GetDeltaTime();
	}
}

void CFrontend::ProcessMenuOptions(bool enter, glm::int8 arrows) {
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
		break;
	case MENUACTION_SCREENRES:
		break;
	default:
		break;
	}
}

void CFrontend::Draw() {
	if (!m_bInitialized || !m_bMenuActive)
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
		float spacing = 20.0f * i;

		Font.Reset();
		Font.SetFontAlignment(FONT_ALIGN_LEFT);
		Font.SetFontStyle(FONT_STYLE_MENU);
		Font.SetWrapX(SCREEN_SCALE_RIGHT(0.0f));
		Font.SetScale(SCREEN_SCALE_Y(11.0f));

		if (m_nCurrentItem == i)
			Font.SetColor(glm::vec4(ToVec4(static_cast<glm::uint8>(m_fItemColorPulse * 255), 16, 0, 255)));
		else
			Font.SetColor(glm::vec4(1.0f));

		if (m_bDrawMouse && CheckHover(SCREEN_SCALE_X(278.0f + 22.0f), SCREEN_SCALE_X(278.0f + 22.0f) + Font.GetStringWidth(item.name), SCREEN_SCALE_Y(240.0f + 11.0f + spacing), SCREEN_SCALE_Y(240.0f + 11.0f + spacing) + Font.GetCharacterSize(item.name.at(0)).y)) {
			m_nCurrentItem = i;

			if (Input.m_NewMouse.button[MOUSE_BUTTON_LEFT])
				ProcessMenuOptions(true, 0);
		}

		Font.PrintString({ SCREEN_SCALE_X(278.0f + 22.0f), SCREEN_SCALE_Y(240.0f + 11.0f + spacing) }, item.name);
		i++;
	}
}

void CFrontend::Shutdown() {
	if (!m_bInitialized)
		return;

	for (auto& it : m_pFrontendSprites) {
		it->Delete();
		it.reset();
	}
	Clear();
}

void CFrontend::Clear() {
	m_bInitialized = false;
	m_pStyle.reset();
	m_nCurrentPage = MENUPAGE_NONE;
	m_nCurrentItem = 0;
	m_vMenuPages = {};
	m_fItemColorPulse = 1.0f;
	m_bItemColorPulseSwap = false;
	m_fCreditsScrollY = 0.0f;
	m_bMenuActive = false;
	m_bWantsToLoad = true;
	m_bDrawMouse = false;
}

bool CFrontend::OpenMenu(glm::int32 page) {
	if (!Init())
		return false;

	ChangeMenuPage(page);
	m_bMenuActive = true;
	GraphicDevice.SetCursorOnOff(true);
	GraphicDevice.CenterMousePosition();
	return true;
}

void CFrontend::CloseMenu() {
	Shutdown();
	m_bMenuActive = false;
	GraphicDevice.SetCursorOnOff(false);
	GraphicDevice.CenterMousePosition();
}

void CFrontend::DrawCredits() {
	Engine.QuitGame();
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

	m_nCurrentPage = page;
	m_nCurrentItem = item;
}

void CFrontend::GoBack() {
	ChangeMenuPage(GetCurrentPage()->previousPage, GetCurrentPage()->previousItem);
}

void CFrontend::DoStuffBeforeStartingGame() {
	m_bWantsToLoad = true;
	m_bMenuActive = false;
}

tMenuPage* CFrontend::AddPage(eMenuPages index, tMenuPage page) {
	m_vMenuPages.at(index) = page;
	return &m_vMenuPages.at(index);
}

void CFrontend::AddItem(tMenuPage* page, tMenuItem item) {
	page->menuItems.push_back(item);
}

bool CFrontend::CheckHover(float x1, float x2, float y1, float y2) {
	return Input.m_NewMouse.pos.x > x1 && Input.m_NewMouse.pos.x < x2&&
		Input.m_NewMouse.pos.y > y1 && Input.m_NewMouse.pos.y < y2;
}
