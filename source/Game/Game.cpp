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
#include "VideoPlayer.h"
#include "AInput.h"

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
	case GS_INIT_FRONTEND:
		StateInitFrontend();
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
	}
}

void CGame::SetGameState(eGameState state) {
	m_eGameState = state;
}

bool CGame::InitialiseGame(glm::uint8 level) {
	LoadingScreen.Draw();

	bool map = World.InitMap(level);
	bool style = World.InitStyle(level);
	bool playa = World.InitPlayer();

	if (map && style && playa)
		return true;

	return false;
}

void CGame::StateBegin() {
	auto v = VideoPlayer.Open("data/Movie/intro.bik");
	if (!v)
		Console.WriteLine("Error while opening bink video");

	SetGameState(GS_INTRO);
}

void CGame::StateIntro() {
	auto v = VideoPlayer.PlayFrame();
	bool skip = Input.GetKeyJustDown(KEY_ENTER) || Input.m_NewMouse.button[MOUSE_BUTTON_LEFT];

	if (v != VIDEOPLAYER_PLAYING || skip) {
		VideoPlayer.Close();
		GraphicDevice.BeginFrame();
		SetGameState(GS_INIT_FRONTEND);
	}
}

void CGame::StateInitFrontend() {
	Frontend.OpenMenu(MENUPAGE_MAIN);
	SetGameState(GS_FRONTEND);
}

void CGame::StateFrontend() {
	Frontend.Update();
	Frontend.Draw();

	if (!Frontend.m_bMenuActive && Frontend.m_bWantsToLoad) {
		SetGameState(GS_LOAD);
		LoadingScreen.Draw();
		Frontend.CloseMenu();
	}
}

void CGame::StateLoad() {
	if (InitialiseGame(LEVEL_DOWNTOWN_SP)) {
		SetGameState(GS_INGAME);
	}
}

void CGame::StateInGame() {
	Camera.Update();
	Frontend.Update();

	Renderer.Render();
}
