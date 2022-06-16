#pragma once
#include "PlayerPed.h"

class CPlayerInfo {
public:
	std::shared_ptr<CPlayerPed> m_pPed;
	glm::int32 m_nMoney;
	glm::int32 m_nVisibleMoney;

public:
	CPlayerInfo();

};
