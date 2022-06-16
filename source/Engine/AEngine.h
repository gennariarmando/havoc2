#pragma once
#include "common.h"

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

class AEngine {
private:
    bool m_bCloseEngine;

#ifdef SUPPORT_MT
    std::shared_ptr<tMultiThread> m_pSecondThread;

private:
    void SetState(eThreadState state);
#endif

public:
    AEngine();
    ~AEngine();

    void Run(glm::int32 argc, char* argv[]);
    void ParseLaunchArguments(glm::int32 argc, char* argv[]);
    bool Init(glm::int32 argc, char* argv[]);
    void Shutdown(glm::uint32 code);
    void BeginFrame();
    void EndFrame();

#ifdef SUPPORT_MT
    void Run2();
#endif
    glm::int32 ThreadCallBack(bool second, std::function<glm::int32()> fun);

    void QuitGame();
};

extern AEngine Engine;
