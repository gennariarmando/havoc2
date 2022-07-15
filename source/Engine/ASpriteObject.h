#pragma once
#include "common.h"
#include "ASprite.h"

class ASpriteObject {
public:
	ASprite* m_pSprite;
	glm::vec3 m_vPosition;
	glm::vec2 m_vScale;
	glm::vec3 m_vRotation;
	float m_fAngle;
	glm::vec4 m_vColor;

public:
	ASpriteObject();
	~ASpriteObject();

	void Render();

	void SetAllValues(glm::vec3 const& pos, glm::vec2 scale, glm::vec3 rot, float angle, glm::vec4 col);
};
