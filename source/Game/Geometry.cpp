#include "Geometry.h"
#include "Camera.h"
#include "ABaseGL.h"

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

    glGenVertexArrays(1, &m_nVao);
    glGenBuffers(NUM_OBJECTS, m_nVbo);

    glBindVertexArray(m_nVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_POS]);
    glBufferData(GL_ARRAY_BUFFER, m_vPos.size() * sizeof(glm::vec3), &m_vPos.at(0), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_UV]);
    glBufferData(GL_ARRAY_BUFFER, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords.at(0), GL_DYNAMIC_DRAW);

    m_bHasRenderData = true;
}

void CGeometry::Clear() {
    if (!ABaseGL::GetWindow())
        return;

    m_nPrimitive = GL_TRIANGLES;

    if (m_nVbo)
        glDeleteBuffers(NUM_OBJECTS, m_nVbo);

    m_vPos = {};
    m_vTexCoords = {};

    m_nVao = 0;

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
   if (m_nVbo[VBO_UV]) {
       glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_UV]);
       glBufferSubData(GL_ARRAY_BUFFER, 0, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords.at(0));
   }
}

void CGeometry::Render() {
    Setup();
    Update();

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

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_POS]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_UV]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(m_nVao);
    glDrawArrays(m_nPrimitive, 0, static_cast<glm::uint32>(m_vPos.size()));
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

void CGeometry::SetLocation(glm::vec3 pos) {
    m_mTransform = glm::mat4(1.0f);
    m_mTransform = glm::translate(m_mTransform, pos);
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

    if (m_vTexCoords.size() != m_vPos.size())
        m_vTexCoords.push_back({ x, y });

    m_vPos.push_back({ x , y, z });
}

void CGeometry::SetVertex(glm::vec3 const& pos) {
    SetVertex(pos.x, pos.y, pos.z);
}

void CGeometry::SetTexCoords(float x, float y) {
    if (m_bHasRenderData)
        return;

    m_vTexCoords.push_back({ x, y });
}

void CGeometry::EditTexCoords(glm::int32 index, float x, float y) {
    m_vTexCoords.at(index) = { x, y };
}

void CGeometry::EditTexCoords(glm::int32 index, glm::vec2 const& pos) {
    EditTexCoords(index, pos.x, pos.y);
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
