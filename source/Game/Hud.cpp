#include "Hud.h"
#include "ASprite.h"
#include "Style.h"
#include "Font.h"
#include "AScreen.h"
#include "ACamera.h"

CHud Hud;

CHud::CHud() {

}

void CHud::Draw() {

}

void CHud::DrawDebugStuff() {
    Font.Reset();
    Font.SetFontAlignment(FONT_ALIGN_LEFT);
    Font.SetWrapX(SCREEN_WIDTH);
    Font.SetScale(SCREEN_SCALE_H(7.0f));
    Font.SetFontStyle(FONT_STYLE_HEADING);

    Font.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(0.0f), 0.0f }, "x:" + std::to_string((Camera.GetPosition().x)));

    Font.SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(128.0f), 0.0f }, "y: " + std::to_string((Camera.GetPosition().y)));

    Font.SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    Font.PrintString({ SCREEN_SCALE_X(256.0f), 0.0f }, "z: " + std::to_string((Camera.GetPosition().z)));
}

void CHud::DrawProgressBar(glm::vec4 rect, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor) {
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    ASprite::DrawRect(rect, backColor);

    rect.z *= progress;
    ASprite::DrawRect(rect, frontColor);
}

void CHud::DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor) {
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    ASprite::DrawRect(glm::vec4(x, y, x + w, y + h), backColor);
    ASprite::DrawRect(glm::vec4(x, y, x + (w * progress), y + h), frontColor);
}

