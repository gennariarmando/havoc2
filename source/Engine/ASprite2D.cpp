#include "ASprite2D.h"
#include "AScreen.h"
#include "AFileLoader.h"
#include "AGraphicDevice.h"
#include "AConsole.h"

std::unique_ptr<ATexture2D> ASprite2D::ms_pTexture = nullptr;
AVertexBuffer ASprite2D::ms_VertexBuffer = {};

ASprite2D::ASprite2D() {
	m_pTexture = std::make_unique<ATexture2D>();
}

ASprite2D::~ASprite2D() {
	m_pTexture.reset();
}

glm::mat4 ASprite2D::GetProjection() {
	glm::mat4 proj = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    return proj;
}

glm::mat4 ASprite2D::GetView() {
    glm::mat4 view = glm::mat4(1.0f);
    return view;
}

void ASprite2D::Delete() {
    if (m_pTexture)
        m_pTexture->Delete();
}

bool ASprite2D::SetTexture(glm::uint32 id) {
    m_pTexture->SetID(id);
    return true;
}

bool ASprite2D::SetTexture(std::string path, std::string name) {
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

void ASprite2D::Draw(float x, float y, float w, float h, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void ASprite2D::Draw(float x, float y, float w, float h, glm::vec2 const& center, float angle, glm::vec4 const& col) {
    Draw(glm::vec4(x, y, x + w, y + h), center, angle, col);
}

void ASprite2D::Draw(glm::vec4 const& rect, glm::vec4 const& col) {
    Draw(rect, glm::vec2(0.5f, 0.5f), 0.0f, col);
}

void ASprite2D::Draw(glm::vec4 const& rect, glm::vec2 const& center, float angle, glm::vec4 const& col) {
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

bool ASprite2D::Init() {
    glm::uint8 rectData[8 * 8 * 4];
    for (glm::uint32 i = 0; i < 8 * 8 * 4; i++) {
        rectData[i] = 255;
    }

    ms_pTexture = std::make_unique<ATexture2D>();
    ms_pTexture->Build(rectData, 8, 8);

    return true;
}

void ASprite2D::Shutdown() {
    ms_pTexture.reset();
}

void ASprite2D::DrawRect(glm::vec4 const& rect, glm::vec4 const& col) {
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
