#include "Hud.h"
#include "Sprite2D.h"
#include "ABaseDevice.h"
#include "Style.h"
#include "Font.h"
#include "Camera.h"
#include "Screen.h"

CHud Hud;

CHud::CHud() {

}

void CHud::BeginPlay() {

}

void CHud::Update() {

}

void CHud::LateUpdate() {

}

void CHud::Render() {

}

void CHud::Draw2D() {

}

void CHud::Draw2DDebug() {
    Font.Reset();
    Font.SetFontAlignment(FONT_ALIGN_LEFT);
    Font.SetWrapX(SCREEN_WIDTH);
    Font.SetScale(SCREEN_SCALE_H(0.9f));
    Font.SetFontStyle(FONT_STYLE_HEADING);

    Font.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(0.0f), 0.0f }, "x:" + std::to_string((Camera.GetPosition().x)));

    Font.SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(128.0f), 0.0f }, "y: " + std::to_string((Camera.GetPosition().y)));

    Font.SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(256.0f), 0.0f }, "z: " + std::to_string((Camera.GetPosition().z)));
}

void CHud::EndPlay() {

}

void CHud::DrawProgressBar(glm::vec4 rect, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor) {
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    CSprite2D::DrawRect(rect, backColor);

    rect.z *= progress;
    CSprite2D::DrawRect(rect, frontColor);
}

void CHud::DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor) {
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    CSprite2D::DrawRect(glm::vec4(x, y, x + w, y + h), backColor);
    CSprite2D::DrawRect(glm::vec4(x, y, x + (w * progress), y + h), frontColor);
}
