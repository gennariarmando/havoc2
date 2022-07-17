#include "AFlipbook.h"
#include "ATime.h"

AFlipbook::AFlipbook() {
	m_nCurrentFrame = 0;

	m_vFrames = {};
	m_fCurrentTime = 0.0f;
	m_nRepeat = 0;
	m_nCycles = 0;
	m_fTimeRate = 1.0f;
}

AFlipbook::AFlipbook(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate) {
	Set(frames, repeat, rate);
}

void AFlipbook::Set(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate) {
	m_vFrames = frames;
	m_nRepeat = repeat;
	m_fTimeRate = rate;
}

void AFlipbook::Update() {
	if (m_nCycles > m_nRepeat) {
		m_nCurrentFrame = static_cast<glm::uint16>(m_vFrames.size()) - 1;
		return;
	}

	if (m_fCurrentTime < Time.GetTime()) {
		m_fCurrentTime = Time.GetTime() + m_fTimeRate;
		m_nCurrentFrame++;
	}

	if (m_nCurrentFrame > m_vFrames.size() - 1) {
		m_nCurrentFrame = 0;

		if (m_nRepeat != 0)
			m_nCycles++;
	}
}

glm::uint16 AFlipbook::GetFrame() {
	return m_vFrames[m_nCurrentFrame];
}
