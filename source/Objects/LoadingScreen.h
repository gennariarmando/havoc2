#pragma once
#include "ABaseObject.h"

class CLoadingScreen : public ABaseObject {
public:
	float m_fLoadingProgress;

public:
	CLoadingScreen();

    void Draw2D()  override;

public:
	void IncreaseLoadingProgress();
	void ResetLoadingProgress();
};
