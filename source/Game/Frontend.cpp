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
#include "AEngineSettings.h"
#include "Text.h"

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
		AddItem(page, { MENUACTION_CHANGEPAGE, "play", MENUPAGE_PLAY });
		AddItem(page, { MENUACTION_CHANGEPAGE, "options", MENUPAGE_OPTIONS });
		AddItem(page, { MENUACTION_CHANGEPAGE, "quit", MENUPAGE_CREDITS });
	}

	// MENUPAGE_PLAY
	if (tMenuPage* page = AddPage(MENUPAGE_PLAY, { MENUPAGE_MAIN, 0, { FE_2_NAME, FE_2_RESTART, FE_2_LEAGUE, FE_2_LEVEL1 }, { } } )) {
		AddItem(page, { MENUACTION_SETPLAYERNAME, "blank", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_RESUMESAVEDSTATUS, "savepos", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_CHANGEPAGE, "hi_scre", MENUPAGE_VIEWHIGHSCORES });
		AddItem(page, { MENUACTION_STARTPLAYINAREA, "strtlev", MENUPAGE_NONE });
	}

	// MENUPAGE_OPTIONS
	if (tMenuPage* page = AddPage(MENUPAGE_OPTIONS, { MENUPAGE_MAIN, 1, { FE_1_OPTIONS, FE_1_OPTIONS, FE_1_OPTIONS }, { } })) {
		AddItem(page, { MENUACTION_CHANGEPAGE, "blank", MENUPAGE_VIDEO });
		AddItem(page, { MENUACTION_CHANGEPAGE, "blank", MENUPAGE_AUDIO });
	}

	// MENUPAGE_CREDITS
	if (tMenuPage* page = AddPage(MENUPAGE_CREDITS, { MENUPAGE_NONE, 0, { FE_CREDITS }, { } })) {

	}

	// MENUPAGE_VIEWHIGHSCORES
	if (tMenuPage* page = AddPage(MENUPAGE_VIEWHIGHSCORES, { MENUPAGE_PLAY, 2, { FE_2_LEAGUE }, { } })) {

	}

	// MENUPAGE_VIDEO
	if (tMenuPage* page = AddPage(MENUPAGE_VIDEO, { MENUPAGE_OPTIONS, 0, { FE_1_OPTIONS, FE_1_OPTIONS, FE_1_OPTIONS }, { } })) {
		AddItem(page, { MENUACTION_SCREENRES, "blank", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_FULLSCREEN, "blank", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_VSYNC, "blank", MENUPAGE_NONE });
	}

	// MENUPAGE_AUDIO
	if (tMenuPage* page = AddPage(MENUPAGE_AUDIO, { MENUPAGE_OPTIONS, 1, { FE_1_OPTIONS, FE_1_OPTIONS	 }, { } })) {
		AddItem(page, { MENUACTION_SFX, "psxeff", MENUPAGE_NONE });
		AddItem(page, { MENUACTION_MUSIC, "psxmus", MENUPAGE_NONE });

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
	bool lmb = Input.m_NewMouse.button[MOUSE_BUTTON_LEFT] && !Input.m_OldMouse.button[MOUSE_BUTTON_LEFT];
	bool itemMouse = m_nHoverItem == m_nCurrentItem && lmb;

	if (abs(Input.m_NewMouse.delta.x) > 0.0f || abs(Input.m_NewMouse.delta.y) > 0.0f) {
		GraphicDevice.SetCursorOnOff(true);
		m_bDrawMouse = true;
	}
	else if (up || down || left || right || enter || back) {
		GraphicDevice.SetCursorOnOff(false);
		m_bDrawMouse = false;
	}

	enter |= itemMouse;

	if (back)
		GoBack();

	if (GetCurrentPage()->menuItems.size() > 0) {
		if (up)
			m_nCurrentItem--;

		if (down)
			m_nCurrentItem++;

		m_nCurrentItem = ClampInverse(m_nCurrentItem, 0, GetCurrentPage()->menuItems.size() - 1);

		if (enter || left || right)
			ProcessMenuOptions(enter, left ? -1 : right ? 1 : 0);

		if (m_nHoverItem != -1)
			m_nCurrentItem = m_nHoverItem;

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
	case MENUACTION_BACK:
		if (enter)
			GoBack();
		break;
	case MENUACTION_STARTPLAYINAREA:
		if (arrows < 0) {

		}
		else if (arrows > 0) {

		}
		else if (enter) {
			ChangeMenuPage(GetCurrentItem()->targetPage);
			DoStuffBeforeStartingGame();
		}
		break;
	case MENUACTION_SCREENRES:
		if (arrows < 0)
			EngineSettings.m_nVideoMode--;
		else if (arrows > 0)
			EngineSettings.m_nVideoMode++;

		EngineSettings.m_nVideoMode = ClampInverse(EngineSettings.m_nVideoMode, 0, GraphicDevice.m_vVideoModes.size() - 1);
		EngineSettings.m_nScreenWidth = GraphicDevice.m_vVideoModes.at(EngineSettings.m_nVideoMode).width;
		EngineSettings.m_nScreenHeight = GraphicDevice.m_vVideoModes.at(EngineSettings.m_nVideoMode).height;

		GraphicDevice.SetVideoMode(EngineSettings.m_nVideoMode);
		EngineSettings.Save();
		break;
	case MENUACTION_FULLSCREEN:
		if (arrows) {
			EngineSettings.m_bFullscreen = !EngineSettings.m_bFullscreen;
			GraphicDevice.SetFullscreen(EngineSettings.m_bFullscreen);
			EngineSettings.Save();
		}
		break;
	case MENUACTION_VSYNC:
		if (arrows) {
			EngineSettings.m_bVSync = !EngineSettings.m_bVSync;
			GraphicDevice.SetVSync(EngineSettings.m_bVSync);
			EngineSettings.Save();
		}
		break;
	case MENUACTION_SFX:
		EngineSettings.m_nSfx += arrows;
		EngineSettings.m_nSfx = Clamp(EngineSettings.m_nSfx, 0, 32);
		break;
	case MENUACTION_MUSIC:
		EngineSettings.m_nMusic += arrows;
		EngineSettings.m_nMusic = Clamp(EngineSettings.m_nMusic, 0, 32);
		break;
	}
}

