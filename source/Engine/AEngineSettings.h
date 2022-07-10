#pragma once
#include "common.h"

enum eEngineLanguages {
	LANGUAGE_ENGLISH,
};

class AEngineSettings {
public:
	glm::uint32 m_nScreenWidth;
	glm::uint32 m_nScreenHeight;
	glm::int32 m_nVideoMode;
	bool m_bFullscreen;
	bool m_bVSync;
	glm::int32 m_nSfx;
	glm::int32 m_nMusic;
	glm::int8 m_nLanguage;

public:
	AEngineSettings();
	void Load();
	void Save();
};

extern AEngineSettings EngineSettings;
