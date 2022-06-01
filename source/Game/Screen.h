#pragma once
#include "Common.h"

#define DEFAULT_SCREEN_WIDTH 1280
#define DEFAULT_SCREEN_HEIGHT 720
#define DEFAULT_ASPECT_RATIO (16.0f / 9.0f)

#define SCREEN_WIDTH static_cast<float>(Screen.GetScreenWidth())
#define SCREEN_HEIGHT static_cast<float>(Screen.GetScreenHeight())
#define SCREEN_ASPECT_RATIO (Screen.GetAspectRatio())

#define HUD_X(x) (Screen.ScaleX(x))
#define HUD_Y(y) (Screen.ScaleY(y))

#define HUD_W(w) (Screen.ScaleW(w))
#define HUD_H(h) (Screen.ScaleH(h))

#define HUD_RIGHT(x) (Screen.ScaleFromRight(x))
#define HUD_BOTTOM(y) (Screen.ScaleFromBottom(y))

class CScreen {
public:
    glm::uint32 m_nScreenWidth;
    glm::uint32 m_nScreenHeight;
    float m_fAspectRatio;
    float m_fSafeZone;

public:
    CScreen() {
        m_nScreenWidth = DEFAULT_SCREEN_WIDTH;
        m_nScreenHeight = DEFAULT_SCREEN_HEIGHT;
        m_fAspectRatio = m_nScreenWidth / (float)m_nScreenHeight;
        m_fSafeZone = 0.0f;
    }
    float ScaleX(float x);
    float ScaleY(float y);
    float ScaleW(float w);
    float ScaleH(float h);
    float ScaleFromRight(float x);
    float ScaleFromBottom(float y);

public:
    glm::uint32& GetScreenWidth() { return m_nScreenWidth; }
    glm::uint32& GetScreenHeight() { return m_nScreenHeight; }
    float& GetAspectRatio() { return m_fAspectRatio; }
    float& GetSafeZone() { return m_fSafeZone; }
};

extern CScreen Screen;