std::string CFrontend::GetActionRightString(glm::uint8 action) {
	std::string no = {};

	switch (action) {
	case MENUACTION_CHANGEPAGE:
		break;
	case MENUACTION_BACK:
		break;
	case MENUACTION_SETPLAYERNAME:
		//no = "_";
		break;
	case MENUACTION_STARTPLAYINAREA:
		break;
	case MENUACTION_SCREENRES:
		no = GraphicDevice.m_vVideoModes.at(EngineSettings.m_nVideoMode).str;
		break;
	case MENUACTION_FULLSCREEN:
		no = std::format("{}", EngineSettings.m_bFullscreen);
		break;
	case MENUACTION_VSYNC:
		no = std::format("{}", EngineSettings.m_bVSync);
		break;
	}

	return no;
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

	m_nHoverItem = -1;

	const glm::int32 menuItems = static_cast<glm::int32>(GetCurrentPage()->menuItems.size());
	glm::uint32 i = 0;
	for (auto& item : GetCurrentPage()->menuItems) {
		float centerOffset = 0.0f;
		if (menuItems > MENU_ITEM_MAX / 2)
			centerOffset = (((MENU_ITEM_SPACING) * (menuItems)) * 0.5f);

		float spacing = (MENU_ITEM_SPACING * i);
		float x = MENU_ITEM_POS_X;
		float y = MENU_ITEM_POS_Y - centerOffset;

		const std::string leftString = TheText.Get(item.name.c_str());
		const std::string rightString = GetActionRightString(m_vMenuPages.at(m_nCurrentPage).menuItems.at(i).action);

		Font.Reset();
		Font.SetFontAlignment(FONT_ALIGN_LEFT);
		Font.SetFontStyle(FONT_STYLE_MENU);
		Font.SetWrapX(SCREEN_SCALE_RIGHT(0.0f));

		bool hasRightString = rightString.empty();

		if (hasRightString)
			Font.SetScale(SCREEN_SCALE_Y(MENU_ITEM_HEIGHT));
		else
			Font.SetScale(SCREEN_SCALE_Y(MENU_ITEM_HEIGHT * 0.9f));

		float leftStringWidth = Font.GetStringWidth(leftString + ' ');
		float rightStringWidth = 0.0f;
		float charHeight = Font.GetCharacterSize(leftString.at(0)).y;

		if (hasRightString)
			rightStringWidth = Font.GetStringWidth(rightString + ' ');

		if (m_nCurrentItem == i)
			Font.SetColor(glm::vec4(ToVec4(static_cast<glm::uint8>(m_fItemColorPulse * MENU_ITEM_COLOR_R), MENU_ITEM_COLOR_G, MENU_ITEM_COLOR_B, 255)));
		else
			Font.SetColor(glm::vec4(1.0f));

		Font.PrintString({ SCREEN_SCALE_X(x), SCREEN_SCALE_Y(y + spacing) }, leftString);

		if (!hasRightString)
			Font.PrintString({ SCREEN_SCALE_X(x) + leftStringWidth, SCREEN_SCALE_Y(y + spacing) }, rightString);

		float sliderSize = 0.0f;
		switch (item.action) {
		case MENUACTION_SFX:
			sliderSize = DrawSlider(SCREEN_SCALE_X(x) + leftStringWidth, SCREEN_SCALE_Y(y + spacing), SCREEN_SCALE_H(MENU_SLIDER_RECT_W), SCREEN_SCALE_H(MENU_SLIDER_RECT_H), EngineSettings.m_nSfx);
			break;
		case MENUACTION_MUSIC:
			sliderSize = DrawSlider(SCREEN_SCALE_X(x) + leftStringWidth, SCREEN_SCALE_Y(y + spacing), SCREEN_SCALE_H(MENU_SLIDER_RECT_W), SCREEN_SCALE_H(MENU_SLIDER_RECT_H), EngineSettings.m_nMusic);
			break;
		case MENUACTION_STARTPLAYINAREA:
			DrawOneChar(SCREEN_SCALE_X(x + MENU_ITEM_AREA_NUMBER), SCREEN_SCALE_Y(y + spacing + 1.0f) + charHeight, '1');
			break;
		}

		if (CheckHover(SCREEN_SCALE_X(x), SCREEN_SCALE_X(x) + leftStringWidth + rightStringWidth + sliderSize, SCREEN_SCALE_Y(y + spacing), SCREEN_SCALE_Y(y + spacing) + charHeight))
			m_nHoverItem = i;

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
	m_nHoverItem = -1;
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
	if (GetCurrentPage()->background.empty())
		return;

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
		m_pFrontendSprites.at(background)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), glm::vec4(1.0f));
	}
	else {
		m_pFrontendSprites.at(left)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), glm::vec4(1.0f));
		m_pFrontendSprites.at(right)->Draw(SCREEN_SCALE_X(MENU_BACKGROUND_LEFT_WIDTH), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(MENU_BACKGROUND_RIGHT_WIDTH), SCREEN_SCALE_H(MENU_BACKGROUND_HEIGHT), glm::vec4(1.0f));
	}
}

