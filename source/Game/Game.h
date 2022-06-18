#include "common.h"

enum eGameState {
    GS_BEGIN,
    GS_INTRO,
    GS_INIT_FRONTEND,
    GS_FRONTEND,
    GS_LOAD,
    GS_INGAME,
    GS_END,
};

class CGame {
public:
    eGameState m_eGameState;

public:
    CGame();
    void Run();
     
public:
    void SetGameState(eGameState state);
    bool InitialiseGame(glm::uint8 level);
    
    void StateBegin();
    void StateIntro();
    void StateInitFrontend();
    void StateFrontend();
    void StateLoad();
    void StateInGame();
    void StateEnd();
};

extern CGame Game;