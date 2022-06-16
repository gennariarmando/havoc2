#pragma once
#include "common.h"

class CRenderer {
public:
	CRenderer();

	void Render();
	void Draw2D();
};

extern CRenderer Renderer;
