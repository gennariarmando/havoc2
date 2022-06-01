#include "Geometry.h"
#include "Camera.h"
#include "ABaseGL.h"

glm::uint32 CGeometry::m_nVbo;
glm::int32 CGeometry::m_nFilter;

CGeometry::CGeometry() {
    Clear();
}

CGeometry::~CGeometry() {
    if (m_nVao)
        glDeleteVertexArrays(1, &m_nVao);
}

void CGeometry::Setup() {
    if (m_bHasRenderData)
        return;

    if (m_vVertices.size() <= 0)
        return;

    glGenVertexArrays(1, &m_nVao);
    glGenBuffers(1, &m_nVbo);

    glBindVertexArray(m_nVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVbo);
    glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(CVertex), &m_vVertices.at(0), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, m_vPos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, m_vNormals));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, m_vTexCoords));
    glEnableVertexAttribArray(2);

    m_bHasRenderData = true;
}

void CGeometry::Clear() {
    m_nPrimitive = GL_TRIANGLES;

    if (m_nVbo)
        glDeleteBuffers(1, &m_nVbo);

    m_nVao = 0;
    m_nVbo = 0;

    m_vVertices = {};

    m_mTransform = glm::mat4(1.0f);
    m_mTransform = glm::translate(m_mTransform, glm::vec3(0.0f, 0.0f, 0.0f));
    m_mTransform = glm::scale(m_mTransform, glm::vec3(1.0f, 1.0f, 1.0f));
    m_mTransform = glm::rotate(m_mTransform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    m_vColor = glm::vec4(1.0f);
    m_nTexture = 0;
    m_nFilter = GL_NEAREST;
    m_bHasRenderData = false;
    m_Shader = NULL;
}

void CGeometry::Update() {
    if (m_nVbo) {
        glBindBuffer(GL_ARRAY_BUFFER, m_nVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vVertices.size() * sizeof(CVertex), &m_vVertices.at(0));
    }
}

void CGeometry::RenderPrimitives() {
    Setup();
    Update();

    if (!m_bHasRenderData)
        return;

    if (!m_Shader)
        m_Shader = ABaseGL::GetDefaultShader().get();

    m_Shader->Apply();

    // vs
    m_Shader->SetShaderUniform("color", m_vColor);

    // fs
    m_Shader->SetShaderUniform("projection", Camera.GetProjection());
    m_Shader->SetShaderUniform("view", Camera.GetView());
    m_Shader->SetShaderUniform("model", m_mTransform);

    m_Shader->SetShaderUniform("nearClip", Camera.GetNearClip());
    m_Shader->SetShaderUniform("farClip", Camera.GetFarClip());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nFilter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nFilter);

    glBindVertexArray(m_nVao);
    glDrawArrays(m_nPrimitive, 0, static_cast<glm::uint32>(m_vVertices.size()));
    glBindVertexArray(0);
}

void CGeometry::SetPrimitive(glm::uint8 p) {
    if (m_bHasRenderData)
        return;

    m_nPrimitive = p;
}

void CGeometry::SetLocation(float x, float y, float z) {
    m_mTransform = glm::mat4(1.0f);
    m_mTransform = glm::translate(m_mTransform, glm::vec3(x, y, z));
}

void CGeometry::Translate(float x, float y, float z) {
    m_mTransform = translate(m_mTransform, glm::vec3(x, y, z));
}

void CGeometry::SetScale(float x, float y, float z) {
    m_mTransform = glm::scale(m_mTransform, glm::vec3(x, y, z));
}

void CGeometry::SetRotation(float x, float y, float z, float angle) {
    m_mTransform = glm::rotate(m_mTransform, angle, glm::vec3(x, y, z));
}

void CGeometry::SetColor(float r, float g, float b, float a) {
    m_vColor = glm::vec4(r, g, b, a);
}

void CGeometry::SetVertex(float x, float y, float z) {
    if (m_bHasRenderData)
        return;

    CVertex v;
    v.m_vPos = glm::vec3(x, y, z);
    m_vVertices.push_back(v);
}

void CGeometry::SetVertex(glm::vec3 const& pos) {
    SetVertex(pos.x, pos.y, pos.z);
}

void CGeometry::SetTexCoords(float x, float y) {
    if (m_bHasRenderData)
        return;

    CVertex v;
    glm::int32 index = static_cast<glm::int32>(m_vVertices.size()) - 1;
    v.m_vPos = m_vVertices.at(index).m_vPos;
    v.m_vTexCoords = glm::vec2(x, y);
    m_vVertices.at(index) = v;
}

void CGeometry::SetTexCoords(glm::vec2 const& pos) {
    SetTexCoords(pos.x, pos.y);
}

void CGeometry::SetTexture(glm::uint32 tex) {
    m_nTexture = tex;
}

void CGeometry::SetTextureFilter(glm::int32 f) {
    m_nFilter = f;
}

void CGeometry::SetShader(CShader* s) {
    m_Shader = s;
}
