#include "ASprite.h"
#include "AScreen.h"
#include "AFileLoader.h"
#include "AGraphicDevice.h"
#include "AConsole.h"
#include "ACamera.h"

ATexture2D* ASprite::ms_pTexture = nullptr;
AVertexBuffer ASprite::ms_VertexBuffer = {};

ASprite::ASprite() {
    m_pTexture = new ATexture2D();
    m_VertexBuffer = {};
}

ASprite::~ASprite() {
	delete m_pTexture;
}

glm::mat4 ASprite::GetProjection() {
	glm::mat4 proj = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    return proj;
}

glm::mat4 ASprite::GetView() {
    glm::mat4 view = glm::mat4(1.0f);
    return view;
}

void ASprite::Delete() {
    if (m_pTexture)
        m_pTexture->Delete();
}

bool ASprite::SetTexture(glm::uint32 id) {
    m_pTexture->SetID(id);
    return true;
}

bool ASprite::SetTexture(std::string path, std::string name) {
    std::string file = path + "/" + name + ".tga";
    glm::uint16 w;
    glm::uint16 h;
    std::shared_ptr<glm::uint8[]> data;

    if (AFileLoader::LoadTGAFromFile(file, w, h, data)) {
        m_pTexture->Build(data.get(), w, h);
    }
    else {
        return false;
    }

    return true;
}

void ASprite::Draw(float x, float y, float w, float h, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void ASprite::Draw(float x, float y, float w, float h, glm::vec2 const& center, float angle, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), center, angle, col);
}

void ASprite::Draw(glm::vec4 const& rect, glm::vec4 const& col) {
    Draw(rect, glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void ASprite::Draw(glm::vec4 const& rect, glm::vec2 const& center, float angle, glm::vec4 const& col) {
    m_VertexBuffer.SetPrimitive(GL_TRIANGLES);
    m_VertexBuffer.SetProjection(GetProjection());
    m_VertexBuffer.SetView(GetView());

    glm::vec2 c;
    c.x = ((rect.z - rect.x)) * center.x;
    c.y = ((rect.w - rect.y)) * center.y;
    m_VertexBuffer.SetLocation(rect.x, rect.y, 0.0f);
    m_VertexBuffer.Translate(c.x, c.y, 0.0f);
    m_VertexBuffer.SetRotation(0.0f, 0.0f, 1.0f, angle);
    m_VertexBuffer.Translate(-c.x, -c.y, 0.0f);

    m_VertexBuffer.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);
    m_VertexBuffer.SetColor(col.r, col.g, col.b, col.a);

    m_VertexBuffer.SetVertex(0.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 0.0f);

    m_VertexBuffer.SetVertex(0.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 0.0f);

    m_VertexBuffer.SetVertex(0.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 0.0f);

    m_VertexBuffer.SetTexture(m_pTexture->GetID());
    m_VertexBuffer.SetShader(GraphicDevice.m_vShaders.at(SHADER_SPRITES));

    m_VertexBuffer.RenderPrimitives();
}

void ASprite::Draw(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec3 const& rotation, float angle, glm::vec4 const& col) {
    m_VertexBuffer.SetPrimitive(GL_TRIANGLES);
    m_VertexBuffer.SetProjection(Camera.GetProjection());
    m_VertexBuffer.SetView(Camera.GetView());

    glm::vec2 v = { m_pTexture->GetWidth(), m_pTexture->GetHeight() };

    float targetAR = 16.0f / 16.0f;
    float imageAR = v.y / v.x;

    v.x = roundf(16.0f / imageAR * targetAR);
    v.y = 16.0f;

    v.x *= scale.x / 64.0f;
    v.y *= scale.y / 64.0f;

    m_VertexBuffer.SetLocation(pos);
    glm::vec2 c;
    c.x = v.x / 2.0f;
    c.y = v.y / 2.0f;
    m_VertexBuffer.Translate(c.x, c.y, 0.0f);
    m_VertexBuffer.SetRotation(rotation.x, rotation.y, rotation.z, angle);
    m_VertexBuffer.Translate(-c.x, -c.y, 0.0f);

    m_VertexBuffer.SetScale(v.x, v.y, 0.0f);
    m_VertexBuffer.SetColor(col.r, col.g, col.b, col.a);

    m_VertexBuffer.SetVertex(0.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 0.0f);

    m_VertexBuffer.SetVertex(0.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 0.0f);

    m_VertexBuffer.SetVertex(0.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(0.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 1.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 1.0f);

    m_VertexBuffer.SetVertex(1.0f, 0.0f);
    m_VertexBuffer.SetTexCoords(1.0f, 0.0f);

    m_VertexBuffer.SetTexture(m_pTexture->GetID());
    m_VertexBuffer.SetShader(GraphicDevice.m_vShaders.at(SHADER_SPRITES));

    m_VertexBuffer.RenderPrimitives();
}

bool ASprite::Init() {
    glm::uint8 rectData[8 * 8 * 4];
    for (glm::uint32 i = 0; i < 8 * 8 * 4; i++) {
        rectData[i] = 255;
    }

    ms_pTexture = new ATexture2D();
    ms_pTexture->Build(rectData, 8, 8);

    return true;
}

void ASprite::Shutdown() {
    delete ms_pTexture;
}

void ASprite::DrawRect(glm::vec4 const& rect, glm::vec4 const& col) {
    ms_VertexBuffer.SetPrimitive(GL_TRIANGLES);
    ms_VertexBuffer.SetProjection(GetProjection());
    ms_VertexBuffer.SetView(GetView());

    ms_VertexBuffer.SetLocation(rect.x, rect.y, 0.0f);
    ms_VertexBuffer.SetScale(rect.z - rect.x, rect.w - rect.y, 1.0f);
    ms_VertexBuffer.SetColor(col.r, col.g, col.b, col.a);

    ms_VertexBuffer.SetVertex(0.0f, 1.0f);
    ms_VertexBuffer.SetVertex(1.0f, 0.0f);
    ms_VertexBuffer.SetVertex(0.0f, 0.0f);
    ms_VertexBuffer.SetVertex(0.0f, 1.0f);
    ms_VertexBuffer.SetVertex(1.0f, 1.0f);
    ms_VertexBuffer.SetVertex(1.0f, 0.0f);

    ms_VertexBuffer.SetTexture(ms_pTexture->GetID());
    ms_VertexBuffer.SetShader(GraphicDevice.m_vShaders.at(SHADER_SPRITES));
    ms_VertexBuffer.RenderPrimitives();
}
