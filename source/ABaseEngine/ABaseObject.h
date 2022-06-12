#pragma once
#include "ABaseHeader.h"
#include "Common.h"

enum eBaseCallEvents {
    BASECALLEVENT_BEGINPLAY,
    BASECALLEVENT_UPDATE,
    BASECALLEVENT_RENDER,
    BASECALLEVENT_DRAW2D,
    BASECALLEVENT_DRAW2DDEBUG,
    BASECALLEVENT_LATEUPDATE,
    BASECALLEVENT_ENDPLAY,
    NUM_BASE_EVENTS
};

enum eObjectState {
    STATE_BEGIN,
    STATE_TICK,
    STATE_END
};

class ABaseObject {
public:
    friend class ABaseEngine;
    typedef ABaseObject Super;

    ABaseObject* m_pObject;
    std::string m_sClassName;
    glm::uint64 m_nId;
    glm::uint8 m_nState;
    bool m_bHeap;

public:
    ABaseObject();
    ~ABaseObject();

protected:
    inline virtual void BeginPlay() { }
    inline virtual void Update() { }
    inline virtual void LateUpdate() { }
    inline virtual void Render() { }
    inline virtual void Draw2D() { }
    inline virtual void Draw2DDebug() { }
    inline virtual void EndPlay() { }

private:
    void AddObjectToEventList();
    void RemoveObjectFromEventList();
    void CallEvent(glm::uint8 e);

public:
    bool IsValid();
    void SetState(eObjectState state);

public:
    static void Flush();
    static void Erase();

    void* operator new(size_t size) {
        void* p = ::operator new(size);
        ABaseObject* a = static_cast<ABaseObject*>(p);
        a->m_bHeap = true;
        return p;
    }

    void operator delete(void* p) {
        if (static_cast<ABaseObject*>(p)->m_bHeap)
            delete p;
    }
};

template<typename T, typename... Args>
static T* NewObject(Args... a) {
    T* t = new T(a...);
    return t;
}

template<typename T>
static void Delete(T* t) {
    t->SetState(STATE_END);
}

extern std::vector<ABaseObject*> vBaseObjects;
extern std::vector<ABaseObject*> vNewObjects;
extern std::vector<glm::uint64> vOldObjects;
