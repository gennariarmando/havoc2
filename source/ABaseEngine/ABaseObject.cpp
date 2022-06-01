#include "ABaseObject.h"

std::list<ABaseObject*> BaseObjectsList = { 0 };

ABaseObject::ABaseObject() {
    m_bInitialized = false;
    m_sClassName = "ABaseObject";
}

ABaseObject::~ABaseObject() {

}
