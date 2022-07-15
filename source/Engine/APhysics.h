#pragma once
#include "common.h"
#include "reactphysics3d.h"

namespace physics = reactphysics3d;

class APhysics {
public:
	physics::PhysicsCommon* m_pPhysicsCommon;
	physics::PhysicsWorld* m_pPhysicsWorld;

public:
	bool Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
};

extern APhysics Physics;
