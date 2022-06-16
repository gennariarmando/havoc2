#pragma once
#include "Style.h"

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
	MENUPAGE_RESUMESAVEDSTATUS,
	MENUPAGE_VIEWHIGHSCORES,
	MENUPAGE_STARTPLAYINAREA,
	MENUPAGE_VIDEO,
	NUM_MENUPAGES
};

enum eFrontendActions {
	MENUACTION_NONE,
	MENUACTION_CHANGEPAGE,
	MENUACTION_SETPLAYERNAME,
	MENUACTION_STARTGAME,
	MENUACTION_SCREENRES,
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
	std::vector<tMenuPage> m_vMenuPages;
	float m_fItemColorPulse;
	bool m_bItemColorPulseSwap;
	float m_fCreditsScrollY;
	bool m_bMenuActive;
	bool m_bWantsToLoad;
	bool m_bDrawMouse;

public:
	CFrontend();

	bool Init();
	void Update();
	void ProcessMenuOptions(bool enter, glm::int8 input);
	void Draw();
	void Shutdown();

public:
	void OpenMenu(glm::int32 page);
	void CloseMenu();
	void DrawCredits();
	void DrawBackground();
	void ChangeMenuPage(glm::int32 page, glm::int32 item = 0);
	void GoBack();
	void DoStuffBeforeStartingGame();

public:
	tMenuPage* AddPage(eMenuPages index, tMenuPage page = { {} });
	void AddItem(tMenuPage* page, tMenuItem item);
	bool CheckHover(float x1, float x2, float y1, float y2);
	tMenuPage* GetCurrentPage() { return &m_vMenuPages.at(m_nCurrentPage); }
	tMenuItem* GetCurrentItem() { return &m_vMenuPages.at(m_nCurrentPage).menuItems.at(m_nCurrentItem); }

public:
	std::shared_ptr<CStyle> const& GetStyle() { return !m_pStyle || m_pStyle->IsLoading() ? nullptr : m_pStyle; }
};

extern CFrontend Frontend;
