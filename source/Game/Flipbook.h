#pragma once
#include "common.h"

class CFlipbook {
private:
	glm::uint16 m_nCurrentFrame;
	std::vector<glm::uint16> m_vFrames;
	float m_fCurrentTime;
	glm::uint8 m_nRepeat;
	glm::uint8 m_nCycles;
	float m_fTimeRate;
	
public:
	CFlipbook();
	CFlipbook(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate);

	void Update();

public:
	void Set(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate);
	glm::uint16 GetFrame();
};
