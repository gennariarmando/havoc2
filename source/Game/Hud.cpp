#include "Hud.h"
#include "Sprite2D.h"
#include "ABaseGL.h"
#include "Style.h"
#include "Font.h"
#include "Camera.h"
#include "Screen.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CHud, "CHud");

CHud Hud;

void CHud::Construct() {

}

void CHud::Init() {

}

void CHud::Update() {

}

void CHud::LateUpdate() {

}

void CHud::Render() {

}

void CHud::Draw2D() {
    std::string cam = "Camera position: " + std::to_string(Camera.GetPosition().x) + ", " + std::to_string(Camera.GetPosition().y) + ", " + std::to_string(Camera.GetPosition().z);
    Font.SetFontAlignment(FONT_ALIGN_RIGHT);
    Font.SetWrapX(SCREEN_WIDTH);
    Font.SetScale(2.0f);
    Font.SetColor(glm::vec4(1.0f));
    Font.SetFontStyle(FONT_SUBTITLES);
    Font.PrintString({ (SCREEN_WIDTH), (0.0f) }, cam);
}

void CHud::Draw2DDebug() {

}

void CHud::Destruct() {

}

void CHud::Shutdown() {

}

void CHud::DrawProgressBar(float x, float y, float w, float h, float progress, glm::vec4 const& frontColor, glm::vec4 const& backColor) {
    if (progress <= 0.0f)
        progress = 0.0f;
    else if (progress >= 1.0f)
        progress = 1.0f;

    CSprite2D::DrawRect(glm::vec4(x, y, x + w, y + h), backColor);
    CSprite2D::DrawRect(glm::vec4(x, y, x + (w * progress), y + h), frontColor);
}
