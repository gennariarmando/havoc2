#pragma once
#include "common.h"
#include <reactphysics3d/reactphysics3d.h>

class APhysics {
public:
	rp3d::PhysicsCommon* m_pPhysicsCommon;
	rp3d::PhysicsWorld* m_pPhysicsWorld;

public:
	bool Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
};

extern APhysics Physics;
