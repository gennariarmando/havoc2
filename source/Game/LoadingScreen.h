#pragma once
#include "common.h"

class CLoadingScreen {
public:
	float m_fLoadingProgress;

public:
	CLoadingScreen();
    void Draw();

public:
	void IncreaseLoadingProgress();
	void ResetLoadingProgress();
};

extern CLoadingScreen LoadingScreen;
