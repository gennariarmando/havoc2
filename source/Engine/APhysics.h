#pragma once
#include "common.h"
#include <reactphysics3d/reactphysics3d.h>

namespace phy = reactphysics3d;

class APhysics {
public:
	phy::PhysicsCommon* m_pPhysicsCommon;
	phy::PhysicsWorld* m_pPhysicsWorld;

public:
	bool Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
};

extern APhysics Physics;
