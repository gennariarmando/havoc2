#include "Flipbook.h"
#include "ABaseDevice.h"

CFlipbook::CFlipbook() {
	m_nCurrentFrame = 0;

	m_vFrames = {};
	m_fCurrentTime = 0.0f;
	m_nRepeat = 0;
	m_nCycles = 0;
	m_fTimeRate = 1.0f;
}

CFlipbook::CFlipbook(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate) : Super::ABaseObject() {
	Set(frames, repeat, rate);
}

void CFlipbook::Set(std::vector<glm::uint16> frames, glm::uint8 repeat, float rate) {
	m_vFrames = frames;
	m_nRepeat = repeat;
	m_fTimeRate = rate;
}

void CFlipbook::Update() {
	if (m_nCycles > m_nRepeat) {
		m_nCurrentFrame = m_vFrames.size() - 1;
		return;
	}

	if (m_fCurrentTime < static_cast<float>(DeviceObjects.m_fCurrentTime)) {
		m_fCurrentTime = static_cast<float>(DeviceObjects.m_fCurrentTime) + m_fTimeRate;
		m_nCurrentFrame++;
	}

	if (m_nCurrentFrame > m_vFrames.size() - 1) {
		m_nCurrentFrame = 0;

		if (m_nRepeat != 0)
			m_nCycles++;
	}
}

glm::uint16 CFlipbook::GetFrame() {
	return m_vFrames[m_nCurrentFrame];
}
