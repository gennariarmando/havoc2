#pragma once
#include "ABaseObject.h"

class CHud : public ABaseObject {
public:
    CHud();

    void BeginPlay() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
    void Draw2D()  override;
    void Draw2DDebug()  override;
    void EndPlay()  override;

public:
    void DrawProgressBar(glm::vec4 rect, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor);
    void DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor);
};

extern CHud Hud;
