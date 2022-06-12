#include "LoadingScreen.h"
#include "Frontend.h"
#include "Hud.h"
#include "Font.h"
#include "Screen.h"
#include "ABaseDevice.h"

CLoadingScreen::CLoadingScreen() {
	m_fLoadingProgress = 0.0f;
}

void CLoadingScreen::Draw2D() {
	Frontend.m_pFrontendSprites.at(FE_LEVELCOMPLETE)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(640.0f), SCREEN_SCALE_H(480.0f), glm::vec4(1.0f));
	Font.Reset();
	Font.SetFontAlignment(FONT_ALIGN_CENTER);
	Font.SetFontStyle(FONT_STYLE_MENU);
	Font.SetWrapX(SCREEN_WIDTH);
	Font.SetScale(SCREEN_SCALE_Y(11.0f));
	Font.PrintString({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, "GET READY...");
#ifdef SUPPORT_MT
	Hud.DrawProgressBar({ SCREEN_SCALE_X(22.0f), SCREEN_SCALE_BOTTOM(22.0f), SCREEN_SCALE_RIGHT(22.0f), SCREEN_SCALE_BOTTOM(16.0f) }, m_fLoadingProgress, glm::vec4(1.0f, 0.06f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	IncreaseLoadingProgress();
}

void CLoadingScreen::IncreaseLoadingProgress() {
	if (m_fLoadingProgress >= 1.0f) {
		Delete(this);
	}
	else {
		m_fLoadingProgress += 1.0f * DeviceObjects.m_fDeltaTime;
		m_fLoadingProgress = Clamp(m_fLoadingProgress, 0.0f, 1.0f);
	}
}

void CLoadingScreen::ResetLoadingProgress() {
	m_fLoadingProgress = 0.0f;
}
