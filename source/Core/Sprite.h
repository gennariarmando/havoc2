#pragma once
#include "Geometry.h"
#include "Sprite2D.h"

class CSprite {
private:
	std::unique_ptr<CTexture2D> m_pTexture;
	CGeometry m_Geometry;

public:
	CSprite();
	~CSprite();
	void DrawPlane(glm::vec3 pos, glm::vec2 const& scale, glm::vec4 const& col);
};
