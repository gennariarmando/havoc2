#include "AEngine.h"
#include "AGraphicDevice.h"
#include "AConsole.h"
#include "AGraphicDevice.h"
#include "ATime.h"
#include "AScreen.h"
#include "ASprite.h"
#include "AEngineSettings.h"
#include "AInput.h"
#include "ACamera.h"
#include "APhysics.h"
#include "Game.h"
#include "Frontend.h"
#include "LoadingScreen.h"
#include "Text.h"
#include "World.h"
#include "version.h"

AEngine Engine;

AEngine::AEngine() {
    m_bCloseEngine = false;

#ifdef ASYNC_DATA_LOAD
    m_pSecondThread = std::make_shared<tMultiThread>();
    m_pSecondThread->state = TSTATE_IDLE;
    m_pSecondThread->thread = std::thread([&]() { this->Run2(); });
    m_pSecondThread->stop = false;
#endif
}

AEngine::~AEngine() {

}

#ifdef ASYNC_DATA_LOAD
void AEngine::SetState(eThreadState state) {
    m_pSecondThread->state = state;
}
#endif

glm::int32 AEngine::ThreadCallBack(bool second, std::function<glm::int32()> fun) {
#ifdef ASYNC_DATA_LOAD
    if (second) {
        m_pSecondThread->funcs.push_back(fun);
    }
    else
#endif
        return fun();
}

void AEngine::QuitGame() {
    m_bCloseEngine = true;
}

void AEngine::Run(glm::int32 argc, char* argv[]) {
    if (!Init(argc, argv)) {
        Shutdown(EXIT_FAILURE);
    }

    while (true) {
        if (m_bCloseEngine || glfwWindowShouldClose(GraphicDevice.m_pWindow))
            break;

        BeginFrame();
        Game.Run();
        EndFrame();   
    }

#ifdef ASYNC_DATA_LOAD
    m_pSecondThread->stop = true;
    m_pSecondThread->thread.join();
#endif

    Shutdown(EXIT_SUCCESS);
}

#ifdef ASYNC_DATA_LOAD
void AEngine::Run2() {
    while (!m_pSecondThread->stop) {
        glm::uint32 i = 0;
        for (auto it : m_pSecondThread->funcs) {
            SetState(TSTATE_LOAD);
            it();
            m_pSecondThread->funcs.erase(m_pSecondThread->funcs.begin() + i);
            i++;
        }

        SetState(TSTATE_IDLE);
    }
}
#endif

void AEngine::ParseLaunchArguments(glm::int32 argc, char* argv[]) {
    if (argc > 64)
        throw std::runtime_error("Too many input parameters");

    const std::vector<std::string_view> args(argv + 1, argv + argc);
    for (const auto& arg : args) {
        if (arg == "-fullscreen") {
            EngineSettings.m_bFullscreen = true;
        }
        if (arg == "-windowed") {
            EngineSettings.m_bFullscreen = false;
        }
        if (arg == "-nointro") {
            Game.SetGameState(GS_INIT_FRONTEND);
        }
    }
}

bool AEngine::Init(glm::int32 argc, char* argv[]) {
    Console.Init();
    Console.WriteLine("Initialize engine");
    Console.WriteLine(std::format("{} v{}.{}.{}", GAME_TITLE, GAME_MAJOR_VERSION, GAME_MINOR_VERSION, GAME_BUILD_VERSION));

    EngineSettings.Load();
    ParseLaunchArguments(argc, argv);

    if (!GraphicDevice.Init()) {
        Console.WriteLine("Error initializing graphic device");
        return false;
    }

    if (!ASprite::Init()) {
        Console.WriteLine("Error initializing sprites");
        return false;
    }

    if (!Camera.Init()) {
        Console.WriteLine("Error initializing camera");
        return false;
    }

    if (!TheText.Load()) {
        Console.WriteLine("Error initializing text");
        return false;
    }

    if (!Physics.Init()) {
        Console.WriteLine("Error initializing physics engine");
    }

    if (!World.Init()) {
        Console.WriteLine("Error initializing world");
    }

    return true;
}

void AEngine::Shutdown(glm::uint32 code) {
    Console.WriteLine("Shutting down engine");

    if (code == EXIT_SUCCESS) {
        EngineSettings.Save();
    }

    Physics.Shutdown();
    World.Shutdown();
    ASprite::Shutdown();
    Console.Shutdown();
    GraphicDevice.Shutdown();
    Camera.Shutdown();
    TheText.UnLoad();

    exit(code);
}

void AEngine::BeginFrame() {
    Time.BeginFrame();
    Input.BeginFrame();
    GraphicDevice.BeginFrame();

    { // Add here
        Physics.BeginFrame();

    }

    GraphicDevice.Update();
}

void AEngine::EndFrame() {
    { // Add here
        Physics.EndFrame();
        
    }
    GraphicDevice.EndFrame();
    Input.EndFrame();
    Time.EndFrame();
}
