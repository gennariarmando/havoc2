#pragma once
#include "Common.h"

#ifndef IMPLEMENT_PRIMARY_GAME_MODULE
#define IMPLEMENT_PRIMARY_GAME_MODULE(cl, str) cl::cl() { Super::ABaseObject(), this->m_sClassName = str, this->m_nId = BaseObjectsList.size(), BaseObjectsList.push_back(this), this->Construct(); } cl::~cl() { this->Destruct(); }
#endif

class ABaseObject {
public:
    typedef ABaseObject Super;

public:
    bool m_bInitialized;
    std::string m_sClassName;
    glm::uint32 m_nId;

public:
    ABaseObject();
    ~ABaseObject();
    virtual void Construct() { }
    virtual void Init() { }
    virtual void Update() { }
    virtual void LateUpdate() { }
    virtual void Render() { }
    virtual void Draw2D() { }
    virtual void Draw2DDebug() { }
    virtual void Shutdown() { }
    virtual void Destruct() { }
};

extern std::list<ABaseObject*> BaseObjectsList;
