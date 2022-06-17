#include "VideoPlayer.h"
#include "AScreen.h"
#include "ASprite.h"
#include "AGraphicDevice.h"
#include "LoadingScreen.h"

CVideoPlayer VideoPlayer;

CVideoPlayer::CVideoPlayer() {
	m_pBink = nullptr;
	m_pPixels = nullptr;
	m_nFrames = 0;
}

bool CVideoPlayer::Open(std::string const& path) {
	BinkSoundUseDirectSound(nullptr);

	m_pBink = BinkOpen(path.c_str(), 0);

	if (!m_pBink)
		return false;

	m_pPixels = std::make_shared<glm::uint8[]>(m_pBink->Width * m_pBink->Height * 3);
	return true;
}

void CVideoPlayer::Close() {
	BinkClose(m_pBink);
	m_pBink = nullptr;
	m_pPixels.reset();
	m_nFrames = 0;
}

int CVideoPlayer::PlayFrame() {
	if (!m_pBink)
		return VIDEOPLAYER_ERROR;

	GraphicDevice.SetCursorOnOff(false);

	BinkDoFrame(m_pBink);
	BinkCopyToBuffer(m_pBink, m_pPixels.get(), m_pBink->Width * 3, m_pBink->Height, 0, 0, 0x4000000);

	ASprite m_pSprite;
	m_pSprite.m_pTexture->Build(m_pPixels.get(), m_pBink->Width, m_pBink->Height, GL_RGB8, GL_BGR);
	m_pSprite.Draw(glm::vec4((SCREEN_WIDTH / 2) - SCREEN_SCALE_W(m_pBink->Width / 2), 0.0f, (SCREEN_WIDTH / 2) + SCREEN_SCALE_W(m_pBink->Width / 2), SCREEN_HEIGHT), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	if (m_nFrames >= m_pBink->Frames)
		return VIDEOPLAYER_END;

	if (!BinkWait(m_pBink)) {
		BinkNextFrame(m_pBink);
		m_nFrames++;
	}
	return VIDEOPLAYER_PLAYING;
}
