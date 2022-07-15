#include "AVertexBuffer.h"
#include "AGraphicDevice.h"
#include "AConsole.h"
#include "ACamera.h"

AVertexBuffer::AVertexBuffer() {
    m_nPrimitive = GL_TRIANGLES;
    m_nVao = 0;
    m_nVbo[VBO_POS] = 0;
    m_nVbo[VBO_UV] = 0;

    m_vPos = {};
    m_vTexCoords = {};

    m_mProjection = {};
    m_mView = {};
    m_mTransform = {};
    m_vColor = {};
    m_nTexture = 0;
    m_bHasRenderData = false;
    m_pShader = nullptr;
    m_bUpdateRenderData = false;
}

void AVertexBuffer::SetPrimitive(glm::uint8 p) {
    if (m_bHasRenderData)
        return;

    m_nPrimitive = p;
}

void AVertexBuffer::SetProjection(glm::mat4 mat) {
    m_mProjection = mat;
}

void AVertexBuffer::SetView(glm::mat4 mat) {
    m_mView = mat;
}

void AVertexBuffer::SetLocation(float x, float y, float z) {
    m_mTransform = glm::mat4(1.0f);
    m_mTransform = glm::translate(m_mTransform, glm::vec3(x, y, z));
}

void AVertexBuffer::SetLocation(glm::vec3 pos) {
    SetLocation(pos.x, pos.y, pos.z);
}

void AVertexBuffer::Translate(float x, float y, float z) {
    m_mTransform = translate(m_mTransform, glm::vec3(x, y, z));
}

void AVertexBuffer::SetScale(float x, float y, float z) {
    m_mTransform = glm::scale(m_mTransform, glm::vec3(x, y, z));
}

void AVertexBuffer::SetScale(glm::vec3 scale) {
    m_mTransform = glm::scale(m_mTransform, scale);
}

void AVertexBuffer::SetRotation(float x, float y, float z, float angle) {
    m_mTransform = glm::rotate(m_mTransform, angle, glm::vec3(x, y, z));
}

void AVertexBuffer::SetRotation(glm::vec3 rot, float angle) {
    SetRotation(rot.x, rot.y, rot.z, angle);
}

void AVertexBuffer::SetColor(float r, float g, float b, float a) {
    m_vColor = glm::vec4(r, g, b, a);
}

void AVertexBuffer::SetVertex(float x, float y, float z) {
    if (m_bHasRenderData)
        return;

    if (m_vTexCoords.size() != m_vPos.size())
        m_vTexCoords.push_back({ x, y });

    m_vPos.push_back({ x , y, z });
}

void AVertexBuffer::SetVertex(glm::vec3 const& pos) {
    SetVertex(pos.x, pos.y, pos.z);
}

void AVertexBuffer::SetIndices(glm::uint32 i) {
    m_vIndices.push_back(i);
}

void AVertexBuffer::SetTexCoords(float x, float y) {
    if (m_bHasRenderData)
        return;

    m_vTexCoords.push_back({ x, y });
}

void AVertexBuffer::SetTexCoords(glm::vec2 const& pos) {
    SetTexCoords(pos.x, pos.y);
}

void AVertexBuffer::EditTexCoords(glm::int32 index, float x, float y) {
    if (static_cast<glm::int32>(m_vTexCoords.size()) > index)
        m_vTexCoords.at(index) = { x, y };
}

void AVertexBuffer::EditTexCoords(glm::int32 index, glm::vec2 const& pos) {
    EditTexCoords(index, pos.x, pos.y);
}

void AVertexBuffer::SetTexture(glm::uint32 tex) {
    m_nTexture = tex;
}

void AVertexBuffer::SetShader(AShader* shader) {
    m_pShader = shader;
}

void AVertexBuffer::SetUpdateData(bool on) {
    m_bUpdateRenderData = on;
}

void AVertexBuffer::Setup() {
    if (m_bHasRenderData)
        return;

    if (m_vPos.size() > 0 && m_vTexCoords.size() > 0) {
        glGenVertexArrays(1, &m_nVao);
        glGenBuffers(NUM_OBJECTS, m_nVbo);

        glBindVertexArray(m_nVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_POS]);
        glBufferData(GL_ARRAY_BUFFER, m_vPos.size() * sizeof(glm::vec3), &m_vPos.at(0), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_UV]);
        glBufferData(GL_ARRAY_BUFFER, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords.at(0), GL_DYNAMIC_DRAW);

        m_bHasRenderData = true;
    }
}

void AVertexBuffer::Update() {
    if (!m_bUpdateRenderData)
        return;

    if (m_nVbo[VBO_UV] && m_vTexCoords.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_nVbo[VBO_UV]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vTexCoords.size() * sizeof(glm::vec2), &m_vTexCoords.at(0));
    }
}

void AVertexBuffer::RenderPrimitives() {
    Setup();
    Update();

    if (m_pShader) {
        m_pShader->Apply();

        // vs
        m_pShader->SetShaderUniform("color", m_vColor);

        // fs
        m_pShader->SetShaderUniform("projection", m_mProjection);
        m_pShader->SetShaderUniform("view", m_mView);
        m_pShader->SetShaderUniform("model", m_mTransform);

        m_pShader->SetShaderUniform("nearClip", Camera.GetNearClip());
        m_pShader->SetShaderUniform("farClip", Camera.GetFarClip());
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);

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
