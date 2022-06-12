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

public:
    ABaseObject* m_pObject;
    std::string m_sClassName;
    glm::uint64 m_nId;
    glm::uint8 m_nState;

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
    void CallEvent(glm::uint8 e);

public:
    bool IsValid();

public:
    static void Flush();
};

extern std::vector<ABaseObject*> baseObjects;
extern std::vector<ABaseObject*> newObjects;
extern std::vector<glm::uint64> oldObjects;

template<typename T>
static T* NewObject() {
    T* t = new T();
    return t;
}

template<typename T>
static void Delete(T* t) {
    t->m_nState = STATE_END;
}
