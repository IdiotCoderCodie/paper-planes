#include <cassert>

#include "Entity.h"


Entity::Entity(Scene& parent, const entityId_t& id)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(),
        m_Components()
{
}


Entity::Entity(Scene& parent, const entityId_t& id, Frame& transform)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(transform),
        m_Components()
{
}


Entity::Entity(Scene& parent, const entityId_t& id, const glm::vec3& pos, const glm::vec3& forward, 
               const glm::vec3& up)
    :   m_Parent(parent),
        m_id(id),
        m_Transform(pos, forward, up),
        m_Components()
{
}


Entity::~Entity(void)
{
    try
    {
        ClearComponents();
    }
    catch (int e)
    {
        // Exception thrown trying to clear components.
        assert(false);
    }
}


Component* Entity::GetComponent(const componentId_t& familyId)
{
    if(m_Components.find(familyId) != m_Components.end())
    {
        return m_Components.find(familyId)->second;
    }
    return 0;
}

Component* Entity::GetComponent(const componentId_t& familyId, const componentId_t& componentId)
{
    if(m_Components.find(familyId) != m_Components.end())
    {
        std::string compId;
        m_Components.find(familyId)->second->ComponentID(compId);
        if(compId.compare(componentId) == 0)
        {
            // This is a component of the asks type.
            return m_Components.find(familyId)->second;
        }
    }

    return 0;
}


void Entity::SetComponent(Component* newComponent)
{
    componentId_t gocId;
    newComponent->FamilyID(gocId);

    Component* temp = m_Components[gocId];
    // Discard of this old component.
    if(temp)
        delete temp;
    temp = 0;

    // Add the component, setting it's parent.
    newComponent->SetParent(*this);
    m_Components[gocId] = newComponent;
}


void Entity::ClearComponents()
{   // Iterate through all components and delete them.
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        delete it->second;
        it->second = 0;
    }
    // Now clear the m_Components map.
    m_Components.erase(m_Components.begin(), m_Components.end());
}


void Entity::Update(double time)
{
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        it->second->Update(time);
    }
}


void Entity::Draw(D3D& d3d)
{
    for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        it->second->Draw(d3d);
    }
}
