#pragma once
#define MENU_LEFT_PERCENTAGE (0.44f)
#define MENU_RIGHT_PERCENTAGE (0.56f)

#define MENU_BACKGROUND_WIDTH (640.0f)
#define MENU_BACKGROUND_HEIGHT (480.0f)

#define MENU_BACKGROUND_LEFT_WIDTH (MENU_BACKGROUND_WIDTH * MENU_LEFT_PERCENTAGE)
#define MENU_BACKGROUND_RIGHT_WIDTH (MENU_BACKGROUND_WIDTH * MENU_RIGHT_PERCENTAGE)

#define MENU_OPTIONS_OFFSET (22.0f)

#define MENU_ITEM_POS_X ((MENU_BACKGROUND_WIDTH * MENU_LEFT_PERCENTAGE) + MENU_OPTIONS_OFFSET)
#define MENU_ITEM_POS_Y ((MENU_BACKGROUND_HEIGHT * 0.5f) + (MENU_OPTIONS_OFFSET * 0.5f))

#define MENU_ITEM_SPACING (20.0f)

#define MENU_ITEM_AREA_NUMBER (106.0f)

#define MENU_ITEM_HEIGHT (11.0f)

#define MENU_ITEM_COLOR_R (255)
#define MENU_ITEM_COLOR_G (16)
#define MENU_ITEM_COLOR_B (0)

#define MENU_SLIDER_RECTANGLES (32)
#define MENU_SLIDER_RECTANGLE_SPACING (2.0f)
#define MENU_SLIDER_RECT_W (3.0f)
#define MENU_SLIDER_RECT_H (19.0f)

#define MENU_ONE_CHAR_SIZE 30.0f

#include "Style.h"
#include "AEngineSettings.h"

enum eFrontendSprites {
	FE_1,
	FE_1_OPTIONS,
	FE_1_PLAY,
	FE_1_QUIT,
	FE_2,
	FE_2_BONUS1,
	FE_2_BONUS2,
	FE_2_BONUS3,
	FE_2_LEAGUE,
	FE_2_LEVEL1,
	FE_2_LEVEL2,
	FE_2_LEVEL3,
	FE_2_NAME,
	FE_2_RESTART,
	FE_3,
	FE_3_TABLES,
	FE_CREDITS,
	FE_DEMOINFO,
	FE_GAMECOMPLETE,
	FE_LEVELCOMPLETE,
	FE_MASK,
	FE_MASK2,
	FE_MASK3,
	FE_MPLOSE,
	FE_PLAYERDEAD,
	NUM_FRONTEND_SPRITES,
};

enum eMenuPages {
	MENUPAGE_NONE = -1,
	MENUPAGE_MAIN,
	MENUPAGE_PLAY,
	MENUPAGE_OPTIONS,
	MENUPAGE_CREDITS,
	MENUPAGE_VIEWHIGHSCORES,
	MENUPAGE_VIDEO,
	MENUPAGE_AUDIO,
	NUM_MENUPAGES
};

enum eFrontendActions {
	MENUACTION_NONE,
	MENUACTION_CHANGEPAGE,
	MENUACTION_BACK,
	MENUACTION_SETPLAYERNAME,
	MENUACTION_STARTPLAYINAREA,
	MENUACTION_RESUMESAVEDSTATUS,
	MENUACTION_SCREENRES,
	MENUACTION_FULLSCREEN,
	MENUACTION_VSYNC,
	MENUACTION_SFX,
	MENUACTION_MUSIC,
};

enum {
	MENU_ITEM_MAX = 6,
};

struct tMenuItem {
	glm::uint8 action;
	std::string name;
	glm::int8 targetPage;
};

struct tMenuPage {
	eMenuPages previousPage;
	glm::uint8 previousItem;
	std::vector<glm::uint8> background;
	std::vector<tMenuItem> menuItems;
};

struct tPreviousPage {
	glm::int32 page;
	glm::int32 item;
};

class CFrontend {
public:
	bool m_bInitialized;
	std::shared_ptr<CStyle> m_pStyle;
	std::vector<std::shared_ptr<ASprite>> m_pFrontendSprites;
	glm::int32 m_nCurrentPage;
	glm::int32 m_nCurrentItem;
	glm::int32 m_nHoverItem;
	std::vector<tMenuPage> m_vMenuPages;
	float m_fItemColorPulse;
	bool m_bItemColorPulseSwap;
	float m_fCreditsScrollY;
	bool m_bMenuActive;
	bool m_bWantsToLoad;
	bool m_bDrawMouse;

public:
	CFrontend();
	~CFrontend();

	bool Init();
	void Update();
	void ProcessMenuOptions(bool enter, glm::int8 input);
	std::string GetActionRightString(glm::uint8 action);
	void Draw();
	void Shutdown();
	void Clear();

public:
	bool OpenMenu(glm::int32 page);
	void CloseMenu();
	void DrawCredits();
	void DrawBackground();
	void ChangeMenuPage(glm::int32 page, glm::int32 item = 0);
	void GoBack();
	void DoStuffBeforeStartingGame();
	void AddItem(tMenuPage* page, tMenuItem item);
	glm::uint8 CheckHover(float x1, float x2, float y1, float y2);
	float DrawSlider(float x, float y, float w, float h, glm::int32& progress);
	void DrawOneChar(float x, float y, char c);

private:
	tMenuPage* AddPage(eMenuPages index, tMenuPage page = { {} });
	tMenuPage* GetCurrentPage() { return &m_vMenuPages.at(m_nCurrentPage); }
	tMenuItem* GetCurrentItem() { return &m_vMenuPages.at(m_nCurrentPage).menuItems.at(m_nCurrentItem); }

public:
	std::shared_ptr<CStyle> GetStyle() { return !m_pStyle || m_pStyle->IsLoading() ? nullptr : m_pStyle; }
};

extern CFrontend Frontend;
