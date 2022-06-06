#include "Sprite2D.h"
#include "ABaseGL.h"
#include "Camera.h"

const char* textureFileFormat = ".tga";

CSprite2D::CSprite2D() {
    texture = std::make_unique<CTexture2D>();
}

CSprite2D::~CSprite2D() {
    texture.release();
}

void CSprite2D::Draw(float x, float y, float w, float h, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void CSprite2D::Draw(float x, float y, float w, float h, glm::vec2 const& center, float angle, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), center, angle, col);
}

void CSprite2D::Draw(glm::vec4 const& rect, glm::vec4 const& col) {
    Draw(rect, glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void CSprite2D::Draw(glm::vec4 const& rect, glm::vec2 const& center, float angle, glm::vec4 const& col) {
    im3D.SetTexture(texture->GetID());

    glm::vec2 c;
    c.x = ((rect.z - rect.x)) * center.x;
    c.y = ((rect.w - rect.y)) * center.y;
    im3D.SetLocation(rect.x, rect.y, 0.0f);
    im3D.Translate(c.x, c.y, 0.0f);
    im3D.SetRotation(0.0f, 0.0f, 1.0f, angle);
    im3D.Translate(-c.x, -c.y, 0.0f);

    im3D.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);

    im3D.SetColor(col.r, col.g, col.b, col.a);

    im3D.SetPrimitive(GL_TRIANGLES);

    im3D.SetVertex(0.0f, 1.0f);
    im3D.SetTexCoords(0.0f, 1.0f);

    im3D.SetVertex(1.0f, 0.0f);
    im3D.SetTexCoords(1.0f, 0.0f);

    im3D.SetVertex(0.0f, 0.0f);
    im3D.SetTexCoords(0.0f, 0.0f);

    im3D.SetVertex(0.0f, 1.0f);
    im3D.SetTexCoords(0.0f, 1.0f);

    im3D.SetVertex(1.0f, 1.0f);
    im3D.SetTexCoords(1.0f, 1.0f);

    im3D.SetVertex(1.0f, 0.0f);
    im3D.SetTexCoords(1.0f, 0.0f);

    im3D.Render();
}

void CSprite2D::DrawRect(glm::vec4 const& rect, glm::vec4 const& col) {
    CGeometry im3D;
    im3D.SetTexture(0);
    //
    im3D.SetLocation(rect.x, rect.y, 0.0f);
    im3D.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);
    im3D.SetColor(col.r, col.g, col.b, col.a);
    
    im3D.SetPrimitive(GL_TRIANGLES);
    im3D.SetVertex(0.0f, 1.0f);
    im3D.SetVertex(1.0f, 0.0f);
    im3D.SetVertex(0.0f, 0.0f);
    im3D.SetVertex(0.0f, 1.0f);
    im3D.SetVertex(1.0f, 1.0f);
    im3D.SetVertex(1.0f, 0.0f);
    im3D.Render();
}
