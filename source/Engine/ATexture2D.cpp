#include "ATexture2D.h"

ATexture2D::ATexture2D() {
    m_nId = 0;
    m_nWidth = 0;
    m_nHeight = 0;
    m_nInternalFormat = GL_RGBA;
    m_nFormat = GL_BGRA;
}

ATexture2D::~ATexture2D() {
    Delete();
}

void ATexture2D::SetID(glm::uint32 id) {
    m_nId = id;
}

void ATexture2D::Build(const void* p, glm::int32 w, glm::int32 h, glm::int32 internalFormat, glm::int32 format) {
    glGenTextures(1, &m_nId);
    glBindTexture(GL_TEXTURE_2D, m_nId);

    if (w != -1)
        m_nWidth = w;

    if (h != -1)
        m_nHeight = h;

    if (internalFormat != -1)
        m_nInternalFormat = internalFormat;

    if (format != -1)
        m_nFormat = format;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, m_nInternalFormat, m_nWidth, m_nHeight, 0, m_nFormat, GL_UNSIGNED_BYTE, p);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ATexture2D::Update(const void* p, glm::uint32 x, glm::uint32 y, glm::uint32 w, glm::uint32 h, glm::int32 format) {
    glBindTexture(GL_TEXTURE_2D, m_nId);

    if (w != -1)
        m_nWidth = w;

    if (h != -1)
        m_nHeight = h;

    if (format != -1)
        m_nFormat = h;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, m_nFormat, GL_UNSIGNED_BYTE, p);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ATexture2D::Delete() {
    glDeleteTextures(1, &m_nId);
}
