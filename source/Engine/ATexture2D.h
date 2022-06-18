#pragma once
#include "common.h"

class ATexture2D {
private:
    glm::uint32 m_nId;
    glm::int32 m_nWidth;
    glm::int32 m_nHeight;
    glm::int32 m_nInternalFormat;
    glm::int32 m_nFormat;

public:
    ATexture2D();
    ~ATexture2D();

    void SetID(glm::uint32 id);
    void Build(const void* p, glm::int32 w, glm::int32 h, glm::int32 internalFormat = -1, glm::int32 format = -1);

    void Update(const void* p, glm::uint32 x, glm::uint32 y, glm::uint32 w, glm::uint32 h, glm::int32 format = -1);
    void Delete();

public:
    glm::uint32 GetID() { return m_nId; }
    glm::uint32 GetWidth() { return m_nWidth; }
    glm::uint32 GetHeight() { return m_nHeight; }
};
