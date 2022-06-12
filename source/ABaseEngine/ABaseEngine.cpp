#include "ABaseEngine.h"
#include "ABaseObject.h"
#include "ABaseDevice.h"

ABaseEngine BaseEngine;

void ABaseEngine::SetState(eBaseEngineState state) {
    m_eState = state;
}

void ABaseEngine::AddFun(void (*function)()) {
    m_pFunPointers.push_back(function);
}

void ABaseEngine::FlushFun() {
    SetState(STATE_LOAD);

    glm::uint32 i = 0;
    for (auto it : m_pFunPointers) {
        it();
        m_pFunPointers.erase(m_pFunPointers.begin() + i);
        i++;
    }

    SetState(STATE_IDLE);
}

void ABaseEngine::Run() {
    if (m_bRunning)
        return;

    m_bRunning = true;
    SetState(STATE_IDLE);

    BaseDevice = std::make_unique<ABaseDevice>();
    BaseDevice->Init();

    auto a = []() {
        while (!BaseDevice->WindowShouldClose()) {
            BaseEngine.FlushFun();
        }
    };

    std::unique_ptr<std::thread> t = std::make_unique<std::thread>(a);

    while (!BaseDevice->WindowShouldClose()) {
        BaseDevice->BeginDrawing();
        if (BaseDevice->Update()) {
            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_BEGINPLAY);

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_UPDATE);

            BaseDevice->BeginScene3D();

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_RENDER);

            BaseDevice->EndScene3D();

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_DRAW2D);

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_DRAW2DDEBUG);

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_LATEUPDATE);

                ABaseObject::Flush();

            for (glm::uint32 i = 0; i < baseObjects.size(); i++)
                    if (baseObjects.at(i)->IsValid()) baseObjects.at(i)->CallEvent(BASECALLEVENT_ENDPLAY);

            BaseDevice->EndDrawing();
        }
    }
    t->join();



    BaseDevice->Shutdown();
}