#pragma once
#include "Common.h"
#include "Config.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480
#define DEFAULT_ASPECT_RATIO (4.0f / 3.0f)

#define SCREEN_WIDTH static_cast<float>(Screen.GetScreenWidth())
#define SCREEN_HEIGHT static_cast<float>(Screen.GetScreenHeight())
#define SCREEN_ASPECT_RATIO (Screen.GetAspectRatio())

#ifdef FRONTEND_KEEP_PROPORTIONS
#define SCREEN_SCALE_X(x) (Screen.ScaleXKeepCentered(x))
#else
#define SCREEN_SCALE_X(x) (Screen.ScaleX(x))
#endif
#define SCREEN_SCALE_Y(y) (Screen.ScaleY(y))

#define SCREEN_SCALE_W(w) (Screen.ScaleW(w))
#define SCREEN_SCALE_H(h) (Screen.ScaleH(h))

#define SCREEN_SCALE_RIGHT(x) (Screen.ScaleFromRight(x))
#define SCREEN_SCALE_BOTTOM(y) (Screen.ScaleFromBottom(y))

#define SCREEN_SCALE_CENTER_X(x) ((SCREEN_WIDTH / 2) + SCREEN_SCALE_X(x))
#define SCREEN_SCALE_CENTER_Y(y) ((SCREEN_HEIGHT / 2) + SCREEN_SCALE_Y(y))

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
    float ScaleXKeepCentered(float x);
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