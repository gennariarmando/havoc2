#pragma once
#include "ABaseObject.h"
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

class CFrontend : public ABaseObject {
private:
	std::shared_ptr<CStyle> m_pStyle;
	std::vector<std::shared_ptr<CSprite2D>> m_pFrontendSprites;

public:
	std::shared_ptr<CStyle> const& GetStyle() { return m_pStyle; }

public:
	CFrontend();
	~CFrontend();

	void Construct();
	void Init();
	void Update();
	void Draw2D();
	void Shutdown();
};

extern CFrontend Frontend;