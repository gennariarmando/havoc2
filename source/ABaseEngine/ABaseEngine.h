#pragma once
#include "ABaseHeader.h"

enum eBaseEngineState {
    STATE_IDLE,
    STATE_LOAD,
};

class ABaseEngine {
private:
    bool m_bRunning;
    eBaseEngineState m_eState;
    std::vector<void(*)()> m_pFunPointers;
    std::unique_ptr<std::thread> m_pThread;

private:
    void SetState(eBaseEngineState state);
    void FlushFun();

public:
    bool const& IsLoading() const { return m_eState == STATE_LOAD; }

public:
    void Run();
    void AddFun(void (*function)());
};

extern ABaseEngine BaseEngine;
