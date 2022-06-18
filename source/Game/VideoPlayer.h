#pragma once
#include "common.h"
#include "bink.h"

enum {
	VIDEOPLAYER_ERROR,
	VIDEOPLAYER_WAIT,
	VIDEOPLAYER_PLAYING,
	VIDEOPLAYER_END
};

class CVideoPlayer {
public:
	BINK* m_pBink;
	std::shared_ptr<glm::uint8[]> m_pPixels;

	glm::int32 m_nWidth;
	glm::int32 m_nHeight;
	glm::int32 m_nPitch;
	glm::uint64 m_nCurrentFrame;
	glm::uint64 m_nLastFrame;

public:
	CVideoPlayer();
	CVideoPlayer(std::string const& path);

	bool Open(std::string const& path);
	void Close();
	int PlayFrame();
};

extern CVideoPlayer VideoPlayer;
