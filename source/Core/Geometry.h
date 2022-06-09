#pragma once
#define MAX_BONE_INFLUENCE 4
#include "Common.h"
#include "Shader.h"
#include "ABaseObject.h"
#include "Collision.h"

enum eBufferObjects {
    VBO_POS,
    VBO_UV,
    NUM_OBJECTS,
};

class CGeometry {
public:
    glm::uint8 m_nPrimitive;
    glm::uint32 m_nVao;
    glm::uint32 m_nVbo[NUM_OBJECTS];
    std::vector<glm::vec3> m_vPos;
    std::vector<glm::vec2> m_vTexCoords;
    glm::int32 m_nFilter;
    glm::mat4 m_mTransform;
    glm::vec4 m_vColor;
    glm::uint32 m_nTexture;

    bool m_bHasRenderData;

    std::shared_ptr<CShader> m_pShader;
    std::shared_ptr<CCollision> m_Collision;

private:
    void Setup();
    void Update();

public:
    CGeometry();
    ~CGeometry();
    void Clear();
    void Render();

    void SetPrimitive(glm::uint8 mode);
    void SetLocation(float x, float y, float z);
    void SetLocation(glm::vec3 pos);
    void Translate(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float x, float y, float z, float angle);
    void SetColor(float r, float g, float b, float a);

    void SetVertex(float x, float y, float z = 0.0f);
    void SetVertex(glm::vec3 const& pos);
    void SetTexCoords(float x, float y);
    void SetTexCoords(glm::vec2 const& pos);
    void EditTexCoords(glm::int32 index, float x, float y);
    void EditTexCoords(glm::int32 index, glm::vec2 const& pos);

    void SetTexture(glm::uint32 tex);
    void SetShader(std::shared_ptr<CShader> shader);

public:
    void SetTextureFilter(glm::int32 f);
    
    
public:
    std::vector<glm::vec3>& GetPos() { return m_vPos; }
    std::vector<glm::vec2>& GetTexCoords() { return m_vTexCoords; }

    glm::uint32& GetTexture() { return m_nTexture; }

public:
    const glm::mat4& GetTransform() { return m_mTransform; }
};
