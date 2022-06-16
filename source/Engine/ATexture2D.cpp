#include "ATexture2D.h"

ATexture2D::ATexture2D() {
    m_nId = 0;
    m_nWidth = 0;
    m_nHeight = 0;
}

ATexture2D::~ATexture2D() {
    Delete();
}

void ATexture2D::SetID(glm::uint32 id) {
    m_nId = id;
}

void ATexture2D::Build(const void* p, glm::uint32 w, glm::uint32 h) {
    glGenTextures(1, &m_nId);
    glBindTexture(GL_TEXTURE_2D, m_nId);

    if (w != -1)
        m_nWidth = w;

    if (h != -1)
        m_nHeight = h;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, p);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ATexture2D::Update(const void* p, glm::uint32 x, glm::uint32 y, glm::uint32 w, glm::uint32 h) {
    glBindTexture(GL_TEXTURE_2D, m_nId);

    if (w != -1)
        m_nWidth = w;

    if (h != -1)
        m_nHeight = h;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, p);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ATexture2D::Delete() {
    glDeleteTextures(1, &m_nId);
}
