#pragma once
#include "ABaseObject.h"
#include "Common.h"
#include "Flipbook.h"
#include "Collision.h"

class CEntity : public ABaseObject {
private:
	glm::vec3 m_vPosition;
	std::shared_ptr<CFlipbook> m_pFlipbook;

public:
	CEntity();
	~CEntity();

    virtual void Construct();
    //virtual void Init();
    //virtual void Update();
    //virtual void LateUpdate();
    //virtual void Render();
    //virtual void Draw2D();
    //virtual void Draw2DDebug();
    //virtual void Destruct();
    //virtual void Shutdown();
};
