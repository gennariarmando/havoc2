#pragma once
#include "ABaseObject.h"
#include "box2d.h"

class CWorld : public ABaseObject {
public:
    std::unique_ptr<b2World> world;

public:
    CWorld();
    ~CWorld();

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

extern CWorld Debug;
