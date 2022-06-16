#pragma once
#include "common.h"

class ATime {
private:
	double m_dDeltaTime;
	double m_dPreviousTime;
	double m_dCurrentTime;
	double m_dTimeDifference;
	double m_dFramePerSecond;
	glm::int32 m_nFrameCounter;

public:
	ATime();
	void BeginFrame();
	void EndFrame();

public:
	float const& GetDeltaTime() { return static_cast<float>(m_dDeltaTime); }
	float const& GetPreviousTime() { return static_cast<float>(m_dPreviousTime); }
	float const& GetTime() { return static_cast<float>(m_dCurrentTime); }
	float const& GetTimeDifference() { return static_cast<float>(m_dTimeDifference); }
	float const& GetFramePerSecond() { return static_cast<float>(m_dFramePerSecond); }

	glm::int32 const& GetFrameCounter() { return m_nFrameCounter; }
};

extern ATime Time;
