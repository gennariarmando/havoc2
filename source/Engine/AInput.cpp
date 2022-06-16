#include "AInput.h"

AInput Input;

AInput::AInput() {
    Clear();
}

void AInput::Clear() {
    m_TempMouse = {};
    m_OldMouse = {};
    m_NewMouse = {};

    m_TempKeyboard = {};
    m_OldKeyboard = {};
    m_NewKeyboard = {};
}

void AInput::BeginFrame() {
    m_OldKeyboard = m_NewKeyboard;
    m_NewKeyboard = m_TempKeyboard;

    m_OldMouse = m_NewMouse;
    m_NewMouse = m_TempMouse;

    if (Abs(glm::distance(m_OldMouse.pos, m_NewMouse.pos)) > 0.0f) {
        m_NewMouse.delta.x = m_NewMouse.pos.x - m_OldMouse.pos.x;
        m_NewMouse.delta.y = m_OldMouse.pos.y - m_NewMouse.pos.y;

        m_OldMouse = m_NewMouse;
    }
}

void AInput::EndFrame() {
    ;;
}

bool AInput::GetKeyDown(glm::int32 key) {
    return m_NewKeyboard.key[key];
}

bool AInput::GetKeyJustDown(glm::int32 key) {
    return m_NewKeyboard.key[key] && !m_OldKeyboard.key[key];
}

bool AInput::GetKeyJustUp(glm::int32 key) {
    return !m_NewKeyboard.key[key] && m_OldKeyboard.key[key];
}
