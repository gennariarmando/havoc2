#pragma once
#include "common.h"

class ATexture2D {
private:
    glm::uint32 m_nId;
    glm::uint32 m_nWidth;
    glm::uint32 m_nHeight;

public:
    ATexture2D();
    ~ATexture2D();

    void SetID(glm::uint32 id);
    void Build(const void* p, glm::uint32 w, glm::uint32 h);
    void Update(const void* p, glm::uint32 x, glm::uint32 y, glm::uint32 w, glm::uint32 h);
    void Delete();

public:
    glm::uint32 const& GetID() { return m_nId; }
    glm::uint32 const& GetWidth() { return m_nWidth; }
    glm::uint32 const& GetHeight() { return m_nHeight; }
};