void CFrontend::ChangeMenuPage(glm::int32 page, glm::int32 item) {
	if (page == MENUPAGE_NONE || m_vMenuPages.size() < page)
		return;

	m_nCurrentPage = page;
	m_nCurrentItem = item;
	m_nHoverItem = -1;
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

glm::uint8 CFrontend::CheckHover(float x1, float x2, float y1, float y2) {
	glm::uint8 result = 0;

	if (m_bDrawMouse) {
		bool pos = (Input.m_NewMouse.pos.x > x1 && Input.m_NewMouse.pos.x < x2) && (Input.m_NewMouse.pos.y > y1 && Input.m_NewMouse.pos.y < y2);
		bool lmb = Input.m_NewMouse.button[MOUSE_BUTTON_LEFT] && !Input.m_OldMouse.button[MOUSE_BUTTON_LEFT];

		if (pos)
			result = 1;

		if (pos && lmb)
			result = 2;
	}

	return result;
}

float CFrontend::DrawSlider(float x, float y, float w, float h, glm::int32& progress) {
	const float spacing = SCREEN_SCALE_Y(MENU_SLIDER_RECTANGLE_SPACING) + w;
	const float numRectsF = static_cast<float>(MENU_SLIDER_RECTANGLES);
	glm::vec4 col;
	float p = progress / static_cast<float>(MENU_SLIDER_RECTANGLES);

	float sliderSize = 0.0f;
	for (glm::int32 i = 0; i < MENU_SLIDER_RECTANGLES; i++) {
		if ((i / numRectsF) < p)
			col = { i / numRectsF, 1.0f - (i / numRectsF), 0.1f, 1.0f };
		else
			col = glm::vec4(0.1f);

		ASprite::DrawRect({ x, y, x + w, y + h }, col);

		if (CheckHover(x, x + w, y, y + h) == 2)
			progress = i + 1;

		x += spacing;
		sliderSize += spacing;
	}

	return sliderSize;
}

void CFrontend::DrawOneChar(float x, float y, char c) {
	static ASprite sprite;
	sprite.SetTexture(GetStyle()->GetSprite().at(4));

	sprite.Draw(x, y, SCREEN_SCALE_W(MENU_ONE_CHAR_SIZE), SCREEN_SCALE_H(MENU_ONE_CHAR_SIZE), glm::vec4(1.0f));

	Font.Reset();
	Font.SetFontAlignment(FONT_ALIGN_CENTER);
	Font.SetFontStyle(FONT_STYLE_MENU);
	Font.SetWrapX(SCREEN_SCALE_RIGHT(0.0f));
	Font.SetScale(SCREEN_SCALE_Y(MENU_ITEM_HEIGHT));
	Font.SetColor(glm::vec4(1.0f));
	Font.PrintString({ x + SCREEN_SCALE_W(MENU_ONE_CHAR_SIZE * 0.5f), y + (Font.GetCharacterSize(c).y * 0.125f) }, {c});
}
