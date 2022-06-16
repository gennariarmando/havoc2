#include "AScreen.h"

AScreen Screen;

AScreen::AScreen() {
	m_nDefaultWidth = 640;
	m_nDefaultHeight = 480;
	m_nScreenWidth = m_nDefaultWidth;
	m_nScreenHeight = m_nDefaultHeight;
	m_fAspectRatio = m_nDefaultWidth / static_cast<float>(m_nDefaultHeight);
	m_nRefreshRate = false;
	m_bFullscreen = false;
}

float AScreen::ScaleX(float x) {
    float f = ((x) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

float AScreen::ScaleXKeepCentered(float x) {
    float f = ((SCREEN_WIDTH == DEFAULT_SCREEN_WIDTH) ? (x) : (SCREEN_WIDTH - ScaleX(DEFAULT_SCREEN_WIDTH)) / 2 + ScaleX((x)));
    return f;
}

float AScreen::ScaleY(float y) {
    float f = ((y) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

float AScreen::ScaleW(float w) {
    float f = ((w) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

float AScreen::ScaleH(float h) {
    float f = ((h) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

float AScreen::ScaleFromRight(float x) {
#ifdef FRONTEND_KEEP_PROPORTIONS
    return SCREEN_WIDTH - ScaleXKeepCentered(x);
#else
    return SCREEN_WIDTH - ScaleX(x);
#endif
}

float AScreen::ScaleFromBottom(float y) {
    return SCREEN_HEIGHT - ScaleY(y);
}
