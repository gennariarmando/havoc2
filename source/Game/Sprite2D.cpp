#include "Sprite2D.h"
#include "ABaseGL.h"
#include "Targa.h"

CGeometry CSprite2D::ms_Geometry = {};

CSprite2D::CSprite2D() {
    m_pTexture = std::make_unique<CTexture2D>();
}

CSprite2D::CSprite2D(glm::uint32 id) {
    m_pTexture = std::make_unique<CTexture2D>();
    SetTexture(id);
}

CSprite2D::CSprite2D(std::string path, std::string name) {
    m_pTexture = std::make_unique<CTexture2D>();
    SetTexture(path, name);
}

CSprite2D::~CSprite2D() {
    m_pTexture.release();
}

bool CSprite2D::SetTexture(glm::uint32 id) {
    m_pTexture->GetID() = id;
    return true;
}

bool CSprite2D::SetTexture(std::string path, std::string name) {
    std::string file = path + "/" + name + ".tga";
    CTarga tga;
    if (tga.LoadTexture(file.c_str())) {
        m_pTexture->Build(tga.imageData.get(), tga.imageWidth, tga.imageHeight, tga.bitCount / 8, GL_NEAREST);
        tga.Free();
    }
    else {
        return false;
    }

    return true;
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
    m_Geometry.SetTexture(m_pTexture->GetID());

    glm::vec2 c;
    c.x = ((rect.z - rect.x)) * center.x;
    c.y = ((rect.w - rect.y)) * center.y;
    m_Geometry.SetLocation(rect.x, rect.y, 0.0f);
    m_Geometry.Translate(c.x, c.y, 0.0f);
    m_Geometry.SetRotation(0.0f, 0.0f, 1.0f, angle);
    m_Geometry.Translate(-c.x, -c.y, 0.0f);

    m_Geometry.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);

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

void CSprite2D::DrawRect(glm::vec4 const& rect, glm::vec4 const& col) {
    ms_Geometry.SetTexture(0);

    ms_Geometry.SetLocation(rect.x, rect.y, 0.0f);
    ms_Geometry.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);
    ms_Geometry.SetColor(col.r, col.g, col.b, col.a);
    
    ms_Geometry.SetPrimitive(GL_TRIANGLES);
    ms_Geometry.SetVertex(0.0f, 1.0f);
    ms_Geometry.SetVertex(1.0f, 0.0f);
    ms_Geometry.SetVertex(0.0f, 0.0f);
    ms_Geometry.SetVertex(0.0f, 1.0f);
    ms_Geometry.SetVertex(1.0f, 1.0f);
    ms_Geometry.SetVertex(1.0f, 0.0f);
    ms_Geometry.Render();
}
