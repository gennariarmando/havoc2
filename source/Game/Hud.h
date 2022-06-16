#pragma once
#include "common.h"

class CHud {
public:
    CHud();

public:
    void DrawDebugStuff();
    void DrawProgressBar(glm::vec4 rect, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor);
    void DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor);
};

extern CHud Hud;
