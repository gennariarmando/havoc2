#include "Sprite.h"

CSprite::CSprite() {
	m_pTexture = std::make_unique<CTexture2D>();
}

CSprite::~CSprite() {
	m_pTexture.release();
}

void CSprite::DrawPlane(glm::vec3 pos, glm::vec2 const& scale, glm::vec4 const& col) {
    m_Geometry.SetTexture(m_pTexture->GetID());

    m_Geometry.SetLocation(pos.x, pos.y, pos.z);
    m_Geometry.SetScale(scale.x, scale.y, 1.0f);
    m_Geometry.SetColor(col.r, col.g, col.b, col.a);

    m_Geometry.SetPrimitive(GL_TRIANGLES);

    m_Geometry.SetVertex(0.0f, 1.0f);
    m_Geometry.SetTexCoords(0.0f, 1.0f);

    m_Geometry.SetVertex(1.0f, 0.0f);
    m_Geometry.SetTexCoords(1.0f, 0.0f);

    m_Geometry.SetVertex(0.0f, 0.0f);
    m_Geometry.SetTexCoords(0.0f, 0.0f);

    m_Geometry.SetVertex(0.0f, 1.0f);
    m_Geometry.SetTexCoords(0.0f, 1.0f);

    m_Geometry.SetVertex(1.0f, 1.0f);
    m_Geometry.SetTexCoords(1.0f, 1.0f);

    m_Geometry.SetVertex(1.0f, 0.0f);
    m_Geometry.SetTexCoords(1.0f, 0.0f);

    m_Geometry.Render();
}
