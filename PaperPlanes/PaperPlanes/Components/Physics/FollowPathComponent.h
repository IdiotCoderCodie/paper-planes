#pragma once
// My Includes
#include "../Component.h"
#include "../../Entities/Entity.h"

// STL
#include <vector>

class FollowPathComponent : public Component
{
public:
    FollowPathComponent(void);
    ~FollowPathComponent(void);

    void FamilyID(componentId_t& out) const { out = "PathComponent"; }
    void ComponentID(componentId_t& out) const { out = "BasicPathComponent"; }

    void Update(float time);

    /**
     * Add the Entity 'newNode' to the next position in the path.
     */
    void AddNode(Entity const* newNode);

    /**
     * Clears all nodes currently on the path.
     */
    void ClearNodes();

private:
    int GetNextNode();

private:
    std::vector<Entity const*> m_nodes;
    int                        m_nextNode;
    float                      m_speed;
};

