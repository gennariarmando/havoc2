#include "Game.h"
#include "ASprite2D.h"
#include "AConsole.h"
#include "Frontend.h"
#include "AEngine.h"
#include "ACamera.h"
#include "Hud.h"
#include "AGraphicDevice.h"
#include "LoadingScreen.h"
#include "VideoPlayer.h"

CGame Game;

std::vector<tLevelList> levelList = {
	{ "data/wil.gmp", "data/wil.sty" },
	{ "data/ste.gmp", "data/ste.sty" },
	{ "data/bil.gmp", "data/bil.sty" },
	{ "data/wil-multi.gmp", "data/wil.sty" },
	{ "data/ste-multi.gmp", "data/ste.sty" },
	{ "data/bil-multi.gmp", "data/bil.sty" },
	{ "data/MP1-comp.gmp", "data/bil.sty" },
	{ "data/MP2-comp.gmp", "data/wil.sty" },
	{ "data/MP5-comp.gmp", "data/ste.sty" },
	{ "data/lorne2e.gmp", "data/wil.sty" },
	{ "data/lorne2m.gmp", "data/wil.sty" },
	{ "data/lorne2h.gmp", "data/wil.sty" },
	{ "data/mike1e.gmp", "data/ste.sty" },
	{ "data/mike1m.gmp", "data/ste.sty" },
	{ "data/mike1h.gmp", "data/ste.sty" },
	{ "data/mike2e.gmp", "data/bil.sty" },
	{ "data/mike2m.gmp", "data/bil.sty" },
	{ "data/mike2h.gmp", "data/bil.sty" },
};

CGame::CGame() {
	m_eGameState = GS_BEGIN;
	m_pMap = nullptr;
	m_pStyle = nullptr;
	m_fGravity = 9.81f;
}

void CGame::Run() {
	switch (m_eGameState) {
	case GS_BEGIN:
		SetGameState(GS_INTRO);
		break;
	case GS_INTRO:
		Frontend.OpenMenu(MENUPAGE_MAIN);
		SetGameState(GS_FRONTEND);
		break;
	case GS_FRONTEND:
		Frontend.Update();
		Frontend.Draw();

		if (!Frontend.m_bMenuActive && Frontend.m_bWantsToLoad) {
			SetGameState(GS_LOAD);
			LoadingScreen.Draw();
		}
		break;
	case GS_LOAD:
		if (InitialiseGame(LEVEL_DOWNTOWN_SP)) {
			SetGameState(GS_INGAME);
		}
		break;
	case GS_INGAME:
		RenderScene();
		break;
	case GS_END:
		break;
	}
}

void CGame::SetGameState(eGameState state) {
	m_eGameState = state;
}

bool CGame::InitialiseGame(glm::uint8 level) {
	LoadingScreen.Draw();

	if (m_pMap)
		m_pMap.reset();
	m_pMap = std::make_shared<CMap>(levelList.at(level).gmp);

	LoadingScreen.Draw();

	if (m_pStyle)
		m_pStyle.reset();
	m_pStyle = std::make_shared<CStyle>(levelList.at(level).sty);

	LoadingScreen.Draw();

	if ((m_pMap && !m_pMap->IsLoading()) && (m_pStyle && !m_pStyle->IsLoading()))
		return true;

	return false;
}

void CGame::RenderScene() {
	GraphicDevice.SetCursorOnOff(false);
	m_pMap->Render(m_pStyle);

	Hud.DrawDebugStuff();

	ASprite2D::DrawRect(glm::vec4(0.0f, 0.0f, 10.0f, 10.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
}
