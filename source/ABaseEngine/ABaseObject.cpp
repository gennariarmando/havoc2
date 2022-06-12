#include "ABaseObject.h"

std::vector<ABaseObject*> baseObjects;
std::vector<ABaseObject*> newObjects;
std::vector<glm::uint64> oldObjects;

ABaseObject::ABaseObject() {
    m_pObject = this;
    m_sClassName = typeid(this).name();
    m_nId = baseObjects.size();
    m_nState = STATE_BEGIN;
    newObjects.push_back(this);
}

ABaseObject::~ABaseObject() {
    oldObjects.push_back(m_nId);
}

void ABaseObject::CallEvent(glm::uint8 e) {
    if (!m_pObject)
        return;

    switch (e) {
    case BASECALLEVENT_BEGINPLAY:
        if (m_nState == STATE_BEGIN) {
            BeginPlay();
            m_nState = STATE_TICK;
        }
        break;
    case BASECALLEVENT_UPDATE:
        if (m_nState == STATE_TICK)
            Update();
        break;
    case BASECALLEVENT_RENDER:
        if (m_nState == STATE_TICK)
            Render();
        break;
    case BASECALLEVENT_DRAW2D:
        if (m_nState == STATE_TICK)
            Draw2D();
        break;
    case BASECALLEVENT_DRAW2DDEBUG:
        if (m_nState == STATE_TICK)
            Draw2DDebug();
        break;
    case BASECALLEVENT_LATEUPDATE:
        if (m_nState == STATE_TICK)
            LateUpdate();
        break;
    case BASECALLEVENT_ENDPLAY:
        if (m_nState == STATE_END) {
            EndPlay();
            delete this;
        }
        break;
    }
}

bool ABaseObject::IsValid() {
    return m_pObject != nullptr;
}

void ABaseObject::Flush() {
    for (auto& it : newObjects) {
        if (it->IsValid())
            baseObjects.push_back(it);
    }

    if (baseObjects.size() > 1) {
        for (auto it : oldObjects) {
            if (it < baseObjects.size())
                baseObjects.erase(baseObjects.begin() + it);
        }
    }

    if (oldObjects.size() > 0) {
        oldObjects.clear();
        oldObjects.shrink_to_fit();
    }

    if (newObjects.size() > 0) {
        newObjects.clear();
        newObjects.shrink_to_fit();
    }
}
