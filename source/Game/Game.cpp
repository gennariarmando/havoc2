#include "Game.h"
#include "ASprite.h"
#include "AConsole.h"
#include "Frontend.h"
#include "AEngine.h"
#include "ACamera.h"
#include "Hud.h"
#include "AGraphicDevice.h"
#include "LoadingScreen.h"
#include "VideoPlayer.h"
#include "World.h"
#include "Renderer.h"

CGame Game;

CGame::CGame() {
	m_eGameState = GS_BEGIN;
}

void CGame::Run() {
	switch (m_eGameState) {
	case GS_BEGIN:
		StateBegin();
		break;
	case GS_INTRO:
		StateIntro();
		break;
	case GS_FRONTEND:
		StateFrontend();
		break;
	case GS_LOAD:
		StateLoad();
		break;
	case GS_INGAME:
		StateInGame();
		break;
	case GS_END:
		StateEnd();
		break;
	}
}

void CGame::SetGameState(eGameState state) {
	m_eGameState = state;
}

bool CGame::InitialiseGame(glm::uint8 level) {
	LoadingScreen.Draw();
	bool map = World.InitMap(level);
	LoadingScreen.Draw();
	bool style = World.InitStyle(level);
	LoadingScreen.Draw();
	bool playa = World.InitPlayer();
	LoadingScreen.Draw();

	if (map && style && playa)
		return true;

	return false;
}

void CGame::StateBegin() {
	SetGameState(GS_INTRO);
}

void CGame::StateIntro() {
	Frontend.OpenMenu(MENUPAGE_MAIN);
	SetGameState(GS_FRONTEND);
}

void CGame::StateFrontend() {
	Frontend.Update();
	Frontend.Draw();

	if (!Frontend.m_bMenuActive && Frontend.m_bWantsToLoad) {
		SetGameState(GS_LOAD);
		LoadingScreen.Draw();
	}
}

void CGame::StateLoad() {
	Frontend.CloseMenu();

	if (InitialiseGame(LEVEL_DOWNTOWN_SP)) {
		SetGameState(GS_INGAME);
	}
}

void CGame::StateInGame() {
	Camera.Update();
	Frontend.Update();

	Renderer.Render();
}

void CGame::StateEnd() {

}
