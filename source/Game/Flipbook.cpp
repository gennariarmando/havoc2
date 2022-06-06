#include "Flipbook.h"
#include "ABaseGL.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CFlipbook, "CFlipbook");

void CFlipbook::Construct() {
	currentFrame = 0;

	frames = {};
	currentTime = 0.0;
	loop = true;
	timeToPassForNextFrame = 1.0;
}

void CFlipbook::Update() {
	if (!ABaseGL::GetObjects().window)
		return;

	if (currentTime < ABaseGL::GetObjects().currentTime) {
		currentTime = ABaseGL::GetObjects().currentTime + timeToPassForNextFrame;
		currentFrame++;
	}

	if (currentFrame >= frames.size() - 1) {
		if (loop)
			currentFrame = 0;
		else
			currentFrame = frames.size() - 1;
	}
}

glm::uint32 CFlipbook::GetFrame() {
	return frames[currentFrame];
}
