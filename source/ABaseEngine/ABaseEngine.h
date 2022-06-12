#pragma once
#include "ABaseHeader.h"
#include "Config.h"

enum eThreadState {
    TSTATE_IDLE,
    TSTATE_LOAD,
};

struct tMultiThread {
    std::thread thread;
    glm::uint8 state;
    bool stop;
    std::vector<std::function<void()>> funcs;
};

class ABaseEngine {
private:
#ifdef SUPPORT_MT
    std::shared_ptr<tMultiThread> m_pSecondThread;

private:
    void SetState(eThreadState state);
#endif

public:
    ABaseEngine();
    ~ABaseEngine();

    void Run(int argc, char* argv[]);

#ifdef SUPPORT_MT
    void Run2();
#endif
    void ThreadCallBack(bool second, std::function<void()> fun);
};

extern ABaseEngine BaseEngine;
