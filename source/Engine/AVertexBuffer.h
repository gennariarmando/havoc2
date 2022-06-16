#pragma once
#include "AShader.h"

enum eBufferObjects {
    VBO_POS,
    VBO_UV,
    NUM_OBJECTS,
};

class AVertexBuffer {
public:
    glm::uint8 m_nPrimitive;
    glm::uint32 m_nVao;
    glm::uint32 m_nVbo[NUM_OBJECTS];
    std::vector<glm::vec3> m_vPos;
    std::vector<glm::vec2> m_vTexCoords;

    glm::mat4 m_mProjection;
    glm::mat4 m_mView;
    glm::mat4 m_mTransform;
    glm::vec4 m_vColor;
    glm::uint32 m_nTexture;
    bool m_bHasRenderData;

    AShader* m_pShader;

public:
	AVertexBuffer();

    void SetPrimitive(glm::uint8 mode);
    void SetProjection(glm::mat4 mat);
    void SetView(glm::mat4 mat);
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
    void SetShader(AShader* shader);

    void RenderPrimitives();

private:
    void Setup();
    void Update();

};
