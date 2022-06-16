#include "ATime.h"
#include "AScreen.h"

ATime Time;

ATime::ATime() {
	m_dDeltaTime = 0.0;
	m_dPreviousTime = 0.0;
	m_dCurrentTime = 0.0;
	m_dTimeDifference = 0.0;
	m_dFramePerSecond = 0.0;
	m_nFrameCounter = 0;
}

void ATime::BeginFrame() {
	m_nFrameCounter++;

	m_dCurrentTime = glfwGetTime();
	m_dTimeDifference = Time.m_dCurrentTime - Time.m_dPreviousTime;

	m_dFramePerSecond = (1.0 / m_dTimeDifference) * m_nFrameCounter;
	m_dDeltaTime = (Time.m_dTimeDifference / Time.m_nFrameCounter);
}

void ATime::EndFrame() {
	if (Time.m_dTimeDifference > 1.0 / static_cast<double>(Screen.m_nRefreshRate)) {
		Time.m_dPreviousTime = Time.m_dCurrentTime;
		Time.m_nFrameCounter = 0;
	}

	while (glfwGetTime() < Time.m_dPreviousTime + 1.0 / static_cast<double>(Screen.m_nRefreshRate));
}
