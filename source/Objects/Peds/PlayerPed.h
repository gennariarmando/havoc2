#pragma once
#include "Common.h"
#include "Entity.h"

class CPlayerPed {
public:
	std::shared_ptr<CEntity> m_pEntity;

public:
	CPlayerPed();
	~CPlayerPed();
};
