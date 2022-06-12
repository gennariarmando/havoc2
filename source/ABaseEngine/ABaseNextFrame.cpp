#include "ABaseNextFrame.h"

ABaseNextFrame::ABaseNextFrame() {
	m_pFun = nullptr;
	m_nFramesToSkip = 0;
	m_nFramesSkipped = 0;
}

ABaseNextFrame::ABaseNextFrame(std::function<void()> fun, glm::uint32 frameSkip) : Super::ABaseObject() {
	m_pFun = fun;
	m_nFramesToSkip = frameSkip;
	m_nFramesSkipped = 0;
}

void ABaseNextFrame::Update() {
	if (m_nFramesSkipped >= m_nFramesToSkip)
		Delete(this);

	m_nFramesSkipped++;
}

void ABaseNextFrame::EndPlay() {
	m_pFun();
}
