#include "Flipbook.h"
#include "ABaseGL.h"

IMPLEMENT_PRIMARY_GAME_MODULE(CFlipbook, "CFlipbook");

void CFlipbook::Construct() {
	currentFrame = 0;

	frames = {};
	currentTime = 0.0f;
	loop = true;
	timeToPassForNextFrame = 1.0f;
}

void CFlipbook::Update() {
	if (!ABaseGL::GetObjects().window)
		return;

	if (currentTime < static_cast<float>(ABaseGL::GetObjects().currentTime)) {
		currentTime = static_cast<float>(ABaseGL::GetObjects().currentTime) + timeToPassForNextFrame;
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
