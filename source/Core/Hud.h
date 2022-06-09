#pragma once
#include "ABaseObject.h"

class CHud : public ABaseObject {
public:
    CHud();
    ~CHud();

    void Construct();
    void Init();
    void Update();
    void LateUpdate();
    void Render();
    void Draw2D();
    void Draw2DDebug();
    void Destruct();
    void Shutdown();

public:
    void DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor);
};

extern CHud Hud;
