#include "ABaseEngine.h"
#include "ABaseObject.h"
#include "ABaseDevice.h"
#include "ABaseNextFrame.h"

ABaseEngine BaseEngine;

ABaseEngine::ABaseEngine() {
#ifdef SUPPORT_MT
    m_pSecondThread = std::make_shared<tMultiThread>();
    m_pSecondThread->state = TSTATE_IDLE;
    m_pSecondThread->thread = std::thread([&]() { this->Run2(); });
    m_pSecondThread->stop = false;
#endif
}

ABaseEngine::~ABaseEngine() {

}

#ifdef SUPPORT_MT
void ABaseEngine::SetState(eThreadState state) {
    m_pSecondThread->state = state;
}
#endif

void ABaseEngine::ThreadCallBack(bool second, std::function<void()> fun) {
#ifdef SUPPORT_MT
    if (second) {
        m_pSecondThread->funcs.push_back(function);
    }
    else
#endif
        NewObject<ABaseNextFrame>(fun, 1);
}

void ABaseEngine::Run(int argc, char* argv[]) {
    BaseDevice = std::make_unique<ABaseDevice>();
    BaseDevice->Init();

    while (!BaseDevice->WindowShouldClose()) {
        BaseDevice->BeginDrawing();
        if (BaseDevice->Update()) {
            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_BEGINPLAY);

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_UPDATE);

            BaseDevice->BeginScene3D();

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_RENDER);

            BaseDevice->EndScene3D();

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_DRAW2D);

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_DRAW2DDEBUG);

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_LATEUPDATE);

                ABaseObject::Flush();

            for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
                    if (vBaseObjects.at(i)->IsValid()) vBaseObjects.at(i)->CallEvent(BASECALLEVENT_ENDPLAY);

            BaseDevice->EndDrawing();
        }
    }

    ABaseObject::Erase();

#ifdef SUPPORT_MT
    m_pSecondThread->stop = true;
    m_pSecondThread->thread.join();
#endif

    BaseDevice->Shutdown();
}

#ifdef SUPPORT_MT
void ABaseEngine::Run2() {
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
