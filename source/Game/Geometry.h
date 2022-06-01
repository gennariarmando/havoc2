#pragma once
#define MAX_BONE_INFLUENCE 4
#include "Common.h"
#include "Shader.h"
#include "ABaseObject.h"

class CVertex {
public:
    glm::vec3 m_vPos;
    glm::vec3 m_vNormals;
    glm::vec2 m_vTexCoords;
};

class CGeometry {
private:
    static glm::uint32 m_nVbo;
    static glm::int32 m_nFilter;

    glm::uint8 m_nPrimitive;
    glm::uint32 m_nVao;
    std::vector<CVertex> m_vVertices;

    glm::mat4 m_mTransform;
    glm::vec4 m_vColor;
    glm::uint32 m_nTexture;

    bool m_bHasRenderData;

    CShader* m_Shader;

public:
    CGeometry();
    ~CGeometry();
    void Setup();
    void RenderPrimitives();
    void Clear();
    void Update();

public:
    void SetPrimitive(glm::uint8 mode);
    void SetLocation(float x, float y, float z);
    void Translate(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float x, float y, float z, float angle);
    void SetColor(float r, float g, float b, float a);

    void SetVertex(float x, float y, float z = 0.0f);
    void SetVertex(glm::vec3 const& pos);
    void SetTexCoords(float x, float y);
    void SetTexCoords(glm::vec2 const& pos);
    void SetTexture(glm::uint32 tex);
    void SetShader(CShader* shader);

public:
    static void SetTextureFilter(glm::int32 f);
    
public:
    std::vector<CVertex>& GetVertices() { return m_vVertices; }
    glm::uint32& GetTexture() { return m_nTexture; }

public:
    const glm::mat4& GetTransform() { return m_mTransform; }
};
