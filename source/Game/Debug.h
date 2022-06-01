#pragma once
#include "ABaseObject.h"

class CDebug : public ABaseObject {
public:
    CDebug();
    ~CDebug();

    void Construct();
    void Init();
    void Update();
    void LateUpdate();
    void Render();
    void Draw2D();
    void Draw2DDebug();
    void Destruct();
    void Shutdown();
};

extern CDebug Debug;
