#pragma once
#include "Common.h"
#include "ABaseObject.h"
#include "Style.h"
#include "Map.h"
#include "PlayerPed.h"

enum {
    LEVEL_WIL, // wil - William Mills - Downtown District (Singleplayer)
    LEVEL_STE, // ste - Stephen Banks - Residential District (Singleplayer)
    LEVEL_BIL, // bil - Billy Thompson - Industrial District (Singleplayer)

    LEVEL_DOWNTOWN_SP = LEVEL_WIL, 
    LEVEL_RESIDENT_SP = LEVEL_STE, 
    LEVEL_INDUST_SP = LEVEL_BIL, 

    LEVEL_DOWNTOWN_MP, // wil-multi
    LEVEL_RESIDENT_MP, // ste-multi
    LEVEL_INDUST_MP, // bil-multi

    LEVEL_MP1, // mp1
    LEVEL_MP2, // mp2
    LEVEL_MP3, // mp3 

    LEVEL_BONUS_A, // lorne2e - Lorne Campbell 2 Easy
    LEVEL_BONUS_B, // lorne2m - Lorne Campbell 2 Medium	
    LEVEL_BONUS_C, // lorne2h - Lorne Campbell 2 Hard

    LEVEL_BONUS_D, // mike1e - Mike Stirling 1 Easy	
    LEVEL_BONUS_E, // mike1m - Mike Stirling 1 Medium	
    LEVEL_BONUS_F, // mike1h - Mike Stirling 1 Hard	

    LEVEL_BONUS_G, // mike2e - Mike Stirling 2 Easy	
    LEVEL_BONUS_H, // mike2m - Mike Stirling 2 Medium	
    LEVEL_BONUS_I, // mike2h - Mike Stirling 2 Hard	
};

struct tLevelList {
    std::string gmp;
    std::string sty;
};

enum eGameState {
    GSTATE_INTRO,
    GSTATE_MENU,
    GSTATE_LOAD,
    GSTATE_PLAY,
    GSTATE_CLOSE,
};

class CGame : public ABaseObject {
public:
    glm::uint8 m_nGameState;
    std::shared_ptr<CMap> m_pMap;
    std::shared_ptr<CStyle> m_pStyle;

    float m_fGravity;
    bool m_bInGame;
    std::shared_ptr<CPlayerPed> m_pPlayerPed;

public:
    CGame();

    void BeginPlay() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
    void EndPlay()  override;
   
public:
    std::shared_ptr<CStyle> const& GetStyle() { return !m_pStyle || m_pStyle->IsLoading() ? nullptr : m_pStyle; }

public:
    void LoadLevel(glm::uint8 area);
};

extern CGame Game;
