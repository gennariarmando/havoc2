#include "ABaseEngine.h"
#include "ABaseObject.h"
#include "ABaseGL.h"
#include "Sprite2D.h"
#include "Camera.h"
#include "Common.h"

void ABaseEngine::Run() {
    ABaseGL::Init();

    while (!ABaseGL::WindowShouldClose()) {
        ABaseGL::BeginDrawing();

        for (auto& iter : BaseObjectsList)
            iter && !iter->m_bInitialized ? iter->Init(), iter->m_bInitialized = true : NULL;

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Update(), NULL : NULL;

        ABaseGL::BeginScene3D();

        for (auto& iter : BaseObjectsList)
            iter&& iter->m_bInitialized ? iter->Render(), NULL : NULL;

        ABaseGL::EndScene3D();

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Draw2D(), NULL : NULL;

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Draw2DDebug(), NULL : NULL;

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->LateUpdate(), NULL : NULL;

        ABaseGL::EndDrawing();
    }

    for (auto& iter : BaseObjectsList)
        iter && iter->m_bInitialized ? iter->Shutdown(), iter->m_bInitialized = false : NULL;

    ABaseGL::Shutdown();
}
