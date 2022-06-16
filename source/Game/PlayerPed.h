#pragma once
#include "Ped.h"

class CPlayerPed : public CPed {
public:
	CPlayerPed();
	~CPlayerPed();

public:
	static void SetupPlayerPed();

public:
	void Update() override;

};
