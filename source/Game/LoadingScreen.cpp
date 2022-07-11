#include "LoadingScreen.h"
#include "Frontend.h"
#include "Font.h"
#include "AScreen.h"
#include "ATime.h"
#include "Hud.h"
#include "AGraphicDevice.h"
#include "Text.h"

CLoadingScreen LoadingScreen;

CLoadingScreen::CLoadingScreen() {
	m_fLoadingProgress = 0.0f;
}

void CLoadingScreen::Draw() {
	if (!Frontend.m_bInitialized)
		return;
	
	Frontend.m_pFrontendSprites.at(FE_LEVELCOMPLETE)->Draw(SCREEN_SCALE_X(0.0f), SCREEN_SCALE_Y(0.0f), SCREEN_SCALE_W(640.0f), SCREEN_SCALE_H(480.0f), glm::vec4(1.0f));
	Font.Reset();
	Font.SetFontAlignment(FONT_ALIGN_CENTER);
	Font.SetFontStyle(FONT_STYLE_MENU);
	Font.SetWrapX(SCREEN_WIDTH);
	Font.SetScale(SCREEN_SCALE_Y(11.0f));
	Font.PrintString({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, TheText.Get("loading"));
	IncreaseLoadingProgress();
}

void CLoadingScreen::IncreaseLoadingProgress() {
	if (m_fLoadingProgress < 1.0f) {
		m_fLoadingProgress += 1.0f * Time.GetDeltaTime();
		m_fLoadingProgress = Clamp(m_fLoadingProgress, 0.0f, 1.0f);
	}
}

void CLoadingScreen::ResetLoadingProgress() {
	m_fLoadingProgress = 0.0f;
}
