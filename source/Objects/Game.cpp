#include "Game.h"
#include "ABaseDevice.h"
#include "ABaseEngine.h"
#include "Camera.h"
#include "Common.h"
#include "Entity.h"
#include "Frontend.h"
#include "LoadingScreen.h"

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
    m_pMap = NULL;
    m_pStyle = NULL;
    m_fGravity = 9.81f;
    m_bInGame = false;
    m_nGameState = GSTATE_MENU;
}

void CGame::BeginPlay() {

}

void CGame::Update() {
    switch (m_nGameState) {
    case GSTATE_INTRO:
        Frontend.OpenMenu(MENUPAGE_MAIN);
        m_nGameState++;
        break;
    case GSTATE_MENU:
        if (!Frontend.m_bMenuActive)
            m_nGameState++;
        break;
    case GSTATE_LOAD:
        if ((m_pMap && !m_pMap->IsLoading()) && (m_pStyle && !m_pStyle->IsLoading()))
            m_nGameState++;
        break;
    case GSTATE_PLAY:
        m_bInGame = true;
        break;
    case GSTATE_CLOSE:
        break;
    }
}

void CGame::LateUpdate() {

}

void CGame::Render() {
    if (!m_bInGame)
        return;

    m_pMap->Render(m_pStyle);
}

void CGame::EndPlay() {
    m_bInGame = false;
}

void CGame::LoadLevel(glm::uint8 area) {
    m_pMap.reset();
    m_pMap = std::make_shared<CMap>(levelList.at(area).gmp); 

    m_pStyle.reset();
    m_pStyle = std::make_shared<CStyle>(levelList.at(area).sty);
}
