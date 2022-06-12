#pragma once
#include "ABaseObject.h"
#include "Common.h"
#include "Flipbook.h"
#include "Collision.h"
#include "Geometry.h"
#include "Sprite.h"

class CEntity : public ABaseObject {
private:
	glm::vec3 m_vPosition;
    glm::vec2 m_vScale;
    glm::vec4 m_vColor;
	std::shared_ptr<CFlipbook> m_pFlipbook;
    std::shared_ptr<CSprite> m_pSprite;

public:
    CEntity();

    void BeginPlay() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
    void EndPlay()  override;
};
