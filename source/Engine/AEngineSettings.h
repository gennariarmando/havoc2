#pragma once
#include "common.h"

class AEngineSettings {
public:
	glm::uint32 m_nScreenWidth;
	glm::uint32 m_nScreenHeight;
	bool m_bFullscreen;
	bool m_bVSync;

public:
	AEngineSettings();
	void Load();
	void Save();
};

extern AEngineSettings EngineSettings;
