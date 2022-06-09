#include "ABaseEngine.h"
#include "ABaseObject.h"
#include "ABaseDevice.h"

ABaseEngine BaseEngine;

void ABaseEngine::Run() {
    std::unique_ptr<ABaseDevice> BaseDevice = std::make_unique<ABaseDevice>();
    BaseDevice->Init();

    while (!BaseDevice->WindowShouldClose()) {
        BaseDevice->BeginDrawing();
        BaseDevice->Update();

        for (auto& iter : BaseObjectsList)
            iter && !iter->m_bInitialized ? iter->Init(), iter->m_bInitialized = true : NULL;
        
        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Update(), NULL : NULL;

        BaseDevice->BeginScene3D();

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Render(), NULL : NULL;

        BaseDevice->EndScene3D();

        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Draw2D(), NULL : NULL;
        
        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->Draw2DDebug(), NULL : NULL;
        
        for (auto& iter : BaseObjectsList)
            iter && iter->m_bInitialized ? iter->LateUpdate(), NULL : NULL;

        BaseDevice->EndDrawing();
    }

    //for (auto& iter : BaseObjectsList)
    //    iter && iter->m_bInitialized ? iter->Shutdown(), iter->m_bInitialized = false : NULL;

    BaseDevice->Shutdown();
}
