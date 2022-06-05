#pragma once
#include "ABaseObject.h"
#include "box2d.h"
#include "Style.h"
#include "Map.h"

class CWorld : public ABaseObject {
public:
    std::unique_ptr<b2World> b2World;
    std::unique_ptr<CMap> map;

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
