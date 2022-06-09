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

class CWorld : public ABaseObject {
public:
    std::unique_ptr<CMap> m_pMap;
    float m_fGravity;
    std::shared_ptr<CPlayerPed> m_pPlayerPed;

public:
    CWorld();
    ~CWorld();

    void Construct();
    void Init();
    void Update();
    void LateUpdate();
    void Render();
    void Destruct();
    void Shutdown();
   
public:
    bool const& IsLevelLoaded() { return m_pMap && m_pMap->m_bInitialised; }
    std::shared_ptr<CStyle> const& GetStyle() { return m_pMap->GetStyle(); }

public:
    void LoadLevel(glm::uint8 area);
};

extern CWorld World;
