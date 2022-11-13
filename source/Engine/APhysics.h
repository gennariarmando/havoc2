#pragma once
#include "common.h"

class APhysics {
public:

public:
	APhysics();
	~APhysics();
	bool Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
};

extern APhysics Physics;
