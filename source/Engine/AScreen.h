#pragma once
#include "common.h"

#define DEFAULT_SCREEN_WIDTH (static_cast<float>(Screen.m_nDefaultWidth))
#define DEFAULT_SCREEN_HEIGHT (static_cast<float>(Screen.m_nDefaultHeight))
#define DEFAULT_SCREEN_ASPECT_RATIO (DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT)

#define SCREEN_WIDTH (static_cast<float>(Screen.m_nScreenWidth))
#define SCREEN_HEIGHT (static_cast<float>(Screen.m_nScreenHeight))
#define SCREEN_ASPECT_RATIO (SCREEN_WIDTH / SCREEN_HEIGHT)

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

class AScreen {
public:
	glm::uint32 m_nDefaultWidth;
	glm::uint32 m_nDefaultHeight;
	glm::uint32 m_nScreenWidth;
	glm::uint32 m_nScreenHeight;
	float m_fAspectRatio;
	glm::uint32 m_nRefreshRate;
	bool m_bFullscreen;

public:
	AScreen();

	float ScaleX(float x);
	float ScaleXKeepCentered(float x);
	float ScaleY(float y);
	float ScaleW(float w);
	float ScaleH(float h);
	float ScaleFromRight(float x);
	float ScaleFromBottom(float y);
};

extern AScreen Screen;
