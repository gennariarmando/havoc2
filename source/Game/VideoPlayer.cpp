#include "VideoPlayer.h"
#include "AScreen.h"
#include "ASprite.h"
#include "AGraphicDevice.h"
#include "LoadingScreen.h"

CVideoPlayer VideoPlayer;

CVideoPlayer::CVideoPlayer() {
	m_pBink = nullptr;
	m_pPixels = nullptr;
	m_nCurrentFrame = 0;
}

bool CVideoPlayer::Open(std::string const& path) {
	BinkSoundUseDirectSound(nullptr);

	m_pBink = BinkOpen(path.c_str(), 0);

	if (!m_pBink)
		return false;

	m_nWidth = m_pBink->Width;
	m_nHeight = m_pBink->Height;
	m_nPitch = m_pBink->Width * 3;

	m_nCurrentFrame = 0;
	m_nLastFrame = m_pBink->LastFrameNum;

	m_pPixels = std::make_unique<glm::uint8[]>(m_nPitch * m_nHeight);
	return true;
}

void CVideoPlayer::Close() {
	BinkClose(m_pBink);
	m_pBink = nullptr;
	m_pPixels.reset();

	m_nWidth = 0;
	m_nHeight = 0;
	m_nPitch = 0;

	m_nCurrentFrame = 0;
	m_nLastFrame = 0;
}

int CVideoPlayer::PlayFrame() {
	if (!m_pBink)
		return VIDEOPLAYER_ERROR;

	GraphicDevice.SetCursorOnOff(false);

	BinkDoFrame(m_pBink);
	BinkCopyToBuffer(m_pBink, m_pPixels.get(), m_nPitch, m_nHeight, 0, 0, 0x4000000);

	ASprite m_pSprite;
	m_pSprite.m_pTexture->Build(m_pPixels.get(), m_nWidth, m_nHeight, GL_RGB8, GL_BGR);
	m_pSprite.Draw(glm::vec4((SCREEN_WIDTH / 2) - SCREEN_SCALE_W(static_cast<float>(m_nWidth) / 2), 0.0f, (SCREEN_WIDTH / 2) + SCREEN_SCALE_W(static_cast<float>(m_nWidth) / 2), SCREEN_HEIGHT), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	if (m_nCurrentFrame >= m_nLastFrame)
		return VIDEOPLAYER_END;

	if (!BinkWait(m_pBink)) {
		BinkNextFrame(m_pBink);
		m_nCurrentFrame++;
	}
	return VIDEOPLAYER_PLAYING;
}
