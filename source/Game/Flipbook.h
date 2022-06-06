#pragma once
#include "ABaseObject.h"
#include "Common.h"

class CFlipbook : public ABaseObject {
private:
	glm::uint16 currentFrame;

public:
	std::vector<glm::uint16> frames;
	double currentTime;
	bool loop;
	double timeToPassForNextFrame;
	
public:
	CFlipbook();
	~CFlipbook();

public:
	void Construct();
	void Update();

public:
	glm::uint32 GetFrame();
};