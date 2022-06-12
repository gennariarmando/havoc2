#pragma once
#include "ABaseObject.h"

class ABaseNextFrame : public ABaseObject {
private:
	std::function<void()> m_pFun;
	glm::uint32 m_nFramesToSkip;
	glm::uint32 m_nFramesSkipped;

public:
	ABaseNextFrame();
	ABaseNextFrame(std::function<void()> fun, glm::uint32 frameSkip);

	void Update() override;
	void EndPlay() override;
};
