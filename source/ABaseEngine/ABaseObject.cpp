#include "ABaseObject.h"
#include "ABaseEngine.h"

std::vector<ABaseObject*> vBaseObjects;
std::vector<ABaseObject*> vNewObjects;
std::vector<glm::uint64> vOldObjects;

ABaseObject::ABaseObject() {
    m_pObject = this;
    m_sClassName = typeid(this).name();
    m_nId = 0;
    m_nState = STATE_BEGIN;
    m_bHeap = false;
    AddObjectToEventList();
}

ABaseObject::~ABaseObject() {
    RemoveObjectFromEventList();
}

void ABaseObject::AddObjectToEventList() {
    m_nId = vBaseObjects.size();
    vNewObjects.push_back(this);
}

void ABaseObject::RemoveObjectFromEventList() {
    vOldObjects.push_back(m_nId);
}

void ABaseObject::CallEvent(glm::uint8 e) {
    if (!m_pObject)
        return;

    switch (e) {
    case BASECALLEVENT_BEGINPLAY:
        if (m_nState == STATE_BEGIN) {
            m_nState = STATE_TICK;
            BeginPlay();
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

void ABaseObject::SetState(eObjectState state) {
    m_nState = state;
}

void ABaseObject::Flush() {
    for (auto& it : vNewObjects) {
        if (it->IsValid())
            vBaseObjects.push_back(it);
    }

    if (vBaseObjects.size() > 1) {
        for (auto it : vOldObjects) {
            if (it < vBaseObjects.size())
                vBaseObjects.erase(vBaseObjects.begin() + it);
        }
    }

    if (vOldObjects.size() > 0) {
        vOldObjects.clear();
        vOldObjects.shrink_to_fit();
    }

    if (vNewObjects.size() > 0) {
        vNewObjects.clear();
        vNewObjects.shrink_to_fit();
    }
}

void ABaseObject::Erase() {
    for (glm::uint32 i = 0; i < vBaseObjects.size(); i++)
        if (vBaseObjects.at(i)->IsValid()) {
            Delete<ABaseObject>(vBaseObjects.at(i));
            vBaseObjects.at(i)->CallEvent(BASECALLEVENT_ENDPLAY);
        }
}
