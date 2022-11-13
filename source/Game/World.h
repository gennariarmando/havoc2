#pragma once
#include "common.h"
#include "Entity.h"
#include "PlayerInfo.h"
#include "Map.h"
#include "Style.h"

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

class CWorld {
private:
	std::list<CEntity*> m_vEntities;
	CPlayerInfo* m_pPlayer;
    CMap* m_pMap;
    CStyle* m_pStyle;

public:
	CWorld();
    ~CWorld();

    bool Init();
    void Shutdown();

	void Add(CEntity* e);
	void Remove(CEntity* e);
	bool InitMap(glm::uint8 level);
	bool InitStyle(glm::uint8 level);
    bool InitPlayer();
    void DestroyAllEntities();

public:
    std::list<CEntity*> GetEntityList() { return m_vEntities; }
    CPlayerInfo* GetPlayer() { return m_pPlayer; }
    CMap* GetMap() { return m_pMap; }
    CStyle* GetStyle() { return !m_pStyle || m_pStyle->IsLoading() ? nullptr : m_pStyle; }
};

extern CWorld World;