#include "Component.h"


Component::Component(void)
{
}


Component::~Component(void)
{
    // Note: Not deleting the parent entity, as a component should not be responsible for this.
    m_Parent = 0;
}
