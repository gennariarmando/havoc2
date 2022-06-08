#pragma once
#include "ABaseObject.h"
#include "Geometry.h"

class CTexture2D {
private:
    glm::uint32 m_nId;
    glm::int32 m_nWidth;
    glm::int32 m_nHeight;
    glm::int32 m_nChannels;

public:
    CTexture2D() {
        m_nId = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_nChannels = 0;
    }

    ~CTexture2D() {
        glDeleteTextures(1, &m_nId);
    }

    glm::uint32 GetFormat(bool flip, glm::int32 channels) {
        switch (channels) {
        case 3:
            return flip ? GL_BGR : GL_RGB;
        case 4:
            return flip ? GL_BGRA : GL_RGBA;
        }
    }

    void Build(void* p, glm::int32 w, glm::int32 h, glm::int32 c, glm::int32 filter) {
        glGenTextures(1, &m_nId);
        glBindTexture(GL_TEXTURE_2D, m_nId);

        if (w != -1)
            m_nWidth = w;

        if (h != -1)
            m_nHeight = h;

        if (c != -1)
            m_nChannels = c;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , m_nWidth, m_nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, p);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Update(void* p, glm::int32 x, glm::int32 y, glm::int32 w, glm::int32 h, glm::int32 c, glm::int32 filter) {
        glBindTexture(GL_TEXTURE_2D, m_nId);

        if (w != -1)
            m_nWidth = w;

        if (h != -1)
            m_nHeight = h;

        if (c != -1)
            m_nChannels = c;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, p);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

public:
    glm::uint32& GetID() { return m_nId; }
    glm::int32& GetWidth() { return m_nWidth; }
    glm::int32& GetHeight() { return m_nHeight; }
    glm::int32& GetChannels() { return m_nChannels; }
};

class CSprite2D {
private:
    std::unique_ptr<CTexture2D> m_pTexture;
    CGeometry m_Geometry;

public:
    static CGeometry ms_Geometry;

public:
    CSprite2D();
    CSprite2D(glm::uint32 id);
    CSprite2D(std::string path, std::string name);
    ~CSprite2D();

public:
    bool SetTexture(glm::uint32 id);
    bool SetTexture(std::string path, std::string name);
    void Draw(float x, float y, float w, float h, glm::vec4 const& col);
    void Draw(float x, float y, float w, float h, glm::vec2 const& center, float angle, glm::vec4 const& col);

    void Draw(glm::vec4 const& rect, glm::vec4 const& col);
    void Draw(glm::vec4 const& rect, glm::vec2 const& center, float angle, glm::vec4 const& col);

public:
    static void DrawRect(glm::vec4 const& rect, glm::vec4 const& col);

public:
    std::unique_ptr<CTexture2D>& GetTexture() { return m_pTexture; }

};
