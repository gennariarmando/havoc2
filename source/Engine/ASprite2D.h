#pragma once
#include "common.h"
#include "ATexture2D.h"
#include "AVertexBuffer.h"

class ASprite2D {
public:
	std::unique_ptr<ATexture2D> m_pTexture;
    AVertexBuffer m_VertexBuffer;

public:
    static std::unique_ptr<ATexture2D> ms_pTexture;
    static AVertexBuffer ms_VertexBuffer;

public:
    ASprite2D();
    ~ASprite2D();

	void Delete();
    bool SetTexture(glm::uint32 id);
    bool SetTexture(std::string path, std::string name);
    void Draw(float x, float y, float w, float h, glm::vec4 const& col);
    void Draw(float x, float y, float w, float h, glm::vec2 const& center, float angle, glm::vec4 const& col);

    void Draw(glm::vec4 const& rect, glm::vec4 const& col);
    void Draw(glm::vec4 const& rect, glm::vec2 const& center, float angle, glm::vec4 const& col);

public:
    static glm::mat4 GetProjection();
    static glm::mat4 GetView();
    static bool Init();
    static void Shutdown();
    static void DrawRect(glm::vec4 const& rect, glm::vec4 const& col);
};
